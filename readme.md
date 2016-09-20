## Sudoku Solver (C++)

Algorithm that solves a Sudoku using first a 'smart' method and then a bruteforce method if the first one is not enough.

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

 - Error message when Sudoku is invalid does not appear anymore (maybe due to GTK2.x -> GTK3.0 change)
 - The GUI window is way to large when using GTK3.0 as the command below does not take effect when compiled with GTK3.0 and the number grids take their default size.

'''
104. gtk_widget_set_size_request(grid_widg[i][j], 40, 40);

'''
