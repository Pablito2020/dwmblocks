#define LOCALE "" /* Use empty string for default system locale  */
#define TIME_FORMAT "%a %b %d %H:%M" /* Used for CMD_TIME */

/* Native commands */
#define CMD_TIME 1
#define CMD_LOAD 2

/*
 * You can use external or native commands for the blocks.
 * Set 0 as native command value if you want to use external command - and vice
 * versa.
 */

static const Block blocks[] = {
	/* icon /   native command  /       external command        /update interval / update signal */
	{ " ",        0,          "uname -r",                              0,              10 },
	{ "🌡 ",         0,          "sensors | awk '/Core 0/{print $3}'",    5,              0 },
	{ "♪ ",         0,          "pamixer --get-volume-human",            0,              10 },
	{ "⌨ ",         0,          "get_keyboard",                          0,              12 },
	{ "",        CMD_TIME,            0,                                 60,             9 }
};

/* Sets delimeter between status commands */
static char delim[] = " | ";
