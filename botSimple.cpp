#include "bots.h"

cBotSimple::cBotSimple() {}
cBotSimple::~cBotSimple() {}

// Always move closer to the target in terms of X and Y.
void cBotSimple::ChooseNextGridPosition() {
	int newX = PositionX();
	int newY = PositionY();

	if (PositionX() < gTarget.PositionX()) {
		newX += 1;
	} else if (PositionX() > gTarget.PositionX()) {
		newX -= 1;
	}

	if (PositionY() < gTarget.PositionY()) {
		newY += 1;
	} else if (PositionY() > gTarget.PositionY()) {
		newY -= 1;
	}

	// If the intended move fails, don't move for this cycle.
	if (!SetNext(newX, newY, gLevel)) {
		SetNext(PositionX(), PositionY(), gLevel);
	}

}
