#ifndef _timer_h
#define _timer_h

#include <glib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <libnotify/notify.h>
#include <gtk/gtk.h>

typedef struct Timer {
	gboolean running;
    gboolean paused;
	struct tm *startLocalTime;
	struct tm *stopLocalTime;
	time_t startTime;
	time_t endTime;
	gchar elapsedTime[256];
    gchar subject[256];
	guint timeoutIdentifier;
    int elapsedSeconds;
}*TimerP;

typedef struct TimerData {
    TimerP timerPointer;
    GtkWidget *grid;
    GtkWidget *window;
    GtkWidget *startButton;
    GtkWidget *stopButton;
	GtkWidget *lapButton;
    GtkWidget *pauseButton;
} *TimerDataP;

void debug(TimerP);
void initTimer(TimerP);
void pauseTimer(TimerP);
void resumeTimer(TimerP);

gchar *getCurrentTime(TimerP);
void setElapsedTime(int, TimerP);
void loadCurrentTime(TimerP);
void getElapsedTime(TimerP);
const gchar *getTime();
gboolean alertUser(TimerP);
gboolean startTimer(TimerP);
gboolean stopTimer(TimerP);

#endif
