# FileWatch

A simple Linux command-line tool written in C that monitors directories for file changes using the `inotify` API. This project is built for learning purposes, focusing on systems programming concepts such as file descriptors, system calls, signals, and event-driven I/O.

---

## 🚀 Goals

The goal of this project is to:

- Learn **low-level Linux system programming** using C.
- Understand **event notification mechanisms** with `inotify`.
- Build a **modular CLI tool** using `CMake`.
- Explore **signal handling**, **I/O multiplexing**, and **recursive directory traversal**.

---

## 🧱 Project Structure

```
filewatch/
├── CMakeLists.txt       # CMake build configuration
├── include/             # Header files
│   └── filewatch.h
├── src/                 # C source files
│   ├── main.c
│   └── filewatch.c
├── build/               # CMake build directory (ignored by git)
└── .gitignore
```

---

## ⚙️ Setup Instructions

### Prerequisites

- Linux system (with `inotify` support)
- GCC or Clang compiler
- CMake 3.10 or higher

### Build

```bash
mkdir -p build
cd build
cmake ..
make
```

### Run

```bash
./bin/filewatch [OPTIONS] <directory>
```

---

## 🧩 Command-Line Options

| Option | Description |
|---------|-------------|
| `-r`, `--recursive` | Watch directories recursively |
| `-v`, `--verbose` | Show detailed event information |
| `-h`, `--help` | Display usage information |

### Examples

```bash
# Watch a directory
./bin/filewatch /tmp/test

# Recursive and verbose mode
./bin/filewatch -rv /tmp/test

# Show help
./bin/filewatch -h
```

---

## 🧠 Features Implemented

### ✅ Basic File Watching
- Uses the Linux `inotify` API to detect file and directory changes.
- Handles events such as **create**, **modify**, and **delete**.

### ✅ Recursive Directory Monitoring
- When `-r` is used, all subdirectories are added to the watch list.
- Automatically adds new subdirectories as they are created.

### ✅ Signal Handling
- Graceful shutdown on `Ctrl+C` (SIGINT).
- Cleans up open file descriptors before exiting.

---

## 📜 License

[MIT](./LICENSE) © 2025 Bruno Maio
