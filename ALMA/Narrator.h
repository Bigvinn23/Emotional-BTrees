#pragma once
#include <iostream>
#include <string>

class Narrator
{
public:
	Narrator() = default;

	void Story(int act)
	{
		std::string story = "";
		switch (act)
		{
			case 1:
			{
				story = "\n The kingdom is under attack and the king has summoned you the great hero to partake in a great quest to save the realm.\nIt is at this point that you introduce your party member\n";
				break;
			}
			case 2:
			{
				story = "\nAfter our heros have departed from the kingdom in order to defeat the evil forces\nAlong the way the NPC has developed feelings for a fellow companion and decides he will share them with the team leader\n";
				break;
			}
			case 3:
			{
				story = "\n Mid way through their quest our heros have encountered a particualarly difficult battle\nThe NPC has found himself in a difficult situation and requires help";
				break;
			}
			case 4:
			{
				story = "\n During a rare break from constant battling our group of heros have gathered around the campfire enjoying themselfs\n";
				break;
			}
			case 5:
			{
				story = "The heros have arrived at the final battle ground and are engaged in battle\nThings take a turn for the worst when the team leader falls into a disadvantage and now requires help";
				break;
			}
			case 11:
			{
				story = "Not shakir is acting now";
					break;
			}
			case 12:
			{
				story = "Not shakir is acting now";
				break;
			}
			case 13:
			{
				story = "Not shakir is acting now";
				break;
			}
			case 14:
			{
				story = "Not shakir is acting now";
				break;
			}
		}
		std::cout << story << std::endl;
	}
};