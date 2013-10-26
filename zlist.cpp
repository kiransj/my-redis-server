#include <zlist.h>
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

