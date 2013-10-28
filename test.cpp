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
    int count = argc >= 2 ? atoi(argv[1]) : 1000;

    for(int i = 0; i < count; i++)
    {
        int key = random() % 3214600 + 1;
        char buffer[64];
        snprintf(buffer, 64, "N%d", key);
        zl.ZADD(key, buffer);
        if(key % 7 == 0)
        {
            snprintf(buffer, 64, "N-%d", key);
            zl.ZADD(key, buffer);
        }
    }
    zl.ZADD(56394, "kiran");
    zl.ZADD(898784, "ind");
    zl.ZADD(898784, "pak");
    cout<<"ZCARD MYSET = "<<zl.ZCARD()<<endl;
    cout<<"ZCOUNT MYSET = "<<zl.ZCOUNT(0, 100000)<<endl;
    cout<<"ZRANK MYSET = "<<zl.ZRANK("N0940037")<<endl;
    zl.check();
    if(argc == 4)
    {
        int min = atoi(argv[2]), max = atoi(argv[3]);
        cout<<endl<<endl<<"ZRange("<<min<<", "<<max<<")"<<endl;
        if(zl.ZRANGE(min, max, false))
        {
            char str[1024];
            int key = 0, count = 0;
            while(zl.GetNext(&key, str, 1024))
            {
                cout<<++count<<"> "<<key<<" = "<<str<<endl;
            }
        }
    }
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
