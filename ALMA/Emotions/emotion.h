#pragma once

#include<string>
#include"EmotionType.h"
#include"EmotionAppraisalVars.h"
#include"../Mood/Mood.h"
#include <stdexcept>
#include <chrono>
#include <memory>
#include "../Personality/Personality.h"

class Emotion {

public:

	enum class ElicitType
	{
		Unknown,
		String,
		Emotion,
		Personality
	};

	struct Elicit
	{
		ElicitType Type;
		std::string string;
		std::shared_ptr<Emotion> Emo;
		Personality Pre;
		Elicit()
		{
			Type = ElicitType::Unknown;
		}
		Elicit(const std::string& str)
		{
			string = str;
			Type = ElicitType::String;
		}
		Elicit(std::shared_ptr<Emotion>& val)
		{
			/*MAKE SHARED*/
		//	Emo = std::move(val);
			Emo = val;
			Type = ElicitType::Emotion;
		}
		Elicit(Personality& val)
		{
			/*MAKE THIS SHARED*/
		//	 Pre = std::move(val);
			Pre = val;
			Type = ElicitType::Personality;
		}	

		//Elicit(const Elicit&) = delete;

		bool operator==(std::shared_ptr<Elicit> e)
		{
			return Type == e->Type
				&& Emo->operator==(e->Emo)
				&& Pre.equals(&e->Pre);
		}
	};

	typedef EmotionType Type;

	const std::vector<std::string> TypeNames = {
		"Undefined", "Joy", "Distress", "HappyFor", "Gloating", "Resentment", "Pity", "Hope", "Fear",
		"Satisfaction", "Relief", "FearsConfirmed", "Disappointment", "Pride", "Admiration",
		"Shame", "Reproach", "Liking", "Disliking", "Gratitude", "Anger", "Gratification",
		"Remorse", "Love", "Hate", "Physical"
	};

	Type type;

	EmotionAppraisalVars mApraisalVars;
	double intensity;
	double baseline;
	long double mTimeOfElicitation;
	long double mCalculatedEndTime;
	long double  mCalculatedDuration;
	std::shared_ptr<Elicit> elicitor;

protected:
	Mood mPADValues;
	/**
	* <code>Emotion</code> object with the specified type and the specified
	* Constructs a new
	* intensity which is associated with an emotion-inducing situation.
	*
	* @param type the type of the emotion.
	* @param intensity the intensity of the emotion.
	* @param baseline the baseline of the emotion.
	* @param elicitor the emotion-inducing situation.
	* @throws IllegalArgumentException if the intensity or the baseline is not a
	* real number in the interval <code>[0.0,1.0]</code> or if the intensity is
	* less than the baseline.
	*/

public:
	Emotion(){}

	Emotion(Type Type, double Intensity, double Baseline,  std::shared_ptr<Elicit> Elicitor)
	{
		if (Intensity < 0.0 || Intensity > 1.0) {
			throw std::invalid_argument("Invalid intensity ");
		}

		if (Baseline < 0.0 || Baseline > 1.0) {
			throw std::invalid_argument("Invalid baseline ");
		}
		if (Intensity < Baseline) {
			throw std::invalid_argument("Error: intensity ");
		}
		mTimeOfElicitation = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		type = Type;
		intensity = Intensity;
		baseline = Baseline;
		elicitor = Elicitor;
	}

	Emotion(Type Type, double Intensity, double Baseline, std::string Elicitor)
		: elicitor(new Elicit(Elicitor))
	{
		if (Intensity < 0.0 || Intensity > 1.0) {
			throw std::invalid_argument("Invalid intensity ");
		}
		if (Baseline < 0.0 || Baseline > 1.0) {
			throw std::invalid_argument("Invalid baseline ");
		}
		if (Intensity < Baseline) {
			throw std::invalid_argument("Error: intensity ");
		}
		mTimeOfElicitation = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		type = Type;
		intensity = Intensity;
		baseline = Baseline;
	}
	Emotion(Type Type, double Intensity, double Baseline, Personality Elicitor)
		: elicitor(new Elicit(Elicitor))
	{
		if (Intensity < 0.0 || Intensity > 1.0) {
			throw std::invalid_argument("Invalid intensity ");
		}
		if (Baseline < 0.0 || Baseline > 1.0) {
			throw std::invalid_argument("Invalid baseline ");
		}
		if (Intensity < Baseline) {
			throw std::invalid_argument("Error: intensity ");
		}
		mTimeOfElicitation = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		
		type = Type;
		intensity = Intensity;
		baseline = Baseline;
	
	}

	Emotion(Type Type, double Intensity, double Baseline, std::shared_ptr<Emotion>& Elicitor)
		: elicitor(new Elicit(Elicitor))
	{
		if (Intensity < 0.0 || Intensity > 1.0) {
			throw std::invalid_argument("Invalid intensity ");
		}
		if (Baseline < 0.0 || Baseline > 1.0) {
			throw std::invalid_argument("Invalid baseline ");
		}
		if (Intensity < Baseline) {
			throw std::invalid_argument("Error: intensity ");
		}
		mTimeOfElicitation = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		
		type = Type;
		intensity = Intensity;
		baseline = Baseline;
	}

	
	Emotion(Type Type, double Intensity, double Baseline, EmotionAppraisalVars eav, std::shared_ptr<Emotion>& Elicitor)
	: Emotion(Type, Intensity, Baseline, Elicitor)
	{
		mApraisalVars = eav;
	}
	
	Emotion(Type Type, double Intensity, double Baseline, EmotionAppraisalVars eav, std::shared_ptr<Elicit> Elicitor)
		: Emotion(Type, Intensity, Baseline, Elicitor)
	{
		mApraisalVars = eav;
	}

	/**
	* Returns the creation time of this emotion.
	*
	* @return the emotion type.
	*/
	long getStart() const {
		return mTimeOfElicitation;
	}

	/**
	* Returns the type of this emotion.
	*
	* @return the emotion type.
	*/
	Type getType() const {
		return type;
	}

	/**
	* Returns the intensity of this emotion.
	*
	* @return the emotion intensity.
	*/
	double getIntensity() const {
		return intensity;
	}

	/**
	* Sets the intensity of this emotion.
	*
	* @param intensity the new intensity
	*/
	void setIntensity(double Intensity) {
		intensity = Intensity;
	}

	/**
	* Returns the baseline of this emotion.
	*
	* @return the emotion baseline.
	*/
	double getBaseline() const {
		return baseline;
	}

	/**
	* Returns the list of appraisal variables
	*
	* @return eecs
	*/
	EmotionAppraisalVars getAppraisalVariables() const {
		return mApraisalVars;
	}

	/**
	* Returns the elicitor of this emotion.
	*
	* @return the emotion elicitor.
	*/
	std::shared_ptr<Elicit> getElicitor() const {
		return elicitor;
	}

	/**
	* Returns the pleasure, dominance, and arousal values of this emotion
	*
	* @return defined pleasure, dominance, and arousal values.
	*/
	Mood getPADValues() const {
		return mPADValues;
	}

	/**
	* Returns a concise representation of this emotion consisting of the type and
	* the intensity.
	*
	* @return a string representation of the emotion.
	*/
	std::string toString() const {
		return TypeNames[type] + " (" + std::to_string(intensity) + "/" + std::to_string(baseline) + ") << " + 
			((elicitor->Type != ElicitType::Unknown) ? elicitor->string : "");
	}

	/**
	* Compares this emotional state to the specified object. The result is
	* <code>true</code> if and only if the argument is an
	* <code>Emotion</code> object with the same emotion type emotion intensity,
	* and emotion elicitor.
	*
	* @param o the object to compare this emotion against.
	* @return <code>true</code> if the two emotions are equal; <code>false</code>
	* otherwise.
	*/
	bool operator==(std::shared_ptr<Emotion> e)
	{
		if (e == nullptr)
		{
			return false;
		}

		//figure out how to compare enums
		return e->getType() == type
			&& e->getIntensity() == intensity && e->getBaseline() == baseline
			&& (elicitor == e->getElicitor());
	}

	size_t getHash() const
	{
		return std::hash<size_t>()(type)
			* std::hash<double>()(intensity)
			<< std::hash<double>()(baseline)
			<< std::hash<long double>()(mTimeOfElicitation)
			* std::hash<long double>()(mCalculatedEndTime)
			^ std::hash<long double>()(mCalculatedDuration)
			* (std::hash<EmotionAppraisalVars>()(mApraisalVars) << 1);
	}
};

namespace std
{
	template<>
	struct hash<std::shared_ptr<Emotion>>
	{						
		size_t operator()(const std::shared_ptr<Emotion> k) const
		{
			// Compute individual hash values for two data members and combine them using XOR and bit shifting
			return k->getHash();
		}
	};
}