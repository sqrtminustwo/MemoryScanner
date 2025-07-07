#include "ProcessScanner/ProcessScanner.h"
#include <vector>
#include <iostream>
using namespace std;

int main() {
    ProcessScanner scanner;
    for (ProcessNameAndID process: scanner.getAllProcesses()) {
        cout << process.name << " " << process.id << endl;
    }

    cout << endl;

    for (ProcessNameAndID process: scanner.getProcessesByName("firefox")) {
        cout << process.name << " " << process.id << endl;
    }

    cout << endl;

    try {
        ProcessNameAndID process = scanner.getProcessById(20208);
        cout << process.name << " " << process.id << endl;
    } catch (ProcessNotFoundException ex) {
        cout << "Couldnt find: " << ex.getId();
    }
}