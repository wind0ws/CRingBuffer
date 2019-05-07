//
// Created by Threshold on 2019/5/7.
//
#include "ring_msg_queue.h"
#include "mlog.h"

static const size_t g_one_queue_msg_byte_size = sizeof(QueueMsg);

void *RingMsgQueue_create(__in uint32_t max_msg_capacity){
    LOGD("create ring msg queue. one_queue_msg_byte_size=%d, maxQueueMsgSize=%d",
         g_one_queue_msg_byte_size, max_msg_capacity);
    return RingBuffer_create(g_one_queue_msg_byte_size * max_msg_capacity);
}

bool RingMsgQueue_push(__in void* ring_msg_queue_p,__in QueueMsg *msg_p){
    if (RingMsgQueue_is_full(ring_msg_queue_p)){
        return false;
    }
    return RingBuffer_write(ring_msg_queue_p,msg_p,g_one_queue_msg_byte_size) == g_one_queue_msg_byte_size;
}

bool RingMsgQueue_pop(__in void* ring_msg_queue_p,__out QueueMsg *msg_p){
    if (RingMsgQueue_is_empty(ring_msg_queue_p)){
        return false;
    }
    return RingBuffer_read(ring_msg_queue_p,msg_p,g_one_queue_msg_byte_size) == g_one_queue_msg_byte_size;
}

inline void RingMsgQueue_clear(__in void* ring_msg_queue_p){
    RingBuffer_clear(ring_msg_queue_p);
}

inline uint32_t RingMsgQueue_available_pop_msg_size(__in void *ring_msg_queue_p){
    return RingBuffer_available_data(ring_msg_queue_p) / g_one_queue_msg_byte_size;
}

inline uint32_t RingMsgQueue_available_push_msg_size(__in void *ring_msg_queue_p){
    return RingBuffer_available_space(ring_msg_queue_p) / g_one_queue_msg_byte_size;
}

inline bool RingMsgQueue_is_empty(__in void* ring_msg_queue_p){
    return RingMsgQueue_available_pop_msg_size(ring_msg_queue_p) == 0;
}

inline bool RingMsgQueue_is_full(__in void* ring_msg_queue_p){
    return RingMsgQueue_available_push_msg_size(ring_msg_queue_p) == 0;
}

void RingMsgQueue_destroy(__in void* ringMsgQueueHandle){
    RingBuffer_destroy(ringMsgQueueHandle);
}
