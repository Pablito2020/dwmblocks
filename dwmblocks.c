#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <X11/Xlib.h>

#define MAX_BLOCK 50
#define MAX_DELIM 4
#define MAX_ICO 10
#define LENGTH(X) (sizeof(X) / sizeof (X[0]))

typedef struct {
	char* icon;
	char* command;
	unsigned int interval;
	unsigned int signal;
} Block;

void sighandler(int num);
void getcmds(int time);
void getsigcmds(int signal);
void setupsignals();
void getstatus(char *str);
void setroot();
void statusloop();
void sighandler(int signum);
void termhandler(int signum);

#include "config.h"

static Display *dpy;
static int screen;
static Window root;
static char statusbar[LENGTH(blocks)][MAX_BLOCK] = { 0 };
static int statusContinue = 1;

// opens process *cmd and stores output in *output
void getcmd(const Block *block, char *output)
{
	FILE *cmdf = popen(block->command, "r");

	if (!cmdf)
		return;

	int len_ico = strlen(block->icon);

	if (len_ico > MAX_ICO)
		len_ico = MAX_ICO;

	memmove(output, block->icon, len_ico);
	output[len_ico] = 0;

	int x = MAX_BLOCK - strlen(output) - strlen(delim);
	char *buf;

	buf = (char*) malloc(x);
	fgets(buf, x, cmdf);

	if (buf[strlen(buf) - 1] == '\n')
		buf[strlen(buf) - 1] = 0;

	if (!strlen(buf)) {
		output[0] = 0;
		return;
	}

	strcat(output, buf);
	free(buf);
	pclose(cmdf);

	strcat(output, delim);
}

void getcmds(int time)
{
	const Block* current;

	for (int i = 0; i < LENGTH(blocks); i++)
	{
		current = blocks + i;
		if ((current->interval != 0 && time % current->interval == 0) || time == -1)
			getcmd(current, statusbar[i]);
	}
}

void getsigcmds(int signal)
{
	const Block *current;
	for (int i = 0; i < LENGTH(blocks); i++)
	{
		current = blocks + i;
		if (current->signal == signal)
			getcmd(current, statusbar[i]);
	}
}

void setupsignals()
{
	for (int i = 0; i < LENGTH(blocks); i++)
	{
		if (blocks[i].signal > 0)
			signal(SIGRTMIN + blocks[i].signal, sighandler);
	}

}

void getstatus(char *str)
{
	int j = 0;

	for (int i = 0; i < LENGTH(blocks); j += strlen(statusbar[i++])) {
		strcpy(str + j, statusbar[i]);
	}

	// Remove last delimiter
	str[strlen(str) - strlen(delim)] = '\0';
}

void setroot()
{
	char *statusstr;

	Display *d = XOpenDisplay(NULL);

	if (d)
		dpy = d;

	screen = DefaultScreen(dpy);
	root = RootWindow(dpy, screen);

	statusstr = (char*) malloc(LENGTH(blocks) * MAX_BLOCK);
	getstatus(statusstr);
	XStoreName(dpy, root, statusstr);
	free(statusstr);
	XCloseDisplay(dpy);
}


void statusloop()
{
	int i = 0;

	setupsignals();
	getcmds(-1);

	while(statusContinue)
	{
		getcmds(i);
		setroot();
		sleep(1.0);
		i++;
	}
}

void sighandler(int signum)
{
	getsigcmds(signum - SIGRTMIN);
	setroot();
}

void termhandler(int signum)
{
	statusContinue = 0;
	exit(0);
}

int main(int argc, char** argv)
{
	if (strlen(delim) > MAX_DELIM - 1) {
		delim[MAX_DELIM] = 0;
	}

	signal(SIGTERM, termhandler);
	signal(SIGINT, termhandler);
	statusloop();
}
