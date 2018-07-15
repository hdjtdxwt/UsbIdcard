/*
 * Queue.h
 *
 *  Created on: 2015-3-18
 *      Author: liuzg
 */

#ifndef QUEUE_H_
#define QUEUE_H_

/**
 * 队列（C语言实现，基于链式结构）
 * 指定数据类型为整型
 * 不采用头结点
 */
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0

typedef int Status;
typedef int ElemType;

//定义队列节点的结构
typedef struct QueueNode {
	ElemType data;
	struct QueueNode* next;
} QueueNode;
//定义队列的结构
typedef struct Queue {
	QueueNode* front;
	QueueNode* rear;
} Queue;

/*
 * 初始化一个空队列
 */
Status InitQueue(Queue* q);
/*
 * 销毁队列
 */
Status DestroyQueue(Queue* q);
/*
 * 清空队列
 */
Status ClearQueue(Queue* q);
/*
 * 判断队列是否为空
 */
Status QueueEmpty(Queue q);
/*
 * 获取队列第一个元素的值
 */
Status GetHead(Queue q, ElemType *e);
/*
 * 入队列
 */
Status EnQueue(Queue* q, ElemType e);
/*
 * 出队列
 */
Status DeQueue(Queue* q, ElemType* e);
/*
 * 获取队列长度
 */
int QueueLength(Queue q);
/*
 * 打印队列所有元素
 */
void PrintQueue(Queue q);
/*
 *  查找元素
 */
Status QueryQueue(Queue* q, ElemType e,QueueNode *node);
#endif /* QUEUE_H_ */
