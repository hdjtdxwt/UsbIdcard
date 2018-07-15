/*
 * EnvQueue.h
 *
 *  Created on: 2015-3-19
 *      Author: liuzg
 */

#ifndef ENVQUEUE_H_
#define ENVQUEUE_H_

#include <jni.h>
#include<pthread.h>
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0

typedef int Status;

//定义队列节点的结构
typedef struct QueueNode {
	pthread_t tid;
	JNIEnv *env;
	struct QueueNode* next;
} QueueNode;

//定义队列的结构
typedef struct EnvQueue {
	QueueNode* front;
	QueueNode* rear;
} EnvQueue;

/*
 * 初始化一个空队列
 */
Status InitQueue(EnvQueue* q);
/*
 * 销毁队列
 */
Status DestroyQueue(EnvQueue* q);
/*
 * 清空队列
 */
Status ClearQueue(EnvQueue* q);
/*
 * 判断队列是否为空
 */
Status QueueEmpty(EnvQueue q);
/*
 * 入队列
 */
Status EnQueue(EnvQueue* q, pthread_t tid,JNIEnv *env);
/*
 * 出队列
 */
QueueNode *DeQueue(EnvQueue* q);
/*
 * 获取队列长度
 */
int QueueLength(EnvQueue q);
/*
 * 打印队列所有元素
 */
void PrintQueue(EnvQueue q);
/*
 *  查找元素
 */
QueueNode *QueryQueue(EnvQueue* q, pthread_t tid);
#endif /* ENVQUEUE_H_ */
