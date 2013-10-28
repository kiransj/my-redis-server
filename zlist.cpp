#include <zlist.h>

#include <set>
#include <iostream>
using namespace std;
int ZList::ZADD(const int key, const string data)
{
    int *tmp = &dict[data];
    if(0 == *tmp)
    {
        if(tr.AddNode(key, data) != NULL)
        {
            *tmp = key;
            count++;
            return 1;
        }
    }
    return 0;
}

int ZList::ZCARD(void)
{
    return tr.GetNumElements();
}

int ZList::ZCOUNT(int min, int max)
{
    int count = 0;
    Node<int> *n;
    if(min < 0 && (min > max) && (min >= count)) 
    {
        return 0;
    }

    n = tr.FindNear(min);
    while(!IS_NULL(n) && (n->GetKey() >= min) && (n->GetKey() <= max))
    {
        count += n->GetData().size();
        n = n->GetNext();
    }
    return count;
}


int ZList::ZRANK(string data)
{
    int rank = 0;
    Node<int> *n;
    int score = dict[data];
    if(score != 0)
    {
        n = tr.GetKeyNodeAndNumber(score, &rank);
        if(!IS_NULL(n))
        {
            for(set<string>::iterator ii = n->GetData().begin(); ii != n->GetData().end(); ++ii)
            {
                if(*ii == data)
                {
                    break;
                }
                rank++;
            } 
        }
    }
    return rank;
}

int ZList::ZRANGE(int min, int max, bool WITHSCORES)
{
    
    int rth = 0;

    if(min < 0)
        min = tr.GetNumElements() + min;

    if(max < 0)
        max = tr.GetNumElements() + max;

    if(max < min)
    {
        log_msg("Max (%d) < Min(%d) hence zrange failed", max, min);
        return 0;
    }

    n = tr.GetNthElement(min, &rth);
    
    rth = min - rth;

    Min = min;
    Max = max;
    while(!IS_NULL(n))
    {
        for(ii = n->GetData().begin(); ii != n->GetData().end(); ++ii)
        {
#if 0
            if(min == max)
            {
                return 0;
            }
#endif
            if(!rth)
            {
                return 1;
            }
            else
            {
                --rth;
            }
        }
        n = n->GetNext();
    }
    return 0; 
}

int ZList::GetNext(int *key, char *str, int str_len)
{
    if(IS_NULL(n) || (Min > Max)) 
    {
        Min = Max = 0;
        n = NULL;
        return 0;
    }

    if(ii != n->GetData().end())
    {
        *key = n->GetKey();
        Min++;
        snprintf(str, str_len, "%s", ii->c_str());
        ii++;
        return 1;
    }

    n = n->GetNext();
    if(!IS_NULL(n))
    {
        ii = n->GetData().begin();
        if(ii != n->GetData().end())
        {
            *key = n->GetKey();
            Min++;
            snprintf(str, str_len, "%s", ii->c_str());
            ++ii;
            return 1;
        }
    }
    return 0;
}
