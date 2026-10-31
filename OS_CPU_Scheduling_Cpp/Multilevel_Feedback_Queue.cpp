// Multilevel Feedback Queue (MLFQ)
// Three queues: Q1 = RR (TQ 2), Q2 = RR (TQ 4), Q3 = FCFS.
// All processes enter Q1. If a process uses its full quantum, it moves down.
// A process that finishes early leaves the system. Higher queues have priority.

#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
using namespace std;

struct Process {
    int id, at, bt, remaining, level = 0, ct = 0;
};

int main() {
    int n;
    cout << "MLFQ: Q1=RR(TQ 2), Q2=RR(TQ 4), Q3=FCFS\n";
    cout << "Enter number of processes: ";
    cin >> n;
    if (n <= 0) { cout << "Invalid number of processes.\n"; return 0; }

    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cout << "P" << i + 1 << " Arrival Time: ";
        cin >> p[i].at;
        cout << "P" << i + 1 << " Burst Time: ";
        cin >> p[i].bt;
        p[i].remaining = p[i].bt;
    }

    queue<int> q[3];
    vector<bool> added(n, false);
    int quantum[2] = {2, 4};
    int time = 0, completed = 0;

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (!added[i] && p[i].at <= time) {
                q[0].push(i);
                added[i] = true;
            }
        }

        int level = -1;
        for (int k = 0; k < 3; k++) {
            if (!q[k].empty()) { level = k; break; }
        }

        if (level == -1) { time++; continue; }

        int i = q[level].front();
        q[level].pop();

        if (level == 2) {
            // Lowest queue: FCFS, run until completion.
            time += p[i].remaining;
            p[i].remaining = 0;
            p[i].ct = time;
            completed++;
        } else {
            int run = min(quantum[level], p[i].remaining);
            for (int t = 0; t < run; t++) {
                time++;
                p[i].remaining--;
                // New arrivals always enter the top queue.
                for (int j = 0; j < n; j++) {
                    if (!added[j] && p[j].at <= time) {
                        q[0].push(j);
                        added[j] = true;
                    }
            }
            }
            if (p[i].remaining == 0) {
                p[i].ct = time;
                completed++;
            } else {
                p[i].level = level + 1;
                q[level + 1].push(i);
            }
        }
    }

    cout << "\nProcess\tFinal Queue\tAT\tBT\tCT\tTAT\tWT\n";
    double totalWT = 0, totalTAT = 0;
    for (auto x : p) {
        int tat = x.ct - x.at;
        int wt = tat - x.bt;
        cout << "P" << x.id << "\tQ" << (x.level + 1) << "\t\t"
             << x.at << "\t" << x.bt << "\t" << x.ct << "\t"
             << tat << "\t" << wt << "\n";
        totalWT += wt;
        totalTAT += tat;
    }
    cout << fixed << setprecision(2);
    cout << "Average WT  = " << totalWT / n << "\n";
    cout << "Average TAT = " << totalTAT / n << "\n";
    return 0;
}
