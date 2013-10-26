#ifndef _TREE_H_
#define _TREE_H_
#include "util.h"

#include <string>
#include <set>
using namespace std;

template <class KEY>
class Node
{
    private:
        int height;
        /*Data structure needed for tree*/
        Node *parent, *left, *right;

        /*Data structure needed for Doubly linked list*/
        Node *prev, *next;

        /*Key and Data associated with it*/
        KEY            key;
        set<string>    values;
    public:

        Node(KEY k)
        {
            height = 0;
            key = k;
            parent = left = right = prev = next = NULL;
        }

        ~Node()
        {
            key = height = 0;
            parent = left = right = prev = next = NULL;
        }

        set<string>& GetData(void) { return values; }
        
        bool SetData(string data) { return values.insert(data).second; }
        
        int GetHeight(void) { return height;}
        KEY GetKey(void) { return key; }
        int UpdateHeight(void);
        int GetBalance(void);

        void InsertBefore(Node<KEY> *node);
        void InsertAfter(Node<KEY> *node);

        void SetLeftNode(Node *left);
        void SetRightNode(Node *right);
        void SetParentNode(Node *parent) { this->parent = parent; }
        void SetNext(Node *next) { this->next = next; }
        void SetPrev(Node *prev) { this->prev = prev; }
        Node<KEY>* GetLeft(void) { return left; }
        Node<KEY>* GetRight(void) { return right; }
        Node<KEY>* GetParent(void) { return parent; }
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
        void RotateLeft(Node<KEY> *n);
        void RotateRight(Node<KEY> *n);
        void BalanceAt(Node<KEY> *n);
        
        void SetLast(Node<KEY> *n);
        void SetFirst(Node<KEY> *n);
        int CheckAndInsert(Node<KEY> *n1, Node<KEY> *n2);
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

        int GetKeys(void) { return num_keys; }
        int  Height(void);
        void PreOrder(void);
        void InOrder(void);
        void PostOrder(void);
        void PrintTree(void);
        void CheckList(void);
};

#endif
