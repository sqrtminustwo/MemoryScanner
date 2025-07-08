#include "ProcessScanner.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <vector>
#include <algorithm>
#include <cctype>
#include <bits/stdc++.h>

using namespace std;

// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1
ProcessNameAndID ProcessScanner::getProcessNameAndID(DWORD processID) {
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    HANDLE hProcess = OpenProcess(
        PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, 
        processID
    );

    if (NULL != hProcess) {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
            GetModuleBaseName(
                hProcess, 
                hMod, 
                szProcessName, 
                sizeof(szProcessName)/sizeof(TCHAR)
            );
        }
    }

    CloseHandle(hProcess);
    return ProcessNameAndID(string(szProcessName), processID);

}

vector<ProcessNameAndID> ProcessScanner::getAllProcesses() {
    // Get the list of process identifiers.
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;
    vector<ProcessNameAndID> processes;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        return processes;
    }


    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded/sizeof(DWORD);

    for (i = 0; i < cProcesses; i++ ) {
        if(aProcesses[i] != 0) {
           processes.push_back(getProcessNameAndID(aProcesses[i]));
        }
    }

    return processes;
}

ProcessNameAndID ProcessScanner::getProcessById(int id) {
    for (ProcessNameAndID process: getAllProcesses()) {
        if (process.id == id) return process;
    }
    throw ProcessNotFoundException(id, "");
}

void ProcessScanner::toLower(string str) {
    transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::tolower(c); });
}

vector<ProcessNameAndID> ProcessScanner::getProcessesByName(string search) {
    toLower(search);

    vector<ProcessNameAndID> processes;
    for (ProcessNameAndID process: getAllProcesses()) {
        string name = process.name;
        toLower(name);
        if (name.find(search) != string::npos) processes.push_back(process);
    }

    return processes;
}