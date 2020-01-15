#pragma once
#ifndef PersonalityEmotionsRelations_H
#define PersonalityEmotionsRelations_H
#include "../Emotions/EmotionType.h"
#include <unordered_map>
#include<memory>
/**
* The <code>Influence</code> class represents the influence of
* personality traits for the intensity of specific emotions.
*/

enum SignumType : int
{
	Neg,
	Null,
	Pos
};

class Influence
{
public:
	int opennessInfluence = 0;
	int conscientiousnessInfluence = 0;
	int extraversionInfluence = 0;
	int agreeablenessInfluence = 0;
	int neurotismInfluence = 0;

	Influence() = default;
	Influence(const Influence&) = default;
	/**
	* Constructs a  <code>Influence</code> object representing
	* the influence of personality traits for the intensity of a
	* specific emotion.
	* <p>
	* The influence parameters take the following values:
	* <pre>
	*     -1   - negative correlation (decrease intensity)
	*      0   - no correlation
	*      1   - positive correlation (increase intensity)
	* </pre>
	*
	* @param  extraversionInfluence The influence of the extraversion trait
	*                               on the emotion intensity.
	* @param  agreeablenessInfluence The influence of the agreeableness trait
	*                                on the emotion intensity.
	* @param  neuroticismInfluence The influence of the neuroticism trait
	*                              on the emotion intensity.
	*/
	  Influence(SignumType o, SignumType c, SignumType e, SignumType a, SignumType n) {
		opennessInfluence = o - 1;
		conscientiousnessInfluence = c - 1;
		extraversionInfluence = e - 1;
		agreeablenessInfluence = a - 1;
		neurotismInfluence = n - 1;
	}

};

/**
* The <code>Correlation</code> class represents the correlation
* between personality traits and the intensity of emotion types.
*/

extern const std::unordered_map<EmotionType, Influence> Relations;

Influence getInfluence(EmotionType type);

class Personality;

class PersonalityEmotionsRelations {
private:

	// correlation between personality and emotions
	// Correlation correlation;
	// static std::unique_ptr< PersonalityEmotionsRelations> instance;
	double fPersonalityEmotionInfluence = 0.5;
	double fEmotionMaxBaseline = 0.40;

public:
	PersonalityEmotionsRelations() {
	}
	void setPersonalityEmotionInfluence(double personalityEmotionInfluence) {
		fPersonalityEmotionInfluence = personalityEmotionInfluence;
	}

	void setUp(double EmoInfluence, double Emobaseline )
	{
		setPersonalityEmotionInfluence(EmoInfluence);
		setEmotionMaxBaseline(Emobaseline);
	}

	void setEmotionMaxBaseline(double emotionMaxBaseline) {
		fEmotionMaxBaseline = emotionMaxBaseline;
	}

	/**
	* Returns the combined influence of the character's personality traits
	* for the specified emotion type.
	*
	* @param personality the character's personality
	* @param type the emotion type
	* @return the combined influence.
	*/
	 double getPersonalityInfluence(const Personality& personality, EmotionType type) const;

	/**
	* Returns the baseline for the specified emotion type. The baseline
	* depends on the character's personality.
	*
	* @param personality the character's personality
	* @param type the emotion type
	* @return the emotion baseline.
	*/
	const double getEmotionBaseline(const Personality& personality, EmotionType type) const
	{
		double value = getPersonalityInfluence(personality, type);
		//DEBUG Logger.global.info("EMOTION " + type + " is influenced by personality by " + value);
		value = (value > fEmotionMaxBaseline) ? fEmotionMaxBaseline : value;
		value = (value < 0.0) ? 0.0 : value;
		//DEBUG Logger.global.info("EMOTION " + type + " has baseline " + value);
		return value;
	}


};


#endif