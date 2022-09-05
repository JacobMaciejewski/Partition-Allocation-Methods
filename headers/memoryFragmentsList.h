#ifndef MEMORY_FRAGMENTS_LIST_H
#define MEMORY_FRAGMENTS_LIST_H

#include "../headers/auxiliarry.h"

class memoryFragmentsList
{
private:
    unsigned int memorySize;
    unsigned int allocatedProcesses;
    memoryFragment *memory;
public:
    memoryFragmentsList(unsigned int);
    ~memoryFragmentsList();
    unsigned int getMemorySize();
    unsigned int getAllocatedProcesses();
    bool isEmpty();
    memoryFragment* getMemory();
    void setMemorySize(unsigned int);
    void setAllocatedProcesses(unsigned int);
    void incrementAllocatedProcesses();
    void decrementAllocatedProcesses();
    void setMemory(memoryFragment*);
    void freeProcessMemory(memoryFragment*);
    void print();
    memoryFragment* findFragmentToAllocateProcess(unsigned int, bool);
    memoryFragment* getAllocatedFragment(unsigned int, memoryFragment*);
};

#endif