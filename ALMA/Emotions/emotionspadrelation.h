#pragma once
#include <unordered_map>
#include"Mood/Mood.h"
#include "Emotions/EmotionType.h"
#include"Emotions/Emotion.h"
#include <memory>
class EmotionsPADRelation {
	/** correlation between mood and emotions */
private:
	std::unordered_map<Emotion::Type, Mood> relations;
	//static HashMap<EmotionType, Mood> relations =  HashMap<EmotionType, Mood>(25);//fix hashmap
	static std::shared_ptr<EmotionsPADRelation> instance;// was set to nuill before 

protected:

	EmotionsPADRelation() {
		// ... defeat instanciation

		relations.emplace(EmotionType::Undefined, Mood(0.0, 0.0, 0.0));
		relations.emplace(EmotionType::Joy, Mood(0.4, 0.2, 0.1));
		relations.emplace(EmotionType::Distress, Mood(-0.4, -0.2, -0.5));
		relations.emplace(EmotionType::HappyFor, Mood(0.4, 0.2, 0.2));
		relations.emplace(EmotionType::Gloating, Mood(0.3, -0.3, -0.1));
		relations.emplace(EmotionType::Resentment, Mood(-0.2, -0.3, -0.2));
		relations.emplace(EmotionType::Pity, Mood(-0.4, -0.2, -0.5));
		relations.emplace(EmotionType::Hope, Mood(0.2, 0.2, -0.1));
		relations.emplace(EmotionType::Fear, Mood(-0.64, 0.60, -0.43));
		relations.emplace(EmotionType::Satisfaction, Mood(0.3, -0.2, 0.4));
		relations.emplace(EmotionType::Relief, Mood(0.2, -0.3, -0.4));
		relations.emplace(EmotionType::FearsConfirmed, Mood(-0.5, -0.3, -0.7));
		relations.emplace(EmotionType::Disappointment, Mood(-0.3, 0.1, -0.4));
		relations.emplace(EmotionType::Pride, Mood(0.4, 0.3, 0.3));
		relations.emplace(EmotionType::Admiration, Mood(0.5, 0.3, -0.2));
		relations.emplace(EmotionType::Shame, Mood(-0.3, 0.1, -0.6));
		relations.emplace(EmotionType::Reproach, Mood(-0.3, -0.1, 0.4));
		relations.emplace(EmotionType::Liking, Mood(0.40, 0.16, -0.24));
		relations.emplace(EmotionType::Disliking, Mood(-0.4, 0.2, 0.1));
		relations.emplace(EmotionType::Gratitude, Mood(0.4, 0.2, -0.3));
		relations.emplace(EmotionType::Anger, Mood(-0.51, 0.59, 0.25));
		relations.emplace(EmotionType::Gratification, Mood(0.6, 0.5, 0.4));
		relations.emplace(EmotionType::Remorse, Mood(-0.3, 0.1, -0.6));
		relations.emplace(EmotionType::Love, Mood(0.3, 0.1, 0.2));
		relations.emplace(EmotionType::Hate, Mood(-0.6, 0.6, 0.4));
	}

	/**
	* Returns the relating pleasure arousal and dominance values of an given
	* emotion type.
	*
	* @param type the emotion type
	*
	* @return a mood vector containing the pleasure arousal and dominance values
	*/
public:

	static Mood getEmotionPADMapping(EmotionType type) {//fixing hash map sorts this
		auto& emotionPR = getEmotionsPADRelation();
		auto& relations = emotionPR->relations;

		auto search = relations.find(type);
		if (search != relations.end()) {
			return search->second;	
		}
		else {
			std::cout << "Not found\n";
		}
		
	}

	/**
	* Returns the baseline for the specified emotion type. The baseline
	* depends on the agent's personality.
	*
	* @param type the emotion type
	* @param mood the mood
	*/
	 static void setEmotionPADMapping(EmotionType type, Mood mood) {
		 getEmotionsPADRelation()->relations.emplace(type, mood);
	}

	 static std::shared_ptr<EmotionsPADRelation> getEmotionsPADRelation() {
		if (instance == nullptr) {
			instance =  std::shared_ptr<EmotionsPADRelation>(new EmotionsPADRelation());
		}
		// Do initialization ...
		return instance;
	}

};

std::shared_ptr<EmotionsPADRelation> EmotionsPADRelation::instance;
// std::unordered_map<Emotion::Type, Mood> EmotionsPADRelation::relations = std::unordered_map<Emotion::Type, Mood>();