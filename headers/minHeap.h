#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include "../headers/auxiliarry.h"
using namespace std;
 
class minHeap
{
    private:
        vector<heapNode*> heap;
    public:
        minHeap();
        ~minHeap();
        int getParentIndex(int);
        int getLeftChildIndex(int);
        int getRightChildIndex(int);
        void heapify_down(int);
        void heapify_up(int nodeIndex);
        unsigned int getSize();
        bool isEmpty();
        bool haveTerminatedTill(double);
        void push(heapNode*);
        heapNode* pop();
        heapNode* top();
};




#endif