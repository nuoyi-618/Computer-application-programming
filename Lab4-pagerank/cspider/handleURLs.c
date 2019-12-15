#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "cspider.h"

#define buf 1024
#define EVENTSNUM 50
#define SOCKETNUM 200
#define DELAYTIME 1000
int persistence_html_switch = 0; // 是否持久化爬取的html文件

int handleURLs(Queue *queue_url, BF *bf, char* serverip, int port)
{
	int begin_url = 1;
    int len_queue, i;
    int flag = 0;  // 退出循环标志
    int para_num = 1024;
    char* para_url[para_num];
    struct epoll_event ev, events[EVENTSNUM];	//ev用于注册事件，events数组用于回传要处理的事件
    char text[buf];
    int epfd, loop_count = 0;
	FILE* html_file = NULL;
    epfd = epoll_create(para_num);
    while (queue_is_empty(queue_url)==0)
    {
		time(&cur_time);
		duration = (int)difftime(cur_time, start_time);
		printf("\n[INFO] ---------请求波数：%d[%dmin%ds]----------\n", ++loop_count, duration / 60, duration % 60);
		fprintf(debug, "[INFO] ---------请求波数：%d[%dmin%ds]----------\n", loop_count, duration / 60, duration % 60);
		len_queue = queue_length(queue_url);
		//最多同时放出去SOCKETNUM条socket连接
		if (len_queue > SOCKETNUM) {
			len_queue = SOCKETNUM;
		}
        flag = len_queue;
		// 分别为此次的请求个数，队列等待请求个数，不重复的url个数
		printf("[INFO] --*** flag [%d/%d/%d]\n", flag, queue_length(queue_url), urls_num);
		fprintf(debug, "[INFO] --*** flag [%d/%d/%d]\n", flag, queue_length(queue_url), urls_num);
		// 发出 flag 个请求
		fprintf(debug, "[INFO] --*** 开始发送请求\n");
        for (i = 0; i < len_queue; ++i)
        {
            para_url[i] = queue_pop(queue_url);
			// printf("--serverip：[%s]\n", serverip);
			// printf("--url：[%s]\n", para_url[i]);
            int sockfd;
            buildConnect(&sockfd, serverip, port);
            setnoblocking(sockfd);
            sendRequest(para_url[i], sockfd, serverip);

            Ev_arg *arg = (Ev_arg *)calloc(sizeof(Ev_arg), 1);
            arg->urlName = para_url[i];
            arg->fd = sockfd;
            ev.data.ptr = arg;
            ev.events = EPOLLIN | EPOLLET;	//设置要处理的事件类型。可读，边缘触发
            epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);	//注册ev

        }
		fprintf(debug, "[INFO] --*** 发送请求完成\n");

		// 等待循环处理 flag 个请求
		fprintf(debug, "[INFO] --*** 开始等待处理响应\n");
        while (flag > 0)
        {
            int events_num = 1;
			fprintf(debug, "[INFO] --*** epoll_wait\n");
            events_num = epoll_wait(epfd, events, EVENTSNUM, DELAYTIME);	//等待sockfd可读，即等待http response
			fprintf(debug, "[INFO] --*** epoll_wait over with %d events\n", events_num);
            for (i = 0; i < events_num; ++i, --flag)
            {
                if (events[i].events & EPOLLIN)     //接收到数据，读socket
                {
                    Ev_arg *arg = (Ev_arg *) (events[i].data.ptr);
                    memset(source_code, 0, BUF);
                    while(read(arg->fd, text, buf))  //读
                    {
                        strcat(source_code, text);
                        memset(text, '\0', buf);
                    }
                    // 输出状态码
                    char *s = source_code;
                    s += 9;
                    char d[3];
                    memcpy(d, s, 3);
                    int code = atoi(d);
                    if(code == 200)
                    {
                        URLs[urls_num] = arg->urlName;
						fprintf(url_list, "%s %d\n", arg->urlName, urls_num);   // 写入 url.txt 文件
						// 将 html 文件保存
						if (persistence_html_switch == 1) {
							if (begin_url) {
								begin_url = 0;
								html_file = fopen("index.html", "w");
								if (html_file) {
									fprintf(html_file, "%s", source_code);
									fflush(html_file);
									fclose(html_file);
								}
								else {
									fprintf(debug, "[ERROR] --创建文件失败：[%s]\n", arg->urlName);
								}
							}
							else {
								mkdirs(arg->urlName + 1);
								html_file = fopen(arg->urlName + 1, "w");
								if (html_file) {
									fprintf(html_file, "%s", source_code);
									fflush(html_file);
									fclose(html_file);
								}
								else {
									fprintf(debug, "[ERROR] --创建文件失败：[%s]\n", arg->urlName);
								}
							}
						}
                        // DFA函数将解析得到的网页，并通过自动机识别url，然后加入队列中；同时通过bloomfilter进行网站过滤
                        DFA(source_code, queue_url, bf, urls_num);
						// 从0开始编号并存入数组
						urls_num++;
                    }
                    else
                    {
						fprintf(debug, "[ERROR] --** Resp Code Err: [%d][%s]\n", code, arg->urlName);
                        free(arg);
                    }
                    close(arg->fd);
                    continue;
                }
                else
                {
                    printf("[ERROR] Closed connection on descriptor %d\n", events[i].data.fd);
					fprintf(debug, "[ERROR] Closed connection on descriptor %d\n", events[i].data.fd);
                    close(events[i].data.fd);
                }
            }
        }
		fprintf(debug, "[INFO] --*** 等待处理响应完成\n");
    }
    close(epfd);
    return 0;
}


// 建立连接
int buildConnect(int *sockfd, char* serverip, int port)
{
    if ( -1 == (*sockfd = socket(AF_INET, SOCK_STREAM, 0)) )   // 穿件套接字  | SOCK_NONBLOCK 异步
    {
        printf("[ERROR] sockfd open error!\n");
		fprintf(debug, "[ERROR] sockfd open error!\n");
        return -1;
    }

    struct sockaddr_in addr;
	int sock_len = sizeof(struct sockaddr);
	bzero(&addr, sock_len);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(serverip); // 按照网络字节顺序存储IP地址
	addr.sin_port = htons(port);

	if (-1 == connect(*sockfd, (struct sockaddr *)(&addr), sock_len))
    {
        printf("[ERROR] connect fail! try again...\n");
		fprintf(debug, "[ERROR] connect fail! try again...\n");
        close(*sockfd);
        return -1;
    }
    return 0;
}

// 设置文件描述符为非阻塞模式
int setnoblocking(int sockfd)
{
	int opts;
	opts = fcntl(sockfd, F_GETFL, 0);	//获取文件标志和访问模式
	opts |= O_NONBLOCK;	//非阻塞
	fcntl(sockfd, F_SETFL, opts);
}


// 发送http request
int sendRequest(char* url, int sockfd, char* serverip)
{
    char myurl[BUFSIZ];
    char *pHost = 0, *pGET = 0;
	char host[BUFSIZ], GET[BUFSIZ];
	char header[BUFSIZ] = "";

	strcpy(myurl, serverip);
	strcat(myurl, url);
	for (pHost = myurl; *pHost != '/' && *pHost != '\0'; ++pHost);
	if ((int)(pHost - myurl) == strlen(myurl))
		strcpy(GET, "/");
	else
		strcpy(GET, pHost);
	*pHost = '\0';
	strcpy(host, myurl);

	strcat(header, "GET ");
	strcat(header, GET);
	strcat(header, " HTTP/1.1\r\n");
	strcat(header, "HOST: ");
	strcat(header, host);
	strcat(header, "\r\nConnection: Close\r\n\r\n");
	write(sockfd, header, strlen(header));
	return 0;
}

void mkdirs(char *muldir)
{
	int i, len;
	char str[512];
	strncpy(str, muldir, 512);
	len = strlen(str);
	for (i = 0; i < len; i++){
		if (str[i] == '/'){
			str[i] = '\0';
			if (access(str, 0) != 0){
				mkdir(str, 0777);
			}
			str[i] = '/';
		}
	}

	return;
}