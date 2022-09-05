#ifndef PROCESS_ENTRY_H
#define PROCESS_ENTRY_H

#include "../headers/auxiliarry.h"
using namespace std;
 
class processEntry
{
    private:
        unsigned int identifier;
        unsigned int size;
        double arrivalTime;
        double activationTime;
        double lifeTime;
    public:
        processEntry(unsigned int, unsigned int, double, double, double);
        ~processEntry();
        unsigned int getID();
        unsigned int getSize();
        double getArrivalTime();
        double getActivationTime();
        double getLifeTime();
        double getTerminationTime();
        double getWaitingTime();
        void setID(unsigned int);
        void setSize(unsigned int);
        void setArrivalTime(double);
        void setActivationTime(double);
        void setLifeTime(double);
        string print();
};

#endif