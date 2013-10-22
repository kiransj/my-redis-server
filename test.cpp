#include <stdio.h>
#include <stdlib.h>
#include "tree.h"


int main(int argc, char *argv[])
{
    int i = 0, count;
    Tree<int> t;
    if(argc == 1)
    {
        log_msg("usage ./a.out <number>");
        return -1;
    }
    count = atoi(argv[1]);

    for(i = count; i >= 1; i--)
    {
        Node<int> *n = new Node<int>(i);
        Node<int> *tmp;
        tmp = t.AddNode(n);
        if(tmp != n)
        {
            delete n;
        }
    }
    

#if 1 
    if(argc == 3)
    for(i = count+1; i >= 0; i--)
    {
        Node<int> *tmp = t.FindNode(i);
        if(IS_NULL(tmp))
        {
            log_msg("key '%d' not found", i);
        }
    }    
    t.PrintTree();
    t.InOrder();
    t.PrintList();
    log_msg("Height of the tree : %d", t.Height());
#endif    
    return 0;
}
