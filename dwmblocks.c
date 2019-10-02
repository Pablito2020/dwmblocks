#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <X11/Xlib.h>

#define MAX_BLOCK 50
#define MAX_DELIM 5
#define MAX_ICO 10
#define LENGTH(X) (sizeof(X) / sizeof (X[0]))

#define ERR_DELIM 1

typedef struct {
	char* icon;
	unsigned int command_native;
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

#ifndef LOCALE
#define LOCALE "C"
#endif

#ifndef TIME_FORMAT
#define TIME_FORMAT "%c"
#endif

static char statusbar[LENGTH(blocks)][MAX_BLOCK] = { 0 };
static int statusContinue = 1;

int
block_time(char *output, unsigned int len)
{
	time_t now = time(0);
	unsigned int max_len = MAX_BLOCK - len - sizeof(delim) + 1;

	if(!strftime(output + len, max_len, TIME_FORMAT, localtime(&now))) {
		output[max_len+len] = '\0';
	}

	return 0;
}

int
block_load(char *output, unsigned int len)
{
	double avgs[3];
	char buf[15] = { 0 };

	if (getloadavg(avgs, 3) < 0)
		return 1;

	snprintf(buf, 15, "%.2f %.2f %.2f", avgs[0], avgs[1], avgs[2]);

	strcat(output, buf);

	return 0;
}

int
getico(const Block *block, char *output)
{
	int len = strlen(block->icon);

	/* truncate icon */
	if (len > MAX_ICO)
		len = MAX_ICO;

	memmove(output, block->icon, len);
	output[len] = '\0';

	return len;
}

int
external_cmd(const Block *block, char *output, unsigned int len) {
	char *buf;
	FILE *cmdf = popen(block->command, "r");

	if (!cmdf)
		return 1;

	int x = MAX_BLOCK - len - sizeof(delim) + 1;

	buf = (char*) calloc(1, x);
	fgets(buf, x, cmdf);
	int lenbuf = strlen(buf);

	if (lenbuf && (buf[lenbuf - 1] == '\n'))
		buf[lenbuf - 1] = 0;

	strcat(output, buf);

	free(buf);
	pclose(cmdf);

	return lenbuf ? 0 : 1;
}

/* opens process *cmd and stores output in *output */
void
getcmd(const Block *block, char *output)
{
	int hide = 0;

	unsigned int len = getico(block, output);

	if (block->command_native == CMD_TIME)
		hide = block_time(output, len);
	else if (block->command_native == CMD_LOAD)
		hide = block_load(output, len);
	else
		hide = external_cmd(block, output, len);

	if (hide)
		output[0] = '\0';
	else
		strcat(output, delim);
}

void
getcmds(int time)
{
	const Block* current;

	for (int i = 0; i < LENGTH(blocks); i++) {
		current = blocks + i;
		if ((current->interval != 0 && time % current->interval == 0) || time == -1)
			getcmd(current, statusbar[i]);
	}
}

void
getsigcmds(int signal)
{
	const Block *current;
	for (int i = 0; i < LENGTH(blocks); i++) {
		current = blocks + i;
		if (current->signal == signal)
			getcmd(current, statusbar[i]);
	}
}

void
setupsignals()
{
	for (int i = 0; i < LENGTH(blocks); i++) {
		if (blocks[i].signal > 0)
			signal(SIGRTMIN + blocks[i].signal, sighandler);
	}
}

void
getstatus(char *str)
{
	int j = 0;

	for (int i = 0; i < LENGTH(blocks); j += strlen(statusbar[i++])) {
		strcpy(str + j, statusbar[i]);
	}

	/* Remove last delimiter */
	str[strlen(str) - sizeof(delim) + 1] = '\0';
}

void
setroot()
{
	char *statusstr;

	Display *dpy = XOpenDisplay(NULL);
	int screen = DefaultScreen(dpy);
	Window root = RootWindow(dpy, screen);

	statusstr = (char*) malloc(LENGTH(blocks) * MAX_BLOCK);
	getstatus(statusstr);
	XStoreName(dpy, root, statusstr);
	free(statusstr);
	XCloseDisplay(dpy);
}

void
statusloop()
{
	int i = 0;

	setupsignals();
	getcmds(-1);

	while(statusContinue) {
		getcmds(i);
		setroot();
		sleep(1.0);
		i++;
	}
}

void
sighandler(int signum)
{
	getsigcmds(signum - SIGRTMIN);
	setroot();
}

void
termhandler(int signum)
{
	statusContinue = 0;
	exit(EXIT_SUCCESS);
}

int
main(int argc, char** argv)
{
	if (sizeof(delim) > MAX_DELIM) {
		fprintf(stderr, "err: delim too long (max: %d).\n", MAX_DELIM - 1);
		return ERR_DELIM;
	}

	setlocale(LC_TIME, LOCALE);
	signal(SIGTERM, termhandler);
	signal(SIGINT, termhandler);
	statusloop();
}
