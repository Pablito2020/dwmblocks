#define LOCALE "" /* Use empty string for default system locale  */
#define TIME_FORMAT "%a %b %d %H:%M" /* Used for CMD_TIME */

/* Native commands */
#define CMD_TIME 1

/*
 * You can use external or native commands for the blocks.
 * Set 0 as native command value if you want to use external command - and vice
 * versa.
 */

static const Block blocks[] = {
	/* icon / native command / external command / update interval / update signal */
	{ "", 0, "kb", 0, 1 },
	{ "M: ", 0, "mailchk -l", 0, 2 },
	{ "♪: ", 0, "amixer get Master | grep -o \"\\(\\[off\\]\\|[0-9]*%\\)\"", 0, 10 },
	{ "", 0, "cut -d ' ' -f1-3 /proc/loadavg", 10, 0 },
	{ "", 0, "free -h | awk '/^Mem/ { print $3\"/\"$2 }' | sed s/i//g", 30, 0 },
	{ "", CMD_TIME, 0, 60, 0 }
};

/* Sets delimeter between status commands */
static char delim[] = "  ";
