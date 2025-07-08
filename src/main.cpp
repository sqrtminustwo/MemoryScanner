#include "ProcessScanner/ProcessScanner.h"
#include "MemoryScanner/MemoryScanner.h"
#include <vector>
#include <iostream>
using namespace std;

int main() {
    ProcessScanner processScanner;
    for (ProcessNameAndID process: processScanner.getAllProcesses()) {
        cout << process.name << " " << process.id << endl;
    }

    cout << endl;

    MemoryScanner memoryScanner;
    for (ProcessNameAndID process: processScanner.getProcessesByName("steam")) {
        cout << "Looking in process: " << process.name << " " << process.id << endl;
        for (BufferInfo bufferInfo: memoryScanner.getProcessMatches(process.id, 23)) {
            cout << "Matching vlaue in: " << bufferInfo.baseAddress << endl;
        }
    }
}