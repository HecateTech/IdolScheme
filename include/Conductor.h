#ifdef _WIN32
	#ifndef _IOSTREAM_
		#include <iostream>
	#endif //!_IOSTREAM_
#else // !_WIN32
	#ifndef _GLIBCXX_IOSTREAM
		#include <iostream>
	#endif //!_GLIBCXX_IOSTREAM
#endif //!_WIN32

#ifndef CONDUCTOR_TIME
	#include <chrono>
	#include <ctime>
	#include "ChronoTypes.h"
#endif

#ifdef _WIN32
	#ifndef _INC_MATH
		#include <math.h>
	#endif //!_INC_MATH
#else
	#ifndef _GLIBCXX_MATH_H
		#include <math.h>
	#endif //!_GLIBCXX_MATH_H
#endif //!_WIN32


/* @brief - Conductor
 * The conductor is a representation of a
 * metronome and tells what note you are currently
 * on based on the given parameters.
 */
class Conductor {
public:
	ChronoTimePoint startTime;
	ChronoTimePoint currTime;
	ChronoMs timeDiff;
	int _bpm;
	int _offsetInMs;
	double _lengthInS;
	double totalBeats;
	double offsetBeats;
	double currBeat;
	double beatSinceRefresh;
	float numBeatsSinceRefresh;

	float calcTotalBeats(double lengthInS, int bpm) {
		return (lengthInS / 60) * bpm;
	}

	Conductor() {}

	Conductor(int bpm, double lengthInS, int offsetInMs) {
		_bpm = bpm;
		_lengthInS = lengthInS;
		_offsetInMs = offsetInMs;
	}

	float calcCurrentBeat();
	float calcBeatsInMs(int bpm);
	void startTimer();
	void refreshMembers();
};
