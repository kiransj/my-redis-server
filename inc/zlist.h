#ifndef _ZLIST_H_
#define _ZLIST_H_
#include "tree.h"

#include <string>
#include <map>
using namespace std;

class ZList
{
    private:
        /* used for commands z* */
        unsigned int count;
        Tree<int> tr;
        map<string, int> dict;


        bool iterator_active;
        Node<int> *n;
        set<string>::iterator ii;
    public:
        ZList()  { count = 0; }
        ~ZList() { }
        int ZADD(int key, string data);
        int ZCARD(void);
        int ZCOUNT(int min, int max);
        int ZRANGE(int min, int max, bool WITHSCORES);

        /*Return the idx in the sorted list i.e. return the link number*/
        int ZRANK(string data);
        /*Returns the score of the given data*/
        int ZSCORE(string data);

        int ZRANGEBYSCORE(int min, int max, bool WITHSCORES);

        void check(void) { tr.CheckList(); }

        int GetNext(int *key, char **str, int *length);
};

#endif
