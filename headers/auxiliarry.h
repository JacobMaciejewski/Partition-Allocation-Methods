#ifndef AUXILIARRY_H
#define AUXILIARRY_H

using namespace std;

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h> 
#include <string>
#include <iostream>
#include <errno.h>
#include <time.h>
#include <fstream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <iterator>
#include <random>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <mqueue.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/mman.h>

// shared memory
#define MESSAGE_SIZE 2 * sizeof(double) + sizeof(unsigned int)
#define SHARED_MEMORY_NAME "Message-Memory"

// semaphores
#define WRITTING_SEM "Writting-Semaphore"
#define READING_SEM "Reading-Semaphore"

class memoryFragment;
class memoryFragmentsList;
class minHeap;
class heapNode;
class processEntry;
class buddyNode;
class buddyTree;

#include "../headers/memoryFragment.h"
#include "../headers/memoryFragmentsList.h"
#include "../headers/minHeap.h"
#include "../headers/heapNode.h"
#include "../headers/processEntry.h"
#include "../headers/buddyNode.h"
#include "../headers/buddyTree.h"

void getArguments(int, char**, string*, string*, unsigned int*, unsigned int*, unsigned int*, double*, double*, double*);
void checkArguments(string, string, unsigned int, unsigned int, unsigned int, double, double, double);
void printArguments(string, string, unsigned int, unsigned int, unsigned int, double, double, double);
void createVirtualProcesses(sem_t*, sem_t*, char*, unsigned int, unsigned int, unsigned int, double, double, double);
void storeProcessArrivalTime(char*, double);
void storeProcessLifeTime(char*, double);
void storeProcessMemorySize(char*, unsigned int);
void destroyFragment(memoryFragment*);
void extractProcessInfo(char*, unsigned int*, double*, double*);
void updateCurrentTime(double, double*);
void fitSimulation(fstream*, char*, sem_t*, sem_t*, unsigned int, double, string);
void buddySimulation(fstream*, char*, sem_t*, sem_t*, unsigned int, double, string);
double getProcessArrivalTime(char*);
double getProcessLifeTime(char*);
unsigned int getProcessMemorySize(char*);
int initializeMemoryManager(string*, string*, unsigned int*, double*);
bool simulationTerminated(double, double);
sem_t *sem_initialize(const char *, int);
#endif