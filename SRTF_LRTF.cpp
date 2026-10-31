#include <iostream>
#include <vector>
#include <climits>

using namespace std;

// Process ki details store karne ke liye struct
struct Process {
    int id;       // Process ID
    int at;       // Arrival Time
    int bt;       // Total Burst Time
    int rt;       // Remaining Time (Preemptive ke liye zaroori hai)
    int ct = 0;   // Completion Time
    int tat = 0;  // Turnaround Time
    int wt = 0;   // Waiting Time
    bool done = false; 
};

// Function 1: User se inputs lene ke liye
void inputProcesses(vector<Process>& p) {
    for (int i = 0; i < p.size(); i++) {
        p[i].id = i + 1;
        cout << "\nProcess P" << p[i].id << " ka Arrival Time aur Burst Time enter karein: ";
        cin >> p[i].at >> p[i].bt;
        p[i].rt = p[i].bt; // Shuru mein remaining time burst time ke barabar hoga
    }
}

// Function 2: Preemptive SJF (SRTF) Algorithm
void calculatePreemptiveSJF(vector<Process>& p) {
    int n = p.size();
    int time = 0;
    int completed = 0;

    while (completed < n) {
        int best = -1;
        int min_rt = INT_MAX; // Chota remaining time dhoondne ke liye max value set ki

        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].at <= time) {
                // Jis process ka Remaining Time (rt) sab se kam ho
                if (p[i].rt < min_rt) {
                    min_rt = p[i].rt;
                    best = i;
                }
            }
        }

        if (best == -1) {
            time++; // CPU Idle
            continue;
        }

        // PREEMPTIVE LOGIC: Process ko sirf 1 second ke liye chalayein
        p[best].rt--;
        time++;

        // Agar process poora khatam ho gaya
        if (p[best].rt == 0) {
            p[best].ct = time;
            p[best].tat = p[best].ct - p[best].at;
            p[best].wt = p[best].tat - p[best].bt;
            p[best].done = true;
            completed++;
        }
    }
}

// Function 3: Preemptive LJF (LRTF) Algorithm
void calculatePreemptiveLJF(vector<Process>& p) {
    int n = p.size();
    int time = 0;
    int completed = 0;

    while (completed < n) {
        int best = -1;
        int max_rt = INT_MIN; // Bada remaining time dhoondne ke liye min value set ki

        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].at <= time) {
                // Jis process ka Remaining Time (rt) sab se zyada ho
                if (p[i].rt > max_rt) {
                    max_rt = p[i].rt;
                    best = i;
                }
            }
        }

        if (best == -1) {
            time++; // CPU Idle
            continue;
        }

        // PREEMPTIVE LOGIC: Process ko sirf 1 second ke liye chalayein
        p[best].rt--;
        time++;

        // Agar process poora khatam ho gaya
        if (p[best].rt == 0) {
            p[best].ct = time;
            p[best].tat = p[best].ct - p[best].at;
            p[best].wt = p[best].tat - p[best].bt;
            p[best].done = true;
            completed++;
        }
    }
}

// Function 4: Results ko table form mein print karne ke liye
void displayResults(const vector<Process>& p) {
    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    for (int i = 0; i < p.size(); i++) {
        cout << "P" << p[i].id << "\t" 
             << p[i].at << "\t" 
             << p[i].bt << "\t" 
             << p[i].ct << "\t" 
             << p[i].tat << "\t" 
             << p[i].wt << "\n";
    }
}

int main() {
    int n, choice;
    cout << "Total kitne processes hain? ";
    cin >> n;

    vector<Process> p(n);
    inputProcesses(p);

    cout << "\nKaun sa Preemptive algorithm chalana hai?\n1. Preemptive SJF (SRTF)\n2. Preemptive LJF (LRTF)\nEnter choice: ";
    cin >> choice;

    if (choice == 1) {
        calculatePreemptiveSJF(p);
        cout << "\n--- Preemptive SJF (SRTF) Results ---";
    } else if (choice == 2) {
        calculatePreemptiveLJF(p);
        cout << "\n--- Preemptive LJF (LRTF) Results ---";
    } else {
        cout << "Invalid Choice!";
        return 0;
    }

    displayResults(p);
    return 0;
}
