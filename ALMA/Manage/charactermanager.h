#ifndef CharacterManager_H
#define CharacterManager_H
#include"EntityManager.h"
#include"compute/EmotionEngine.h"
#include"compute/DecayFunction.h"
#include"compute/LinerDecayFunction.h"
#include"compute/ExponentialDecayFunction.h"
#include"compute/TangensHyperbolicusDecayFunction.h"
#include"compute/MoodEngine.h"
#include"Apprasial/EEC.h"
#include"Emotions/EmotionHistory.h"
#include"Emotions/EmotionVector.h"
#include"Emotions/Emotion.h"
#include "Mood/Mood.h"
#include"Personality/Personality.h"
#include"PErsonality/PersonalityEmotionsRelations.h"
#include"Event/EmotionMaintenanceEvent.h"
#include "asynctimerqueue.hh"
#include "Memory.h"
#include "MemStructs.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
//#include"AppraisalManager.h"

/*
gui.AffectMonitor
gui.AffectStatusDisplay
gui.AffectMonitorFrame
*/
#include "timer_manager.h"
#include"Data/AffectConsts.h"
#include"Emotions/EmotionType.h"
#include"Emotions/PADEmotion.h"
#include<mutex>
#include <Windows.h>
#include "BehaviorTree.h"
#include "json.hpp"

/*
manage.event.EmotionMaintenanceEvent
manage.event.EmotionMaintenancestd::listener
import static java.lang.Thread.sleep;*/

/**
* The class
* <code>CharacterManager</code> stores all necessary data and provides emotion
* and mood computation functions.
*
* @author Patrick Gebhard
*
* @version 1.0
*/
using json = nlohmann::json;
using namespace std;

 class CharacterManager :public EntityManager  {
	 friend class EmotionDecayTask;
	//public static Logger sLog = Logger.getLogger("Alma");
 private:


	 std::recursive_mutex _mutex;
	// static AffectManager.InterfaceHolder affectManager = AffectManager.sInterface;
	CharacterManager* fCharacterManagerInstance = nullptr;
	std::shared_ptr<PersonalityEmotionsRelations> fPersEmoRels = nullptr;
	MoodEngine* fMoodEngine = nullptr;
	EmotionEngine* fEmotionEngine = nullptr;
	EmotionHistory* fEmotionHistory = nullptr;
	bool fDerivedPersonality = false;
	DecayFunction* fDecayFunction = nullptr;
	//Timer fDecayTimer = nullptr;
	//Timer fMoodComputationTimer = nullptr;
	//Timer fInternalAppraisalTimer = nullptr;
	bool fShowAffectMonitor = false;
	bool fAffectComputationPaused = false;
	int emotionDecayId;
	int moodDecayId;
	double memDecayTime;
	BehaviorTree Btree;
	Memory memory;
	std::vector<std::string> MemoryNames;
	AppraisalManager* appraisalManagerInstance;
	std::unordered_map<std::string, BehaviorTree*> treeHolder;
	static PersonalityMoodRelations* instance;
	PersonalityMoodRelations* getInstance();
	PersonalityMoodRelations* getInstance(Mood OpennessRelation, Mood ConscientiousnessRelation, Mood ExtraversionRelation, Mood AgreeablenessRelation, Mood NeurotismRelation
		, Personality PleasureRelation, Personality ArousalRelation, Personality DominanceRelation);
	

	float random();
	/**
	* The
	* <code>EmotionDecayTask</code> class manages the emotion decay process.
	*/
	class EmotionDecayTask {
		friend class CharacterManager;
	private:
		EmotionDecayTask(CharacterManager* _this) : character(_this) {
		}
		CharacterManager* character;
	public:
		//synchronized 
		void  run() {			
				character->fEmotionEngine->decay(character->fEmotionHistory, character->fEmotionVector, character->createEmotionVector());
		}
	};



	/**
	* The
	* <code>MoodComputationTask</code> class manages the compuatation of the
	* actual mood.
	*/
	class MoodComputationTask {
		friend class CharacterManager;
	private:
		MoodComputationTask(CharacterManager* _this) : character(_this) {
		}
		CharacterManager* character;

	public:
		//synchronized 
		void  run() {
			character->fCurrentMood = character->fMoodEngine->compute(character->fCurrentMood, character->fEmotionVector);
			character->fCurrentMoodTendency = character->fMoodEngine->getCurrentMoodTendency();
		/*	if ((fAffectMonitor != nullptr) && fShowAffectMonitor) {
				fAffectMonitor.updateMoodDisplay(fName, fEmotionVector, fMoodEngine.getEmotionsCenter(),
					fDefaultMood, fCurrentMood, fCurrentMoodTendency);
			}*/
		}
	};

 public:

	 unique_ptr<Node> processBNode(const json& node);
	 

	 void createTree(std::string treename, std::string filename);


	 bool runTree(std::string treename);

	 std::string runTree(std::string treename, std::string targetId, std::string endType, int choice = -1);

	 void configSetup();
	

	 CharacterManager(std::string name, Personality personality,
		 AffectConsts* ac, bool derivedPersonality,
		 DecayFunction* decayFunction,
		 std::list<EmotionType> emotions);
		
 
	 void incermentTime();


	 void sendToMem(std::string emotion, std::string name, std::string Trigger,  Mood theMood, double intensity, std::string memname);//std::string reaction,

	 void 
		 emotionAffector(std::string memoryName);
	/**
	* Starts emotion decay, if not running.
	*/
	 void enableEmotionDecay(); 
	/**
	* Starts emotion monitoring, if not running.
	
	 void enableEmotionMonitoring() {
		fDecayTimer = new Timer(true);
		fDecayTimer.schedule(new EmotionMonitorTask(this), fAc->emotionDecayPeriod, fAc->emotionDecayPeriod);
	}*/

	/**
	* Stops emotion decay, if running.
	*/
	 void disableEmotionDecay();

	/**
	* Returns a flag if affect computation is paused.
	*/
	 bool isAffectComputationPaused(); 
	

	/**
	* Pauses all dynamic affect computation
	*
	* @return true if the affect computation is pause, false otherwise
	*/
	 bool pauseAffectComputation(); 

	/**
	* Resumes a paused dynamic affect computation
	*
	* @return true if the affect computation is continued, false otherwise
	*/
	 bool resumeAffectComputation();
	 

	/**
	* Performs a single step for a paused affect computation (emotion decay and
	* mood computation)
	*
	* @return true if one step affect computation is performed correctly, false
	* otherwise
	*/
	 bool stepwiseAffectComputation(); 
	 
	/**
	* Sets the visible state of the character's affect monitor
	*
	* @param visible true shows the character's affect monitor, false otherwise
	
	 void showMonitor( bool visible) {

		// TODO: Put the operation into a thread if the affectMonitor object

		// has not been yet instanciated
		if ((fAffectMonitor == nullptr) && visible) {
			Thread waitNShowMonitor = new Thread(){
				public void run() {
				bool exit = false;
				while (!exit) {
					//debug log.info("Waiting for affect monitor ...");
					if (fAffectMonitor != nullptr) {
						exit = true;
						fShowAffectMonitor = visible;
						fAffectMonitor.showFrame(visible);
					}
					try {
						sleep(500);
					}
					catch (InterruptedException ie) {
						ie.printStackTrace();
					}
				}
			}
			};
			waitNShowMonitor.start();
			return;
		}
		if (fAffectMonitor != nullptr) {
			fShowAffectMonitor = visible;
			fAffectMonitor.showFrame(visible);
		}
	}*/

	/**
	* Returns the visible state of the character's affect monitor
	*
	* @return true if the character's affect monitor is visible, false otherwise
	*/
	 bool hasActiveAffectMonitor();

	/**
	* Returns the character's affect monitor
	*
	* @return true if the character's affect monitor is visible, false otherwise
	
	 AffectMonitor getAffectMonitor() {
		return fAffectMonitor;
	}*/

	/**
	* Sets a new affect monitor for this character.
	*
	* @param affectMonitor the new affect monitor
	
	// synchronized
	 void setAffectMonitor(AffectMonitor affectMonitor) {
		disableEmotionDecay();
		disableMoodComputation();
		if (affectMonitor != nullptr) {
			fAffectMonitor = affectMonitor;
			fShowAffectMonitor = true;
			fAffectMonitor.addEmotionMaintenancestd::listener(fCharacterManagerInstance);
			fAffectMonitor.updateEmotionDisplay(fName, fEmotionVector);
			fAffectMonitor.updateMoodDisplay(fName, fEmotionVector, fMoodEngine.getEmotionsCenter(),
				fDefaultMood, fCurrentMood, fCurrentMoodTendency);
		}
		enableMoodComputation();
		enableEmotionDecay();
	}*/

	/**
	* Sets a new affect status display for this character.
	*
	* @param affectStatus the new affect status display
	
	// synchronized 
	 void setAffectStatusDisplay(AffectStatusDisplay affectStatus) {
		disableEmotionDecay();
		disableMoodComputation();
		if (affectStatus != nullptr) {
			fAffectStatusDisplay = affectStatus;
			fAffectStatusDisplay.updateStatusDisplay(fName, fEmotionVector,
				fDefaultMood, fCurrentMood, fCurrentMoodTendency);
		}
		enableMoodComputation();
		enableEmotionDecay();
	}*/



	/**
	* Starts mood computation, if not running.
	*/
	 void enableMoodComputation();

	/**
	* Stops mood computation, if running.
	*/
	 void disableMoodComputation();

	 void addEEC(EEC* eec);

	/**
	* Sets a new emotion decay function and decaySteps and reinitialize the
	* character's emotion computation engine.
	*
	* @param decayFunction the new emotion decay function
	*/
	// synchronized 
	 void setDecayFunction(LinearDecayFunction* decayFunction);

	 void setDecayFunction(ExponentialDecayFunction* decayFunction);

	 void setDecayFunction(TangensHyperbolicusDecayFunction* decayFunction);

	/**
	* Returns the character's actual decay function
	*
	* @return the character's actual decay function
	*/
	// synchronized
	 DecayFunction* getDecayFunction();

	/**
	* Returns the character's emotion history containing all active emotions
	*
	* @return an EmotionHistory object
	*/
	// synchronized
	 EmotionHistory* getEmotionHistory();

	/**
	* Returns the character's emotion engine
	*
	* @return an EmotionEngine object
	*/
	// synchronized
	 EmotionEngine* getEmotionEngine();

	 MoodEngine* getMoodEngine();
	/**
	* Sets a new personality for this character and reinitialize the character's
	* emotion computation engine.
	*
	* @param personality the new personality
	*/
	// synchronized
	 void setPersonality(Personality personality);

	 //synchronized 
	 void setAffectConsts(AffectConsts* ac);

	
		// synchronized
	 AffectConsts* getAffectConsts();
	/**
	* Infers emotions based on the specified std::list of Emotion Eliciting Conditions
	* and updates the character's emotional state displayed by the affect
	* monitor.
	*
	* @return generated emotions
	*/
	// synchronized
	 std::shared_ptr<EmotionVector> inferEmotions();

	// synchronized 
	 std::shared_ptr<EmotionVector> infuseBioSignalEmotions(std::shared_ptr<PADEmotion> e);

	/**
	* Prints all emotions in the specified collection whose intensity is greater
	* than the baseline.
	*/
	/*private void printEmotions() {
		std::string newLine = System.getProperty("line.separator");
		std::string indent = " ";
		std::stringBuffer sb = new std::stringBuffer("[EmotionVector: ");
		for (Iterator it = fEmotionVector.getEmotions().iterator(); it.hasNext();) {
			Emotion emotion = (Emotion)it.next();
			if (emotion.getIntensity() > emotion.getBaseline()) {
				sb.append(indent);
				sb.append(emotion.tostd::string());
				sb.append(newLine);
			}
		}
		sb.append("]");
		sLog.info(sb.tostd::string());
	}*/

	 bool isDerivedPersonality();

	/**
	* Stops all affect processing, decaying tasks
	*/
	 void stopAll();

	/**
	* Implements EmotionMaintenancestd::listener
	*/
	 void maintainEmotion(EmotionMaintenanceEvent e);



	 void computeMood();
	 
};

#endif