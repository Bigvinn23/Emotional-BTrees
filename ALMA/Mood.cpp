#include"Mood.h"
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "MoodType.h"

Mood::Mood() {}
Mood::Mood(double Pleasure, double Arousal, double Dominance) {
	if (pleasure < -1.0 || pleasure > 1.0)
		//throw("Invalid degree of pleasure: " + Pleasure);
		if (arousal < -1.0 || arousal > 1.0)
			//throw("Invalid degree of arousal: " + Arousal);
			if (dominance < -1.0 || dominance > 1.0)
				//	throw ("Invalid degree of dominance: " + Dominance);
				pleasure = pleasure;
	arousal = arousal;
	dominance = dominance;
}


double Mood::getPleasure() {
	return pleasure;
}

double Mood::getArousal() {
	return arousal;
}

double Mood::getDominance() {
	return dominance;
}

bool Mood::isPleasureDominant() {
	if ((abs(pleasure) > abs(arousal)) && (abs(pleasure) > abs(dominance))) {
		return true;
	}
	else {
		return false;
	}
}

bool Mood::isArousalDominant() {
	if ((abs(arousal) > abs(pleasure)) && (abs(arousal) > abs(dominance))) {
		return true;
	}
	else {
		return false;
	}
}

bool Mood::isDominanceDominant() {
	if ((abs(dominance) > abs(pleasure)) && (abs(dominance) > abs(arousal))) {
		return true;
	}
	else {
		return false;
	}
}

std::string Mood::getMoodWord() {
	if ((pleasure == 0.0) && (arousal == 0.0) && (dominance == 0.0))
		return MoodIntensityNames[Moodtype::Neutral];
	int index = 0;
	index = (pleasure < 0.0) ? 4 : index;
	index = (arousal < 0.0) ? index + 2 : index;
	index = (dominance < 0.0) ? index + 1 : index;
	index++;
	return MoodIntensityNames[index];
}

double Mood::getIntensity() {
	return sqrt((pleasure*pleasure) + (arousal*arousal) + (dominance*dominance));
}


std::string Mood::getMoodWordIntensity() {
	double normOfMood = sqrt((pleasure*pleasure) + (arousal*arousal) + (dominance*dominance));
	if (normOfMood == 0.0)
		return "neutral";
	int index = 0;
	index = (normOfMood > 0.50) ? 1 : index;
	index = (normOfMood > 1.00) ? 2 : index;
	return MoodIntensityNames[index + 1];
}

bool Mood::equals(Mood m) {
	return ((pleasure == m.getPleasure()) &&
		(arousal == m.getArousal()) &&
		(dominance == m.getDominance()));
}

std::string Mood::toString() {
	return getMoodWordIntensity() + " " + getMoodWord() +
		" (P: " + std::to_string(pleasure) +
		", A: " + std::to_string(arousal) +
		", D: " + std::to_string(dominance) + ")";
}

std::string Mood::padString() {
	return
		" (P: " + std::to_string(pleasure) +
		", A: " + std::to_string(arousal) +
		", D: " + std::to_string(dominance) + ")";
}