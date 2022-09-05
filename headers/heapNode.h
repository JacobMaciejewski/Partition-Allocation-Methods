#ifndef HEAP_NODE_H
#define HEAP_NODE_H

#include "../headers/auxiliarry.h"
using namespace std;
 
class heapNode
{
    private:
        double terminationTime;
        void *data;
        processEntry *processInfo;
    public:
        heapNode(double, processEntry*, void*);
        ~heapNode();
        double getTerminationTime();
        void *getData();
        processEntry *getProcessInfo();
        void setTerminationTime(double);
        void setData(void*);
        void setProcessInfo(processEntry*);
        bool smallerValue(heapNode*);
        bool biggerValue(heapNode*);
        bool equalValue(heapNode*);
};




#endif