//
// Created by Threshold on 2019/5/7.
//
#include <malloc.h>
#include "ring_msg_queue.h"
#include "mlog.h"

struct __ring_msg_queue {
    ring_buf ring_buf_p;
};

static const size_t g_one_queue_msg_byte_size = sizeof(queue_msg_t);

ring_msg_queue RingMsgQueue_create(__in uint32_t max_msg_capacity) {
    LOGD("create ring msg queue. one_queue_msg_byte_size=%d, maxQueueMsgSize=%d",
         g_one_queue_msg_byte_size, max_msg_capacity);
    ring_msg_queue msg_queue_p = calloc(1, sizeof(struct __ring_msg_queue));
    msg_queue_p->ring_buf_p = RingBuffer_create(g_one_queue_msg_byte_size * max_msg_capacity);
    return msg_queue_p;
}

bool RingMsgQueue_push(__in ring_msg_queue ring_msg_queue_p, __in queue_msg_t *msg_p) {
    if (RingMsgQueue_is_full(ring_msg_queue_p)) {
        return false;
    }
    return RingBuffer_write(ring_msg_queue_p->ring_buf_p, msg_p, g_one_queue_msg_byte_size) ==
           g_one_queue_msg_byte_size;
}

bool RingMsgQueue_pop(__in ring_msg_queue ring_msg_queue_p, __out queue_msg_t *msg_p) {
    if (RingMsgQueue_is_empty(ring_msg_queue_p)) {
        return false;
    }
    return RingBuffer_read(ring_msg_queue_p->ring_buf_p, msg_p, g_one_queue_msg_byte_size) ==
           g_one_queue_msg_byte_size;
}

inline void RingMsgQueue_clear(__in ring_msg_queue ring_msg_queue_p) {
    RingBuffer_clear(ring_msg_queue_p->ring_buf_p);
}

inline uint32_t RingMsgQueue_available_pop_msg_amount(__in ring_msg_queue ring_msg_queue_p) {
    return RingBuffer_available_data(ring_msg_queue_p->ring_buf_p) / g_one_queue_msg_byte_size;
}

inline uint32_t RingMsgQueue_available_push_msg_amount(__in ring_msg_queue ring_msg_queue_p) {
    return RingBuffer_available_space(ring_msg_queue_p->ring_buf_p) / g_one_queue_msg_byte_size;
}

inline bool RingMsgQueue_is_empty(__in ring_msg_queue ring_msg_queue_p) {
    return RingMsgQueue_available_pop_msg_amount(ring_msg_queue_p) == 0;
}

inline bool RingMsgQueue_is_full(__in ring_msg_queue ring_msg_queue_p) {
    return RingMsgQueue_available_push_msg_amount(ring_msg_queue_p) == 0;
}

void RingMsgQueue_destroy(__in ring_msg_queue ring_msg_queue_p) {
    RingBuffer_destroy(ring_msg_queue_p->ring_buf_p);
    free(ring_msg_queue_p);
}
