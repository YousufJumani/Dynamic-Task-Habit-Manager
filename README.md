# Dynamic Task/Habit Manager

## Problem It Solves
Tracks habits and completion streaks with a dynamic number of user-defined tasks.

## Skills Covered
- Heap allocation with `std::make_unique` for each task
- Dynamic growth of tasks with vector of pointers
- Modular helper functions for validation and streak calculation
- CRUD-style terminal interaction

## Build and Run
```powershell
g++ -std=c++17 -O2 .\src\main.cpp -o habits.exe
.\habits.exe
```

## Why This Design
- Tasks are heap allocated because count is unknown at compile time.
- `calculateStreak` is isolated so logic can be tested independently.
- Date conversion to day numbers makes streak logic deterministic.

## Extension Ideas
- Save tasks to JSON.
- Add recurring habits and reminder times.
- Add weekly progress charts.
