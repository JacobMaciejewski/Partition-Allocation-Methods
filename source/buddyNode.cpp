#include "../headers/buddyNode.h"

buddyNode::buddyNode(unsigned int size) {
    this->setLeftChild(NULL);
    this->setRightChild(NULL);
    this->setParent(NULL);
    this->setSize(size);
    // initially the whole space is available
    this->setMaxAvailableSpace(size);
    this->setInternalFragmentation(0);
    this->setTotalAvailableSpace(size);
}

buddyNode::~buddyNode() {
    if(!this->isLeaf()) {
        delete this->getLeftChild();
        delete this->getRightChild();
    }
}

buddyNode *buddyNode::getParent(){
    return this->parent;
}

buddyNode *buddyNode::getLeftChild() {
    return this->leftChild;
}

buddyNode *buddyNode::getRightChild() {
    return this->rightChild;
}

unsigned int buddyNode::getSize() {
    return this->size;
}

unsigned int buddyNode::getMaxAvailableSpace() {
    return this->maxAvailableSpace;
}

unsigned int buddyNode::getTotalAvailableSpace() {
    return this->totalAvailableSpace;
}

unsigned int buddyNode::getInternalFragmentation() {
    return this->internalFragmentation;
}

// only empty leaf nodes can be broken
// if requested space is less than half of the available
// and new break won't produce too small fragments
bool buddyNode::toBreak(unsigned int processSize, unsigned int minFragmentSize) {
    unsigned int halfNodeSizeCeil = (unsigned int)(ceil(this->getSize() / 2));
    unsigned int halfNodeSizeFloor = (unsigned int)(floor(this->getSize() / 2));

    return (this->isLeaf() && !this->isAllocated() && (processSize <= halfNodeSizeCeil && halfNodeSizeFloor >= minFragmentSize));
}

// fragment is empty
// when its total space is equal to the most available space
bool buddyNode::isLeaf() {
    return (this->getLeftChild() == NULL && this->getRightChild() == NULL);
}

// is allocated when the available space is less than total space
bool buddyNode::isAllocated() {
    return (this->getMaxAvailableSpace() != this->getSize());
}

// can allocate only if internal node has big enough free space in children subspaces
// or is a free fragment with enough space (leaf node)
bool buddyNode::canAllocate(unsigned int spaceToAllocate) {
    bool enoughSpace = this->getMaxAvailableSpace() >= spaceToAllocate;

    return (!this->isLeaf()) ? enoughSpace : (!this->isAllocated() && enoughSpace);
}

// in the case you are an internal node
// max available space for your subtree
// is the size of the biggest available space in children subtrees
// allocated children have no available space
void buddyNode::decrementMaxAvailableSpace() {
    if(!this->isLeaf()) {
        buddyNode *leftChild = this->getLeftChild();
        buddyNode *rightChild = this->getRightChild();
        int leftChildAvailableSpace = (leftChild->isLeaf() && leftChild->isAllocated()) ? 0 : leftChild->getMaxAvailableSpace();
        int rightChildAvailableSpace = (rightChild->isLeaf() && rightChild->isAllocated()) ? 0 : rightChild->getMaxAvailableSpace();

        this->setMaxAvailableSpace(max(leftChildAvailableSpace, rightChildAvailableSpace));
    }
}

// update the total available space of an upper level fragment
// and set it as sum of the available spaces in two subtrees (sub fragments)
void buddyNode::decrementTotalAvailableSpace() {
    if(!this->isLeaf()) {
        buddyNode *leftChild = this->getLeftChild();
        buddyNode *rightChild = this->getRightChild();

        this->setTotalAvailableSpace(leftChild->getTotalAvailableSpace() + rightChild->getTotalAvailableSpace());
    }
}

// update the total internal fragmentation of an upper level fragment
// and set it as sum of internal fragmentations in two subtrees (sub fragments)
void buddyNode::incrementInternalFragmentation() {
    if(!this->isLeaf()) {
        buddyNode *leftChild = this->getLeftChild();
        buddyNode *rightChild = this->getRightChild();

        this->setInternalFragmentation(leftChild->getInternalFragmentation() + rightChild->getInternalFragmentation());
    }
}


void buddyNode::setParent(buddyNode* parent) {
    this->parent = parent;
}
void buddyNode::setLeftChild(buddyNode* leftChild) {
    this->leftChild = leftChild;
}

void buddyNode::setRightChild(buddyNode* rightChild) {
    this->rightChild = rightChild;
}

void buddyNode::setSize(unsigned int size) {
    this->size = size;
}

void buddyNode::setMaxAvailableSpace(unsigned int maxAvailableSpace) {
    this->maxAvailableSpace = maxAvailableSpace;
}

void buddyNode::setTotalAvailableSpace(unsigned int totalAvailableSpace) {
    this->totalAvailableSpace = totalAvailableSpace;
}

void buddyNode::setInternalFragmentation(unsigned int internalFragmentation) {
    this->internalFragmentation = internalFragmentation;
}

void buddyNode::print() {

    if(this->isLeaf()) {
        unsigned int leafSize = this->getSize();
        unsigned int leafInternalFragmentation = this->getInternalFragmentation();

        if(this->isAllocated()) {
            cout << "Allocated[" << leafSize - leafInternalFragmentation << "MB]";
            cout << " | Fragmented[" << leafInternalFragmentation << "MB]" << endl;
        }else {
            cout << "Free[" << this->getSize() << "MB]" << endl;
        }
        
    }else {
        this->getLeftChild()->print();
        this->getRightChild()->print();
    }
}

void destroyBuddyNode(buddyNode *targetNode) {
    targetNode->setLeftChild(NULL);
    targetNode->setRightChild(NULL);
    delete targetNode;
}

void makeLeafNode(buddyNode *targetNode) {
    unsigned int targetFragmentSize = targetNode->getSize();

    targetNode->setTotalAvailableSpace(targetFragmentSize);
    targetNode->setMaxAvailableSpace(targetFragmentSize);
    targetNode->setInternalFragmentation(0);
    targetNode->setLeftChild(NULL);
    targetNode->setRightChild(NULL);
}