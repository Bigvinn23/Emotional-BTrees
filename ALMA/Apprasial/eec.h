#pragma once
#include"eec/Agency.h"
#include"eec/Appealingness.h"
#include"eec/Desirability.h"
#include"eec/Likelihood.h"
#include"eec/Liking.h"
#include"eec/Praiseworthiness.h"
#include"eec/Realization.h"
#include "../Emotions/emotion.h"


/**
* The <code>EEC</code> class represents an Emotion Eliciting Condition.
* Each Emotion Eliciting Condition (EEC) is the result of an character's
* subjective appraisal of an emotion-inducing situation:
* <ol>
* <li> the desirability of events with respect to the character's goals
* <li> the praiseworthiness of actions with respect to a set of standards
* <li> the appealingness of objects with respect to a set of attitudes
* </ol>
* A character's appraisal is based on three central variables:
* <em>desirability</em>, <em>praiseworthiness</em>, and
* <em>appealingness</em> which apply to event-based emotions, character-based
* emotions, and object-based emotions respectively. Besides, there are other
* local variables, which only affect particular groups of emotions.
* <P>
* The methods of this class all throw a <code>NullPointerException</code>
* if the arguments provided to them are null.
*
* @author Patrick Gebhard (based on the work of Wenji Mao and Martin Klesen)
* @version 2.0
*/
class EEC {
private:
	EEC() {} // defeat instanciation
public:
	std::shared_ptr<Emotion::Elicit> elicitor;
	// variables affecting the intensity of particular groups of emotions
	 Desirability* desirabilityForSelf;
	 Desirability* desirabilityForOther;
	 Praiseworthiness* praiseworthiness;
	 Appealingness* appealingness;
	 Likelihood* likelihood;
	 Realization* realization;
	 Likin* liking;
	 Agency* character;


					 /**
					 * Returns a new Emotion Eliciting Condition representing the
					 * subjective appraisal of an event as being desirable or
					 * undesirable for the character itself.
					 *
					 * @param elicitor the emotion-inducing situation
					 * @param desirability the desirability of the event
					 * @return the new Emotion Eliciting Condition.
					 */
	 static EEC* appraiseEvent(std::shared_ptr<Emotion::Elicit> elicitor, Desirability* desirability) {
		if (desirability == nullptr)
			throw std::invalid_argument("Desirability not specified.");
		EEC* eec = new EEC();
		eec->elicitor = elicitor;
		eec->desirabilityForSelf = desirability;
		return eec;
	}

	/**
	* Returns a new Emotion Eliciting Condition representing the
	* subjective appraisal of an anticipated event as being
	* desirable or undesirable for the character itself.
	*
	* @param elicitor the emotion-inducing situation
	* @param desirability the desirability of the anticipated event
	* @param likelihood the likelihood of the anticipated event
	* @return the new Emotion Eliciting Condition.
	*/
	 static EEC* appraiseEvent(std::shared_ptr<Emotion::Elicit> elicitor, Desirability* desirability,
		Likelihood* likelihood) {
		if (desirability == nullptr)
			throw std::invalid_argument("Desirability not specified.");
		if (likelihood == nullptr)
			throw std::invalid_argument("Likelihood not specified.");
		EEC* eec = new EEC();
		eec->elicitor = elicitor;
		eec->desirabilityForSelf = desirability;
		eec->likelihood = likelihood;
		return eec;
	}

	/**
	* Returns a new Emotion Eliciting Condition representing the
	* status of an anticipated event.
	*
	* @param elicitor the emotion-inducing situation
	* @param realization the status of an anticipated event
	* @return the new Emotion Eliciting Condition.
	*/
	 static EEC* appraiseEvent(std::shared_ptr<Emotion::Elicit>elicitor, Realization* realization) {
		if (realization == nullptr)
			throw std::invalid_argument("Realization not specified.");
		EEC* eec = new EEC();
		eec->elicitor = elicitor;
		eec->realization = realization;
		return eec;
	}

	/**
	* Returns a new Emotion Eliciting Condition representing the
	* subjective appraisal of an event as being desirable or
	* undesirable for another character.
	*
	* @param elicitor the emotion-inducing situation
	* @param desirability the desirability of the event for the other person
	* @param liking the liking for the other person
	* @return the new Emotion Eliciting Condition.
	*/
	 static EEC* appraiseEvent(std::shared_ptr<Emotion::Elicit> elicitor, Desirability* desirability, Likin* liking) {
		if (desirability == nullptr)
			throw new std::invalid_argument("Desirability not specified.");
		if (liking == nullptr)
			throw std::invalid_argument("Liking not specified.");
		EEC* eec = new EEC();
		eec->elicitor = elicitor;
		eec->desirabilityForOther = desirability;
		eec->liking = liking;
		return eec;
	}

	/**
	* Returns a new Emotion Eliciting Condition representing the
	* subjective appraisal of an action as being praiseworthy or
	* blameworthy.
	*
	* @param elicitor the emotion-inducing situation
	* @param praiseworthiness the praiseworthiness of the action.
	* @param character the character considered responsible for the action.
	* @return the new Emotion Eliciting Condition.
	*/
	 static EEC* appraiseAction(std::shared_ptr<Emotion::Elicit> elicitor, Praiseworthiness* praiseworthiness,
		Agency* character) {
		if (praiseworthiness == nullptr)
			throw std::invalid_argument("Praiseworthiness not specified.");
		if (character == nullptr)
			throw std::invalid_argument("Agency not specified.");
		EEC* eec = new EEC();
		eec->elicitor = elicitor;
		eec->praiseworthiness = praiseworthiness;
		eec->character = character;
		return eec;
	}

	/**
	* Returns a new Emotion Eliciting Condition representing the
	* subjective appraisal of an object as being appealing or
	* unappealing.
	*
	* @param elicitor the emotion-inducing situation
	* @param appealingness the appealingness of the object
	* @return the new Emotion Eliciting Condition.
	*/
	 static EEC* appraiseObject(std::shared_ptr<Emotion::Elicit> elicitor, Appealingness* appealingness) {
		if (appealingness == nullptr)
			throw std::invalid_argument("Appealingness not specified.");
		EEC* eec = new EEC();
		eec->elicitor = elicitor;
		eec->appealingness = appealingness;
		return eec;
	}

	/**
	* Returns the emotion-inducing situation whose subjective appraisal
	* is represented by this Emotion Eliciting Condition.
	*
	* @return the emotion-inducing situation.
	*/
	 std::shared_ptr<Emotion::Elicit> getElicitor() {
		return elicitor;
	}

	/**
	* Returns a string describing the Emotion Eliciting Condition.
	*
	* @return a string describing the Emotion Eliciting Condition.
	
	public String toString() {
		StringBuffer result = new StringBuffer();
		result.append("[EEC: ");
		result.append("elicitor=" + elicitor);
		if (desirabilityForSelf != null)
			result.append(", desirability=" + desirabilityForSelf);
		if (likelihood != null)
			result.append(", likelihood=" + likelihood);
		if (realization != null)
			result.append(", realization=" + realization);
		if (desirabilityForOther != null)
			result.append(", desirability=" + desirabilityForOther);
		if (liking != null)
			result.append(", liking=" + liking);
		if (praiseworthiness != null)
			result.append(", praiseworthiness=" + praiseworthiness);
		if (character != null)
			result.append(", character=" + character);
		if (appealingness != null)
			result.append(", appealingness=" + appealingness);
		result.append("]");
		return result.toString();
	}*/

};