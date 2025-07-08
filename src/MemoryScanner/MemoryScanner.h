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
    private:
    template <class T> BufferInfo<T> getBufferInfo(HANDLE hProc, MEMORY_BASIC_INFORMATION mbi);
    template <class T> vector<BufferInfo<T>> getMemInfo(DWORD pid);
    public:
    template <class T> vector<BufferInfo<T>> getProcessMatches(DWORD pid, T search);
};

#endif