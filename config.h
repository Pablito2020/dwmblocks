#define LOCALE "" /* Use empty string for default system locale  */
#define TIME_FORMAT "%I:%M" /* Used for CMD_TIME */

/* Native commands */
#define CMD_TIME 1
#define CMD_LOAD 2

/*
 * You can use external or native commands for the blocks.
 * Set 0 as native command value if you want to use external command - and vice
 * versa.
 */

static const Block blocks[] = {
	/* icon /   native command  /       external command                /update interval / update signal */
	{ "",          0,          "~/scripts/dwmblocks/kernel_version",             0,               1 },
	{ "",          0,          "~/scripts/dwmblocks/sensors",                    1,               0 },
	{ "",          0,          "~/scripts/dwmblocks/volume",                     0,              10 },
	{ "",          0,          "~/scripts/dwmblocks/keyboard_info",              0,              12 },
	{ " ",        CMD_TIME,            0,                                       60,              2 },
	{ "",          0,          "~/scripts/dwmblocks/date",                       0,               3 }

};

/* Sets delimeter between status commands */
static char delim[] = " | ";
