#pragma once
#include"Mood/Mood.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct longTermMem
{
	Mood mood;
	std::string emotion;
	std::string name;
	std::string reaction;
	double intensity;
	std::string trigger;

	friend ostream& operator<<(ostream& out, const longTermMem& m)
	{
		return m.write(out);
	}

	friend istream& operator>>(istream& in, longTermMem& m)
	{
		return m.read(in);
	}

	ostream& write(ostream& out) const
	{
		mood.write(out);
		out << emotion << "\n"
			<< name << "\n"
			<< reaction << "\n"
			<< intensity << "\n"
			<< trigger << "\n";
		return out;
	}

	istream& read(istream& in)
	{
		mood.read(in);
		string s;
		getline(in, emotion);
		getline(in, name);
		getline(in, reaction);
		getline(in, s);
		intensity = stod(s);
		getline(in, trigger);
		in.ignore(numeric_limits<streamsize>::max(), '\n');
		return in;
	}

	longTermMem()
	{
		mood = *new Mood();
		emotion = "";
		name = "";
		reaction = "";
		intensity = 0.0;
		trigger = "";
	}


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

	friend ostream& operator<<(ostream& out, const shortTermMem& m)
	{
		return m.write(out);
	}

	friend istream& operator>>(istream& in, shortTermMem& m)
	{
		return m.read(in);
	}

	ostream& write(ostream& out) const
	{
		//mood.write(out);
		out << mood;
		out << emotion << "\n"
			<< name << "\n"
			<< reaction << "\n"
			<< intensity << "\n"
			<< trigger << "\n";
		return out;
	}

	istream& read(istream& in)
	{
		//mood.read(in);
		in >> mood;
		string s;
		getline(in, emotion);
		getline(in, name);
		getline(in, reaction);
		getline(in, s);
		intensity = stod(s);
		getline(in, trigger);
		in.ignore(numeric_limits<streamsize>::max(), '\n');
		return in;
	}

	shortTermMem()
	{
		mood = *new Mood();
		emotion = "";
		name = "";
		reaction = "";
		intensity = 0.0;
		trigger = "";
	}

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