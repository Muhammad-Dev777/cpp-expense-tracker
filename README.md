# C++ Expense Tracker

A console expense tracker built as my first C++ project while studying Object-Oriented Programming and Data Structures. I developed it incrementally with AI guidance and practised making changes myself, including displaying the expense count and filtering records.

## Features

- Add expenses with an ID, title, category, and amount in PKR.
- View all expenses and their count.
- Calculate the overall total or a total for a selected category.
- Edit or delete an expense by ID, with confirmation.
- Save records to `expenses.txt` and reload them on startup.
- Validate numeric ranges and reject `|` in titles and categories.

## Requirements

A C++17 compiler, such as GCC/MinGW-w64. VS Code is optional; its C++ extension alone does not provide a compiler.

## Build and run

Open a terminal in the folder containing `main.cpp`.

Windows PowerShell:

```powershell
g++ -std=c++17 -Wall -Wextra main.cpp -o expense-tracker.exe
.\expense-tracker.exe
```

Linux or macOS with a suitable compiler:

```sh
g++ -std=c++17 -Wall -Wextra main.cpp -o expense-tracker
./expense-tracker
```

Stop the running program using menu option `0` before rebuilding.

## Menu

```text
1. Add expense
2. View expenses
3. Show total
4. Show total by category
5. Delete expense
6. Edit expense
0. Exit
```

## Data storage

Records are stored in the current working directory as `expenses.txt`, with one record per line:

```text
1|Notebook|Education|150
```

The fields are ID, title, category, and amount. IDs stay unchanged when another record is deleted, so gaps are expected. On restart, the next ID is calculated from the highest loaded ID; it is not a permanent history of deleted IDs.

The program starts empty if no data file exists. For a demo, copy the fictional `sample_expenses.txt` to `expenses.txt` before starting. Do this only when you do not already have personal records in that folder.

Personal `expenses.txt` files and compiled `.exe` files are excluded through `.gitignore`. The machine-specific VS Code configuration is also excluded.

## What I practised

Classes and objects, vectors, references, loops, switch statements, functions, iterators, input validation, and text file handling.

## Current limitations

- Amounts use whole rupees; decimal amounts are not supported.
- Category matching is case-sensitive.
- Numeric input accepts a valid integer prefix, so `250abc` is interpreted as `250`.
- End-of-input is not handled gracefully by the input helper.
- Invalid saved records are skipped rather than reported.
- Saving rewrites the data file directly; there is no automatic backup or atomic replacement.
- Dates, budgets, and monthly reports are not included in this version.

## Validation

Compiled using GCC with C++17, `-Wall`, and `-Wextra`. Checked adding, viewing, totals, category totals, editing/deleting with cancellation, nonexistent IDs, numeric range checks, and persistence across restarts using separate test data.
