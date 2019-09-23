//
// Created by Administrator on 2019/8/23.
//

#ifndef CRINGBUFFER_RING_BUF_H
#define CRINGBUFFER_RING_BUF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef struct __ring_buf_t *ring_handle;

/**
 * create ring_buf.
 * @param size ring_buf size. can hold (size - 1) byte.
 * @return ring_buf handle
 */
ring_handle ring_buf_create(size_t size);

/**
 * create ring_buf with provided pbuf.
 * <p>you maybe need free pbuf yourself after ring_buf_destroy() if your pbuf is malloced</p>
 * @param pbuf the buf to hold ring data.
 * @param buf_size the pbuf memory size.
 * @return ring_buf handle
 */
ring_handle ring_buf_create_with_mem(void *pbuf, size_t buf_size);

/**
 * The data count available to read.
 * @param handle ring_buf handle
 * @return size of current available data.
 */
size_t ring_buf_available_data(ring_handle handle);

/**
 * The space count available to write.
 * @param handle ring_buf handle
 * @return size of current available space.
 */
size_t ring_buf_available_space(ring_handle handle);

/**
 * read data from ring_buf
 * @param handle ring_buf handle
 * @param target copy read data to this pointer.
 * @param len the length you want to read.
 * @return size of real read.
 */
size_t ring_buf_read(ring_handle handle, void *target, size_t len);

/**
 * write data to ring_buf
 * @param handle ring_buf handle
 * @param source copy this pointer data to ring_buf.
 * @param len the length you want to write.
 * @return size of real write.
 */
size_t ring_buf_write(ring_handle handle, void *source, size_t len);

/**
 * clear ring_buf data.
 * <p>WARN: you shouldn't call clear while calling read/write, otherwise it will have thread safe issue</p>
 * @param handle ring_buf handle.
 */
void ring_buf_clear(ring_handle handle);

/**
 * destroy ring_buf and release memory.
 * <p>maybe you need free pbuf yourself after this destroy call if you provided malloced pbuf.</p>
 * @param handle_p
 */
void ring_buf_destroy(ring_handle *handle_p);

#ifdef __cplusplus
};
#endif

#endif //CRINGBUFFER_RING_BUF_H
