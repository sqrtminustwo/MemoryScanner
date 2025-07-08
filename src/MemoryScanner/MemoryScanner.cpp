#include "MemoryScanner.h"
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <vector>
#include <iostream>

template <class T> BufferInfo<T> MemoryScanner::getBufferInfo(HANDLE hProc, MEMORY_BASIC_INFORMATION mbi) {
    SIZE_T bytesRead;
    LPVOID buffer = malloc(mbi.RegionSize);
    if (buffer == NULL) throw FailedToReadBuffer(mbi);
    
    BufferInfo<T> bufferInfo(mbi.BaseAddress);

    if (ReadProcessMemory(hProc, mbi.BaseAddress, buffer, mbi.RegionSize, &bytesRead)) {
        unsigned char* bytes = (unsigned char*) buffer;

        for (SIZE_T i = 0; i < bytesRead - sizeof(T); i++) {
            T value;
            memcpy(&value, bytes + i, sizeof(T));
            bufferInfo.addValue(value);
        }

    } else throw FailedToReadBuffer(mbi);

    free(buffer);

    return bufferInfo;
}
template BufferInfo<int> MemoryScanner::getBufferInfo<int>(HANDLE hProc, MEMORY_BASIC_INFORMATION mbi);
template BufferInfo<float> MemoryScanner::getBufferInfo<float>(HANDLE hProc, MEMORY_BASIC_INFORMATION mbi);
template BufferInfo<double> MemoryScanner::getBufferInfo<double>(HANDLE hProc, MEMORY_BASIC_INFORMATION mbi);
template BufferInfo<string> MemoryScanner::getBufferInfo<string>(HANDLE hProc, MEMORY_BASIC_INFORMATION mbi);

template <class T> vector<BufferInfo<T>> MemoryScanner::getMemInfo(DWORD pid) {
    HANDLE hProc;
    SYSTEM_INFO si;
    MEMORY_BASIC_INFORMATION mbi;
    LPVOID minAddress, maxAddress;
    vector<BufferInfo<T>> memInfos;

    GetSystemInfo(&si);
    minAddress = si.lpMinimumApplicationAddress;
    maxAddress = si.lpMaximumApplicationAddress;

    hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (!hProc) {
        printf("[-] OpenProcess() failed.\n");
        return memInfos;
    }

    while (minAddress < maxAddress) {
        if(!VirtualQueryEx(hProc, minAddress, &mbi, sizeof(mbi))) printf("[-] VirtualQueryEx() failed. %d\n", GetLastError());
        if (mbi.State == MEM_COMMIT && mbi.Protect == PAGE_READWRITE && mbi.Type == MEM_PRIVATE) {
            memInfos.push_back(this->getBufferInfo<T>(hProc, mbi));
        }
        minAddress = (LPBYTE) mbi.BaseAddress + mbi.RegionSize;
    }

    return memInfos;
}

template <class T> vector<BufferInfo<T>> MemoryScanner::getProcessMatches(DWORD pid, T value) {
    vector<BufferInfo<T>> matches;
    for (BufferInfo<T> bufferInfo : this->getMemInfo<T>(pid)) {
        BufferInfo<T> match = bufferInfo.getMatches(value);
        if (!match.values.empty()) matches.push_back(match);
    }
    return matches;
}

template std::vector<BufferInfo<int>> MemoryScanner::getProcessMatches<int>(unsigned long, int);
template std::vector<BufferInfo<float>> MemoryScanner::getProcessMatches<float>(unsigned long, float);
template std::vector<BufferInfo<double>> MemoryScanner::getProcessMatches<double>(unsigned long, double);
template std::vector<BufferInfo<string>> MemoryScanner::getProcessMatches<string>(unsigned long, string);