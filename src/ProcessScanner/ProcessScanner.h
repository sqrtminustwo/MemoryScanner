#ifndef PROCESS_SCANNER
#define PROCESS_SCANNER

#include <string>
#include <windows.h>
#include <vector>
#include <iostream>
using namespace std;

class ProcessNotFoundException: public exception {
    int id;
    string name;
    public:
    ProcessNotFoundException(int id, string name): id(id), name(name) {};
    int getId() { return id; };
    string getName() { return name; }
};

struct ProcessNameAndID {
    DWORD id;
    string name;
    ProcessNameAndID(string name, DWORD id): name(name), id(id) {};
    void print() { cout << name << " " << id << endl; }
};

class ProcessScanner {
    void toLower(string str);
    ProcessNameAndID getProcessNameAndID(DWORD processID);
    public:
    vector<ProcessNameAndID> getAllProcesses();
    ProcessNameAndID getProcessById(int id);
    vector<ProcessNameAndID> getProcessesByName(string name);
};

#endif