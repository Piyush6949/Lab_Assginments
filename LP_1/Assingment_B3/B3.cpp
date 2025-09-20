// Write a program to simulate memory placement strategies - bestfit,firstfit,nextfit,worstfit

#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

class Process
{
public:
    int processSize;
    int processNo;
    int num = 1;
    int blockNo;

    Process(int s, int pnum)
    {
        processSize = s;
        blockNo = -1;
        processNo = pnum;
    }
};

vector<Process> create_Process(int n)
{
    vector<Process> processes;
    for (int i = 1; i <= n; i++)
    {
        cout << "Enter the Size of the Process p" << i << " : ";
        int size;
        cin >> size;
        Process p(size, i);
        processes.push_back(p);
    }
    return processes;
}

vector<int> create_Blocks(int n)
{
    vector<int> Blocks;
    for (int i = 0; i < n; i++)
    {
        cout << "Enter the size of the Block : ";
        int blockSize;
        cin >> blockSize;
        Blocks.push_back(blockSize);
    }
    return Blocks;
}
void printOutput(vector<Process> &p, vector<int> &b)
{
    cout << "Process no, Process size ,Block number" << endl;
    for (auto &proc : p)
    {
        cout << proc.processNo << setw(15) << proc.processSize << setw(15) << proc.blockNo << endl;
    }
    cout << "Memory Remaining" << endl;
    for (auto &block : b)
    {
        cout << block << " ";
    }
    cout << endl;
}
class memory_placement_strategies
{
public:
    void firstFit(vector<Process> &processes, vector<int> &blocks)
    {
        for (auto &i : processes)
        {
            for (int j = 0; j < blocks.size(); j++)
            {
                if (blocks[j] >= i.processSize)
                {
                    blocks[j] -= i.processSize;
                    i.blockNo = j + 1;
                    break;
                }
            }
        }
        printOutput(processes, blocks);
    }

    void bestFit(vector<Process> &processes, vector<int> &blocks)
    {
        for (auto &proc : processes)
        {
            int bestIdx = -1;
            for (int j = 0; j < blocks.size(); j++)
            {
                if (blocks[j] >= proc.processSize)
                {
                    if (bestIdx == -1 || blocks[j] < blocks[bestIdx])
                    {
                        bestIdx = j;
                    }
                }
            }
            if (bestIdx != -1)
            {
                blocks[bestIdx] -= proc.processSize;
                proc.blockNo = bestIdx + 1;
            }
        }
        printOutput(processes, blocks);
    }

    void nextFit(vector<Process> &processes, vector<int> &blocks)
    {
        int lastAllocatedIndex = 0;
        int n = blocks.size();

        for (auto &proc : processes)
        {
            int count = 0;
            bool allocated = false;
            int j = lastAllocatedIndex;

            while (count < n)
            {
                if (blocks[j] >= proc.processSize)
                {
                    blocks[j] -= proc.processSize;
                    proc.blockNo = j + 1;
                    lastAllocatedIndex = j;
                    allocated = true;
                    break;
                }
                j = (j + 1) % n;
                count++;
            }
        }
        printOutput(processes, blocks);
    }

    void worstFit(vector<Process> &processes, vector<int> &blocks)
    {
        for (auto &proc : processes)
        {
            int worstIdx = -1;
            for (int j = 0; j < blocks.size(); j++)
            {
                if (blocks[j] >= proc.processSize)
                {
                    if (worstIdx == -1 || blocks[j] > blocks[worstIdx])
                    {
                        worstIdx = j;
                    }
                }
            }
            if (worstIdx != -1)
            {
                blocks[worstIdx] -= proc.processSize;
                proc.blockNo = worstIdx + 1;
            }
        }
        printOutput(processes, blocks);
    }
};

int main()
{
    cout << "Enter Number of Blocks : ";
    int numofBlocks;
    cin >> numofBlocks;
    vector<int> Blocks = create_Blocks(numofBlocks);

    cout << "Enter Number of Processes : ";
    int numofProcesses;
    cin >> numofProcesses;
    vector<Process> Processes = create_Process(numofProcesses);

    memory_placement_strategies m;
    while (true)
    {
        cout << "Original Memory Block" << endl;
        for (auto &block : Blocks)
        {
            cout << block << " ";
        }
        cout << endl;
        cout << "\nChoose allocation strategy:\n";
        cout << "1. First Fit\n";
        cout << "2. Best Fit\n";
        cout << "3. Next Fit\n";
        cout << "4. Worst Fit\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;

        vector<Process> procCopy = Processes;
        vector<int> blocksCopy = Blocks;
        cout << "Original Memory Block" << endl;
        for (auto &block : blocksCopy)
        {
            cout << block << " ";
        }
        cout << endl;
        switch (choice)
        {
        case 1:
            cout << "\nRunning First Fit...\n";
            m.firstFit(procCopy, blocksCopy);
            break;
        case 2:
            cout << "\nRunning Best Fit...\n";
            m.bestFit(procCopy, blocksCopy);
            break;
        case 3:
            cout << "\nRunning Next Fit...\n";
            m.nextFit(procCopy, blocksCopy);
            break;
        case 4:
            cout << "\nRunning Worst Fit...\n";
            m.worstFit(procCopy, blocksCopy);
            break;
        case 5:
            cout << "Exiting...\n";
            return 0;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}