// ============================================================
//  Sudoku Solver  (C++)
// ------------------------------------------------------------
//  How to type a puzzle: enter 9 rows, each row has 9 characters.
//  Use digits 1-9 for given numbers and 0 (or .) for empty cells.
//  Example row:  530070001.....spaces between characters are allowed.
// ============================================================

#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>
using namespace std;

// ---------- Constants ----------
const int SIZE = 9;       // the grid is 9 x 9
const int BOX_SIZE = 3;   // each subgrid is 3 x 3
const int EMPTY = 0;      // 0 means "empty cell"

// A sample puzzle that has a solution (0 = empty cell).
const int SAMPLE_PUZZLE[SIZE][SIZE] = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9}
};

// ============================================================
//  PART 1: Displaying the grid
// ============================================================

// Prints the grid with lines between the 3x3 subgrids.
// Empty cells are shown as a dot.
void printGrid(const int grid[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        if (row % BOX_SIZE == 0)
            cout << "+-------+-------+-------+\n";

        for (int col = 0; col < SIZE; col++) {
            if (col % BOX_SIZE == 0)
                cout << "| ";

            if (grid[row][col] == EMPTY)
                cout << ". ";
            else
                cout << grid[row][col] << " ";
        }
        cout << "|\n";
    }
    cout << "+-------+-------+-------+\n";
}

// ============================================================
//  PART 2: Sudoku rules
// ============================================================

// Returns true if 'num' can be placed at (row, col) without breaking
// the row rule, the column rule or the 3x3 subgrid rule.
bool isSafe(const int grid[SIZE][SIZE], int row, int col, int num) {
    // Rule 1 and 2: the number must not already be in this row or column.
    for (int i = 0; i < SIZE; i++) {
        if (grid[row][i] == num)
            return false;
        if (grid[i][col] == num)
            return false;
    }

    // Rule 3: the number must not already be in this 3x3 subgrid.
    int boxStartRow = row - row % BOX_SIZE;
    int boxStartCol = col - col % BOX_SIZE;
    for (int r = 0; r < BOX_SIZE; r++) {
        for (int c = 0; c < BOX_SIZE; c++) {
            if (grid[boxStartRow + r][boxStartCol + c] == num)
                return false;
        }
    }

    return true;
}

// Checks the numbers already given in the puzzle.
// Returns false (and tells where) if a given number breaks a rule.
// 'grid' is not changed when the function ends.
bool isPuzzleValid(int grid[SIZE][SIZE], int &badRow, int &badCol) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            int num = grid[row][col];
            if (num == EMPTY)
                continue;

            // Take the number out, check if it is still allowed, put it back.
            grid[row][col] = EMPTY;
            bool ok = isSafe(grid, row, col, num);
            grid[row][col] = num;

            if (!ok) {
                badRow = row;
                badCol = col;
                return false;
            }
        }
    }
    return true;
}

// ============================================================
//  PART 3: Backtracking solver
// ============================================================

// Looks for the first empty cell (left to right, top to bottom).
// Returns true and gives its position if one exists.
bool findEmptyCell(const int grid[SIZE][SIZE], int &row, int &col) {
    for (row = 0; row < SIZE; row++) {
        for (col = 0; col < SIZE; col++) {
            if (grid[row][col] == EMPTY)
                return true;
        }
    }
    return false;
}

// Solves the puzzle using recursion and backtracking.
// Returns true if a solution was found (the grid then holds the solution).
bool solveSudoku(int grid[SIZE][SIZE]) {
    int row, col;

    // Base case: no empty cell left, so the puzzle is solved.
    if (!findEmptyCell(grid, row, col))
        return true;

    // Try every number from 1 to 9 in this empty cell.
    for (int num = 1; num <= 9; num++) {
        if (isSafe(grid, row, col, num)) {
            grid[row][col] = num;          // place the number

            if (solveSudoku(grid))         // try to solve the rest (recursion)
                return true;

            grid[row][col] = EMPTY;        // it did not work: backtrack
        }
    }

    // No number fits in this cell, so the previous choices must change.
    return false;
}

// ============================================================
//  PART 4: User input
// ============================================================

// Reads one line from the user. Ends the program if the input is closed.
string readLine(const string &prompt) {
    string line;
    cout << prompt;
    if (!getline(cin, line)) {
        cout << "\nInput ended. Exiting program.\n";
        exit(1);
    }
    return line;
}

// Removes all spaces and tabs from a string.
string removeSpaces(const string &text) {
    string result = "";
    for (size_t i = 0; i < text.size(); i++) {
        if (!isspace(static_cast<unsigned char>(text[i])))
            result += text[i];
    }
    return result;
}

// Checks that a row has exactly 9 characters: digits 0-9 or a dot.
bool isValidRowText(const string &text) {
    if (text.size() != static_cast<size_t>(SIZE))
        return false;
    for (size_t i = 0; i < text.size(); i++) {
        if (!isdigit(static_cast<unsigned char>(text[i])) && text[i] != '.')
            return false;
    }
    return true;
}

// Asks the user for the 9 rows of the puzzle and stores them in the grid.
void readPuzzle(int grid[SIZE][SIZE]) {
    cout << "\nEnter the puzzle row by row (9 characters per row).\n";
    cout << "Use digits 1-9 for given numbers and 0 or . for empty cells.\n";
    cout << "Example: 530070000\n\n";

    for (int row = 0; row < SIZE; row++) {
        while (true) {
            string text = removeSpaces(readLine("Row " + string(1, static_cast<char>('1' + row)) + ": "));

            if (!isValidRowText(text)) {
                cout << "  Invalid row. Please type exactly 9 characters (digits 0-9 or .).\n";
                continue;
            }

            for (int col = 0; col < SIZE; col++) {
                if (text[col] == '.')
                    grid[row][col] = EMPTY;
                else
                    grid[row][col] = text[col] - '0';
            }
            break;
        }
    }
}

// Copies the built-in sample puzzle into the grid.
void loadSamplePuzzle(int grid[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++)
        for (int col = 0; col < SIZE; col++)
            grid[row][col] = SAMPLE_PUZZLE[row][col];
}

// ============================================================
//  PART 5: Putting it all together
// ============================================================

// Shows the puzzle, checks it, solves it and shows the result.
void solveAndShow(int grid[SIZE][SIZE]) {
    cout << "\nYour puzzle:\n";
    printGrid(grid);

    int badRow = 0, badCol = 0;
    if (!isPuzzleValid(grid, badRow, badCol)) {
        cout << "\nThis puzzle is not valid: the number " << grid[badRow][badCol]
             << " at row " << (badRow + 1) << ", column " << (badCol + 1)
             << " breaks the Sudoku rules.\n";
        return;
    }

    cout << "\nSolving...\n";
    if (solveSudoku(grid)) {
        cout << "\nSolution found:\n";
        printGrid(grid);
    } else {
        cout << "\nNo solution exists for this puzzle.\n";
    }
}

int main() {
    cout << "=========================================\n";
    cout << "             SUDOKU SOLVER               \n";
    cout << "=========================================\n";

    while (true) {
        cout << "\nMAIN MENU\n";
        cout << "  1. Enter my own puzzle\n";
        cout << "  2. Solve the sample puzzle\n";
        cout << "  3. Exit\n";

        string choice = removeSpaces(readLine("Enter your choice (1-3): "));

        int grid[SIZE][SIZE];

        if (choice == "1") {
            readPuzzle(grid);
            solveAndShow(grid);
        } else if (choice == "2") {
            loadSamplePuzzle(grid);
            solveAndShow(grid);
        } else if (choice == "3") {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice. Please enter 1, 2 or 3.\n";
        }
    }

    return 0;
}