#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <list>
#include <iostream>
template <typename KeyType, typename ValueType>
class TreeMultimap
{
  public:
    class Iterator
    {
      public:
        Iterator()                                  //default constructor
        :valid(false)
        {}
        
        Iterator(typename std::list<ValueType>::iterator begin, typename std::list<ValueType>::iterator end)
        :m_begin(begin), m_end(end), valid(true)
        {}

        ValueType& get_value() const
        {
            if (is_valid())                         //if is_valid() is true
            {
                return *m_begin;
            }
            throw 1;                     //return reference to item referred to by iterator
        }

        bool is_valid() const
        {
            return valid;                           //Return true if default constructor not called
        }

        void advance()
        {
            if (is_valid())                         //If valid (default constructor not called), execute body
            {
                m_begin++;
                if (m_begin == m_end)
                {
                    valid = false;
                }
            }
        }

      private:
        typename std::list<ValueType>::iterator m_begin;
        typename std::list<ValueType>::iterator m_end;
        bool valid;
    };

    TreeMultimap()
    :empty(true)
    {
        root = nullptr;
    }

    ~TreeMultimap()
    {
       freeTree(root);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        if (empty)
        {
            root = new Node;                        //If tree is empty, create new node and assign root pointer
            root->key = key;
            root->values.push_back(value);
            empty = false;
            return;
        }
        Node* curr = root;
        for (;;)
        {
            if (curr->key == key)                             //Check if root node has the same key
            {
                curr->values.push_back(value);
                return;                               //If so, nothing is done
            }
            
            if (curr->key > key)                         //Left branch compatibility test
            {
                if (curr->leftChild != nullptr)                 //if left child already, move to left
                    curr = curr->leftChild;
                else
                {
                    Node* newNode = new Node;                   //Create new node and assign leftChild pointer to it
                    newNode->key = key;
                    newNode->values.push_back(value);
                    curr->leftChild = newNode;
                    curr->leftChild->leftChild = nullptr;
                    curr->leftChild->rightChild = nullptr;
                    
                    return;
                }
            }
            
            else if (curr->key < key)                         //Right branch compatibility
            {
                if (curr->rightChild != nullptr)                //if right child already, move to right
                    curr = curr->rightChild;
                else
                {
                    Node* newNode = new Node;                   //Create new node and assign rightChild pointer to it
                    newNode->key = key;
                    newNode->values.push_back(value);
                    curr->rightChild = newNode;
                    curr->rightChild->leftChild = nullptr;
                    curr->rightChild->rightChild = nullptr;
                    return;
                }
            }
        }
    }

    Iterator find(const KeyType& key) const
    {
        Node* curr = root;
        while (curr != nullptr)                                 //Starting at root pointer, if node exists
        {
            if (key == curr->key)                               //If key matches, return iterator, passing in list
                return Iterator(curr->values.begin(), curr->values.end());
            else if (key < curr->key)                           //Move down left branch if key is less than parent key
                curr = curr->leftChild;
            else if (key > curr->key)                           //Move down right branch if key is greater than parent key
                curr = curr->rightChild;
        }
        return Iterator();                                      //Return default iterator if node not found (invalid)
    }

  private:
    struct Node
    {
        KeyType key;
        std::list<ValueType> values;
        Node* leftChild;
        Node* rightChild;
        
    };
    
    Node* root;
    bool empty;
    
    //Delete all nodes of tree (used in destructor)
    void freeTree(Node* curr)
    {
        if (curr == nullptr)
            return;
        freeTree(curr->rightChild);
        freeTree(curr->leftChild);
        delete curr;
    }
};

#endif // TREEMULTIMAP_INCLUDED

