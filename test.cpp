#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[])
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
//    t.AddNode(3187889, "kiran");
//    t.AddNode(3287889, "kiran1");
    t.CheckList();

    int rt = 0;
    Node<int> *n = t.GetNthElement(18400, &rt);
    if(!IS_NULL(n))    
        cout<<"18400th element : "<<n->GetKey()<<"  t = "<<rt<<endl;
    return 0;
}
