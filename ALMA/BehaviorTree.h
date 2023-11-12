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

/*
I need the behaviour tree to be able to run from an arbitrary node to another arbitrary node
I plan to accomplish this by overloading the run functions for each relevant node (Action, Emotion Adder, E-Selector, Player Choice)
the new run functions will have 2 extra parameters: targetId and endType
	targetId will hold the id of the node to run
	endType will hold the type of node to stop at

the new run functions will act as follows:
	
	check if the node should run (id is targetId or comes after it in the tree)
	if yes:
		check if node is the same type as endType and if node is NOT the targetid (to stop it from running only one node)
		if yes:
			perform run behaviour for end of chain (only has special meaning for the player action nodes)
			return true
		if no: 
			perform normal run behaviour
			determine next node in chain
			return run for next node in chain

	if no:
		
*/

class Node
{
public:
	Node() = default;
	Node(const Node&) = delete;
	virtual ~Node() {}
	virtual bool run() = 0;
	virtual std::string run(std::string targetId, std::string endType, int choice = -1) = 0; // add new run to the node class

	/*
	this function checks whether the 2 ids are equal in terms of tree navigation
	targetId represents the id of the target node, nodeId represents the id of the calling node
	atm it just checks for length because, in theory, the only way the lengths should be the same is if the ids are the same
	the return value is an int because it must convey one of three possible results:
		0 => it is the same node
		>0 => it is not the same node but instead comes after the target node
		<0 => it is not the same node but instead comes before the target node
	*/
	int distance(std::string targetId, std::string nodeId)
	{
		// subtract the target id length from the node id length
		// if the ids are the same the result should be 0
		// if the node comes before the target, its id should be smaller and the result should be negative
		// if the node comes after the target, its id should be larger and the result should be positive
		return nodeId.length() - targetId.length(); 
	}

	/*
	this function gets which child of the current node should be the next one navigated to
	targetId represents the id of the target node, nodeId represents the id of the calling node
	this function assumes the length of targetId is longer than nodeId
	the function works under the following ideas:
		lets say for example that targetId = "R-0-1-2-0-1" and nodeId = "R-0-1-2".
		The next node will be the 1st child of the current node (children[0]) and therefore the function should return 0
		we can accomplish this like so:
			get the length of nodeId, n, here it is 7
			get the location of the next dash(-) in targetId starting from targetId[n+1], m, this gives index position 9
			now subtract n+1 from m to get the length of the child number, x, this gives us 1
			the child number is the substring denoted by substr(n+2, x), this gives us "0"
			cast this to an int an return it
	*/
	int nextNode(std::string targetId, std::string nodeId)
	{
		int nodeIdLength = nodeId.length(); // get n
		int dashLocation = targetId.find("-", nodeIdLength + 1); // get m
		if (dashLocation < 0) { dashLocation = targetId.length(); }
		int childNumLength = dashLocation - (nodeIdLength + 1);
		std::string child = targetId.substr(nodeIdLength + 1, childNumLength);
		return std::stoi(child);
	}
};

class CompositeNode : public Node
{
public:

	virtual ~CompositeNode() {}

	bool run() override
	{
		return true;
	}

	std::string run(std::string targetId, std::string endType, int choice = -1) override // new run in CompositeNode class
	{
		return getChildren()[0]->run(targetId, endType, choice);
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
	Selector(const std::string id) : Id(id) {}
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
	void setId(const std::string id)
	{
		Id = id;
	}
	std::string getId() const 
	{ 
		return Id;
	}
private:
	std::string Id = 0;
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
	Sequence(const std::string id) : Id(id) {}
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
	void setId(const std::string id)
	{
		Id = id;
	}
	std::string getId() const
	{
		return Id;
	}
private:
	std::string Id = 0;
};

class ESelector : public CompositeNode
{
public:
	ESelector() = default;
	ESelector(const std::string id, std::string Name, CharacterManager* NPC) : Id(id), name(Name), theNPC(NPC){}
	virtual ~ESelector() {}

	/*get the emotion vector
	pull the emotions you want to check from it
	compare the emotions
	take the path that is more dominant*/
	bool run() override;

	std::string run(std::string targetId, std::string endType, int choice = -1) override;
	
	void setId(const std::string id)
	{
		Id = id;
	}
	std::string getId() const
	{
		return Id;
	}
private:
	std::string Id = 0; 
	std::string name;
	CharacterManager* theNPC;
};

class EmotionAdder : public CompositeNode
{
public:
	EmotionAdder() = default;
	EmotionAdder(const std::string id, std::vector<std::string> thechoice, std::vector<std::string> trigger,  CharacterManager* NPC, double Intensity=0.0) :
		Id(id), choices(thechoice), triggers(trigger), theNPC(NPC), intensity(Intensity){	}
	virtual ~EmotionAdder() {}

	//change to int choice in parameter list
	bool run() override { return true; }

	bool run(int _playerChoice);

	std::string run(std::string targetId, std::string endType, int choice = -1) override;

	void setId(const std::string id)
	{
		Id = id;
	}
	std::string getId() const
	{
		return Id;
	}
private:
	std::string Id = 0;
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
	PlayerChoice(const std::string id, std::vector<std::string> thechoices) : Id(id), choices(thechoices) {}
	virtual ~PlayerChoice() {}

	bool run() override
	{
		// present choice here
		
		std::cout << "\n Choose an option from below\n\n";
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

	// the new run function for the PlayerChoice node
	virtual std::string run(std::string targetId, std::string endType, int choice = -1) override
	{
		if (choice == -1)
		{
			choice = 0;
		}
		// check if the node is the target (special case for playerchoice)
		// in this case it is assumed the node was targeted to provide the answer to a choice
		// so it proceeds as if it has already gotten it and doesnt display the choices
		if (distance(targetId, Id) == 0)
		{
			std::cout << targetId << "\t" << Id << std::endl;
			//if child is a emotion adder pass the choice down else just run
			Node* temp = nullptr;
			if (choice >= getChildren().size())
				// error
				;

			if (getChildren().size() > 1)
				temp = getChildren()[choice].get();
			else
				temp = getChildren()[0].get();
			if (dynamic_cast<EmotionAdder*>(temp) != nullptr)
			{
				return dynamic_cast<EmotionAdder*>(temp)->run(targetId, endType, choice);
			}

			return getChildren()[choice]->run(targetId, endType, choice);
		}
		// check if node is end of chain (is end type and not the initial target) (special case for player choice)
		// if this is the case treat do not ask for input, just print the choices
		else if (endType.compare("playerchoice") == 0 && distance(targetId, Id) > 0)
		{
			for (auto& decision : choices)
			{
				std::cout << decision << "\n";
			}
			return Id;
		}

		// check if node is end of tree (no children)
		if (getChildren().empty())
		{
			return "";
		}

		// get the next node and run it if it exists
		int next = nextNode(targetId, Id);

		if (next < getChildren().size())
		{
			return getChildren()[next]->run(targetId, endType, choice);
		}
		else
		{
			return "";
		}
	}


	void setId(const std::string id)
	{
		Id = id;
	}
	std::string getId() const
	{
		return Id;
	}
private:
	std::string Id = 0;
	std::vector<std::string> choices;
};

class Action : public CompositeNode
{
public:
	Action() = default;
	Action(const std::string& newName, const std::string& prob, const std::string myid)
		: output(newName), probabilityOfSuccess(prob), Id(myid) {}
	std::string getId() { return Id; }
	std::string getProb() { return probabilityOfSuccess; }

	// the new run function for the Action node
	virtual std::string run(std::string targetId, std::string endType, int choice = -1) override
	{
		int next;
		// check if this node should run (is target or is after target)
		if (distance(targetId, Id) >= 0)
		{
			std::cout << std::endl << output << std::endl;

			// check if node is end of chain (is end type and not the initial target)
			if (endType.compare("action") == 0 && distance(targetId, Id) != 0)
			{
				return Id;
			}

			next = 0;
		}
		else
		{
			// get the next node and run it if it exists
			next = nextNode(targetId, Id);
		}

		// check if node is end of tree (no children)
		if (getChildren().empty())
		{
			return "";
		}

		if (next < getChildren().size())
		{
			return getChildren()[next]->run(targetId, endType, choice);
		}
		else
		{
			return "";
		}
	}


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

	
	std::string Id;
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

	// the new run function for Roots
	// since the root has no meaningful function it doesnt even need to check if it is the target node
	std::string run(std::string targetId, std::string endType, int choice = -1) override
	{
		if (child)
			return child->run(targetId, endType, choice);
		return "";
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