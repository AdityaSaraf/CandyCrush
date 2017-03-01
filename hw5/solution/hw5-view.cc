// this file will be the view, so it will implement the gtk things
// with the callback functions being mainly message sending
#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

#include "ServerSocket.h"
#include "ClientSocket.h"
#include "MessageHandler.h"
#include "Message.h"
#include "Info_Container.h"

extern "C" {
  #include <jansson.h>
  #include <gtk/gtk.h>
  #include <string.h>
}

typedef struct gridMsg_t{
  GtkWidget *grid;
  MessageHandler *mhp;
}* GridMsg;


GtkWidget *selected;
GtkWidget *moves;
GtkWidget *score;
Info_Container info;

void writeMoves() {
  char msg[32] = {0};
  g_snprintf(msg, sizeof msg, "%d moves made", info.GetMoves());
  gtk_label_set_text(GTK_LABEL(moves), msg);
}

void writeScore() {
  char msg[32] = {0};
  g_snprintf(msg, sizeof msg, "Score: %d", info.GetScore());
  gtk_label_set_text(GTK_LABEL(score), msg);
}

void setButtonImage(GtkWidget *button, const int row, const int col ) {
//Boardstate": {"info.GetRows()": 6, "columns": 6, "data": [2, 1, 2, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1]}}}}'

  GtkWidget *icon;
  int color = info.GetColor(info.GetRows() - row - 1, col);
  int state = info.GetState(info.GetRows() - row - 1, col);
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
  for (int i = 0; i < info.GetRows(); i++) {
    for (int j = 0; j < info.GetCols(); j++) {
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

static void ccswap(GtkWidget *widget, gpointer user_data) {
  if (selected) {
    GridMsg gm = (GridMsg) user_data;
    GtkWidget *grid = gm->grid;
    GValue GRow = G_VALUE_INIT;
    GValue GCol = G_VALUE_INIT;
    g_value_init(&GRow, G_TYPE_INT);
    g_value_init(&GCol, G_TYPE_INT);
    gtk_container_child_get_property(GTK_CONTAINER(widget), selected, "top-attach", &GRow);
    gtk_container_child_get_property(GTK_CONTAINER(widget), selected, "left-attach", &GCol);
    int row = g_value_get_int(&GRow);
    int col = g_value_get_int(&GCol);
    int dir = *(int*) g_object_get_data(G_OBJECT(grid), "bits");
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
    MessageHandler* mhp = gm->mhp;
    MoveMessage moveM(move);  
    mhp->SendMessage(moveM);
    Message update = mhp->GetNextMessage();
    if (update.GetType() != "update")
    {
      cout << "Error: invalid Message Type" << endl;
    }
    const char* newInstance = update.GetData().c_str();
    info.Init(newInstance);
  } else {
    printf("Error: no candy selected!\n");
  }
}

static void ccactivate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
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

  GridMsg gridMsg = (GridMsg)malloc(sizeof(struct gridMsg_t));
  gridMsg->grid = grid;
  gridMsg->mhp = (MessageHandler*)(user_data);

  // Pack the container in the window
  gtk_container_add (GTK_CONTAINER (window), grid);

  for (int i = 0; i < info.GetRows(); i++) {
    for (int j = 0; j < info.GetCols(); j++) {
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
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(ccswap), (gpointer) gridMsg);
  gtk_grid_attach(GTK_GRID(grid), button, info.GetCols() + 1, 2, 5, 1);

  icon = gtk_image_new_from_file("images/direction/left.png");
  button = gtk_button_new();
  int *leftbits = (int*) malloc(sizeof(int));
  *leftbits = 0;
  g_object_set_data(G_OBJECT(button), "bits", leftbits);
  gtk_button_set_image(GTK_BUTTON(button), icon);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(ccswap), (gpointer) gridMsg);
  gtk_grid_attach(GTK_GRID(grid), button, info.GetCols() + 1, 3, 5, 1);

  icon = gtk_image_new_from_file("images/direction/right.png");
  button = gtk_button_new();
  int *rightbits = (int*) malloc(sizeof(int));
  *rightbits = 1;
  g_object_set_data(G_OBJECT(button), "bits", rightbits);
  gtk_button_set_image(GTK_BUTTON(button), icon);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(ccswap), (gpointer) gridMsg);
  gtk_grid_attach(GTK_GRID(grid), button, info.GetCols() + 1, 4, 5, 1);

  icon = gtk_image_new_from_file("images/direction/down.png");
  button = gtk_button_new();
  int *downbits = (int*) malloc(sizeof(int));
  *downbits = 2;
  g_object_set_data(G_OBJECT(button), "bits", downbits);
  gtk_button_set_image(GTK_BUTTON(button), icon);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(ccswap), (gpointer) gridMsg);
  gtk_grid_attach(GTK_GRID(grid), button, info.GetCols() + 1, 5, 5, 1);

  moves = gtk_label_new(NULL);
  gtk_grid_attach(GTK_GRID(grid), moves, info.GetCols() + 1, 0, 5, 1);
  writeMoves();

  score = gtk_label_new(NULL);
  gtk_grid_attach(GTK_GRID(grid), score, info.GetCols() + 1, 1, 5, 1);
  writeScore();

  gtk_widget_show_all (window); 
}

static void ccopen(GtkApplication *app, GFile **files, gint n_files, const gchar *hint, gpointer user_data) {
  json_t *root = json_object();
  json_error_t error; 
  json_t *instance;
  instance = json_load_file(g_file_get_path(files[0]), 0, &error);
  json_t *action = json_string("helloack");
  json_object_set_new(root, "action", action);
  json_object_set_new(root, "gameinstance", instance);
  const char *gameinstance = json_dumps(root, 0);
  info.Init(gameinstance);
  string helloack(gameinstance);
  free((char*)gameinstance);
  HelloackMessage msg(helloack);
  MessageHandler *mhp = (MessageHandler *) user_data;
  mhp->SendMessage(msg);
  g_application_activate(G_APPLICATION(app));
  // json_decref(jrows);
  // json_decref(jcols);
  // json_decref(bCandies);
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
