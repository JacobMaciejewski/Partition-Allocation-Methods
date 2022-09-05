#include "../headers/minHeap.h"   

minHeap::minHeap() {

}

minHeap::~minHeap() {
    int heapSize = this->getSize();

    for(int i = 0; i < heapSize; i++) {
        delete heap[i];
    }
}

int minHeap::getParentIndex(int nodeIndex) {
    return (nodeIndex - 1) / 2;
}

int minHeap::getLeftChildIndex(int nodeIndex) {
    return (2*nodeIndex + 1);
}

int minHeap::getRightChildIndex(int nodeIndex) {
    return (2*nodeIndex + 2);
}

// heapifying recursively to both children
// from top to down (called when head is removed)
void minHeap::heapify_down(int nodeIndex)
{
    // get left and right child of node at index `i`
    int leftChildIndex = this->getLeftChildIndex(nodeIndex);
    int rightChildIndex = this->getRightChildIndex(nodeIndex);
    int heapSize = this->getSize();

    int smallestNodeIndex = nodeIndex;

    // compare value with the left child
    if (leftChildIndex < heapSize && heap[leftChildIndex]->smallerValue(heap[nodeIndex])) {
        smallestNodeIndex = leftChildIndex;
    }

    // compare the smallest value with the right child
    if (rightChildIndex < heapSize && heap[rightChildIndex]->smallerValue(heap[smallestNodeIndex])) {
        smallestNodeIndex = rightChildIndex;
    }

    // node has a child with a smaller value
    // swap them and call heapify down again
    if (smallestNodeIndex != nodeIndex)
    {
        heapNode *temporaryNode;

        temporaryNode = heap[nodeIndex];
        heap[nodeIndex] = heap[smallestNodeIndex];
        heap[smallestNodeIndex] = temporaryNode;

        heapify_down(smallestNodeIndex);
    }
}

// as heapify down but from bottom to top
void minHeap::heapify_up(int nodeIndex)
{
    // check if child has lower value (should be higher)
    // than the parent (not called on root-index 0)
    if (nodeIndex && heap[this->getParentIndex(nodeIndex)]->biggerValue(heap[nodeIndex]))
    {
        heapNode *temporaryNode;

        temporaryNode = heap[nodeIndex];
        heap[nodeIndex] = heap[this->getParentIndex(nodeIndex)];
        heap[this->getParentIndex(nodeIndex)] = temporaryNode;

        // call heapify-up on the parent
        heapify_up(this->getParentIndex(nodeIndex));
    }
}

unsigned int minHeap::getSize() {
    return heap.size();
}

bool minHeap::isEmpty() {
    return (this->getSize() == 0);
}

bool minHeap::haveTerminatedTill(double time) {
    if(this->isEmpty()) {
        return true;
    }else{
        return (this->top()->getTerminationTime() <= time);
    }
}

// insert key into the heap
void minHeap::push(heapNode *newHeapNode)
{
    int lastNodeIndex;
    // initially set new node as the one with the highest value
    heap.push_back(newHeapNode);

    lastNodeIndex = this->getSize() - 1;
    // heapify from lowest index to the root
    heapify_up(lastNodeIndex);
}

heapNode* minHeap::pop()
{
    heapNode* root;
    if(this->isEmpty()) {
        return NULL;
    }else{
        // get the node at the root
        root = heap[0];
        // put the lowest node at the root
        heap[0] = heap.back();
        heap.pop_back();
        // heapify from root to leaves
        this->heapify_down(0);
    }

    return root;
}

// peeks the root of the heap
// without popping it from the data structure
heapNode* minHeap::top()
{
    if(this->isEmpty()) {
        return NULL;
    }else{
        return heap[0];  
    }
}