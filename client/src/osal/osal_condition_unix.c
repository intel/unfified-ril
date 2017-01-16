/*
 * Copyright (c) 2016, Intel Corporation, Intel Mobile Communication
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

#include "osal_condition.h"
#include "osal_mutex.h"

int osal_condition_unix_init(osal_condition_t* c) {
    c->condition = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
    if (c->condition != NULL)
      return pthread_cond_init((pthread_cond_t*) c->condition, NULL);
    else
      return -1;
}

int osal_condition_unix_destroy(osal_condition_t* c) {
    int ret = pthread_cond_destroy((pthread_cond_t*) c->condition);
    free(c->condition);
    return ret;
}

int osal_condition_unix_signal(const osal_condition_t* c) {
    return pthread_cond_signal((pthread_cond_t*) c->condition);
}

int osal_condition_unix_broadcast(const osal_condition_t* c) {
    return pthread_cond_broadcast((pthread_cond_t*) c->condition);
}

int osal_condition_unix_wait(const osal_condition_t* c, osal_mutex_t* mutex) {
    return pthread_cond_wait((pthread_cond_t*) c->condition,
            (pthread_mutex_t*) mutex->mutex);
}

int osal_condition_unix_timedwait(const osal_condition_t* c,
        osal_mutex_t* mutex, uint32_t ms) {
    struct timespec timeout;
    struct timeval tp;

    gettimeofday(&tp, NULL);

    timeout.tv_sec = tp.tv_sec + ms / 1000;
    timeout.tv_nsec = tp.tv_usec + (ms % 1000) * 1000000;
    return pthread_cond_timedwait((pthread_cond_t*) c->condition,
            (pthread_mutex_t*) mutex->mutex, &timeout);
}

pthread_cond_t s_default_condition = PTHREAD_COND_INITIALIZER;

