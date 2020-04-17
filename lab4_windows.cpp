#include <iostream>
#include <vector>
#include <conio.h>
#include <Windows.h>

using namespace std;

typedef struct lpParameters {
	HANDLE hMutex;
	int number;
};

DWORD WINAPI threadFunction(void* pointer) {
	lpParameters* params = (lpParameters*)pointer;
	while (true) {
		WaitForSingleObject(params->hMutex, INFINITE);
		for (int i = 0; i < params->number; i++) {
			cout << "-";
		}
		cout << "Thread " << params->number << endl;
		ReleaseMutex(params->hMutex);
		Sleep(1000);
	}
	ExitThread(0);
}


int main() {
	int threadsNumber = 0;
	vector<HANDLE> threads;
	HANDLE hMutex = CreateMutex(NULL, FALSE, NULL);
	if (hMutex == NULL) {
		cout << "Error create mutex.\n";
		exit(-1);
	}
	cout << "+ --- Create new thread." << endl;
	cout << "- --- Delete last thread." << endl;
	cout << "q --- Delete all threads." << endl << endl;
	while (true) {
		switch (_getch()) {
		case '+': {
			threadsNumber++;
			lpParameters* params = new lpParameters{ &hMutex, threadsNumber };
			HANDLE thread = CreateThread(NULL, 0, &threadFunction, params, 0, NULL);
			if (thread == NULL) {
				cout << "Error create thread.\n";
				exit(-1);
			}
			threads.push_back(thread);
		}
		break;
		case '-': {
			if (threads.size() == 0) {
				cout << "All threads are deleted.\n";
			}
			else {
				TerminateThread(threads.back(), NO_ERROR);
				threads.pop_back();
				threadsNumber--;
			}
		}
		break;
		case 'q': {
			while (threads.size() != 0) {
				TerminateThread(threads.back(), NO_ERROR);
				threads.pop_back();
			}
			CloseHandle(hMutex);
			system("pause");
			exit(0);
		}
		break;
		}
	}
	return 0;
}