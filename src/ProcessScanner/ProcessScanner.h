#ifndef PROCESS_SCANNER
#define PROCESS_SCANNER

#include <string>
#include <windows.h>
#include <vector>
using namespace std;

class ProcessNotFoundException: public std::exception {
    int id;
    string name;
    public:
    ProcessNotFoundException(int id, string name) {
        this->id = id;
        this->name = name;
    };
    int getId() { return id; };
    string getName() { return name; }
};

struct ProcessNameAndID {
    string name;
    int id;
    ProcessNameAndID(string name, int id) {
        this->name = name;
        this->id = id;
    };
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