#pragma once

#include"../Mood/Mood.h"
#include"Personality.h"

class PersonalityMoodRelations {
public:
	static std::shared_ptr<PersonalityMoodRelations> getInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<PersonalityMoodRelations>();

		return instance;
	}
private:
	 static std::shared_ptr<PersonalityMoodRelations> instance ;
	 static Mood conscientiousnessRelation ;
	 static Mood opennessRelation;
	 static Mood extraversionRelation;
	 static Mood agreeablenessRelation;
	 static Mood neurotismRelation;
	 static Personality pleasureRelation;
	 static Personality arousalRelation;
	 static Personality dominanceRelation;
	 // static Logger log = AffectManager.sLog;


protected:


public:

	PersonalityMoodRelations() {
		// ... to defeat instanciation
	}

	 PersonalityMoodRelations(Mood OpennessRelation,Mood ConscientiousnessRelation,
		Mood ExtraversionRelation,Mood AgreeablenessRelation,
		Mood NeurotismRelation,
		 Personality PleasureRelation, Personality ArousalRelation,
		 Personality DominanceRelation) {
		opennessRelation = OpennessRelation;
		conscientiousnessRelation = ConscientiousnessRelation;
		extraversionRelation = ExtraversionRelation;
		agreeablenessRelation = AgreeablenessRelation;
		neurotismRelation = NeurotismRelation;
		pleasureRelation = PleasureRelation;
		arousalRelation = ArousalRelation;
		dominanceRelation = DominanceRelation;
	}
	static PersonalityMoodRelations*
		createPersonalityMoodRelations(Mood opennessRelation,Mood conscientiousnessRelation,
			Mood extraversionRelation,Mood agreeablenessRelation,
			Mood neurotismRelation,
		Personality pleasureRelation, Personality arousalRelation,
			Personality dominanceRelation) {
		if (instance == nullptr) {
			return new PersonalityMoodRelations(opennessRelation,
				conscientiousnessRelation,
				extraversionRelation,
				agreeablenessRelation,
				neurotismRelation,
				pleasureRelation,
				arousalRelation,
				dominanceRelation);
		}
		 
	}

	/**
	* Computes a character's default mood according to it's personality trait
	*
	* @param personality the character's personality
	* @throws NullPointerException if the personality or the
	*                              PersonalityMoodRelations is null.
	* @return Mood object
	*/
	 static Mood getDefaultMood(Personality personality) {
		if ((instance == nullptr) && (personality == nullptr)) {
			throw std::invalid_argument("PersonalityMoodRelations or personality argument null!");
			return Mood();
		}
		else {
			double pleasure =
				pleasureRelation.getOpenness()*personality.getOpenness() +
				pleasureRelation.getConscientiousness()*personality.getConscientiousness() +
				pleasureRelation.getExtraversion()*personality.getExtraversion() +
				pleasureRelation.getAgreeableness()*personality.getAgreeableness() +
				pleasureRelation.getNeurotism()*personality.getNeurotism();
			pleasure = (pleasure > 1.0) ? 1.0 : pleasure;
			pleasure = (pleasure < -1.0) ? -1.0 : pleasure;
			double arousal =
				arousalRelation.getOpenness()*personality.getOpenness() +
				arousalRelation.getConscientiousness()*personality.getConscientiousness() +
				arousalRelation.getExtraversion()*personality.getExtraversion() +

				arousalRelation.getAgreeableness()*personality.getAgreeableness() +
				arousalRelation.getNeurotism()*personality.getNeurotism();
			arousal = (arousal > 1.0) ? 1.0 : arousal;
			arousal = (arousal < -1.0) ? -1.0 : arousal;
			double dominance =
				dominanceRelation.getOpenness()*personality.getOpenness() +
				dominanceRelation.getConscientiousness()*personality.getConscientiousness() +
				dominanceRelation.getExtraversion()*personality.getExtraversion() +
				dominanceRelation.getAgreeableness()*personality.getAgreeableness() +
				dominanceRelation.getNeurotism()*personality.getNeurotism();
			dominance = (dominance > 1.0) ? 1.0 : dominance;
			dominance = (dominance < -1.0) ? -1.0 : dominance;
			// Get the average value of each mood dimension.
			return  Mood(pleasure, arousal, dominance);
		}
	}

	/**
	* Computes a character's personality traits according to a given mood
	*
	* @param mood the character's mood
	* @throws NullPointerException if the personality or the
	*                              PersonalityMoodRelations is null.
	* @return Personality object
	*/
	 Personality getMoodyPersonalityTraits(Mood mood) {
		if ((instance == nullptr) && (mood == nullptr)) {
			throw std::invalid_argument("PersonalityMoodRelations or mood argument null!");
			
		}
		else {
			double pleasure =mood.getPleasure();
			double arousal =mood.getArousal();
			double dominance =mood.getDominance();
			double openness = 0.0;
			double conscientiousness = 0.0;
			double extraversion = 0.0;
			double agreeableness = 0.0;
			double neurotism = 0.0;
			if (mood.isPleasureDominant()) {
				// computed (temporal) personality traits related to pleasure
				openness = (opennessRelation.isPleasureDominant()) ?
					opennessRelation.getPleasure()*pleasure +
					opennessRelation.getArousal()*arousal +
					opennessRelation.getDominance()*dominance : 0.0;
				conscientiousness = (conscientiousnessRelation.isPleasureDominant()) ?
					conscientiousnessRelation.getPleasure()*pleasure +
					conscientiousnessRelation.getArousal()*arousal +
					conscientiousnessRelation.getDominance()*dominance : 0.0;
				extraversion = (extraversionRelation.isPleasureDominant()) ?
					extraversionRelation.getPleasure()*pleasure +
					extraversionRelation.getArousal()*arousal +
					extraversionRelation.getDominance()*dominance : 0.0;
				agreeableness = (agreeablenessRelation.isPleasureDominant()) ?
					agreeablenessRelation.getPleasure()*pleasure +
					agreeablenessRelation.getArousal()*arousal +
					agreeablenessRelation.getDominance()*dominance : 0.0;
				/*neurotism = (neurotismRelation->isPleasureDominant()) ?
					neurotismRelation->getPleasure()*pleasure +
					neurotismRelation->getArousal()*arousal +
					neurotismRelation->getDominance()*dominance : 0.0;*/
			}
			if (mood.isArousalDominant()) {
				// compute (temporal) personality traits related to arousal
				openness = (opennessRelation.isArousalDominant()) ?
					opennessRelation.getPleasure()*pleasure +
					opennessRelation.getArousal()*arousal +
					opennessRelation.getDominance()*dominance : 0.0;
				conscientiousness = (conscientiousnessRelation.isArousalDominant()) ?
					conscientiousnessRelation.getPleasure()*pleasure +
					conscientiousnessRelation.getArousal()*arousal +
					conscientiousnessRelation.getDominance()*dominance : 0.0;
				extraversion = (extraversionRelation.isArousalDominant()) ?
					extraversionRelation.getPleasure()*pleasure +
					extraversionRelation.getArousal()*arousal +
					extraversionRelation.getDominance()*dominance : 0.0;
				agreeableness = (agreeablenessRelation.isArousalDominant()) ?
					agreeablenessRelation.getPleasure()*pleasure +
					agreeablenessRelation.getArousal()*arousal +
					agreeablenessRelation.getDominance()*dominance : 0.0;
			/*	neurotism = (neurotismRelation->isArousalDominant()) ?
					neurotismRelation->getPleasure()*pleasure +
					neurotismRelation->getArousal()*arousal +
					neurotismRelation->getDominance()*dominance : 0.0;*/
			}
			if (mood.isDominanceDominant()) {
				// compute (temporal) personality traits related to dominance
				openness = (opennessRelation.isDominanceDominant()) ?
					opennessRelation.getPleasure()*pleasure +
					opennessRelation.getArousal()*arousal +
					opennessRelation.getDominance()*dominance : 0.0;
				conscientiousness = (conscientiousnessRelation.isDominanceDominant()) ?
					conscientiousnessRelation.getPleasure()*pleasure +
					conscientiousnessRelation.getArousal()*arousal +
					conscientiousnessRelation.getDominance()*dominance : 0.0;
				extraversion = (extraversionRelation.isDominanceDominant()) ?
					extraversionRelation.getPleasure()*pleasure +
					extraversionRelation.getArousal()*arousal +
					extraversionRelation.getDominance()*dominance : 0.0;
				agreeableness = (agreeablenessRelation.isDominanceDominant()) ?
					agreeablenessRelation.getPleasure()*pleasure +
					agreeablenessRelation.getArousal()*arousal +
					agreeablenessRelation.getDominance()*dominance : 0.0;
				/*neurotism = (neurotismRelation->isDominanceDominant()) ?
					neurotismRelation->getPleasure()*pleasure +
					neurotismRelation->getArousal()*arousal +
					neurotismRelation->getDominance()*dominance : 0.0;*/
			}
			// just for being on the safe side ...
			openness = (openness > 1.0) ? 1.0 : openness;
			conscientiousness = (conscientiousness > 1.0) ? 1.0 : conscientiousness;
			extraversion = (extraversion > 1.0) ? 1.0 : extraversion;
			agreeableness = (agreeableness > 1.0) ? 1.0 : agreeableness;
			neurotism = (neurotism > 1.0) ? 1.0 : neurotism;
			// just for being on the safe side ...
			openness = (openness < -1.0) ? -1.0 : openness;
			conscientiousness = (conscientiousness < -1.0) ? -1.0 : conscientiousness;
			extraversion = (extraversion < -1.0) ? -1.0 : extraversion;
			agreeableness = (agreeableness < -1.0) ? -1.0 : agreeableness;
			neurotism = (neurotism < -1.0) ? -1.0 : neurotism;
			Personality p = Personality(openness, conscientiousness, extraversion, agreeableness, neurotism);
			return p;
		}
	}
};

 std::shared_ptr<PersonalityMoodRelations> PersonalityMoodRelations::instance = nullptr;
 Mood PersonalityMoodRelations::conscientiousnessRelation = Mood();
 Mood PersonalityMoodRelations::opennessRelation = Mood();
 Mood PersonalityMoodRelations::extraversionRelation = Mood();
 Mood PersonalityMoodRelations::agreeablenessRelation = Mood();
 Mood PersonalityMoodRelations::neurotismRelation = Mood();
Personality PersonalityMoodRelations::pleasureRelation = Personality();
Personality PersonalityMoodRelations::arousalRelation =  Personality();
Personality PersonalityMoodRelations::dominanceRelation =Personality();