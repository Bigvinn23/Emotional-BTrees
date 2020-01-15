#pragma once
#include<map>
#include<list>
#include<string>
#include<time.h>
#include<iostream>
using namespace std;
class longtermmem{

private:
	std::map<std::string, std::list<std::string>> LongTermMem;

public:
	longtermmem(){}

	void addToLongMem(std::string mem, std::list<std::string> item)
	{
		if (LongTermMem.empty() )
		{
			list<string> hello;
			LongTermMem.emplace(mem, hello);
		}
		srand(time(NULL));
		for (const auto& iv : item)
		{
			if (((rand() % 2) + 1)<2)
			{
				LongTermMem.at(mem).emplace_back(iv);
				cout << iv;
			}

		}
		if (LongTermMem.at(mem) == item)
		{
			cout << "Perfectly Learned";
		}
		else
		{
			cout << "Something went wrong";
		}
		system("pause");
		//LongTermMem.emplace(mem, item);
	}

	void removeFromLongMem(std::string mem)
	{
		LongTermMem.erase(mem);
	}

	std::list<std::string> getLongMem(std::string mem)
	{
		return LongTermMem[mem];
	}

	
	//decay a LongTermMem 
	void decayLongMem()
	{

	}
};