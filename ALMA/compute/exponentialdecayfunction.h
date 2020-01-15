#pragma once
#include"Emotions/Emotion.h"
#include"DecayFunction.h"

/**
* This class uses an exponential function for the emotion decay.
*
* @author Patrick Gebhard
*
* @version 2.0
*/
class ExponentialDecayFunction : public DecayFunction {

private:
	class Values {
	public:
		int xValue;
		int xEndValue;
		double operatingRange;

		Values(int XValue, int XEndValue, double OperatingRange) {
			xValue = XValue;
			xEndValue = XEndValue;
			operatingRange = OperatingRange;
			
		}

	};
	  static double ZEROCONST ;
	 std::unordered_map<std::shared_ptr<Emotion>, Values*> fDecayingEmotions;
	 int DecaySteps;
	 bool isInitialised = false;

	 
public:
		ExponentialDecayFunction() { ZEROCONST = log(0.004); }

	 ExponentialDecayFunction(int decaySteps) {
		init(decaySteps);
	}



	 void init(int DecaySteps) {
		if (DecaySteps <= 0)
			throw std::invalid_argument("Number of decay steps not positive.");
		DecaySteps = DecaySteps;
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
		if (intensity  > baseline) {
			if (fDecayingEmotions.at(emotion)) {
				values = fDecayingEmotions.at(emotion);
				xValue = values->xValue;
				xEndValue = values->xEndValue;
				operatingRange = values->operatingRange;
			}
			else { // first time setup of variables
				operatingRange = (intensity - baseline);
				xValue = 0;
				xEndValue = (int)(DecaySteps * operatingRange); // maximum x
				values = new Values(xValue, xEndValue, operatingRange);
				fDecayingEmotions.emplace(emotion, values);
			}
			xValue++;
			newIntensity = exp(xValue * ZEROCONST / xEndValue) * (operatingRange)+baseline;
			values->xValue = xValue;
			fDecayingEmotions.emplace(emotion, values);
			if ((xValue > xEndValue) || (newIntensity < baseline)) {
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
		return false;
	}

	 double getConfigParameter() {
		return -1.0;
	}

	

};