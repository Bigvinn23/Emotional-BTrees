#pragma once
#include"Mood/Mood.h"

struct longTermMem
{
	Mood mood;
	std::string emotion;
	std::string name;
	std::string reaction;
	double intensity;
	std::string trigger;

	longTermMem(std::string emo, std::string Name, std::string Trigger,  Mood theMood, double Intensity)//std::string Reaction,
	{
		mood = theMood;
		emotion = emo;
		name = Name;
		trigger = Trigger;
		//reaction = Reaction;
		intensity = Intensity;
	}

	std::string getName()
	{
		return name;
	}
};

struct shortTermMem
{
	Mood mood;
	std::string emotion;
	std::string name;
	std::string reaction;
	double intensity; 
	std::string trigger;

	shortTermMem(std::string emo, std::string Name,  std::string Trigger,  Mood theMood, double Intensity)//std::string Reaction,
	{
		mood = theMood;
		emotion = emo;
		name = Name;
		//reaction = Reaction;
		intensity = Intensity;
	}

	std::string getName()
	{
		return name;
	}
};