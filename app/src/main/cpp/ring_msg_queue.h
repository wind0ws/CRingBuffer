//
// Created by Threshold on 2019/5/7.
//

#ifndef XFAR_RING_MSG_QUEUE_H
#define XFAR_RING_MSG_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "ringbuffer.h"

#define MSG_OBJ_MAX_CAPACITY 1024
typedef char MSG_OBJ_DATA_TYPE;

typedef struct{
    int what;
    int arg1;
    int arg2;
    struct {
        MSG_OBJ_DATA_TYPE data[MSG_OBJ_MAX_CAPACITY];
        int data_len;
    }obj;
}QueueMsg;

/**
 * 创建RingMsgQueue
 * @param max_msg_capacity QueueMsg的最大数量
 * @return RingMsgQueue指针
 */
void *RingMsgQueue_create(__in uint32_t max_msg_capacity);

/**
 * push QueueMsg 到队尾。
 * 如果此时队列已满，则添加失败
 * @param ring_msg_queue_p RingMsgQueue指针
 * @param msg_p 要读取的QueueMsg对象指针
 * @return true添加成功，否则失败
 */
bool RingMsgQueue_push(__in void* ring_msg_queue_p,__in QueueMsg *msg_p);

/**
 * 从头部pop出QueueMsg。
 * @param ring_msg_queue_p RingMsgQueue指针
 * @param msg_p 要写入的QueueMsg的对象指针，QueueMsg内存由调用者分配(无需初始化)
 * @return true则pop成功，否则失败
 */
bool RingMsgQueue_pop(__in void* ring_msg_queue_p,__out QueueMsg *msg_p);

/**
 * 清除所有的QueueMsg
 * 注意调用此方法时是不能调用push/pop方法的，会有线程安全问题！！！
 * @param ring_msg_queue_p RingMsgQueue指针
 */
void RingMsgQueue_clear(__in void* ring_msg_queue_p);

/**
 * 获取当前队列中可以pop的QueueMsg数量
 * @param ring_msg_queue_p RingMsgQueue指针
 * @return 返回QueueMsg数量
 */
uint32_t RingMsgQueue_available_pop_msg_size(__in void *ring_msg_queue_p);

/**
 * 获取当前队列中可push的QueueMsg数量
 * @param ring_msg_queue_p RingMsgQueue指针
 * @return 返回QueueMsg数量
 */
uint32_t RingMsgQueue_available_push_msg_size(__in void *ring_msg_queue_p);

/**
 * 当前队列是否有可以pop的QueueMsg
 * @param ring_msg_queue_p RingMsgQueue指针
 * @return true则队列为空
 */
bool RingMsgQueue_is_empty(__in void* ring_msg_queue_p);

/**
 * 当前队列是否可以push QueueMsg
 * @param ring_msg_queue_p RingMsgQueue指针
 * @return true则队列满
 */
bool RingMsgQueue_is_full(__in void* ring_msg_queue_p);

/**
 * 释放内存，销毁RingMsgQueue
 * 调用此方法前应该停止push/pop
 * @param ring_msg_queue_p RingMsgQueue指针
 */
void RingMsgQueue_destroy(__in void* ring_msg_queue_p);

#ifdef __cplusplus
}
#endif

#endif //XFAR_RING_MSG_QUEUE_H
