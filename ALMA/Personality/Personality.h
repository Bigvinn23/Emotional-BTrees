#pragma once

#include <string> 
#include <unordered_map>
#include "../Emotions/EmotionType.h"
#include"PersonalityEmotionsRelations.h"
#include <memory>
#include<ctime>
#include <random>

class Personality {
private:
	
	

	/**
	* Constructs a new <code>Personality</code> object with the
	* specified values for each personality trait.
	*
	* @param openness the degree of openness.
	* @param conscientiousness the degree of conscientiousness.
	* @param extraversion the degree of extraversion.
	* @param agreeableness the degree of agreeableness.
	* @param neurotism the degree of neurotism.
	* @param perEmoRels the influcene of personality on emotions
	*
	* @throws IllegalArgumentException if the degree is not a
	*         real number in the interval <code>[-1.0,1.0]</code>.
	*/
public:
	std::shared_ptr<PersonalityEmotionsRelations> fPerEmoRels;
	double openness;
	double conscientiousness;
	double extraversion;
	double agreeableness;
	double neurotism;
//	Personality(Personality&&) = delete;
	Personality()
	{
		std::random_device r;
		std::default_random_engine random_engine(r());
		std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
		srand(time(NULL));
		 
		openness = distribution(random_engine);
		conscientiousness = distribution(random_engine);
		extraversion = distribution(random_engine);
		agreeableness = distribution(random_engine);
		neurotism = distribution(random_engine);
		fPerEmoRels = std::make_shared<PersonalityEmotionsRelations>();
	}

	Personality(const Personality& p)
	{
		fPerEmoRels = p.fPerEmoRels;
		openness = p.openness;
		conscientiousness = p.conscientiousness;
		extraversion = p.extraversion;
		agreeableness = p.agreeableness;
		neurotism = p.neurotism;
	}

	Personality(double Openness, double Conscientiousness,
		double Extraversion, double Agreeableness,
		double Neurotism, std::shared_ptr<PersonalityEmotionsRelations> perEmoRels = std::make_shared<PersonalityEmotionsRelations>()) {
		if (Openness < -1.0 || Openness > 1.0)
			throw std::runtime_error("Invalid degree of openness: " +
				std::to_string(Openness));
		if (Conscientiousness < -1.0 || Conscientiousness > 1.0)
			throw std::runtime_error("Invalid degree of conscientiousness: " +
				std::to_string(Conscientiousness));
		if (Extraversion < -1.0 || Extraversion > 1.0)
			throw std::runtime_error("Invalid degree of extraversion: " +
				std::to_string(Extraversion));
		if (Agreeableness < -1.0 || Agreeableness > 1.0)
			throw std::runtime_error("Invalid degree of agreeableness: " +
				std::to_string(Agreeableness));
		if (Neurotism < -1.0 || Neurotism > 1.0)
			throw std::runtime_error("Invalid degree of neurotism: " +
				std::to_string(Neurotism));
		openness = Openness;
		conscientiousness = Conscientiousness;
		extraversion = Extraversion;
		agreeableness = Agreeableness;
		neurotism = Neurotism;
		fPerEmoRels = perEmoRels;
	}

	void setPersonalityEmotionsRelations(std::shared_ptr<PersonalityEmotionsRelations> perEmoRels) {
		fPerEmoRels = perEmoRels;
	}


	std::shared_ptr<PersonalityEmotionsRelations> getPersonalityEmotionsRelations() {
		return fPerEmoRels;
	}

	const double getEmotionBaseline(EmotionType type) {
	//	return 0.0;
	//	p = Personality(this->openness,this->conscientiousness,this->extraversion,this->agreeableness, this->neurotism);
		return fPerEmoRels->getEmotionBaseline(*this, type);
	}

	const double getEmotionBaseline(const Personality personality, EmotionType type) {
		return fPerEmoRels->getEmotionBaseline(personality, type);
	}

	/**
	* Returns the degree of openness for this personality.
	*
	* @return the degree of openness.
	*/
	double getOpenness() const { return openness; }

	/**
	* Returns the degree of conscientiousness for this personality.
	*
	* @return the degree of conscientiousness.
	*/
	double getConscientiousness() const { return conscientiousness; }

	/**
	* Returns the degree of extraversion for this personality.
	*
	* @return the degree of extraversion.
	*/
	double getExtraversion() const { return extraversion; }

	/**
	* Returns the degree of agreeableness for this personality.
	*
	* @return the degree of agreeableness.
	*/
	double getAgreeableness() const { return agreeableness; }

	/**
	* Returns the degree of neurotism for this personality.
	*
	* @return the degree of neurotism.
	*/
	double getNeurotism() const { return neurotism; }

	/**
	* Returns a concise representation of this personality
	* consisting of the values for each personality trait.
	*
	* @return a string representation of the personality.
	*/
	std::string toString() const {
		return std::to_string(openness) + ", "
			+ std::to_string(conscientiousness) + ", "
			+ std::to_string(extraversion) + ", "
			+ std::to_string(agreeableness) + ", "
			+ std::to_string(neurotism);
	}

	/**
	* Compares this personality to the specified object.
	* The result is <code>true</code> if and only if the argument
	* is a <code>Personality</code> object with the same value
	* for each personality trait.
	*
	* @param o the object to compare this personality against.
	* @return <code>true</code> if the two personalities are equal;
	*         <code>false</code> otherwise.
	*/
	template<class T>
	bool equals(T* o) {
		Personality* p = nullptr;
		if (!(p = dynamic_cast< Personality*>(o)))
			return false;
	   return
		   p->getOpenness() == openness &&
		   p->getConscientiousness() == conscientiousness &&
		   p->getExtraversion() == extraversion &&
		   p->getAgreeableness() == agreeableness &&
		   p->getNeurotism() == neurotism;
	}

	bool operator==(const Personality* p)
	{
		if (p == nullptr)
		{
			return false;
		}

		//figure out how to compare enums
		return
			p->getOpenness() == openness &&
			p->getConscientiousness() == conscientiousness &&
			p->getExtraversion() == extraversion &&
			p->getAgreeableness() == agreeableness &&
			p->getNeurotism() == neurotism;
	}

	/**
	* Returns a hash code value for this object.
	*
	* @return a hash code value for this object.
	*/
	int hashCode() const {
		size_t result = 17;
		result = 37 * result + std::hash<double>()(openness);
		result = 37 * result + std::hash<double>()(conscientiousness);
		result = 37 * result + std::hash<double>()(extraversion);
		result = 37 * result + std::hash<double>()(agreeableness);
		result = 37 * result + std::hash<double>()(neurotism);
		return result;
	}
};