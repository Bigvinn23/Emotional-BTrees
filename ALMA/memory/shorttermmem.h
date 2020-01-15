#pragma once
#include<string>
#include<list>
#include<map>
using namespace std;


class shorttermmem
{
private:
	std::map<std::string, std::list<std::string>> memmory;
	std::list<std::string> items;

public:
	shorttermmem(){}
	void addToShortMem(std::string mem, std::list<std::string> item)
	{
		memmory.emplace(mem, item);
	}

	void removeFromShortMem(std::string mem)
	{
		memmory.erase(mem);
	}

	std::map<std::string, std::list<std::string >> getAllMem()
	{
		return memmory;
	}

	std::list<std::string > getMem(std::string mem)
	{
		return memmory[mem];
	}
};