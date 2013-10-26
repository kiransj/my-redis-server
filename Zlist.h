#include "tree.h"

#include <string>
#include <map>
using namespace std;

class ZList
{
    private:
        unsigned int count;
        Tree<int> tr;
        map<string, int> dict;
    public:
        ZList()  { count = 0; }
        ~ZList() { }
        int ZADD(int key, string data);
        int ZCARD(void);
        int ZCOUNT(int min, int max);
        int ZRANGE(int min, int max, bool WITHSCORES);

        /*Return the idx in the sorted list i.e. return the link number*/
        int ZRANK(string data);
        int ZSCORE(string data);
        int ZRANGEBYSCORE(int min, int max, bool WITHSCORES);
        int ZREVRANGE(int min, int max, bool WITHSCORES);
};
