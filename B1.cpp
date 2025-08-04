#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

enum EVStatus
{
    WAITING,
    CHARGING,
    DONE
};

class Semaphore
{
private:
    int permits;
    mutex mtx;
    condition_variable cv;

public:
    Semaphore(int p) : permits(p) {}

    void acquire()
    {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&]()
                { return permits > 0; });
        permits--;
    }

    void release()
    {
        unique_lock<mutex> lock(mtx);
        permits++;
        cv.notify_one();
    }
};

class EVUsingMutex
{
private:
    static mutex chargePort;
    EVStatus status;
    string name;
    thread t;

    void charge()
    {
        status = CHARGING;
        this_thread::sleep_for(chrono::seconds(3)); 
        status = DONE;
    }

public:
    EVUsingMutex(string n) : name(n), status(WAITING) {}

    void start()
    {
        t = thread([this]()
                   {
            status = WAITING;
            lock_guard<mutex> lock(chargePort);
            charge(); });
    }

    void join()
    {
        if (t.joinable())
            t.join();
    }

    EVStatus getStatus() { return status; }
    string getName() { return name; }
};
mutex EVUsingMutex::chargePort;

class EVUsingSema
{
private:
    static Semaphore chargePorts;
    EVStatus status;
    string name;
    thread t;

    void charge()
    {
        status = CHARGING;
        this_thread::sleep_for(chrono::seconds(3)); // simulate charging
        status = DONE;
    }

public:
    EVUsingSema(string n) : name(n), status(WAITING) {}

    void start()
    {
        t = thread([this]()
                   {
            status = WAITING;
            chargePorts.acquire();
            charge();
            chargePorts.release(); });
    }

    void join()
    {
        if (t.joinable())
            t.join();
    }

    EVStatus getStatus() { return status; }
    string getName() { return name; }
};
Semaphore EVUsingSema::chargePorts(3);

void monitorMutexStatus(vector<EVUsingMutex> &evs)
{
    bool allDone = false;
    while (!allDone)
    {
        allDone = true;
        cout << "\nEV Charging Status (Mutex - 1 port):\n";
        for (auto &ev : evs)
        {
            auto s = ev.getStatus();
            cout << ev.getName() << ": "
                 << (s == WAITING ? "WAITING" : (s == CHARGING ? "CHARGING" : "DONE")) << "\n";
            if (s != DONE)
                allDone = false;
        }
        cout << "-------------------------\n";
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void monitorSemaStatus(vector<EVUsingSema> &evs)
{
    bool allDone = false;
    while (!allDone)
    {
        allDone = true;
        cout << "\nEV Charging Status (Semaphore - 3 ports):\n";
        for (auto &ev : evs)
        {
            auto s = ev.getStatus();
            cout << ev.getName() << ": "
                 << (s == WAITING ? "WAITING" : (s == CHARGING ? "CHARGING" : "DONE")) << "\n";
            if (s != DONE)
                allDone = false;
        }
        cout << "-------------------------\n";
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void MutexChargingDemo(int num)
{
    vector<EVUsingMutex> evs;
    for (int i = 1; i <= num; i++)
        evs.emplace_back("EV-" + to_string(i));

    for (auto &ev : evs)
        ev.start();

    monitorMutexStatus(evs);

    for (auto &ev : evs)
        ev.join();
}

void SemaphoreChargingDemo(int num)
{
    vector<EVUsingSema> evs;
    for (int i = 1; i <= num; i++)
        evs.emplace_back("EV-" + to_string(i));

    for (auto &ev : evs)
        ev.start();

    monitorSemaStatus(evs);

    for (auto &ev : evs)
        ev.join();
}

// ----------- Main Function -----------
int main()
{
    int num;
    cout << "Enter number of EVs: ";
    cin >> num;

    while (true)
    {
        cout << "\n---- Menu ----\n";
        cout << "1. Charge using Mutex (Only 1 port)\n";
        cout << "2. Charge using Semaphore (3 ports)\n";
        cout << "3. Exit\n";
        cout << "--------------\n";
        int choice;
        cin >> choice;

        if (choice == 1)
        {
            MutexChargingDemo(num);
        }
        else if (choice == 2)
        {
            SemaphoreChargingDemo(num);
        }
        else if (choice == 3)
        {
            break;
        }
        else
        {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}