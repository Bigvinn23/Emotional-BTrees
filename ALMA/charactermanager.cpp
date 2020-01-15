#pragma once
#include"Manage/charactermanager.h"
#include"Manage/EntityManager.h"
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
#include"AppraisalManager.h"
#include<math.h>
#include "timer_manager.h"
#include"Data/AffectConsts.h"
#include"Emotions/EmotionType.h"
#include"Emotions/PADEmotion.h"
#include<mutex>
#include <Windows.h>
#include "BehaviorTree.h"
#include "json.hpp"

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


ofstream outputFile;
std::string filename1 = "./test/negativemed-Memorycapture.csv";

	
	PersonalityMoodRelations* CharacterManager::getInstance()
	{
		if (instance == nullptr)
		{
			instance = PersonalityMoodRelations::createPersonalityMoodRelations(Mood(), Mood(), Mood(), Mood(), Mood
			(), Personality(), Personality(), Personality());
		}
		return instance;
	}
	PersonalityMoodRelations* CharacterManager::getInstance(Mood OpennessRelation, Mood ConscientiousnessRelation, Mood ExtraversionRelation, Mood AgreeablenessRelation, Mood NeurotismRelation
		, Personality PleasureRelation, Personality ArousalRelation, Personality DominanceRelation)
	{
		if (instance == nullptr)
		{
			instance = PersonalityMoodRelations::createPersonalityMoodRelations(OpennessRelation, ConscientiousnessRelation, ExtraversionRelation, AgreeablenessRelation, NeurotismRelation
				, PleasureRelation, ArousalRelation, DominanceRelation);
		}
		return instance;
	}


	float CharacterManager::random() { return static_cast <float> (rand()) / static_cast <float> (RAND_MAX); }


	unique_ptr<Node> CharacterManager::processBNode(const json& node)
	{
		unique_ptr<Node> result;
		switch (node["type"].get<int>())
		{
		case 0:// Selector
		{
			unique_ptr<Selector> selector = make_unique<Selector>(node["id"].get<int>());
			for (auto& child : node["children"])
			{
				selector->addChild(processBNode(child));
			}
			result = move(selector);
			break;
		}
		case 1:// Sequence
		{
			unique_ptr<Sequence> sequence = make_unique<Sequence>(node["id"].get<int>());
			for (auto& child : node["children"])
			{
				sequence->addChild(processBNode(child));
			}
			result = move(sequence);
			break;
		}
		case 2:// Action
		{
			unique_ptr<Action> action = make_unique<Action>(node["description"].get<string>(),
				node["probabilityOfSuccess"].get<string>(),
				node["id"].get<int>());
			for (auto& child : node["children"])
			{
				action->addChild(processBNode(child));
			}
			result = move(action);
			break;
		}
		case 3:// Eselector
		{

			unique_ptr<ESelector> eSelector = make_unique<ESelector>(node["id"].get<int>(),
				node["name"].get<string>(),
				this);
			for (auto& child : node["children"])
			{
				eSelector->addChild(processBNode(child));
			}
			result = move(eSelector);
			break;
		}
		case 4:// Emotionadder
		{
			std::vector<std::string> thechoices;
			std::vector<std::string> triggers;
			double intens = 0.0;
			if (node["intensity"].get<string>() == "small")
			{
				intens = 0.1;
			}
			else if (node["intensity"].get<string>() == "medium")
			{
				intens = 0.5;
			}
			else
			{
				intens = 1.0;
			}
			for (auto& choice : node["parameters"])
			{
				thechoices.push_back(choice.get<std::string>());
			}
			for (auto& trigger : node["trigger"])
			{
				triggers.push_back(trigger.get<std::string>());
			}

			unique_ptr<EmotionAdder> emotionAdder = make_unique<EmotionAdder>(node["id"].get<int>(),
				thechoices,
				triggers,
				this,
				intens);
			for (auto& child : node["children"])
			{
				emotionAdder->addChild(processBNode(child));
			}
			result = move(emotionAdder);
			break;
		}

		case 5:// playerChoice
		{
			std::vector<std::string> thechoices;
			for (auto& choice : node["parameters"])
			{
				thechoices.push_back(choice.get<std::string>());
			}
			unique_ptr<PlayerChoice> playerChoice = make_unique<PlayerChoice>(node["id"].get<int>(),
				thechoices);
			for (auto& child : node["children"])
			{
				playerChoice->addChild(processBNode(child));
			}
			result = move(playerChoice);
			break;
		}
		}
		return result;
	}

	void CharacterManager::createTree(std::string treename, std::string filename)
	{
		json behaviourFormat;
		ifstream inputFile{ filename };
		if (inputFile.bad())
		{
			cerr << "Failed to open 'OpenDoor.json'." << endl;
			exit;
		}
		inputFile >> behaviourFormat;
		BehaviorTree* Behavetree = new BehaviorTree();

		Behavetree->setName(behaviourFormat["title"]);
		Behavetree->setRootChild(processBNode(behaviourFormat["root"]));
		treeHolder.emplace(treename, Behavetree);
	}

	bool CharacterManager::runTree(std::string treename)
	{
		treeHolder[treename]->getRoot()->run();
		return true;
	}

	void CharacterManager::configSetup()
	{
		json setupFormat;
		ifstream inputFile{ "Setup.json" };
		if (inputFile.bad())
		{
			cerr << "Failed to open 'Setup.json'." << endl;
			exit;
		}
		inputFile >> setupFormat;

		Mood OpennessRelation = Mood(setupFormat["OpennessRelation"][0].get<double>(), setupFormat["OpennessRelation"][1].get<double>(), setupFormat["OpennessRelation"][2].get<double>());
		Mood ConscientiousnessRelation = Mood(setupFormat["ConscientiousnessRelation"][0].get<double>(), setupFormat["ConscientiousnessRelation"][1].get<double>(), setupFormat["ConscientiousnessRelation"][2].get<double>());
		Mood ExtraversionRelation = Mood(setupFormat["ExtraversionRelation"][0].get<double>(), setupFormat["ExtraversionRelation"][1].get<double>(), setupFormat["ExtraversionRelation"][2].get<double>());;
		Mood AgreeablenessRelation = Mood(setupFormat["AgreeablenessRelation"][0].get<double>(), setupFormat["AgreeablenessRelation"][1].get<double>(), setupFormat["AgreeablenessRelation"][2].get<double>());
		Mood NeurotismRelation = Mood(setupFormat["NeurotismRelation"][0].get<double>(), setupFormat["NeurotismRelation"][1].get<double>(), setupFormat["NeurotismRelation"][2].get<double>());
		Personality PleasureRelation = Personality(setupFormat["PleasureRelation"][0].get<double>(), setupFormat["PleasureRelation"][1].get<double>(), setupFormat["PleasureRelation"][2].get<double>(), setupFormat["PleasureRelation"][3].get<double>(), setupFormat["PleasureRelation"][4].get<double>(), fPersEmoRels);
		Personality ArousalRelation = Personality(setupFormat["ArousalRelation"][0].get<double>(), setupFormat["ArousalRelation"][1].get<double>(), setupFormat["ArousalRelation"][2].get<double>(), setupFormat["ArousalRelation"][3].get<double>(), setupFormat["ArousalRelation"][4].get<double>(), fPersEmoRels);
		Personality DominanceRelation = Personality(setupFormat["DominanceRelation"][0].get<double>(), setupFormat["DominanceRelation"][1].get<double>(), setupFormat["DominanceRelation"][2].get<double>(), setupFormat["DominanceRelation"][3].get<double>(), setupFormat["DominanceRelation"][4].get<double>(), fPersEmoRels);
		appraisalManagerInstance = new AppraisalManager();

		instance = getInstance(
			OpennessRelation,
			ConscientiousnessRelation,
			ExtraversionRelation,
			AgreeablenessRelation,
			NeurotismRelation,
			PleasureRelation,
			ArousalRelation,
			DominanceRelation);

		fPersEmoRels->setUp(setupFormat["emotioninfluence"].get<double>(), setupFormat["EmotionMaxBaseline"].get<double>());
		fAc->setUp(
			setupFormat["EmotionMaxBaseline"].get<double>(),
			setupFormat["emotioninfluence"].get<double>(),
			setupFormat["Emotiondecaytime"].get<long>(),
			setupFormat["Emotiondecayperiod"].get<int>(),
			setupFormat["emotionDecaySteps"].get<int>(),
			setupFormat["moodReturnOverallTime"].get<long>(),
			setupFormat["moodReturnPeriod"].get<int>(),
			setupFormat["moodReturnSteps"].get<int>(),
			setupFormat["Moodneurotismstability"].get<bool>()
		);
	}

	CharacterManager::CharacterManager(std::string name, Personality personality,
		AffectConsts* ac, bool derivedPersonality,
		DecayFunction* decayFunction,
		std::list<EmotionType> emotions)
		:EntityManager(name)
	{
		//appraisalManagerInstance = new AppraisalManager();
		srand(static_cast <unsigned> (time(0)));
		memDecayTime = 0.0;
		fCharacterManagerInstance = this;
		fPersonality = personality;
		fAc = ac;
		fDerivedPersonality = derivedPersonality;
		fDecayFunction = decayFunction;
		fAvailEmotions = emotions;
		fPersEmoRels = personality.getPersonalityEmotionsRelations();
		configSetup();
		fDefaultMood = getInstance()->getDefaultMood(personality);
		fCurrentMood = getInstance()->getDefaultMood(personality);
		// Setup emotion processing
		fEmotionVector = createEmotionVector();
		fEmotionHistory = new EmotionHistory();
		fDecayFunction->init(fAc->emotionDecaySteps);
		fEmotionEngine = new EmotionEngine(fPersonality, fDecayFunction);
		//fDecayTimer = new Timer(true);
		//emotionDecayId = Timer::AsyncTimerQueue::Instance().create(1000, true, &new EmotionDecayTask(this)->run);
		std::shared_ptr<EmotionDecayTask> hello(new EmotionDecayTask(this));
		timer_manager::func_t EmoDecay = [hello](const timer_manager::timer_ptr_t&)
		{
			hello->run();
		};
		timer_manager::add_timer(fAc->emotionDecayPeriod, EmoDecay, true, "emo");
		//	SetTimer(NULL,0, fAc->emotionDecayPeriod, new EmotionDecayTask(this)->run);
			//fDecayTimer.schedule(new EmotionDecayTask(this), fAc->emotionDecayPeriod, fAc->emotionDecayPeriod);
			// Setup mood processing	
		fMoodEngine = new MoodEngine(fPersonality, fAc->moodStabilityControlledByNeurotism,
			fDefaultMood, fAc->moodReturnOverallTime);
		std::shared_ptr<MoodComputationTask> bye(new MoodComputationTask(this));
		timer_manager::func_t MooddDecay = [bye](const timer_manager::timer_ptr_t&)
		{
			bye->run();
		};
		timer_manager::add_timer(fAc->moodReturnPeriod, MooddDecay, true, "mood");
		//SetTimer(NULL, 1, fAc->moodReturnPeriod,  new MoodComputationTask(this)->run);
		//fMoodComputationTimer = new Timer(true);
		//fMoodComputationTimer.schedule(new MoodComputationTask(this), fAc->moodReturnPeriod, fAc->moodReturnPeriod);
		// Setup affect monitoring
		/*if (!sIntegratedDesktopMode) {
			//Thread startAffectMonitor = new Thread(){
				//@Override

				 void run() {
				fAffectMonitor = (AffectMonitor) new AffectMonitorFrame(fName, fEmotionVector, fCurrentMood);
				fAffectMonitor.addEmotionMaintenancestd::listener(fCharacterManagerInstance);
			}
			};
			startAffectMonitor.start();*/
	}




	void CharacterManager::sendToMem(std::string emotion, std::string name, std::string Trigger,  Mood theMood, double intensity, std::string memname)//std::string reaction,
	{
		MemoryNames.emplace_back(memname);	
		memory.addShortMemory(emotion, name, Trigger,  theMood, intensity, memname);//reaction,
	}

	void CharacterManager::incermentTime()
	{
		memDecayTime = memDecayTime + 0.5;
		memory.decay(memDecayTime);
	}

	void CharacterManager::emotionAffector(std::string playerName)
	{
		std::string memoryName;
		double intensity = 0.0;
		std::string key;
		bool proceed = false;
		
		//for each mem that the player by the name passed is a part of 
		// compare intensities choose mem with highest intensity 
		for (auto& currentMem : memory.Long)
		{
			
			if (currentMem->name == playerName)
			{
				proceed = true;
				if (fabs(currentMem->intensity) >= fabs(intensity))
				{
					intensity = currentMem->intensity;
					memoryName = currentMem->emotion;
				}
			}
		}
		if (proceed)
		{
			longTermMem* longMem = memory.getMemorybyEmotion(memoryName);
			AppraisalVariables* appvar = new AppraisalVariables();
			switch (getTypeByName(longMem->emotion))
			{
				case EmotionType::Admiration:

				{
					key = "Admiration";
					appvar = getAppraisalVariables(key);
					appvar->setPraiseworthiness(longMem->intensity);
					break;
				}
				case EmotionType::Anger:
				{
					key = "Anger";
					appvar = getAppraisalVariables(key);
					appvar->setPraiseworthiness(-longMem->intensity);
					appvar->setDesirability(-longMem->intensity);
					break;
				}


				case EmotionType::Joy:
				{
					key = "Joy";
					appvar = getAppraisalVariables(key);
					appvar->setDesirability(longMem->intensity);
					break;
				}


				case EmotionType::Distress:
				{
					key = "Distress";
					appvar = getAppraisalVariables(key);
					appvar->setDesirability(-longMem->intensity);
					break;
				}


				case EmotionType::HappyFor:
				{
					key = "HappyFor";
					appvar = getAppraisalVariables(key);
					appvar->setDesirability(longMem->intensity);
					appvar->setLiking(longMem->intensity);
					break;
				}


				case EmotionType::Gloating:
				{
					key = "Gloating";
					appvar = getAppraisalVariables(key);
					appvar->setDesirability(-longMem->intensity);
					appvar->setLiking(longMem->intensity);
					break;
				}


				case EmotionType::Resentment:
				{
					key = "Resentment";
					appvar = getAppraisalVariables(key);
					appvar->setDesirability(longMem->intensity);
					appvar->setLiking(-longMem->intensity);
					break;
				}


				case EmotionType::Pity:
				{
					key = "Pity";
					appvar = getAppraisalVariables(key);
					appvar->setDesirability(-longMem->intensity);
					appvar->setDesirability(-longMem->intensity);
					break;
				}


				case EmotionType::Hope:
				{
					key = "Hope";
					appvar = getAppraisalVariables(key);
					appvar->setDesirability(longMem->intensity);
					appvar->setLikelihood(longMem->intensity);
					break;
				}


				case EmotionType::Fear:
				{
					key = "Fear";
					appvar = getAppraisalVariables(key);
					appvar->setDesirability(-longMem->intensity);
					appvar->setLikelihood(-longMem->intensity);
					break;
				}


				case EmotionType::Satisfaction:
				{
					key = "Satisfaction";
					appvar = getAppraisalVariables(key);
					appvar->setDesirability(longMem->intensity);
					appvar->setRealization(longMem->intensity);
					appvar->setLikelihood(longMem->intensity);
					break;
				}


				case EmotionType::Relief:
				{
					key = "Relief";
					appvar = getAppraisalVariables(key);
					appvar->setDesirability(longMem->intensity);
					appvar->setRealization(longMem->intensity);
					appvar->setLikelihood(longMem->intensity);
					break;
				}


				case EmotionType::FearsConfirmed:
				{
					key = "FearsConfirmed";
					appvar = getAppraisalVariables(key);
					appvar->setDesirability(-longMem->intensity);
					appvar->setRealization(longMem->intensity);
					appvar->setLikelihood(longMem->intensity);
					break;
				}


				case EmotionType::Disappointment:
				{
					key = "Disappointment";
					appvar = getAppraisalVariables(key);
					appvar->setDesirability(-longMem->intensity);
					appvar->setLikelihood(longMem->intensity);
					appvar->setRealization(longMem->intensity);
					break;
				}


				case EmotionType::Pride:
				{
					key = "Pride";
					appvar = getAppraisalVariables(key);
					appvar->setPraiseworthiness(longMem->intensity);
					break;
				}


				case EmotionType::Shame:
				{
					key = "Shame";
					appvar = getAppraisalVariables(key);
					appvar->setPraiseworthiness(-longMem->intensity);
					break;
				}


				case EmotionType::Reproach:
				{
					key = "Reproach";
					appvar = getAppraisalVariables(key);
					appvar->setPraiseworthiness(-longMem->intensity);
					break;
				}


				case EmotionType::Liking:
				{
					key = "Liking";
					appvar = getAppraisalVariables(key);
					appvar->setAppealingness(longMem->intensity);
					break;
				}


				case EmotionType::Disliking:
				{
					key = "Disliking";
					appvar = getAppraisalVariables(key);
					appvar->setAppealingness(-longMem->intensity);
					break;
				}


				case EmotionType::Gratitude:
				{
					key = "Gratitude";
					appvar = getAppraisalVariables(key);
					appvar->setDesirability(longMem->intensity);
					appvar->setPraiseworthiness(longMem->intensity);
					break;
				}


				case EmotionType::Gratification:
				{
					key = "Gratification";
					appvar = getAppraisalVariables(key);
					appvar->setDesirability(longMem->intensity);
					appvar->setPraiseworthiness(longMem->intensity);
					break;
				}


				case EmotionType::Remorse:
				{
					key = "Remorse";
					appvar = getAppraisalVariables(key);
					appvar->setDesirability(-longMem->intensity);
					appvar->setPraiseworthiness(-longMem->intensity);
					break;
				}


				case EmotionType::Love:
				{
					key = "Love";
					appvar = getAppraisalVariables(key);
					appvar->setLiking(longMem->intensity);
					appvar->setPraiseworthiness(longMem->intensity);
					break;
				}


				case EmotionType::Hate:
				{
					key = "Hate";
					appvar = getAppraisalVariables(key);
					appvar->setLiking(-longMem->intensity);
					appvar->setPraiseworthiness(-longMem->intensity);
					break;
				}
			}
			std::shared_ptr<Emotion::Elicit> Elictor = std::make_shared<Emotion::Elicit>(longMem->trigger);
			appraisalManagerInstance->processBasicECC(appvar, this, Elictor);
			computeMood();

			outputFile.open(filename1, ios::out | ios::app);
			//outputFile.open(filename1, ios::out | ios::app);
			outputFile << longMem->name << ","
				<< longMem->emotion << "," << longMem->intensity << "," <<  "," << longMem->reaction << ","  << std::endl;
			outputFile.close();
		}
		
	}
	/**
	* Starts emotion decay, if not running.
	*/
	void CharacterManager::enableEmotionDecay() {
		if (!fAffectComputationPaused) {
			//fDecayTimer.cancel();
			//KillTimer(0, 0);
			timer_manager::remove_timer(timer_manager::getemo());
			std::shared_ptr<EmotionDecayTask> hello;
			timer_manager::func_t EmoDecay = [hello](const timer_manager::timer_ptr_t&)
			{
				hello->run();
			};
			timer_manager::add_timer(fAc->emotionDecayPeriod, EmoDecay, false, "emo");
			//SetTimer(NULL, 0, fAc->emotionDecayPeriod, new EmotionDecayTask(this)->run);;
		}
	}

	/**
	* Starts emotion monitoring, if not running.

	 void enableEmotionMonitoring() {
		fDecayTimer = new Timer(true);
		fDecayTimer.schedule(new EmotionMonitorTask(this), fAc->emotionDecayPeriod, fAc->emotionDecayPeriod);
	}*/

	/**
	* Stops emotion decay, if running.
	*/
	void CharacterManager::disableEmotionDecay() {
		// KillTimer(0, 0);
		timer_manager::remove_timer(timer_manager::getemo());
	}

	/**
	* Returns a flag if affect computation is paused.
	*/
	bool CharacterManager::isAffectComputationPaused() {
		return fAffectComputationPaused;
	}

	/**
	* Pauses all dynamic affect computation
	*
	* @return true if the affect computation is pause, false otherwise
	*/
	bool CharacterManager::pauseAffectComputation() {
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
	bool CharacterManager::resumeAffectComputation() {
		if (fAffectComputationPaused) {
			//	sLog.info(fName + " emotion computation resumed ...");
				//SetTimer(NULL, 0, fAc->emotionDecayPeriod, new EmotionDecayTask(this)->run);
			std::shared_ptr<EmotionDecayTask> hello;
			timer_manager::func_t EmoDecay = [hello](const timer_manager::timer_ptr_t&)
			{
				hello->run();
			};
			timer_manager::add_timer(fAc->emotionDecayPeriod, EmoDecay, false, "emo");
			//	sLog.info(fName + " mood computation resumed ...");
			fMoodEngine = new MoodEngine(fPersonality, fAc->moodStabilityControlledByNeurotism, fDefaultMood, fAc->moodReturnOverallTime);
			//KillTimer(0, 1);
			timer_manager::remove_timer(timer_manager::getmood());
			//SetTimer(NULL, 1, fAc->moodReturnPeriod, new MoodComputationTask(this)->run);
			std::shared_ptr<MoodComputationTask> bye;
			timer_manager::func_t MooddDecay = [bye](const timer_manager::timer_ptr_t&)
			{
				bye->run();
			};
			timer_manager::add_timer(fAc->moodReturnPeriod, MooddDecay, false, "mood");
			fAffectComputationPaused = false;
			return true;
		}
		else {
			return false;
		}
	}

	/**
	* Performs a single step for a paused affect computation (emotion decay and
	* mood computation)
	*
	* @return true if one step affect computation is performed correctly, false
	* otherwise
	*/
	bool CharacterManager::stepwiseAffectComputation() {
		if (fAffectComputationPaused) {

			//timer_manager::process();
				/*
				EmotionDecayTask ed =  EmotionDecayTask(this);
				ed.run();
				MoodComputationTask mc =  MoodComputationTask(this);
				mc.run();

				//sLog.info(AffectManager.sInterface.getCurrentAffect().tostd::string());
				*/
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
	bool CharacterManager::hasActiveAffectMonitor() {
		return fShowAffectMonitor;
	}

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
	void CharacterManager::enableMoodComputation() {
		if (!fAffectComputationPaused) {
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
		}
	}

	/**
	* Stops mood computation, if running.
	*/
	void CharacterManager::disableMoodComputation() {
		// KillTimer(0, 1);
		timer_manager::remove_timer(timer_manager::getmood());
	}

	void CharacterManager::addEEC(EEC* eec) {
		fEmotionEngine->addEEC(eec);

	}

	/**
	* Sets a new emotion decay function and decaySteps and reinitialize the
	* character's emotion computation engine.
	*
	* @param decayFunction the new emotion decay function
	*/
	// synchronized 
	void CharacterManager::setDecayFunction(LinearDecayFunction* decayFunction) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		disableEmotionDecay();
		fDecayFunction = decayFunction;
		fDecayFunction->init(fAc->emotionDecaySteps);
		fEmotionEngine = new EmotionEngine(fPersonality, fDecayFunction);
		enableEmotionDecay();
	}

	void CharacterManager::setDecayFunction(ExponentialDecayFunction* decayFunction) {
		disableEmotionDecay();
		fDecayFunction = decayFunction;
		fDecayFunction->init(fAc->emotionDecaySteps);
		fEmotionEngine = new EmotionEngine(fPersonality, fDecayFunction);
		enableEmotionDecay();
	}

	void CharacterManager::setDecayFunction(TangensHyperbolicusDecayFunction* decayFunction) {
		disableEmotionDecay();
		fDecayFunction = decayFunction;
		fDecayFunction->init(fAc->emotionDecaySteps);
		fEmotionEngine = new EmotionEngine(fPersonality, fDecayFunction);
		enableEmotionDecay();
	}

	/**
	* Returns the character's actual decay function
	*
	* @return the character's actual decay function
	*/
	// synchronized
	DecayFunction* CharacterManager::getDecayFunction() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		return fDecayFunction;
	}

	/**
	* Returns the character's emotion history containing all active emotions
	*
	* @return an EmotionHistory object
	*/
	// synchronized
	EmotionHistory* CharacterManager::getEmotionHistory() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		return fEmotionHistory;
	}

	/**
	* Returns the character's emotion engine
	*
	* @return an EmotionEngine object
	*/
	// synchronized
	EmotionEngine* CharacterManager::getEmotionEngine() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		return fEmotionEngine;
	}

	MoodEngine* CharacterManager::getMoodEngine() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		return fMoodEngine;
	}
	/**
	* Sets a new personality for this character and reinitialize the character's
	* emotion computation engine.
	*
	* @param personality the new personality
	*/
	// synchronized
	void CharacterManager::setPersonality(Personality personality) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		disableEmotionDecay();
		disableMoodComputation();

		fPersonality = personality;
		fEmotionVector = createEmotionVector();
		fEmotionHistory = new EmotionHistory();
		fEmotionEngine = new EmotionEngine(fPersonality, fDecayFunction);
		fDefaultMood = PersonalityMoodRelations::getDefaultMood(personality);
		fCurrentMood = PersonalityMoodRelations::getDefaultMood(personality);

		/*if ((fAffectMonitor != nullptr) && fShowAffectMonitor) {
			fAffectMonitor.updateEmotionDisplay(fName, fEmotionVector);
			fAffectMonitor.updateMoodDisplay(fName, fEmotionVector, fMoodEngine->getEmotionsCenter(),
				fDefaultMood, fCurrentMood, fCurrentMoodTendency);
		}*/

		enableMoodComputation();
		enableEmotionDecay();
	}

	//synchronized 
	void CharacterManager::setAffectConsts(AffectConsts* ac) {
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


	// synchronized
	AffectConsts* CharacterManager::getAffectConsts() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		return fAc;
	}

	/**
	* Infers emotions based on the specified std::list of Emotion Eliciting Conditions
	* and updates the character's emotional state displayed by the affect
	* monitor.
	*
	* @return generated emotions
	*/
	// synchronized
	std::shared_ptr<EmotionVector> CharacterManager::inferEmotions() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		std::shared_ptr<EmotionVector> result = createEmotionVector();
		result = fEmotionEngine->inferEmotions(result, fEmotionHistory, fCurrentMood);
		fEmotionEngine->clearEEC();
		fEmotionHistory->add(result);
		fEmotionVector = fEmotionHistory->getEmotionalState(fEmotionVector);

		/*if ((fAffectMonitor != nullptr) && fShowAffectMonitor) {
			fAffectMonitor.updateEmotionDisplay(fName, fEmotionVector);
			fAffectMonitor.updateMoodDisplay(fName, fEmotionVector, fMoodEngine->getEmotionsCenter(),
				fDefaultMood, fCurrentMood, fCurrentMoodTendency);
		}*/
		return result;
	}

	// synchronized 
	std::shared_ptr<EmotionVector> CharacterManager::infuseBioSignalEmotions(std::shared_ptr<PADEmotion> e) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		if (fAffectComputationPaused) {
			// if the affect computation is paused, return the last active affect output document
			return fEmotionHistory->getEmotionalState(fEmotionVector);
		}

		std::shared_ptr<EmotionVector> result = createEmotionVector();
		result->add(e);
		fEmotionHistory->add(result);
		fEmotionVector = fEmotionHistory->getEmotionalState(fEmotionVector);

		/*if ((fAffectMonitor != nullptr) && fShowAffectMonitor) {
			fAffectMonitor.updateEmotionDisplay(fName, fEmotionVector);
			fAffectMonitor.updateMoodDisplay(fName, fEmotionVector, fMoodEngine->getEmotionsCenter(),
				fDefaultMood, fCurrentMood, fCurrentMoodTendency);
		}*/
		return result;
	}

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

	bool CharacterManager::isDerivedPersonality() {
		return fDerivedPersonality;
	}

	/**
	* Stops all affect processing, decaying tasks
	*/
	void CharacterManager::stopAll() {
		timer_manager::remove_timer(timer_manager::getemo());
		timer_manager::remove_timer(timer_manager::getmood());
		// KillTimer(0, 0);
		// KillTimer(0, 1);
	}

	/**
	* Implements EmotionMaintenancestd::listener
	*/
	void CharacterManager::maintainEmotion(EmotionMaintenanceEvent e) {
		EmotionType changedEmotionType = e.emotionType();

		//sLog.info(fName + " got maintain motion event " + e.tostd::string());
		//sLog.info("\t" + e.getCharacterName());

		// only add the emotion to the right character :-)
		if (e.getCharacterName() == fName) {
			std::shared_ptr<EmotionVector> emotions = createEmotionVector();
			std::list<std::shared_ptr<Emotion>> freshDummyEmotions = emotions->getEmotions();
			for (const auto& it : freshDummyEmotions) {
				EmotionType emotionType = it->getType();
				if (changedEmotionType == emotionType) {
					if (emotionType == EmotionType::Physical) {
						// This is a simulation of pad values derived from physical biosensor data
						double p = (random() - 0.5) * 2;
						double a = (random() - 0.5) * 2;
						double d = (random() - 0.5) * 2;
						std::shared_ptr<PADEmotion> newEmotion = std::make_shared<PADEmotion>(Mood(p, a, d), e.intensity(), "Simulated Bio Sensor Input");
						emotions->add(newEmotion);
					}
					else {
						double intensity = (e.intensity() < it->getBaseline()) ? it->getBaseline() : e.intensity();
						std::shared_ptr<Emotion> newEmotion = std::make_shared<Emotion>(emotionType, intensity, it->getBaseline(), "User maintenance");
						emotions->add(newEmotion);
					}
				}
			}
			fEmotionHistory->add(emotions);
			fEmotionVector = fEmotionHistory->getEmotionalState(fEmotionVector);

			// show elicited emotion(s) in affect monitor even if affect computation is paused
		/*	if ((fAffectMonitor != nullptr) && fShowAffectMonitor) {
				fAffectMonitor.updateEmotionDisplay(fName, fEmotionVector);

				fAffectMonitor.updateMoodDisplay(fName, fEmotionVector, fMoodEngine->getEmotionsCenter(),
					fDefaultMood, fCurrentMood, fCurrentMoodTendency);
			}*/
		}
	}



	void CharacterManager::computeMood()
	{
		fMoodEngine = new MoodEngine(fPersonality, fAc->moodStabilityControlledByNeurotism,
			fDefaultMood, fAc->moodReturnOverallTime);
		std::shared_ptr<MoodComputationTask> bye(new MoodComputationTask(this));
		bye->run();
	}


PersonalityMoodRelations* CharacterManager::instance = nullptr;