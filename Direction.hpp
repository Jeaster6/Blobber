#pragma once

enum Direction {
	N,
	E,
	S,
	W
};

inline Direction operator++(Direction& direction, int) {
	Direction returnValue = direction;
	switch (direction) {

		case N:
			direction = E;
			break;

		case E:
			direction = S;
			break;

		case S:
			direction = W;
			break;

		case W:
			direction = N;
			break;
	}
	return returnValue;
}

inline Direction operator--(Direction& direction, int) {
	Direction returnValue = direction;
	switch (direction) {

	case N:
		direction = W;
		break;

	case E:
		direction = N;
		break;

	case S:
		direction = E;
		break;

	case W:
		direction = S;
		break;
	}
	return returnValue;
}