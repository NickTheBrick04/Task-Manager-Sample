# Task Manager

A C-based CLI Task Manager that demonstrates core data structure design and pointer-level implementation. The project uses a circular linked-list queue for scheduling plus stack-backed undo/redo histories for reversible task actions.

## Overview
- **Language:** C
- **Environment:** VS Code with MSYS2
- **Core architecture:**
  - **1 circular linked-list queue** for task scheduling
  - **2 stacks** for undo/redo history management

## Main Functions
- Add, remove, and view tasks
- Undo/redo recent task-state changes
- View undo and redo stacks
- Execute custom queue and stack operations with dynamic memory allocation

## Technical Highlights
- Implemented custom **enqueue/dequeue** queue operations
- Implemented stack **push/pop** operations with **O(1)** retrieval behavior
- Applied dynamic memory allocation and pointer manipulation across queue/stack nodes

## Resume-Ready XYZ Bullets
- Increased task scheduling reliability by building a C CLI task manager around **1 circular linked-list queue**, implementing custom enqueue/dequeue operations to process task actions in deterministic order across the demo workflow.
- Improved recovery speed and usability by implementing **2 stack-backed histories (undo/redo)** with **O(1) push/pop** retrieval, enabling immediate rollback/reapply of task-state changes during command execution.
- Strengthened low-level implementation quality by developing and integrating **3 custom data-structure operations** (enqueue, dequeue, push/pop) with dynamic memory allocation and pointer manipulation, reducing edge-case failures during interactive task updates.

## Demo
The project includes pre-determined commands and outputs to demonstrate workflow behavior.

### Initial Commands
<img width="400" height="444" alt="image" src="https://github.com/user-attachments/assets/09f5da03-e015-4846-8dff-50c97f794289" />

### Output
<img width="503" height="305" alt="image" src="https://github.com/user-attachments/assets/289634ec-61c8-4e48-a266-e949852a72b8" />
