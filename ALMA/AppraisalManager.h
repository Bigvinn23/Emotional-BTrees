#ifndef AppraisalManager_H
#define AppraisalManager_H
#include"Apprasial\EEC.h"
#include"Apprasial\apprasialvariables.h"
#include "Emotions\PADEmotion.h"
#include"Mood\Mood.h"
#include<mutex>
#include"Manage/charactermanager.h"
#include "Emotions\emotion.h"


class AppraisalManager
{
private:
	std::recursive_mutex _mutex;

	void appraise(CharacterManager* character, AppraisalVariables* appVars,
		double intensityCorrection, std::shared_ptr<Emotion::Elicit> currentElicitor);

	
	
public:

	AppraisalManager();

	void processBasicECC(AppraisalVariables* appVar, CharacterManager *performer, std::shared_ptr<Emotion::Elicit> elicitor);


	void processPADInput(CharacterManager* performer, Mood m, double intensity, std::string description);
};
#endif