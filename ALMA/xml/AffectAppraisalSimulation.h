#pragma once
#include"EmotionType.h"
#include"MoodType.h"
#include"Mood.h"
#include"EmotionName.h"
#include"MoodWord.h"
#include"AffectInputDocument.h"
#include"AffectManager.h"
#include"EntityManager.h"
/*
import static de.affect.util.Convert.doubleValue;
import static de.affect.util.Convert.prettyPrint;
*/
class AffectAppraisalSimulation {

private:
	static InterfaceHolder affectManager = AffectManager.sInterface;
	 EntityManager fCharacter = nullptr;
	 Timer fInternalAppraisalTimer = nullptr;

	AffectAppraisalSimulation(EntityManager character) {
		AffectManager.sLog.info("\tInternal affect appraisal simulation active!");
		fCharacter = character;
		fInternalAppraisalTimer = new Timer(true);
		fInternalAppraisalTimer.schedule(new InternalAppraisalTask(), 500, 500);
	}

	/**
	* The <code>InternalAppraisalTask</code> class manages the internal appraisal
	* of the dominant emotion and current mood.
	*/
	 class InternalAppraisalTask : TimerTask {

	 private:
		 std::string sUndefinedEmotion = EmotionType::Undefined.tostd::string();
		 std::string lastDominantEmotion = sUndefinedEmotion;
		 std::string sNeutralMood = MoodIntensity::neutral.tostd::string() + MoodType.Neutral.tostd::string();
		 std::string lastMood = sNeutralMood;

		private InternalAppraisalTask() {
		}

	 public:
		 //synchronized
		 void run() {
			// build affect input structures
			Character character = Character.Factory.newInstance();
			character.setName(fCharacter.fName);
			std::string currentEmotion = fCharacter.fEmotionVector.getDominantEmotion().getType().tostd::string();
			double emotionIntensity = fCharacter.fEmotionVector.getDominantEmotion().getIntensity();
			double emotionBaseline = fCharacter.getAffectConsts().emotionMaxBaseline;
			if ((!currentEmotion.equals(sUndefinedEmotion)) && (emotionIntensity > emotionBaseline)) {
				AffectManager.sLog.info(fCharacter.fName + " appraises own emotion " + currentEmotion + " (alma simulation)");
				AffectInput aiInput = AffectInput.Factory.newInstance();
				lastDominantEmotion = currentEmotion;
				EmotionDisplay emotionDisplay = EmotionDisplay.Factory.newInstance();
				//emotionDisplay.setAddressee(addressees);
				//emotionDisplay.setListener(listener);
				emotionDisplay.setType(EmotionName.Enum.forstd::string(currentEmotion));
				emotionDisplay.setIntensity(prettyPrint(emotionIntensity));
				emotionDisplay.setElicitor("alma internal emotion appraisal");
				aiInput.setCharacter(character);
				aiInput.setEmotionDisplay(emotionDisplay);
				affectManager.processSignal(aiInput);
			}
			std::string currentMood = fCharacter.fCurrentMood.getMoodWordIntensity() + fCharacter.fCurrentMood.getMoodWord();
			if (!currentMood.equals(sNeutralMood)) {
				AffectManager.sLog.info(fCharacter.fName + " appraises own mood " + currentMood + " (alma simulation)");
				AffectInput aiInput = AffectInput.Factory.newInstance();
				lastMood = currentMood;
				MoodDisplay moodDisplay = MoodDisplay.Factory.newInstance();
				//moodDisplay.setAddressee(addressees);
				//moodDisplay.setListener(listener);
				moodDisplay.setType(MoodWord.Enum.forstd::string(fCharacter.fCurrentMood.getMoodWord()));
				moodDisplay.setIntensity(prettyPrint(doubleValue(fCharacter.fCurrentMood.getMoodWordIntensity())));
				moodDisplay.setElicitor("alma internal mood appraisal");
				aiInput.setCharacter(character);
				aiInput.setMoodDisplay(moodDisplay);
				affectManager.processSignal(aiInput);
			}
		}
	 }
};