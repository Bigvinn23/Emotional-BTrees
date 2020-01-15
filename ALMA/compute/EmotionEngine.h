#pragma once
//#include"Emotions/EmotionAppraisalVars.h"
#include"../Emotions/EmotionAppraisalVars.h"
#include"../Apprasial/EEC.h"
#include"../Emotions/Emotion.h"
#include"../Emotions/Emotionhistory.h"
#include"../Emotions/EmotionType.h"
#include"../Emotions/EmotionVector.h"
#include"../Apprasial/eec/Agency.h"
//#include"AffectManager.h"
#include"../Mood/Mood.h"
#include"../Personality/Personality.h"
#include"../Personality/PersonalityEmotionsRelations.h"
#include"../Personality/PersonalityMoodRelations.h"
#include<list>
#include"../compute/DecayFunction.h"
#include<set>

/**
* The
* <code>EmotionEngine</code> class is used to infer the emotional state of an
* agent based on a list of Emotion Eliciting Conditions.
*
* @author Patrick Gebhard (based on the work of Michael Kipp and Martin Klesen)
*
* @version 3.0
*/
class EmotionEngine {

	/**
	* list of Emotion Eliciting Conditions
	*/
private:
	std::recursive_mutex _mutex;
	std::list<EEC*> listOfEEC;
	/**
	* the character's personality
	*/
	Personality fPersonality;
	std::shared_ptr<PersonalityEmotionsRelations> fPerEmoRel = nullptr;
	std::shared_ptr<PersonalityMoodRelations> fPerMoodRel = PersonalityMoodRelations::getInstance();
	/*
	* the emotion engine's decay function
	*/
	DecayFunction* fDecayFunction = nullptr;
	/**
	* the current emotion eliciting conditions
	*/
	EEC* eventEEC = nullptr;
	EEC* actionEEC = nullptr;
	EEC* ElicitEEC = nullptr;


	/**
	* Returns the adjusted emotion intensity according to the correlation of
	* personality traits and emotions.
	*
	* @param intensity the initial emotion intensity
	* @param type the emotion type
	*
	* @return the adjusted emotion intensity.
	*/

	double max(double x, double y);
	/**
	* Constructs a new
	* <code>EmotionEngine</code> Emotion::Elicit with the specified personality for the
	* agent doing the emotional appraisal.
	*
	* @param personality the agent's personality
	* @param decayFunction the engine's emotion decay function
	* @throws nullptrPointerException if the personality or the decay function is
	* not specified.
	*/
public:
	EmotionEngine(Personality personality, DecayFunction* decayFunction);
	double adjustIntensity(double intensity, EmotionType type, Mood mood);
	/**
	* Adds a new element to the list of Emotion Eliciting Conditions. This method
	* throws an
	* <code>IllegalStateException</code> if an an EEC for event-based emotions,
	* action-based emotions, or Emotion::Elicit-based emotions already exists to prevent
	* the generation of emotions of the same type. This is necessary since the
	* <code>EmotionVector</code> used to store the infered emotions contains
	* exactly <b>one</b> emotion for each emotion type.
	*
	* @param eec the std::make_shared<Emotion> Eliciting Condition.
	*
	* @throws nullptrPointerException if the Emotion Eliciting Condition is not
	* specified.
	*
	* @throws IllegalStateException if an EEC for event-based emotions,
	* action-based emotions, or Emotion::Elicit-based emotions already exists.
	*
	* @see EmotionVector
	*/
	void addEEC(EEC* eec);

	/**
	* Removes all elements from the list of Emotion Eliciting Conditions.
	*/
	void clearEEC();



	/**
	* Decays the specified emotion history using the engine's decay function.
	* This method decays each element in the emotion history and returns the new
	* emotional state. It generates an
	* <code>EmotionChangeEvent</code> to signal that the emotional state has
	* changed.
	*
	* @param history the emotion history
	* @param emotions the collection used to store the emotional state
	* @return the std::make_shared<Emotion>al state.
	* @throws nullptrPointerException if no history is specified.
	* @see EmotionVector getEmotionalState(EmotionVector emotions)
	*/
	// synchronized
	std::shared_ptr<EmotionVector> decay(EmotionHistory* history,
		std::shared_ptr<EmotionVector> emotions,
		std::shared_ptr<EmotionVector> defaultEmotions);

	/**
	* Simulates the future decay of a specific emotion using the engine's decay
	* function.
	*
	* @param emotion the emotion that will be decayed
	* @return List of intesity values that denotes the decay of the emotion
	* @throws nullptrPointerException if no history is specified.
	*/
	//synchronized 
	std::list<float> simulateDecay(std::shared_ptr<Emotion> emotion);

	/**
	* Infers all emotions based on the list of Emotion Eliciting Conditions and
	* adds them to the specified collection. If the type of an infered emotion is
	* not contained in the predefined set of emotion types this emotion will be
	* ignored. <P> If a compound emotion (e.g.
	* <code>Gratitude</code>) can be infered then the two corresponding single
	* emotions (
	* <code>Joy</code> and
	* <code>Admiration</code>) are not returned as part of the result.
	*
	* @param emotions the emotions to which the infered one will be added
	* @param history the emotion history
	* @return the modified collection of emotions.
	* @throws nullptrPointerException if the emotion history is not specified.
	*/
	//synchronized 
	std::shared_ptr<EmotionVector> inferEmotions(std::shared_ptr<EmotionVector> emotions, EmotionHistory* history, Mood mood);
};