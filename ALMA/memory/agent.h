#pragma once
#include"longtermmem.h"
#include"promem.h"
#include"shorttermmem.h"
#include<vector>
#include<iostream>

using namespace std;
class agent
{
private:
	longtermmem longmem;
	shorttermmem shortmem;
	promem mem;
	
	//transfer to long term mem
	void addtolong(std::string mem, std::list<std::string> item)
	{
		longmem.addToLongMem(mem, item);
	}

public:
	agent()
	{

	}

	// using the emotion that is entere
	void addtoshort(std::string mem, std::list<std::string> item, int emotionModifier) 
	{
		shortmem.addToShortMem(mem, item);
		addtolong(mem, item);
	}

		

	
	//copy from long term mem to execute reinforce long term mem by repeating action 
	bool executepro(std::string mems, list<string> commands) 
	{
		
		mem.addTOProMem(mems, longmem.getLongMem(mems));
		if (commands==mem.getMem(mems))
		{
			cout << "i am equal";
			return true;
		}
		else
		{
			cout << "i amnnot equal";
			return false;
		}
	}
	

};