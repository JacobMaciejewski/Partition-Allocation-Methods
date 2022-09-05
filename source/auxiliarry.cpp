#include "../headers/auxiliarry.h"

// scans the user's terminal input
// extracts the following argument
// -lo (process memory floor size)
// -hi (process memory ceiling size)
// -t (average time distance between random process arrivals)
// -T (average life duration of a process)
// -S (the total size of the available memory)
// -A (the name of the algorithm to be used)
void getArguments(int argc, char **argv, string *algorithm, string *logfile, unsigned int *totalMemorySize, unsigned int *processMemoryFloor, unsigned int *processMemoryCeiling, double *averageArrivalDistance, double *averageLifeTime, double *simulationDuration)
{
    *algorithm = "BD";
    *logfile = "logfile.txt";
    *totalMemorySize = 1024;
    *processMemoryFloor = 1;
    *processMemoryCeiling = 8;
    *averageArrivalDistance = 0.001;
    *simulationDuration = 10.0;
    *averageLifeTime = 0.2;

    for(int i = 0; i < argc; i++)
    {
        if(!strcmp(argv[i], "-A"))
        {
            *algorithm = (string)(argv[i+1]);
        }
        else if(!strcmp(argv[i], "-lo"))
        {
            *processMemoryFloor = (unsigned int)(atoi(argv[i+1]));
        }
        else if(!strcmp(argv[i], "-hi"))
        {
            *processMemoryCeiling = (unsigned int)(atoi(argv[i+1]));
        }
        else if(!strcmp(argv[i], "-S"))
        {
            *totalMemorySize = (unsigned int)(atoi(argv[i+1]));
        }
        else if(!strcmp(argv[i], "-t"))
        {
            *averageArrivalDistance = (double)(atof(argv[i+1]));
        }
        else if(!strcmp(argv[i], "-T"))
        {
            *averageLifeTime = (double)(atof(argv[i+1]));
        }
        else if(!strcmp(argv[i], "-d"))
        {
            *simulationDuration = (double)(atof(argv[i+1]));
        }
        else if(!strcmp(argv[i], "-L"))
        {
            *logfile = (string)(argv[i+1]);
        }
    }
}
// creates message queue entries containing information about virtual processes
// to be read by the memory manager
void createVirtualProcesses(sem_t *writtenMessage, sem_t *readMessage, char *messageBuffer, unsigned int totalMemorySize, unsigned int processMemoryFloor, unsigned int processMemoryCeiling, double averageArrivalDistance, double averageLifeTime, double  simulationDuration) {

    random_device rd; // uniformly-distributed integer random number generator
    mt19937 rng (rd ()); // mt19937: Pseudo-random number generation

    double currentArrivalTime = 0.0;
    double newArrivalOffset, newProcessLifeTime;
    unsigned int newProcessMemorySize;
    unsigned int totalProcessesSent = 0;

    // used to calculate arrival times
    exponential_distribution<double> poisson (1 / averageArrivalDistance);
    // used to calculate process memory sizes
    uniform_int_distribution<unsigned int> uniform (processMemoryFloor,processMemoryCeiling);
    // used to calculate process lifetimes
    exponential_distribution<double> exponential (1 / averageLifeTime);

    // we keep producing virtual processes till the end of the simulation
    while(currentArrivalTime < simulationDuration) {

        newArrivalOffset = poisson.operator() (rng);
        newProcessMemorySize = uniform(rng);
        newProcessLifeTime = exponential(rng);

        currentArrivalTime += newArrivalOffset;

        // new virtual process added to the message queue
        sem_wait(readMessage);
        storeProcessArrivalTime(messageBuffer, currentArrivalTime);
        storeProcessLifeTime(messageBuffer, newProcessLifeTime);
        storeProcessMemorySize(messageBuffer, newProcessMemorySize);
        sem_post(writtenMessage);
        totalProcessesSent++;
    }
    cout << endl << "Sent " << totalProcessesSent - 1 << " processes!" << endl;
}

// starts the memory manager process
// returns its process identifier
int initializeMemoryManager(string* algorithm, string* logFile, unsigned int* totalMemorySize, double* simulationDuration) {
    int memoryManagerIdentifier;

    string totalMemorySizeStr = to_string(*totalMemorySize);
    string simulationDurationStr = to_string(*simulationDuration);

    if((memoryManagerIdentifier = fork()) < 0)
    {
        perror("Could not fork, unable to initialize memory manager");
    }
    else if(memoryManagerIdentifier == 0)
    {
        execl("./manager", "./manager", algorithm->c_str(), logFile->c_str(), totalMemorySizeStr.c_str(), simulationDurationStr.c_str(), (char*) NULL);
        exit(69);
    }
    return memoryManagerIdentifier;
}

void storeProcessArrivalTime(char *messageBuffer, double arrivalTime) {
    double *infoDouble = (double*)(messageBuffer);
    infoDouble[0] = arrivalTime;
}

void storeProcessLifeTime(char *messageBuffer, double lifeTime) {
    double *infoDouble = (double*)(messageBuffer);
    infoDouble[1] = lifeTime;
}

void storeProcessMemorySize(char *messageBuffer, unsigned int memorySize) {
    double *infoDouble = (double*)(messageBuffer);
    unsigned int *processMemorySizePtr = (unsigned int *)(&(infoDouble[2]));

    *processMemorySizePtr = memorySize;
}

// this function deallocated the target fragment
// without deallocating the following fragments
void destroyFragment(memoryFragment *fragmentToDestroy){
    fragmentToDestroy->setRightNeighbour(NULL);
    delete fragmentToDestroy;
}

void extractProcessInfo(char *messageBuffer, unsigned int *memorySize, double *arrivalTime, double *lifeTime) {
    *arrivalTime = getProcessArrivalTime(messageBuffer);
    *lifeTime = getProcessLifeTime(messageBuffer);
    *memorySize = getProcessMemorySize(messageBuffer);
}

// sets the new time as the current time
void updateCurrentTime(double newTime, double *currentTime) {
    *currentTime = newTime;
    return;
}

void printArguments(string algorithm, string logFile, unsigned int totalMemorySize, unsigned int processMemoryFloor, unsigned int processMemoryCeiling, double averageArrivalDistance, double averageLifeTime, double simulationDuration) {
    cout << "Algorithm[" << algorithm << "]" << endl;
    cout << "Log_File[" << logFile << "]" << endl;
    cout << "Memory_Size[" << totalMemorySize << "]" << endl;
    cout << "Floor[" << processMemoryFloor << "]" << endl;
    cout << "Ceiling[" << processMemoryCeiling << "]" << endl;
    cout << "Arrival_Distance[" << averageArrivalDistance << "]" << endl;
    cout << "Life_Time[" << averageLifeTime << "]" << endl;
    cout << "Simulation_Duration[" << simulationDuration << "]" << endl;
    return;
}

void checkArguments(string algorithm, string logFile, unsigned int totalMemorySize, unsigned int processMemoryFloor, unsigned int processMemoryCeiling, double averageArrivalDistance, double averageLifeTime, double simulationDuration) {

    if(algorithm != "BF" && algorithm != "WF" && algorithm != "BD") {
        cout << "Invalid Algorithm Name- Choose[BF/WF/BD]!" << endl;
        exit(-1);
    }

    if(processMemoryCeiling > totalMemorySize || processMemoryFloor > totalMemorySize) {
        cout << "Invalid Memory Limits!" << endl;
        exit(-1);
    }

    if(processMemoryFloor > processMemoryCeiling) {
        cout << "Memory Floor Should Be Smaller Than The Ceiling!" << endl;
        exit(-1);
    }

    if(averageArrivalDistance <= 0.0 || averageLifeTime <= 0.0 || simulationDuration <= 0.0) {
        cout << "Arrival Distance / Life Time / Simulation Duration should be positive floating point numbers!" << endl;
        exit(-1);
    }
    return;
}

double getProcessArrivalTime(char *messageBuffer) {
    double *infoDouble = (double*)(messageBuffer);
    return infoDouble[0];
}

double getProcessLifeTime(char *messageBuffer) {
    double *infoDouble = (double*)(messageBuffer);
    return infoDouble[1];
}

unsigned int getProcessMemorySize(char *messageBuffer) {
    double *infoDouble = (double*)(messageBuffer);
    unsigned int *processMemorySizePtr = (unsigned int *)(&(infoDouble[2]));

    return *processMemorySizePtr;
}

bool simulationTerminated(double currentTime, double simulationDuration) {
    return (currentTime >= simulationDuration);
}

sem_t *sem_initialize(const char *semaphore_name, int initial_value)
{
    sem_t *new_semaphore;

    new_semaphore = sem_open(semaphore_name, O_CREAT | O_EXCL, 0644, initial_value);

    // named semaphore already exists, reinitialize it
    if(errno == EEXIST && new_semaphore == SEM_FAILED)
    {
        sem_unlink(semaphore_name);
        new_semaphore = sem_open(semaphore_name, O_CREAT, 0644, initial_value);
    }
    return new_semaphore;
}

void fitSimulation(fstream *logFile, char *processInfo, sem_t *readMessage, sem_t *writtenMessage, unsigned int memorySize, double simulationDuration, string algorithm) {
    bool bestFit = (algorithm == "BF");
    double currentTime = 0.0;
    unsigned int totalProcesses = 0, allocatedProcesses = 0, waitingProcesses = 0, stuckProcesses = 0;

    // current process information
    unsigned int pSize;
    double pArrivalTime;
    double pLifeTime;
    unsigned int pID = 0;
    processEntry *pData;
    // fragment in memory where process is stored
    memoryFragment *pFragment;
    
    // active processes terminated while waiting for process arrival
    heapNode *terminatedProcessNode;

    // data structures to store processes
    minHeap *processesInMemory = new minHeap();
    memoryFragmentsList *memory = new memoryFragmentsList(memorySize);

    *logFile << endl;
    *logFile << "PROCESSES ALLOCATED: " << endl;

    while(!simulationTerminated(currentTime, simulationDuration)) {

        // read the next process information
        sem_wait(writtenMessage);
        extractProcessInfo(processInfo, &pSize, &pArrivalTime, &pLifeTime);
        sem_post(readMessage);

        if(simulationTerminated(pArrivalTime, simulationDuration)) {
            break;
        }else {
            totalProcesses++;
        }

        // free the processes that terminate before the arrival of the new process
        while(!processesInMemory->isEmpty() && processesInMemory->haveTerminatedTill(pArrivalTime)) {

            terminatedProcessNode = processesInMemory->pop();
            // free the memory allocated for terminated process
            memory->freeProcessMemory((memoryFragment*)terminatedProcessNode->getData());
            // time set to the moment terminated process leaves memory
            updateCurrentTime(terminatedProcessNode->getTerminationTime(), &currentTime);
            // free memory for heap node and the process entry itself that it contains
            delete terminatedProcessNode;
        }

        // while unable to find space for current process
        // wait for active processes to leave memory
        while((pFragment = memory->findFragmentToAllocateProcess(pSize,bestFit)) == NULL) {

            terminatedProcessNode = processesInMemory->pop();
            // free the memory allocated for terminated process
            memory->freeProcessMemory((memoryFragment*)terminatedProcessNode->getData());
            // time set to the moment terminated process leaves memory
            updateCurrentTime(terminatedProcessNode->getTerminationTime(), &currentTime);
            // free memory for heap node and the process entry itself that it contains
            delete terminatedProcessNode;

            if(simulationTerminated(currentTime, simulationDuration)) {
                break;
            }

        }

        // simulation terminated while waiting
        // for new process memory allocation
        if(simulationTerminated(currentTime,simulationDuration)) {
            break;
        }

        // either process is inserted into the memory at the moment of arrival or after a
        // previous process leaves the memory to leave enough space
        updateCurrentTime(max(currentTime, pArrivalTime),&currentTime);
        // store process in memory
        pData = new processEntry(pID, pSize, pArrivalTime, pLifeTime, currentTime);
        pFragment = memory->getAllocatedFragment(pSize, pFragment);
        processesInMemory->push(new heapNode(pData->getTerminationTime(), pData, (void*)pFragment));
        // print newly allocated processes
        *logFile << pData->print();
        allocatedProcesses++;
        pID++;
    }

    *logFile << endl;
    *logFile << "PROCESSES STILL IN MEMORY: " << endl;
    // remove the processes stuck in memory after termination
    while(!processesInMemory->isEmpty()) {
        processEntry *stuckProcess;

        terminatedProcessNode = processesInMemory->pop();
        // free the memory allocated for terminated process
        memory->freeProcessMemory((memoryFragment*)terminatedProcessNode->getData());
        // print the stuck process
        stuckProcess = terminatedProcessNode->getProcessInfo();

        if(simulationTerminated(stuckProcess->getTerminationTime(),simulationDuration)) {
            *logFile << stuckProcess->print();
            stuckProcesses++;
        }
        // time set to the moment terminated process leaves memory
        updateCurrentTime(terminatedProcessNode->getTerminationTime(), &currentTime);
        // free memory for heap node and the process entry itself that it contains
        delete terminatedProcessNode;
    }

    *logFile << endl;
    *logFile << "NOT ALLOCATED PROCESSES: " << endl; 
    // remove the processes that never entered memory
    while(!simulationTerminated(pArrivalTime, simulationDuration)) {
        // read the next process information
        sem_wait(writtenMessage);
        extractProcessInfo(processInfo, &pSize, &pArrivalTime, &pLifeTime);
        *logFile << "PID[" << pID << "] Size[" << pSize << "] Arrival_Time[" << pArrivalTime << "] Life_Time[" << pLifeTime << "]" << endl;
        waitingProcesses++;
        totalProcesses++;
        pID++;
        sem_post(readMessage);
    }

    cout << "Received " << totalProcesses << " processes!" << endl;
    *logFile << endl;
    *logFile << "Total Processes Received: " << totalProcesses << endl;
    *logFile << "Total Processes Allocated: " << allocatedProcesses << endl;
    *logFile << "Total Processes Still Waiting: " << waitingProcesses << endl;
    *logFile << "Total Processes Stuck In Memory: " << stuckProcesses << endl;

    delete memory;
    delete processesInMemory;
}

 void buddySimulation(fstream *logFile, char *processInfo, sem_t *readMessage, sem_t *writtenMessage, unsigned int memorySize, double simulationDuration, string algorithm) {
         double currentTime = 0.0;
    unsigned int totalProcesses = 0, allocatedProcesses = 0, waitingProcesses = 0, stuckProcesses = 0;

    // current process information
    unsigned int pSize;
    double pArrivalTime;
    double pLifeTime;
    unsigned int pID = 0;
    processEntry *pData;
    // fragment in memory where process is stored
    buddyNode *pFragment;
    
    // active processes terminated while waiting for process arrival
    heapNode *terminatedProcessNode;

    // data structures to store processes
    minHeap *processesInMemory = new minHeap();
    buddyTree *memory = new buddyTree(memorySize, 1);

    *logFile << endl;
    *logFile << "PROCESSES ALLOCATED: " << endl;

    while(!simulationTerminated(currentTime, simulationDuration)) {

        // read the next process information
        sem_wait(writtenMessage);
        extractProcessInfo(processInfo, &pSize, &pArrivalTime, &pLifeTime);
        sem_post(readMessage);

        if(simulationTerminated(pArrivalTime, simulationDuration)) {
            break;
        }else {
            totalProcesses++;
        }

        // free the processes that terminate before the arrival of the new process
        while(!processesInMemory->isEmpty() && processesInMemory->haveTerminatedTill(pArrivalTime)) {

            terminatedProcessNode = processesInMemory->pop();
            // free the memory allocated for terminated process
            memory->freeProcessMemory((buddyNode*)terminatedProcessNode->getData());
            // time set to the moment terminated process leaves memory
            updateCurrentTime(terminatedProcessNode->getTerminationTime(), &currentTime);
            // free memory for heap node and the process entry itself that it contains
            delete terminatedProcessNode;
        }

        // while unable to find space for current process
        // wait for active processes to leave memory
        while((pFragment = memory->allocateFragmentForProcess(pSize)) == NULL) {

            terminatedProcessNode = processesInMemory->pop();
            // free the memory allocated for terminated process
            memory->freeProcessMemory((buddyNode*)terminatedProcessNode->getData());
            // time set to the moment terminated process leaves memory
            updateCurrentTime(terminatedProcessNode->getTerminationTime(), &currentTime);
            // free memory for heap node and the process entry itself that it contains
            delete terminatedProcessNode;

            if(simulationTerminated(currentTime, simulationDuration)) {
                break;
            }

        }

        // simulation terminated while waiting
        // for new process memory allocation
        if(simulationTerminated(currentTime,simulationDuration)) {
            break;
        }

        // either process is inserted into the memory at the moment of arrival or after a
        // previous process leaves the memory to leave enough space
        updateCurrentTime(max(currentTime, pArrivalTime),&currentTime);
        // store process in memory
        pData = new processEntry(pID, pSize, pArrivalTime, pLifeTime, currentTime);
        processesInMemory->push(new heapNode(pData->getTerminationTime(), pData, (void*)pFragment));
        // print newly allocated processes
        *logFile << pData->print();
        allocatedProcesses++;
        pID++;
    }

    *logFile << endl;
    *logFile << "PROCESSES STILL IN MEMORY: " << endl;
    // remove the processes stuck in memory after termination
    while(!processesInMemory->isEmpty()) {
        processEntry *stuckProcess;

        terminatedProcessNode = processesInMemory->pop();
        // free the memory allocated for terminated process
        memory->freeProcessMemory((buddyNode*)terminatedProcessNode->getData());
        // print the stuck process
        stuckProcess = terminatedProcessNode->getProcessInfo();

        if(simulationTerminated(stuckProcess->getTerminationTime(),simulationDuration)) {
            *logFile << stuckProcess->print();
            stuckProcesses++;
        }
        // time set to the moment terminated process leaves memory
        updateCurrentTime(terminatedProcessNode->getTerminationTime(), &currentTime);
        // free memory for heap node and the process entry itself that it contains
        delete terminatedProcessNode;
    }

    *logFile << endl;
    *logFile << "NOT ALLOCATED PROCESSES: " << endl; 
    // remove the processes that never entered memory
    while(!simulationTerminated(pArrivalTime, simulationDuration)) {
        // read the next process information
        sem_wait(writtenMessage);
        extractProcessInfo(processInfo, &pSize, &pArrivalTime, &pLifeTime);
        *logFile << "PID[" << pID << "] Size[" << pSize << "] Arrival_Time[" << pArrivalTime << "] Life_Time[" << pLifeTime << "]" << endl;
        waitingProcesses++;
        totalProcesses++;
        pID++;
        sem_post(readMessage);
    }

    cout << "Received " << totalProcesses << " processes!" << endl;
    *logFile << endl;
    *logFile << "Total Processes Received: " << totalProcesses << endl;
    *logFile << "Total Processes Allocated: " << allocatedProcesses << endl;
    *logFile << "Total Processes Still Waiting: " << waitingProcesses << endl;
    *logFile << "Total Processes Stuck In Memory: " << stuckProcesses << endl;

    delete memory;
    delete processesInMemory;
 }