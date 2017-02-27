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

static void ccactivate(GtkApplication *app, gpointer user_data) {

}

static void ccopen(GtkApplication *app, GFile **files, gint n_files, const gchar *hint, gpointer user_data) {
  json_t *root;
  json_error_t error;
  root = json_load_file(g_file_get_path(files[0]), 0, &error);
  char *gameinstance = json_dumps(root, 0);
  // need to create {"action": "helloack", [rest of file]
  HelloackMessage msg(gameinstance);
  MessageHandler *mhp = (MessageHandler *)user_data;
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

      MessageHandler mh(acceptedFd);
      MessageHandler *mhp = &mh;
      Message msg = mh.GetNextMessage();

      if (msg.GetType() == "hello") {
        //int status = 0;
        GtkApplication *app;
        app = gtk_application_new("com.github.adityasaraf.CandyCrush", G_APPLICATION_HANDLES_OPEN);
        g_signal_connect(app, "open", G_CALLBACK(ccopen), mhp);
        g_signal_connect(app, "activate", G_CALLBACK(ccactivate), mhp);
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
