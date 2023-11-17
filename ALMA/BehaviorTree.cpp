#include "BehaviorTree.h"
#include "Manage/charactermanager.h"
#include"AppraisalManager.h"

#include <vector>
#include <string>

//ofstream outputFile;
std::string filename = "./output/HighNegAgreeNoMem.csv";

// the new run function for the ESelector node
std::string ESelector::run(std::string targetId, std::string endType, int choice)
{
	// check if this node should run (is target or is after target)
	if (distance(targetId, Id) >= 0)
	{
		//check NPC mood state
		std::string mood = theNPC->getCurrentMood().getMoodWord();

		//std::cout << theNPC->getCurrentMood().padString() << std::endl;
		//std::cout << "\"npcMood\":\"" << mood << "\"" <<  std::endl;
		//std::cout << "\n";
		if (mood == "Exuberant" or mood == "Dependent" or mood == "Relaxed" or mood == "Docile")
		{
			for (auto& child : getChildren())
			{
				Node* temp = child.get();
				if (dynamic_cast<Action*>(temp)->getProb() == "positive")
				{
					return child->run(targetId, endType, choice);
				}
			}
		}
		else
		{
			for (auto& child : getChildren())
			{
				Node* temp = child.get();
				if (dynamic_cast<Action*>(temp)->getProb() == "negative")
				{
					return child->run(targetId, endType, choice);
				}
			}
		}


		// check if node is end of chain (is end type and not the initial target)
		if (endType.compare("eselector") == 0 && distance(targetId, Id) != 0)
		{
			return Id;
		}
	}


	// check if node is end of tree (no children)
	if (getChildren().empty())
	{
		return "";
	}

	// get the next node and run it if it exists
	int next = nextNode(targetId, Id);

	if (next < getChildren().size())
	{
		return getChildren()[next]->run(targetId, endType, choice);
	}
	else
	{
		return "";
	}
}


bool ESelector::run() 
{
	//check NPC mood state
	std::string mood=theNPC->getCurrentMood().getMoodWord();

	std::cout << theNPC->getCurrentMood().padString() << std::endl;
	std::cout <<theNPC->getName()<< " is feeling " << mood << std::endl;
	std::cout << "\n";
	if (mood == "Exuberant" or mood == "Dependent" or mood == "Relaxed" or mood == "Docile")
	{
		for (auto& child : getChildren())
		{
			Node* temp = child.get();
			if (dynamic_cast<Action*>(temp)->getProb() == "positive")
			{
				child->run();
			}
		}
	}
	else
	{
		for (auto& child : getChildren())
		{
			Node* temp = child.get();
			if (dynamic_cast<Action*>(temp)->getProb() == "negative")
			{
				child->run();
			}
		}
	}
	
	return true;
}

// the new run function for the EmotionAdder node
std::string EmotionAdder::run(std::string targetId, std::string endType, int choice)
{
	int next;
	// check if this node should run (is target or is after target)
	if (distance(targetId, Id) >= 0)
	{
		if (choice == -1)
		{
			choice = 0;
		}

		theNPC->emotionAffector(theNPC->getName());// makes the memmory impact the emotional state of the npc;
		AppraisalVariables* appvar = new AppraisalVariables();
		appraisalManagerInstance = new AppraisalManager();
		double intensityNum = 0.0;
		double total = 0.0;
		std::shared_ptr<Emotion> Emo;
		std::string key = "";
		intensity = intensityMap[choices[choice]];
		std::shared_ptr<Emotion::Elicit> Elictor = std::make_shared<Emotion::Elicit>(triggers[choice]);

		std::cout << "\"emotionAdded\":[\""; // << "Added- ";
		switch (getTypeByName(choices[choice]))
		{
		case EmotionType::Admiration:
		{
			key = "Admiration";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setPraiseworthiness(intensity);
			break;
		}
		case EmotionType::Anger:
		{
			key = "Anger";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setPraiseworthiness(-intensity);
			appvar->setDesirability(-intensity);
			break;
		}


		case EmotionType::Joy:
		{
			key = "Joy";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			break;
		}


		case EmotionType::Distress:
		{
			key = "Distress";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			break;
		}


		case EmotionType::HappyFor:
		{
			key = "HappyFor";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setLiking(intensity);
			break;
		}


		case EmotionType::Gloating:
		{
			key = "Gloating";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			appvar->setLiking(intensity);
			break;
		}


		case EmotionType::Resentment:
		{
			key = "Resentment";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setLiking(-intensity);
			break;
		}


		case EmotionType::Pity:
		{
			key = "Pity";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			appvar->setDesirability(-intensity);
			break;
		}


		case EmotionType::Hope:
		{
			key = "Hope";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setLikelihood(intensity);
			break;
		}


		case EmotionType::Fear:
		{
			key = "Fear";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			appvar->setLikelihood(-intensity);
			break;
		}


		case EmotionType::Satisfaction:
		{
			key = "Satisfaction";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setRealization(intensity);
			appvar->setLikelihood(intensity);
			break;
		}


		case EmotionType::Relief:
		{
			key = "Relief";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setRealization(intensity);
			appvar->setLikelihood(intensity);
			break;
		}


		case EmotionType::FearsConfirmed:
		{
			key = "FearsConfirmed";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			appvar->setRealization(intensity);
			appvar->setLikelihood(intensity);
			break;
		}


		case EmotionType::Disappointment:
		{
			key = "Disappointment";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			appvar->setLikelihood(intensity);
			appvar->setRealization(intensity);
			break;
		}


		case EmotionType::Pride:
		{
			key = "Pride";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setPraiseworthiness(intensity);
			break;
		}


		case EmotionType::Shame:
		{
			key = "Shame";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setPraiseworthiness(-intensity);
			break;
		}


		case EmotionType::Reproach:
		{
			key = "Reproach";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setPraiseworthiness(-intensity);
			break;
		}


		case EmotionType::Liking:
		{
			key = "Liking";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setAppealingness(intensity);
			break;
		}


		case EmotionType::Disliking:
		{
			key = "Disliking";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setAppealingness(-intensity);
			break;
		}


		case EmotionType::Gratitude:
		{
			key = "Gratitude";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setPraiseworthiness(intensity);
			break;
		}


		case EmotionType::Gratification:
		{
			key = "Gratification";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setPraiseworthiness(intensity);
			break;
		}


		case EmotionType::Remorse:
		{
			key = "Remorse";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			appvar->setPraiseworthiness(-intensity);
			break;
		}


		case EmotionType::Love:
		{
			key = "Love";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setLiking(intensity);
			appvar->setPraiseworthiness(intensity);
			break;
		}


		case EmotionType::Hate:
		{
			key = "Hate";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setLiking(-intensity);
			appvar->setPraiseworthiness(-intensity);
			break;
		}
		}
		std::cout << "\"";

		appraisalManagerInstance->processBasicECC(appvar, theNPC, Elictor);
		theNPC->computeMood();
		theNPC->incermentTime();

		std::cout << ",\"";
		switch (getTypeByName(choices[choice]))
		{
		case EmotionType::Admiration:
		{
			key = "Admiration";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setPraiseworthiness(-intensity);
			break;
		}
		case EmotionType::Anger:
		{
			key = "Anger";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setPraiseworthiness(intensity);
			appvar->setDesirability(intensity);
			break;
		}


		case EmotionType::Joy:
		{
			key = "Joy";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			break;
		}


		case EmotionType::Distress:
		{
			key = "Distress";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			break;
		}


		case EmotionType::HappyFor:
		{
			key = "HappyFor";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			appvar->setLiking(-intensity);
			break;
		}


		case EmotionType::Gloating:
		{
			key = "Gloating";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setLiking(intensity);
			break;
		}


		case EmotionType::Resentment:
		{
			key = "Resentment";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			appvar->setLiking(-intensity);
			break;
		}


		case EmotionType::Pity:
		{
			key = "Pity";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setDesirability(intensity);
			break;
		}


		case EmotionType::Hope:
		{
			key = "Hope";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			appvar->setLikelihood(-intensity);
			break;
		}


		case EmotionType::Fear:
		{
			key = "Fear";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setLikelihood(intensity);
			break;
		}


		case EmotionType::Satisfaction:
		{
			key = "Satisfaction";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			appvar->setRealization(-intensity);
			appvar->setLikelihood(-intensity);
			break;
		}


		case EmotionType::Relief:
		{
			key = "Relief";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			appvar->setLikelihood(-intensity);
			break;
		}


		case EmotionType::FearsConfirmed:
		{
			key = "FearsConfirmed";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setRealization(-intensity);
			appvar->setLikelihood(-intensity);
			break;
		}


		case EmotionType::Disappointment:
		{
			key = "Disappointment";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setLikelihood(-intensity);
			appvar->setRealization(-intensity);
			break;
		}


		case EmotionType::Pride:
		{
			key = "Pride";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setPraiseworthiness(-intensity);
			break;
		}


		case EmotionType::Shame:
		{
			key = "Shame";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setPraiseworthiness(intensity);
			break;
		}


		case EmotionType::Reproach:
		{
			key = "Reproach";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setPraiseworthiness(intensity);
			break;
		}


		case EmotionType::Liking:
		{
			key = "Liking";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setAppealingness(-intensity);
			break;
		}


		case EmotionType::Disliking:
		{
			key = "Disliking";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setAppealingness(intensity);
			break;
		}


		case EmotionType::Gratitude:
		{
			key = "Gratitude";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			appvar->setPraiseworthiness(-intensity);
			break;
		}


		case EmotionType::Gratification:
		{
			key = "Gratification";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			appvar->setPraiseworthiness(-intensity);
			break;
		}


		case EmotionType::Remorse:
		{
			key = "Remorse";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setPraiseworthiness(intensity);
			break;
		}


		case EmotionType::Love:
		{
			key = "Love";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setLiking(-intensity);
			appvar->setPraiseworthiness(-intensity);
			break;
		}


		case EmotionType::Hate:
		{
			key = "Hate";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setLiking(intensity);
			appvar->setPraiseworthiness(intensity);
			break;
		}
		}
		std::cout << "\"]" << endl;


		// check if node is end of chain (is end type and not the initial target)
		if (endType.compare("emotionadder") == 0 && distance(targetId, Id) != 0)
		{
			return Id;
		}

		next = 0;
	}
	else
	{
		// get the next node and run it if it exists
		next = nextNode(targetId, Id);
	}


	// check if node is end of tree (no children)
	if (getChildren().empty())
	{
		return "";
	}

	

	if (next < getChildren().size())
	{
		return getChildren()[next]->run(targetId, endType, choice);
	}
	else
	{
		return "";
	}
}

bool EmotionAdder::run(int _playerChoice)
{
	theNPC->emotionAffector(theNPC->getName());// makes the memmory impact the emotional state of the npc;
	AppraisalVariables* appvar = new AppraisalVariables();
	appraisalManagerInstance = new AppraisalManager();
	double intensityNum = 0.0;
	double total = 0.0;
	std::shared_ptr<Emotion> Emo;
	std::string key = "";
	intensity = intensityMap[choices[_playerChoice]];
	std::shared_ptr<Emotion::Elicit> Elictor = std::make_shared<Emotion::Elicit>(triggers[_playerChoice]); 	
	//positive emotions 
	/*if (choices[_playerChoice] == "joy" or choices[_playerChoice] == "happyfor" or choices[_playerChoice] == "hope" or
		choices[_playerChoice] ==  "satisfaction" or choices[_playerChoice] == "relief" or choices[_playerChoice] == "pride" or choices[_playerChoice] == "admiration" or
		choices[_playerChoice] ==  "liking" or choices[_playerChoice] ==  "gratitude" or choices[_playerChoice] == "gratification" or choices[_playerChoice] == "love")
	{
		total = theNPC->getPersonality().agreeableness / 2 + theNPC->getPersonality().conscientiousness / 2 +theNPC->getPersonality().extraversion; + theNPC->getPersonality().openness / 2;
		intensity = intensityMap[choices[_playerChoice]] * total;
		if (intensity <= 0)
			intensity = 0.001;
		if (intensity > 1)
			intensity = 1;
	}
	else
	{
		double total = -theNPC->getPersonality().agreeableness/2 + (-theNPC->getPersonality().conscientiousness/2) + theNPC->getPersonality().neurotism + theNPC->getPersonality().openness/2;
		intensity = intensityMap[choices[_playerChoice]] * total;
		if (intensity <= 0)
			intensity = 0.001;
		if (intensity > 1)
			intensity = 1;
	}*/

	std::cout << "Added- ";
	switch (getTypeByName(choices[_playerChoice]))
	{
		case EmotionType::Admiration :
		{
			key = "Admiration";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setPraiseworthiness(intensity);
		break;
		}
		case EmotionType::Anger:
		{
			key = "Anger";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setPraiseworthiness(-intensity);
			appvar->setDesirability(-intensity);
			break;
		}
		

		case EmotionType::Joy:
		{
			key = "Joy";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			break;
		}
			

		case EmotionType::Distress:
		{
			key = "Distress";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			break;
		}
			

		case EmotionType::HappyFor:
		{
			key = "HappyFor";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setLiking(intensity);
			break;
		}
			

		case EmotionType::Gloating:
		{
			key = "Gloating";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			appvar->setLiking(intensity);
			break;
		}
			

		case EmotionType::Resentment:
		{
			key = "Resentment";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setLiking(-intensity);
			break;
		}
			

		case EmotionType::Pity:
		{
			key = "Pity";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			appvar->setDesirability(-intensity);
			break;
		}
			

		case EmotionType::Hope:
		{
			key = "Hope";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setLikelihood(intensity);
			break;
		}
			

		case EmotionType::Fear:
		{
			key = "Fear";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			appvar->setLikelihood(-intensity);
			break;
		}
			

		case EmotionType::Satisfaction:
		{
			key = "Satisfaction";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setRealization(intensity);
			appvar->setLikelihood(intensity);
			break;
		}
			

		case EmotionType::Relief:
		{
			key = "Relief";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setRealization(intensity);
			appvar->setLikelihood(intensity);
			break;
		}
			

		case EmotionType::FearsConfirmed:
		{
			key = "FearsConfirmed";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			appvar->setRealization(intensity);
			appvar->setLikelihood(intensity);
			break;
		}
			

		case EmotionType::Disappointment:
		{
			key = "Disappointment";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			appvar->setLikelihood(intensity);
			appvar->setRealization(intensity);
			break;
		}
			

		case EmotionType::Pride:
		{
			key = "Pride";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setPraiseworthiness(intensity);
			break;
		}
			

		case EmotionType::Shame:
		{
			key = "Shame";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setPraiseworthiness(-intensity);
			break;
		}
			

		case EmotionType::Reproach:
		{
			key = "Reproach";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setPraiseworthiness(-intensity);
			break;
		}
			

		case EmotionType::Liking:
		{
			key = "Liking";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setAppealingness(intensity);
			break;
		}
			

		case EmotionType::Disliking:
		{
			key = "Disliking";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setAppealingness(-intensity);
			break;
		}
			

		case EmotionType::Gratitude:
		{
			key = "Gratitude";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setPraiseworthiness(intensity);
			break;
		}
			

		case EmotionType::Gratification:
		{
			key = "Gratification";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(intensity);
			appvar->setPraiseworthiness(intensity);
			break;
		}
			

		case EmotionType::Remorse:
		{
			key = "Remorse";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setDesirability(-intensity);
			appvar->setPraiseworthiness(-intensity);
			break;
		}
			

		case EmotionType::Love:
		{
			key = "Love";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setLiking(intensity);
			appvar->setPraiseworthiness(intensity);
			break;
		}
			

		case EmotionType::Hate:
		{
			key = "Hate";
			appvar = theNPC->getAppraisalVariables(key);
			appvar->setLiking(-intensity);
			appvar->setPraiseworthiness(-intensity);
			break;
		}
	}
	std::cout << endl;
	
	appraisalManagerInstance->processBasicECC(appvar, theNPC, Elictor);
	theNPC->computeMood();
	theNPC->incermentTime();
	// sending to memory 
	//theNPC->sendToMem(key,theNPC->getName(), triggers[_playerChoice], theNPC->getCurrentMood(), intensity, triggers[_playerChoice]);
	
	//outputFile.open(filename, ios::out | ios::app);

	//outputFile << choices[_playerChoice] << ","
	//	<< theNPC->getCurrentMood().getPleasure() << "," << theNPC->getCurrentMood().getArousal() << "," 
	//	<< theNPC->getCurrentMood().getDominance() << "," << theNPC->getCurrentMood().getMoodWord() << "," 
	//	<< theNPC->getCurrentMood().getMoodWordIntensity() << "," << theNPC->getMoodEngine()->getEmotionsCenter()->getPValue() << "," 
	//	<< theNPC->getMoodEngine()->getEmotionsCenter()->getAValue() << "," << theNPC->getMoodEngine()->getEmotionsCenter()->getDValue() << std::endl;
	//outputFile.close();
	theNPC->getMoodEngine()->getEmotionsCenter()->getPValue();
	
	switch (getTypeByName(choices[_playerChoice]))
	{
	case EmotionType::Admiration:
	{
		key = "Admiration";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setPraiseworthiness(-intensity);
		break;
	}
	case EmotionType::Anger:
	{
		key = "Anger";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setPraiseworthiness(intensity);
		appvar->setDesirability(intensity);
		break;
	}


	case EmotionType::Joy:
	{
		key = "Joy";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setDesirability(-intensity);
		break;
	}


	case EmotionType::Distress:
	{
		key = "Distress";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setDesirability(intensity);
		break;
	}


	case EmotionType::HappyFor:
	{
		key = "HappyFor";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setDesirability(-intensity);
		appvar->setLiking(-intensity);
		break;
	}


	case EmotionType::Gloating:
	{
		key = "Gloating";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setDesirability(intensity);
		appvar->setLiking(intensity);
		break;
	}


	case EmotionType::Resentment:
	{
		key = "Resentment";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setDesirability(-intensity);
		appvar->setLiking(-intensity);
		break;
	}


	case EmotionType::Pity:
	{
		key = "Pity";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setDesirability(intensity);
		appvar->setDesirability(intensity);
		break;
	}


	case EmotionType::Hope:
	{
		key = "Hope";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setDesirability(-intensity);
		appvar->setLikelihood(-intensity);
		break;
	}


	case EmotionType::Fear:
	{
		key = "Fear";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setDesirability(intensity);
		appvar->setLikelihood(intensity);
		break;
	}


	case EmotionType::Satisfaction:
	{
		key = "Satisfaction";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setDesirability(-intensity);
		appvar->setRealization(-intensity);
		appvar->setLikelihood(-intensity);
		break;
	}


	case EmotionType::Relief:
	{
		key = "Relief";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setDesirability(-intensity);
		appvar->setLikelihood(-intensity);
		break;
	}


	case EmotionType::FearsConfirmed:
	{
		key = "FearsConfirmed";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setDesirability(intensity);
		appvar->setRealization(-intensity);
		appvar->setLikelihood(-intensity);
		break;
	}


	case EmotionType::Disappointment:
	{
		key = "Disappointment";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setDesirability(intensity);
		appvar->setLikelihood(-intensity);
		appvar->setRealization(-intensity);
		break;
	}


	case EmotionType::Pride:
	{
		key = "Pride";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setPraiseworthiness(-intensity);
		break;
	}


	case EmotionType::Shame:
	{
		key = "Shame";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setPraiseworthiness(intensity);
		break;
	}


	case EmotionType::Reproach:
	{
		key = "Reproach";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setPraiseworthiness(intensity);
		break;
	}


	case EmotionType::Liking:
	{
		key = "Liking";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setAppealingness(-intensity);
		break;
	}


	case EmotionType::Disliking:
	{
		key = "Disliking";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setAppealingness(intensity);
		break;
	}


	case EmotionType::Gratitude:
	{
		key = "Gratitude";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setDesirability(-intensity);
		appvar->setPraiseworthiness(-intensity);
		break;
	}


	case EmotionType::Gratification:
	{
		key = "Gratification";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setDesirability(-intensity);
		appvar->setPraiseworthiness(-intensity);
		break;
	}


	case EmotionType::Remorse:
	{
		key = "Remorse";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setDesirability(intensity);
		appvar->setPraiseworthiness(intensity);
		break;
	}


	case EmotionType::Love:
	{
		key = "Love";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setLiking(-intensity);
		appvar->setPraiseworthiness(-intensity);
		break;
	}


	case EmotionType::Hate:
	{
		key = "Hate";
		appvar = theNPC->getAppraisalVariables(key);
		appvar->setLiking(intensity);
		appvar->setPraiseworthiness(intensity);
		break;
	}
	}
	getChildren()[0]->run();	
	return true;
}


