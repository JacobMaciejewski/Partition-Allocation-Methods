#include "../headers/processEntry.h" 

processEntry::processEntry(unsigned int processID, unsigned int processSize, double processArrivalTime, double processLifeTime, double processActivationTime) {
    this->setID(processID);
    this->setSize(processSize);
    this->setArrivalTime(processArrivalTime);
    this->setLifeTime(processLifeTime);
    this->setActivationTime(processActivationTime);
}

processEntry::~processEntry() {

}

unsigned int processEntry::getID() {
    return this->identifier;
}

unsigned int processEntry::getSize() {
    return this->size;
}

double processEntry::getArrivalTime() {
    return this->arrivalTime;
}

double processEntry::getActivationTime() {
    return this->activationTime;
}

double processEntry::getLifeTime() {
    return this->lifeTime;
}

double processEntry::getTerminationTime() {
    return (this->getActivationTime() + this->getLifeTime());
}

double processEntry::getWaitingTime() {
    return (this->getActivationTime() - this->getArrivalTime());
}


void processEntry::setID(unsigned int identifier) {
    this->identifier = identifier;
}

void processEntry::setSize(unsigned int size) {
    this->size = size;
}

void processEntry::setArrivalTime(double arrivalTime) {
    this->arrivalTime = arrivalTime;
}

void processEntry::setActivationTime(double activationTime) {
    this->activationTime = activationTime;
}

void processEntry::setLifeTime(double lifeTime) {
    this->lifeTime = lifeTime;
}


string processEntry::print() {
    unsigned int pID = this->getID();
    unsigned int pSize = this->getSize();
    double pArrivalTime = this->getArrivalTime();
    double pWaitingTime = this->getWaitingTime();
    double pTerminationTime = this->getTerminationTime();

    return "PID[" + to_string(pID) + "] Size[" + to_string(pSize) + "] Arrival[" + to_string(pArrivalTime) + "] Waiting[" + to_string(pWaitingTime) + "] Termination[" + to_string(pTerminationTime) + "]\n";
}