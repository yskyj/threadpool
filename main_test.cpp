#include <iostream>
#include "threadpool.h"
#include "conqueue.h"
#include <vector>
using namespace std;


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
    vector<params> v;
    vector<task> vt; 
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

    threadPool asynio(2);
    asynio.Init();
    for (int i = 0; i < 50; i++) {
        params p;
        p.num = i;
        v.push_back(p);
    }
    for (int i = 0; i < v.size(); i++) {
        //cout<<i<<endl;
        task ta;
        vt.push_back(ta); 
        vt.back().handle = count;
        vt.back().arg = &v[i];
        asynio.Push(&(vt.back()));
        //cout<<"hah:"<<&v[i]<<":"<<&ta<<endl;
        printf("hha:%p:%p\n", &(vt.back()), &v[i]);
    }

    asynio.Stop();
    return 0;
}
