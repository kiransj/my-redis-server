#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


#include "tree.h"
#include "util.h"

#include <iostream>
#include <queue>
#include <string>
#include <set>
using namespace std;

template <class KEY>
void Node<KEY>::SetLeftNode(Node<KEY> *left)
{
    if(!IS_NULL(left))
    {
        left->parent = this;
    }
    this->left = left;
    UpdateHeight();
}

template <class KEY>
void Node<KEY>::SetRightNode(Node<KEY> *right)
{
    if(!IS_NULL(right))
    {
        right->parent = this;
    }
    this->right = right;
    UpdateHeight();
}

template <class KEY>
void Node<KEY>::UpdateLinks(void)
{
    Node<KEY> *first = NULL;
    if(IS_NULL(this->prev) && IS_NULL(this->next))
    {
        this->count++;
        return;
    }
    if(!IS_NULL(this->prev))
    {
        Node<KEY> *n = this;
        while(n->count == 0)
        {
            n = n->prev;
        }
        ASSERT(n->count == 0);
        n->count++;
        first = n;
    }
    else if(this->count == 0)
    {
        this->count = this->next->count+1;
        this->next->count = 0;
        this->front_link = this->next->front_link;
        if(!IS_NULL(this->next->front_link))
            this->next->front_link->back_link = this;
        this->next->front_link = this->next->back_link = NULL;
        first = this;
    }
    else
    {
        this->count++;
        first = this;
    }

    if(IS_NULL(this->next))
    {
        first->front_link = this;
        this->back_link = first;
    }

    if(first->count >= 200)    
    {
        unsigned int tmp_count = first->values.size();
        Node<KEY> *tmp = first;
        while(tmp_count < 100)
        {
            tmp = tmp->next;
            tmp_count += tmp->values.size();
        }
        tmp = tmp->next;
        tmp->back_link = first;
        if(!IS_NULL(first->front_link))
            first->front_link->back_link = tmp;
        tmp->front_link = first->front_link;
        first->front_link = tmp;
        tmp->count = first->count - tmp_count;
        first->count = tmp_count;
    }
}

template <class KEY>
int Node<KEY>::UpdateHeight(void)
{
    this->height = 0;
    if(!IS_NULL(this->left) && !IS_NULL(this->right))
    {
       int left_height = this->left->height;
       int right_height = this->right->height;
       this->height = MAX(left_height, right_height) + 1;
    }
    else if(!IS_NULL(this->left))
    {
        this->height = this->left->height + 1;
    }
    else if(!IS_NULL(this->right))
    {
        this->height = this->right->height + 1;
    }
    return this->height;
}

template <class KEY>
int Node<KEY>::GetBalance(void)
{
    int balance = 0;

    if(!IS_NULL(this->left) && !IS_NULL(this->right))
    {
        balance = this->left->height - this->right->height;
    }
    else if(!IS_NULL(this->left))
    {
        balance = this->left->height + 1;
    }
    else if(!IS_NULL(this->right))
    {
        balance = (this->right->height + 1) * (-1);
    }   
    return balance;
}

template <class KEY>
void Node<KEY>::InsertBefore(Node<KEY> *node)
{
    Node *prev = this->prev;
    node->next = this;
    node->prev = this->prev;
    this->prev = node;
    if(!IS_NULL(prev))
    {
        prev->next = node;
    }
    return;
}

template <class KEY>
void Node<KEY>::InsertAfter(Node<KEY> *node)
{
    Node *next = this->next;
    node->next = next;
    node->prev = this;
    this->next = node;
    if(!IS_NULL(next))
    {
        next->prev = node;
    }
    return;
}

template <class KEY>
Tree<KEY>::~Tree()
{
    Node<KEY> *tmp = this->first, *old;
    while(!IS_NULL(tmp))
    {
        old = tmp->GetNext();
        delete tmp;
        tmp = old;
    }
}

template <class KEY>
void Tree<KEY>::RotateLeft(Node<KEY> *n)
{
    Node<KEY> *p = n->GetParent(), *temp = n->GetRight();
    enum {LEFT, RIGHT} side = LEFT;

    if(!IS_NULL(p))
    {
        side = (p->GetLeft() == n) ? LEFT : RIGHT;
    }
    n->SetRightNode(temp->GetLeft());
    temp->SetLeftNode(n);
    
    if(!IS_NULL(p))
    {
        (side == LEFT) ? (p->SetLeftNode(temp)) : (p->SetRightNode(temp));
    }
    else
    {
        this->root = temp;
        temp->SetParentNode(NULL); 
    }
}

template <class KEY>
void Tree<KEY>::RotateRight(Node<KEY> *n)
{
    Node<KEY> *p = n->GetParent(), *temp = n->GetLeft();
    enum {LEFT, RIGHT} side = LEFT;

    if(!IS_NULL(p))
    {
        side = (p->GetLeft() == n) ? LEFT : RIGHT;
    }
    n->SetLeftNode(temp->GetRight());
    temp->SetRightNode(n);
    
    if(!IS_NULL(p))
    {
        (side == LEFT) ? (p->SetLeftNode(temp)) : (p->SetRightNode(temp));
    }
    else
    {
        this->root = temp;
        temp->SetParentNode(NULL); 
    }
}

template <class KEY>
void Tree<KEY>::BalanceAt(Node<KEY> *n)
{
    int bal = n->GetBalance();
    if(bal > 1)
    {
        if(n->GetLeft()->GetBalance() < 0)
        {
            RotateLeft(n->GetLeft());
        }
        RotateRight(n);
    }
    else if(bal < -1)
    {
        if(n->GetRight()->GetBalance() > 0)
        {
            RotateRight(n->GetRight());
        }
        RotateLeft(n);
    }
}

template <class KEY>
Node<KEY>* Tree<KEY>::AddNode(const KEY key, const string data)
{
    Node<KEY> *n = new Node<KEY>(key);
    if(IS_NULL(this->root))
    {
        num_keys++;
        this->root = n;
        this->last = this->first = n;
        if(n->SetData(data))
            num_elements++;
        return n;
    }

    Node<KEY> *tmp = this->root;
    while(!IS_NULL(tmp))
    {
        if(tmp->GetKey() > key)
        {
            if(IS_NULL(tmp->GetLeft()))
            {
                tmp->SetLeftNode(n);
                tmp->InsertBefore(n);
                if(this->first->GetKey() > n->GetKey())
                    this->first = n;
                break;
            }
            else
            {
                tmp = tmp->GetLeft();
            }
        }
        else if(tmp->GetKey() < n->GetKey())
        {
            if(IS_NULL(tmp->GetRight()))
            {
                tmp->SetRightNode(n);
                Node<KEY> *n2 = tmp;
                n2->InsertAfter(n);
                if(this->last->GetKey() < n->GetKey())
                    this->last = n;
                break;
            }
            else
            {
                tmp = tmp->GetRight();
            }
        }
        else
        {
            if(tmp->SetData(data))
                num_elements++;
            delete n;
            /* There is a node already with the same key.
             * Return the old node*/
            return tmp;
        }
    }
    this->num_keys++;
    if(n->SetData(data))
        this->num_elements++;

    /*Now check if the tree needs rebalancing*/
    tmp = n;
    while(!IS_NULL(tmp))
    {
        tmp->UpdateHeight();
        this->BalanceAt(tmp);
        tmp = tmp->GetParent();
    }
    return n;   
}

template <class KEY>
Node<KEY>* Tree<KEY>::FindNode(KEY key)
{
    Node<KEY> *tmp = this->root;
    while(!IS_NULL(tmp))
    {
        if(tmp->GetKey() > key)
        {
            tmp = tmp->GetLeft();
        }
        else if(tmp->GetKey() < key)
        {
            tmp = tmp->GetRight();
        }
        else
        {
            break;
        }
    }
    return tmp;
}

template <class KEY>
Node<KEY>* Tree<KEY>::FindNear(KEY key)
{
    Node<KEY> *tmp = this->root, *tmp1;
    while(!IS_NULL(tmp))
    {
        tmp1 = tmp;
        if(tmp->GetKey() > key)
        {
            tmp = tmp->GetLeft();
        }
        else if(tmp->GetKey() < key)
        {

            tmp = tmp->GetRight();
        }
        else
        {
            return tmp;
        }    
    }

    while(!IS_NULL(tmp1) && (key > tmp1->GetKey()))
        tmp1 = tmp->GetNext();
    return tmp1;
}

template <class KEY>
void Tree<KEY>::PrintTree(void)
{
    int i = 0, count = 1;
    std::queue<Node<KEY>*> myq;
    myq.push(this->root);
    while(!myq.empty())
    {
        Node<KEY> *tmp = myq.front(); myq.pop();
        if(!IS_NULL(tmp))
            cout<<" "<<tmp->GetKey()<<" ";
        else
            printf(" %3d ", 0);
        i++;
        if(i == count)
        {
            i = 0;
            count <<= 1;
            printf("\n");
        }
        if(tmp != NULL)
        {
            myq.push(tmp->GetLeft());
            myq.push(tmp->GetRight());
        }
    }
}

template <class KEY>
void Tree<KEY>::CheckList(void)
{
    uint32_t count = 1, tot = 0;
    KEY key = 0;
    Node<KEY> *tmp = this->first;
    while(!IS_NULL(tmp))
    {
        int c = tmp->GetCount();
        tot += c;
        for(set<string>::iterator it = tmp->GetData().begin(); it != tmp->GetData().end(); ++it)
        {
            cout<<count<<"> "<<tmp->GetKey()<<" = "<<*it<<" == "<<c<<"  "<<endl;
            count++;
        }
        if(key <= tmp->GetKey())
            key = tmp->GetKey();
        else
        {
            abort();
        }
        tmp = tmp->GetNext();
    }

    if(num_elements != tot)
    {
        abort();
    }

    cout<<"Number of keys : "<<num_keys<<endl<<"Number of elements : "<<num_elements<<"==="<<tot<<endl;
}

template <class KEY>
void Tree<KEY>::inorder_priv(Node<KEY> *n)
{
    if(IS_NULL(n)) return;
    inorder_priv(n->GetLeft());
    cout<<" "<<n->GetKey()<<" ";
    inorder_priv(n->GetRight());
}
template <class KEY>
void Tree<KEY>::InOrder(void)
{
    printf("\n\n");
    inorder_priv(root);
    printf("\n\n");
}

template <class KEY>
void Tree<KEY>::preorder_priv(Node<KEY> *n)
{
    if(IS_NULL(n)) return;
    cout<<" "<<n->GetKey()<<" ";
    preorder_priv(n->GetLeft());
    preorder_priv(n->GetRight());
}

template <class KEY>
void Tree<KEY>::PreOrder(void)
{
    printf("\n\n");
    preorder_priv(root);
    printf("\n\n");
}

template <class KEY>
void Tree<KEY>::postorder_priv(Node<KEY> *n)
{
    if(IS_NULL(n)) return;
    postorder_priv(n->GetRight());
    cout<<" "<<n->GetKey()<<" ";
    postorder_priv(n->GetLeft());
}

template <class KEY>
void Tree<KEY>::PostOrder(void)
{
    printf("\n\n");
    postorder_priv(root);
    printf("\n\n");
}

template <class KEY>
int Tree<KEY>::height_priv(Node<KEY> *n)
{
    if(IS_NULL(n))
        return 0;
    return 1 + MAX(height_priv(n->GetLeft()), height_priv(n->GetRight()));
}

template <class KEY>
int Tree<KEY>::Height(void)
{
    return height_priv(this->root);
}

template <class KEY>
Node<KEY>* Tree<KEY>::GetNthElement(const int nth, int *rth)
{
    if(nth >= (int)num_elements)
    {
        log_msg("nth'%d' element is greater than total num element '%u'", nth, num_elements);
        return NULL;
    }
    int tmp_count = this->first->GetCount();
    Node<KEY> *n = this->first;
    while(tmp_count <= nth)
    {
        n = n->GetFrontLink();
        tmp_count += n->GetCount();
    }
    tmp_count -= n->GetCount();
    tmp_count += n->GetData().size();
    while(tmp_count < nth)
    {
        n = n->GetNext();
        tmp_count += n->GetData().size();
    }
    tmp_count -= n->GetData().size();
    *rth = tmp_count;
    return n;
}


template <class KEY>
Node<KEY>* Tree<KEY>::GetKeyNodeAndNumber(const KEY key, int *rank)
{
    *rank = 0;
    Node<KEY>* n = FindNode(key);
    if(!IS_NULL(n))
    {
        while(!IS_NULL(n->GetPrev()))
        {
            n = n->GetPrev();
            *rank += n->GetData().size();
            if(!IS_NULL(n->GetFrontLink()))
            {
                break;
            }
        }
        if(n->GetCount() != 0)
        {
            while(!IS_NULL(n->GetBackLink()))
            {
                n = n->GetBackLink();
                *rank += n->GetCount();
            }
        }
    }
    return n;
}

template class Tree<int>;
