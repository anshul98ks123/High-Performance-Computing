# Instructions to run

### 1. Shared memory system calls

Files:
> 2a_shared_mem_system_calls.cpp

Compile and run the program:
```
g++ 2a_shared_mem_system_calls.cpp
./a.out
```

### 2. Semaphore system calls

Files:
> 2b_semaphore_system_calls.cpp

Compile and run the program:

```
g++ 2b_semaphore_system_calls.cpp
./a.out
```


### 3. Producer Consumer Problem - Multiple Processes


Implementation of producer consumer problem for single shared variable using multiple processes. 
 

Default sleep time of 30sec of the main process can be changed in 2c_main.cpp. 

Files:
> sharedmem.h <br>
> 2c_main.cpp <br>
> 2c_producer.cpp <br>
> 2c_consumer.cpp

Compile the program:

```
g++ 2c_main.cpp -o main
g++ 2c_producer.cpp -o producer
g++ 2c_consumer.cpp -o consumer
```

Run the following 3 commands in seperate terminals in the same order:

```
./main
./producer
./consumer
```

### 4. Producer Consumer Problem - Multiple Threads

Implementation of producer consumer problem for single shared variable using multiple threads. 

Files:
> sharedmem.h <br>
> 2d_producer_consumer_multiple_thread.cpp

Compile and run the program:

```
g++ 2d_producer_consumer_multiple_thread.cpp -lpthread
./a.out 
```

### 5. Bounded Buffer Problem

Implementation of bounded buffer problem using pthreads. 

Default value of N (buffer size) and M (number of items to be produced) can be changed in 2e_bounded_buffer.cpp. 


Files:
> sharedmem.h <br>
> 2e_bounded_buffer.cpp

Compile and run the program:

```
g++ 2e_bounded_buffer.cpp -lpthread
./a.out 
```
