#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

#include "zlist.h"

#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
    ZList zl;
    int count = argc == 2 ? atoi(argv[1]) : 1000;

    for(int i = 0; i < count; i++)
    {
        char buffer[64];
        snprintf(buffer, 64, "N%d", i);
        zl.ZADD(i, buffer);
        if(i % 2 == 0)
        {
            snprintf(buffer, 64, "N-%d", i);
            zl.ZADD(i, buffer);
        }
    }
    cout<<"ZCARD MYSET = "<<zl.ZCARD()<<endl;
    cout<<"ZCOUNT MYSET = "<<zl.ZCOUNT(9, 20)<<endl;
    cout<<"ZRANK MYSET = "<<zl.ZRANK("N0")<<endl;
    zl.check();
    cout<<endl<<endl<<"ZRange(0, 30)"<<endl;
    zl.ZRANGE(0, 30, false);
    return 0;
}

int main1(int argc, char *argv[])
{
    int tmp = 0;
    int i = 0, count;
    Tree<int> t;

    count = argc > 1 ? atoi(argv[1]) : 100;

    time_t tm = time(NULL);
    cout<<"Seed "<<tm<<endl;
    srandom(tm);
    for(i = count; i >= 1; i--)
    {
        int key = (random() % 3214300 + 1);
        char buffer[1024];
        snprintf(buffer, 1024, "'%d", key+tmp++); 
        t.AddNode(key, string(buffer));
        snprintf(buffer, 1024, "'%d", key+tmp++); 
        t.AddNode(key, string(buffer));
    }
     
    t.AddNode(0, "Zero");
    t.AddNode(0, "Zero1");
    t.AddNode(0, "Zero2");
    t.AddNode(0, "Zero3");
    t.CheckList();

    int rt = 0;
    Node<int> *n = t.GetNthElement(18400, &rt);
    if(!IS_NULL(n))    
        cout<<"18400th element : "<<n->GetKey()<<"  t = "<<rt<<endl;
    return 0;
}
