#pragma once
#include"../Emotions/Emotion.h"

/**
* Interface for emotion decay functions
*
* @author Patrick Gebhard
* @version 1.0
*/
class DecayFunction {
	/** Initializes the decay function. The decay function computes for each
	* emotion in the emotion vector a decay factor based on the number of
	* steps required to decay to the emotion baseline.
	*
	* @param decaySteps the number of steps to decay
	*
	* @throws NullPointerException if the set of emotions is not specified
	* @throws IllegalArgumentException if the number of decay steps is not positive
	* @throws IllegalArgumentException if the decay threshold is negative
	*/
public:
		virtual void init(int decaySteps) = 0;

	/** Decays an emotion and returns the result. The intensity of an emotion
	* is always greater or equal to the emotion baseline.
	*
	* @param emotion the emotion to decay
	* @return the decayed emotion
	*/
		virtual std::shared_ptr<Emotion> decay(std::shared_ptr<Emotion> emotion) = 0;

	/** Access the amount of decay steps.
	*
	* @return the number of steps to decay.
	*/
		virtual int decaySteps() = 0;

	/** Indicates if function has additional configuration parameter
	*
	* @return existance of additional configuration parameter (true or false)
	*/
		virtual bool hasConfigParameter() = 0;

	/** Returns (if any) the additional configuration parameter
	*
	* @return if exisits, additional configuration parameter, else -1.0d
	*/
		virtual double getConfigParameter() = 0;

	/** Sets the additional configuration parameter
	*/
		virtual void setConfigParameter(double param) = 0;
};