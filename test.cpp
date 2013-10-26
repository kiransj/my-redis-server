#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

#include <string>
using namespace std;

int main(int argc, char *argv[])
{
    int i = 0, count;
    Tree<int> t;

    count = argc > 1 ? atoi(argv[1]) : 100;

    for(i = count; i >= 1; i--)
    {
        int key = random() % 3214300 + 1;
        char buffer[1024];
        snprintf(buffer, 1024, "'%d'", key); 
        Node<int> *n = new Node<int>(key), *tmp;        
        tmp = t.AddNode(n);
        if(tmp != n)
        {            
            delete n;
        }
        tmp->SetData(string(buffer));
        t.IncNumElements();
    }
     
    Node<int> *n = new Node<int>(3187889), *tmp;
    tmp = t.AddNode(n);
    if(tmp != n)
    {            
        delete n;
    }
    tmp->SetData(string("kiran"));
    t.IncNumElements();
    //t.CheckList();
    return 0;
}
