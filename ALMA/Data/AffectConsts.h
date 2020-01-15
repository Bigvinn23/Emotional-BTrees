#pragma once

class AffectConsts {
	/** Maximum baseline intensity for emotions */
 public:
	 double emotionMaxBaseline = 1.0;
	 /** Emotions intensity influenced by personality factor */
	 double personalityEmotionInfluence = 0.5;
	 /** Emotion Decay */
	 long emotionDecayOverallTime = 1.8;
	 int emotionDecayPeriod = 1;
	 int emotionDecaySteps = 1;

	 long moodReturnOverallTime = 600000;
	 int moodReturnPeriod = 1;
	 int moodReturnSteps = 1;

	 bool moodStabilityControlledByNeurotism = true;

	 AffectConsts() = default;

	 void setUp(double emobaseline, double pemoinfulence, long emodecaytime, int emodecayperiod, int emodecaysteps,
		 long moodreturntime, int moodreturnperiod, int moodreturnsteps, bool control)
	 {
		emotionMaxBaseline = emobaseline;
		personalityEmotionInfluence = pemoinfulence;
		emotionDecayOverallTime = emodecaytime;
		emotionDecayPeriod = emodecayperiod;
		emotionDecaySteps = emodecaysteps;
		moodReturnOverallTime = moodreturntime;
		moodReturnPeriod = moodreturnperiod;
		moodReturnSteps = moodreturnsteps;
		moodStabilityControlledByNeurotism = false;

	 }
 };