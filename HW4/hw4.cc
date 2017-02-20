#include <string>

#include "Game.h"
#include <iostream>

extern "C" {
  #include <gtk/gtk.h>
  #include <string.h>
}

Game game;
GtkWidget *selected;
GtkWidget *moves;
GtkWidget *score;

void writeMoves() {
  char msg[32] = {0};
  g_snprintf(msg, sizeof msg, "%d moves made", game.GetMoves());
  gtk_label_set_text(GTK_LABEL(moves), msg);
}

void writeScore() {
  char msg[32] = {0};
  g_snprintf(msg, sizeof msg, "Score: %d", game.GetScore());
  gtk_label_set_text(GTK_LABEL(score), msg);
}

void setButtonImage(GtkWidget *button, const int row, const int col ) {
  GtkWidget *icon;
  int color = game.GetColor(game.GetRows() - row - 1, col);
  int state = game.GetState(game.GetRows() - row - 1, col);
  char filename[50];
  strcpy(filename, "images/regular/");
  if (state == 0) strcat(filename, "state0/");
  else if (state == 1) strcat(filename, "state1/");
  else if (state == 2) strcat(filename, "state2/");

  if (color == -1) strcat(filename, "nocolor.png");
  else if (color == 0) strcat(filename, "blue.png");
  else if (color == 1) strcat(filename, "green.png");
  else if (color == 2) strcat(filename, "orange.png");
  else if (color == 3) strcat(filename, "purple.png");
  else if (color == 4) strcat(filename, "red.png");
  else if (color == 5) strcat(filename, "yellow.png");
  icon = gtk_image_new_from_file(filename);
  gtk_button_set_image(GTK_BUTTON(button), icon);
}

void redraw(GtkWidget *grid) {
  for (int i = 0; i < game.GetRows(); i++) {
    for (int j = 0; j < game.GetCols(); j++) {
      GtkWidget *button = gtk_grid_get_child_at(GTK_GRID(grid), j, i);
      setButtonImage(button, i, j);
    }
  }
}

void ccselect(GtkWidget *widget, gpointer user_data) {
  if (selected) gtk_button_set_relief(GTK_BUTTON(selected), GTK_RELIEF_NONE);
  selected = widget;
  gtk_button_set_relief(GTK_BUTTON(widget), GTK_RELIEF_NORMAL);
}

void ccswap(GtkWidget *widget, gpointer user_data) {
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
    int code = game.Swap(game.GetRows() - row - 1, col, game.GetRows() - row - 1 + ud, col + lr);
    if (code == 1) {
      gtk_button_set_relief(GTK_BUTTON(selected), GTK_RELIEF_NONE);
      selected = (GtkWidget *) 0;
      writeMoves();
      writeScore();
      redraw(widget);
      if (game.IsWon()) {
        std::cout<<"You've won!"<<std::endl;
      }
    }
    else if (code == 0) // cannot settle
    {
    	std::cout<<"Error: swap wouldn't result in removal of any candies!"<<std::endl;
    }
    else
    {
      printf("Error: cannot swap candy off board!\n");
    }
    g_value_unset(&GRow);
    g_value_unset(&GCol);
  } else {
    printf("Error: no candy selected!\n");
  }
}

void ccactivate (GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button;
  GtkWidget *icon;

  /* create a new window, and set its title */
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "CandyCrush");
  gtk_window_set_default_size (GTK_WINDOW (window), 400, 300);
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);

  /* Here we construct the container that is going pack our buttons */
  grid = gtk_grid_new ();
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

  /* Pack the container in the window */
  gtk_container_add (GTK_CONTAINER (window), grid);

  for (int i = 0; i < game.GetRows(); i++) {
    for (int j = 0; j < game.GetCols(); j++) {
      button = gtk_button_new();
      setButtonImage(button, i, j);
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
  gtk_grid_attach(GTK_GRID(grid), button, game.GetCols() + 1, 2, 5, 1);

  icon = gtk_image_new_from_file("images/direction/left.png");
  button = gtk_button_new();
  int *leftbits = (int*) malloc(sizeof(int));
  *leftbits = 4;
  g_object_set_data(G_OBJECT(button), "bits", leftbits);
  gtk_button_set_image(GTK_BUTTON(button), icon);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(ccswap), grid);
  gtk_grid_attach(GTK_GRID(grid), button, game.GetCols() + 1, 3, 5, 1);

  icon = gtk_image_new_from_file("images/direction/right.png");
  button = gtk_button_new();
  int *rightbits = (int*) malloc(sizeof(int));
  *rightbits = 1;
  g_object_set_data(G_OBJECT(button), "bits", rightbits);
  gtk_button_set_image(GTK_BUTTON(button), icon);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(ccswap), grid);
  gtk_grid_attach(GTK_GRID(grid), button, game.GetCols() + 1, 4, 5, 1);

  icon = gtk_image_new_from_file("images/direction/down.png");
  button = gtk_button_new();
  int *downbits = (int*) malloc(sizeof(int));
  *downbits = 2;
  g_object_set_data(G_OBJECT(button), "bits", downbits);
  gtk_button_set_image(GTK_BUTTON(button), icon);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(ccswap), grid);
  gtk_grid_attach(GTK_GRID(grid), button, game.GetCols() + 1, 5, 5, 1);

  moves = gtk_label_new(NULL);
  gtk_grid_attach(GTK_GRID(grid), moves, game.GetCols() + 1, 0, 5, 1);
  writeMoves();

  score = gtk_label_new(NULL);
  gtk_grid_attach(GTK_GRID(grid), score, game.GetCols() + 1, 1, 5, 1);
  writeScore();

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
