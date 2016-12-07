#ifndef TIMER_H
#define TIMER_H

#include <glib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <libnotify/notify.h>
#include <gtk/gtk.h>
#include <stdlib.h>

#define WINDOW_WIDTH 550
#define WINDOW_HEIGHT 200

/**
 * The "class" that has all the data for the timer itself and handles all
 * the user interaction
 *
 * @TODO Make this just deal with the timer itself and not anything to do
 * with the UI (shouldn't have subject and stuff)
 */
typedef struct Timer {
    gboolean running;
    gboolean paused;
    struct tm *startLocalTime;
    struct tm *stopLocalTime;
    time_t startTime;
    time_t endTime;
    gchar elapsedTime[256];
    gchar subject[256];
    // Used for notifying user that we are not running
    guint timeoutIdentifier;
    int elapsedSeconds;
}*TimerP;

// Public function prototypes
/**
 * Start the timer. Returns whether the starting was successful or not
 */
gboolean startTimer(TimerP);
/**
 * Stop the timer. Returns whether or not the stopping was successful
 */
gboolean stopTimer(TimerP);
/**
 * Returns the current time that has elapsed
 */
gchar *getElapsedTime(TimerP);
/**
 * Used for outputting debugging information to the console
 *
 * @TODO This should be able to be initiated in the calling parameters
 */
void debug(TimerP);
/**
 * Initial setup of the timer (constructor). Sets that we are not running
 * and also sets a notification
 */
void initTimer(TimerP);
/**
 * Pause the timer so it is no longer running but able to be easily resumed
 */
void pauseTimer(TimerP);
/**
 * Resume the timer after it has been paused
 */
void resumeTimer(TimerP);

#endif
