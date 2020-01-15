#pragma once
#include <functional>
class EmotionAppraisalVars {

public:
	double desirability = 0.0;
	double praiseworthy = 0.0;
	double appealingness = 0.0;
	double likelihood = 0.0;
	bool realization = 0.0;
	double liking = 0.0;
	bool agency = 0.0;

	EmotionAppraisalVars() {
	}

	void setDesirability(double d) {
		desirability = d;
	}

	void setPraiseworthy(double p) {
		praiseworthy = p;
	}

	void setAppealingness(double a) {
		appealingness = a;
	}

	void setLikelihood(double l) {
		likelihood = l;
	}

	void setLiking(double l) {
		liking = l;
	}

	void setRealization(bool r) {
		realization = r;
	}

	void setAgency(bool a) {
		agency = a;
	}

	bool isSetDesirability() {
		return (desirability == 0.0) ? false : true;
	}

	bool isSetpraiseworthy() {
		return (praiseworthy == 0.0) ? false : true;
	}

	bool isSetAppealingness() {
		return (appealingness == 0.0) ? false : true;
	}

	bool isSetLikelihood() {
		return (likelihood == 0.0) ? false : true;
	}

	bool isSetLiking() {
		return (liking == 0.0) ? false : true;
	}

	bool isSetRealization() {
		return (realization == 0.0) ? false : true;
	}

	bool isSetAgency() {
		return (agency == 0.0) ? false : true;
	}

	double getDesirability() {
		return desirability;
	}

	double getpraiseworthy() {
		return praiseworthy;
	}

	double getAppealingness() {
		return appealingness;
	}

	double getLikelihood() {
		return likelihood;
	}

	double getLiking() {
		return liking;
	}

	bool getRealization() {
		return realization;
	}

	bool getAgency() {
		return agency;
	}

	size_t getHash() const
	{
		return std::hash<double>()(desirability)
			* std::hash<double>()(praiseworthy)
			<< std::hash<double>()(appealingness)
			<< std::hash<double>()(likelihood)
			<< std::hash<double>()(liking)
			* std::hash<bool>()(realization)
			^ std::hash<bool>()(agency);
	}
};

namespace std
{
	template<>
	struct hash<EmotionAppraisalVars>
	{
		size_t operator()(const EmotionAppraisalVars& k) const
		{
			// Compute individual hash values for two data members and combine them using XOR and bit shifting
			return k.getHash();
		}
	};
}