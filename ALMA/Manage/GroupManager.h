#pragma once
#include"EmotionEngine.h"
#include"compute/DecayFunction.h"
#include"compute/MoodEngine.h"
#include"Apprasial/ApprasialVariables.h"
#include"EEC.h"
#include"Emotions/EmotionHistory.h"
#include"EmotionVector.h"
#include"Emotion.h"
#include"Mood/Mood.h"
#include"Personality/Personality.h"
#include"Personality/PersonalityEmotionsRelations.h"
//GroupAffectMonitor;
//#include"GroupAffectMonitorFrame.h"
//#include"GroupAffectMonitorInternalFrame.h"
#include"Data/AffectConsts.h"
#include"Emotions/EmotionType.h"
//#include"EmotionMaintenanceEvent.h"
//EmotionMaintenancestd::listener;
#include"EntityManager.h"
#include"CharacterManager.h"
#include"Personality/PersonalityMoodRelations.h"
#include<mutex>
//#include<Winuser.h>
//import static de.affect.gui.AlmaGUI.sIntegratedDesktopMode;
//import static de.affect.util.Convert.doubleValue;

/**
* The class
* <code>GroupManager</code> stores all necessary data and provides emotion and
* mood computation functions for a group of characters.
*
* @author Patrick Gebhard
*
* @version 1.0
*/
 class GroupManager :public EntityManager  {
	 friend class EmotionDecayTask;
	 friend class EmotionMonitorTask;
//	public static Logger sLog = Logger.getLogger("Alma");
 private:
	 std::recursive_mutex _mutex;
	 GroupManager* fGroupManagerInstance = nullptr;
	 std::vector<std::shared_ptr<CharacterManager>> fCharacters;
	 std::shared_ptr<PersonalityEmotionsRelations> fPersEmoRels = nullptr;
	 MoodEngine* fMoodEngine = nullptr;
	 EmotionEngine* fEmotionEngine = nullptr;
	 EmotionHistory* fEmotionHistory = nullptr;
	// GroupAffectMonitor fGroupAffectMonitor = nullptr;
	 DecayFunction* fDecayFunction = nullptr;
	// Timer fDecayTimer = nullptr;
	// Timer fMoodComputationTimer = nullptr;
	 double fSocialIntegrity = 0.0;
	 std::string fInSimilarMood = "none";
	 std::vector<std::shared_ptr<CharacterManager>> fInExtremeMood ;
	 bool fEnableDecay = true;
	 bool fShowAffectMonitor = false;
	bool fAffectComputationPaused = false;


	/**
	* Prints all emotions in the specified collection whose intensity is greater
	* than the baseline.
	
	void printEmotions() {
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

	/**
	* The
	* <code>EmotionDecayTask</code> class manages the emotion decay process.
	*/
	class MoodComputationTask {
		friend class GroupManager;
	private:
		std::unordered_map<std::shared_ptr<CharacterManager>, std::shared_ptr<CharacterManager>> characterInSimilarMood;


		MoodComputationTask(GroupManager* _this) : group(_this) {
		}
		GroupManager* group;
		std::recursive_mutex _mutex;

	public:
		//synchronized 
		void  run() {
			group->fCurrentMood = group->fMoodEngine->compute(group->fCurrentMood, group->fEmotionVector);
			// compute social integrity
			double pleasure = 0.0;
			double arousal = 0.0;
			double dominance = 0.0;
			double compPleasure = 0.0;
			double compArousal = 0.0;
			double compDominance = 0.0;
			double rate = 0.0;
			double distanceToDefaultMood = 0.0;
			int cnt = 0;
			characterInSimilarMood.clear();

			if(!group->fInExtremeMood.empty()) {
				group->fInExtremeMood.clear();
				for (cnt = 0; cnt < group->fCharacters.size; cnt++) {
					//log.info("CharacterNumber "+cnt);
					pleasure = group->fCharacters[cnt]->getCurrentMood().getPleasure();
					arousal = group->fCharacters[cnt]->getCurrentMood().getArousal();
					dominance = group->fCharacters[cnt]->getCurrentMood().getDominance();
					// extreme mood
					distanceToDefaultMood = group->fCharacters[cnt]->getDistancetoDefaultMood();
					if (group->fInExtremeMood.empty()) {
						//log.info("Character added");
						group->fInExtremeMood.emplace_back(group->fCharacters[cnt]);
					}
					else {
						int ccnt = 0;
						double lastDistance = 0.0;
						for (const auto& cm : group->fInExtremeMood) {
							lastDistance = cm->getDistancetoDefaultMood();
							ccnt = (lastDistance > distanceToDefaultMood) ? ccnt + 1 : ccnt;
						}
						group->fInExtremeMood[ccnt]= group->fCharacters[cnt];
						//log.info("Character was sorted in at pos " + ccnt);
					}

					// social integrity and similar mood
					if ((cnt + 1) < group->fCharacters.size) {
						for (int ccnt = cnt + 1; ccnt < group->fCharacters.size; ccnt++) {
							compPleasure = group->fCharacters[ccnt]->getCurrentMood().getPleasure();
							compArousal = group->fCharacters[ccnt]->getCurrentMood().getArousal();
							compDominance = group->fCharacters[ccnt]->getCurrentMood().getDominance();
							// determitation of the oktant difference
							// 0 means same oktant, 1 means adjacent oktant, 
							// 2 means obliquely opposite, 3 completely opposite (through pad origin)
							// The number also indicates how many oktants awy the other mood is
							// located from 
							int diff = 0;
							diff = (((pleasure < 0.0) && (compPleasure > 0.0))
								|| ((pleasure > 0.0) && (compPleasure < 0.0))) ? diff + 1 : diff;
							//log.info("difference in pleasure " + diff);
							diff = (((arousal < 0.0) && (compArousal > 0.0))
								|| ((arousal > 0.0) && (compArousal < 0.0))) ? diff + 1 : diff;
							//log.info("difference in arousal " + diff);
							diff = (((dominance < 0.0) && (compDominance > 0.0))
								|| ((dominance > 0.0) && (compDominance < 0.0))) ? diff + 1 : diff;
							//log.info("difference in dominance " + diff);
							//log.info(group->fCharacters[cnt].getName() + "'s mood is  " + diff + 
							//         " oktants away from " + group->fCharacters[ccnt].getName() + "'s mood");
							// save all pairs of characters which's mood is in the same oktant
							if (diff == 0) {
								if (characterInSimilarMood.count(group->fCharacters[cnt])) {
									characterInSimilarMood.emplace(group->fCharacters[ccnt], group->fCharacters[cnt]);
								}
								else {
									characterInSimilarMood.emplace(group->fCharacters[cnt], group->fCharacters[ccnt]);
								}
							}
							// rate the difference of oktants
							rate += diff * 3.0; //add +1 for each oktaant
												 // computation of distance
							rate += sqrt(pow(abs(pleasure - compPleasure), 2.0)
								+ pow(abs(arousal - compArousal), 2.0)
								+ pow(abs(dominance - compDominance), 2.0));
						}
					}
				}
			}
			//log.info("---");
			group->fSocialIntegrity = (1.0 / group->fCharacters.size) * rate;
			//log.info("Social integrity value: " + fSocialIntegrity);
			group->fInSimilarMood = "none";
			if (!characterInSimilarMood.empty()) {
				group->fInSimilarMood = "";
				int size = characterInSimilarMood.size();
				for (const auto& cEnum : characterInSimilarMood) {
					std::shared_ptr<CharacterManager> firstChar = cEnum.first;
					std::shared_ptr<CharacterManager> secondChar = cEnum.second;
					if (size != 0)
					{
						group->fInSimilarMood += (size==1)
												? firstChar->getName() + " - " + secondChar->getName() + ", "
												: firstChar->getName() + " - " + secondChar->getName();
						size--;
					}
					
				}
			}
			/*	//log.info("Characters in similar mood: " + fInSimilarMood);
			if ((fGroupAffectMonitor != nullptr) && fShowAffectMonitor) {
			fGroupAffectMonitor.updateMoodDisplay(fName, fEmotionVector,
			fMoodEngine.getEmotionsCenter(),
			fDefaultMood, fCurrentMood,
			fSocialIntegrity, fInSimilarMood,
			fInExtremeMood);
			}*/
		}
	};

	class EmotionDecayTask {
		friend class GroupManager;
	private:
		EmotionDecayTask(GroupManager* _this) : group(_this) {
		}
		GroupManager* group;

	public:
		//synchronized
		void run() {
			group->fEmotionVector =
				group->fEmotionEngine->decay(group->fEmotionHistory, group->fEmotionVector, group->createEmotionVector());
			/*if ((fGroupAffectMonitor != nullptr) && fShowAffectMonitor) {
				fGroupAffectMonitor.updateEmotionDisplay(fName, fEmotionVector);
			}*/
		}
	};

	/**
	* The
	* <code>EmotionMonitorTask</code> class displays the elicited emotions. This
	* is usually done by the EmotionDecayTask, but in case it is diabled some other
	* method has to monitor the emotions.
	
	class EmotionMonitorTask {
		friend class GrouprManager;
	private:
		EmotionMonitorTask() {
		}

	public:
		//synchronized 
		void run() {
			if ((fGroupAffectMonitor != nullptr) && fShowAffectMonitor) {
				fGroupAffectMonitor.updateEmotionDisplay(fName, fEmotionVector);
			}
		}
	};*/

 public:
	 GroupManager(std::string name, std::vector<std::shared_ptr<CharacterManager>> characters,
		Personality personality, AffectConsts* ac,
		DecayFunction* decayFunction,
		std::list<EmotionType> emotions)
		 :EntityManager(name)
		{
		fGroupManagerInstance = this;
		fCharacters = characters;
		fPersonality = personality;
		fAc = ac;
		fDecayFunction = decayFunction;
		fAvailEmotions = emotions;
		fPersEmoRels = personality.getPersonalityEmotionsRelations();
		fDefaultMood = PersonalityMoodRelations::getDefaultMood(personality);
		fCurrentMood = PersonalityMoodRelations::getDefaultMood(personality);
		fSocialIntegrity = 0.0;
		fInSimilarMood = "none";
		fInExtremeMood = std::vector<std::shared_ptr<CharacterManager>>();
		// Setup emotion processing
		fEmotionVector = createEmotionVector();
		fEmotionHistory = new EmotionHistory();
		fDecayFunction->init(fAc->emotionDecaySteps);
		fEmotionEngine = new EmotionEngine(fPersonality, fDecayFunction);
		if (fEnableDecay) {
			std::shared_ptr<EmotionDecayTask> hello;
			timer_manager::func_t EmoDecay = [hello](const timer_manager::timer_ptr_t&)
			{
				hello->run();
			};
			timer_manager::add_timer(fAc->emotionDecayPeriod, EmoDecay, false, "emo");
			//SetTimer(NULL, 0, fAc->emotionDecayPeriod, new EmotionDecayTask(this)->run);
			//fDecayTimer = new Timer(true);
			//fDecayTimer.schedule(new EmotionDecayTask(this), fAc->emotionDecayPeriod, fAc->emotionDecayPeriod);
		}
		// Setup mood processing	
		fMoodEngine = new MoodEngine(fPersonality, fAc->moodStabilityControlledByNeurotism,
			fDefaultMood, fAc->moodReturnOverallTime);
		std::shared_ptr<MoodComputationTask> bye;
		timer_manager::func_t MooddDecay = [bye](const timer_manager::timer_ptr_t&)
		{
			bye->run();
		};
		timer_manager::add_timer(fAc->moodReturnPeriod, MooddDecay, false, "mood");
		//SetTimer(NULL, 1, fAc->moodReturnPeriod, new MoodComputationTask(this)->run);
		//fMoodComputationTimer = new Timer(true);
	//	fMoodComputationTimer.schedule(new MoodComputationTask(this), fAc->moodReturnPeriod, fAc->moodReturnPeriod);
		// Setup affect monitoring
		/*if (!sIntegratedDesktopMode) {
			Thread startAffectMonitor = new Thread(){
				public void run() {
				fGroupAffectMonitor =
					(GroupAffectMonitor) new GroupAffectMonitorFrame(fName, fEmotionVector,
						fCurrentMood,
						fSocialIntegrity,
						fInSimilarMood,
						fInExtremeMood);

			}
			};
			startAffectMonitor.start();
		}*/
	}

	



	/**
	* Returns a flag if emotion decay is enabled.
	*/
	 bool isEnabledEmotionDecay() {
		return fEnableDecay;
	}

	/**
	* Starts emotion decay, if not running.
	*/
	 void enableEmotionDecay() {
		if (!fEnableDecay) {
			timer_manager::remove_timer(timer_manager::getemo());
		//	KillTimer(0, 0);
			std::shared_ptr<EmotionDecayTask> hello;
			timer_manager::func_t EmoDecay = [hello](const timer_manager::timer_ptr_t&)
			{
				hello->run();
			};
			timer_manager::add_timer(fAc->emotionDecayPeriod, EmoDecay, false, "emo");
			//SetTimer(NULL, 0, fAc->emotionDecayPeriod, new EmotionDecayTask(this)->run);
			fEnableDecay = true;
		}
	}

	/**
	* Starts emotion monitoring, if not running.
	
	 void enableEmotionMonitoring() {
		if (!fEnableDecay) {
			fDecayTimer = new Timer(true);
			fDecayTimer.schedule(new EmotionMonitorTask(), fAc->emotionDecayPeriod, fAc->emotionDecayPeriod);
		}
	}*/

	/**
	* Stops emotion decay, if running.
	*/
	 void disableEmotionDecay() {
		if (fEnableDecay) {
			timer_manager::remove_timer(timer_manager::getemo());
			//KillTimer(0, 0);
			fEnableDecay = false;
		}
	}

	/**
	* Returns a flag if affect computation is paused.
	*/
	 bool isAffectComputationPaused() {
		return fAffectComputationPaused;
	}

	/**
	* Pauses all dynamic affect computation
	*
	* @return true if the affect computation is pause, false otherwise
	*/
	 bool pauseAffectComputation() {
		//sLog.info(fName + " affect computation paused ...");
		stopAll();
		fAffectComputationPaused = true;
		return true;
	}

	/**
	* Resumes a paused dynamic affect computation
	*
	* @return true if the affect computation is continued, false otherwise
	*/
	 bool resumeAffectComputation() {
		if (fAffectComputationPaused) {
			//sLog.info(fName + " emotion computation resumed ...");
			std::shared_ptr<EmotionDecayTask> hello;
			timer_manager::func_t EmoDecay = [hello](const timer_manager::timer_ptr_t&)
			{
				hello->run();
			};
			timer_manager::add_timer(fAc->emotionDecayPeriod, EmoDecay, false, "emo");
			//SetTimer(NULL, 0, fAc->emotionDecayPeriod, new EmotionDecayTask(this)->run);

			//sLog.info(fName + " mood computation resumed ...");
			fMoodEngine = new MoodEngine(fPersonality, fAc->moodStabilityControlledByNeurotism, fDefaultMood, fAc->moodReturnOverallTime);
			//KillTimer(0, 1);
			timer_manager::remove_timer(timer_manager::getmood());
			std::shared_ptr<MoodComputationTask> bye;
			timer_manager::func_t MooddDecay = [bye](const timer_manager::timer_ptr_t&)
			{
				bye->run();
			};
			timer_manager::add_timer(fAc->moodReturnPeriod, MooddDecay, false, "mood");
			//SetTimer(NULL, 1, fAc->moodReturnPeriod, new MoodComputationTask(this)->run);
			
			return true;
		}
		else {
			return false;
		}
	}

	/**
	* Performs a single step for affect computation (emotion decay and mood computation)
	*
	* @return true if the affect computation is continued, false otherwise
	*/
	 bool stepwiseAffectComputation() {
		if (fAffectComputationPaused) {

			EmotionDecayTask* ed = new EmotionDecayTask(this);
			ed->run();
			MoodComputationTask* mc = new MoodComputationTask(this);
			mc->run();

			return true;
		}
		else {
			return false;
		}
	}
	/**
	* Sets the visible state of the character's affect monitor
	*
	* @param visible true shows the character's affect monitor, false otherwise
	
	 void showMonitor( bool visible) {
		// Put the operation into a thread if the affectMonitor object
		// has not been yet instanciated
		if ((fGroupAffectMonitor == nullptr) && visible) {
			Thread waitNShowMonitor = new Thread(){
				public void run() {
				bool exit = false;
				while (!exit) {
					//debug log.info("Waiting for affect monitor ...");
					if (fGroupAffectMonitor != nullptr) {
						exit = true;
						fShowAffectMonitor = visible;
						fGroupAffectMonitor.showFrame(visible);
					}
					try {
						this.sleep(500);
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
		if (fGroupAffectMonitor != nullptr) {
			fShowAffectMonitor = visible;
			fGroupAffectMonitor.showFrame(visible);
		}
	}*/

	/**
	* Returns the visible state of the character's affect monitor
	*
	* @return true if the character's affect monitor is visible, false otherwise
	*/
	 bool hasActiveAffectMonitor() {
		return fShowAffectMonitor;
	}

	/**
	* Returns the character's affect monitor
	*
	* @return true if the character's affect monitor is visible, false otherwise
	
	 GroupAffectMonitor getAffectMonitor() {
		return fGroupAffectMonitor;
	}*/

	/**
	* Sets a new affect monitor for this character.
	*
	* @param affectMonitor the new affect monitor
	
	 synchronized void setAffectMonitor(GroupAffectMonitor affectMonitor) {
		disableEmotionDecay();
		disableMoodComputation();
		if (affectMonitor != nullptr) {
			fGroupAffectMonitor = affectMonitor;
			fShowAffectMonitor = true;
			fGroupAffectMonitor.updateEmotionDisplay(fName, fEmotionVector);
			fGroupAffectMonitor.updateMoodDisplay(fName, fEmotionVector,
				fMoodEngine.getEmotionsCenter(),
				fDefaultMood, fCurrentMood,
				fSocialIntegrity, fInSimilarMood,
				fInExtremeMood);
		}
		enableMoodComputation();
		enableEmotionDecay();
	}*/

	/**
	* The
	* <code>MoodComputationTask</code> class manages the compuatation of the
	* actual mood.
	*/
	

	/**
	* Starts emotion decay, if not running.
	*/
	 void enableMoodComputation() {
		fMoodEngine = new MoodEngine(fPersonality, fAc->moodStabilityControlledByNeurotism, fDefaultMood, fAc->moodReturnOverallTime);
		timer_manager::remove_timer(timer_manager::getmood());
		//KillTimer(0, 1);
		std::shared_ptr<MoodComputationTask> bye;
		timer_manager::func_t MooddDecay = [bye](const timer_manager::timer_ptr_t&)
		{
			bye->run();
		};
		timer_manager::add_timer(fAc->moodReturnPeriod, MooddDecay, false, "mood");
		//SetTimer(NULL, 1, fAc->moodReturnPeriod, new MoodComputationTask(this)->run);
		
	}

	/**
	* Stops emotion decay, if running.
	*/
	 void disableMoodComputation() {
		 timer_manager::remove_timer(timer_manager::getmood());
		// KillTimer(0, 1);
	}

	 void addEEC(EEC* eec) {
		fEmotionEngine->addEEC(eec);
	}

	 /**
	 * Sets a new emotion decay function and decaySteps and reinitialize the
	 * character's emotion computation engine.
	 *
	 * @param decayFunction the new emotion decay function
	 */
	 // synchronized 
	 void setDecayFunction(LinearDecayFunction* decayFunction) {
		 std::unique_lock<std::recursive_mutex> lock(_mutex);
		 disableEmotionDecay();
		 fDecayFunction = decayFunction;
		 fDecayFunction->init(fAc->emotionDecaySteps);
		 fEmotionEngine = new EmotionEngine(fPersonality, fDecayFunction);
		 enableEmotionDecay();
	 }

	 void setDecayFunction(ExponentialDecayFunction* decayFunction) {
		 disableEmotionDecay();
		 fDecayFunction = decayFunction;
		 fDecayFunction->init(fAc->emotionDecaySteps);
		 fEmotionEngine = new EmotionEngine(fPersonality, fDecayFunction);
		 enableEmotionDecay();
	 }

	 void setDecayFunction(TangensHyperbolicusDecayFunction* decayFunction) {
		 disableEmotionDecay();
		 fDecayFunction = decayFunction;
		 fDecayFunction->init(fAc->emotionDecaySteps);
		 fEmotionEngine = new EmotionEngine(fPersonality, fDecayFunction);
		 enableEmotionDecay();
	 }

	/**
	* Sets a new personality for this character and reinitialize the character's
	* emotion computation engine.
	*
	* @param personality the new personality
	
	// synchronized 
	 void setPersonality(std::shared_ptr<Personality> personality) {
		disableEmotionDecay();
		disableMoodComputation();
		fPersonality = personality;
		//fEmotionVector = createEmotionVector();
		//fEmotionHistory = new EmotionHistory();

		fEmotionEngine = new EmotionEngine(fPersonality, fDecayFunction);

		// Not for groups! fDefaultMood = getDefaultMood(personality);
		// Not for groups! fCurrentMood = getDefaultMood(personality);
		if ((fGroupAffectMonitor != nullptr) && fShowAffectMonitor) {
			fGroupAffectMonitor.updateEmotionDisplay(fName, fEmotionVector);
			fGroupAffectMonitor.updateMoodDisplay(fName, fEmotionVector,
				fMoodEngine->getEmotionsCenter(),
				fDefaultMood, fCurrentMood,
				fSocialIntegrity, fInSimilarMood,
				fInExtremeMood);
		}
		enableMoodComputation();
		enableEmotionDecay();
	}*/

	
	 //synchronized 
	 void setAffectConsts(AffectConsts* ac) {
		 std::unique_lock<std::recursive_mutex> lock(_mutex);
		disableEmotionDecay();
		disableMoodComputation();
		fAc = ac;
		std::shared_ptr<PersonalityEmotionsRelations> perEmoRels =
			fPersonality.getPersonalityEmotionsRelations();
		perEmoRels->setPersonalityEmotionInfluence(ac->personalityEmotionInfluence);
		perEmoRels->setEmotionMaxBaseline(ac->emotionMaxBaseline);
		fPersonality.setPersonalityEmotionsRelations(perEmoRels);
		fEmotionEngine = new EmotionEngine(fPersonality, fDecayFunction);
		enableMoodComputation();
		enableEmotionDecay();
	}

	/**
	* Infers emotions based on the specified std::list of Emotion Eliciting Conditions
	* and updates the character's emotional state displayed by the affect
	* monitor.
	*
	* @return generated emotions
	*/
	// synchronized 
	 std::shared_ptr<EmotionVector> inferEmotions() {
		 std::unique_lock<std::recursive_mutex> lock(_mutex);
		 std::shared_ptr<EmotionVector> result = createEmotionVector();
		result = fEmotionEngine->inferEmotions(result, fEmotionHistory, fCurrentMood);
		fEmotionEngine->clearEEC();
		fEmotionHistory->add(result);
		fEmotionVector = fEmotionHistory->getEmotionalState(fEmotionVector);
		/*if ((fGroupAffectMonitor != nullptr) && fShowAffectMonitor) {
			fGroupAffectMonitor.updateEmotionDisplay(fName, fEmotionVector);
		}*/
		return result;
	}

	

	 std::vector<std::shared_ptr<CharacterManager>> getCharacters() {
		return fCharacters;
	}

	 bool hasCharacter(std::shared_ptr<CharacterManager> queryCharacter) {
		bool isAGroupCharacter = false;
		for (const auto& character : fCharacters) {
			isAGroupCharacter = (character == queryCharacter) ? true : isAGroupCharacter;
		}
		return isAGroupCharacter;
	}

	bool hasCharacter(std::string characterName) {
		bool isAGroupCharacter = false;
		for (const auto& character : fCharacters) {
			isAGroupCharacter =
				(character->getName()==characterName) ? true : isAGroupCharacter;
		}
		return isAGroupCharacter;
	}

	// synchronized
	double getSocialIntegrity() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		return fSocialIntegrity;
	}

	 //synchronized
	std::string getCharactersInSimilarMood() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		return fInSimilarMood;
	}

	// synchronized was suppose to be seperated by ,
	std::string getCharacterArrayInSimilarMood() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		return fInSimilarMood;
	}

	// synchronized 
	std::vector<std::shared_ptr<CharacterManager>> getCharactersInExtremeMood() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		return fInExtremeMood;
	}

	/**
	* Stops all affect processing, decaying tasks
	*/
	 void stopAll() {
		 timer_manager::remove_timer(timer_manager::getmood());
		 timer_manager::remove_timer(timer_manager::getemo());
		// KillTimer(0, 0);
		// KillTimer(0, 1);
	}

	/**
	* Implements EmotionMaintenancestd::listener
	*/
	// synchronized 
	void maintainEmotion(EmotionMaintenanceEvent e) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		EmotionType changedEmotionType = e.emotionType();
		// only add the emotion to the right character :-)
		if (e.getCharacterName()==fName) {
			std::shared_ptr<EmotionVector> emotions = createEmotionVector();
			std::list<std::shared_ptr<Emotion>> freshDummyEmotions = emotions->getEmotions();
			for (const auto& it : freshDummyEmotions) {
				std::shared_ptr<Emotion> emotion = it;
				EmotionType emotionType = emotion->getType();
				if (changedEmotionType==emotionType) {
					double intensity = (e.intensity() < emotion->getBaseline()) ? emotion->getBaseline() : e.intensity();
					std::shared_ptr<Emotion> newEmotion = std::make_shared<Emotion>(emotionType, intensity, emotion->getBaseline(), "User maintenance");
					emotions->add(newEmotion);
				}
			}
			fEmotionHistory->add(emotions);
			fEmotionVector = fEmotionHistory->getEmotionalState(fEmotionVector);
		}
	}
};