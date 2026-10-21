# 💣 Minesweeper in C

Classic Minesweeper for the terminal, written in plain C.

## Features
- Four difficulty levels: 9×9 / 10 mines, 16×16 / 40 mines, 16×30 / 99 mines, or your own map size and mine count
- The first move is always safe; empty areas are revealed with recursive flood fill
- Flags (`f row col`) and digging (`r row col`), coloured board via ANSI escape codes
- Score = revealed cells × level (own map is not multiplied), top-5 leaderboard in `Score.txt`, full history in `Score2.txt`
- Test mode that replays a game from a file
- Dynamic 2D boards (`int**` / `char**`) with manual memory management

## Project structure
| File | Purpose |
|---|---|
| `Projektsaper/proj.c` | Game loop, menu, input validation, leaderboard, test mode |
| `Projektsaper/sotw.c`, `sotw.h` | Game engine: mine spawning, board rendering, flood-fill reveal, win check |
| `Projektsaper/Makefile` | Build with gcc |
| `Projektsaper/tek.txt` | Example input for test mode |

## Build & run (Linux / WSL / macOS)
```sh
cd Projektsaper
make
./project_output
```

## Test mode
```sh
./project_output -a tek.txt
```
The file contains the map size, the map (`-1` = mine, `0` = empty) and a list of moves `row column`.
The program prints `<correct moves> <revealed cells> <1 if no mine was hit, else 0>`.

Made as a semester project for the *Programming Languages and Methods* course (Warsaw University of Technology).
