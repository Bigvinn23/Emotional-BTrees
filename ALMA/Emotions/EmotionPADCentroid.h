#pragma once
#include<math.h>
#include<string>
 class EmotionPADCentroid {
 private:
	 double fPValue = 0.0;
	 double fAValue = 0.0;
	 double fDValue = 0.0;
	 double fWeight = 0.0;
	 int fEmotionCnt = 0;


public:
	EmotionPADCentroid() {
	}

	 void add(double pValue, double aValue, double dValue, double intensity) {
		double oldWeight = fWeight;
		fEmotionCnt++;
		//log.info(prettyPrint(intensity));
		fWeight += intensity;
		fPValue = ((fPValue*oldWeight) + (pValue*intensity)) / fWeight;
		fAValue = ((fAValue*oldWeight) + (aValue*intensity)) / fWeight;
		fDValue = ((fDValue*oldWeight) + (dValue*intensity)) / fWeight;
	}

	 bool isActive() {
		return ((fPValue != 0.0) || (fAValue != 0.0) || (fDValue != 0.0));
	}

	 double getPValue() {
		return fPValue;
	}

	 double getAValue() {
		return fAValue;
	}

	 double getDValue() {
		return fDValue;
	}

	  double getWeight() {
		return fWeight;
	}

	 double getIntensity() {
		return sqrt((fPValue*fPValue) + (fAValue*fAValue) + (fDValue*fDValue));
	}

	 void clear() {
		fPValue = 0.0;
		fAValue = 0.0;
		fDValue = 0.0;
		fWeight = 0.0;
		fEmotionCnt = 0;
	}

	std::string toString() {
		return "EmotionsCenter (" + std::to_string(fEmotionCnt) +  "emotions): p:" + std::to_string(fPValue) + " a:" + std::to_string(fAValue) + " d:" + std::to_string(fDValue) + " intensity: " + std::to_string(fWeight);
	}
};