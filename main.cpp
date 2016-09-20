
/*************************************************************************/
///     SUDOKU SOLVER                                                   ///
///     PROGRAM CREATED BY ALEXANDRE BENOIT                             ///
///     1st YEAR COMPUTER LABS PROJECT                                  ///
///     DATE OF SUBMISSION: 29/03/2013                                  ///
/*************************************************************************/

// indluding the libraries
#include <gtk/gtk.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <time.h>

/// READ ME

/// This program can input a sudoku from a file, or directly using a Graphical User Interface.
/// Using the Menu and Buttons, you can choose to Reset the grid, Input from a file, Save the solution to a .txt file, Quit the solver or Solve the Sudoku
/// It uses a first function solve to fill in the easy gaps and can even solve the easy Sudokus - it is very quick
/// The second function solve_2 is more general and a bit slower (the program is still very very fast) and tries in all the possibilities within the rules to not waste time
/// The longest processing time of the general solve function (not the GUI enabling functions) was 0,188s for an 'evil' Sudoku (I didnt try all of them though!)

/// This program uses the GTK+ library and gtkmm (integration with C++) to create the GUI (Graphical User Interface)
/// This source code cannot be compiled without having downloaded the GTK+ library and configured if with gtkmm !!!


using namespace std;

// Functions that serve for the 1st Part of the solve function
bool verif_1(int i, int j, int k);
void solve();

// Functions that serve for the 2nd Part of the solve function
bool verif_2(int i, int j);
void solve_2(int row, int column);

// Static variable that serve thoughout the whole program
static int grid_array[9][9];
static GtkWidget* grid_widg[9][9], *window;

// GUI Enabling Functions
void initialise_GUI(int*, char***);
static void menu_event(GtkWidget*, gpointer);

// GUI Functions that display errors in windows
void open_error();



int main(int argc, char* argv[])   /// This is the main function, it first initializes the board and then call the GUI enablinf function
{
        for(int i = 0; i<9; i++)
                for(int j = 0; j<9; j++)
                        grid_array[i][j] = 0;   // initialise the grid but assigning 0 to all the values
        initialise_GUI(&argc, &argv);   // initialise the GUI that will then call the solve function when demanded
        return 0;
}

static void get_numbers(GtkWidget* widget,gpointer data)   /// This function collect all the numbers from the grid (that are inputed by the user for example) and puts them into a 2D array grid_array[i][j] and then solves the Sudoku
{
        for(int i = 0; i<9; i++)
        {
                for(int j = 0; j<9; j++)
                {
                        grid_array[i][j] = atoi(gtk_entry_get_text(GTK_ENTRY(grid_widg[i][j])));   // atoi functions converts the char in the GUI grid to an integer to be able to be put in the 2D array
                }
        }
        solve();
}

void initialise_GUI(int* argc, char*** argv)   /// This function initializes the Graphics and assigns Functions to the different Widgets (by clicking the solve button, the solve function is activated for example)
{
    const char* file[] = {"Reset Grid", "Open File", "Save Grid"};   // creating char variables for the Options Menu
    gtk_init(argc, argv);   // STARTS THE GTK MAIN FUNCTION
    GtkWidget *horizontal_box, *vertical_box, *separator, *solve_button, *exit_button, *options_menu, *menu,  *menu_bar;
    vertical_box = gtk_vbox_new(0, 0);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);   // creates a TOP LEVEL Window (it comes in front of otehr windows)
    menu = gtk_menu_new();
    menu_bar = gtk_menu_bar_new();
    options_menu = gtk_menu_item_new_with_label("Options");   // creates a menu widget called Options

    for(int i = 0; i<3; i++)   // gives the names and  actions to the different labels on the Options Menu (their actions are further defined in menu_event function)
    {
        GtkWidget* menu_item = gtk_menu_item_new_with_label(file[i]);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);
        g_signal_connect(menu_item, "activate", G_CALLBACK(menu_event), window);
    }

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(options_menu), menu);   // creates sub-menu for the Options Tab
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), options_menu);
    menu = gtk_menu_new();
    gtk_box_pack_start(GTK_BOX(vertical_box), menu_bar, 0, 0, 0);

    for(int i = 0; i<9; i++)   // these next two 'for loops' creat the cells for the Graphical Sudoku using the grid_widg[i][j]
    {
        horizontal_box=gtk_hbox_new(0, 0);
        for(int j = 0; j<9; j++)
        {
            grid_widg[i][j] = gtk_entry_new();
            gtk_entry_set_max_length(GTK_ENTRY(grid_widg[i][j]), 1);   // sets the maximum lenghts of the cell's inputs
            gtk_widget_set_size_request(grid_widg[i][j], 40, 40);   // sets the size of the cells
            gtk_box_pack_start(GTK_BOX(horizontal_box), grid_widg[i][j], 0, 0, 0);   // creates the cells
            if((j+1)%3 == 0)
            {
                separator = gtk_vseparator_new();   // inserts a seperator between vertical columns 3 and 4 & 6 and 7
                gtk_box_pack_start(GTK_BOX(horizontal_box), separator, 0, 0, 0);
            }
        }
            gtk_box_pack_start(GTK_BOX(vertical_box), horizontal_box, 0, 0, 0);
            if((i+1)%3 == 0)
            {
                separator = gtk_hseparator_new();   // inserts a seperator between hozitontal rows 3 and 4 & 6 and 7
                gtk_box_pack_start(GTK_BOX(vertical_box), separator, 0, 0, 0);
            }
    }
    solve_button = gtk_button_new_with_label("Solve the Sudoku!");   // creates the appropriate label for the Solve Button
    g_signal_connect(solve_button, "clicked", G_CALLBACK(get_numbers), NULL);   // calls the function get_numbers when Solve Button clicked (this function collects the numbers and solves the Sudoku as well)
    gtk_box_pack_start(GTK_BOX(vertical_box), solve_button, 0, 0, 0);   // creates the Solve Button
    gtk_widget_set_size_request(solve_button, 50, 50);   // makes the Solve Button larger than the Exit Button
    exit_button = gtk_button_new_with_label("Exit (Ctrl + C in Terminal to Abort)");   // creates the appropriate label for the Exit Button
    g_signal_connect(exit_button, "clicked", G_CALLBACK(gtk_main_quit), NULL);   // calls the function gtk_main_quit (that terminates the program) when Exit Button clicked
    gtk_box_pack_start(GTK_BOX(vertical_box), exit_button, 0, 0, 0);   // creates the Exit Button
    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);   //if the red cross if pressed, the program terminates when we close the window
    gtk_container_add(GTK_CONTAINER(window), vertical_box);
    gtk_widget_show_all(window);   // displays all the widgets that have been created
    gtk_main();   // ENDS THE GTK MAIN FUNCTION
}

static void menu_event(GtkWidget* menu_item,gpointer data)   /// This function defines the diferent actions that have the different actions in the Drop-Down Menu
{
        if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(menu_item)), "Reset Grid") == 0)   // the use of the function strcmp is the equivalent of saying "if this was clicked..."
        {
                for(int i = 0; i<9; i++)   // in this case we want to reset the Grid
                        for(int j = 0; j<9; j++)
                        {
                                grid_array[i][j] = 0;   // we set all the 2D array data to 0
                                gtk_entry_set_text(GTK_ENTRY(grid_widg[i][j]), "0");   // display it on the GUI Grid
                        }
        }

        else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(menu_item)), "Open File") == 0)   // if we clicked "Oen File"
        {
                GtkWidget *dialog;   // this dialog widget will open a window to choose a .txt sudoku file (any file, with any name)
                dialog = gtk_file_chooser_dialog_new("Choose the sudoku file", GTK_WINDOW(data), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,NULL);
                gtk_widget_show_all(dialog);   // shows the dialog window
                int resp = gtk_dialog_run(GTK_DIALOG(dialog));
                if(resp == GTK_RESPONSE_OK)   // if we choose a file...
                {
                        ifstream infile(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));   //... input the file into the 2D grid_array[i][j]
                        for(int i = 0; i<9; i++)
                        {
                                for(int j = 0; j<9; j++)
                                {
                                        infile >> grid_array[i][j];
                                        char charact[2];   // creates a char array
                                        sprintf(charact, "%d", grid_array[i][j]);   // input it into a char called charact
                                        gtk_entry_set_text(GTK_ENTRY(grid_widg[i][j]), charact);   // displays it into the GUI Grid
                                }
                        }
                        infile.close();   // closes the input file tp prevent memory leaks
                }
                gtk_widget_destroy(dialog);   // destoys the memory of the dialog window (we don't need it anymore)
        }

        else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(menu_item)),"Save Grid")==0) // if we click "Save Grid"
        {
        ofstream outfile("sudoku_completed.txt");   // creats an output file
        for (int i = 0; i < 9; i++)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            char charact[2];   // creates a char array
                            sprintf(charact, "%d", grid_array[i][j]);   // input it into a char called charact
                            gtk_entry_set_text(GTK_ENTRY(grid_widg[i][j]), charact);   // displays it into the GUI Grid
                        }
                    }
        int i = 0;
        while (i<9)
            {
                for (int j = 0; j < 9; j++)
                    outfile << grid_array[i][j] << " ";   // displays the 2D array contents with the appropriate new lines and spaces
                    outfile << endl;
                i++;
            }
        outfile.close();   // closes the output file
        cout << "The current Grid has been saved under the name 'sudoku_completed.txt' in the program directory" << endl << endl;
        }
}

void solve()   // This is the general solve function that incorporates the first part of the solving program
{
    clock_t t;
    t = clock();   // initialise the clock timer

    int number_gaps = 0, temp;
    bool clean_loop = 1;

    for(int i = 0; i<9; i++)   // these two 'for loops' count the number of gaps in the input sudoku
    {
        for(int j = 0; j<9; j++)
        {
            if(grid_array[i][j] == 0)   // if a value of the grid is 0...
            {
                number_gaps++;   //... the variable 'number_gaps' is incremented
            }
            if(verif_2(i, j) == false && (grid_array[i][j] != 0))   // check if the Sudoku inputs are valid
            {
                open_error();   // if they are not open the error dialog
                goto error;   // stop the program, go back to the GUI grid to change the erroneous value(s)
            }
        }
    }

    while(number_gaps && clean_loop)   // this is the first loop that fills in the easy numbers and possibly solve the easy Sudokus
    {
        clean_loop = 0;
        for(int i = 0; i<9; i++)
        {
            for(int j = 0; j<9; j++)
            {
                if(grid_array[i][j] != 0)   // if the cell is not a gap, then change cell (go to the next one)
                    continue;
                temp = 0;   // set a temporary variable to 0
                for (int k = 1; k<10; k++)
                {
                    if (verif_1(i, j, k))   // verifies if the placed nummber is within the rules of the sudoku
                    {
                        if(!temp)
                        {
                            temp = k;   // if it finds a first correct value for the gap, it will replace the temp
                        }
                        else
                        {
                            temp = 0;   // if two values are found on a second loop, it makes temp's value returns  back to 0
                            break;
                        }
                    }
                }
                if (temp)   // if a value of temp is found...
                {
                    grid_array[i][j] = temp;   //... it repplaces the gap
                    clean_loop = 1;
                    number_gaps--;   // the number of gaps is therefore reduced
                }
            }
        }
    }

    if (number_gaps>0)   // this if statement calls in another function if the Sudou hasn't been solved yet while still keeping current progress
    {
        solve_2(0, 0);
        number_gaps = 0;   // resets the number of gaps in the Sudoku
        for(int i = 0; i<9; i++)
        {
                for(int j = 0; j<9; j++)
                {
                        grid_array[i][j] = atoi(gtk_entry_get_text(GTK_ENTRY(grid_widg[i][j])));   // enetrs the value of the grid to the grid_array
                        if(grid_array[i][j] == 0)   // find the number of gaps in the Sudok (it is most certainly going to be 0)
                        {
                            number_gaps++;
                        }
                }
        }

    }

    if (number_gaps == 0)   // if there are no remaining gaps in the Sudoku, it is solved!
    {
        cout << "The Sudoku has been successfuly solved!" << endl;
        t = clock() - t;   // these next 2 lines assaign/display the time it took for the solve function to solve the sudoku
        cout << "It took " << ((float)t)/CLOCKS_PER_SEC << " seconds to solve this sudoku!" << endl << endl;

         for(int i = 0; i<9; i++)   // these next two 'for lops' enter the final values into the GUI/grid
            {
                for(int j = 0; j<9; j++)
                    {
                    char charact[2];   // creates a char array
                    sprintf(charact, "%d", grid_array[i][j]);   // input it into a char called charact
                    gtk_entry_set_text(GTK_ENTRY(grid_widg[i][j]), charact);   // displays it into the GUI Grid
                    }
            }
    }
    else if (number_gaps > 0)   // if notdisplays and error, both in the terminal and the GUI
    {
        open_error();   // opens the dialog window to display an error, the sudoku cannot be solved
        error:
        cout << "For some reason, the Sudoku cannot be solved..." << endl;
        cout << "Try checking the values entered, the input file, or even the Sudoku itself!" << endl << endl;

    }
}

void solve_2(int row, int column)   /// If the Sudoku is still not solved after the first solve function, this one is called
{
	static bool found = false; // set true when one solution is found

    bool free_cell[9][9];   // we will use this boolean value to determine where are the remaining free cells in the grid

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (grid_array[i][j] == 0)
            {
                free_cell[i][j] = true;   // it will be equal to 1/true if it is free...
            }
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (grid_array[i][j] > 0)
            {
                free_cell[i][j] = false;   //... and to 0/false if not
            }


	if (!free_cell[row][column])   // checks to see if the cell is not free
	{
	    if (column<8)   // if it is not, check the next one in the column
			solve_2(row, column+1);   // calls back the solve_2 function but if column is under 8 excluded just add 1 to column and start again
		else if (row<8)   // then change row at the end of the previous column
			solve_2(row+1, 0);
		else   // else (all are filled, so it's good news) fill-up solutions in the GUI Grid
		{
            for (int i = 0; i<9; i++)
            {
                for (int j = 0; j<9; j++)
                {
                    char charact[2];   // creates a char array
                    sprintf(charact, "%d", grid_array[i][j]);   // input it into a char called charact
                    gtk_entry_set_text(GTK_ENTRY(grid_widg[i][j]), charact);   // displays it into the GUI Grid
                }
            }
 //           found = true;   // if a solution is found, change bool value to 1/true
		}
	}
	else   // else (if the cell is free)
	{
		for (int k = 1; k < 10; k++)   // input a value to the 2D array (temporary one)
		{
			grid_array[row][column] = k;   // this is the temporary one
			if (verif_2(row, column))   // if the number we placed verifies the rules of the Sudoku...
			{
				if (column <8)   //... continue the solve_2 loop following the same rules as it the fisrt loop
                {
					solve_2(row, column + 1);   // increassing column value at the end of the column
                }
				else if (row <8)
                {
					solve_2(row + 1, 0);   // increasing the row value to be able to check all of the cells
                }
				else   // here again fill-up solutions in the GUI Grid
				{
                    for (int i = 0; i < 9; i++)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            char charact[2];   // creates a char array
                            sprintf(charact, "%d", grid_array[i][j]);   // input it into a char called charact
                            gtk_entry_set_text(GTK_ENTRY(grid_widg[i][j]), charact);   // displays it into the GUI Grid
                        }
                    }
				}
			}
		}
		if (!found)   // if no value is found...
            grid_array[row][column] = 0;   //... (if they all fail to obeys the Sudoku rules) it is set back to 0
	}
}

bool verif_1(int i, int j, int k)   /// This function verifies that the number k can be put in the cell grid_array[i][j]
{
    for(int column = 0; column<9; column++)   // verifies if there is the same number on the same row
        if (grid_array[i][column] == k)
            return false;

    for(int row = 0; row<9; row++)   // verifies if there is the same number on the same column
        if (grid_array[row][j] == k)
            return false;

    int row;
    if (i>=0 && i<3)   // creates 'virtual' blocks using logic
        row = 1;
    if (i>=3 && i<6)
        row = 2;
    if (i>=6 && i<9)
        row =3;

    int column;
    if (j>=0 && j<3)   // creates 'virtual' blocks using logic
        column = 1;
    if (j>=3 && j<6)
        column = 2;
    if (j>=6 && j<9)
        column = 3;

    for(int f = (row-1)*3; f<(row-1)*3+3; f++)
        for(int e = (column-1)*3; e<(column-1)*3+3; e++)
            if(grid_array[f][e] == k)   // verifies if there is the same number in the same block
                return false;

    return true;  // the value k that we are going to place at grid_array[i][j] is correct (method 1 of verificcation)
}

bool verif_2(int i, int j)   /// This function verifies that the value already entered at the cell grid_array[i][j] is correct
{
    for (int column = 0; column<9; column++)   // verifies if there is the same number on the same row
        if (column != j && grid_array[i][column] == grid_array[i][j])
			return false;

    for (int row = 0; row<9; row++)   // verifies if there is the same number on the same column
		if (row != i && grid_array[row][j] == grid_array[i][j])
			 return false;

	for (int row = (i/3)*3; row<(i/3)*3+3; row++)   // verifies if there is the same number on the same block
		for (int col = (j/3)*3; col<(j/3)*3+3; col++)
			 if (row != i && col != j && grid_array[row][col] == grid_array[i][j])
				return false;

	return true;   // the current value already inputed at grid[i][j] is valid (method 2 of verification)
}

void open_error()   /// This function is called when there is an error/the programm cannot solve the Sudoku and displays an error through an Error Window
{
        GtkWidget* dialog, *label;
        dialog = gtk_dialog_new_with_buttons("Error", GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);   // creating a window that will pop-up when an error is found
        label = gtk_label_new("This Sudoku cannot be solved! Try checking the inputs...");   // It will have this appropriate label
        // gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog) -> vbox), label, 0, 0, 0);
        gtk_widget_show_all(dialog);   // shows the widget created
        gtk_dialog_run(GTK_DIALOG(dialog));   // continues to run until the OK signal is sent back
        gtk_widget_destroy(dialog);   // again destroys the dialog as we don't need it after the void function is completed
}

/// END OF CODE ///
/// by Alexandre Benoit ///
