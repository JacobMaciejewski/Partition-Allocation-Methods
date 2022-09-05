#ifndef MEMORY_FRAGMENT_H
#define MEMORY_FRAGMENT_H

#include "../headers/auxiliarry.h"

class memoryFragment
{
private:
    unsigned int size;
    bool allocated;
    memoryFragment *leftNeighbour;
    memoryFragment *rightNeighbour;
public:
    memoryFragment(unsigned int);
    memoryFragment(unsigned int, memoryFragment*, memoryFragment*);
    ~memoryFragment();
    unsigned int getSize();
    bool isAllocated();
    bool hasLeftNeighbour();
    bool hasRightNeighbour();
    memoryFragment* getLeftNeighbour();
    memoryFragment* getRightNeighbour();
    void setSize(unsigned int);
    void setAllocated(bool);
    void setLeftNeighbour(memoryFragment*);
    void setRightNeighbour(memoryFragment*);
    void findFragmentToAllocateProcess(memoryFragment**, unsigned int, bool);
    void print();
};

#endif