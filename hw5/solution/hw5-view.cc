// this file will be the view, so it will implement the gtk things
// with the callback functions being mainly message sending
#include <iostream>
#include <cstdlib>

using namespace std;

#include "ServerSocket.h"
#include "ClientSocket.h"
#include "MessageHandler.h"
#include "Message.h"

extern "C" {
  #include <jansson.h>
  #include <gtk/gtk.h>
}

/*
static void ccswap(GtkWidget *widget, gpointer user_data) {
  if (selected) {
    GValue GRow = G_VALUE_INIT;
    GValue GCol = G_VALUE_INIT;
    g_value_init(&GRow, G_TYPE_INT);
    g_value_init(&GCol, G_TYPE_INT);
    gtk_container_child_get_property(GTK_CONTAINER(widget), selected, "top-attach", &GRow);
    gtk_container_child_get_property(GTK_CONTAINER(widget), selected, "left-attach", &GCol);
    int row = g_value_get_int(&GRow);
    int col = g_value_get_int(&GCol);
    int dir = *(int*) g_object_get_data(G_OBJECT(user_data), "bits");
    json_t *root = json_object();
    json_t *action = json_string("move");
    json_t *jrow = json_integer(row);
    json_t *jcol = json_integer(col);
    json_t *jdir = json_integer(dir);
    json_object_set_new(root, "action", action);
    json_object_set_new(root, "row", jrow);
    json_object_set_new(root, "column", jcol);
    json_object_set_new(root, "direction", jdir);
    char *jmove = json_dumps(root, 0);
    string move(jmove);
    free(jmove);
    json_decref(root);
    
  }
}
*/
static void ccactivate(GtkApplication *app, gpointer user_data) {
/*  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button;
  GtkWidget *icon;

  // create a new window, and set its title
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "CandyCrush");
  gtk_window_set_default_size (GTK_WINDOW (window), 400, 300);
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);

  // Here we construct the container that is going pack our buttons
  grid = gtk_grid_new ();
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

  // Pack the container in the window
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
  *upbits = 3;
  g_object_set_data(G_OBJECT(button), "bits", upbits);
  gtk_button_set_image(GTK_BUTTON(button), icon);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(ccswap), grid);
  gtk_grid_attach(GTK_GRID(grid), button, game.GetCols() + 1, 2, 5, 1);

  icon = gtk_image_new_from_file("images/direction/left.png");
  button = gtk_button_new();
  int *leftbits = (int*) malloc(sizeof(int));
  *leftbits = 0;
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

  gtk_widget_show_all (window); */
}

static void ccopen(GtkApplication *app, GFile **files, gint n_files, const gchar *hint, gpointer user_data) {
  json_t *root;
  json_error_t error;
  root = json_load_file(g_file_get_path(files[0]), 0, &error);
  json_t *action = json_string("helloack");
  json_object_set_new(root, "action", action);
  char *gameinstance = json_dumps(root, 0);
  string helloack(gameinstance);
  free(gameinstance);
  HelloackMessage msg(helloack);
  MessageHandler *mhp = (MessageHandler *) user_data;
  mhp->SendMessage(msg);
  g_application_activate(G_APPLICATION(app));
}

void usage(const char *name) {
  cout << "Usage: " << name << " file [port]" << endl;
  cout << "  Gives the file to connecting models " << endl;
  cout << "  to initialize from. Uses a random " << endl;
  cout << "  if not given one. " << endl;
  exit(1);
}

int main(int argc, char **argv) {
  // make a socket
  // deserialize the json
  // wait for a connection
  // send the gamestate to model
  // start gtk upon getting the update from the model
  
  if (argc != 2 && argc != 3) usage(argv[0]);

  int port = 0;
  try {
    if (argc == 3) port = stoi(argv[2]);
  } catch (...) {
    usage(argv[0]);
  }

  try {
    int socketFd;
    hw5_net::ServerSocket ssock(port);
    ssock.BindAndListen(AF_INET, &socketFd);
    cout << "Created bound socket on port = " << ssock.port() << endl;
    
    while (1) {
      int acceptedFd;
      string clientAddr;
      uint16_t clientPort;
      string clientDNSName;
      string serverAddress;
      string serverDNSName;

      ssock.Accept(&acceptedFd,
                   &clientAddr,
                   &clientPort,
                   &clientDNSName,
                   &serverAddress,
                   &serverDNSName);

      hw5_net::ClientSocket csock(acceptedFd);

      MessageHandler msgh(acceptedFd);
      MessageHandler *mhp = &msgh;
      Message msg = mhp->GetNextMessage();

      if (msg.GetType() == "hello") {
        //int status = 0;
        GtkApplication *app;
        app = gtk_application_new("com.github.adityasaraf.CandyCrush", G_APPLICATION_HANDLES_OPEN);
        g_signal_connect(app, "open", G_CALLBACK(ccopen), (gpointer) mhp);
        g_signal_connect(app, "activate", G_CALLBACK(ccactivate), (gpointer) mhp);
        g_application_run(G_APPLICATION(app), argc, argv);
        g_object_unref(app);
      }
    }

  } catch (std::string strError) {
    std::cout << strError << std::endl;
    return EXIT_FAILURE;
  }

  return 1;
}
