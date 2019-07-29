//
// Created by Threshold on 2018/12/4.
//
#ifndef CRINGBUFFER_RINGBUFFER_H
#define CRINGBUFFER_RINGBUFFER_H

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
 * create RingBuffer.
 * <p>WARN: The ring buffer real size is not always as same as you provided.</p>
 * @param size RingBuffer size, must be pow of 2. if not, will change size to next pow of it automatically.
 * @return RingBuffer pointer
 */
ring_buf RingBuffer_create(__in uint32_t size);

/**
 * destroy RingBuffer and free memory
 * @param ring_buf_p RingBuffer pointer
 */
void RingBuffer_destroy(__in const ring_buf ring_buf_p);

/**
 * copy specified size memory from RingBuffer to target
 * <p><If current RingBuffer can read data size is little than specified size, then only max readable data size to target /p>
 * @param ring_buf_p RingBuffer
 * @param target the target pointer to write data
 * @param size copy specified size memory
 * @return the real read data size
 */
uint32_t RingBuffer_read(__in const ring_buf ring_buf_p, __out void *target, uint32_t size);

/**
 * copy specified size memory from source to RingBuffer
 * <p><If current RingBuffer can write data size is little than specified size, then only copy max writable data size from source /p>
 * @param ring_buf_p RingBuffer
 * @param source the source pointer to read data
 * @param size copy specified size memory
 * @return the real wrote data size
 */
uint32_t RingBuffer_write(__in const ring_buf ring_buf_p, __in const void *source, uint32_t size);

/**
 * clear RingBuffer all data.
 * <p>WARN: you shouldn't call clear while calling read/write, otherwise it will have thread safe issue</p>
 * @param ring_buffer_p RingBuffer
 */
inline void RingBuffer_clear(__in const ring_buf ring_buffer_p);

/**
 * get current RingBuffer available data size
 * @param ring_buf_p RingBuffer
 * @return available read byte size
 */
inline uint32_t RingBuffer_available_data(__in const ring_buf ring_buf_p);

/**
 * get current RingBuffer available space to write
 * @param ring_buf_p RingBuffer
 * @return available write byte size
 */
inline uint32_t RingBuffer_available_space(__in const ring_buf ring_buf_p);

/**
 * indicate RingBuffer whether is empty(no data to read)
 * @param ring_buf_p RingBuffer
 * @return true indicate no data to read
 */
inline bool RingBuffer_is_empty(__in const ring_buf ring_buf_p);

/**
 * indicate RingBuffer whether is full(no space to write)
 * @param ring_buf_p RingBuffer
 * @return true indicate no space to write
 */
inline bool RingBuffer_is_full(__in const ring_buf ring_buf_p);

#ifdef __cplusplus
}
#endif

#endif //CRINGBUFFER_RINGBUFFER_H
