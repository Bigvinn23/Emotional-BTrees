#pragma once
#include"Apprasial/ApprasialVariables.h"
#include <unordered_map>
/**
* The class<code> AppraisalRules</code> holds all appraisal rules of one
* entity.
*
* @author Patrick Gebhard
*
* @version 1.0
*/
class RuleSet {
public:
std::unordered_map<std::string, std::string> types;
		
std::unordered_map<std::string, AppraisalVariables*> rules;


		RuleSet() {	}
	};

class AppraisalRules {

private:
	std::unordered_map<std::string, RuleSet*> entityRelatedRules;

	
	
public:
	AppraisalRules() { entityRelatedRules = std::unordered_map<std::string, RuleSet*>(); }

	 

	 void add(std::string entity, std::string key, std::string type, AppraisalVariables appraisalVariables[]) {
		 RuleSet* ruleSet= nullptr;
		 //= entityRelatedRules.at(entity);
		if (ruleSet == nullptr) { // add a new rule set for a new entity
			entityRelatedRules.emplace(entity, new RuleSet());
			ruleSet = entityRelatedRules.at(entity);
		}
		ruleSet->types.emplace(key, type);
		ruleSet->rules.emplace(key, appraisalVariables);
	}

	 void removeEntityRelatedRules(std::string entity) {
		entityRelatedRules.erase(entity);
	}

	 void removeKey(std::string entity, std::string key) {
		RuleSet* ruleSet = entityRelatedRules.at(entity);
		if (ruleSet != nullptr) {
			ruleSet->types.erase(key);
			ruleSet->rules.erase(key);
		}
	}

	AppraisalVariables* getAppraisalVariables(std::string entity, std::string key) {
		RuleSet* ruleSet = entityRelatedRules.at(entity);
		return (ruleSet == nullptr) ? nullptr : ruleSet->rules.at(key);
	}

	 AppraisalRules getAppraisalRulesByType(std::string entity, std::string type) {
		AppraisalRules ruleSubSet =  AppraisalRules();
		RuleSet* ruleSet = entityRelatedRules.at(entity);
		if (ruleSet != nullptr)
			for (const auto& e : ruleSet->types) {
				std::string key = e.first;
				if (type==(ruleSet->types.at(key)))
					ruleSubSet.add(entity, key, type, ruleSet->rules.at(key));
			}
		return ruleSubSet;
	}

	 std::string* getKeys(std::string entity) {
		 std::string* a;
		std::vector<std::string> keyList;
		RuleSet* ruleSet = entityRelatedRules.at(entity);
		if (ruleSet != nullptr) {
			for (const auto& e : ruleSet->rules)
				keyList.push_back(e.first);
			//std::string result[]= keyList;
			return keyList.data();
		}
		return  a;
	}

	 std::string tostring(std::string entity) {
		 std::string sb;
		RuleSet* ruleSet = entityRelatedRules.at(entity);
		if (ruleSet != nullptr) {
			for (const auto& keyEnum : ruleSet->rules) {
				std::string key = keyEnum.first;
				sb.append(key).append("\n");
				for (const auto av : ruleSet->rules) {
					sb.append(av.first).append("\n");
				}
			}
		}
		return sb;
	}

};