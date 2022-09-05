#include "../headers/auxiliarry.h"

int main(int argc, char** argv)
{
    // arguments given by the user
    string algorithm, logFile;
    unsigned int totalMemorySize, processMemoryFloor, processMemoryCeiling;
    double averageArrivalDistance, averageLifeTime, simulationDuration;

    // auxiliarry variables
    int memoryManagerStatus;
    // shared memory variables
    int sharedMemoryFD;
    char *messageMemory;

    getArguments(argc, argv, &algorithm, &logFile, &totalMemorySize, &processMemoryFloor, &processMemoryCeiling, &averageArrivalDistance, &averageLifeTime, &simulationDuration);
    checkArguments(algorithm, logFile, totalMemorySize, processMemoryFloor, processMemoryCeiling, averageArrivalDistance, averageLifeTime, simulationDuration);
    printArguments(algorithm, logFile, totalMemorySize, processMemoryFloor, processMemoryCeiling, averageArrivalDistance, averageLifeTime, simulationDuration);

    // creating shared memory for process information messages
    if((sharedMemoryFD = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666)) == -1) {
        cout << "Unable to create shared memory, leaving program" << endl;
    }
    // allocating shared memory for process information messages
    ftruncate(sharedMemoryFD, MESSAGE_SIZE); 
    // get the pointer to the shared memory space
    messageMemory = (char *) mmap(0, MESSAGE_SIZE, PROT_WRITE, MAP_SHARED, sharedMemoryFD, 0);

    // semaphores for reading and writting to shared memory
    sem_t *writtenMessage = sem_initialize(WRITTING_SEM, 0);
    sem_t *readMessage = sem_initialize(READING_SEM, 1);

    // // initialize memory manager process and feed it with necessary data for algorithm initialization
    initializeMemoryManager(&algorithm, &logFile, &totalMemorySize, &simulationDuration);
    // // creates entries containing info about virtual processes and stores them in shared memory
    createVirtualProcesses(writtenMessage, readMessage, messageMemory, totalMemorySize, processMemoryFloor, processMemoryCeiling, averageArrivalDistance, averageLifeTime, simulationDuration);

    wait(&memoryManagerStatus);

    if(WCOREDUMP(memoryManagerStatus))
    {
        cout << "Memory manager process core dumped!" << endl;
    }else
    {
        cout << endl << "Simulation terminated successfully!" << endl;
    }

    // destroy the semaphores
    sem_destroy(writtenMessage);
    sem_destroy(readMessage);
    // unlink shared memory
    shm_unlink(SHARED_MEMORY_NAME);

    return 0;
}
