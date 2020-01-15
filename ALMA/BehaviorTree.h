#pragma once

#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <unordered_map>
//#include"AppraisalManager.h"
//#include"Manage\charactermanager.h"
class CharacterManager;
class AppraisalManager;

class Node
{
public:
	Node() = default;
	Node(const Node&) = delete;
	virtual ~Node() {}
	virtual bool run() = 0;
};

class CompositeNode : public Node
{
public:

	virtual ~CompositeNode() {}

	bool run() override
	{
		return true;
	}

	const std::vector<std::unique_ptr<Node>>& getChildren() const
	{
		return children;
	}

	void addChild(std::unique_ptr<Node>& child)
	{
		children.emplace_back(std::move(child));
	}
	/*
	void addChildren(std::initializer_list<std::unique_ptr<Node>&>&& newChildren)
	{
		for (std::unique_ptr<Node> child : newChildren)
		{
			addChild(child);
		}
	}*/
	template <typename CONTAINER>
	void addChildren(CONTAINER& newChildren)
	{
		for (std::unique_ptr<Node>& child : newChildren)
		{
			addChild(child);
		}
	}
protected:
	void childrenShuffle()
	{
		std::random_shuffle(children.begin(), children.end());
	}
private:
	std::vector<std::unique_ptr<Node>> children;
};

class Selector : public CompositeNode
{
public:
	Selector() = default;
	Selector(const int id) : Id(id) {}
	virtual ~Selector() {}

	bool run() override
	{
		for (auto& child : getChildren())
		{
			if (child->run())
				return true;
		}
		return false;
	}
	void setId(const int id)
	{
		Id = id;
	}
	int getId() const 
	{ 
		return Id;
	}
private:
	int Id = 0;
};

class RandomSelector : public CompositeNode
{
	RandomSelector() = default;
	virtual ~RandomSelector() {}

	bool run() override
	{
		childrenShuffle();
		for (auto& child : getChildren())
		{
			if (child->run())
				return true;
		}
		return false;
	}
};

class Sequence : public CompositeNode
{
public:
	Sequence() = default;
	Sequence(const int id) : Id(id) {}
	virtual ~Sequence() {}

	bool run() override
	{
		for (auto& child : getChildren())
		{
			if (!child->run())
				return false;
		}
		return true;
	}
	void setId(const int id)
	{
		Id = id;
	}
	int getId() const
	{
		return Id;
	}
private:
	int Id = 0;
};

class ESelector : public CompositeNode
{
public:
	ESelector() = default;
	ESelector(const int id, std::string Name, CharacterManager* NPC) : Id(id), name(Name), theNPC(NPC){}
	virtual ~ESelector() {}

	/*get the emotion vector
	pull the emotions you want to check from it
	compare the emotions
	take the path that is more dominant*/
	bool run() override;
	
	void setId(const int id)
	{
		Id = id;
	}
	int getId() const
	{
		return Id;
	}
private:
	int Id = 0; 
	std::string name;
	CharacterManager* theNPC;
};

class EmotionAdder : public CompositeNode
{
public:
	EmotionAdder() = default;
	EmotionAdder(const int id, std::vector<std::string> thechoice, std::vector<std::string> trigger,  CharacterManager* NPC, double Intensity=0.0) :
		Id(id), choices(thechoice), triggers(trigger), theNPC(NPC), intensity(Intensity){	}
	virtual ~EmotionAdder() {}

	//change to int choice in parameter list
	bool run() override { return true; }

	bool run(int _playerChoice);
	void setId(const int id)
	{
		Id = id;
	}
	int getId() const
	{
		return Id;
	}
private:
	int Id = 0;
	std::vector<std::string> choices;
	std::vector<std::string> triggers;
	CharacterManager *theNPC;
	int playerChoice;
	double intensity;
	AppraisalManager* appraisalManagerInstance;
	std::unordered_map<std::string, double> intensityMap = {
		{"joy",0.9894736842105263},
		{"distress",0.959349593495935},
		{"happyfor",0.9842105263157895},
		{"gloating",0.9473684210526316},
		{"resentment",0.983739837398374},
		{"pity",0.6504065040650407},
		{"hope",0.8736842105263158},
		{"fear",0.8292682926829268},
		{"shame",0.9430894308943089},
		{"reproach",0.9024390243902439},
		{"liking",0.9421052631578947},
		{"disliiking",0.983739837398374},
		{"gratitude",0.9842105263157895},
		{"anger",0.9186991869918699},
		{"gratification",0.9842105263157895},
		{"remorse",0.9105691056910569},
		{"love",0.9315789473684211},
		{"hate",0.959349593495935},
		{"satisfaction",0.9578947368421053},
		{"relief",0.7736842105263158},
		{"fearsconfirmed",0.937189280982098209},
		{"disappointment",0.967479674796748},
		{"pride",0.8421052631578947},
		{"admiration",0.9263157894736842},
		{"physical",0.0}
	};
};

class PlayerChoice : public CompositeNode
{
public:
	PlayerChoice() = default;
	PlayerChoice(const int id, std::vector<std::string> thechoices) : Id(id), choices(thechoices) {}
	virtual ~PlayerChoice() {}

	bool run() override
	{
		// present choice here
		
		std::cout << "\n Choose an option from below\n\n ";
		for (auto& decision : choices)
		{
			std::cout << decision << "\n\n";
		}
		int choice;
		std::cin >> choice;
		
		//if child is a emotion adder pass the choice down else just run
		Node* temp = nullptr;
		if (choice >= getChildren().size())
			// error
			;

		if(getChildren().size()>1)
			temp = getChildren()[choice-1].get();
		else
			temp = getChildren()[0].get();
		if (dynamic_cast<EmotionAdder*>(temp) != nullptr)
		{
			dynamic_cast<EmotionAdder*>(temp)->run(choice-1);
			return true;
		}

		getChildren()[choice-1]->run();

		
		return true;
	}
	void setId(const int id)
	{
		Id = id;
	}
	int getId() const
	{
		return Id;
	}
private:
	int Id = 0;
	std::vector<std::string> choices;
};

class Action : public CompositeNode
{
public:
	Action() = default;
	Action(const std::string& newName, const std::string& prob, const int myid)
		: output(newName), probabilityOfSuccess(prob), Id(myid) {}
	int getId() { return Id; }
	std::string getProb() { return probabilityOfSuccess; }
private:
	virtual bool run() override
	{
		std::cout << std::endl<< "NPC:" << output << std::endl;
		if (getChildren().empty())
		{
			std::cout << "\n End of the act\n";
			return false;
		}
			
		getChildren()[0]->run();
		return true;
	}

	int Id;
	std::string output;
	std::string probabilityOfSuccess;
};

class Root : public Node
{
public:
	Root() = default;
	bool run() override
	{
		if (child)
			return child->run();
		return false;
	}
private:
	std::unique_ptr<Node> child;
	std::string name;
	void setChild(std::unique_ptr<Node>& newChild)
	{
		child.swap(newChild);
	}

	
	void setName(std::string& Name) { name = Name; }
	
	friend class BehaviorTree;
};

class BehaviorTree
{
public:
	BehaviorTree()
	{
		root = std::make_unique<Root>();
	}

	void setRootChild(std::unique_ptr<Node>& newChild)
	{
		root->setChild(newChild);
	}

	void setName(const std::string& names)
	{
		name = names;
		root->setName(name);
	}
	std::string getName()
	{
		return name;
	}

	Root* getRoot()
	{
		return root.get();
	}

	bool run(float willing) const
	{
		return root->run();
	}
private:
	std::unique_ptr<Root> root;
	std::string name;
};