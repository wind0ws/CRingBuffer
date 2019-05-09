//
// Created by Administrator on 2019/5/8.
//
#include <pthread.h>
#include <malloc.h>
#include <semaphore.h>
#include "mlog.h"
#include "msg_queue_handler.h"

struct __queue_handler {
    ring_msg_queue msg_queue_p;
    msg_handler_callback callback;
    pthread_t thread_handler;
    sem_t sem_handler;
    bool flag_exit_thread;
};

static void *thread_fun_handle_msg(void *thread_context) {
    queue_handler handler_p = (queue_handler) thread_context;
    queue_msg_t msg = {0};
    while (!(handler_p->flag_exit_thread)) {
        sem_wait(&handler_p->sem_handler);
        if (handler_p->flag_exit_thread) {
            break;
        }
        if (!RingMsgQueue_pop(handler_p->msg_queue_p, &msg)) {
            continue;
        }
        handler_p->callback(&msg);
    }
    return NULL;
}

queue_handler
QueueHandler_create(__in uint32_t max_msg_capacity, __in msg_handler_callback callback) {
    LOGD("create queue_handler. max_msg_capacity=%d", max_msg_capacity);
    queue_handler handler_p = calloc(1, sizeof(struct __queue_handler));
    handler_p->flag_exit_thread = false;
    handler_p->callback = callback;
    sem_init(&(handler_p->sem_handler), 0, 0);
    if (pthread_create(&(handler_p->thread_handler), NULL, thread_fun_handle_msg, handler_p) == 0) {
        handler_p->msg_queue_p = RingMsgQueue_create(max_msg_capacity);
    } else {
        LOGE("error on create pthread of queue handle msg");
        sem_destroy(&(handler_p->sem_handler));
        free(handler_p);
        handler_p = NULL;
    }
    return handler_p;
}

bool QueueHandler_send(__in queue_handler queue_handler_p, __in queue_msg_t *msg_p) {
    if (!queue_handler_p || queue_handler_p->thread_handler == 0 ||
        queue_handler_p->msg_queue_p == NULL) {
        return false;
    }
    if (!RingMsgQueue_push(queue_handler_p->msg_queue_p, msg_p)) {
//        LOGE("send msg to queue handled failed. queue is full");
        return false;
    }
    sem_post(&(queue_handler_p->sem_handler));
    return true;
}

inline uint32_t QueueHandler_available_send_msg_amount(__in queue_handler handler_p) {
    return RingMsgQueue_available_push_msg_amount(handler_p->msg_queue_p);
}

inline uint32_t QueueHandler_current_queue_msg_amount(__in queue_handler handler_p) {
    return RingMsgQueue_available_pop_msg_amount(handler_p->msg_queue_p);
}

inline bool QueueHandler_is_empty(__in queue_handler handler_p){
    return QueueHandler_current_queue_msg_amount(handler_p) == 0;
}

inline bool QueueHandler_is_full(__in queue_handler handler_p){
    return QueueHandler_available_send_msg_amount(handler_p) == 0;
}

void QueueHandler_destroy(__in queue_handler handler_p) {
    if (!handler_p) {
        return;
    }
    handler_p->flag_exit_thread = true;
    if (handler_p->thread_handler != 0) {
        //send a signal to make sure thread is not stuck at sem_wait
        sem_post(&(handler_p->sem_handler));
        if (pthread_join(handler_p->thread_handler, NULL) != 0) {
            LOGE("error on join handle msg thread.");
        }
        sem_destroy(&(handler_p->sem_handler));
        RingMsgQueue_destroy(handler_p->msg_queue_p);
        handler_p->msg_queue_p = NULL;
    }
    handler_p->thread_handler = 0;
    free(handler_p);
}
