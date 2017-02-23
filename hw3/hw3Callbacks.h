#ifndef HW3CALLBACKS_H
#define HW3CALLBACKS_H

// Sets the image of the button to correspond to the value from the deserialized
// file
// Parameter: 	button: the button to be set
//				row, col: the coordinates of the buttons
void
setButtonImage(GtkWidget *button, int row, int col);

// Redraw the candies at the two given coordinates
// Parameter: 	widget = the grid containing the buttons
//				r1, c1, r2, c2 = the coordinates of the modified buttons
void
redraw(GtkWidget *widget, int r1, int c1, int r2, int c2);

// Updates move label with the correct value of moves
void
writeMoves();

// Sets the currently selected button to the most recently clicked
// Paramter:	widget: the button clicked on
void
select (GtkWidget *widget, gpointer user_data);

// Swap the given button whichever direction it should go. Increments moves
// Parameter:	widget: the selected button
//				user_data: contains the direction that the button should be
//						   swapped
// Prints error msgs to stdout for invalid swaps
void
swap (GtkWidget *widget, gpointer user_data);

// Opens the file given via stdin to the program and constructs the Array2D
// for the board

// Initalizes the app and calls the other functions to set the correct signals
void
activatecc (GtkApplication *app,
          gpointer        user_data);

#endif // HW3CALLBACKS_H
