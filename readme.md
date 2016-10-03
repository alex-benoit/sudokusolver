## Sudoku Solver (C++)

Graphical Sudoku Solver in C++ and GTK. The solver uses a combination of a smart home-made algorithm and a bruteforce algorithm to solve Sudokus in the best possible time. See attached PDF for code explanation.

## Functionalities

 - Solve a Sudoku
 - Show the results in a GUI
 - Read a Sudoku from a text file and solve it
 - Save a solved Sudoku to a text file

## Usage

Make sure you have 'gtk' and 'gtkmm' libs installed to allow compilation. To install on Ubuntu:

```
sudo apt-get install libgtk2.0-dev
sudo apt-get install libgtkmm-2.4-dev
```

To compile (with g++ compiler):

```
g++ main.cpp -o SudokuSolver `pkg-config --cflags --libs gtkmm-2.4`
```

To run from terminal:

```
./SudokuSolver
```

![alt tag](http://i.imgur.com/iNz6urv.png)
![alt tag](http://i.imgur.com/lyGTPXe.png)
