# Design Logic

## Core Mental Model
Each habit task is a dynamic entity with an id, title, and completion history.

## Heap Allocation Reasoning
Task count is unknown at compile time. Using `std::unique_ptr<HabitTask>` allocates tasks on heap with automatic cleanup.

## Streak Calculation
Dates are converted to day numbers and sorted. Consecutive day differences of 1 build the streak. This is simple and deterministic.

## Validation Strategy
Date format and task id inputs are validated before mutation. This keeps data clean and avoids exception crashes during normal interaction.

## Complexity
- Add task: O(1)
- Find/delete task: O(n)
- Streak calc per task listing: O(k log k), where k is completion count for that task.
