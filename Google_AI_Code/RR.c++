#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Process {
    int id;
    int at;       // Arrival Time
    int bt;       // Burst Time
    int rt;       // Remaining Time
    int ct = 0;   // Completion Time
    int tat = 0;  // Turnaround Time
    int wt = 0;   // Waiting Time
};

// Function 1: Inputs lene ke liye
void inputProcesses(vector<Process>& p) {
    for (int i = 0; i < p.size(); i++) {
        p[i].id = i + 1;
        cout << "\nProcess P" << p[i].id << " ka Arrival Time aur Burst Time enter karein: ";
        cin >> p[i].at >> p[i].bt;
        p[i].rt = p[i].bt; // Shuru mein remaining time burst time ke barabar hoga
    }
}

// Helper Function: Arrival Time ke mutabiq sort karne ke liye (ताके ready queue sahi sequence mein shuru ho)
bool compareArrival(Process a, Process b) {
    return a.at < b.at;
}

// Function 2: Round Robin Algorithm
void calculateRoundRobin(vector<Process>& p, int tq) {
    int n = p.size();
    
    // Asal vector ko change kiye bagair index track karne ke liye sorting zaroori hai
    // Lekin simple rakhne ke liye hum assume kar rahe hain processes sorted hain ya hum ready queue ko carefully manage karenge.
    sort(p.begin(), p.end(), compareArrival);

    queue<int> q; // Ready Queue jo sirf processes ke indices (0, 1, 2...) store karegi
    vector<bool> in_queue(n, false); // Track rakhne ke liye ki kaun sa process queue mein hai

    int time = 0;
    int completed = 0;

    // Sab se pehle aane wale process ko queue mein dalna
    q.push(0);
    in_queue[0] = true;

    while (completed < n) {
        // Agar queue khali hai lekin abhi saare process khatam nahi hue (CPU Idle state)
        if (q.empty()) {
            time++;
            // Check karein ki kya is naye time par koi process aaya hai
            for (int i = 0; i < n; i++) {
                if (p[i].at <= time && p[i].rt > 0 && !in_queue[i]) {
                    q.push(i);
                    in_queue[i] = true;
                    break;
                }
            }
            continue;
        }

        int curr = q.front(); // Queue ke shuru se process nikalna
        q.pop();

        // Execution logic based on Time Quantum
        if (p[curr].rt > tq) {
            time += tq;
            p[curr].rt -= tq;
        } else {
            // Agar remaining time time quantum se kam ya barabar hai
            time += p[curr].rt;
            p[curr].rt = 0;
            p[curr].ct = time;
            p[curr].tat = p[curr].ct - p[curr].at;
            p[curr].wt = p[curr].tat - p[curr].bt;
            completed++;
        }

        // 1. Pehle check karein ki is dauran kaun se naye processes aa chuke hain unhe queue mein dalein
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0 && !in_queue[i] && i != curr) {
                q.push(i);
                in_queue[i] = true;
            }
        }

        // 2. Agar current process abhi baaki hai, to use naye processes ke BAAD queue ke aakhri mein dobara dalein
        if (p[curr].rt > 0) {
            q.push(curr);
        }
    }
}

// Function 3: Print karne ke liye
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
    int n, tq;
    cout << "Total kitne processes hain? ";
    cin >> n;

    vector<Process> p(n);
    inputProcesses(p);

    cout << "Time Quantum (TQ) enter karein: ";
    cin >> tq;

    calculateRoundRobin(p, tq);

    cout << "\n--- Round Robin (RR) Results ---";
    displayResults(p);

    return 0;
}
