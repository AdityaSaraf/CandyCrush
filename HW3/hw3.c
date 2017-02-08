#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <jansson.h>

#include "hw2_modified.h"

Array2D board;
GtkWidget *selected;

static void
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

static void
select (GtkWidget *widget, gpointer user_data) {
  if (selected) gtk_button_set_relief(GTK_BUTTON(selected), GTK_RELIEF_NONE);
  selected = widget;
  gtk_button_set_relief(GTK_BUTTON(widget), GTK_RELIEF_NORMAL);
}

static void
redraw(GtkWidget *widget, int r1, int c1, int r2, int c2) {
  GtkWidget *button = gtk_grid_get_child_at(GTK_GRID(widget), c1, r1);
  setButtonImage(button, r1, c1);
  button = gtk_grid_get_child_at(GTK_GRID(widget), c2, r2);
  setButtonImage(button, r2, c2);
}

static void
swap_up (GtkWidget *widget, gpointer user_data) {
  if (selected) {
    GValue GRow = G_VALUE_INIT;
    GValue GCol = G_VALUE_INIT;
    g_value_init(&GRow, G_TYPE_INT);
    g_value_init(&GCol, G_TYPE_INT);
    gtk_container_child_get_property(GTK_CONTAINER(widget), selected, "top-attach", &GRow);
    gtk_container_child_get_property(GTK_CONTAINER(widget), selected, "left-attach", &GCol);
    int row = g_value_get_int(&GRow);
    int col = g_value_get_int(&GCol);
    if (Array2D_swap(board, board->rows - row - 1, col, board->rows - row, col) == 1) {
      redraw(widget, row, col, row - 1, col);
    }
    gtk_button_set_relief(GTK_BUTTON(selected), GTK_RELIEF_NONE);
  }
}

static void
swap_left (GtkWidget *widget, gpointer user_data) {
  if (selected) {
    GValue GRow = G_VALUE_INIT;
    GValue GCol = G_VALUE_INIT;
    g_value_init(&GRow, G_TYPE_INT);
    g_value_init(&GCol, G_TYPE_INT);
    gtk_container_child_get_property(GTK_CONTAINER(widget), selected, "top-attach", &GRow);
    gtk_container_child_get_property(GTK_CONTAINER(widget), selected, "left-attach", &GCol);
    int row = g_value_get_int(&GRow);
    int col = g_value_get_int(&GCol);
    if (Array2D_swap(board, board->rows - row - 1, col, board->rows - row - 1, col - 1) == 1) {
      redraw(widget, row, col, row, col - 1);
    }
    gtk_button_set_relief(GTK_BUTTON(selected), GTK_RELIEF_NONE);
  }
}

static void
swap_right (GtkWidget *widget, gpointer user_data) {

  if (selected) {
    GValue GRow = G_VALUE_INIT;
    GValue GCol = G_VALUE_INIT;
    g_value_init(&GRow, G_TYPE_INT);
    g_value_init(&GCol, G_TYPE_INT);
    gtk_container_child_get_property(GTK_CONTAINER(widget), selected, "top-attach", &GRow);
    gtk_container_child_get_property(GTK_CONTAINER(widget), selected, "left-attach", &GCol);
    int row = g_value_get_int(&GRow);
    int col = g_value_get_int(&GCol);
    if (Array2D_swap(board, board->rows - row - 1, col, board->rows - row - 1, col + 1) == 1) {
      redraw(widget, row, col, row, col + 1);
    }
    gtk_button_set_relief(GTK_BUTTON(selected), GTK_RELIEF_NONE);
  }
}

static void
swap_down (GtkWidget *widget, gpointer user_data) {
  if (selected) {
    GValue GRow = G_VALUE_INIT;
    GValue GCol = G_VALUE_INIT;
    g_value_init(&GRow, G_TYPE_INT);
    g_value_init(&GCol, G_TYPE_INT);
    gtk_container_child_get_property(GTK_CONTAINER(widget), selected, "top-attach", &GRow);
    gtk_container_child_get_property(GTK_CONTAINER(widget), selected, "left-attach", &GCol);
    int row = g_value_get_int(&GRow);
    int col = g_value_get_int(&GCol);
    if (Array2D_swap(board, board->rows - row - 1, col, board->rows - row - 2, col) == 1) {
      redraw(widget, row, col, row + 1, col);
    }
    gtk_button_set_relief(GTK_BUTTON(selected), GTK_RELIEF_NONE);
  }
}

static void
open (GtkApplication *app,
      GFile          **files,
      gint            n_files,
      const gchar    *hint)
{
  board = deserialize_array(g_file_get_path(files[0]));
  g_application_activate(G_APPLICATION(app));
}

static void
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
  gtk_window_set_default_size (GTK_WINDOW (window), 1000, 1000);
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
      g_signal_connect(button, "pressed", G_CALLBACK(select), grid);
      gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1);
    }
  }
  
  icon = gtk_image_new_from_file("images/direction/up.png");
  button = gtk_button_new();
  gtk_button_set_image(GTK_BUTTON(button), icon);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(swap_up), grid);
  gtk_grid_attach(GTK_GRID(grid), button, board->cols + 1, 0, 5, 1);

  icon = gtk_image_new_from_file("images/direction/left.png");
  button = gtk_button_new();
  gtk_button_set_image(GTK_BUTTON(button), icon);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(swap_left), grid);
  gtk_grid_attach(GTK_GRID(grid), button, board->cols + 1, 1, 5, 1);

  icon = gtk_image_new_from_file("images/direction/right.png");
  button = gtk_button_new();
  gtk_button_set_image(GTK_BUTTON(button), icon);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(swap_right), grid);
  gtk_grid_attach(GTK_GRID(grid), button, board->cols + 1, 2, 5, 1);

  icon = gtk_image_new_from_file("images/direction/down.png");
  button = gtk_button_new();
  gtk_button_set_image(GTK_BUTTON(button), icon);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(swap_down), grid);
  gtk_grid_attach(GTK_GRID(grid), button, board->cols + 1, 3, 5, 1);
  

  gtk_widget_show_all (window);
}

int main(int argc, char *argv[]) {
  int status = 0;
	if (argc != 2)
	{
		printf("usage: %s filename\n", argv[0]);
    return EXIT_FAILURE;
	}
	else
	{

      GtkApplication *app;

		  app = gtk_application_new ("com.github.adityasaraf.CandyCrush", G_APPLICATION_HANDLES_OPEN);
		  g_signal_connect (app, "open", G_CALLBACK (open), NULL);
      g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
		  status = g_application_run (G_APPLICATION (app), argc, argv);
		  g_object_unref (app);
      Array2D_destroy(board, free);
	}


	return status;
}
