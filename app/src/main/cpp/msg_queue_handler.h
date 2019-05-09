//
// Created by Administrator on 2019/5/7.
//

#ifndef CRINGBUFFER_MSG_QUEUE_HANDLER_H
#define CRINGBUFFER_MSG_QUEUE_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ring_msg_queue.h"

typedef struct __queue_handler *queue_handler;

typedef void (*msg_handler_callback)(queue_msg_t *msg_p);

queue_handler QueueHandler_create(__in uint32_t max_msg_capacity, __in msg_handler_callback callback);

bool QueueHandler_send(__in queue_handler handler_p,__in queue_msg_t *msg_p);

uint32_t QueueHandler_available_send_msg_amount(__in queue_handler handler_p);

uint32_t QueueHandler_current_queue_msg_amount(__in queue_handler handler_p);

bool QueueHandler_is_empty(__in queue_handler handler_p);

bool QueueHandler_is_full(__in queue_handler handler_p);

void QueueHandler_destroy(__in queue_handler handler_p);

#ifdef __cplusplus
}
#endif

#endif //CRINGBUFFER_MSG_QUEUE_HANDLER_H
