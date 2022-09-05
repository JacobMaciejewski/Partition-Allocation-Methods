#include "../headers/auxiliarry.h"

int main(int argc, char** argv) {
    // information from generator process
    string algorithm = string(argv[1]);
    string logFile = string(argv[2]);
    unsigned int memorySize = (unsigned int)(strtoul(argv[3], NULL, 0));
    double simulationDuration = (double)(atof(argv[4]));

    // semaphores to read/write to shared memory
    sem_t *readMessage = sem_open(READING_SEM, 0);
    sem_t *writtenMessage = sem_open(WRITTING_SEM, 0);

    // shared memory to store new processes info
    int sharedMemoryFD;
    char *processInfo;
    if( (sharedMemoryFD = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0666)) == -1) {
        cout << "Unable to open shared memory, leaving program" << endl;
        exit(-1);
    }
    processInfo = (char *) mmap(0, MESSAGE_SIZE, PROT_WRITE, MAP_SHARED, sharedMemoryFD, 0);

    // initialize writing to log file
    fstream logs;
    logs.open(logFile, ios::out | ios::trunc);
    // simulate
    if(algorithm == "BD") {
        buddySimulation(&logs, processInfo, readMessage, writtenMessage, memorySize, simulationDuration, algorithm);
    }else{
        fitSimulation(&logs, processInfo, readMessage, writtenMessage, memorySize, simulationDuration, algorithm);
    }
    // close log file
    logs.close();
}