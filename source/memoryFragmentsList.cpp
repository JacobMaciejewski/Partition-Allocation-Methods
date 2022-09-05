#include "../headers/memoryFragmentsList.h"    
    
memoryFragmentsList::memoryFragmentsList(unsigned int memorySize){
    this->setMemorySize(memorySize);
    // initialize single fragment node for the whole memory
    memory = new memoryFragment(memorySize);
    this->setMemory(memory);
    this->setAllocatedProcesses(0);
}


memoryFragmentsList::~memoryFragmentsList() {
    delete this->getMemory();
}

unsigned int memoryFragmentsList::getMemorySize(){
    return this->memorySize;
}

unsigned int memoryFragmentsList::getAllocatedProcesses() {
    return this->allocatedProcesses;
}

bool memoryFragmentsList::isEmpty() {
    return (this->getAllocatedProcesses() == 0);
}

memoryFragment* memoryFragmentsList::getMemory() {
    return this->memory;
}

void memoryFragmentsList::setMemorySize(unsigned int memorySize) {
    this->memorySize = memorySize;
}

void memoryFragmentsList::setMemory(memoryFragment *memory) {
    this->memory = memory;
}

void memoryFragmentsList::setAllocatedProcesses(unsigned int allocatedProcesses) {
    this->allocatedProcesses = allocatedProcesses;
}

void memoryFragmentsList::incrementAllocatedProcesses() {
    this->setAllocatedProcesses(this->getAllocatedProcesses() + 1);
}

void memoryFragmentsList::decrementAllocatedProcesses() {
    this->setAllocatedProcesses(this->getAllocatedProcesses() - 1);
}

// deallocates the virtual process stored in the given fragment
// merges it with neighbouring free fragments
// new fragment points to the initially allocated fragment
void memoryFragmentsList::freeProcessMemory(memoryFragment *processMemoryFragment) {

    memoryFragment *leftNeighbour, *rightNeighbour;
    memoryFragment *newLeftNeighbour = NULL;
    memoryFragment *newRightNeighbour = NULL;
    // should the new fragment be at the beginning of the memory
    bool setHead = false;
    // the new free fragment will be at least as big as the previously
    // allocated memory for the given process
    unsigned int newFragmentSize = processMemoryFragment->getSize();

    if(processMemoryFragment->hasLeftNeighbour()) {
        leftNeighbour = processMemoryFragment->getLeftNeighbour();

        // left neighbour is free, merge
        if(!leftNeighbour->isAllocated()) {
            
            // new left neighbour is the old left neighbour of our left neighbour
            newLeftNeighbour = leftNeighbour->getLeftNeighbour();
            // we inherit the size of the left neighbour
            newFragmentSize += leftNeighbour->getSize();

            // left free neighbour is at the start of the memory
            // need to set new fragment as the head of memory
            if(!leftNeighbour->hasLeftNeighbour()) {
                setHead = true;
            }

            // free space merged, delete old entry
            destroyFragment(leftNeighbour);

        }else{
            newLeftNeighbour = leftNeighbour;
        }
    }

    if(processMemoryFragment->hasRightNeighbour()) {
        rightNeighbour = processMemoryFragment->getRightNeighbour();

        // left neighbour is free, merge
        if(!rightNeighbour->isAllocated()) {
            
            // new left neighbour is the old left neighbour of our left neighbour
            newRightNeighbour = rightNeighbour->getRightNeighbour();
            // we inherit the size of the left neighbour
            newFragmentSize += rightNeighbour->getSize();
            // free space merged, delete old entry
            destroyFragment(rightNeighbour);

        }else{
            newRightNeighbour = rightNeighbour;
        }
    }

    // new fragment is free and its size
    // has been incremented by the size of the neighbouring free spaces
    processMemoryFragment->setAllocated(false);
    processMemoryFragment->setSize(newFragmentSize);
    processMemoryFragment->setLeftNeighbour(newLeftNeighbour);
    processMemoryFragment->setRightNeighbour(newRightNeighbour);

    // new free fragment is on the right of the previous left neighbour of its left neighbour
    if(newLeftNeighbour != NULL){
        newLeftNeighbour->setRightNeighbour(processMemoryFragment);
    }

    // new free fragment is on the left of the previous right neighbour of its right neighbour
    if(newRightNeighbour != NULL){
        newRightNeighbour->setLeftNeighbour(processMemoryFragment);
    }

    // new free fragment is at the beginning of the memory space
    if(setHead){
        this->setMemory(processMemoryFragment);
    }

    this->decrementAllocatedProcesses();
}

void memoryFragmentsList::print() {
    cout << "Memory Size[" << this->getMemorySize() << "]" << endl;
    cout << "Allocated Processes[" << this->getAllocatedProcesses() << "]" << endl << endl;
    cout << "FRAGMENTS: " << endl;
    if(!this->isEmpty()) {
        this->getMemory()->print();
    }
}

// traversing the memory fragments
// looking for the appopriate one to store the given memory size
// according to best/worst fit (true/false in second argument) algorithm policy 
// if no available space found, return NULL
memoryFragment* memoryFragmentsList::findFragmentToAllocateProcess(unsigned int processMemorySize, bool bestFit) {
    memoryFragment *processMemoryFragment = NULL;
    this->getMemory()->findFragmentToAllocateProcess(&processMemoryFragment, processMemorySize, bestFit);
    return processMemoryFragment;
}

// function constructs a new allocated fragment, connects it with the existing memory
// returns a pointer to the new fragment where the process is now stored
memoryFragment *memoryFragmentsList::getAllocatedFragment(unsigned int processMemorySize, memoryFragment *freeFragment) {
    unsigned int freeFragmentSize = freeFragment->getSize();
    unsigned int allocatedFragmentSize = processMemorySize;
    unsigned int remainingFragmentSize = freeFragmentSize - allocatedFragmentSize;

    // new fragment that will be connected with the right neighbour of the
    // previously free fragment
    memoryFragment *remainingFragment;
    memoryFragment *rightNeighbour = freeFragment->getRightNeighbour();

    // free fragment is now allocated and its size is equal
    // to the size of the process
    memoryFragment *allocatedFragment = freeFragment;
    allocatedFragment->setSize(allocatedFragmentSize);
    allocatedFragment->setAllocated(true);

    // if we have space left, we construct a new free fragment on the right
    // of the allocated one
    if(remainingFragmentSize > 0) {
        remainingFragment = new memoryFragment(remainingFragmentSize);
        remainingFragment->setLeftNeighbour(allocatedFragment);
        remainingFragment->setRightNeighbour(rightNeighbour);
        remainingFragment->setAllocated(false);

        // connect the newly allocated fragment with the remaining free one
        allocatedFragment->setRightNeighbour(remainingFragment);
    }else {
        // remaining fragment not existent, set to allocated fragment
        remainingFragment = allocatedFragment;
    }

    // if previously free fragment had a right neighbour
    // connect him with the new remaining free space or the allocated fragment
    if(rightNeighbour != NULL) {
        rightNeighbour->setLeftNeighbour(remainingFragment);
    }

    this->incrementAllocatedProcesses();
    return allocatedFragment;
}