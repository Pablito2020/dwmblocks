// Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
	/*Icon*/	/*Command*/		/*Update Interval*/	/*Update Signal*/
	{ "", "kb", 0, 1 },
	{ "M: ", "mailchk -l", 0, 2 },
	{ "♪: ", "amixer get Master | grep -o \"\\(\\[off\\]\\|[0-9]*%\\)\"", 0, 10 },
	{"", "cut -d ' ' -f1-3 /proc/loadavg", 10, 0},
	{ "", "free -h | awk '/^Mem/ { print $3\"/\"$2 }' | sed s/i//g", 30, 0 },
	{ "", "date '+%a %b %d %H:%M:%S'", 60, 0 },
};

// Sets delimeter between status commands.
static char delim[] = "   ";
