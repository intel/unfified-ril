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

#include "osal_mutex.h"

int osal_mutex_unix_init(osal_mutex_t* m) {
    m->mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    if (m->mutex != NULL)
      return pthread_mutex_init((pthread_mutex_t*) m->mutex, NULL);
    else
      return -1;
}

int osal_mutex_unix_destroy(osal_mutex_t* m) {
    int ret = pthread_mutex_destroy((pthread_mutex_t*) m->mutex);
    free(m->mutex);
    return ret;
}

int osal_mutex_unix_lock(const osal_mutex_t* m) {
    return pthread_mutex_lock((pthread_mutex_t*) m->mutex);
}

int osal_mutex_unix_timedlock(const osal_mutex_t* m, uint32_t ms) {
    struct timespec timeout;
    struct timeval tp;

    gettimeofday(&tp, NULL);

    timeout.tv_sec = tp.tv_sec + ms / 1000;
    timeout.tv_nsec = tp.tv_usec + (ms % 1000) * 1000000;
    return pthread_mutex_timedlock((pthread_mutex_t*) m->mutex, &timeout);
}

int osal_mutex_unix_trylock(const osal_mutex_t* m) {
    return pthread_mutex_trylock((pthread_mutex_t*) m->mutex);
}

int osal_mutex_unix_unlock(const osal_mutex_t* m) {
    return pthread_mutex_unlock((pthread_mutex_t*) m->mutex);
}

pthread_mutex_t s_default_mutex = PTHREAD_MUTEX_INITIALIZER;
