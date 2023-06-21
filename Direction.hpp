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

		case Direction::N:
			direction = Direction::E;
			break;

		case Direction::E:
			direction = Direction::S;
			break;

		case Direction::S:
			direction = Direction::W;
			break;

		case Direction::W:
			direction = Direction::N;
			break;
	}
	return returnValue;
}

inline Direction operator--(Direction& direction, int) {
	Direction returnValue = direction;
	switch (direction) {

	case Direction::N:
		direction = Direction::W;
		break;

	case Direction::E:
		direction = Direction::N;
		break;

	case Direction::S:
		direction = Direction::E;
		break;

	case Direction::W:
		direction = Direction::S;
		break;
	}
	return returnValue;
}