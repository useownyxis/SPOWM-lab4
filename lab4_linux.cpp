#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include <iomanip>
#include <thread>
#include <mutex>
#include <algorithm>

using namespace std;

int lastProcess = -1;
mutex global_mutex;
vector<thread::id> threads;

void worker(int current_id) {
    while (true) {
        global_mutex.lock(); // захватываем мьютекс
        if(threads.end() == find(threads.begin(), threads.end(), this_thread::get_id())) {
            lastProcess = -1;
           global_mutex.unlock(); // освобождаем мьютекс
           return;
        }
        if (lastProcess != current_id - 1) {
            global_mutex.unlock(); // // освобождаем мьютекс
            this_thread::sleep_for(chrono::milliseconds(1));
            continue;
        }

        if (current_id == threads.size() - 1) {
            lastProcess= -1;
        }
        else lastProcess = current_id;

        if (current_id == 0) {
            this_thread::sleep_for(chrono::milliseconds(500));
            cout << setfill('-') << setw(40) << '-' << endl;
        }

        cout << "Thread " << current_id + 1 << endl;
        if (threads.size() == 0) {
            cout << setfill('-') << setw(40) << '-' << endl;

        }
        global_mutex.unlock();
    }
}

int main() {
    cout << "+ --- Create new thread.\n- --- Delete last thread.\nq --- Delete all threads.\n";
    while(true){
        switch(cin.get()){
            case '+': {
                thread* tr = new thread(worker, threads.size());
                threads.push_back(tr->get_id());
                tr->detach();
            }
                break;
            case '-': {
                threads.pop_back();
                if(threads.size() == 0) {
                    cout << "Threads treads is clear.\n";
                }
            }
                break;
            case 'q': {
                threads.clear();
                exit(0);
            }
        }
    }
    return 0;
}


