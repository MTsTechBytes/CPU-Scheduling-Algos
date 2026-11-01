#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>

struct Process
{
    int id;
    int arrivalTime;
    int burstTime;
    int priority;      // Used for Priority Scheduling (Lower number = Higher priority)
    int remainingTime; // Tracker for processing intervals
    int completionTime;
    int turnaroundTime;
    int waitingTime;
};

// Sorts processes by Arrival Time
bool compareArrival(const Process &a, const Process &b)
{
    if (a.arrivalTime == b.arrivalTime)
        return a.id < b.id;
    return a.arrivalTime < b.arrivalTime;
}


bool compareID(const Process &a, const Process &b)
{
    return a.id < b.id;
}

void printMetrics(std::vector<Process> processes)
{
    // This line means
    sort(processes.begin(), processes.end(), compareID);
    float totalWT = 0, totalTAT = 0;
    int n = processes.size();

    std::cout << "\n"
              << std::setfill('-') << std::setw(65) << "-" << std::setfill(' ') << "\n";
    std::cout << std::setw(5) << "PID" << std::setw(12) << "Arrival" << std::setw(10) << "Burst"
              << std::setw(15) << "Completion" << std::setw(12) << "Turnaround" << std::setw(10) << "Waiting" << "\n";
    std::cout << std::setfill('-') << std::setw(65) << "-" << std::setfill(' ') << "\n";

    for (const auto &p : processes)
    {
        totalWT += p.waitingTime;
        totalTAT += p.turnaroundTime;
        std::cout << std::setw(5) << p.id
                  << std::setw(12) << p.arrivalTime
                  << std::setw(10) << p.burstTime
                  << std::setw(15) << p.completionTime
                  << std::setw(12) << p.turnaroundTime
                  << std::setw(10) << p.waitingTime << "\n";
    }

    std::cout << std::setfill('-') << std::setw(65) << "-" << std::setfill(' ') << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Average Waiting Time    : " << totalWT / n << "\n";
    std::cout << "Average Turnaround Time : " << totalTAT / n << "\n";
}

void FCFS(std::vector<Process> &processes)
{
    int n = processes.size();
    std::sort(processes.begin(), processes.end(), compareArrival);

    int currentTime = 0;

    for (int i = 0; i < n; ++i)
    {
        if (currentTime < processes[i].arrivalTime)
        {
            currentTime = processes[i].arrivalTime;
        }
        currentTime += processes[i].burstTime;
        processes[i].completionTime = currentTime;
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
        processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
    }

    std::cout << "\n--- FCFS Scheduling Results ---";
    printMetrics(processes);
}

void SJF(std::vector<Process> &processes)
{
    int n = processes.size();
    std::vector<bool> isCompleted(n, false);

    int currentTime = 0, completed = 0;

    int shortestIdx = -1;
    int minBurst = 1e9;

    while (completed < n)
    {
        for (int i = 0; i < n; i++)
        {
            if (processes[i].arrivalTime <= currentTime && isCompleted[i] == false)
            {
                if (processes[i].burstTime < minBurst)
                {
                    minBurst = processes[i].burstTime;
                    shortestIdx = i;
                }
                else if (processes[i].burstTime == minBurst)
                {
                    if (shortestIdx != -1 && processes[i].arrivalTime < processes[shortestIdx].arrivalTime)
                    {
                        shortestIdx = i;
                    }
                }
            }
        }

        if (shortestIdx != -1)
        {
            processes[shortestIdx].completionTime = currentTime + processes[shortestIdx].burstTime;
            processes[shortestIdx].turnaroundTime = processes[shortestIdx].completionTime - processes[shortestIdx].arrivalTime;
            processes[shortestIdx].waitingTime = processes[shortestIdx].turnaroundTime - processes[shortestIdx].burstTime;

            currentTime = processes[shortestIdx].completionTime;
            isCompleted[shortestIdx] = true;
            completed++;
        }
        else
        {
            currentTime++;
        }
    }

    std::cout << "\n--- SJF (Non-Preemptive) Scheduling Results ---";
    printMetrics(processes);
}

void SRTF(std::vector<Process> &processes)
{
    int n = processes.size();
    for (int i = 0; i < n; i++)
    {
        processes[i].remainingTime = processes[i].burstTime;
    }

    int minRemaining = 1e9;
    int shortestIdx = -1;
    int currentTime = 0, completed = 0;
    bool check = false;

    while (completed < n)
    {
        for (int i = 0; i < n; i++)
        {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0)
            {
                if (processes[i].remainingTime < minRemaining)
                {
                    minRemaining = processes[i].remainingTime;
                    shortestIdx = i;
                    check = true;
                }
                else if (processes[i].remainingTime == minRemaining)
                {
                    if (shortestIdx != 1 && processes[i].arrivalTime < processes[shortestIdx].arrivalTime)
                    {
                        shortestIdx = i;
                    }
                }
            }
        }

        if (!check)
        {
            currentTime++;
            continue;
        }

        processes[shortestIdx].remainingTime--;
        minRemaining = processes[shortestIdx].remainingTime;

        if (minRemaining == 0)
        {
            minRemaining = 1e9;
        }

        if (processes[shortestIdx].remainingTime == 0)
        {
            completed++;
            check = false;
            processes[shortestIdx].completionTime = currentTime + 1;
            processes[shortestIdx].turnaroundTime = processes[shortestIdx].completionTime - processes[shortestIdx].arrivalTime;
            processes[shortestIdx].waitingTime = processes[shortestIdx].turnaroundTime - processes[shortestIdx].burstTime;
        }

        currentTime++;
    }

    std::cout << "\n--- SRTF (Preemptive SJF) Scheduling Results ---";
    printMetrics(processes);
}

void RoundRobin(std::vector<Process> &processes, int timeQuantum)
{
    int n = processes.size();
    sort(processes.begin(), processes.end(), compareArrival);

    for (int i = 0; i < n; i++)
    {
        processes[i].remainingTime = processes[i].burstTime;
    }

    std::queue<int> readyQueue;
    std::vector<bool> inQueue(n, false);
    readyQueue.push(0);
    inQueue[0] = true;

    int currentTime = 0, completed = 0;

    while (completed < n)
    {
        if (readyQueue.empty())
        {
            for (int i = 0; i < n; i++)
            {
                if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0 && inQueue[i] == false)
                {
                    readyQueue.push(i);
                    inQueue[i] = true;
                    break;
                }
            }
            currentTime++;
            continue;
        }

        int curr = readyQueue.front();
        readyQueue.pop();

        if (processes[curr].remainingTime > timeQuantum)
        {
            currentTime += timeQuantum;
            processes[curr].remainingTime -= timeQuantum;
        }
        else
        {
            currentTime += processes[curr].remainingTime;
            processes[curr].remainingTime = 0;
            processes[curr].turnaroundTime = processes[curr].completionTime - processes[curr].arrivalTime;
            processes[curr].waitingTime = processes[curr].turnaroundTime - processes[curr].burstTime;
            completed++;
        }

        for (int i = 0; i < n; i++)
        {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0 && inQueue[i] == false)
            {
                readyQueue.push(i);
                inQueue[i] = true;
            }
        }

        if (processes[curr].remainingTime > 0)
        {
            readyQueue.push(curr);
        }
    }

    std::cout << "\n--- Round Robin (Quantum = " << timeQuantum << ") Results ---";
    printMetrics(processes);
}



int main()
{
    std::vector<Process> processes = {
        {1, 0, 8, 2},
        {2, 1, 4, 1},
        {3, 2, 9, 3},
        {4, 3, 5, 2}};

    FCFS(processes);
    SJF(processes);
    SRTF(processes);
    RoundRobin(processes, 2);
}