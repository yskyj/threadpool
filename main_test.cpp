#include <iostream>
#include "threadpool.h"
#include "conqueue.h"
#include <vector>
#include <signal.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
using namespace std;

void Signal()
{
        signal(SIGINT, SIG_IGN);
        signal(SIGHUP, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGPIPE, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGCHLD, SIG_IGN);
        signal(SIGTERM, SIG_IGN);

        struct sigaction sig;
        sig.sa_handler = SIG_IGN;
        sig.sa_flags = 0;
        sigemptyset(&sig.sa_mask);
        sigaction(SIGPIPE, &sig, NULL);
}

void DumpTraceback(int Signal) {
        const int len = 2000;
        void* buffer[len];
        int nptrs = ::backtrace(buffer, len);
        char** strings = ::backtrace_symbols(buffer, nptrs);
        if (strings) {
                for (int i = 0; i < nptrs; ++i) {
                        printf("line=%d||trace_back=%s||\n", i, strings[i]);
                }
                printf("\n");
                free(strings);
        }
        exit(1);
}


void* hi(void *arg) {
        cout<<"hi"<<endl;
        return NULL;
}

void* count(void *arg) {
        params *p = (params *)arg;
        if (p == NULL) {
                return p;
        } 
        int c = p->num;
        // cout<<"num:"<< *c <<endl;
        printf("ret:%p:%d\n", arg, c);
        return NULL;
}
int main() {
       Signal();
       // signal(SIGBUS,  DumpTraceback);
       // signal(SIGSEGV, DumpTraceback);
       // signal(SIGABRT, DumpTraceback);
       // signal(SIGILL,  DumpTraceback);
       // signal(SIGFPE,  DumpTraceback);
       // signal(SIGTERM, DumpTraceback);

        vector<params> v;
        vector<task*> vt; 
        v.clear();

#if 0
        hi(NULL);
        conqueue conq;
        task  f1;
        f1.handle = hi;
        f1.arg = NULL;
        conq.push(&f1);
        cout<<conq.size()<<endl;
        task *tmp =  conq.pop();
        (*(tmp->handle))(tmp->arg);
        cout<<conq.size()<<endl;
#endif

        threadPool asynio(10);
        asynio.Init();
        for (int i = 0; i < 100000; i++) {
                params p;
                p.num = i;
                v.push_back(p);
        }
        for (int i = 0; i < v.size(); i++) {
                task *ta = new task;
                vt.push_back(ta); 
                ta->handle = count;
                ta->arg = &v[i];
                asynio.Push(ta);
        }
        //task *t = asynio.getTask();
        //printf("Task:%p\n", t);

        printf("beigin wait\n"); 
        asynio.Wait(); 
        printf("end wait\n"); 
        for (int i = 0; i < vt.size(); i++) {
            delete vt[i];
        }
        return 0;
}
