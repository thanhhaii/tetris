# Contributors

| Name | Role | Contact |
|------|------|---------|
| Lâm Nguyễn Thanh Hải | Team Leader | 25730021@ms.uit.edu.vn |
| Nguyễn Trần Minh Thuận | Team Member | 25730074@ms.uit.edu.vn |
| Lê Trọng Nghĩa | Team Member | 22730041@ms.uit.edu.vn |

# Tetris Game

A simple command-line Tetris game implementation in C++.

## Overview

This is a classic Tetris game that runs in the terminal. It features a game board where pieces fall and can be moved horizontally. The game includes score tracking and increasing difficulty levels.

## Features

- **Cross-platform support**: Runs on both Windows and Unix-like systems (Linux, macOS)
- **Console-based gameplay**: Play directly in your terminal
- **Audio support**: Background music and sound effects (via AudioManager)
- **Terminal utilities**: Enhanced console rendering with colors and cursor control
- **Score tracking**: Keep track of your score as you play
- **Progressive difficulty**: Game speed increases as you score more points

## Project Structure

```
Tetris/
├── main.cpp              # Main game logic and implementation
├── include/
│   ├── AudioManager.h    # Audio handling interface
│   └── ConsoleHelper.h   # Terminal utility functions
└── src/
    ├── AudioManager.cpp  # Audio implementation
    └── ConsoleHelper.cpp # Console utility implementation
```

## Requirements

- C++ compiler (C++11 or later)
- Standard library

## Building

Compile the project using your preferred C++ compiler:

```bash
g++ -o tetris main.cpp src/AudioManager.cpp src/ConsoleHelper.cpp -I./
```

## Running

Execute the compiled binary:

```bash
./tetris
```

## Game Controls

Use arrow keys or keyboard inputs to control the falling pieces:
- **Left Arrow**: Move piece left
- **Right Arrow**: Move piece right
- **Down Arrow** / **Space**: Rotate or speed up piece

## Technical Details

- **Board size**: 22 rows × 16 columns
- **Default game speed**: 300ms per tick
- **Cross-platform input handling**: Native input handling for Windows and Unix systems
- **Terminal enhancements**: Cursor hiding, color support, and echo control for better UX

## Notes

This is a university project for learning game development and console programming concepts.
