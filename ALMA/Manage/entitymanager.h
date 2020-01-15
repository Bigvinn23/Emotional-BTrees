#pragma once
#include "Apprasial/ApprasialVariables.h"
#include"AppraisalRules.h"
#include "Emotions/EmotionVector.h"
#include "Mood/Mood.h"
#include "Personality/Personality.h"
#include "Data/AffectConsts.h"
#include "Emotions/EmotionType.h"

#include"Personality/PersonalityMoodRelations.h"

/**
* The class <code>EntityManager</code> initially stores name and
* appraisal rules of an abstract entity.
* It also provides placeholders for basic affective structure such as
* personality, mood, emotions, these must be inspirit by higher-level entities
* (e.g. by the <code>CharacterManager</code> or <code>GroupManager</code>)
*
* @author Patrick Gebhard
*
* @version 1.0
*/
class EntityManager {

protected:
	std::recursive_mutex _mutex;
	std::string fName;
	AppraisalRules* fAppraisalRules = nullptr;
	Personality fPersonality;
	Mood fDefaultMood ;
	Mood fCurrentMood ;
	Mood fCurrentMoodTendency ;
	std::shared_ptr<EmotionVector> fEmotionVector = nullptr;
	std::list<EmotionType> fAvailEmotions ;
	AffectConsts* fAc = nullptr;
	//private Logger log = Logger.getLogger(Logger.GLOBAL_LOGGER_NAME);;

public:
	EntityManager(){}
	EntityManager(std::string name) {
		fName = name;
		//Initialise the affect placeholders with default values
		/*fPersonality = std::make_shared<Personality>(0.0, 0.0, 0.0, 0.0, 0.0);
		fDefaultMood = PersonalityMoodRelations::getDefaultMood(fPersonality);
		fCurrentMood = PersonalityMoodRelations::getDefaultMood(fPersonality);
		fCurrentMoodTendency = PersonalityMoodRelations::getDefaultMood(fPersonality);
		fAvailEmotions = std::list<EmotionType>();
		fAvailEmotions.push_back(EmotionType::Undefined);
		// PG: added a new common type of emotions that are derived by bio sensors
		fAvailEmotions.push_back(EmotionType::Physical);
		fEmotionVector = createEmotionVector();
		fAc = new AffectConsts();*/
	}

	/**
	* <code>getName</code> returns the name of this entity
	*
	* @return std::string contains the name of this entity
	*/
	 std::string getName() {
		return fName;
	}

	/**
	* <code>setAppraisalRules</code> set the appraisalRules for this entity.
	*
	* @param rules a new AppraisalRules
	*/
	  void setAppraisalRules(AppraisalRules* rules) {
		  std::unique_lock<std::recursive_mutex> lock(_mutex);
		fAppraisalRules = rules;
	}

	/**
	* <code>getAppraisalRules</code> returns all appraisalRules for this entity.
	*
	* @return AppraisalRules contains the actual entitie's appraisal rules
	*/
	 AppraisalRules* getAppraisalRules() {
		 std::unique_lock<std::recursive_mutex> lock(_mutex);
		return fAppraisalRules;
	}

	/**
	* <code>getAppraisalRules</code> returns appraisalRules of the given type
	* for this entity.
	*
	* @param type type of appraisal variables
	*
	* @return AppraisalRules contains the actual entitie's appraisal rules
	*/
	 AppraisalRules getAppraisalRules(std::string type) {
		 std::unique_lock<std::recursive_mutex> lock(_mutex);
		return fAppraisalRules->getAppraisalRulesByType(fName, type);
	}

	/**
	* <code>getAppraisalRuleKeys</code> returns appraisal rule keys of the given
	* type for this entity.
	*
	* @param type type of appraisal variables
	*
	* @return AppraisalRules contains the actual entitie's appraisal rules
	*/
	 std::string* getAppraisalRuleKeys(std::string type) {
		 std::unique_lock<std::recursive_mutex> lock(_mutex);
		return fAppraisalRules->getAppraisalRulesByType(fName, type).getKeys(fName);
	}

	/**
	* <code>getAppraisalVariables</code> returns AppraisalVariables for a given
	* key.
	*
	* @param key the rule key that is related to a signal of the world
	*
	* @return AppraisalVariables[] contains the appraisal variables related to
	*                               a key and this entity
	*/
	 AppraisalVariables* getAppraisalVariables(std::string key) {
		 std::unique_lock<std::recursive_mutex> lock(_mutex);
		AppraisalVariables* appVars = fAppraisalRules->getAppraisalVariables(fName, key);
		appVars = (appVars == nullptr) ? new AppraisalVariables() : appVars;
		return appVars;
	}

	/**
	* <code>getAppraisalVariables</code> returns AppraisalVariables for a given
	* key.
	*
	* @param key the rule key that is related to a signal of the world
	* @param type type of appraisal variables
	*
	* @return AppraisalVariables[] contains the appraisal variables related to
	*                               a key and this entity
	*/
	 AppraisalVariables* getAppraisalVariables(std::string key, std::string type) {
		 std::unique_lock<std::recursive_mutex> lock(_mutex);
		AppraisalVariables* appVars =
			fAppraisalRules->getAppraisalRulesByType(fName, type).getAppraisalVariables(fName, key);
		appVars = (appVars == nullptr) ? new AppraisalVariables[0] : appVars;
		return appVars;
	}

	/**
	* <code>getAppraisalVariables</code> returns AppraisalVariables for a given
	* key.
	*
	* @param entity another entity that is related to that key
	* @param key the rule key that is related to a signal of the world
	*
	* @return AppraisalVariables[] contains the appraisal variables related to
	*                               a key and entity
	*/
	 AppraisalVariables* getAppraisalVariables(EntityManager entity, std::string key) {
		 std::unique_lock<std::recursive_mutex> lock(_mutex);
		AppraisalVariables* appVars = fAppraisalRules->getAppraisalVariables(entity.getName(), key);
		appVars = (appVars == nullptr) ? new AppraisalVariables[0] : appVars;
		return appVars;
	}

	/**
	* <code>getAppraisalVariables</code> returns AppraisalVariables for a given
	* key.
	*
	* @param entity another entity that is related to that key
	* @param key the rule key that is related to a signal of the world
	* @param type type of appraisal variables
	*
	* @return AppraisalVariables[] contains the appraisal variables related to
	*                               a key and entity
	*/
	 AppraisalVariables* getAppraisalVariables(EntityManager entity, std::string key, std::string type) {
		 std::unique_lock<std::recursive_mutex> lock(_mutex);
		AppraisalVariables* appVars =
			fAppraisalRules->getAppraisalRulesByType(entity.getName(), type).getAppraisalVariables(entity.getName(), key);
		appVars = (appVars == nullptr) ? new AppraisalVariables[0] : appVars;
		return appVars;
	}

	/**
	* <code>getAppraisalVariables</code> returns all keys related to signals
	*
	* @param entity the entity that is related to that key
	*
	* @return std::string[] contains all keys related to the entity
	*/
	std::string* getAppraisalRuleKeys(EntityManager entity) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		return fAppraisalRules->getKeys(entity.getName());
	}

	/**
	* Gets the personality of this entity
	*
	* @return the entity current personality
	*/
	 Personality getPersonality() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		return fPersonality;
	}

	 Mood getCurrentMood() {
		 std::unique_lock<std::recursive_mutex> lock(_mutex);
		return fCurrentMood;
	}

	 Mood defaultMood() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		return fDefaultMood;
	}

	 Mood getCurrentMoodTendency() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		return fCurrentMood;
	}


	/**
	* Sets the mood of this entity
	*
	* @return the entity current personality
	*/
	 void setCurrentMood(Mood m) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		fCurrentMood = m;
	}

	 void setDefaultMood(Mood m) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		fDefaultMood = m;
	}

	 void resetDefaultMood() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		fDefaultMood = PersonalityMoodRelations::getDefaultMood(fPersonality);
		fCurrentMood = fDefaultMood;
	}

	 double getDistancetoDefaultMood() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		double cPleasure = fCurrentMood.getPleasure();
		double cArousal = fCurrentMood.getArousal();
		double cDominance = fCurrentMood.getDominance();
		double dPleasure = fDefaultMood.getPleasure();
		double dArousal = fDefaultMood.getArousal();
		double dDominance = fDefaultMood.getDominance();
		return
			sqrt(pow(abs(cPleasure - dPleasure), 2.0) +
				pow(abs(cArousal - dArousal), 2.0) +
				pow(abs(cDominance - dDominance), 2.0));
	}

	 std::shared_ptr<EmotionVector> getCurrentEmotions() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		//log.info("Get current Emotions " + fEmotionVector.tostd::string());
		return fEmotionVector;
	}

	 AffectConsts* getAffectConsts() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		return fAc;
	}

	 std::shared_ptr<EmotionVector> createEmotionVector() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		return std::make_shared< EmotionVector>(fPersonality, fCurrentMood, fAvailEmotions);
	}

};