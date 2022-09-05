#include "../headers/heapNode.h"  

heapNode::heapNode(double terminationTime, processEntry *processInfo, void *data) {
    this->setData(data);
    this->setProcessInfo(processInfo);
    this->setTerminationTime(terminationTime);
}

heapNode::~heapNode() {
    delete this->getProcessInfo();
}

double heapNode::getTerminationTime() {
    return this->terminationTime;
}

processEntry *heapNode::getProcessInfo() {
    return this->processInfo;
}

void *heapNode::getData() {
    return this->data;
}

void heapNode::setTerminationTime(double terminationTime) {
    this->terminationTime = terminationTime;
}

void heapNode::setData(void *data) {
    this->data = data;
}

void heapNode::setProcessInfo(processEntry *processInfo) {
    this->processInfo = processInfo;
}

bool heapNode::smallerValue(heapNode *otherNode) {
    double thisTerminationTime = this->getTerminationTime();
    double otherTerminationTime = otherNode->getTerminationTime();

    return (thisTerminationTime < otherTerminationTime);
}

bool heapNode::biggerValue(heapNode *otherNode) {
    double thisTerminationTime = this->getTerminationTime();
    double otherTerminationTime = otherNode->getTerminationTime();

    return (thisTerminationTime > otherTerminationTime);
}

bool heapNode::equalValue(heapNode *otherNode) {
    return (!this->biggerValue(otherNode) && !this->smallerValue(otherNode));
}