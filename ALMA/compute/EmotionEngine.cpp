#pragma once
/*
#include"Emotions/EmotionAppraisalVars.h"
#include"Apprasial/EEC.h"
#include"Emotions/Emotion.h"
#include"Emotions/Emotionhistory.h"
#include"Emotions/EmotionType.h"
#include"Emotions/EmotionVector.h"
#include"Apprasial/eec/Agency.h"
//#include"AffectManager.h"
#include"Mood/Mood.h"
#include"Personality/Personality.h"
#include"Personality/PersonalityEmotionsRelations.h"
#include"Personality/PersonalityMoodRelations.h"
#include<list>
#include"compute/DecayFunction.h"
#include<set>*/
#include"../compute/EmotionEngine.h"


	double EmotionEngine::max(double x, double y)
	{
		if (x > y)
			return x;
		return y;
	}


	EmotionEngine::EmotionEngine(Personality personality, DecayFunction* decayFunction) {
		fPerEmoRel = personality.getPersonalityEmotionsRelations();
		if (personality == nullptr) {
			throw std::invalid_argument("Personality not specified.");
		}
		if (decayFunction == nullptr) {
			throw new std::invalid_argument("Decay function not specified.");
		}
		fDecayFunction = decayFunction;
		fPersonality = personality;
	}
	double EmotionEngine::adjustIntensity(double intensity, EmotionType type, Mood mood) {
		double result = intensity * (1 + fPerEmoRel->getPersonalityInfluence(fPersonality, type));
		Personality temporalPersonality = fPerMoodRel->getMoodyPersonalityTraits(mood);
		//log.info("Temporal mood personality is " + temporalPersonality);
		double moodyResult = fPerEmoRel->getPersonalityInfluence(temporalPersonality, type);
		//log.info(type.toString() + " personality influcenced intensity= " + result + " mood influence= " + moodyResult);
		result = result + moodyResult;
		double baseline = fPerEmoRel->getEmotionBaseline(fPersonality, type);
		double moodyBaseline = fPerEmoRel->getEmotionBaseline(fPerMoodRel->getMoodyPersonalityTraits(mood), type);
		baseline = baseline + moodyBaseline;
		result = (result > 1.0) ? 1.0 : result;
		result = (result < baseline) ? baseline : result;
		return result;
	}
	/**
	* Adds a new element to the list of Emotion Eliciting Conditions. This method
	* throws an
	* <code>IllegalStateException</code> if an an EEC for event-based emotions,
	* action-based emotions, or Emotion::Elicit-based emotions already exists to prevent
	* the generation of emotions of the same type. This is necessary since the
	* <code>EmotionVector</code> used to store the infered emotions contains
	* exactly <b>one</b> emotion for each emotion type.
	*
	* @param eec the std::make_shared<Emotion> Eliciting Condition.
	*
	* @throws nullptrPointerException if the Emotion Eliciting Condition is not
	* specified.
	*
	* @throws IllegalStateException if an EEC for event-based emotions,
	* action-based emotions, or Emotion::Elicit-based emotions already exists.
	*
	* @see EmotionVector
	*/
	void EmotionEngine::addEEC(EEC* eec) {
		if (eec == nullptr) {
			throw std::invalid_argument("Emotion Eliciting Condition not specified.");
		}
		if (eec->appealingness != nullptr) // appraisal of Emotion::Elicit
		{
			if (ElicitEEC == nullptr) {
				ElicitEEC = eec;
			}
			else {
				throw std::invalid_argument("EEC for Emotion::Elicit-based emotions already exists: ");
			}
		}
		else if (eec->praiseworthiness != nullptr) // appraisal of action
		{
			if (actionEEC == nullptr) {
				actionEEC = eec;
			}
			else {
				throw std::invalid_argument("EEC for action-based emotions already exists: ");
			}
		}


		else if (eventEEC == nullptr) { // appraisal of event
			eventEEC = eec;
		}
		else // 	{
			 //  	// THIS IS THE CASE, IF TWO Event-based EECs (second is confirmation)
			 //  	// are given as input
			 // 	EEC oldEEC = eventEEC;
			 // 	Emotion::Elicit oldElicitor = oldEEC->getElicitor();
			 // 	Emotion::Elicit newElicitor = eec->getElicitor();
			 // 	if (!(oldElicitor == newElicitor) && !(eec.realization != nullptr)) //second eec is realization nof first eec.
		{
			//	throw std::invalid_argument("EEC for event-based emotions already exists: ");
		}
		//       }
		listOfEEC.push_back(eec);
	}

	/**
	* Removes all elements from the list of Emotion Eliciting Conditions.
	*/
	void EmotionEngine::clearEEC() {
		listOfEEC.clear();
		eventEEC = nullptr;
		actionEEC = nullptr;
		ElicitEEC = nullptr;
	}



	/**
	* Decays the specified emotion history using the engine's decay function.
	* This method decays each element in the emotion history and returns the new
	* emotional state. It generates an
	* <code>EmotionChangeEvent</code> to signal that the emotional state has
	* changed.
	*
	* @param history the emotion history
	* @param emotions the collection used to store the emotional state
	* @return the std::make_shared<Emotion>al state.
	* @throws nullptrPointerException if no history is specified.
	* @see EmotionVector getEmotionalState(EmotionVector emotions)
	*/
	// synchronized
	std::shared_ptr<EmotionVector> EmotionEngine::decay(EmotionHistory* history,
		std::shared_ptr<EmotionVector> emotions,
		std::shared_ptr<EmotionVector> defaultEmotions) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		if (history == nullptr) {
			throw std::invalid_argument("No history specified.");
		}
		for (int i = 0; i < history->size(); i++)
		{
			auto& it = history->get(i);
			for (auto& emotion = it->Getmap().begin(); emotion != it->Getmap().end(); ++emotion)
			{

				if (emotion->second->getIntensity() > emotion->second->getBaseline()) {
					//debug log.info("At emotion history " + i + " decaying " + emotion.getType() + "(" + prettyPrint(emotion->getIntensity()) + ")");
					history->get(i)->add(fDecayFunction->decay(emotion->second));
				}
			}
		}
		history->removeFirstIfInactive();
		emotions = history->getEmotionalState(defaultEmotions);
		return emotions;
	}

	/**
	* Simulates the future decay of a specific emotion using the engine's decay
	* function.
	*
	* @param emotion the emotion that will be decayed
	* @return List of intesity values that denotes the decay of the emotion
	* @throws nullptrPointerException if no history is specified.
	*/
	//synchronized 
	std::list<float> EmotionEngine::simulateDecay(std::shared_ptr<Emotion> emotion) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		std::list<float> results;
		results.push_back(emotion->getIntensity());

		std::shared_ptr<Emotion> decayedEmotion = std::make_shared<Emotion>(emotion->getType(), emotion->getIntensity(), emotion->getBaseline(), emotion->getElicitor());
		while (decayedEmotion->getIntensity() - 0.001 > decayedEmotion->getBaseline()) {
			//log.info("At emotion history " + i + " decaying " + decayedEmotion.getType() + "(" + prettyPrint(decayedEmotion->getIntensity()) + ")");
			fDecayFunction->decay(decayedEmotion);
			results.push_back(decayedEmotion->getIntensity());
		}

		return results;
	}

	/**
	* Infers all emotions based on the list of Emotion Eliciting Conditions and
	* adds them to the specified collection. If the type of an infered emotion is
	* not contained in the predefined set of emotion types this emotion will be
	* ignored. <P> If a compound emotion (e.g.
	* <code>Gratitude</code>) can be infered then the two corresponding single
	* emotions (
	* <code>Joy</code> and
	* <code>Admiration</code>) are not returned as part of the result.
	*
	* @param emotions the emotions to which the infered one will be added
	* @param history the emotion history
	* @return the modified collection of emotions.
	* @throws nullptrPointerException if the emotion history is not specified.
	*/
	//synchronized 
	std::shared_ptr<EmotionVector> EmotionEngine::inferEmotions(std::shared_ptr<EmotionVector> emotions, EmotionHistory* history, Mood mood) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		if (history == nullptr) {
			throw std::invalid_argument("Emotion history not specified.");
		}
		if (mood == nullptr) {
			throw std::invalid_argument("mood not specified.");
		}
		// temporarily generated emotions
		std::list<std::shared_ptr<Emotion>> tempEmotions;
		// emotions used for compound emotions
		std::set<std::shared_ptr<Emotion>> usedEmotions;
		// --------------------------------------------
		// Resolve single emotions from groups of EECs.
		// --------------------------------------------
		if (listOfEEC.empty()) {

			return nullptr;
		}
		for (auto& allEEC : listOfEEC) {
			EEC* eec = allEEC;
			//log.fine("EEC input " + eec->getElicitor());
			EmotionType type = EmotionType::Undefined;
			double intensity = 0.0;


			// Well-being and Prospect-based Group:
			if (eec->desirabilityForSelf != nullptr) {

				if (eec->likelihood == nullptr) {
					type = eec->desirabilityForSelf->isDesirable()
						? EmotionType::Joy : EmotionType::Distress;
					intensity = eec->desirabilityForSelf->absDegree();
					intensity = adjustIntensity(intensity, type, mood);

					// collect all relevant appraisal variables for this emotion
					EmotionAppraisalVars eav = EmotionAppraisalVars();
					eav.setDesirability(eec->desirabilityForSelf->getDegree());
					eav.setAgency(true);
					tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
						fPerEmoRel->getEmotionBaseline(fPersonality, type), eav, eec->elicitor));
				}
				else {
					type = eec->desirabilityForSelf->isDesirable()
						? EmotionType::Hope : EmotionType::Fear;
					//PG: TODO: provide better intensity calculation with regrad to the likelihood in the situation
					intensity = max(eec->desirabilityForSelf->absDegree(), eec->likelihood->absDegree());
					intensity = adjustIntensity(intensity, type, mood);

					// collect all relevant appraisal variables for this emotion
					EmotionAppraisalVars eav = EmotionAppraisalVars();
					eav.setDesirability(eec->desirabilityForSelf->getDegree());
					eav.setLikelihood(eec->likelihood->absDegree());
					eav.setAgency(true);

					tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
						fPerEmoRel->getEmotionBaseline(fPersonality, type), eav, eec->elicitor));
				}
			}
			// Fortunes-Of-Others Group:
			if (eec->desirabilityForOther != nullptr && eec->liking != nullptr) {
				if (eec->liking->isLikely()) {
					type = eec->desirabilityForOther->isDesirable()
						? EmotionType::HappyFor : EmotionType::Pity;
				}
				else {
					type = eec->desirabilityForOther->isDesirable()
						? EmotionType::Resentment : EmotionType::Gloating;
				}

				intensity = max(eec->desirabilityForOther->absDegree(), eec->liking->absDegree());
				intensity = adjustIntensity(intensity, type, mood);

				// collect all relevant appraisal variables for this emotion
				EmotionAppraisalVars eav = EmotionAppraisalVars();
				eav.setDesirability(eec->desirabilityForOther->getDegree());
				eav.setLikelihood(eec->liking->absDegree());
				eav.setAgency(false);


				tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
					fPerEmoRel->getEmotionBaseline(fPersonality, type), eec->elicitor));
			}
			// Confirmation Group:
			if (eec->realization != nullptr) {
				// 	// THIS IS THE CASE, IF TWO Event-based EECs (second is confirmation)
				// 	// are given as input
				// 	// is there a recently elicited relevant emotion hope or fear
				// 	for (Iterator it = tempEmotions.iterator(); it.hasNext();) {
				// 	  Emotion old = (Emotion)it.next();
				// 	  if (old->getElicitor() != nullptr) {
				// 	    EmotionType oldType = old.getType();
				// 	    if ((oldType == EmotionType::Hope || oldType == EmotionType::Fear) &&
				// 		old->getElicitor().operator==(eec.elicitor)) {
				// 	      if (eec.realization.isConfirmed())
				// 		type = (oldType == EmotionType::Hope) ?
				// 		  EmotionType::Satisfaction : EmotionType::FearsConfirmed;
				// 	      else
				// 		type = (oldType == EmotionType::Hope) ?
				// 		  EmotionType::Disappointment : EmotionType::Relief;
				// 	      intensity = max(old->getIntensity(), eec.realization.absDegree());
				// 	      intensity = adjustIntensity(intensity, type, mood);
				// 	      tempEmotions.clear();
				// 	      tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
				// 					   fPerEmoRel->getEmotionBaseline(fPersonality, type),
				// 					   eec.elicitor));
				// 	    }
				// 	  }
				// 	}
				// traverse history in reverse order, latest element first
				for (int i = history->size() - 1; i >= 0; i--) {
					std::list<std::shared_ptr<Emotion>> oldEmotions = history->get(i)->getEmotions();
					for (auto& old : oldEmotions) {
						if (old->getElicitor() != nullptr) {
							EmotionType oldType = old->getType();
							if ((oldType == EmotionType::Hope || oldType == EmotionType::Fear)
								&& old->getElicitor()->operator==(eec->getElicitor())) {
								if (eec->realization->isConfirmed()) {
									type = (oldType == EmotionType::Hope)
										? EmotionType::Satisfaction : EmotionType::FearsConfirmed;
								}
								else {
									type = (oldType == EmotionType::Hope)
										? EmotionType::Disappointment : EmotionType::Relief;
								}
								intensity = max(old->getIntensity(), eec->realization->absDegree());
								intensity = adjustIntensity(intensity, type, mood);

								// collect all relevant appraisal variables for this emotion
								EmotionAppraisalVars eav = EmotionAppraisalVars();

								eav.setDesirability(old->getAppraisalVariables().desirability);
								eav.setLikelihood(old->getAppraisalVariables().likelihood);
								eav.setRealization(eec->realization->isConfirmed());
								eav.setAgency(true);

								tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
									fPerEmoRel->getEmotionBaseline(fPersonality, type),
									eec->elicitor));
								// set intensity of hope or fear in history to baseline
								// if anticipated event (elicitor) has occurred (is
								// confirmed) or has failed to occur (is disconfirmed)
								history->get(i)->add(std::make_shared<Emotion>(oldType, old->getBaseline(), old->getBaseline(), eav,
									old->getElicitor()));
							}
						}
					}
				}
			}
			// Attribution Group:
			if (eec->praiseworthiness != nullptr) {
				if (eec->praiseworthiness->isPraiseworthy()) {
					type = (eec->character->getAgency() == Agency::SELF.getAgency()) ? EmotionType::Pride : EmotionType::Admiration;
				}
				else {
					type = (eec->character->getAgency() == Agency::SELF.getAgency()) ? EmotionType::Shame : EmotionType::Reproach;
				}
				intensity = eec->praiseworthiness->absDegree();
				intensity = adjustIntensity(intensity, type, mood);

				// collect all relevant appraisal variables for this emotion
				EmotionAppraisalVars eav = EmotionAppraisalVars();

				eav.setPraiseworthy(eec->praiseworthiness->getDegree());
				eav.setAgency((eec->character->getAgency() == Agency::SELF.getAgency()) ? true : false);

				tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
					fPerEmoRel->getEmotionBaseline(fPersonality, type), eav, eec->elicitor));
			}
			// Attraction Group:
			if (eec->appealingness != nullptr) {
				type = eec->appealingness->isAppealing() ? EmotionType::Liking : EmotionType::Disliking;
				intensity = eec->appealingness->absDegree();
				intensity = adjustIntensity(intensity, type, mood);

				// collect all relevant appraisal variables for this emotion
				EmotionAppraisalVars eav = EmotionAppraisalVars();

				eav.setAppealingness(eec->appealingness->absDegree());

				tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
					fPerEmoRel->getEmotionBaseline(fPersonality, type), eec->elicitor));
			}
		} // end-for
		  // ---------------------------------------------------
		  // Resolve compound emotions according to single ones.
		  // ---------------------------------------------------
		  // Candidates for compound emotions
		std::shared_ptr<Emotion> joy = nullptr;
		std::shared_ptr<Emotion> pride = nullptr;
		std::shared_ptr<Emotion> admiration = nullptr;
		std::shared_ptr<Emotion> distress = nullptr;
		std::shared_ptr<Emotion> shame = nullptr;
		std::shared_ptr<Emotion> reproach = nullptr;
		std::shared_ptr<Emotion> liking = nullptr;
		std::shared_ptr<Emotion> disliking = nullptr;
		for (auto& it : tempEmotions) {
			std::shared_ptr<Emotion> e = it;
			EmotionType type = e->getType();
			if (type == EmotionType::Joy) {//all of the following nullptrs were e
				joy = e;
			}
			else if (type == EmotionType::Pride) {
				pride = nullptr;
			}
			else if (type == EmotionType::Admiration) {
				admiration = nullptr;
			}
			else if (type == EmotionType::Distress) {
				distress = nullptr;
			}
			else if (type == EmotionType::Shame) {
				shame = nullptr;
			}
			else if (type == EmotionType::Reproach) {
				reproach = nullptr;
			}
			else if (type == EmotionType::Liking) {
				liking = nullptr;
			}
			else if (type == EmotionType::Disliking) {
				disliking = nullptr;
			}
		}
		// Candidates for compound emotions elicited in earlier time
		std::shared_ptr<Emotion> histJoy = nullptr;
		std::shared_ptr<Emotion>histPride = nullptr;
		std::shared_ptr<Emotion> histAdmiration = nullptr;
		std::shared_ptr<Emotion>histDistress = nullptr;
		std::shared_ptr<Emotion> histShame = nullptr;
		std::shared_ptr<Emotion> histReproach = nullptr;
		std::shared_ptr<Emotion>histLiking = nullptr;
		std::shared_ptr<Emotion>histDisliking = nullptr;
		// --------------------------------------------------------------------------------
		// Well-being/Attribution Group:
		// --------------------------------------------------------------------------------
		// Checking emotion history, if a suitable emotion with the same elicitor
		// exisits - but only if one of the needed emotion is missing.
		// If so, use this emotion!
		// Compount Emotion Gratification
		histJoy = ((joy == nullptr) && (pride != nullptr))
			? history->getEmotionByElicitor(EmotionType::Joy, pride->getElicitor()->Emo) : nullptr;
		histPride = ((joy != nullptr) && (pride == nullptr))
			? history->getEmotionByElicitor(EmotionType::Pride, joy->getElicitor()->Emo) : nullptr;
		if ((histJoy != nullptr) || (histPride != nullptr)) {
			EmotionType type = EmotionType::Gratification;
			if (joy != nullptr) {
				std::shared_ptr<Emotion::Elicit> elicitor = joy->getElicitor();
				double intensity = max(joy->getIntensity(), histPride->getIntensity());
				intensity = adjustIntensity(intensity, type, mood);

				// collect all relevant appraisal variables for this emotion
				EmotionAppraisalVars eav = EmotionAppraisalVars();

				eav.setDesirability(joy->getAppraisalVariables().desirability);
				eav.setPraiseworthy(histPride->getAppraisalVariables().praiseworthy);
				eav.setAgency(true);


				tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
					fPerEmoRel->getEmotionBaseline(fPersonality, type), eav, elicitor));
				usedEmotions.insert(joy);
				history->removeEmotionByElicitor(histPride, elicitor);
			}
			if (pride != nullptr) {
				std::shared_ptr<Emotion::Elicit> elicitor = pride->getElicitor();
				double intensity = max(histJoy->getIntensity(), pride->getIntensity());
				intensity = adjustIntensity(intensity, type, mood);

				// collect all relevant appraisal variables for this emotion
				EmotionAppraisalVars eav = EmotionAppraisalVars();

				eav.setDesirability(histJoy->getAppraisalVariables().desirability);
				eav.setPraiseworthy(pride->getAppraisalVariables().praiseworthy);
				eav.setAgency(true);


				tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
					fPerEmoRel->getEmotionBaseline(fPersonality, type), eav, elicitor));
				usedEmotions.insert(pride);
				history->removeEmotionByElicitor(histJoy, elicitor);
			}
		}

		// current emotions elicited at the same time
		if (joy != nullptr && pride != nullptr && joy->getElicitor() != nullptr
			&& joy->getElicitor()->operator==(pride->getElicitor())) {
			//log.info("Build gratification out of current elicitors");
			EmotionType type = EmotionType::Gratification;
			std::shared_ptr<Emotion::Elicit> elicitor = pride->getElicitor();
			double intensity = max(joy->getIntensity(), pride->getIntensity());
			intensity = adjustIntensity(intensity, type, mood);

			// collect all relevant appraisal variables for this emotion
			EmotionAppraisalVars eav = EmotionAppraisalVars();

			eav.setDesirability(joy->getAppraisalVariables().desirability);
			eav.setPraiseworthy(pride->getAppraisalVariables().praiseworthy);
			eav.setAgency(true);


			tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
				fPerEmoRel->getEmotionBaseline(fPersonality, type), eav, elicitor));
			usedEmotions.insert(joy);
			usedEmotions.insert(pride);
		}
		// Checking emotion history, if a suitable emotion with the same elicitor
		// exisits - but only if one of the needed emotion is missing.
		// If so, use this emotion!
		// Compount Emotion Gratitude
		histJoy = ((joy == nullptr) && (admiration != nullptr))
			? history->getEmotionByElicitor(EmotionType::Joy, admiration->getElicitor()->Emo) : nullptr;
		histAdmiration = ((joy != nullptr) && (admiration == nullptr))
			? history->getEmotionByElicitor(EmotionType::Admiration, joy->getElicitor()->Emo) : nullptr;
		if ((histJoy != nullptr) || (histAdmiration != nullptr)) {
			EmotionType type = EmotionType::Gratitude;
			if (joy != nullptr) {
				std::shared_ptr<Emotion::Elicit> elicitor = joy->getElicitor();
				double intensity = max(joy->getIntensity(), histAdmiration->getIntensity());
				intensity = adjustIntensity(intensity, type, mood);

				// collect all relevant appraisal variables for this emotion
				EmotionAppraisalVars eav = EmotionAppraisalVars();

				eav.setDesirability(joy->getAppraisalVariables().desirability);
				eav.setPraiseworthy(histAdmiration->getAppraisalVariables().praiseworthy);
				eav.setAgency(false);


				tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
					fPerEmoRel->getEmotionBaseline(fPersonality, type), eav, elicitor));
				usedEmotions.insert(joy);
				history->removeEmotionByElicitor(histAdmiration, elicitor);
			}
			if (admiration != nullptr) {
				std::shared_ptr<Emotion::Elicit> elicitor = admiration->getElicitor();
				double intensity = max(admiration->getIntensity(), histJoy->getIntensity());
				intensity = adjustIntensity(intensity, type, mood);

				// collect all relevant appraisal variables for this emotion
				EmotionAppraisalVars eav = EmotionAppraisalVars();

				eav.setDesirability(histJoy->getAppraisalVariables().desirability);
				eav.setPraiseworthy(histJoy->getAppraisalVariables().praiseworthy);
				eav.setAgency(false);

				tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
					fPerEmoRel->getEmotionBaseline(fPersonality, type), eav, elicitor));
				usedEmotions.insert(admiration);
				history->removeEmotionByElicitor(histJoy, elicitor);
			}
		}
		// current emotions elicited at the same time
		if (joy != nullptr && admiration != nullptr && joy->getElicitor() != nullptr
			&& joy->getElicitor()->operator==(admiration->getElicitor())) {
			EmotionType type = EmotionType::Gratitude;
			std::shared_ptr<Emotion::Elicit> elicitor = admiration->getElicitor();
			double intensity = max(joy->getIntensity(), admiration->getIntensity());
			intensity = adjustIntensity(intensity, type, mood);
			tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
				fPerEmoRel->getEmotionBaseline(fPersonality, type), elicitor));
			usedEmotions.insert(joy);
			usedEmotions.insert(admiration);
		}
		// Checking emotion history, if a suitable emotion with the same elicitor
		// exisits - but only if one of the needed emotion is missing.
		// If so, use this emotion!
		// Compount Emotion Remorse
		histDistress = ((distress == nullptr) && (shame != nullptr))
			? history->getEmotionByElicitor(EmotionType::Distress, shame->getElicitor()->Emo) : nullptr;
		histShame = ((distress != nullptr) && (shame == nullptr))
			? history->getEmotionByElicitor(EmotionType::Shame, distress->getElicitor()->Emo) : nullptr;
		if ((histDistress != nullptr) || (histShame != nullptr)) {
			EmotionType type = EmotionType::Remorse;
			if (distress != nullptr) {
				std::shared_ptr<Emotion::Elicit> elicitor = distress->getElicitor();
				double intensity = max(distress->getIntensity(), histShame->getIntensity());
				intensity = adjustIntensity(intensity, type, mood);

				// collect all relevant appraisal variables for this emotion
				EmotionAppraisalVars eav = EmotionAppraisalVars();

				eav.setDesirability(distress->getAppraisalVariables().desirability);
				eav.setPraiseworthy(histShame->getAppraisalVariables().praiseworthy);
				eav.setAgency(true);

				tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
					fPerEmoRel->getEmotionBaseline(fPersonality, type), eav, elicitor));
				usedEmotions.insert(distress);
				history->removeEmotionByElicitor(histShame, elicitor);
			}
			if (shame != nullptr) {
				std::shared_ptr<Emotion::Elicit> elicitor = shame->getElicitor();
				double intensity = max(shame->getIntensity(), histDistress->getIntensity());
				intensity = adjustIntensity(intensity, type, mood);

				// collect all relevant appraisal variables for this emotion
				EmotionAppraisalVars eav = EmotionAppraisalVars();

				eav.setDesirability(histDistress->getAppraisalVariables().desirability);
				eav.setPraiseworthy(shame->getAppraisalVariables().praiseworthy);
				eav.setAgency(true);

				tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
					fPerEmoRel->getEmotionBaseline(fPersonality, type), eav, elicitor));
				usedEmotions.insert(shame);
				history->removeEmotionByElicitor(histDistress, elicitor);
			}
		}
		// current emotions elicited at the same time
		if (distress != nullptr && shame != nullptr && distress->getElicitor() != nullptr
			&& distress->getElicitor()->operator==(shame->getElicitor())) {
			EmotionType type = EmotionType::Remorse;
			std::shared_ptr<Emotion::Elicit> elicitor = shame->getElicitor();
			double intensity = max(distress->getIntensity(), shame->getIntensity());
			intensity = adjustIntensity(intensity, type, mood);

			// collect all relevant appraisal variables for this emotion
			EmotionAppraisalVars eav = EmotionAppraisalVars();

			eav.setDesirability(distress->getAppraisalVariables().desirability);
			eav.setPraiseworthy(shame->getAppraisalVariables().praiseworthy);
			eav.setAgency(true);

			tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
				fPerEmoRel->getEmotionBaseline(fPersonality, type), eav, elicitor));
			usedEmotions.insert(distress);
			usedEmotions.insert(shame);
		}
		// Checking emotion history, if a suitable emotion with the same elicitor
		// exisits - but only if one of the needed emotion is missing.
		// If so, use this emotion!
		// Compount Emotion Anger
		histDistress = ((distress == nullptr) && (reproach != nullptr))
			? history->getEmotionByElicitor(EmotionType::Distress, reproach->getElicitor()->Emo) : nullptr;
		histReproach = ((distress != nullptr) && (reproach == nullptr))
			? history->getEmotionByElicitor(EmotionType::Reproach, distress->getElicitor()->Emo) : nullptr;
		if ((histDistress != nullptr) || (histReproach != nullptr)) {
			EmotionType type = EmotionType::Anger;
			if (distress != nullptr) {
				std::shared_ptr<Emotion::Elicit> elicitor = distress->getElicitor();
				double intensity = max(distress->getIntensity(), histReproach->getIntensity());
				intensity = adjustIntensity(intensity, type, mood);

				// collect all relevant appraisal variables for this emotion
				EmotionAppraisalVars eav = EmotionAppraisalVars();

				eav.setDesirability(distress->getAppraisalVariables().desirability);
				eav.setPraiseworthy(histReproach->getAppraisalVariables().praiseworthy);
				eav.setAgency(false);

				tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
					fPerEmoRel->getEmotionBaseline(fPersonality, type), eav, elicitor));
				usedEmotions.insert(distress);
				history->removeEmotionByElicitor(histReproach, elicitor);
			}
			if (reproach != nullptr) {
				std::shared_ptr<Emotion::Elicit> elicitor = reproach->getElicitor();
				double intensity = max(reproach->getIntensity(), histDistress->getIntensity());
				intensity = adjustIntensity(intensity, type, mood);

				// collect all relevant appraisal variables for this emotion
				EmotionAppraisalVars eav = EmotionAppraisalVars();

				eav.setDesirability(histDistress->getAppraisalVariables().desirability);
				eav.setPraiseworthy(reproach->getAppraisalVariables().praiseworthy);
				eav.setAgency(false);

				tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
					fPerEmoRel->getEmotionBaseline(fPersonality, type), elicitor));
				usedEmotions.insert(reproach);
				history->removeEmotionByElicitor(histDistress, elicitor);
			}
		}
		// Current emotions elicited at the same time
		if (distress != nullptr && reproach != nullptr && distress->getElicitor() != nullptr
			&& distress->getElicitor()->operator==(reproach->getElicitor())) {

			EmotionType type = EmotionType::Anger;
			std::shared_ptr<Emotion::Elicit> elicitor = reproach->getElicitor();
			double intensity = max(distress->getIntensity(), reproach->getIntensity());
			intensity = adjustIntensity(intensity, type, mood);

			// collect all relevant appraisal variables for this emotion
			EmotionAppraisalVars eav = EmotionAppraisalVars();

			eav.setDesirability(distress->getAppraisalVariables().desirability);
			eav.setPraiseworthy(reproach->getAppraisalVariables().praiseworthy);
			eav.setAgency(true);

			tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
				fPerEmoRel->getEmotionBaseline(fPersonality, type), elicitor));
			usedEmotions.insert(distress);
			usedEmotions.insert(reproach);
		}
		// --------------------------------------------------------------------------------
		// Attraction/Attribution Group:
		// --------------------------------------------------------------------------------
		// Checking emotion history, if a suitable emotion with the same elicitor
		// exisits - but only if one of the needed emotion is missing.
		// If so, use this emotion!
		// Compount Emotion Love
		histLiking = ((liking == nullptr) && (admiration != nullptr))
			? history->getEmotionByElicitor(EmotionType::Liking, admiration->getElicitor()->Emo) : nullptr;
		histAdmiration = ((liking != nullptr) && (admiration == nullptr))
			? history->getEmotionByElicitor(EmotionType::Admiration, liking->getElicitor()->Emo) : nullptr;
		if ((histLiking != nullptr) || (histAdmiration != nullptr)) {
			EmotionType type = EmotionType::Love;
			if (liking != nullptr) {
				std::shared_ptr<Emotion::Elicit> elicitor = liking->getElicitor();
				double intensity = max(liking->getIntensity(), histAdmiration->getIntensity());
				intensity = adjustIntensity(intensity, type, mood);

				// collect all relevant appraisal variables for this emotion
				EmotionAppraisalVars eav = EmotionAppraisalVars();

				eav.setLiking(liking->getAppraisalVariables().desirability);
				eav.setPraiseworthy(histAdmiration->getAppraisalVariables().praiseworthy);
				eav.setAgency(false);

				tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
					fPerEmoRel->getEmotionBaseline(fPersonality, type), eav, elicitor));
				usedEmotions.insert(liking);
				history->removeEmotionByElicitor(histAdmiration, elicitor);
			}
			if (admiration != nullptr) {
				std::shared_ptr<Emotion::Elicit> elicitor = admiration->getElicitor();
				double intensity = max(admiration->getIntensity(), histLiking->getIntensity());
				intensity = adjustIntensity(intensity, type, mood);

				// collect all relevant appraisal variables for this emotion
				EmotionAppraisalVars eav = EmotionAppraisalVars();

				eav.setLiking(histLiking->getAppraisalVariables().desirability);
				eav.setPraiseworthy(admiration->getAppraisalVariables().praiseworthy);
				eav.setAgency(false);

				tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
					fPerEmoRel->getEmotionBaseline(fPersonality, type), elicitor));
				usedEmotions.insert(admiration);
				history->removeEmotionByElicitor(histLiking, elicitor);
			}
		}
		// Current emotions elicited at the same time
		if (liking != nullptr && admiration != nullptr && liking->getElicitor() != nullptr
			&& liking->getElicitor()->operator==(admiration->getElicitor())) {
			EmotionType type = EmotionType::Love;
			std::shared_ptr<Emotion::Elicit> elicitor = admiration->getElicitor();
			double intensity = max(liking->getIntensity(), admiration->getIntensity());
			intensity = adjustIntensity(intensity, type, mood);

			// collect all relevant appraisal variables for this emotion
			EmotionAppraisalVars eav = EmotionAppraisalVars();

			eav.setLiking(histLiking->getAppraisalVariables().desirability);
			eav.setPraiseworthy(admiration->getAppraisalVariables().praiseworthy);
			eav.setAgency(false);

			tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
				fPerEmoRel->getEmotionBaseline(fPersonality, type), elicitor));
			usedEmotions.insert(liking);
			usedEmotions.insert(admiration);
		}
		// Checking emotion history, if a suitable emotion with the same elicitor
		// exisits - but only if one of the needed emotion is missing.
		// If so, use this emotion!
		// Compount Emotion Hate
		histDisliking = ((disliking == nullptr) && (reproach != nullptr))
			? history->getEmotionByElicitor(EmotionType::Disliking, reproach->getElicitor()->Emo) : nullptr;
		histReproach = ((disliking != nullptr) && (reproach == nullptr))
			? history->getEmotionByElicitor(EmotionType::Reproach, disliking->getElicitor()->Emo) : nullptr;
		if ((histDisliking != nullptr) || (histReproach != nullptr)) {
			EmotionType type = EmotionType::Hate;
			if (disliking != nullptr) {
				std::shared_ptr<Emotion::Elicit> elicitor = disliking->getElicitor();
				double intensity = max(disliking->getIntensity(), histReproach->getIntensity());
				intensity = adjustIntensity(intensity, type, mood);

				// collect all relevant appraisal variables for this emotion
				EmotionAppraisalVars eav = EmotionAppraisalVars();

				eav.setDesirability(disliking->getAppraisalVariables().desirability);
				eav.setPraiseworthy(histReproach->getAppraisalVariables().praiseworthy);
				eav.setAgency(false);

				tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
					fPerEmoRel->getEmotionBaseline(fPersonality, type), eav, elicitor));
				usedEmotions.insert(disliking);
				history->removeEmotionByElicitor(histReproach, elicitor);
			}
			if (reproach != nullptr) {
				std::shared_ptr<Emotion::Elicit> elicitor = reproach->getElicitor();
				double intensity = max(reproach->getIntensity(), histDisliking->getIntensity());
				intensity = adjustIntensity(intensity, type, mood);

				// collect all relevant appraisal variables for this emotion
				EmotionAppraisalVars eav = EmotionAppraisalVars();

				eav.setDesirability(histDisliking->getAppraisalVariables().desirability);
				eav.setPraiseworthy(reproach->getAppraisalVariables().praiseworthy);
				eav.setAgency(false);

				tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
					fPerEmoRel->getEmotionBaseline(fPersonality, type), eav, elicitor));
				usedEmotions.insert(reproach);
				history->removeEmotionByElicitor(histDisliking, elicitor);
			}
		}
		// Current emotions elicited at the same time
		if (disliking != nullptr && reproach != nullptr && disliking->getElicitor() != nullptr
			&& disliking->getElicitor()->operator==(reproach->getElicitor())) {
			EmotionType type = EmotionType::Hate;
			std::shared_ptr<Emotion::Elicit> elicitor = reproach->getElicitor();
			double intensity = max(disliking->getIntensity(), reproach->getIntensity());
			intensity = adjustIntensity(intensity, type, mood);

			// collect all relevant appraisal variables for this emotion
			EmotionAppraisalVars eav = EmotionAppraisalVars();

			eav.setDesirability(disliking->getAppraisalVariables().desirability);
			eav.setPraiseworthy(reproach->getAppraisalVariables().praiseworthy);
			eav.setAgency(false);


			tempEmotions.push_back(std::make_shared<Emotion>(type, intensity,
				fPerEmoRel->getEmotionBaseline(fPersonality, type), eav, elicitor));
			usedEmotions.insert(disliking);
			usedEmotions.insert(reproach);
		}
		// -----------------------------------------------------
		// Delete single emotions, if we have got compound ones.
		// -----------------------------------------------------
		if (!usedEmotions.empty()) {
			for (auto& it : usedEmotions)
			{
				tempEmotions.remove(it);
			}

		}
		// -----------------------------------------------------
		// Update emotion vector and notify listeners.
		// -----------------------------------------------------
		for (auto& it : tempEmotions) {
			std::shared_ptr<Emotion> emotion = it;
			if (emotions->hasType(emotion->getType())) {
				emotions->add((emotion));
				//log.info("Elicited emotion " + emotion.getType() + " (" + prettyPrint(emotion->getIntensity()) + "/" + prettyPrint(emotion.getBaseline()) + ")");
			}
			else {
				//log.warning("Elicited emotion " + emotion.getType() + " not used! Not defined as available emotion.");
			}
		}
		return emotions;
	}
