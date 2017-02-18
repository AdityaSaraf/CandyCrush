#include <string>

#include "Game.h"

extern "C" {
  #include <gtk/gtk.h>
}

Game game;
GtkWidget *selected;

void ccselect(GtkWidget *widget, gpointer user_data) {

}

void ccswap(GtkWidget *widget, gpointer user_data) {

}

void writeMoves() {

}

void ccactivate (GtkApplication *app, gpointer user_data) {
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

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      button = gtk_button_new();
      //setButtonImage(button, i, j);
      gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);
      g_signal_connect(button, "clicked", G_CALLBACK(ccselect), grid);
      gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1);
    }
  }
  
  icon = gtk_image_new_from_file("images/direction/up.png");
  button = gtk_button_new();
  int *upbits = (int*) malloc(sizeof(int));
  *upbits = 8;
  g_object_set_data(G_OBJECT(button), "bits", upbits);
  gtk_button_set_image(GTK_BUTTON(button), icon);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(ccswap), grid);
  gtk_grid_attach(GTK_GRID(grid), button, 1, 1, 5, 1);

  icon = gtk_image_new_from_file("images/direction/left.png");
  button = gtk_button_new();
  int *leftbits = (int*) malloc(sizeof(int));
  *leftbits = 4;
  g_object_set_data(G_OBJECT(button), "bits", leftbits);
  gtk_button_set_image(GTK_BUTTON(button), icon);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(ccswap), grid);
  gtk_grid_attach(GTK_GRID(grid), button, 1, 2, 5, 1);

  icon = gtk_image_new_from_file("images/direction/right.png");
  button = gtk_button_new();
  int *rightbits = (int*) malloc(sizeof(int));
  *rightbits = 1;
  g_object_set_data(G_OBJECT(button), "bits", rightbits);
  gtk_button_set_image(GTK_BUTTON(button), icon);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(ccswap), grid);
  gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 5, 1);

  icon = gtk_image_new_from_file("images/direction/down.png");
  button = gtk_button_new();
  int *downbits = (int*) malloc(sizeof(int));
  *downbits = 2;
  g_object_set_data(G_OBJECT(button), "bits", downbits);
  gtk_button_set_image(GTK_BUTTON(button), icon);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(ccswap), grid);
  gtk_grid_attach(GTK_GRID(grid), button, 1, 4, 5, 1);

  //label = gtk_label_new(NULL);
  //gtk_grid_attach(GTK_GRID(grid), label, 1, 0, 5, 1);
  //writeMoves();
  

  gtk_widget_show_all (window);
}

static void ccopen(GtkApplication *app, GFile **files, gint n_files, const gchar *hint, gpointer user_data) {
  game.Init(g_file_get_path(files[0]));
  g_application_activate(G_APPLICATION(app));
}

int main(int argc, char **argv) {
  int status = 0;

  // create a new gtk application to run it it in
  // the app will open files and on activation 
  // create the board from the initial game state
  // in the serialization file
  // can't use the methods from hw2 for json anymore
  // since we'll have an array of Candys, instead of
  // ints, and hw2 specifies ints, tho we could just
  // modify that method
  GtkApplication *app;

  app = gtk_application_new("com.github.adityasaraf.CandyCrush", G_APPLICATION_HANDLES_OPEN);
  g_signal_connect(app, "open", G_CALLBACK(ccopen), NULL);
  g_signal_connect(app, "activate", G_CALLBACK(ccactivate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
