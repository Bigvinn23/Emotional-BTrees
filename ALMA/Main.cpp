#define no_init_all

#include <iostream>
#include <fstream>
#include "Emotions/Emotion.h"
#include <cstdlib>
#include "Manage/charactermanager.h"
#include"Manage/appraisalrules.h"
#include "Personality/PersonalityMoodRelations.h"
//#include"AppraisalManager.h"
#include "Narrator.h"

#include <vector>  
#include <string>  
#include <stdio.h>  
#include <stdlib.h>

AppraisalRules* addRules(const json& node, CharacterManager* npc)
{
	AppraisalRules* rule = new AppraisalRules();
	AppraisalVariables* variable;
	int counter = 0;
	Desirability* Desire;
	Praiseworthiness* praise;
	Appealingness* appeal;
	Likelihood* likehood;
	Realization* real;
	Likin* like;
	Agency* agent;
	for (auto& child : node["children"])
	{
		if (child["Desirability"].get<double>() != 0)
		{
			Desire = new Desirability(child["Desirability"].get<double>());
		}
		else
			 Desire = nullptr;
		if (child["Praiseworthiness"].get<double>() != 0)
		{
			praise = new Praiseworthiness(child["Praiseworthiness"].get<double>());
		}
		else
			praise = nullptr;
		if (child["Appealingness"].get<double>() != 0)
		{
			appeal = new Appealingness(child["Appealingness"].get<double>());
		}
		else
			appeal = nullptr;
		if (child["Likelihood"].get<double>() != 0)
		{
			likehood = new Likelihood(child["Likelihood"].get<double>());
		}
		else
			likehood = nullptr;
		if (child["Realization"].get<double>() != 0)
		{
			real = new Realization(child["Realization"].get<double>());
		}
		else
		real = nullptr;
		if (child["Likin"].get<double>() != 0)
		{
			like = new Likin(child["Likin"].get<double>());
		}
		else
			like = nullptr;

		if ((child["Agency"].get<std::string>()) == "other" or (child["Agency"].get<std::string>()) == "self")
		{
			agent = new Agency(child["Agency"].get<std::string>());
		}
		else
			agent = nullptr;
			

		variable=  new AppraisalVariables(child["type"].get<std::string>(),
			child["signal"].get<std::string>(),
			Desire,
			praise,
			appeal,
			likehood,
			real,
		    agent,
			like,
			child["signal"].get<std::string>());
		
		rule->add(npc->getName(), child["signal"].get<std::string>(), "Basic", variable);

	}

	
	return rule;
}
// create an ofstream for the file output (see the link on streams for
// more info)


	std::string filename = "./output/-HighPosExtraNoMem.csv";
	std::string filename1 = "./output/HighPosNeuro-Memorycapture.csv";



int main4(int argc, char *argv[])
{
	cout << "Hello World!\nThis is " << argv[0];

	return 0;
}

int main(int argc, char *argv[])
{

	// declare the necessary things
	Personality personality = Personality(0.04, 0.02, 0.05, 0.01, 0.01);//ocean
	AffectConsts* affectconstant = new AffectConsts();
	//	affectconstant->moodStabilityControlledByNeurotism = true;
	DecayFunction* decayfunction = new LinearDecayFunction();

	std::list<EmotionType> emotionlist = { Joy, Distress, HappyFor, Gloating, Resentment, Pity, Hope, Fear,
		Satisfaction, Relief, FearsConfirmed, Disappointment, Pride, Admiration,
		Shame, Reproach, Liking, Disliking, Gratitude, Anger, Gratification,
		Remorse, Love, Hate, Physical };


	// create the charmanager
	string npcName = "john";
	CharacterManager npc(npcName, personality, affectconstant, false, decayfunction, emotionlist);

	json apprasialRules;
	ifstream inputFile{ "ApprasialVariables1.json" };
	if (inputFile.bad() || inputFile.fail())
	{

		cerr << "Failed to open 'AppraisalVariables.json'." << endl;
		exit;
	}
	inputFile >> apprasialRules;
	npc.setAppraisalRules(addRules(apprasialRules, &npc));

	// create the tree(s)
	//npc.createTree("feedback", "ProgrammerFeedback.json");
	npc.createTree("feedback", "Introduction_new_ids.json");

	std::string id = "", endType = "playerchoice";
	int choice;

	id = npc.runTree("feedback", id, endType);

	while (id.compare("") != 0)
	{
		cout << "Choice: ";
		cin >> choice;
		id = npc.runTree("feedback", id, endType, choice);
	}

	// run the tree(s)
	//npc.runTree("feedback");

	system("pause");
	return 0;
}

int main2()
{

	// declare the necessary things
	Personality personality = Personality(0.04, 0.02, 0.05, 0.01, 0.01);//ocean
	AffectConsts* affectconstant = new AffectConsts();
	//	affectconstant->moodStabilityControlledByNeurotism = true;
	DecayFunction* decayfunction = new LinearDecayFunction();

	std::list<EmotionType> emotionlist = { Joy, Distress, HappyFor, Gloating, Resentment, Pity, Hope, Fear,
		Satisfaction, Relief, FearsConfirmed, Disappointment, Pride, Admiration,
		Shame, Reproach, Liking, Disliking, Gratitude, Anger, Gratification,
		Remorse, Love, Hate, Physical };


	// create the charmanager
	string npcName;
	std::cout << "NPC Name: ";
	//std::cin >> npcName; // get npc name as input
	cout << "hi1\n";
	CharacterManager npc(npcName, personality, affectconstant, false, decayfunction, emotionlist);


	json apprasialRules;
	ifstream inputFile{ "ApprasialVariables1.json" };
	if (inputFile.bad() || inputFile.fail())
	{

		cerr << "Failed to open 'AppraisalVariables.json'." << endl;
		exit;
	}
	inputFile >> apprasialRules;
	npc.setAppraisalRules(addRules(apprasialRules, &npc));


	// create the tree(s)
	//npc.createTree("feedback", "ProgrammerFeedback.json");
	npc.createTree("feedback", "Introduction_new_ids.json");

	// run the tree(s)
	npc.runTree("feedback");

	system("pause");
	return 0;
}

int main_old()
{	
	Narrator narrator= Narrator();
	Personality personality = Personality(0.04,0.02,0.05,0.01,0.01);//ocean
	AffectConsts* affectconstant =new AffectConsts();
//	affectconstant->moodStabilityControlledByNeurotism = true;
	DecayFunction* decayfunction = new LinearDecayFunction();

	std::list<EmotionType> emotionlist = { Joy, Distress, HappyFor, Gloating, Resentment, Pity, Hope, Fear,
		Satisfaction, Relief, FearsConfirmed, Disappointment, Pride, Admiration,
		Shame, Reproach, Liking, Disliking, Gratitude, Anger, Gratification,
		Remorse, Love, Hate, Physical };
	
	CharacterManager shakir ("shakir", personality, affectconstant, false, decayfunction, emotionlist);
	CharacterManager notshakir("notshakir", Personality(-0.1, -0.1, -0.1, -0.1, -0.1), affectconstant, false, decayfunction, emotionlist);

	ofstream outputFile;
	outputFile.open(filename);
	// write the file headers
	outputFile << "Emotion" << "," << "Pleasure" << "," << "Arousal" << "," << "Dominance" << "," << "Mood" << "," << "Intensity" << "," << "EmotionPleasure" << "," << "EmotionArousal" << "," << "EmotionDominance" << std::endl;
	outputFile << "None" << "," << shakir.getCurrentMood().getPleasure() << "," << shakir.getCurrentMood().getArousal() << "," << shakir.getCurrentMood().getDominance() << "," << shakir.getCurrentMood().getMoodWord() << "," << shakir.getCurrentMood().getMoodWordIntensity()
		 << std::endl;
	outputFile.close();

	outputFile.open(filename1);
	// write the file headers
	outputFile << "Name" << "," << "Emotion" << "," << "Intensity" <<  std::endl;
	outputFile.close();
	


	json apprasialRules;
	ifstream inputFile{ "ApprasialVariables1.json" };
	if (inputFile.bad() || inputFile.fail())
	{

		cerr << "Failed to open 'AppraisalVariables.json'." << endl;
		exit;
	}
	inputFile >> apprasialRules;
	shakir.setAppraisalRules(addRules(apprasialRules, &shakir));
	notshakir.setAppraisalRules(addRules(apprasialRules, &notshakir));

	shakir.createTree("intro","Introduction.json");
	shakir.createTree("story", "Story.json");
	shakir.createTree("battle", "Battle.json");
	shakir.createTree("feelings", "Feelings.json");
	shakir.createTree("finalbattle", "Finalbattle.json");

	/*notshakir.createTree("intro2", "Introduction2.json");
	notshakir.createTree("battle2", "Battle2.json");
	notshakir.createTree("feeling2", "Feelings2.json");
	notshakir.createTree("story2", "Story2.json");*/
	std::cout << shakir.getCurrentMood().padString() << std::endl;
	std::cout << "Shakir is feeling " << shakir.getCurrentMood().getMoodWord() << std::endl;
	
	narrator.Story(1);
	shakir.runTree("intro");
	/*
	std::cout << notshakir.getCurrentMood().padString() << std::endl;
	std::cout << "Not-Shakir is feeling " << notshakir.getCurrentMood().getMoodWord() << std::endl;
	narrator.Story(11);
	notshakir.runTree("intro2");
*/
	std::cout << shakir.getCurrentMood().padString() << std::endl;
	std::cout << "Shakir is feeling " << shakir.getCurrentMood().getMoodWord() << std::endl;
	narrator.Story(2);
	shakir.runTree("feelings");
	/*
		std::cout << notshakir.getCurrentMood().padString() << std::endl;
	std::cout << "Not-Shakir is feeling " << notshakir.getCurrentMood().getMoodWord() << std::endl;
	narrator.Story(12);
	notshakir.runTree("feeling2");
*/
	std::cout << shakir.getCurrentMood().padString() << std::endl;
	std::cout << "Shakir is feeling " << shakir.getCurrentMood().getMoodWord() << std::endl;
	narrator.Story(3);
	shakir.runTree("battle");
	/*
	std::cout << notshakir.getCurrentMood().padString() << std::endl;
	std::cout << "Not-Shakir is feeling " << notshakir.getCurrentMood().getMoodWord() << std::endl;
	narrator.Story(13);
	notshakir.runTree("battle2");
*/
	std::cout << shakir.getCurrentMood().padString() << std::endl;
	std::cout << "Shakir is feeling " << shakir.getCurrentMood().getMoodWord() << std::endl;
	narrator.Story(4);
	shakir.runTree("story");
	/*
		std::cout << notshakir.getCurrentMood().padString() << std::endl;
	std::cout << "Not-Shakir is feeling " << notshakir.getCurrentMood().getMoodWord() << std::endl;
	narrator.Story(14);
	notshakir.runTree("story2");
*/
	std::cout << shakir.getCurrentMood().padString() << std::endl;
	std::cout << "Shakir is feeling " << shakir.getCurrentMood().getMoodWord() << std::endl;
	narrator.Story(5);
	shakir.runTree("finalbattle");
	/*shakir.addEEC(EEC::appraiseEvent(std::make_shared<Emotion::Elicit>(personality), new Desirability(0.5)));
	std::shared_ptr<EmotionVector> test = shakir.inferEmotions();
	std::shared_ptr<Emotion> hello=test->getDominantEmotion();
	std::thread ever_lasting([]() {
		while (timer_manager::process())
			std::this_thread::yield();
	});
	
	ever_lasting.join();*/
	system("pause");
	return 0;
}