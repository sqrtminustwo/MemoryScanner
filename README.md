**MemoryScanner** is a C++ project for analyzing the memory of running Windows applications. It includes two main components:

- `ProcessScanner`: Identifies running processes by name.
- `MemoryScanner`: Searches memory regions within a process for specific values.

This project serves as a learning exercise in C++ systems programming and the use of the Windows API.

---

## Usage

```bash
.\Scanner <ProcessName> <SearchValue>
```

- `<ProcessName>`: The name of the process to search (e.g., `steam`).
- `<SearchValue>`: The value to locate in memory. Supported types include `int`, `float`, `double`, and `string`.

---

## Example

```bash
.\Scanner steam 529.9
```

Example output:

```
Looking in process: steam.exe 8832
Matching value at: 0x3120000
...
Matching value at: 0xa990000
Looking in process: steamwebhelper.exe 16888
Matching value at: 0x180e1600000
...
Matching value at: 0x4c9c01250000
Looking in process: steamwebhelper.exe 7772
Looking in process: steamwebhelper.exe 14780
Matching value at: 0xe9ec3aa000
...
Matching value at: 0x6cb0018b4000
Looking in process: steamwebhelper.exe 12900
Matching value at: 0x192800188000
Looking in process: steamwebhelper.exe 5936
Looking in process: steamwebhelper.exe 15628
Matching value at: 0x1517c1e0000
...
Matching value at: 0x7170192c0000
Looking in process: steamwebhelper.exe 17104
Matching value at: 0x63c500181000
...
Matching value at: 0x6f8400b70000
```
---
