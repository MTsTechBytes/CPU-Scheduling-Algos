// Non-Preemptive CPU Scheduling: FCFS, SJF, LJF
// SJF/LJF use the burst time of processes that have already arrived.
// A running process is never interrupted.

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    int id, at, bt, ct, tat, wt;
    bool done = false;
};

void calculate(vector<Process>& p, int choice) {
    int n = p.size(), time = 0, completed = 0;
    while (completed < n) {
        int best = -1;

        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].at <= time) {
                if (best == -1 ||
                    (choice == 1 && p[i].at < p[best].at) ||
                    (choice == 2 && p[i].bt < p[best].bt) ||
                    (choice == 3 && p[i].bt > p[best].bt)) {
                    best = i;
                }
            }
        }

        if (best == -1) { time++; continue; } // CPU is idle

        time += p[best].bt;
        p[best].ct = time;
        p[best].tat = p[best].ct - p[best].at;
        p[best].wt = p[best].tat - p[best].bt;
        p[best].done = true;
        completed++;
    }
}

int main() {
    int n, choice;
    cout << "1. FCFS\n2. SJF\n3. LJF\nChoose algorithm: ";
    cin >> choice;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cout << "P" << i + 1 << " Arrival Time: ";
        cin >> p[i].at;
        cout << "P" << i + 1 << " Burst Time: ";
        cin >> p[i].bt;
    }

    if (choice < 1 || choice > 3 || n <= 0) {
        cout << "Invalid input.\n";
        return 0;
    }

    calculate(p, choice);

    cout << "\nProcess\tAT\tBT\tCT\tTAT\tWT\n";
    double totalWT = 0, totalTAT = 0;
    for (auto x : p) {
        cout << "P" << x.id << "\t" << x.at << "\t" << x.bt << "\t"
             << x.ct << "\t" << x.tat << "\t" << x.wt << "\n";
        totalWT += x.wt;
        totalTAT += x.tat;
    }
    cout << fixed << setprecision(2);
    cout << "Average WT  = " << totalWT / n << "\n";
    cout << "Average TAT = " << totalTAT / n << "\n";
    return 0;
}
