// Preemptive CPU Scheduling: SRTF, LRTF, Round Robin, Priority
// Priority rule: smaller priority number means higher priority.
// For SRTF/LRTF/Priority, ties are resolved by the earlier process in the input.

#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
using namespace std;

struct Process {
    int id, at, bt, remaining, priority, ct = 0, tat = 0, wt = 0, rt = -1;
};

int main() {
    int n, choice;
    cout << "1. SRTF\n2. LRTF\n3. Round Robin\n4. Priority (Preemptive)\n";
    cout << "Choose algorithm: ";
    cin >> choice;
    cout << "Enter number of processes: ";
    cin >> n;
    if (n <= 0 || choice < 1 || choice > 4) {
        cout << "Invalid input.\n";
        return 0;
    }

    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cout << "P" << i + 1 << " Arrival Time: ";
        cin >> p[i].at;
        cout << "P" << i + 1 << " Burst Time: ";
        cin >> p[i].bt;
        p[i].remaining = p[i].bt;
        if (choice == 4) {
            cout << "P" << i + 1 << " Priority (smaller = higher): ";
            cin >> p[i].priority;
        }
    }

    int quantum = 0;
    if (choice == 3) {
        cout << "Enter time quantum: ";
        cin >> quantum;
        if (quantum <= 0) { cout << "Invalid quantum.\n"; return 0; }
    }

    int time = 0, completed = 0;
    queue<int> ready;
    vector<bool> added(n, false);

    // Round Robin simulation
    if (choice == 3) {
        while (completed < n) {
            for (int i = 0; i < n; i++)
                if (!added[i] && p[i].at <= time) {
                    ready.push(i);
                    added[i] = true;
                }

            if (ready.empty()) { time++; continue; }

            int i = ready.front(); ready.pop();
            if (p[i].rt == -1) p[i].rt = time;
            int run = min(quantum, p[i].remaining);

            for (int t = 0; t < run; t++) {
                time++;
                p[i].remaining--;
                for (int j = 0; j < n; j++)
                    if (!added[j] && p[j].at <= time) {
                        ready.push(j);
                        added[j] = true;
                    }
            }

            if (p[i].remaining == 0) {
                p[i].ct = time;
                completed++;
            } else ready.push(i);
        }
    }
    // SRTF, LRTF and preemptive Priority
    else {
        while (completed < n) {
            int best = -1;
            for (int i = 0; i < n; i++) {
                if (p[i].at <= time && p[i].remaining > 0) {
                    if (best == -1) best = i;
                    else if (choice == 1 && p[i].remaining < p[best].remaining) best = i;
                    else if (choice == 2 && p[i].remaining > p[best].remaining) best = i;
                    else if (choice == 4 && p[i].priority < p[best].priority) best = i;
                }
            }

            if (best == -1) { time++; continue; }

            if (p[best].rt == -1) p[best].rt = time - p[best].at;
            p[best].remaining--;
            time++;

            if (p[best].remaining == 0) {
                p[best].ct = time;
                completed++;
            }
        }
    }

    cout << "\nProcess\tAT\tBT";
    if (choice == 4) cout << "\tPriority";
    cout << "\tCT\tTAT\tWT\tRT\n";

    double totalWT = 0, totalTAT = 0;
    for (auto x : p) {
        x.tat = x.ct - x.at;
        x.wt = x.tat - x.bt;
        cout << "P" << x.id << "\t" << x.at << "\t" << x.bt << "\t";
        if (choice == 4) cout << x.priority << "\t";
        cout << x.ct << "\t" << x.tat << "\t" << x.wt << "\t" << x.rt << "\n";
        totalWT += x.wt;
        totalTAT += x.tat;
    }
    cout << fixed << setprecision(2);
    cout << "Average WT  = " << totalWT / n << "\n";
    cout << "Average TAT = " << totalTAT / n << "\n";
    return 0;
}
