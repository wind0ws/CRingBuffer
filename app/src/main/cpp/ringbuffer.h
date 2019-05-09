//
// Created by Threshold on 2018/12/4.
//
#ifndef XFAR_RINGBUFFER_H
#define XFAR_RINGBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#ifndef __in
#define __in
#endif
#ifndef __out
#define __out
#endif
#ifndef __in_opt
#define __in_opt
#endif
#ifndef __out_opt
#define __out_opt
#endif

typedef struct __ring_buffer_t *ring_buf;

/**
 * 创建RingBuffer
 * @param size RingBuffer大小，必须是2的n次方
 * @return RingBuffer对象指针
 */
ring_buf RingBuffer_create(uint32_t size);

/**
 * 释放内存，销毁RingBuffer对象
 * @param ring_buf_p RingBuffer对象指针
 */
void RingBuffer_destroy(__in ring_buf ring_buf_p);

/**
 * 从RingBuffer中读取指定size的数据放到target中。
 * 如果当前RingBuffer中可读取数据个数availableDataLen小于size，则只读取availableDataLen个数据
 * @param ring_buf_p RingBuffer结构体指针
 * @param target 要写数据的对象指针
 * @param size 要读取数据大小
 * @return 真正读取到的数据size
 */
uint32_t RingBuffer_read(__in ring_buf ring_buf_p, __out void *target, uint32_t size);

/**
 * 从source中读取指定size的数据写入到RingBuffer队列中。
 * 如果当前RingBuffer可写空间availableSpace小于size，则只写入当前可用空间大小availableSpace个数据
 * @param ring_buf_p RingBuffer结构体指针
 * @param source 要读取数据的指针
 * @param size 从source中读取的大小
 * @return 真正写入的数据size
 */
uint32_t RingBuffer_write(__in ring_buf ring_buf_p, __in const void *source, uint32_t size);

/**
 * RingBuffer当前是否有数据
 * @param ring_buf_p RingBuffer指针
 * @return true则没有数据
 */
bool RingBuffer_empty(__in const ring_buf ring_buf_p);

/**
 * RingBuffer当前是否满
 * @param ring_buf_p RingBuffer指针
 * @return true则队列已满
 */
bool RingBuffer_full(__in const ring_buf ring_buf_p);

/**
 * 清除RingBuffer数据
 * 注意一定要等没有线程在read/write的时候才能clear,否则会有线程安全问题!!!
 * @param ring_buffer_p RingBuffer指针
 */
void RingBuffer_clear(__in ring_buf ring_buffer_p);

/**
 * 当前RingBuffer队列可读取数据大小
 * @param ring_buf_p RingBuffer指针
 * @return 可读取数据byte size
 */
uint32_t RingBuffer_available_data(__in const ring_buf ring_buf_p);

/**
 * 当前RingBuffer队列可用空间
 * @param ring_buf_p RingBuffer指针
 * @return 可用空间byte size
 */
uint32_t RingBuffer_available_space(__in const ring_buf ring_buf_p);


#ifdef __cplusplus
}
#endif

#endif //XFAR_RINGBUFFER_H
