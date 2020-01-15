#pragma once
#include"Emotions/EmotionType.h"

/**
* This class represents an event triggered by a change in
* the agent's emotions by the user.
*
* @author Patrick Gebhard
*/

 class EmotionMaintenanceEvent  {
 protected:
	 EmotionType fEmotionType;
	 std::string fCharacterName;
	 double fEmotionIntensity;

	/**
	* Constructs an event fired by a change in the specified emotions.
	*
	* @param source the object that generated this event
	* @param name the name of the character to which the emotion change event is related
	* @param type the type of emotion
	* @param intensity the emotion's intensity
	*///Object source,
 public:
	 //might need to addelicitor-shakir
	 EmotionMaintenanceEvent( std::string name,
		EmotionType type, double intensity) {
		//super(source);
		fEmotionType = type;
		fEmotionIntensity = intensity;
		fCharacterName = name;
	}

	/**
	* Returns the changed emotion type.
	*/
	 EmotionType emotionType() {
		return fEmotionType;
	}

	/**
	* Returns the intensity of the changed emotion type.
	*/
	 double intensity() {
		return fEmotionIntensity;
	}

	/**
	* Returns the name of the agent, which emotion has been changed.
	*/
	 std::string getCharacterName() {
		return fCharacterName;
	}
};