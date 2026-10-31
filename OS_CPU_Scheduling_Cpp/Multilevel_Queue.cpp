// Multilevel Queue Scheduling
// Queue 1 has higher priority and uses Round Robin (time quantum = 2).
// Queue 2 has lower priority and uses FCFS.
// A process stays in its assigned queue. Queue 1 always runs before Queue 2.

#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
using namespace std;

struct Process {
    int id, at, bt, remaining, q, ct = 0, tat = 0, wt = 0;
};

int main() {
    int n;
    const int quantum = 2;
    cout << "Multilevel Queue: Q1 = Round Robin (TQ=2), Q2 = FCFS\n";
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
        cout << "P" << i + 1 << " Queue (1 or 2): ";
        cin >> p[i].q;
        p[i].remaining = p[i].bt;
        if (p[i].q != 1 && p[i].q != 2) {
            cout << "Queue must be 1 or 2.\n";
            return 0;
        }
    }

    queue<int> q1, q2;
    vector<bool> added(n, false);
    int time = 0, completed = 0;

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (!added[i] && p[i].at <= time) {
                if (p[i].q == 1) q1.push(i);
                else q2.push(i);
                added[i] = true;
            }
        }

        if (!q1.empty()) {
            int i = q1.front(); q1.pop();
            int run = min(quantum, p[i].remaining);
            for (int t = 0; t < run; t++) {
                time++;
                p[i].remaining--;
                for (int j = 0; j < n; j++) {
                    if (!added[j] && p[j].at <= time) {
                        if (p[j].q == 1) q1.push(j);
                        else q2.push(j);
                        added[j] = true;
                    }
                }
            }
            if (p[i].remaining == 0) {
                p[i].ct = time;
                completed++;
            } else q1.push(i);
        }
        else if (!q2.empty()) {
            int i = q2.front(); q2.pop();
            // FCFS process runs until completion. Q1 jobs arriving meanwhile wait.
            time += p[i].remaining;
            p[i].remaining = 0;
            p[i].ct = time;
            completed++;
        }
        else time++;
    }

    cout << "\nProcess\tQueue\tAT\tBT\tCT\tTAT\tWT\n";
    double totalWT = 0, totalTAT = 0;
    for (auto x : p) {
        x.tat = x.ct - x.at;
        x.wt = x.tat - x.bt;
        cout << "P" << x.id << "\tQ" << x.q << "\t" << x.at << "\t"
             << x.bt << "\t" << x.ct << "\t" << x.tat << "\t" << x.wt << "\n";
        totalWT += x.wt;
        totalTAT += x.tat;
    }
    cout << fixed << setprecision(2);
    cout << "Average WT  = " << totalWT / n << "\n";
    cout << "Average TAT = " << totalTAT / n << "\n";
    return 0;
}
