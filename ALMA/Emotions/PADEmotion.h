#pragma once
#include "Emotion.h"
#include"EmotionType.h"
#include<string>
 class PADEmotion : public Emotion {

 public:
	 PADEmotion(Mood padVals, double intensity, std::string description) 
		 :Emotion(EmotionType::Physical, intensity, 0.0, description)
	 {
		  
		mPADValues = padVals;
	}


 };