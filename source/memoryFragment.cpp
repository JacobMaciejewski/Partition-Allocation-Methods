#include "../headers/memoryFragment.h"    
   
memoryFragment::memoryFragment(unsigned int size) {
    this->setSize(size);
    this->setLeftNeighbour(NULL);
    this->setRightNeighbour(NULL);
}

memoryFragment::memoryFragment(unsigned int size, memoryFragment *leftNeighbour, memoryFragment *rightNeighbour) {
    this->setSize(size);
    this->setLeftNeighbour(leftNeighbour);
    this->setRightNeighbour(rightNeighbour);
}

memoryFragment::~memoryFragment(){
    // in case there are fragment left to the right
    // delete them first
    if(this->hasRightNeighbour()){
        delete this->getRightNeighbour();
    }
}

unsigned int memoryFragment::getSize() {
    return this->size;
}

bool memoryFragment::isAllocated() {
    return this->allocated;
}

bool memoryFragment::hasLeftNeighbour() {
    return (this->getLeftNeighbour() != NULL);
}

bool memoryFragment::hasRightNeighbour() {
    return (this->getRightNeighbour() != NULL);
}

memoryFragment* memoryFragment::getLeftNeighbour() {
    return this->leftNeighbour;
}

memoryFragment* memoryFragment::getRightNeighbour() {
    return this->rightNeighbour;
}

void memoryFragment::findFragmentToAllocateProcess(memoryFragment **processMemoryFragment, unsigned int processMemorySize, bool bestFit) {

    memoryFragment *currentCandidateFragment = *processMemoryFragment;
    memoryFragment *biggerFragment, *smallerFragment;
    unsigned int currentFragmentSize = this->getSize();
    unsigned int candidateFragmentSize;

    // process can be stored in current fragment
    if(currentFragmentSize >= processMemorySize && !this->isAllocated()) {

        if(currentCandidateFragment != NULL) { // compare current candidate to the traversed fragment
            candidateFragmentSize = currentCandidateFragment->getSize();

            // find bigger and smaller fragment between candidate and current one
            biggerFragment = (candidateFragmentSize >= currentFragmentSize) ?  currentCandidateFragment : this;
            smallerFragment = (candidateFragmentSize < currentFragmentSize) ? currentCandidateFragment : this;
            
            *processMemoryFragment = bestFit ? biggerFragment : smallerFragment;

        }else { // no candidate chosen yet, set current fragment
            *processMemoryFragment = this;
        }

    }

    // got to the end of the memory
    // return results
    if(!this->hasRightNeighbour()) {
        return;
    }else{
        this->getRightNeighbour()->findFragmentToAllocateProcess(processMemoryFragment, processMemorySize, bestFit);
    }
}

void memoryFragment::setSize(unsigned int size) {
    this->size = size;
}

void memoryFragment::setAllocated(bool allocated) {
    this->allocated = allocated;
}

void memoryFragment::setLeftNeighbour(memoryFragment *leftNeighbour) {
    this->leftNeighbour = leftNeighbour;
}

void memoryFragment::setRightNeighbour(memoryFragment *rightNeighbour) {
    this->rightNeighbour = rightNeighbour;
}

void memoryFragment::print() {
    string type = this->isAllocated() ? "Allocated" : "Free";

    cout << type << " " << this->getSize() << "MB" << endl;

    if(this->hasRightNeighbour()) {
        this->getRightNeighbour()->print();
    }
}