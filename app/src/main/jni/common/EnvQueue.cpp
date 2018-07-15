/*
 * EnvQueue.cpp
 *
 *  Created on: 2015-3-19
 *      Author: liuzg
 */
#include <stdio.h>
#include "EnvQueue.h"
/*
 * 初始化一个空队列
 */
Status InitQueue(EnvQueue* q) {
	q->front = NULL;
	q->rear = NULL;
}
/*
 * 销毁队列
 */
Status DestroyQueue(EnvQueue* q){
	QueueNode* qn_tmp_ptr;
	while (q->front) {
		qn_tmp_ptr = q->front;
		q->front = q->front->next;
		qn_tmp_ptr->env=NULL;
		free(qn_tmp_ptr);
	}
	free(q);
	return OK;
}
/*
 * 清空队列
 */
Status ClearQueue(EnvQueue* q){
	QueueNode* qn_tmp_ptr;
	qn_tmp_ptr = q->front;
	while (qn_tmp_ptr) {
		qn_tmp_ptr->tid = 0;
		qn_tmp_ptr->env=NULL;
		qn_tmp_ptr = qn_tmp_ptr->next;
	}
	return OK;
}
/*
 * 判断队列是否为空
 */
Status QueueEmpty(EnvQueue q){
	if (!q.front)
		return TRUE;
	else
		return FALSE;
}
/*
 * 入队列
 */
Status EnQueue(EnvQueue* q, pthread_t tid,JNIEnv *env){
	QueueNode* qn_ptr = (QueueNode*) malloc(sizeof(QueueNode));
	qn_ptr->tid=tid;
	qn_ptr->env=env;
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
/*
 * 出队列
 */
QueueNode* DeQueue(EnvQueue* q){
	QueueNode* queue_node;
	if(q->front==NULL){//空对列
		return NULL;
	}
	if (q->front == q->rear) { //处理只有一个节点
		queue_node=q->front;
		q->front = NULL;
		q->rear = NULL;
		queue_node->next=NULL;
		return queue_node;
	}
	//队列里两个以上节点，先取第一个节点
	queue_node = q->front;
	q->front=queue_node->next;
	queue_node->next=NULL;
	return queue_node;
}
/*
 * 获取队列长度
 */
int QueueLength(EnvQueue q){
	int count = 0;
	if(q.front==NULL) return 0;
	QueueNode* queue_node=q.front;
	while(queue_node!=NULL){
		queue_node=queue_node->next;
		count++;
	}
	return count;
}
/*
 * 打印队列所有元素
 */
void PrintQueue(EnvQueue q){
	QueueNode* queue_node=q.front;
	while (queue_node!=NULL) {
		printf("%d\n", queue_node->tid);
		queue_node=queue_node->next;
	}
}
/*
 *  查找元素
 */
QueueNode *QueryQueue(EnvQueue* q, pthread_t tid){
	QueueNode *find_node=q->front;
	while(find_node!=NULL && find_node->tid!=tid){
		find_node=find_node->next;
	}
	return find_node;
}
