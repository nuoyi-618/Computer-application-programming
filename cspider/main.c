#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <time.h>
#include "cspider.h"
unsigned int mem = 0;
void* bupt_malloc(size_t size) {
	if (size <= 0) {
		return NULL;
	}
	mem += (int)size;
	return (void*)malloc(size);
}
int DFA(char* source_code, Queue* q, BF* bf, int id)//提取url
{

	const char* pre_url = "/news.sohu.com";
	int pre_url_size = sizeof(pre_url);
	char* new_url;
	int flag = 0;  // 判断链接最后是否为换行符
	int length;    // 链接长度
	char urlname[1024];  // 匹配成功的链接
	char* start = 0;
	char* end = 0;
	int state = 0;
	while (*source_code != '\0')
	{
		switch (state)
		{
		case 0:
			switch (*source_code)
			{
			case '<':
				state = 1;
				source_code++;
				break;
			default:
				state = 0;
				source_code++;
				break;
			}
			break;

		case 1:
			switch (*source_code)
			{
			case 'a':
				state = 2;
				source_code++;
				break;
			default:
				state = 0;
				source_code++;
				break;
			}
			break;

		case 2:
			switch (*source_code)
			{
			case 'h':
				state = 3;
				source_code++;
				break;
			case '>':
				state = 0;
				source_code++;
				break;
			default:
				state = 2;
				source_code++;
				break;
			}
			break;

		case 3:
			switch (*source_code)
			{
			case 'r':
				state = 4;
				source_code++;
				break;
			case '>':
				state = 0;
				source_code++;
				break;
			case 'h':               
				state = 3;
				source_code++;
				break;
			default:
				state = 2;
				source_code++;
				break;
			}
			break;

		case 4:
			switch (*source_code)
			{
			case 'e':
				state = 5;
				source_code++;
				break;
			case '>':
				state = 0;
				source_code++;
				break;
			default:
				state = 2;
				source_code++;
				break;
			}
			break;

		case 5:
			switch (*source_code)
			{
			case 'f':
				state = 6;
				source_code++;
				break;
			case '>':
				state = 0;
				source_code++;
				break;
			default:
				state = 2;
				source_code++;
				break;
			}
			break;

		case 6:
			switch (*source_code)
			{
			case '=':
				state = 7;
				source_code++;
				break;
			case '>':
				state = 0;
				source_code++;
				break;
			default:
				state = 2;
				source_code++;
				break;
			}
			break;

		case 7:
			switch (*source_code)
			{
			case '"':
				state = 8;
				source_code++;
				break;
			case '\'':   // 有的网址是单引号
				state = 8;
				source_code++;
				break;
			case '>':
				state = 0;
				source_code++;
				break;
			default:
				state = 2;
				source_code++;
				break;
			}
			break;

		case 8:
			switch (*source_code)
			{
			case '/':

				source_code += 2;

				char temp[20];
				memcpy(temp, source_code, 13);       // 匹配 news.sohu.com
				*(temp + 13) = '\0';
				if (0 == strcmp(temp, "news.sohu.com"))
				{
					state = 9;
					source_code = source_code + 13;   // 跳过 news.sohu.com
					start = source_code;

				}
				else
					state = 0;

				break;
			case '"':
				state = 0;
				source_code++;
				break;
			case '>':
				state = 0;
				source_code++;
				break;
			case '#':
				state = 0;
				source_code++;
				break;
			default:
				state = 8;
				source_code++;
				break;
			}
			break;

		case 9:
			switch (*source_code)
			{
			case '\n':     // 如果"前有换行符则需要去掉
				state = 9;
				flag = 1;
				source_code++;
				break;
			case '"':
				state = 10;
				end = source_code;
				if (flag)
				{
					length = (int)(end - start - 1);
					flag = 0;
				}
				else
				{
					length = (int)(end - start);
				}
				memcpy(urlname, start, length);   // urlname是匹配成功的链接名称
				urlname[length] = 0;
				if ((new_url = malloc(pre_url_size + sizeof(urlname) + 1)) == NULL)
					return -1;
				memset(new_url, 0, pre_url_size + sizeof(urlname) + 1);
				strcpy(new_url, pre_url);
				strcat(new_url, urlname);
				// printf("--getted:[%s]\n", new_url);
				fprintf(url_cp, "%d %s\n", id, new_url);   // 写入 url_cp.txt 文件
				if (check_bf_bits(bf, new_url) == 0)  // 
				{
					queue_add(q, new_url);
					// printf("--added\n");
					set_bf_bits(bf, new_url);
					// printf("--setted\n");
				}
				else
				{
					//重复则释放空间
					free(new_url);
					state = 3;
				}
				source_code++;
				break;
			case '\'':
				state = 10;
				end = source_code;
				if (flag)
				{
					length = (int)(end - start - 1);
					flag = 0;
				}
				else
				{
					length = (int)(end - start);
				}
				memcpy(urlname, start, length);   // urlname是匹配成功的链接名称
				urlname[length] = 0;
				if ((new_url = malloc(pre_url_size + sizeof(urlname) + 1)) == NULL)
					return -1;
				memset(new_url, 0, pre_url_size + sizeof(urlname) + 1);
				strcpy(new_url, pre_url);
				strcat(new_url, urlname);
				fprintf(url_cp, "%d %s\n", id, new_url);  
				if (check_bf_bits(bf, new_url) == 0)  
				{
					queue_add(q, new_url);
					set_bf_bits(bf, new_url);
				}
				else
				{
					//重复则释放空间
					free(new_url);
					state = 3;
				}
				source_code++;
				break;
			case '>':
				state = 0;
				source_code++;
				break;
			default:
				state = 9;
				source_code++;
				break;
			}
			break;
		case 10:
			switch (*source_code)
			{
			case 'h':              // 处理一个a标签中有多个链接的情况
				state = 3;
				source_code++;
				break;
			case '>':
				state = 0;
				source_code++;
				break;
			default:
				state = 10;
				source_code++;
				break;
			}
			break;
		default:
			break;
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{

	char *serverip = argv[1];
	int port = 80;
	char *file_name = argv[2];

	FILE* url_list = fopen("./url_list.txt", "w");
	FILE* url_cp = fopen("./url_cp.txt", "w");
	FILE* debug_file = fopen("./debug.txt", "w");

	if (url_list == NULL || url_cp == NULL || debug_file == NULL)
	{
		printf("Unable to open this file!\n");
		exit(0);
	}

	Queue *queue_url = create_queue();
	// 将首页地址入队
	char* index_url = "/";
	queue_add(queue_url, index_url);
	// 构建布隆过滤器
	bf_params();
	BF *bf = bupt_malloc(sizeof(BF));
	bf->bits = bupt_malloc(BF_SIZE);
	init_bf(bf, BF_SIZE);
	set_bf_bits(bf, index_url);

	time(&start_time);
	int spider = 1;
	if (spider == 1) {
		fprintf(debug, "[INFO] 开始爬虫:\n");
		handleURLs(queue_url, bf, serverip, port);
		free_bf(bf);
		queue_destroy(queue_url);
		fflush(url_list);
		fclose(url_list);
		fflush(url_cp);
		fclose(url_cp);
		fprintf(debug, "[INFO] 爬虫完毕!\n");
		fflush(debug);
	}
	
	time(&cur_time);
	duration = (int)difftime(cur_time, start_time);
	fprintf(debug, "\n[NOTICE] 内存使用：%.2fKB  耗时：%dmin%ds\n", ((double)mem / (double)1024), duration / 60, duration % 60);
	fflush(debug);

	// 对应关系
	TREE *tree = tr_alloc();
	char buf_url[100];
	char temp_source_id[10], temp_dest_url[100];
	int source_id, dest_id, urlLen;
	int num = 0;
	int p, q, j;
	int buf[500] = { 0 };
	int camp = 1;
	int buf_num = 0, flagrep;
	url_list = fopen("./url_list.txt", "r");
	url_cp = fopen("./url_cp.txt", "r");
	url = fopen(file_name, "w");
		// 爬虫过程中，生成URLs
	if (spider == 1) {
		for (int u = 0; u < urls_num; u++)
		{
			add_url(tree, URLs[u], strlen(URLs[u]), u);
			fprintf(url, "%s %d\n", URLs[u], u);
			free(URLs[u]);
		}
	}
	else {// 爬虫完成，从文件读取URL
		for (fgets(buf_url, 100, url_list); !feof(url_list); fgets(buf_url, 100, url_list)) {
			//提取该行的源id和目的url
			memset(temp_source_id, 0, 10);
			memset(temp_dest_url, 0, 100);
			for (p = 0; buf_url[p] != ' '; ++p);
			strncpy(temp_dest_url, buf_url, p);
			temp_dest_url[p] = '\0';
			urlLen = strlen(temp_dest_url);
			for (q = p; buf_url[q] != '\n'; ++q);
			strncpy(temp_source_id, buf_url + p + 1, q - p - 1);
			source_id = atoi(temp_source_id);

			add_url(tree, temp_dest_url, urlLen, source_id);
			fprintf(url, "%s %d\n", temp_dest_url, source_id);
		}
	}
	fprintf(url, "\n");
	time(&cur_time);
	duration = (int)difftime(cur_time, start_time);
	fflush(debug);

	for(fgets(buf_url, 100, url_cp); !feof(url_cp); fgets(buf_url, 100, url_cp)) {
        //提取该行的源id和目的url
        memset(temp_source_id, 0, 10);
        memset(temp_dest_url, 0, 100);
        for (p = 0; buf_url[p] != ' ' ; ++p);
        strncpy(temp_source_id, buf_url, p);
        source_id = atoi(temp_source_id);
        for (q = p; buf_url[q] != '\n' ; ++q);
        if((q-p) == 1)
        {
			printf("ERROR!\n");
            continue;
        }
        strncpy(temp_dest_url, buf_url+p+1, q-p-1);
        temp_dest_url[q - p] = '\0';
        urlLen = strlen(temp_dest_url);

        dest_id = search(tree, urlLen, temp_dest_url);

		if(dest_id >= 0)
        {
            // 去掉重复的source_id->dest_id对
            flagrep = 0;    
            if(camp == source_id)
            {
                for(j = 0; j < buf_num; j++)
                {
                    if(buf[j] != dest_id)
                        flagrep = 0;
                    else
                    {
                        flagrep = 1;
                        break;
                    }
                }
                if(flagrep == 0)
                {
					buf[buf_num] = dest_id;
                    fprintf(url, "%d -> %d\n",  source_id, dest_id);
                    num++;
                    buf_num++;
                }
                else
                    continue;
            }
            else
            {
                buf[0] = dest_id;
                buf_num = 1;
                camp = source_id;
				fprintf(url, "%d -> %d\n",  source_id, dest_id);
            }
		}
    }
	// 统计信息
	time(&cur_time);
	duration = (int)difftime(cur_time, start_time);
	fprintf(url, "\n%u %d", mem/1024, duration);
    fclose(url_list);
	fclose(url_cp);
	fflush(url);
    fclose(url);

	// 释放空间
	free_tree(tree);
	fclose(debug);

	return 0;
}
