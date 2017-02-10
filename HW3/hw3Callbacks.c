#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <jansson.h>

#include "hw2_modified.h"
#include "hw3Callbacks.h"

extern Array2D board;
extern GtkWidget *selected;
extern int moves;
extern GtkWidget* label;

void
setButtonImage(GtkWidget *button, int row, int col) {
  GtkWidget *icon;
  int color = *(int*) Array2D_get(board, board->rows - row - 1, col);
  char filename[30];
  strcpy(filename, "images/40x40/");
  if (color == 0) strcat(filename, "blue.png");
  else if (color == 1) strcat(filename, "green.png");
  else if (color == 2) strcat(filename, "orange.png");
  else if (color == 3) strcat(filename, "purple.png");
  else if (color == 4) strcat(filename, "red.png");
  else if (color == 5) strcat(filename, "yellow.png"); 
  icon = gtk_image_new_from_file(filename);
  gtk_button_set_image(GTK_BUTTON(button), icon);
}

void
redraw(GtkWidget *widget, int r1, int c1, int r2, int c2) {
  GtkWidget *button = gtk_grid_get_child_at(GTK_GRID(widget), c1, r1);
  setButtonImage(button, r1, c1);
  button = gtk_grid_get_child_at(GTK_GRID(widget), c2, r2);
  setButtonImage(button, r2, c2);
}

void
writeMoves ()
{
  char msg[32] = {0};
  g_snprintf(msg, sizeof msg, "%d moves made", moves);
  gtk_label_set_text(GTK_LABEL(label), msg);
}

void
select (GtkWidget *widget, gpointer user_data) {
  if (selected) gtk_button_set_relief(GTK_BUTTON(selected), GTK_RELIEF_NONE);
  selected = widget;
  gtk_button_set_relief(GTK_BUTTON(widget), GTK_RELIEF_NORMAL);
}

void
swap (GtkWidget *widget, gpointer user_data) {
  if (selected) {
    GValue GRow = G_VALUE_INIT;
    GValue GCol = G_VALUE_INIT;
    g_value_init(&GRow, G_TYPE_INT);
    g_value_init(&GCol, G_TYPE_INT);
    gtk_container_child_get_property(GTK_CONTAINER(widget), selected, "top-attach", &GRow);
    gtk_container_child_get_property(GTK_CONTAINER(widget), selected, "left-attach", &GCol);
    int row = g_value_get_int(&GRow);
    int col = g_value_get_int(&GCol);
    int bits = *(int*) g_object_get_data(G_OBJECT(user_data), "bits");
    // 1000 (8) = up, 0100 (4) = left, 0010 (2) = down, 0001 (1) = right. think WASD. what follows are
    // fancy masks such that ud is 1 for up and -1 for down and lr is -1 for left and 1 for right
    int ud = ((bits&8)>>3) + ~((bits&2)>>1)+1;
    int lr = ~((bits&4)>>2)+1 + (bits&1);
    if (Array2D_swap(board, board->rows - row - 1, col, board->rows - row - 1 + ud, col + lr) == 1) {
      redraw(widget, row, col, row - ud, col + lr);
      gtk_button_set_relief(GTK_BUTTON(selected), GTK_RELIEF_NONE);
      selected = NULL;
      moves++;
      writeMoves();
    }
    else
    {
      printf("Error: cannot swap candy off board!\n");
    }
    g_value_unset(&GRow);
    g_value_unset(&GCol);
  }
  else
  {
    printf("Error: no candy selected!\n");
  }
}

void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button;
  GtkWidget *icon;

  /* create a new window, and set its title */
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "CandyCrush");
  gtk_window_set_default_size (GTK_WINDOW (window), 600, 600);
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);

  /* Here we construct the container that is going pack our buttons */
  grid = gtk_grid_new ();
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

  /* Pack the container in the window */
  gtk_container_add (GTK_CONTAINER (window), grid);

  for (int i = 0; i < board->rows; i++) {
    for (int j = 0; j < board->cols; j++) {
      button = gtk_button_new();
      setButtonImage(button, i, j);
      gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);
      g_signal_connect(button, "clicked", G_CALLBACK(select), grid);
      gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1);
    }
  }
  
  icon = gtk_image_new_from_file("images/direction/up.png");
  button = gtk_button_new();
  int *upbits = (int*) malloc(sizeof(int));
  *upbits = 8;
  g_object_set_data(G_OBJECT(button), "bits", upbits);
  gtk_button_set_image(GTK_BUTTON(button), icon);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(swap), grid);
  gtk_grid_attach(GTK_GRID(grid), button, board->cols + 1, 1, 5, 1);

  icon = gtk_image_new_from_file("images/direction/left.png");
  button = gtk_button_new();
  int *leftbits = (int*) malloc(sizeof(int));
  *leftbits = 4;
  g_object_set_data(G_OBJECT(button), "bits", leftbits);
  gtk_button_set_image(GTK_BUTTON(button), icon);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(swap), grid);
  gtk_grid_attach(GTK_GRID(grid), button, board->cols + 1, 2, 5, 1);

  icon = gtk_image_new_from_file("images/direction/right.png");
  button = gtk_button_new();
  int *rightbits = (int*) malloc(sizeof(int));
  *rightbits = 1;
  g_object_set_data(G_OBJECT(button), "bits", rightbits);
  gtk_button_set_image(GTK_BUTTON(button), icon);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(swap), grid);
  gtk_grid_attach(GTK_GRID(grid), button, board->cols + 1, 3, 5, 1);

  icon = gtk_image_new_from_file("images/direction/down.png");
  button = gtk_button_new();
  int *downbits = (int*) malloc(sizeof(int));
  *downbits = 2;
  g_object_set_data(G_OBJECT(button), "bits", downbits);
  gtk_button_set_image(GTK_BUTTON(button), icon);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(swap), grid);
  gtk_grid_attach(GTK_GRID(grid), button, board->cols + 1, 4, 5, 1);

  label = gtk_label_new(NULL);
  gtk_grid_attach(GTK_GRID(grid), label, board->cols + 1, 0, 5, 1);
  writeMoves();
  

  gtk_widget_show_all (window);
}
