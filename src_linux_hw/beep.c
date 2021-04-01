/*
 * Vgk.sys now lives in Linux userland
 * Long live the penguin march.
 * 
 * 
 * Original code from:
 * This code is copyright (C) Johnathan Nightingale, 2000.
 *
 * This code may distributed only under the terms of the GNU Public License 
 * which can be found at http://www.gnu.org/copyleft or in the file COPYING 
 * supplied with this code.
 *
 * This code is not distributed with warranties of any kind, including implied
 * warranties of merchantability or fitness for a particular use or ability to 
 * breed pandas in captivity, it just can't be done.
 *
 * Bug me, I like it:  http://johnath.com/  or johnath@johnath.com
 */

#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/kd.h>
#include <linux/input.h>

#ifndef CLOCK_TICK_RATE
#define CLOCK_TICK_RATE 1193180
#endif

#define VERSION_STRING "beep-1.3"
char * copyright =
    "Copyright (C) Johnathan Nightingale, 2002.  "
"Use and Distribution subject to GPL.  "
"For information: http://www.gnu.org/copyleft/.";

#define NO_STDIN_BEEP 0
#define LINE_STDIN_BEEP 1
#define CHAR_STDIN_BEEP 2

enum {
    BEEP_TYPE_CONSOLE,
    BEEP_TYPE_EVDEV
};

/* Momma taught me never to use globals, but we need something the signal 
   handlers can get at.*/
int console_fd = -1;
int console_type = BEEP_TYPE_CONSOLE;
char * console_device = NULL;

void do_beep(int freq) {
    int period = (freq != 0 ? (int)(CLOCK_TICK_RATE / freq) : freq);

    if (console_type == BEEP_TYPE_CONSOLE) {
        if (ioctl(console_fd, KIOCSOUND, period) < 0) {
            putchar('\a'); /* Output the only beep we can, in an effort to fall back on usefulness */
            perror("ioctl");
        }
    } else {
        /* BEEP_TYPE_EVDEV */
        struct input_event e;

        e.type = EV_SND;
        e.code = SND_TONE;
        e.value = freq;

        if (write(console_fd, & e, sizeof(struct input_event)) < 0) {
            putchar('\a'); /* See above */
            perror("write");
        }
    }
}

/* If we get interrupted, it would be nice to not leave the speaker beeping in
   perpetuity. */
void handle_signal(int signum) {

    if (console_device)
        free(console_device);

    switch (signum) {
    case SIGINT:
    case SIGTERM:
        if (console_fd >= 0) {
            /* Kill the sound, quit gracefully */
            do_beep(0);
            close(console_fd);
            exit(signum);
        } else {
            /* Just quit gracefully */
            exit(signum);
        }
    }
}

void delay(int delay) {
    usleep(1000 * delay);
}

void beep(float freq, int aDelay) {
    do_beep(freq);
    delay(aDelay);
    do_beep(0);
}

void play_beeps() {
    int i;

    /* try to snag the console */
    if (console_device)
        console_fd = open(console_device, O_WRONLY);
    else
    if ((console_fd = open("/dev/tty0", O_WRONLY)) == -1)
        console_fd = open("/dev/vc/0", O_WRONLY);

    if (console_fd == -1) {
        fprintf(stderr, "Could not open %s for writing\n",
            console_device != NULL ? console_device : "/dev/tty0 or /dev/vc/0");
        printf("\a"); /* Output the only beep we can, in an effort to fall back on usefulness */
        perror("open");
        exit(1);
    }

    if (ioctl(console_fd, EVIOCGSND(0)) != -1)
        console_type = BEEP_TYPE_EVDEV;
    else
        console_type = BEEP_TYPE_CONSOLE;

    /* Let the proprietery sequence commence! Beep-boop! */
    beep(494, 75);
    delay(75);
    beep(988, 75);
    delay(75);
    beep(740, 75);
    delay(75);
    beep(622, 75);
    delay(75);
    beep(988, 75);
    beep(740, 75);
    delay(150);
    beep(622, 225);
    delay(75);
    beep(523, 75);
    delay(75);
    beep(988, 75);
    delay(75);
    beep(784, 75);
    delay(75);
    beep(659, 75);
    delay(75);
    beep(988, 75);
    beep(784, 75);
    delay(150);
    beep(659, 225);
    delay(75);
    beep(494, 75);
    delay(75);
    beep(988, 75);
    delay(75);
    beep(740, 75);
    delay(75);
    beep(622, 75);
    delay(75);
    beep(988, 75);
    beep(740, 75);
    delay(150);
    beep(622, 225);
    delay(75);
    beep(622, 75);
    beep(659, 75);
    beep(698, 75);
    delay(75);
    beep(698, 75);
    beep(740, 75);
    beep(784, 75);
    delay(75);
    beep(784, 75);
    beep(831, 75);
    beep(880, 75);
    delay(75);
    beep(988, 150);

    close(console_fd);
}

int main(int argc, char ** argv) {
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    
    play_beeps();

    if (console_device)
        free(console_device);

    return EXIT_SUCCESS;
}