#pragma once
#include"Emotions/Emotion.h"
#include"DecayFunction.h"

/**
* This class realizes emotion decay using the tangens hyperbolicus function.
*
* @author Patrick Gebhard
*
* @version 2.0
*/
 class TangensHyperbolicusDecayFunction : public DecayFunction {

 private:
	 class Values {
	 public:
		 int xValue;
		 int xEndValue;
		 double startValue;
		 double operatingRange;

		 Values(int xValue, int xEndValue, double startValue, double operatingRange) {
			 xValue = xValue;
			 xEndValue = xEndValue;
			 startValue = startValue;
			 operatingRange = operatingRange;
		 }

	 };
	 double TANH_CHARACTER = 0.2;
	 //std::unordered_map<Emotion::Type, std::shared_ptr<Emotion>> emotions;
	 std::unordered_map<std::shared_ptr<Emotion>, Values*> fDecayingEmotions;
	// Hashtable<Emotion, Values> fDecayingEmotions = new Hashtable<Emotion, Values>();
	 double scalingFactor = 0.0;
	 int DecaySteps;
	 bool isInitialised = false;

public:
		TangensHyperbolicusDecayFunction() {}

	 TangensHyperbolicusDecayFunction(int decaySteps) {
		init(decaySteps);
	}

	

	 void init(int decaySteps) {
		if (decaySteps <= 0)
			throw std::invalid_argument("Number of decay steps not positive.");
		DecaySteps = decaySteps;
		scalingFactor = 1 / (TANH_CHARACTER * decaySteps);
		isInitialised = true;
	}

	 std::shared_ptr<Emotion> decay(std::shared_ptr<Emotion> emotion) {
		if (!isInitialised)
			throw std::invalid_argument("Decay function not initialized!");
		Values* values = nullptr;
		double intensity = emotion->getIntensity();
		double baseline = emotion->getBaseline();
		double newIntensity = intensity;
		int xValue;
		int xEndValue;
		double startValue;
		double operatingRange;
		if (intensity > baseline) {
			if (fDecayingEmotions.at(emotion)!=nullptr) {
				values = fDecayingEmotions.at(emotion);
				xValue = values->xValue;
				xEndValue = values->xEndValue;
				startValue = values->startValue;
				operatingRange = values->operatingRange;
			}
			else { // first time setup of variables
				operatingRange = (intensity - baseline);
				xValue = 0;
				xEndValue = (int)(DecaySteps * operatingRange); //maximum x
				startValue = xEndValue * 0.5 / (operatingRange)* scalingFactor;
				values = new Values(xValue, xEndValue, startValue, operatingRange);
				fDecayingEmotions.emplace(emotion, values);
			}
			// debug log.info("Step " + xValue + "/" + xEndValue);
			double input = -startValue + (xValue / operatingRange * scalingFactor);
			newIntensity = operatingRange + baseline - ((tanh(input) * 0.5 + 0.5)) * operatingRange;
			// debug log.info("Input=" + prettyPrint(input) + " new intensity=" + prettyPrint(newIntensity));
			xValue++;
			values->xValue = xValue;
			fDecayingEmotions.emplace(emotion, values);
			if (xValue > xEndValue) {
				newIntensity = baseline;
				fDecayingEmotions.erase(emotion);
			}
		}
		emotion->setIntensity(newIntensity);
		return emotion;
	}

	 int decaySteps() {
		return DecaySteps;
	}

	 bool hasConfigParameter() {
		return true;
	}

	 double getConfigParameter() {
		return TANH_CHARACTER;
	}

	 void setConfigParameter(double param) {
		TANH_CHARACTER = param;
	}

}
;