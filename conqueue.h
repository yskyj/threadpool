#ifndef __CON_QUEUE__
#define __CON_QUEUE__
#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>


typedef void* (*FuncHandle)(void *);

typedef struct _task{
        FuncHandle handle;
        void *arg;
}task;

typedef struct _params{
        int start;
        int end;
        int num;
        sem_t *sem;
}params;

class conqueue{
        public:
                conqueue() {
                        pthread_mutex_init(&mutex, NULL);
                        pthread_cond_init(&cond, NULL);
                }
                ~conqueue() {}

                void push(task* &t) {
                        pthread_mutex_lock(&mutex);
                        q.push(t);
                        pthread_mutex_unlock(&mutex);
                        pthread_cond_signal(&cond);
                }

                void pop(task* &t) {
                        pthread_mutex_lock(&mutex);
                        while (q.empty()) {
                            pthread_cond_wait(&cond, &mutex);
                        }
                        t = q.front(); 
                        q.pop();
                        pthread_mutex_unlock(&mutex);
                }

                void size(size_t& size) {
                        pthread_mutex_lock(&mutex);
                        size = q.size();
                        pthread_mutex_unlock(&mutex);
                }
        private:
                std::queue<task*> q;
                pthread_mutex_t mutex;
                pthread_cond_t cond;

};
#endif
