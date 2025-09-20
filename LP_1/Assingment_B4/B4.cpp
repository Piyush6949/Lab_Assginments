#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

enum class Strategy {
    LRU,
    MRU,
    FIFO
};

// Helper to print frequencies of only the pages in current frames
string getFrameFrequencies(const vector<int>& frameList, const unordered_map<int, int>& frequency) {
    string result = "{";
    for (size_t i = 0; i < frameList.size(); ++i) {
        int page = frameList[i];
        result += to_string(page) + "=" + to_string(frequency.at(page));
        if (i != frameList.size() - 1)
            result += ", ";
    }
    result += "}";
    return result;
}

void simulate(const vector<int>& referenceString, int frames, Strategy strat) {
    vector<int> frameList;
    queue<int> fifoQueue;
    unordered_map<int, int> frequency; // Track frequency of each page

    int pageFaults = 0, hits = 0;

    cout << "\n--- ";
    switch (strat) {
        case Strategy::LRU: cout << "LRU"; break;
        case Strategy::MRU: cout << "MRU"; break;
        case Strategy::FIFO: cout << "FIFO"; break;
    }
    cout << " Page Replacement Simulation ---\n\n";

    cout << left << setw(10) << "Reference"
         << setw(25) << "Frames"
         << setw(30) << "Frequencies"
         << setw(25) << "Action"
         << setw(12) << "Status" << "\n";

    for (int page : referenceString) {
        frequency[page]++;

        auto it = find(frameList.begin(), frameList.end(), page);
        if (it != frameList.end()) {
            hits++;
            string action = "Page HIT";

            if (strat == Strategy::LRU || strat == Strategy::MRU) {
                frameList.erase(it);
                frameList.push_back(page); // most recently used
            }

            cout << left << setw(10) << page
                 << setw(25) << "[" + to_string(frameList[0]) + ((frameList.size() > 1) ? ", ..." : "") + "]"
                 << setw(30) << getFrameFrequencies(frameList, frequency)
                 << setw(25) << action
                 << setw(12) << "HIT" << "\n";
        } else {
            pageFaults++;
            string action;

            if (frameList.size() < frames) {
                frameList.push_back(page);
                fifoQueue.push(page);
                action = "Added " + to_string(page) + " (empty slot)";
            } else {
                int victim;

                if (strat == Strategy::LRU) {
                    victim = frameList.front(); // least recently used
                    frameList.erase(frameList.begin());
                    frameList.push_back(page);
                    action = "Replaced (LRU) " + to_string(victim) + " with " + to_string(page);
                } else if (strat == Strategy::MRU) {
                    victim = frameList.back(); // most recently used
                    frameList.pop_back();
                    frameList.push_back(page);
                    action = "Replaced (MRU) " + to_string(victim) + " with " + to_string(page);
                } else { // FIFO
                    victim = fifoQueue.front();
                    fifoQueue.pop();
                    frameList.erase(remove(frameList.begin(), frameList.end(), victim), frameList.end());
                    frameList.push_back(page);
                    fifoQueue.push(page);
                    action = "Replaced (FIFO) " + to_string(victim) + " with " + to_string(page);
                }
            }

            cout << left << setw(10) << page
                 << setw(25) << "[" + to_string(frameList[0]) + ((frameList.size() > 1) ? ", ..." : "") + "]"
                 << setw(30) << getFrameFrequencies(frameList, frequency)
                 << setw(25) << action
                 << setw(12) << "FAULT" << "\n";
        }
    }

    cout << "\n--- Summary ---\n";
    cout << "Total Page Faults: " << pageFaults << "\n";
    cout << "Total Hits: " << hits << "\n";
    double hitRatio = static_cast<double>(hits) / referenceString.size();
    double faultRatio = static_cast<double>(pageFaults) / referenceString.size();
    cout << fixed << setprecision(2);
    cout << "Hit Ratio: " << hitRatio << " | Fault Ratio: " << faultRatio << "\n";
}

int main() {
    int frames, n;
    cout << "Enter number of frames: ";
    cin >> frames;

    cout << "Enter length of reference string: ";
    cin >> n;

    vector<int> referenceString(n);
    cout << "Enter reference string (space separated): ";
    for (int i = 0; i < n; ++i) {
        cin >> referenceString[i];
    }

    while (true) {
        cout << "\nChoose Page Replacement Strategy:\n";
        cout << "1. LRU\n";
        cout << "2. MRU\n";
        cout << "3. FIFO\n";
        cout << "4. Exit\n";
        cout << "Enter choice [1-4]: ";
        int choice;
        cin >> choice;

        if (choice == 4) break;

        Strategy strat;
        switch (choice) {
            case 1: strat = Strategy::LRU; break;
            case 2: strat = Strategy::MRU; break;
            case 3: strat = Strategy::FIFO; break;
            default:
                cout << "Invalid choice!\n";
                continue;
        }

        simulate(referenceString, frames, strat);
    }

    return 0;
}
