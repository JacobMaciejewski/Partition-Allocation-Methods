# Partition-Allocation-Methods
Implements a Virtual Process Generator and a Memory Manager supporting Buddy and Fit Partition Allocation Algorithms 💿

In modern computers, memory is *logically* divided into different **Blocks**, also known as **Partitions**. The problem of [Partition Allocation](https://eng.libretexts.org/Courses/Delta_College/Operating_System%3A_The_Basics/07%3A_Memory/7.4%3A_Memory_Partitioning),
appears when there is more than one partition freely available to accommodate a process’s request and a partition must be selected.
A **Partition Allocation Method** is needed to choose a particular partition.
Those methods aim to avoid [Internal & External Fragmentation](https://www.geeksforgeeks.org/difference-between-internal-and-external-fragmentation/) of the memory.
The following Partition Allocation Methods have been implemented:
* [Buddy Allocation](https://en.wikipedia.org/wiki/Buddy_memory_allocation) - memory is managed in power of two increments. Assume the memory size is $2^U$. 
Suppose a size of S is required. If $2U-1 < S \leq 2U$, the whole block is allocated. Else, the block is recursively divided and the condition is tested at each time. If satisfied, block
is allocated. Unallocated blocks are tracked, so when two neighbouring blocks appear to be both empty, they are merged. Allows for
*fast memory allocation* and *de-allocation*, but causes *Internal Fragmentation*
* [Best Fit](https://www.geeksforgeeks.org/best-fit-allocation-in-operating-system/) - the **Best Fitting Partition** in terms of size is chosen, this
can be achieved by ordering processes in terms of their size and searching the partition with the smallest size difference for each process
* [Worst Fit](https://www.geeksforgeeks.org/worst-fit-allocation-in-operating-systems/) - the **Largest Available Partition** is chosen, resulting in 
**Big Internal Fragmentation** and allowing many small processes to be placed in that leftover partition

## Compilation & Execution:

In order to compile the source code, type: `make` </br>
Compilation produces two executable files, namely `generator` and `manager`. In order to execute the program, type: </br>
`./generator -A <Algorithm: BD (Buddy) / BF (Best Fit) / WF (Worst Fit)> -lo <Minimum Process Memory> -hi <Maximum Process Memory> -S <Total Memory Size> -t <Average Arrival Distance> -T <Average Process Life Time> -d <Simulation Duration> -L <Log File Name>`

We just execute the **Virtual Process Generator** program, as **Memory Manager** is initialized and managed internally! </br>
Only the absolute path to the **Log File** needs to exist, new file is initialized for each execution! </br>

Output **Log File** includes information about:

* **Allocated Processes** - processes for which memory has been allocated and managed to terminate in time with their </br> *Size*, *Arrival*, *Waiting* and *Termination Time*
* **Stuck Processes** - processes for which memory has been allocated but didn't terminate in time with their *Size*, *Arrival*, *Waiting* and *Termination Time*
* **Unallocated Processes** - processes for which memory has not been allocated with their *Size*, *Arrival* and *Life Time*

Non-terminated process with the smallest termination time is **NOT INCLUDED** in the **Stuck Processes** due to grading script intricacy!

### Further Information:

*Built as part of the course: Operational Systems , Winter of 2019. University of Athens, DiT.*
