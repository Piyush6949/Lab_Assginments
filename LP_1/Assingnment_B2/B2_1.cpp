#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <thread>
#include <chrono>
#include <iomanip>
#include <climits>

using namespace std;

struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;
    int startTime = -1;
    int completionTime = 0;
    int waitingTime = 0;
    int turnaroundTime = 0;
    bool isCompleted = false;

    Process(int id, int at, int bt, int pr = 0)
        : pid(id), arrivalTime(at), burstTime(bt), remainingTime(bt), priority(pr) {}
};

void printStates(const vector<Process>& processes, const Process* current, int time) {
    cout << "Time " << time << ": ";
    for (const auto& p : processes) {
        if (&p == current) {
            cout << "[P" << p.pid << ": RUNNING] ";
        } else if (!p.isCompleted && p.arrivalTime <= time) {
            cout << "[P" << p.pid << ": WAITING] ";
        } else if(p.isCompleted){
            cout << "[P" << p.pid << ": COMPLETED] ";
        }
         else {
            cout << "[P" << p.pid << ": IDLE] ";
        }
    }
    cout << endl;
}

void printSummary(const vector<Process>& processes, bool showPriority) {
    cout << "--- Summary ---" << endl;
    if (showPriority) {
        cout << left << setw(6) << "PID" << setw(10) << "Arrival" << setw(10) << "Burst"
             << setw(10) << "Priority" << setw(10) << "Start" << setw(12)
             << "Turnaround" << setw(10) << "Waiting" << endl;

        for (const auto& p : processes) {
            cout << "P" << left << setw(5) << p.pid << setw(10) << p.arrivalTime << setw(10)
                 << p.burstTime << setw(10) << p.priority << setw(10) << p.startTime
                 << setw(12) << p.turnaroundTime << setw(10) << p.waitingTime << endl;
        }
    } else {
        cout << left << setw(6) << "PID" << setw(10) << "Arrival" << setw(10)
             << "Burst" << setw(10) << "Start" << setw(12) << "Turnaround"
             << setw(10) << "Waiting" << endl;

        for (const auto& p : processes) {
            cout << "P" << left << setw(5) << p.pid << setw(10) << p.arrivalTime
                 << setw(10) << p.burstTime << setw(10) << p.startTime
                 << setw(12) << p.turnaroundTime << setw(10) << p.waitingTime << endl;
        }
    }
}

void simulateFCFS(vector<Process> processes) {
    sort(processes.begin(), processes.end(), [](Process& a, Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int time = 0, completed = 0, index = 0;
    cout << "\n--- FCFS Runtime Simulation ---\n\n";

    while (completed < (int)processes.size()) {
        bool found = false;

        while (index < (int)processes.size() && processes[index].arrivalTime <= time && processes[index].isCompleted)
            index++;

        if (index < (int)processes.size() && processes[index].arrivalTime <= time) {
            Process& current = processes[index];
            found = true;

            if (current.startTime == -1) {
                current.startTime = time;
                cout << "Time " << time << ": Process P" << current.pid << " started." << endl;
            }

            current.remainingTime--;
            printStates(processes, &current, time);

            if (current.remainingTime == 0) {
                current.isCompleted = true;
                current.completionTime = time + 1;
                current.turnaroundTime = current.completionTime - current.arrivalTime;
                current.waitingTime = current.turnaroundTime - current.burstTime;
                cout << "Time " << (time + 1) << ": Process P" << current.pid << " completed.\n" << endl;
                completed++;
                index++;
            }
        } else {
            cout << "Time " << time << ": CPU is IDLE." << endl;
        }

        this_thread::sleep_for(chrono::milliseconds(500));
        time++;
    }

    printSummary(processes, false);
}

void simulatePriority(vector<Process> processes) {
    sort(processes.begin(), processes.end(), [](Process& a, Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int time = 0, completed = 0;
    Process* current = nullptr;

    cout << "\n--- Non-Preemptive Priority Scheduling Simulation ---\n\n";

    while (completed < (int)processes.size()) {
        if (current == nullptr) {
            Process* highest = nullptr;
            int bestPriority = INT_MAX;

            for (auto& p : processes) {
                if (!p.isCompleted && p.arrivalTime <= time && p.priority < bestPriority) {
                    highest = &p;
                    bestPriority = p.priority;
                }
            }

            if (!highest) {
                cout << "Time " << time << ": CPU is IDLE." << endl;
                time++;
                this_thread::sleep_for(chrono::milliseconds(500));
                continue;
            }

            current = highest;
            if (current->startTime == -1)
                current->startTime = time;
            cout << "Time " << time << ": Process P" << current->pid << " started." << endl;
        }

        printStates(processes, current, time);

        current->remainingTime--;
        time++;

        if (current->remainingTime == 0) {
            current->isCompleted = true;
            current->completionTime = time;
            current->turnaroundTime = time - current->arrivalTime;
            current->waitingTime = current->turnaroundTime - current->burstTime;
            cout << "Time " << time << ": Process P" << current->pid << " completed.\n" << endl;
            completed++;
            current = nullptr;  
        }

        this_thread::sleep_for(chrono::milliseconds(500));
    }

    printSummary(processes, true);
}


void simulateRoundRobin(vector<Process> processes, int quantum) {
    sort(processes.begin(), processes.end(), [](Process& a, Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    queue<Process*> q;
    int time = 0, completed = 0;
    vector<bool> added(processes.size(), false);

    cout << "\n--- Round Robin Scheduling Simulation ---\n\n";

    while (completed < (int)processes.size()) {
        for (int i = 0; i < (int)processes.size(); ++i) {
            if (!added[i] && processes[i].arrivalTime <= time) {
                q.push(&processes[i]);
                added[i] = true;
            }
        }

        if (q.empty()) {
            cout << "Time " << time << ": CPU is IDLE." << endl;
            time++;
            this_thread::sleep_for(chrono::milliseconds(500));
            continue;
        }

        Process* current = q.front(); q.pop();
        if (current->startTime == -1) current->startTime = time;

        int execTime = min(quantum, current->remainingTime);
        cout << "Time " << time << ": Process P" << current->pid << " executing for " << execTime << " unit(s)." << endl;

        for (int i = 0; i < execTime; ++i) {
            printStates(processes, current, time + i);
            this_thread::sleep_for(chrono::milliseconds(500));
        }

        time += execTime;
        current->remainingTime -= execTime;

        for (int i = 0; i < (int)processes.size(); ++i) {
            if (!added[i] && processes[i].arrivalTime <= time) {
                q.push(&processes[i]);
                added[i] = true;
            }
        }

        if (current->remainingTime == 0) {
            current->isCompleted = true;
            current->completionTime = time;
            current->turnaroundTime = time - current->arrivalTime;
            current->waitingTime = current->turnaroundTime - current->burstTime;
            cout << "Time " << time << ": Process P" << current->pid << " completed.\n" << endl;
            completed++;
        } else {
            q.push(current);
        }
    }

    printSummary(processes, false);
}

void simulateSJF(vector<Process> processes) {
    sort(processes.begin(), processes.end(), [](Process& a, Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int time = 0, completed = 0;
    Process* current = nullptr;

    cout << "\n--- Preemptive SJF (SRTF) Scheduling Simulation ---\n\n";

    while (completed < (int)processes.size()) {
        Process* shortest = nullptr;
        int minRemaining = INT_MAX;

        for (auto& p : processes) {
            if (!p.isCompleted && p.arrivalTime <= time && p.remainingTime < minRemaining) {
                minRemaining = p.remainingTime;
                shortest = &p;
            }
        }

        if (!shortest) {
            cout << "Time " << time << ": CPU is IDLE." << endl;
            time++;
            this_thread::sleep_for(chrono::milliseconds(500));
            continue;
        }

        if (current != shortest) {
            current = shortest;
            if (current->startTime == -1)
                current->startTime = time;
            cout << "Time " << time << ": Process P" << current->pid << " started/resumed." << endl;
        }

        printStates(processes, current, time);
        current->remainingTime--;
        time++;

        if (current->remainingTime == 0) {
            current->isCompleted = true;
            current->completionTime = time;
            current->turnaroundTime = time - current->arrivalTime;
            current->waitingTime = current->turnaroundTime - current->burstTime;
            cout << "Time " << time << ": Process P" << current->pid << " completed.\n" << endl;
            completed++;
            current = nullptr;
        }

        this_thread::sleep_for(chrono::milliseconds(500));
    }

    printSummary(processes, false);
}

int main() {
    while (true) {
        cout << "\nCPU Scheduling Simulator" << endl;
        cout << "1. FCFS" << endl;
        cout << "2. Priority Scheduling" << endl;
        cout << "3. Round Robin" << endl;
        cout << "4. Preemptive SJF" << endl;
        cout << "5. Exit" << endl;
        cout << "Choose an algorithm (1-5): ";
        int choice;
        cin >> choice;

        if (choice == 5) {
            cout << "Exiting..." << endl;
            break;
        }

        cout << "Enter number of processes: ";
        int n;
        cin >> n;

        vector<Process> processes;
        for (int i = 0; i < n; ++i) {
            int at, bt, pr = 0;
            cout << "Enter arrival time for Process P" << (i + 1) << ": ";
            cin >> at;
            cout << "Enter burst time for Process P" << (i + 1) << ": ";
            cin >> bt;
            if (choice == 2) {
                cout << "Enter priority for Process P" << (i + 1) ;
                cin >> pr;
            }
            processes.emplace_back(i + 1, at, bt, pr);
        }

        if (choice == 1) {
            simulateFCFS(processes);
        } else if (choice == 2) {
            simulatePriority(processes);
        } else if (choice == 3) {
            cout << "Enter time quantum: ";
            int quantum;
            cin >> quantum;
            simulateRoundRobin(processes, quantum);
        } else if (choice == 4) {
            simulateSJF(processes);
        } else {
            cout << "Invalid choice." << endl;
        }
    }

    return 0;
}
