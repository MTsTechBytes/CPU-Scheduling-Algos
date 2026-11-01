#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>

using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;       // Used for Priority Scheduling (Lower number = Higher priority)
    int remainingTime;  // Tracker for processing intervals
    int completionTime;
    int turnaroundTime;
    int waitingTime;
};

bool compareArrival(const Process& a, const Process& b) {
    if (a.arrivalTime == b.arrivalTime)
        return a.id < b.id;
    return a.arrivalTime < b.arrivalTime;
}

bool compareID(const Process& a, const Process& b) {
    return a.id < b.id;
}

void printMetrics(vector<Process> processes) {
    sort(processes.begin(), processes.end(), compareID);
    float totalWT = 0, totalTAT = 0;
    int n = processes.size();
    
    cout << "\n" << setfill('-') << setw(75) << "-" << setfill(' ') << "\n";
    cout << setw(5) << "PID" << setw(12) << "Arrival" << setw(10) << "Burst" << setw(10) << "Priority"
         << setw(15) << "Completion" << setw(12) << "Turnaround" << setw(10) << "Waiting" << "\n";
    cout << setfill('-') << setw(75) << "-" << setfill(' ') << "\n";

    for (const auto& p : processes) {
        totalWT += p.waitingTime;
        totalTAT += p.turnaroundTime;
        cout << setw(5) << p.id 
             << setw(12) << p.arrivalTime 
             << setw(10) << p.burstTime 
             << setw(10) << p.priority
             << setw(15) << p.completionTime 
             << setw(12) << p.turnaroundTime 
             << setw(10) << p.waitingTime << "\n";
    }
    
    cout << setfill('-') << setw(75) << "-" << setfill(' ') << "\n";
    cout << fixed << setprecision(2);
    cout << "Average Waiting Time    : " << totalWT / n << "\n";
    cout << "Average Turnaround Time : " << totalTAT / n << "\n";
}

// 1. Shortest Remaining Time First (SRTF)
void findSRTF(vector<Process> processes) {
    int n = processes.size();
    for (auto& p : processes) p.remainingTime = p.burstTime;

    int currentTime = 0, completedCount = 0;
    int minRemaining = 1e9;
    int shortestIdx = -1;
    bool check = false;

    while (completedCount < n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
                if (processes[i].remainingTime < minRemaining) {
                    minRemaining = processes[i].remainingTime;
                    shortestIdx = i;
                    check = true;
                } else if (processes[i].remainingTime == minRemaining) {
                    if (shortestIdx != -1 && processes[i].arrivalTime < processes[shortestIdx].arrivalTime) {
                        shortestIdx = i;
                    }
                }
            }
        }

        if (!check) {
            currentTime++;
            continue;
        }

        processes[shortestIdx].remainingTime--;
        minRemaining = processes[shortestIdx].remainingTime;
        if (minRemaining == 0) minRemaining = 1e9;

        if (processes[shortestIdx].remainingTime == 0) {
            completedCount++;
            check = false;
            processes[shortestIdx].completionTime = currentTime + 1;
            processes[shortestIdx].turnaroundTime = processes[shortestIdx].completionTime - processes[shortestIdx].arrivalTime;
            processes[shortestIdx].waitingTime = processes[shortestIdx].turnaroundTime - processes[shortestIdx].burstTime;
        }
        currentTime++;
    }
    cout << "\n--- SRTF (Preemptive SJF) Scheduling Results ---";
    printMetrics(processes);
}

// 2. Longest Remaining Time First (LRTF)
void findLRTF(vector<Process> processes) {
    int n = processes.size();
    for (auto& p : processes) p.remainingTime = p.burstTime;

    int currentTime = 0, completedCount = 0;
    
    while (completedCount < n) {
        int longestIdx = -1;
        int maxRemaining = -1;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
                if (processes[i].remainingTime > maxRemaining) {
                    maxRemaining = processes[i].remainingTime;
                    longestIdx = i;
                } else if (processes[i].remainingTime == maxRemaining) {
                    if (longestIdx != -1 && processes[i].id < processes[longestIdx].id) {
                        longestIdx = i;
                    }
                }
            }
        }

        if (longestIdx == -1) {
            currentTime++;
            continue;
        }

        processes[longestIdx].remainingTime--;

        if (processes[longestIdx].remainingTime == 0) {
            completedCount++;
            processes[longestIdx].completionTime = currentTime + 1;
            processes[longestIdx].turnaroundTime = processes[longestIdx].completionTime - processes[longestIdx].arrivalTime;
            processes[longestIdx].waitingTime = processes[longestIdx].turnaroundTime - processes[longestIdx].burstTime;
        }
        currentTime++;
    }
    cout << "\n--- LRTF (Preemptive LJF) Scheduling Results ---";
    printMetrics(processes);
}

// 3. Round Robin (RR)
void findRoundRobin(vector<Process> processes, int quantum) {
    int n = processes.size();
    sort(processes.begin(), processes.end(), compareArrival);
    for (auto& p : processes) p.remainingTime = p.burstTime;

    queue<int> readyQueue;
    vector<bool> inQueue(n, false);
    
    int currentTime = processes[0].arrivalTime;
    readyQueue.push(0);
    inQueue[0] = true;
    
    int completedCount = 0;

    while (completedCount < n) {
        if (readyQueue.empty()) {
            for (int i = 0; i < n; i++) {
                if (processes[i].remainingTime > 0) {
                    currentTime = max(currentTime, processes[i].arrivalTime);
                    readyQueue.push(i);
                    inQueue[i] = true;
                    break;
                }
            }
            continue;
        }

        int idx = readyQueue.front();
        readyQueue.pop();

        int executionTime = min(processes[idx].remainingTime, quantum);
        processes[idx].remainingTime -= executionTime;
        currentTime += executionTime;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0 && !inQueue[i]) {
                readyQueue.push(i);
                inQueue[i] = true;
            }
        }

        if (processes[idx].remainingTime > 0) {
            readyQueue.push(idx);
        } else {
            processes[idx].completionTime = currentTime;
            processes[idx].turnaroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
            processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
            completedCount++;
        }
    }
    cout << "\n--- Round Robin (Quantum = " << quantum << ") Results ---";
    printMetrics(processes);
}

// 4. Priority Scheduling (Preemptive)
void findPriorityPreemptive(vector<Process> processes) {
    int n = processes.size();
    for (auto& p : processes) p.remainingTime = p.burstTime;

    int currentTime = 0, completedCount = 0;
    int highestPriority = 1e9;
    int priorityIdx = -1;
    bool check = false;

    while (completedCount < n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
                if (processes[i].priority < highestPriority) {
                    highestPriority = processes[i].priority;
                    priorityIdx = i;
                    check = true;
                } else if (processes[i].priority == highestPriority) {
                    if (priorityIdx != -1 && processes[i].arrivalTime < processes[priorityIdx].arrivalTime) {
                        priorityIdx = i;
                    }
                }
            }
        }

        if (!check) {
            currentTime++;
            continue;
        }

        processes[priorityIdx].remainingTime--;
        highestPriority = processes[priorityIdx].priority;
        if (processes[priorityIdx].remainingTime == 0) highestPriority = 1e9;

        if (processes[priorityIdx].remainingTime == 0) {
            completedCount++;
            check = false;
            processes[priorityIdx].completionTime = currentTime + 1;
            processes[priorityIdx].turnaroundTime = processes[priorityIdx].completionTime - processes[priorityIdx].arrivalTime;
            processes[priorityIdx].waitingTime = processes[priorityIdx].turnaroundTime - processes[priorityIdx].burstTime;
        }
        currentTime++;
    }
    cout << "\n--- Priority (Preemptive) Scheduling Results ---";
    printMetrics(processes);
}

int main() {
    vector<Process> processes = {
        {1, 0, 8, 3},
        {2, 1, 4, 1},
        {3, 2, 9, 4},
        {4, 3, 5, 2}
    };

    int timeQuantum = 2;

    cout << "=== RUNNING PREEMPTIVE ALGORITHMS ===" << endl;
    findSRTF(processes);
    findLRTF(processes);
    findRoundRobin(processes, timeQuantum);
    findPriorityPreemptive(processes);

    return 0;
}
