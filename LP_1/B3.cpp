// Write a program to simulate memory placement strategies - bestfit,firstfit,nextfit,worstfit

#include <iostream>
#include <vector>

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
        blockNo = 0;
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

class memory_placement_strategies
{
public:

    void firstFit(vector<Process>& processes, vector<int>& blocks){
        for(auto &i: processes){
            for(int j = 0;j<blocks.size();j++){
                if(blocks[j] >= i.processSize){
                    blocks[j] -= i.processSize;
                    i.blockNo = j+1;
                    break;
                }
            }
        }
        cout<<"Process no, Process size ,Block number"<<endl;
        for(auto i: processes){
            cout<<i.processNo<<" "<<i.processSize<<" "<<i.blockNo<<endl;
        }
        cout<<"Memory Remaining"<<endl;
        for(auto i: blocks){
            cout<<i<<" ";
        }
        cout<<endl;
    }

    void bestFit(vector<Process>& processes, vector<int>& blocks){
        for(auto &i: processes){
            for(int j = 0;j<blocks.size();j++){
                if(blocks[j] >= i.processSize){
                    blocks[j] -= i.processSize;
                    i.blockNo = j+1;
                    break;
                }
            }
        }
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
    m.firstFit(Processes,Blocks);

    return 0;
}