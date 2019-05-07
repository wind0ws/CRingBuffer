//
// RingBuffer实现
// Created by Threshold on 2018/12/4.
//
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ringbuffer.h"
#include "mlog.h"

/* 取a和b中最小值 */
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/*
** the bitwise version :
** we apply n - 1 mask to n, and then check that is equal to 0
** it will be true for all numbers that are power of 2.
** Lastly we make sure that n is superior to 0.
*/
static inline int is_power_of_2(unsigned int n) {
    return (n > 0 && !(n & (n - 1)));
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
static inline uint32_t roundup_pow_of_two(uint32_t x) {
    if (x == 0 || is_power_of_2(x)) {
        return x;
    }
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
}
#pragma clang diagnostic pop

typedef struct {
    uint32_t in; /*写指针 buffer end*/
    uint32_t out; /*读指针 buffer start*/
    uint32_t size; /*环形缓冲区大小 buffer length*/
    char *buf; /*环形缓冲区buffer, 大小一定要为2的n次方才能使用该高级用法 */
} RingBuffer;

//buf_size 应是2的n次幂
void *RingBuffer_create(uint32_t buf_size) {
    if (!is_power_of_2(buf_size)) {
        LOGW("RingBuffer_create buf_size=%d is not power of 2", buf_size);
        buf_size = roundup_pow_of_two(buf_size);
        LOGW("RingBuffer_create changed buf_size to %d ", buf_size);
    }
    RingBuffer *ring_buffer_p = calloc(1, sizeof(RingBuffer));
    ring_buffer_p->buf = (char *) calloc(buf_size, 1);
    if (!ring_buffer_p->buf) {
        LOGE("can't malloc ring_buffer->buf. size=%d", buf_size);
    }
    ring_buffer_p->size = buf_size;
    ring_buffer_p->in = 0;
    ring_buffer_p->out = 0;
    return ring_buffer_p;
}

void RingBuffer_destroy(void *ring_buffer_p) {
    if (ring_buffer_p) {
        ((RingBuffer *)ring_buffer_p)->in = 0;
        ((RingBuffer *)ring_buffer_p)->out = 0;
        ((RingBuffer *)ring_buffer_p)->size = 0;

        if (((RingBuffer *)ring_buffer_p)->buf) {
            free(((RingBuffer *)ring_buffer_p)->buf);
            ((RingBuffer *)ring_buffer_p)->buf = NULL;
        }
        free(ring_buffer_p);
    }
}

inline bool RingBuffer_empty(const void *buffer) {
    return RingBuffer_available_data(buffer) == 0;
}

inline bool RingBuffer_full(const void *ring_buffer_p) {
    return RingBuffer_available_space(ring_buffer_p) == 0;
}

inline void RingBuffer_clear(void *ring_buffer_p) {
    if (ring_buffer_p) {
        ((RingBuffer *)ring_buffer_p)->in = 0;
        ((RingBuffer *)ring_buffer_p)->out = 0;
    }
}

inline uint32_t RingBuffer_available_data(const void *ring_buffer_p) {
    return ((RingBuffer *)ring_buffer_p)->in - ((RingBuffer *)ring_buffer_p)->out;
}

inline uint32_t RingBuffer_available_space(const void *ring_buffer_p) {
    return ((RingBuffer *)ring_buffer_p)->size - RingBuffer_available_data(ring_buffer_p);
}

uint32_t RingBuffer_write(void *ring_buf_p, const void *source, uint32_t size) {
    if (ring_buf_p == NULL || source == NULL) {
        return 0;
    }
    RingBuffer *ring_buf = ring_buf_p;
    uint32_t start =0, len = 0, rest = 0;
    size = MIN(size, ring_buf->size - ring_buf->in + ring_buf->out);
    /* first put the data starting from fifo->in to buffer end */
    start = ring_buf->in & (ring_buf->size - 1);
    len = MIN(size, ring_buf->size - start);
    memcpy(ring_buf->buf + start, source, len);
    /* then put the rest (if any) at the beginning of the buffer */
    if ((rest = size - len) > 0) {
        memcpy(ring_buf->buf, (char *) source + len, rest);
    }
    ring_buf->in += size;
    return size;
}

uint32_t RingBuffer_read(void *ring_buf_p, void *target, uint32_t size) {
    if (ring_buf_p == NULL || target == NULL) {
        return 0;
    }
    RingBuffer *ring_buf = ring_buf_p;
    uint32_t start = 0, len = 0, rest = 0;
    size = MIN(size, ring_buf->in - ring_buf->out);
    /* first get the data from fifo->out until the end of the buffer */
    start = ring_buf->out & (ring_buf->size - 1);
    len = MIN(size, ring_buf->size - start);
    memcpy(target, ring_buf->buf + start, len);
    /* then get the rest (if any) from the beginning of the buffer */
    if ((rest = size - len) > 0) {
        memcpy(target + len, ring_buf->buf, rest);
    }
    ring_buf->out += size;
    return size;
}
