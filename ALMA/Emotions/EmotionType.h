#pragma once

#include <string>
#include <algorithm>
#include <vector>

enum EmotionType : size_t {
		Undefined = 0, Joy, Distress, HappyFor, Gloating, Resentment, Pity, Hope, Fear,
		Satisfaction, Relief, FearsConfirmed, Disappointment, Pride, Admiration,
		Shame, Reproach, Liking, Disliking, Gratitude, Anger, Gratification,
		Remorse, Love, Hate, Physical
};

namespace std
{
	template<>
	struct hash<EmotionType>
	{
		size_t operator()(const EmotionType k) const
		{
			// Compute individual hash values for two data members and combine them using XOR and bit shifting
			return std::hash<size_t>()(k);
		}
	};
}

static EmotionType getTypeByName(std::string n) {
	EmotionType eType = EmotionType::Undefined;
	int i = 0;
	char c;

	for (char &a : n)
	{
		a = putchar(tolower(a));
	}
	/*while (n[i])
	{
		c = n[i];
		putchar(tolower(c));
		i++;
	}*/
	eType = n.compare("undefined") ? eType: EmotionType::Undefined ;
	eType = n.compare("joy") ?  eType: EmotionType::Joy;
	eType = n.compare("distress") ? eType:EmotionType::Distress  ;
	eType = n.compare("happyfor") ? eType:EmotionType::HappyFor  ;
	eType = n.compare("gloating") ? eType:EmotionType::Gloating  ;
	eType = n.compare("resentment") ?  eType: EmotionType::Resentment ;
	eType = n.compare("pity") ?  eType: EmotionType::Pity;
	eType = n.compare("hope") ? eType: EmotionType::Hope  ;
	eType = n.compare("fear") ?  eType: EmotionType::Fear ;
	eType = n.compare("satisfaction") ?  eType:EmotionType::Satisfaction ;
	eType = n.compare("relief") ?  eType:EmotionType::Relief ;
	eType = n.compare("fearsconfirmed") ?  eType: EmotionType::FearsConfirmed ;
	eType = n.compare("disappointment") ?  eType: EmotionType::Disappointment ;
	eType = n.compare("pride") ?  eType: EmotionType::Pride ;
	eType = n.compare("admiration") ?  eType: EmotionType::Admiration;
	eType = n.compare("shame") ?  eType: EmotionType::Shame;
	eType = n.compare("reproach") ?  eType: EmotionType::Reproach;
	eType = n.compare("liking") ?  eType:EmotionType::Liking ;
	eType = n.compare("disliking") ?  eType:EmotionType::Disliking ;
	eType = n.compare("gratitude") ?  eType:EmotionType::Gratitude ;
	eType = n.compare("anger") ?  eType:EmotionType::Anger ;
	eType = n.compare("gratification") ? eType :EmotionType::Gratification ;
	eType = n.compare("remorse") ?  eType:EmotionType::Remorse ;
	eType = n.compare("love") ?  eType:EmotionType::Love ;
	eType = n.compare("hate") ? eType:EmotionType::Hate  ;
	// physical is a special type of emotion - its p a d values are derived/mesured by bio sensors
	eType = n.compare("physical") ?  eType:EmotionType::Physical ;
	return eType;
}
//rewrite function
/*
static std::string getNames() {
std::string result[1];//check array size issues
std::vector<std::string> nameList = new ArrayList<std::string>();
for (EmotionType:: type : EmotionType::values())
nameList.add(type.toString());
result = nameList.toArray(result);
return result;
}*/

static std::string getCategoryByName(std::string n) {
	std::string cat = "undefined";
	cat = n.compare("undefined") ? "Undefined" : cat;
	cat = n.compare("joy") ? "well-being" : cat;
	cat = n.compare("distress") ? "well-being" : cat;
	cat = n.compare("happyfor") ? "fortunes-of-others" : cat;
	cat = n.compare("gloating") ? "fortunes-of-others" : cat;
	cat = n.compare("resentment") ? "fortunes-of-others" : cat;
	cat = n.compare("pity") ? "fortunes-of-others" : cat;
	cat = n.compare("hope") ? "prospect-based" : cat;
	cat = n.compare("fear") ? "prospect-based" : cat;
	cat = n.compare("satisfaction") ? "prospect-based" : cat;
	cat = n.compare("relief") ? "prospect-based" : cat;
	cat = n.compare("fearsconfirmed") ? "prospect-based" : cat;
	cat = n.compare("disappointment") ? "prospect-based" : cat;
	cat = n.compare("pride") ? "attribution" : cat;
	cat = n.compare("admiration") ? "attribution" : cat;
	cat = n.compare("shame") ? "attribution" : cat;
	cat = n.compare("reproach") ? "attribution" : cat;
	cat = n.compare("liking") ? "attraction" : cat;
	cat = n.compare("disliking") ? "attraction" : cat;
	cat = n.compare("gratitude") ? "well-being/attribution" : cat;
	cat = n.compare("anger") ? "well-being/attribution" : cat;
	cat = n.compare("gratification") ? "well-being/attribution" : cat;
	cat = n.compare("remorse") ? "well-being/attribution" : cat;
	cat = n.compare("love") ? "attraction/attribution" : cat;
	cat = n.compare("hate") ? "attraction/attribution" : cat;
	cat = n.compare("physical") ? "physical" : cat;
	return cat;
}
