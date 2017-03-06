#include "gpsParser.h"
#include "debug.h"

int main() {
	while (1) {
		msg_t c = getch();
		gpsStepParser(c);
	}
	return 0;
}