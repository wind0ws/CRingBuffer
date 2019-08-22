//
// Created by Threshold on 2018/11/7.
//
#include <malloc.h>
#include <stdlib.h>
#include "simple_queue.h"

struct __simple_queue_handle {
    volatile SimpleQueueDataType *PutPt;
    volatile SimpleQueueDataType *GetPt;
    SimpleQueueDataType Fifo[SIMPLE_QUEUE_FIFO_SIZE];
};

/* Initialization. */
simple_queue_handle simple_queue_init(void) {
    simple_queue_handle handle = malloc(sizeof(struct __simple_queue_handle));
    handle->PutPt = handle->GetPt = &(handle->Fifo[0]);
    return handle;
}

int simple_queue_available_space(simple_queue_handle handle) {
    return SIMPLE_QUEUE_FIFO_SIZE - simple_queue_available_data(handle) - 1;
}

int simple_queue_available_data(simple_queue_handle handle) {
    int nextPutPt = (int) handle->PutPt;
    int nextGetPt = (int) handle->GetPt;
    int temp = (nextPutPt - nextGetPt) / sizeof(SimpleQueueDataType);
    if (temp >= 0) {
        return temp;
    }
    return (temp + SIMPLE_QUEUE_FIFO_SIZE) % SIMPLE_QUEUE_FIFO_SIZE;
}

bool simple_queue_is_empty(simple_queue_handle handle) {
    return simple_queue_available_data(handle) == 0;
}

bool simple_queue_is_full(simple_queue_handle handle) {
    return simple_queue_available_space(handle) == 0;
}

int simple_queue_push(simple_queue_handle handle, SimpleQueueDataType data) {
    SimpleQueueDataType volatile *nextPutPt = handle->PutPt + 1;
    if (nextPutPt == &(handle->Fifo[SIMPLE_QUEUE_FIFO_SIZE])) {
        nextPutPt = &(handle->Fifo[0]); /* Wrap around. */
    }
    if (nextPutPt == handle->GetPt) {
        /* Fifo is full. */
        return SIMPLE_QUEUE_FAIL;
    }

    *(handle->PutPt) = data;
    handle->PutPt = nextPutPt;
    return SIMPLE_QUEUE_SUCCESS;
}

int simple_queue_pop(simple_queue_handle handle, SimpleQueueDataType *dataPtr) {
    if (handle->PutPt == handle->GetPt) {
        /* Fifo is empty. */
        return SIMPLE_QUEUE_FAIL;
    }

    *dataPtr = *(handle->GetPt);
    handle->GetPt++;
    if (handle->GetPt == &(handle->Fifo[SIMPLE_QUEUE_FIFO_SIZE])) {
        handle->GetPt = &(handle->Fifo[0]); /* Wrap around */
    }
    return SIMPLE_QUEUE_SUCCESS;
}

int simple_queue_destroy(simple_queue_handle *handle_p) {
    simple_queue_handle handle;
    if (handle_p != NULL && (handle = *handle_p) != NULL) {
        free(handle);
        *handle_p = NULL;
    }
    return SIMPLE_QUEUE_SUCCESS;
}

