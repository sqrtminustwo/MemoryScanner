#ifndef MEMORY_SCANNER
#define MEMORY_SCANNER

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <vector>
#include <iostream>
using namespace std;

struct FailedToReadBuffer: public exception {
    MEMORY_BASIC_INFORMATION baseAddress;
    FailedToReadBuffer(MEMORY_BASIC_INFORMATION baseAddress): baseAddress(baseAddress) {};
};

template <class T> struct BufferInfo {
    void* baseAddress;
    vector<T> values;
    BufferInfo(void* baseAddr) : baseAddress(baseAddr) {}

    void addValue(T value) { values.push_back(value); };
    BufferInfo<T> getMatches(T search) {
        BufferInfo matches(baseAddress);
        for (T value: values) {
            if (search == value) matches.addValue(value);
        }
        return matches;
    };
};

class MemoryScanner {
    template <class T> BufferInfo<T> getBufferInfo(HANDLE hProc, MEMORY_BASIC_INFORMATION mbi) {
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

    template <class T> vector<BufferInfo<T>> getMemInfo(DWORD pid) {
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
                memInfos.push_back(getBufferInfo<T>(hProc, mbi));
            }
            minAddress = (LPBYTE) mbi.BaseAddress + mbi.RegionSize;
        }

        return memInfos;
    }
public:
    template <class T> vector<BufferInfo<T>> getProcessMatches(DWORD pid, T value) {
        vector<BufferInfo<T>> matches;
        for (BufferInfo<T> bufferInfo : getMemInfo<T>(pid)) {
            BufferInfo<T> match = bufferInfo.getMatches(value);
            if (!match.values.empty()) matches.push_back(match);
        }
        return matches;
    }
};

#endif