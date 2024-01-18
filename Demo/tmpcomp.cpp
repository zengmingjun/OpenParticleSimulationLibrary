#include<graphics.h>

unsigned PartUnit[] = {33,33};

void drawragn(unsigned x, unsigned y) {
	unsigned corx = x * PartUnit[0];
	unsigned cory = y * PartUnit[1];
	rectangle(corx, cory + PartUnit[1], corx + PartUnit[0], cory);
}
