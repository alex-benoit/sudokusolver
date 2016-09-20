## Sudoku Solver (C++)

Graphical Sudoku Solver in C++ and GTK. The solver uses a combination of a smart home-made algorithm and a bruteforce algorithm to solve Sudokus in the best possible time. See attached PDF for code explanation.

## Functionalities

 - Solve a Sudoku
 - Show the results in a GUI
 - Read a Sudoku from a text file and solve it
 - Save a solved Sudoku to a text file

## Usage

To compile (with g++ compiler):

```
g++ main.cpp -o sudokusolver `pkg-config --cflags --libs gtkmm-3.0`
```

Make sure you have 'gtk' and 'gtkmm' libs installed to allow compilation. To install on Ubuntu:

```
sudo apt-get install libgtk-3-dev
sudo apt-get install libgtkmm-3.0-dev
```

## Known bugs

 - Error message when Sudoku is invalid does not appear anymore (maybe due to GTK2.x -> GTK3.0 change -- line 427 uncommented raises an error at compilation)
 - The GUI window is way to large when using GTK3.0 as the command below does not take effect when compiled with GTK3.0 and the number grids take their default size.

```
104.    gtk_widget_set_size_request(grid_widg[i][j], 40, 40);
```
