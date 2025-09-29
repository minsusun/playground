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
        if (isEmpty()) {
            front = p;
            rear = p;
        }
        else {
            rear -> next = p;
            rear = p;
        }
    }

    Process* dequeue() {
        // TODO
        if (!isEmpty()) {
            Process* result = front;
            if (front == rear) {
                front = nullptr;
                rear = nullptr;
            }
            else {
                front = front -> next;
            }
            return result;
        }
        return nullptr;
    }

    bool isEmpty() {
        // TODO
        return front == nullptr && rear == nullptr;
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
    int time = 0;
    for (int i = 0; i < n; i++) {
        q.enqueue(processes[i]);
    }
    while (!q.isEmpty()) {
        Process* target = q.dequeue();
        cout << time << " " << target -> pid << endl;
        if (target -> burst <= quantum) {
            time += target -> burst;
        }
        else {
            time += quantum;
            target -> burst -= quantum;
            q.enqueue(target);
        }
    }

    delete[] processes;
    return 0;
}
