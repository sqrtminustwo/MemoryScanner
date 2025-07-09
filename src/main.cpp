#include "ProcessScanner/ProcessScanner.h"
#include "MemoryScanner/MemoryScanner.h"
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

using namespace std;

template <class T> void searchAnyValue(const string& process_name, T value);
void searchValue(const string& process_name, int value);
void searchValue(const string& process_name, double value);
void searchValue(const string& process_name, float value);
void searchValue(const string& process_name, const char* value);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: .\\Scanner <ProcessName> <SearchValue>" << endl;
        return 1;
    }

    string processName = argv[1];
    string searchStr = argv[2];

    try {
        try {
            int val = stoi(searchStr);
            searchValue(processName, val);
        } catch (...) {
            try {
                float val = stof(searchStr);
                searchValue(processName, val);
            } catch (...) {
                try {
                    double val = stod(searchStr);
                    searchValue(processName, val);
                } catch (...) {
                    searchValue(processName, searchStr.c_str());
                }
            }
        }
    } catch (...) {
        cout << "Invalid arguments!" << endl;
        return 1;
    }

    return 0;
}

template <class T> void searchAnyValue(const string& process_name, T value) {
    ProcessScanner processScanner;
    MemoryScanner memoryScanner;
    for (const ProcessNameAndID& process : processScanner.getProcessesByName(process_name)) {
        cout << "Looking in process: " << process.name << " " << process.id << endl;
        for (BufferInfo bufferInfo: memoryScanner.getProcessMatches(process.id, value)) {
            cout << "Matching value at: " << bufferInfo.baseAddress << endl;
        }
    }
}

void searchValue(const string& process_name, int value) { searchAnyValue(process_name, value); }
void searchValue(const string& process_name, float value) { searchAnyValue(process_name, value); }
void searchValue(const string& process_name, double value) { searchAnyValue(process_name, value); }
void searchValue(const string& process_name, const char* value) { searchAnyValue(process_name, value); }