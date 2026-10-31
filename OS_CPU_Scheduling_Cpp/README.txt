OS CPU Scheduling Algorithms in C++

Files:
1. Non_Preemptive.cpp
   - FCFS, SJF, LJF

2. Preemptive.cpp
   - SRTF, LRTF, Round Robin, Preemptive Priority
   - For Priority, a smaller number means a higher priority.

3. Multilevel_Queue.cpp
   - Q1: Round Robin, time quantum = 2
   - Q2: FCFS
   - Q1 has higher priority than Q2; processes stay in their assigned queue.

4. Multilevel_Feedback_Queue.cpp
   - Q1: Round Robin, time quantum = 2
   - Q2: Round Robin, time quantum = 4
   - Q3: FCFS
   - All processes enter Q1; unfinished processes move to the next lower queue.

Compile each file separately using:
g++ filename.cpp -o program

Example:
g++ Non_Preemptive.cpp -o nonpreemptive
./nonpreemptive

On Windows, run:
nonpreemptive.exe

All programs ask for arrival time and burst time. The priority program also asks
for priority, and Round Robin asks for a time quantum. These are beginner-friendly
console programs and use standard C++ vectors/queues.
