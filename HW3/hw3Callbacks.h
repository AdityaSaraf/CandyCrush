#ifndef HW3CALLBACKS_H
#define HW3CALLBACKS_H

// Sets the image of the button to correspond to the value from the deserialized
// file
// Parameter: 	button: the button to be set
//				row, col: the coordinates of the buttons
static void
setButtonImage(GtkWidget *button, int row, int col);

// Redraw the candies at the two given coordinates
// Parameter: 	widget = the grid containing the buttons
//				r1, c1, r2, c2 = the coordinates of the modified buttons
static void
redraw(GtkWidget *widget, int r1, int c1, int r2, int c2);

// Updates move label with the correct value of moves
static void
writeMoves();

// Sets the currently selected button to the most recently clicked
// Paramter:	widget: the button clicked on
static void
select (GtkWidget *widget, gpointer user_data);

// Swap the given button whichever direction it should go. Increments moves
// Parameter:	widget: the selected button
//				user_data: contains the direction that the button should be
//						   swapped
static void
swap (GtkWidget *widget, gpointer user_data);

// Opens the file given via stdin to the program and constructs the Array2D
// for the board
static void
open (GtkApplication *app,
      GFile          **files,
      gint            n_files,
      const gchar    *hint);

// Initalizes the app and calls the other functions to set the correct signals
static void
activate (GtkApplication *app,
          gpointer        user_data);

#endif // HW3CALLBACKS_H