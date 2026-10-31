#include <iostream>
#include <vector>
#include <algorithm> // is used for 
#include <iomanip>

using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
};

// Sorts processes by Arrival Time
bool compareArrival(const Process& a, const Process& b) {
    if (a.arrivalTime == b.arrivalTime)
        return a.id < b.id;
    return a.arrivalTime < b.arrivalTime;
}

// Sorts final results by Process ID for clean presentation
bool compareID(const Process& a, const Process& b) {
    return a.id < b.id;
}

// Displays calculation data cleanly
void printMetrics(vector<Process> processes) {
    //This line means
    sort(processes.begin(), processes.end(), compareID);
    float totalWT = 0, totalTAT = 0;
    int n = processes.size();
    
    cout << "\n" << setfill('-') << setw(65) << "-" << setfill(' ') << "\n";
    cout << setw(5) << "PID" << setw(12) << "Arrival" << setw(10) << "Burst"
         << setw(15) << "Completion" << setw(12) << "Turnaround" << setw(10) << "Waiting" << "\n";
    cout << setfill('-') << setw(65) << "-" << setfill(' ') << "\n";

    for (const auto& p : processes) {
        totalWT += p.waitingTime;
        totalTAT += p.turnaroundTime;
        cout << setw(5) << p.id 
             << setw(12) << p.arrivalTime 
             << setw(10) << p.burstTime 
             << setw(15) << p.completionTime 
             << setw(12) << p.turnaroundTime 
             << setw(10) << p.waitingTime << "\n";
    }
    
    cout << setfill('-') << setw(65) << "-" << setfill(' ') << "\n";
    cout << fixed << setprecision(2);
    cout << "Average Waiting Time    : " << totalWT / n << "\n";
    cout << "Average Turnaround Time : " << totalTAT / n << "\n";
}

// 1. First-Come, First-Served (FCFS)
void findFCFS(vector<Process> processes) {
    sort(processes.begin(), processes.end(), compareArrival);
    int currentTime = 0;

    for (auto& p : processes) {
        if (currentTime < p.arrivalTime) {
            currentTime = p.arrivalTime;
        }
        p.completionTime = currentTime + p.burstTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
        currentTime = p.completionTime;
    }
    cout << "\n--- FCFS Scheduling Results ---";
    printMetrics(processes);
}

// 2. Shortest Job First (SJF)
void findSJF(vector<Process> processes) {
    int n = processes.size();
    vector<bool> isCompleted(n, false);
    int currentTime = 0, completedCount = 0;

    while (completedCount < n) {
        int idx = -1;
        int minBurst = 1e9;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && !isCompleted[i]) {
                if (processes[i].burstTime < minBurst) {
                    minBurst = processes[i].burstTime;
                    idx = i;
                } else if (processes[i].burstTime == minBurst) {
                    if (idx != -1 && processes[i].arrivalTime < processes[idx].arrivalTime) {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            processes[idx].completionTime = currentTime + processes[idx].burstTime;
            processes[idx].turnaroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
            processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
            
            currentTime = processes[idx].completionTime;
            isCompleted[idx] = true;
            completedCount++;
        } else {
            currentTime++;
        }
    }
    cout << "\n--- SJF (Non-Preemptive) Scheduling Results ---";
    printMetrics(processes);
}

// 3. Longest Job First (LJF)
void findLJF(vector<Process> processes) {
    int n = processes.size();
    vector<bool> isCompleted(n, false);
    int currentTime = 0, completedCount = 0;

    while (completedCount < n) {
        int idx = -1;
        int maxBurst = -1;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && !isCompleted[i]) {
                if (processes[i].burstTime > maxBurst) {
                    maxBurst = processes[i].burstTime;
                    idx = i;
                } else if (processes[i].burstTime == maxBurst) {
                    if (idx != -1 && processes[i].arrivalTime < processes[idx].arrivalTime) {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            processes[idx].completionTime = currentTime + processes[idx].burstTime;
            processes[idx].turnaroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
            processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
            
            currentTime = processes[idx].completionTime;
            isCompleted[idx] = true;
            completedCount++;
        } else {
            currentTime++;
        }
    }
    cout << "\n--- LJF (Non-Preemptive) Scheduling Results ---";
    printMetrics(processes);
}

int main() {
    vector<Process> processes = {
        {1, 0, 8},
        {2, 1, 4},
        {3, 2, 9},
        {4, 3, 5}
    };

    cout << "=== RUNNING NON-PREEMPTIVE ALGORITHMS ===" << endl;
    findFCFS(processes);
    findSJF(processes);
    findLJF(processes);

    return 0;
}
