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
 class LinearDecayFunction : public DecayFunction {

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
	std::unordered_map<std::shared_ptr<Emotion>, Values*> fDecayingEmotions;
	 int DecaySteps;
	 bool isInitialised = false;

public:
	LinearDecayFunction() {
	}

	 LinearDecayFunction(int decaySteps) {
		init(decaySteps);
	}

	

	 void init(int decaySteps) {
		if (decaySteps <= 0) {
			throw std::invalid_argument("Number of decay steps not positive.");
		}
		DecaySteps = decaySteps;
		isInitialised = true;
	}

	 std::shared_ptr<Emotion> decay(std::shared_ptr<Emotion> emotion) {
		if (!isInitialised) {
			throw std::invalid_argument("Decay function not initialized!");
		}
		Values* values = nullptr;
		double intensity = emotion->getIntensity();
		double baseline = emotion->getBaseline();
		double newIntensity = intensity;
		int xValue;
		int xEndValue;
		double operatingRange;
		if (intensity > baseline) {
			if (fDecayingEmotions.at(emotion)) {
				values = fDecayingEmotions.at(emotion);
				xValue = values->xValue;
				xEndValue = values->xEndValue;
				operatingRange = values->operatingRange;
			}
			else {
				operatingRange = (intensity - baseline);
				xValue = 0;
				xEndValue = (int)(DecaySteps * operatingRange);
				values = new Values(xValue, xEndValue, operatingRange);
				fDecayingEmotions.emplace(emotion, values);
			}
			newIntensity = intensity - (1.0 / DecaySteps);
			xValue++;
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

	 void setConfigParameter(double param) {
	}
};