#include <iostream>
#include <vector>

using namespace std;

// Process ki details store karne ke liye struct
struct Process {
    int id;       // Process ID (e.g., 1, 2, 3)
    int at;       // Arrival Time
    int bt;       // Burst Time
    int ct = 0;   // Completion Time
    int tat = 0;  // Turnaround Time
    int wt = 0;   // Waiting Time
    bool done = false; // Process khatam hua ya nahi
};

void calculateSJF(vector<Process>& p) {
    int n = p.size();
    int time = 0;
    int completed = 0;

    while (completed < n) {
        int best = -1;

        // Ready queue mein se sab se chhota (Shortest) process dhoondna
        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].at <= time) {
                // Agar pehla valid process mila ya koi aisa process mila jiska Burst Time kam hai
                //LJF mein convert krne keliye bs "<" ki jgah ">" lgana hai
                if (best == -1 || p[i].bt < p[best].bt) {
                    best = i;
                }
            }
        }

        // Agar us time par koi process nahi aaya, to CPU idle rahega aur time badhega
        if (best == -1) {
            time++;
            continue;
        }

        // Process ko execute karna aur calculations
        time += p[best].bt;
        p[best].ct = time;
        p[best].tat = p[best].ct - p[best].at;
        p[best].wt = p[best].tat - p[best].bt;
        p[best].done = true;
        completed++;
    }
}

int main() {
    int n;
    cout << "Total kitne processes hain? ";
    cin >> n;

    vector<Process> p(n);

    // User se input lena
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cout << "\nProcess P" << p[i].id << " ka Arrival Time aur Burst Time enter karein: ";
        cin >> p[i].at >> p[i].bt;
    }

    // SJF function ko call karna
    calculateSJF(p);

    // Results print karna
    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << p[i].id << "\t" 
             << p[i].at << "\t" 
             << p[i].bt << "\t" 
             << p[i].ct << "\t" 
             << p[i].tat << "\t" 
             << p[i].wt << "\n";
    }

    return 0;
}
