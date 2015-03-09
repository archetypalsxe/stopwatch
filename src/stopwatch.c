#include <stdio.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <time.h>
#include <cairo.h>
#include "Timer.h"


static void updateStartTime(gchar time[256], GtkWidget *grid, GtkWidget *window, const gchar *text) {
	GtkWidget *windowTime;
	GtkWidget *windowAction;
	GtkWidget *windowElapsed;

	windowTime = gtk_label_new(time);
	windowAction = gtk_label_new("Timer Started");
	strlen(text);
	if(strlen(text) == 0) {
		text = "Not Entered";
	}
	windowElapsed = gtk_label_new(text);
	gtk_grid_remove_row(GTK_GRID(grid), 0);
	gtk_grid_insert_row(GTK_GRID(grid), 0);
	gtk_grid_attach(GTK_GRID(grid), windowTime, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), windowAction, 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), windowElapsed, 2, 0, 1, 1);

	gtk_widget_show(windowTime);
	gtk_widget_show(windowAction);
	gtk_widget_show(windowElapsed);
}

static void displayWorkingRequest (gchar time[256], GtkWidget *grid, GtkWidget *window) {
	GtkWidget *dialog;
	GtkWidget *textEntry;
	GtkWidget *label;

	label = gtk_label_new("What Are You Working On?");

	GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
	dialog = gtk_dialog_new_with_buttons(
		"Starting Timer",
		GTK_WINDOW(window),
		flags,
		NULL
	);

	textEntry = gtk_entry_new();
	gtk_entry_set_activates_default(GTK_ENTRY(textEntry), TRUE);
	gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_ACCEPT);

	gtk_box_pack_start(
		GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		label,
		FALSE,
		FALSE,
		0
	);
	gtk_dialog_add_action_widget(GTK_DIALOG(dialog), textEntry, 1);

	gtk_widget_show_all(dialog);
	gtk_dialog_run(GTK_DIALOG(dialog));

	updateStartTime(time, grid, window, gtk_entry_get_text(GTK_ENTRY(textEntry)));
	gtk_widget_destroy(dialog);
}

static void startTimerPressed (GtkWidget *widget, gpointer data, void *params[5]) {
	TimerP timer = params[0];

	gboolean success = startTimer(timer);
	if(success) {
		gchar buffer[256];
		GtkWidget *windowTime;
		GtkWidget *windowAction;
		GtkWidget *windowElapsed;
		GtkWidget *grid = params[1];
		GtkWidget *window = params[2];
		GtkWidget *startButton = params[3];
		GtkWidget *stopButton = params[4];

		strftime(buffer, 256, "%I:%M:%S%P", timer->startLocalTime);
		windowTime = gtk_label_new(buffer);
		windowAction = gtk_label_new("Timer Started");
		windowElapsed = gtk_label_new("--");
		gtk_grid_remove_row(GTK_GRID(grid), 3);
		gtk_grid_insert_row(GTK_GRID(grid), 0);
		gtk_grid_attach(GTK_GRID(grid), windowTime, 0, 0, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), windowAction, 1, 0, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), windowElapsed, 2, 0, 1, 1);
		gtk_widget_hide(startButton);
		gtk_widget_show(stopButton);
		gtk_widget_show(windowTime);
		gtk_widget_show(windowAction);
		gtk_widget_show(windowElapsed);
		displayWorkingRequest(buffer, grid, window);
	}
}

static void stopTimerPressed (GtkWidget *widget, gpointer data, void *params[3]) {
	TimerP timer = params[0];
	GtkWidget *grid = params[1];

	gboolean success = stopTimer(timer);
	if(success) {
		getElapsedTime(timer);
		gchar buffer[256];
		GtkWidget *windowTime;
		GtkWidget *windowAction;
		GtkWidget *windowElapsed;
		GtkWidget *grid = params[1];
		GtkWidget *window = params[2];
		GtkWidget *startButton = params[3];
		GtkWidget *stopButton = params[4];

		strftime(buffer, 256, "%I:%M:%S%P", timer->stopLocalTime);
		windowTime = gtk_label_new(buffer);
		windowAction = gtk_label_new("Timer Stopped");
		windowElapsed = gtk_label_new(timer->elapsedTime);
		gtk_grid_remove_row(GTK_GRID(grid), 3);
		gtk_grid_insert_row(GTK_GRID(grid), 0);
		gtk_grid_attach(GTK_GRID(grid), windowTime, 0, 0, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), windowAction, 1, 0, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), windowElapsed, 2, 0, 1, 1);
		gtk_widget_hide(stopButton);
		gtk_widget_show(startButton);
		gtk_widget_show(windowTime);
		gtk_widget_show(windowAction);
		gtk_widget_show(windowElapsed);
	}
}

int main (int argc, char *argv[]) {
	GtkWidget *window;
	GtkWidget *grid;
	GtkWidget *startButton;
	GtkWidget *stopButton;
	GtkWidget *windowTime;
	GtkWidget *windowAction;
	GtkWidget *windowElapsed;
	struct Timer timer;

	/* This is called in all GTK applications. Arguments are parsed
	* from the command line and are returned to the application.
	*/
	gtk_init (&argc, &argv);
	initTimer(&timer);

	/* create a new window, and set its title */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), 550, 200);
	gtk_window_set_title (GTK_WINDOW (window), "Stopwatch");
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);

	/* Here we construct the container that is going pack our buttons */
	grid = gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);

	/* Pack the container in the window */
	gtk_container_add (GTK_CONTAINER (window), grid);

	startButton = gtk_button_new_with_label ("Start");
	stopButton = gtk_button_new_with_label ("Stop");

	void *data[5];
	data[0] = &timer;
	data[1] = grid;
	data[2] = window;
	data[3] = startButton;
	data[4] = stopButton;
	g_signal_connect (startButton, "clicked", G_CALLBACK (startTimerPressed), data);
	g_signal_connect (stopButton, "clicked", G_CALLBACK (stopTimerPressed), data);

	gtk_grid_attach (GTK_GRID (grid), startButton, 0, 4, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), stopButton, 0, 4, 1, 1);

	gtk_widget_show_all(window);
	gtk_widget_hide(stopButton);


	/* All GTK applications must have a gtk_main(). Control ends here
	* and waits for an event to occur (like a key press or a mouse event),
	* until gtk_main_quit() is called.
	*/
	gtk_main ();

	return 0;
}
