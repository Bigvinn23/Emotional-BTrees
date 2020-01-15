#pragma once
#include"Emotions/emotion.h"
#include"Mood/Mood.h"
#include"Personality/Personality.h"
#include"Emotions/EmotionType.h"
#include <unordered_map>
#include "MemStructs.h"
#include "Apprasial/apprasialvariables.h"
#include <vector>

class Memory
{
private:
	
public:

	typedef EmotionType Type;
   
	
	std::vector<shortTermMem*> Short;
	std::vector<longTermMem*> Long;

	void addShortMemory(std::string emo, std::string Name, std::string Trigger, Mood theMood, double intensity, std::string memname)// std::string Reaction,
	{
		Short.emplace_back( new shortTermMem(emo, Name,Trigger,  theMood, intensity));		// find a way to name differciate between the memories Reaction,
		addLongMemory(emo, Name, Trigger, theMood, intensity, memname);
	}
	
	void addLongMemory(std::string emo, std::string Name, std::string Trigger,  Mood theMood, double intensity, std::string memname)//std::string Reaction,
	{
		Long.emplace_back(new longTermMem(emo, Name, Trigger,  theMood, intensity));//Reaction,
	}

	void removeShortTermMemory(std::string key)
	{
		for (int i = 0; i < Short.size(); i++)
		{
			if (Short[i]->name == key)
				Short.erase(Short.begin() + i);
		}
	}

	void removeLongTermMemory(std::string key)
	{
		for (int i = 0; i < Long.size(); i++)
		{
			if (Long[i]->name == key)
				Long.erase(Long.begin()+i);
		}
	}
	longTermMem* getMemorybyEmotion(std::string emo)
	{
		for (int i = 0; i < Long.size(); i++)
		{
			if (Long[i]->emotion == emo)
				return Long[i];
		}
	}

	longTermMem* getMemorybyName(std::string name)
	{
		for(int i = 0; i<Long.size(); i++)
		{
			if (Long[i]->name == name)
				return Long[i];
		}
	}

	void decay( double time)
	{
		for (int i = 0; i < Long.size(); i++)
		{
			if (Long[i]->intensity > 0)
			{
				double temp = Long[i]->intensity* pow(1 + time/(Long.size() - i), 0.2);
				if (temp <= 0.0)
					Long[i]->intensity = 0.000000001;
				else
					Long[i]->intensity = temp;
			}
			else if (Long[i]->intensity < 0)
			{
				double temp = Long[i]->intensity* pow(1 + time/(Long.size() - i), -0.2);
				if (temp >= 0)
					Long[i]->intensity = -0.000000001;
				else
					Long[i]->intensity = temp;
			}

		}
		
	}
};