#ifndef _TREE_H_
#define _TREE_H_
#include "util.h"

#include <string>
#include <set>
using namespace std;

/*
 * This datastructure is combination of AVL-Tree + doubly linked list + skip list
 * all in the same node. This allow us a O(log(n)) access to any element.
 * */

template <class KEY>
class Node
{
    private:
        int height;
        /*Data structure needed for tree*/
        Node *parent, *left, *right;

        /*Data structure needed for Doubly linked list*/
        Node *prev, *next;

        /* data structure which allows to print from random 10 -100.
         * Used in implementing zrange*/
        Node *front_link, *back_link;
        int count;

        /*Key and Data associated with it*/
        KEY            key;
        set<string>    values;
    public:

        Node(KEY k)
        {
            count = height = 0;
            key = k;
            parent = left = right = prev = next = NULL;
            front_link = back_link = NULL;
        }

        ~Node()
        {
            key = height = 0;
            parent = left = right = prev = next = NULL;
        }

        /* Function to get and set data*/
        bool SetData(string data) { if(values.insert(data).second == true) {  UpdateLinks(); return true;} return false; }
        set<string>& GetData(void) { return values; }
        int GetCount(void) { return count; }
        KEY GetKey(void) { return key; }

        /* Function the maintain the skip list data structure*/
        void UpdateLinks(void);
        Node<KEY>* GetFrontLink(void) { return front_link; }
        Node<KEY>* GetBackLink(void) { return back_link; }

        /* Function used to maintain the tree*/
        int GetHeight(void) { return height;}
        int UpdateHeight(void);
        int GetBalance(void);

        void SetLeftNode(Node *left);
        void SetRightNode(Node *right);
        void SetParentNode(Node *parent) { this->parent = parent; }
        void SetNext(Node *next) { this->next = next; }
        void SetPrev(Node *prev) { this->prev = prev; }
        Node<KEY>* GetLeft(void) { return left; }
        Node<KEY>* GetRight(void) { return right; }
        Node<KEY>* GetParent(void) { return parent; }

        /* Function to maintain the Doubly linked list*/
        void InsertBefore(Node<KEY> *node);
        void InsertAfter(Node<KEY> *node);
        Node<KEY>* GetPrev(void) { return prev; }
        Node<KEY>* GetNext(void) { return next; }
};

template <class KEY>
class Tree
{
    private:
        uint32_t num_keys;
        uint32_t num_elements;
        /*The root of the tree node*/
        Node<KEY> *root;

        /*The first and the last node of the doubly linked list*/
        Node<KEY> *first, *last;

    private:  /*Priv functions*/

        /* Functions used in balancing the tree when there is a imbalance*/
        void RotateLeft(Node<KEY> *n);
        void RotateRight(Node<KEY> *n);
        void BalanceAt(Node<KEY> *n);

        /* Get the first and last node in the doubly linked list*/
        void SetLast(Node<KEY> *n);
        void SetFirst(Node<KEY> *n);

        /*debug function*/
        static int  height_priv(Node<KEY> *n);
        static void preorder_priv(Node<KEY> *n);
        static void inorder_priv(Node<KEY> *n);
        static void postorder_priv(Node<KEY> *n);
    public:
        Tree()
        {
            root = first = last = NULL;
            num_keys= 0;
            num_elements = 0;
        }
        int GetNumElements(void) { return num_elements; }
        ~Tree();
        /* This function adds a given node (the node has a key) to the tree.
         * The different cases handled.
         * 1. Key Not present: Adds the node to the tree and returns the added node.
         * 2. Key present: returns the old node with same key and does not add the new node.
         * */
        Node<KEY>* AddNode(const KEY n, const string data);

        /* Finds a given node (with key set) in the tree.
         * if found returns the node else NULL
         * */
        Node<KEY>* FindNode(KEY key);
        Node<KEY>* FindNear(KEY key);

        Node<KEY>* GetKeyNodeAndNumber(const KEY key, int *rank);

        /* This function finds the nth element in the doubly linked list.
         * Uses the skip list data structure*/
        Node<KEY>* GetNthElement(const int nth, int *rth);
        int GetKeys(void) { return num_keys; }

        /*Debug functions*/
        int  Height(void);
        void PreOrder(void);
        void InOrder(void);
        void PostOrder(void);
        void PrintTree(void);
        void CheckList(void);
};

#endif
