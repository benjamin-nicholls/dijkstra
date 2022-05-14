#include "bots.h"
#include "stdlib.h"

cBotSimple2::cBotSimple2() {}
cBotSimple2::~cBotSimple2() {}

// Always move closer to the target in terms of X and Y.
void cBotSimple2::ChooseNextGridPosition() {
	int newX = PositionX();
	int newY = PositionY();

	if (PositionX() < gTarget.PositionX()) {
		newX += 1;
	}
	else if (PositionX() > gTarget.PositionX()) {
		newX -= 1;
	}

	if (PositionY() < gTarget.PositionY()) {
		newY += 1;
	}
	else if (PositionY() > gTarget.PositionY()) {
		newY -= 1;
	}

	// NOT FULLY IMPLEMENTED

	int deltaX = newX - PositionX();
	int deltaY = newY - PositionY();
	int correctedX = PositionX();
	int correctedY = PositionY();

	int random = rand() % 2;
	if (random == 0) { random = -1; }

	if (deltaX < 0) {  // object is moving left
		correctedY += random;
	} else if (deltaX > 0) { //object is moving right
		correctedY += random;
	} //else {  // no lateral movement

	if (deltaY < 0) {
		//object is moving up
	} else if (deltaX > 0) {
		//object is moving down
	} else {
		// no vertical movement
	}



	// If the intended move fails, try to move in a different direction.
	if (!SetNext(newX, newY, gLevel)) {
		int correctedX = PositionX();
		int correctedY = PositionY();
		
		int random = rand() % 2;
		if (random == 0) { random = -1; }
		
		if (newX != PositionX()) {
			correctedY = PositionY() + random;
		}
		if (newY != PositionY()) {
			correctedX = PositionX() + random;
		}

		// If the new intended move fails, don't move for this cycle.
		if (!SetNext(correctedX, correctedY, gLevel)) {
			SetNext(PositionX(), PositionY(), gLevel);
		}
	}

}
