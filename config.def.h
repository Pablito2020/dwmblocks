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
	{ "TIME: ", 0, "date '+%H:%M'", 60, 0 },
	{ "", CMD_TIME, 0, 5, 0 }
};

/* Sets delimeter between status commands */
static char delim[] = "  ";
