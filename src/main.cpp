#include "ProcessScanner/ProcessScanner.h"
#include "MemoryScanner/MemoryScanner.h"
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

using namespace std;

int main() {
    
    ProcessScanner ps;
    for (ProcessNameAndID process: ps.getAllProcesses()) {
        process.print();
    }

    MemoryScanner ms;
    for (BufferInfo<int> bufferInfo: ms.getProcessMatches<int>(ps.getProcessById(21072).id, 536)) {
        cout << "Matching vlue in: " << bufferInfo.baseAddress << endl;
    }

    return 0;
}