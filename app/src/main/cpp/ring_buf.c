//
// Created by Administrator on 2019/8/23.
//
#include "ring_buf.h"
#include <string.h>
#include <malloc.h>
#include <stdbool.h>

#define RING_BUF_TAKE_MIN(a, b) ((a) > (b) ? (b) : (a))


struct __ring_buf_t{
    char *pbuf;
    bool is_pbuf_internal_malloced;
    size_t size;
    size_t offset_read;
    size_t offset_write;
};

ring_handle ring_buf_create(size_t size){
    char *pbuf = (char *)malloc(size);
    if (!pbuf) {
        return NULL;
    }
    ring_handle handle = ring_buf_create_with_mem(pbuf, size);
    if (!handle) {
        free(pbuf);
    } else {
        handle->is_pbuf_internal_malloced = true;
    }
    return handle;
}

ring_handle ring_buf_create_with_mem(void *pbuf, size_t buf_size){
    ring_handle handle = malloc(sizeof(struct __ring_buf_t));
    if (!handle) {
        return NULL;
    }
    handle->is_pbuf_internal_malloced = false;
    handle->size = buf_size;
    handle->pbuf = pbuf;
    handle->offset_read = handle->offset_write = 0;
    return handle;
}

extern inline size_t ring_buf_available_data(ring_handle handle){
    return (handle->size + handle->offset_write - handle->offset_read) % (handle->size);
//    ssize_t temp = (ssize_t)handle->offset_write - (ssize_t)handle->offset_read;
//    if (temp >= 0) {
//        return (size_t)temp;
//    }
//    return (handle->size + temp) % (handle->size);
}

extern inline size_t ring_buf_available_space(ring_handle handle){
    return handle->size - ring_buf_available_data(handle) - 1;
}

size_t ring_buf_read(ring_handle handle, void *target, size_t len){
    if (len == 0 || len > ring_buf_available_data(handle)) {
        return 0;
    }

    //first part: from read offset to end.
    size_t first_part_len = RING_BUF_TAKE_MIN(len, handle->size - handle->offset_read);
    memcpy(target, handle->pbuf + handle->offset_read, first_part_len);

    //second part: from begin
    size_t second_part_len = len - first_part_len;
    if (0 == second_part_len) {
        size_t new_offset_read = handle->offset_read + first_part_len;
        if (new_offset_read == handle->size) {
            new_offset_read = 0;
        }
        handle->offset_read = new_offset_read;
    } else {
        memcpy(target + first_part_len, handle->pbuf, second_part_len);
        handle->offset_read = second_part_len;
    }
    return len;
}

size_t ring_buf_write(ring_handle handle, void *source, size_t len){
    if (len == 0 || len > ring_buf_available_space(handle)) {
        return 0;
    }

    //first part: from write offset to end.
    size_t first_part_len = RING_BUF_TAKE_MIN(len, handle->size - handle->offset_write);
    if (first_part_len) {
        memcpy(handle->pbuf + handle->offset_write, source, first_part_len);
    }

    //second part: from begin
    size_t second_part_len = len - first_part_len;
    if (0 == second_part_len) {
        size_t new_offset_write = handle->offset_write + first_part_len;
        if (new_offset_write == handle->size) {
            new_offset_write = 0;
        }
        handle->offset_write = new_offset_write;
    } else {
        memcpy(handle->pbuf, source + first_part_len, second_part_len);
        handle->offset_write = second_part_len;
    }
    return len;
}

void ring_buf_clear(ring_handle handle){
    handle->offset_read = handle->offset_write = 0;
}

void ring_buf_destroy(ring_handle *handle_p){
    if (!handle_p) {
        return;
    }
    ring_handle handle = *handle_p;
    if (!handle) {
        return;
    }
    if (handle->pbuf && handle->is_pbuf_internal_malloced) {
        free(handle->pbuf);
        handle->pbuf = NULL;
    }
    free(handle);
    *handle_p = NULL;
}