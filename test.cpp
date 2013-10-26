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
        t.AddNode(key, string(buffer));
    }
     
    t.AddNode(0, "Zero");
    t.AddNode(3187889, "kiran");
    t.AddNode(3187889, "kiran1");
    t.CheckList();
    return 0;
}
