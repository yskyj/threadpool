#ifndef __THREAD_POOL__
#define __THREAD_POOL__
#include "conqueue.h"
#include <list>
#include <vector>
#include <semaphore.h>
#include <iostream>
void* Work(void *arg);

class threadPool{
        public:
                threadPool(int num):thread_num(num){
                    pthread_mutex_init(&mutex, NULL);
                }
                ~threadPool() {}


                int Init() {
                    is_alive = true;
                    for(int i=0; i < thread_num; i++) {
                        pthread_t tid;
                        pthread_create(&tid, NULL, Work, (void*)this);
                        thread_ids.push_back(tid);
                    }
                    return 0;
                }

                void Push(task *t) {
                    m_cqueue.push(t);
                   // m_list.insert(t);
                    return ;
                }

                void Wait() {
                    return ;
                }

                void Stop() {
                    pthread_mutex_lock(&mutex);                    
                    is_alive = false;
                    pthread_mutex_unlock(&mutex);                    

                    for (int i = 0; i < thread_ids.size(); i++) {
                        pthread_join(thread_ids[i], NULL);
                    }
                }

                bool isAlive() {
                    pthread_mutex_lock(&mutex);
                    bool ret = is_alive;
                    pthread_mutex_unlock(&mutex);
                    return ret;
                }

                conqueue& getConQueue() {
                    return m_cqueue;
                }
        private:
                conqueue  m_cqueue;
                std::list<task*> m_list;
                std::vector<pthread_t> thread_ids;
                bool is_alive;
                pthread_mutex_t mutex;
                int thread_num;
};

void* Work(void *arg) {
        threadPool *pool = (threadPool*)arg;
        while(pool->isAlive()) {
                while(pool->getConQueue().size() > 0) {
                        task *work = pool->getConQueue().pop();
                        if (work != NULL) {
                                params *p = (params *)(work->arg);
                                if (work->handle != NULL) {
                                        printf("qq %p:%p\n", work, p);
                                        (*work->handle)((void*)p);
                                        //std::cout<<work<<":"<<p<<std::endl;
                                }
                        }
                }
        } 
        return NULL;
}
#endif
