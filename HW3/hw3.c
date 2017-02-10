#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <jansson.h>

#include "hw2_modified.h"
#include "hw3Callbacks.h"

Array2D board;
GtkWidget *selected;
int moves;
GtkWidget* label;

int main(int argc, char *argv[]) {
  int status = 0;
	if (argc != 2)
	{
		printf("usage: %s filename\n", argv[0]);
    return EXIT_FAILURE;
	}
	else
	{
    moves = 0;
    GtkApplication *app;

	  app = gtk_application_new ("com.github.adityasaraf.CandyCrush", G_APPLICATION_HANDLES_OPEN);
	  g_signal_connect (app, "open", G_CALLBACK (open), NULL);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	  status = g_application_run (G_APPLICATION (app), argc, argv);
	  g_object_unref (app);
    g_object_unref (selected);
    g_object_unref (label);
    Array2D_destroy(board, (Array2DDataFreeFnPtr) &free);
	}

	return status;
}
