/*
 * Queue.cpp
 *
 *  Created on: 2015-3-18
 *      Author: liuzg
 */
#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"
/**
 * 初始化队列，队列的头部和尾部都为空（无元素，空队列)
 */
Status InitQueue(Queue* q) {
	q->front = NULL;
	q->rear = NULL;
}
/**
 * 销毁队列。释放队列的每一个元素，最后释放队列本身。从头部遍历
 */
Status DestroyQueue(Queue* q) {
	QueueNode* qn_tmp_ptr;
	while (q->front) {
		qn_tmp_ptr = q->front;
		q->front = q->front->next;
		free(qn_tmp_ptr);
	}
	free(q);
	return OK;
}
/**
 * 清除队列元素内容。从队列头部开始遍历
 */
Status ClearQueue(Queue* q) {
	QueueNode* qn_tmp_ptr;
	qn_tmp_ptr = q->front;
	while (qn_tmp_ptr) {
		qn_tmp_ptr->data = 0;
		qn_tmp_ptr = qn_tmp_ptr->next;
	}
	return OK;
}
/**
 * 检查队列是否为空
 */
Status QueueEmpty(Queue q) {
	if (!q.front)
		return TRUE;
	else
		return FALSE;
}
/**
 * 获取队列第一个节点的内容
 */
Status GetHead(Queue q, ElemType *e) {
	if(q.front==NULL) return FALSE;
	*e = q.front->data;
	return OK;
}
/**
 * 将节点压入队列，节点默认加入到队列尾部
 */
Status EnQueue(Queue* q, ElemType e) {
	QueueNode* qn_ptr = (QueueNode*) malloc(sizeof(QueueNode));
	qn_ptr->data = e;
	qn_ptr->next = NULL;
	if (!q->front) {//空对列时
		q->front = qn_ptr;
		q->rear = qn_ptr;
	} else { //处理最后一个节点
		q->rear->next = qn_ptr;
		q->rear = qn_ptr;
	}
	return OK;
}
/**
 *
 */
Status DeQueue(Queue* q, ElemType* e) {
	QueueNode* qn_tmp_ptr;
	QueueNode* queue_node;
	if(q->front==NULL){//空对列
		return FALSE;
	}
	if (q->front == q->rear) { //处理只有一个节点
		* e=q->front->data;
		free(q->front);
		q->front = NULL;
		q->rear = NULL;
		return TRUE;
	}
	//队列里两个以上节点，先取第一个节点
	queue_node = q->front;
	q->front=queue_node->next;
	queue_node->next=NULL;
	*e=queue_node->data;
	free(queue_node);
	return OK;
}
/**
 * 获取队列长度
 */
int QueueLength(Queue q) {
	int count = 0;
	if(q.front==NULL) return 0;
	QueueNode* queue_node=q.front;
	while(queue_node!=NULL){
		queue_node=queue_node->next;
		count++;
	}
	return count;
}
/**
 * 打印队列的所有节点
 */
void PrintQueue(Queue q) {
	QueueNode* queue_node=q.front;
	while (queue_node!=NULL) {
		printf("%d\n", queue_node->data);
		queue_node=queue_node->next;
	}
}
Status QueryQueue(Queue* q, ElemType e,QueueNode **node){
	QueueNode *find_node=q->front;
	while(find_node!=NULL && find_node->data!=e){
		find_node=find_node->next;
	}
	if(find_node!=NULL){
		**node=&find_node;
		return TRUE;
	}
	return FALSE;
}
int test()
{
    Queue q;
    ElemType e_tmp;
    InitQueue(&q);
    EnQueue(&q,1);
    EnQueue(&q,2);
    EnQueue(&q,3);
    DeQueue(&q,&e_tmp);
    PrintQueue(q);
    printf("the lenght of queue is %d\n", QueueLength(q));
}
