#ifndef __CON_QUEUE__
#define __CON_QUEUE__
#include <queue>
#include <pthread.h>

typedef void* (*FuncHandle)(void *);

typedef struct _task{
        FuncHandle handle;
        void *arg;
}task;

typedef struct _params{
        int start;
        int end;
        int num;
}params;

class conqueue{
        public:
                conqueue() {
                        pthread_mutex_init(&mutex, NULL);
                }
                ~conqueue() {}

                void push(task* t) {
                        pthread_mutex_lock(&mutex);
                        q.push(t);
                        pthread_mutex_unlock(&mutex);
                }

                task* pop() {
                        pthread_mutex_lock(&mutex);
                        task *ret = q.front(); 
                        q.pop();
                        pthread_mutex_unlock(&mutex);
                        return ret;
                }

                int size() {
                        pthread_mutex_lock(&mutex);
                        int size = q.size();
                        pthread_mutex_unlock(&mutex);
                        return size;
                }
        private:
                std::queue<task*> q;
                pthread_mutex_t mutex;

};
#endif
