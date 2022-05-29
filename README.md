# OS_FileReader_Scheduler
Operating System Assignment No 2 - Utilised Threading to Read Multiple Files and Used Signaling(Stop and Alarm) for Scheduler

Following are the graphs obtained from Process P1 and P2 ( Read and Sum Speed)

Hundred.txt file 

![image](https://raw.githubusercontent.com/asuru56/OS_FileReader_Scheduler/main/Images/Screenshot%20from%202022-05-29%2021-40-10.png)

Thousand.txt file 

![image](https://raw.githubusercontent.com/asuru56/OS_FileReader_Scheduler/main/Images/Screenshot%20from%202022-05-29%2021-40-39.png)

One Lakh.txt file 

![image](https://raw.githubusercontent.com/asuru56/OS_FileReader_Scheduler/main/Images/Screenshot%20from%202022-05-29%2021-41-06.png)

One Million.txt file 

![image](https://raw.githubusercontent.com/asuru56/OS_FileReader_Scheduler/main/Images/Screenshot%20from%202022-05-29%2021-41-29.png)

We can identify that the perfect number of threads to use is highly variable, depending on the number of threads used, the amount of data read and processed. As of now, we have not reached any conclusive results on the perfect number of threads to use in a process.And as in
most cases(pursuing the txt files), the single threaded solution works, this might be because of

File Locking followed by the filesystem or because of us using a HDD to read from.

“It is never a good idea to read from the same physical (spinning) hard disk from
different threads simultaneously, because every switch causes an extra delay of around
10ms to position the read head of the hard disk (would be different on SSD)” ~
Souced(StackOverflow)

Also you can see a significant difference in speed in bigger files when using threads. As the
10ms delay mentioned above is small comparatively.
But as we look into P2. The process is significantly faster than P1. This is due to the nature of
processing the data that is being sent to it and it does not have to go through the hassle of the
HDD. Albeit even there a single thread is faster due to use of mutex lock on the sum integer on
the multithread for synchronization.
