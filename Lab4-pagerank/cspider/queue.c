#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cspider.h"

Queue* create_queue() {
	Queue* queue = (Queue*)bupt_malloc(sizeof(Queue));
	queue->ql = NULL;
	queue->tail = NULL;
	queue->size = 0;
	return queue;
}

QueueNode* create_queue_node(char* url) {
	QueueNode* list = (QueueNode*)bupt_malloc(sizeof(QueueNode));
	list->url = url;
	list->next = NULL;
	return list;
}

void queue_add(Queue* q, char* url) {
	QueueNode* cur_list = q->tail;
	if (cur_list == NULL) {
		q->tail = create_queue_node(url);
		q->ql = q->tail;
	}
	else {
		cur_list->next = create_queue_node(url);
		q->tail = q->tail->next;
	}
	q->size += 1;
	return;
}

char* queue_pop(Queue* q) {
	if (q->ql == NULL) {
		return NULL;
	}
	QueueNode* l = q->ql;
	q->ql = l->next;
	char* url = l->url;
	free(l);
	q->size -= 1;
	if (q->size <= 0) {
		q->size = 0;
		q->ql = NULL;
		q->tail = NULL;
	}
	return url;
}

int queue_is_empty(Queue* q) {
	return q->size <= 0 ? 1 : 0;
}

int queue_length(Queue* q) {
	return q->size;
}

void queue_destroy(Queue* q) {
	QueueNode* list = q->ql, *temp_list = NULL;
	while (list != NULL) {
		temp_list = list;
		list = list->next;
		free(temp_list->url);
		free(temp_list);
	}
	free(q);
	return;
}
