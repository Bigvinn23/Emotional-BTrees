#pragma once
#include<list>
#include<map>
#include<string>
using namespace std;
class promem
{
private:
	std::map<std::string, std::list<std::string>> memmory;
	std::list<std::string> items;

public:
	promem(){}
	void addTOProMem(std::string mem, std::list<std::string> item)
	{
		memmory.emplace(mem, item);
	}

	void removeFromProMem(std::string mem)
	{
		memmory.erase(mem);
	}

	std::list<std::string > getMem(std::string mem)
	{
		return memmory[mem];
	}
};