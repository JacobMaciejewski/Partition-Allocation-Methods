#ifndef BUDDY_NODE_H
#define BUDDY_NODE_H

#include "../headers/auxiliarry.h"
using namespace std;
 
class buddyNode
{
    private:
        buddyNode *parent;
        buddyNode *leftChild;
        buddyNode *rightChild;
        unsigned int size;
        unsigned int maxAvailableSpace;
        unsigned int totalAvailableSpace;
        unsigned int internalFragmentation;
    public:
        buddyNode(unsigned int);
        ~buddyNode();
        buddyNode *getParent();
        buddyNode *getLeftChild();
        buddyNode *getRightChild();
        unsigned int getSize();
        unsigned int getMaxAvailableSpace();
        unsigned int getTotalAvailableSpace();
        unsigned int getInternalFragmentation();
        bool toBreak(unsigned int, unsigned int);
        bool isLeaf();
        bool isAllocated();
        bool canAllocate(unsigned int);
        void decrementMaxAvailableSpace();
        void decrementTotalAvailableSpace();
        void incrementInternalFragmentation();
        void setParent(buddyNode*);
        void setLeftChild(buddyNode*);
        void setRightChild(buddyNode*);
        void setSize(unsigned int);
        void setMaxAvailableSpace(unsigned int);
        void setTotalAvailableSpace(unsigned int);
        void setInternalFragmentation(unsigned int);
        void print();
};

void destroyBuddyNode(buddyNode*);
void makeLeafNode(buddyNode*);




#endif