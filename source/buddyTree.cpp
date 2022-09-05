#include "../headers/buddyTree.h"

buddyTree::buddyTree(unsigned int memorySize, unsigned int minFragmentSize) {
    buddyNode *memory = new buddyNode(memorySize);
    this->setMemorySize(memorySize);
    this->setMinFragmentSize(minFragmentSize);
    this->setAllocatedProcesses(0);
    this->setMemory(memory);
}

buddyTree::~buddyTree() {
    delete this->getMemory();
}

unsigned int buddyTree::getMemorySize() {
    return this->memorySize;
}

unsigned int buddyTree::getMinFragmentSize() {
    return this->minFragmentSize;
}

unsigned int buddyTree::getAllocatedProcesses() {
    return this->allocatedProcesses;
}

unsigned int buddyTree::getMaxAvailableSpace() {
    return this->getMemory()->getMaxAvailableSpace();
}

unsigned int buddyTree::getTotalAvailableSpace() {
    return this->getMemory()->getTotalAvailableSpace();
}

unsigned int buddyTree::getInternalFragmentation() {
    return this->getMemory()->getInternalFragmentation();
}

bool buddyTree::canAllocate(unsigned int size) {
    return (this->getMemory()->canAllocate(size));
}

buddyNode *buddyTree::getMemory() {
    return this->memory;
}

buddyNode *buddyTree::allocateFragmentForProcess(unsigned int processSize) {
    return this->canAllocate(processSize) ? this->allocateFragmentForProcess(processSize, this->getMemory()) : NULL;
}

buddyNode *buddyTree::allocateFragmentForProcess(unsigned int processSize, buddyNode *candidateNode) {
    buddyNode *processFragment;

    // fragment doesn't exist
    if(candidateNode == NULL) {
        return NULL;
    }

    // there is no big enough free fragment
    // in current subspace
    if(!candidateNode->canAllocate(processSize)) {
        return NULL;
    }

    // left child cannot allocate process
    if((processFragment = this->allocateFragmentForProcess(processSize, candidateNode->getLeftChild())) == NULL){
        // right child cannot allocate process
        if((processFragment = this->allocateFragmentForProcess(processSize, candidateNode->getRightChild())) == NULL){

            // break current node and look for child that can store process
            if(candidateNode->toBreak(processSize, this->getMinFragmentSize())) {
                buddyNode *leftChild, *rightChild;

                leftChild = new buddyNode((unsigned int)ceil(((double)candidateNode->getSize()) / 2.0));
                rightChild = new buddyNode((unsigned int)floor(((double)candidateNode->getSize()) / 2.0));
                leftChild->setParent(candidateNode);
                rightChild->setParent(candidateNode);

                candidateNode->setLeftChild(leftChild);
                candidateNode->setRightChild(rightChild);

                processFragment = this->allocateFragmentForProcess(processSize, leftChild);
            }else{
                // cannot break, target node found
                processFragment = candidateNode;
                // no more available space in allocated fragment
                processFragment->setTotalAvailableSpace(0);
                // add new processes to current fragment
                processFragment->setMaxAvailableSpace(processFragment->getMaxAvailableSpace() - processSize);
                processFragment->setInternalFragmentation(processFragment->getMaxAvailableSpace());
                this->incrementAllocatedProcesses();
            }
        }
    }

    // new available space is the min of left or right child available space
    // if node allocated, is the remaining available space
    candidateNode->decrementMaxAvailableSpace();
    candidateNode->decrementTotalAvailableSpace();
    candidateNode->incrementInternalFragmentation();
    return processFragment;
}

void buddyTree::freeProcessMemory(buddyNode *targetFragment) {
    buddyNode *currentFragment = targetFragment;
    buddyNode *currentLeftChild, *currentRightChild;
    bool leftChildEmpty, rightChildEmpty;
    unsigned int newMaxAvailableSpace, newTotalAvailableSpace, newInternalFragmentation;

    while(currentFragment != NULL) {

        // allocated fragment
        // free the process
        if(currentFragment->isLeaf()){
            makeLeafNode(currentFragment);
        }else{ // fragmented memory space
            currentLeftChild = currentFragment->getLeftChild();
            currentRightChild = currentFragment->getRightChild();

            leftChildEmpty = (currentLeftChild == NULL) ? false : (currentLeftChild->isLeaf() && !currentLeftChild->isAllocated());
            rightChildEmpty = (currentRightChild == NULL) ? false : (currentRightChild->isLeaf() && !currentRightChild->isAllocated());

            // both subspaces empty, merge them
            if(leftChildEmpty && rightChildEmpty) {
                destroyBuddyNode(currentLeftChild);
                destroyBuddyNode(currentRightChild);

                makeLeafNode(currentFragment);
            }else { // memory partially empty

                if(leftChildEmpty) { //only left subspace empty
                    newMaxAvailableSpace = currentLeftChild->getSize();
                    newTotalAvailableSpace = currentLeftChild->getSize() + currentRightChild->getTotalAvailableSpace();
                    newInternalFragmentation = currentRightChild->getInternalFragmentation();
                }else if(rightChildEmpty){ //only right subspace empty
                    newMaxAvailableSpace = currentRightChild->getSize();
                    newTotalAvailableSpace = currentRightChild->getSize() + currentLeftChild->getTotalAvailableSpace();
                    newInternalFragmentation = currentLeftChild->getInternalFragmentation();
                }else{ // no child is empty, just update memory values
                    newMaxAvailableSpace = max(currentRightChild->getMaxAvailableSpace(), currentLeftChild->getMaxAvailableSpace());
                    newTotalAvailableSpace = currentRightChild->getTotalAvailableSpace() + currentLeftChild->getTotalAvailableSpace();
                    newInternalFragmentation = currentLeftChild->getInternalFragmentation() + currentRightChild->getInternalFragmentation();
                }
                
                currentFragment->setMaxAvailableSpace(newMaxAvailableSpace);
                currentFragment->setTotalAvailableSpace(newTotalAvailableSpace);
                currentFragment->setInternalFragmentation(newInternalFragmentation);
            }
        }
        currentFragment = currentFragment->getParent();
    }
    this->decrementAllocatedProcesses();
}

void buddyTree::setMemorySize(unsigned int memorySize) {
    this->memorySize = memorySize;
}

void buddyTree::setMemory(buddyNode *memory) {
    this->memory = memory;
}

void buddyTree::setMinFragmentSize(unsigned int minFragmentSize) {
    this->minFragmentSize = minFragmentSize;
}

void buddyTree::setAllocatedProcesses(unsigned int allocatedProcesses) {
    this->allocatedProcesses = allocatedProcesses;
}

void buddyTree::incrementAllocatedProcesses() {
    this->setAllocatedProcesses(this->getAllocatedProcesses() + 1);
}

void buddyTree::decrementAllocatedProcesses() {
    this->setAllocatedProcesses(this->getAllocatedProcesses() - 1);
}

void buddyTree::print() {

    cout << endl << "Memory Size[" << this->getMemorySize() << "]" << endl;
    cout << "Total Available Space[" << this->getTotalAvailableSpace() << "]" << endl;
    cout << "Biggest Available Fragment[" << this->getMaxAvailableSpace() << "]" << endl;
    cout << "Total Internal Fragmentation[" << this->getInternalFragmentation() << "]" << endl;
    cout << "Allocated Processes[" << this->getAllocatedProcesses() << "]" << endl << endl;
    cout << "Fragments: " << endl;

    this->getMemory()->print();
}