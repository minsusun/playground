#include <iostream>
using namespace std;

struct Process {
    int pid;
    int arrival;
    int burst;
    Process* next;
    Process(int id, int at, int bt) : pid(id), arrival(at), burst(bt), next(nullptr) {}
};

struct Queue {
    Process* front;
    Process* rear;
    Queue() : front(nullptr), rear(nullptr) {}

    void enqueue(Process* p) {
        // TODO
    }

    Process* dequeue() {
        // TODO
        return nullptr;
    }

    bool isEmpty() {
        // TODO
        return true;
    }
};

int main() {
    int n, quantum;
    cin >> n >> quantum;

    Process** processes = new Process*[n];
    for (int i = 0; i < n; i++) {
        int pid, arrival, burst;
        cin >> pid >> arrival >> burst;
        processes[i] = new Process(pid, arrival, burst);
    }

    Queue q;

    // TODO: implement Round Robin scheduling using the queue and time quantum

    delete[] processes;
    return 0;
}
