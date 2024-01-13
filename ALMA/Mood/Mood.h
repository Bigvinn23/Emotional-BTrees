#ifndef MOOD_H
#define MOOD_H
#include <iostream>
#include <string>
#include <memory>
#include <vector>
//#include "MoodType.h"
#include<ctime>

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Mood {

 private:
	 friend ostream& operator<<(ostream& out, const Mood& m)
	 {
		 return m.write(out);
	 }

	 friend istream& operator>>(istream& in, Mood& m)
	 {
		 return m.read(in);
	 }

	 enum MoodIntensity : size_t { neutral, slightly, moderate, fully };
	 bool empty = true;

	 enum Moodtype : size_t {
		 Neutral, Exuberant, Dependent, Relaxed, Docile, Hostile, Anxious, Disdainful, Bored
	 };
	 const std::vector<std::string> MoodtypeNames = {
		 "Neutral", "Exuberant", "Dependent", "Relaxed", "Docile", "Hostile", "Anxious", "Disdainful", "Bored"
	 };

	 const std::vector<std::string> MoodIntensityNames = {
		 "neutral", "slightly", "moderate", "fully"
	 };
	 
	 double pleasure;

	 double arousal;
	 double dominance;

	  
	/**
	* Constructs a new <code>Mood</code> object with the
	* specified values for each mood dimension in the PAD mood space
	*
	* @param pleasure the degree of pleasure
	* @param arousal the degree of arousal
	* @param dominance the degree of dominance
	*
	* @throws IllegalArgumentException if the degree is not a
	*         real number in the interval <code>[-1.0,1.0]</code>.
	*/
 public:
	 
	 Mood()
	 {
		 srand(time(NULL));

		 pleasure = (1 * rand() / RAND_MAX - 1);
		 arousal = (1 * rand() / RAND_MAX - 1);
		 dominance = (1 * rand() / RAND_MAX - 1);
	 }

	 Mood(const Mood &obj)
	 {
		 pleasure = obj.pleasure;
		 arousal = obj.arousal;
		 dominance = obj.dominance;
		 empty = false;
	 }


	 Mood(double Pleasure, double Arousal, double Dominance) {
	//	if (pleasure < -1.0 || pleasure > 1.0)
			//throw("Invalid degree of pleasure: " + Pleasure);
//if (arousal < -1.0 || arousal > 1.0)
			//throw("Invalid degree of arousal: " + Arousal);
	//	if (dominance < -1.0 || dominance > 1.0)
		//	throw ("Invalid degree of dominance: " + Dominance);
		pleasure = Pleasure;
		arousal = Arousal;
		dominance = Dominance;
		empty = false;
	}


	 ostream& write(ostream& out) const
	 {
		 out << empty << ","
			 << pleasure << ","
			 << arousal << ","
			 << dominance << "\n";
		 return out;
	 }

	 istream& read(istream& in)
	 {
		 string s;
		 getline(in, s, ',');
		 empty = std::stoi(s);
		 getline(in, s, ',');
		 pleasure = stod(s);
		 getline(in, s, ',');
		 arousal = stod(s);
		 getline(in, s);
		 dominance = stod(s);
		 in.ignore(numeric_limits<streamsize>::max(), '\n');
		 return in;
	 }

	 bool getEmpty() { return empty; }
	 double getPleasure() {
		return pleasure;
	}

	 double getArousal() {
		return arousal;
	}

	 double getDominance() {
		return dominance;
	}

	 bool isPleasureDominant() {
		if ((abs(pleasure) > abs(arousal)) && (abs(pleasure) > abs(dominance))) {
			return true;
		}
		else {
			return false;
		}
	}

	 bool isArousalDominant() {
		if ((abs(arousal) > abs(pleasure)) && (abs(arousal) > abs(dominance))) {
			return true;
		}
		else {
			return false;
		}
	}

	 bool isDominanceDominant() {
		if ((abs(dominance) > abs(pleasure)) && (abs(dominance) > abs(arousal))) {
			return true;
		}
		else {
			return false;
		}
	}

	 std::string getMoodWord() {
		if ((pleasure == 0.0) && (arousal == 0.0) && (dominance == 0.0))
			return MoodtypeNames[Moodtype::Neutral];
		int index = 0;
		index = (pleasure < 0.0) ? 4 : index;
		index = (arousal < 0.0) ? index + 2 : index;
		index = (dominance < 0.0) ? index + 1 : index;
		index++;
		return MoodtypeNames[index];
	}

	 double getIntensity() {
		return sqrt((pleasure*pleasure) + (arousal*arousal) + (dominance*dominance));
	}


	 std::string getMoodWordIntensity() {
		double normOfMood = sqrt((pleasure*pleasure) + (arousal*arousal) + (dominance*dominance));
		if (normOfMood == 0.0)
			return "neutral";
		int index = 0;
		index = (normOfMood > 0.50) ? 1 : index;
		index = (normOfMood > 1.00) ? 2 : index;
		return MoodIntensityNames[index + 1];
	}

	 bool equals(Mood m) {
		return ((pleasure == m.getPleasure()) &&
			(arousal == m.getArousal()) &&
			(dominance == m.getDominance()));
	}

	std::string toString() {
		return getMoodWordIntensity() + " " + getMoodWord() +
			"\n (P: " + std::to_string(pleasure) +
			", \nA: " + std::to_string(arousal) +
			", \nD: " + std::to_string(dominance) + ")";
	}

	std::string padString() {
		return
			" (P: " + std::to_string(pleasure) +
			", A: " + std::to_string(arousal) +
			", D: " + std::to_string(dominance) + ")";
	}
	inline explicit operator bool() const 
	{
		return !empty;
	}
	bool operator!() const
	{
		return empty;
	}

	bool operator == (const std::nullptr_t) const
	{
		return empty;
	}

	bool operator != (const std::nullptr_t) const
	{
		return !empty;
	}

	bool operator==(const Mood& rhs) const
	{
		return empty == rhs.empty
			&& arousal == rhs.arousal
			&& dominance == rhs.dominance
			&& pleasure == rhs.pleasure;

	}

	bool operator!=(const Mood& rhs) const
	{
		return empty != rhs.empty
			|| arousal != rhs.arousal
			|| dominance != rhs.dominance
			|| pleasure != rhs.pleasure;

	}

	Mood& operator=(const Mood& rhs)
	{
		if (this->empty = rhs.empty)
			return *this;

		this->arousal = rhs.arousal;
		this->dominance = rhs.dominance;
		this->pleasure = rhs.pleasure;
		return *this;
	}
};
#endif 