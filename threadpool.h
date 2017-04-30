#ifndef __THREAD_POOL__
#define __THREAD_POOL__
#include "conqueue.h"
#include <list>
#include <vector>
#include <semaphore.h>
#include <iostream>
#include <sys/types.h>    
#include <sys/stat.h>    
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include<sys/syscall.h>
#include<sys/types.h>
#define gettid() syscall(__NR_gettid)
void* Work(void *arg);

class threadPool{
        public:
                threadPool(int num):thread_num(num){
                    pthread_mutex_init(&mutex, NULL);
                    //sem_unlink("sem");
                    //sem = sem_open("sem", O_CREAT, 0, 0);
                    sem_init(&sem, 0, 0);
                }
                ~threadPool() {
                }


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
                    ((params *)(t->arg))->sem = &sem;
                    m_cqueue.push(t);
                    m_list.push_back(t);
                    return ;
                }

                void Wait() {
                    int count = m_list.size();
                    while (count >0) {
                        sem_wait(&sem);
                        count--;
                        //printf("wait -1, size:%d\n", count);
                    }
                    return ;
                }
            
                void Stop() {
                    pthread_mutex_lock(&mutex);                    
                    is_alive = false;
                    pthread_mutex_unlock(&mutex);                    

                    //for (int i = 0; i < thread_ids.size(); i++) {
			        //    printf("waiting for %ld\n", (unsigned int)thread_ids[i]);
                    //    int ret = pthread_join(thread_ids[i], NULL);
                    //    if (ret != 0) {
                    //        printf("thread:%ld join failed. retid:%d\n", (unsigned int)thread_ids[i], ret);
                    //    } else {
                    //        printf("thread:%ld join succeed. retid:%d\n", (unsigned int)thread_ids[i], ret);
                    //    }
                    //}
                }

                size_t Size() {
                    size_t ret = 0;
                    m_cqueue.size(ret);
                    return ret;
                }

                bool isAlive() {
                    bool ret = is_alive;
                    return ret;
                }

                conqueue& getConQueue() {
                    return m_cqueue;
                }

                task* getTask() {
                    task *t = NULL;
                    m_cqueue.pop(t);
                    printf("inner getTask:%p\n", t);
                    return t;
                }
        private:
                conqueue  m_cqueue;
                std::list<task*> m_list;
                std::vector<pthread_t> thread_ids;
                sem_t sem;
                bool is_alive;
                pthread_mutex_t mutex;
                int thread_num;
};

void* Work(void *arg) {
        printf("Work start %ld  %ld\n", gettid(), (unsigned int)pthread_self());
        threadPool *pool = (threadPool*)arg;
        while(pool->isAlive()) {
                task *work = pool->getTask();
                if (work != NULL) {
                        params *p = (params *)(work->arg);
                        if (work->handle != NULL) {
                                (work->handle)((void*)p);
                                sem_post(p->sem);
                        }
                } else {
                        printf("work eq NULL\n");
                }
                //printf("pool size: %d,threadid:%ld\n", pool->Size(),(unsigned int)pthread_self());
        } 
        printf("Work Exit %ld\n", (unsigned int)pthread_self());
        return NULL;
}
#endif
