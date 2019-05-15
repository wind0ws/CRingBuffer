//
// Created by Threshold on 2018/11/7.
//
#ifndef CRINGBUFFER_SIMPLE_QUEUE_H
#define CRINGBUFFER_SIMPLE_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#define SIMPLE_QUEUE_FIFO_SIZE 4096
#define SIMPLE_QUEUE_SUCCESS 1
#define SIMPLE_QUEUE_FAIL 0

typedef char SimpleQueueDataType;

typedef struct __simple_queue_handle *simple_queue_handle;

/**
 * init queue
 * @return queue_handle
 */
simple_queue_handle simple_queue_init(void);

/**
 * push one data to queue
 * @param handle queue_handle
 * @param data to push data
 * @return SIMPLE_QUEUE_SUCCESS for success, SIMPLE_QUEUE_FAIL for failed.
 */
int simple_queue_push(simple_queue_handle handle, SimpleQueueDataType data);

/**
 * pop one data from queue
 * @param handle queue_handle
 * @param data to push data
 * @return SIMPLE_QUEUE_SUCCESS for success, SIMPLE_QUEUE_FAIL for failed.
 */
int simple_queue_pop(simple_queue_handle handle, SimpleQueueDataType *dataPtr);

/**
 * available space for queue
 * @param handle queue_handle
 * @return space size
 */
int simple_queue_available_space(simple_queue_handle handle);

/**
 * available data count for queue
 * @param handle queue_handle
 * @return data count
 */
int simple_queue_available_data(simple_queue_handle handle);

/**
 * if queue is empty(no data to pop)
 * @param handle queue_handle
 * @return true indicate queue is empty.
 */
bool simple_queue_is_empty(simple_queue_handle handle);

/**
 * if queue is full(can't push data to queue)
 * @param handle queue_handle
 * @return true indicate queue is full.
 */
bool simple_queue_is_full(simple_queue_handle handle);

/**
 * destroy queue.
 * @param handle_p pointer for queue_handle
 * @return QUEUE_SUCCESS for success
 */
int simple_queue_destroy(simple_queue_handle *handle_p);

#ifdef __cplusplus
}
#endif

#endif //CRINGBUFFER_SIMPLE_QUEUE_H
