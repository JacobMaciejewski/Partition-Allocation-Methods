#ifndef BUDDY_TREE_H
#define BUDDY_TREE_H

#include "../headers/auxiliarry.h"
using namespace std;
 
class buddyTree
{
    private:
        buddyNode *memory;
        unsigned int memorySize;
        unsigned int minFragmentSize;
        unsigned int allocatedProcesses;
    public:
        buddyTree(unsigned int, unsigned int);
        ~buddyTree();
        unsigned int getMemorySize();
        unsigned int getMinFragmentSize();
        unsigned int getAllocatedProcesses();
        unsigned int getMaxAvailableSpace();
        unsigned int getTotalAvailableSpace();
        unsigned int getInternalFragmentation();
        bool canAllocate(unsigned int);
        buddyNode *getMemory();
        buddyNode *allocateFragmentForProcess(unsigned int);
        buddyNode *allocateFragmentForProcess(unsigned int, buddyNode*);
        void setMemory(buddyNode*);
        void setMemorySize(unsigned int);
        void setMinFragmentSize(unsigned int);
        void setAllocatedProcesses(unsigned int);
        void incrementAllocatedProcesses();
        void decrementAllocatedProcesses();
        void freeProcessMemory(buddyNode*);
        void print();
};




#endif