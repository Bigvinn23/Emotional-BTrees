#pragma once
#include"AppraisalManager.h"


	void AppraisalManager::appraise(CharacterManager* character, AppraisalVariables* appVars,
		double intensityCorrection, std::shared_ptr<Emotion::Elicit> currentElicitor) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		if (appVars != nullptr) {
			std::shared_ptr<Emotion::Elicit> elicitor = (currentElicitor != nullptr) ? currentElicitor : nullptr;
			std::string type = appVars->getType();
			Desirability* desirability = appVars->getDesirability();
			Praiseworthiness* praiseworthiness = appVars->getPraiseworthiness();
			Appealingness* appealingness = appVars->getAppealingness();
			Likelihood* likelihood = appVars->getLikelihood();
			Realization* realization = appVars->getRealization();
			Likin* liking = appVars->getLiking();
			Agency* agency = appVars->getAgency();

			if (type == "EEC") {

				// if EEC contains a realization value, than no other variable is considered ...
				if ((realization != nullptr)) {
					Realization* corrRealization =
						new Realization(realization->getDegree()*intensityCorrection);
					character->addEEC(EEC::appraiseEvent(elicitor, corrRealization));
					return;
				}

				// eecs for compound emotions (desirability and praiseworthiness)
				if ((desirability != nullptr) && (praiseworthiness != nullptr) && (agency != nullptr) && (likelihood == nullptr) && (liking == nullptr)) {
					Desirability* corrDesirability =
						new Desirability(desirability->getDegree()*intensityCorrection);
					character->addEEC(EEC::appraiseEvent(elicitor, corrDesirability));
					Praiseworthiness* corrPraiseworthiness =
						new Praiseworthiness(praiseworthiness->getDegree()*intensityCorrection);
					character->addEEC(EEC::appraiseAction(elicitor, corrPraiseworthiness, agency));
					return;
				}

				if ((desirability != nullptr) && (likelihood == nullptr) && (liking == nullptr)) {
					Desirability* corrDesirability =
						new Desirability(desirability->getDegree()*intensityCorrection);
					character->addEEC(EEC::appraiseEvent(elicitor, corrDesirability));
					return;
				}
				if ((desirability != nullptr) && (likelihood != nullptr) && (liking == nullptr)) {
					Likelihood* corrLikelihood =
						new Likelihood(likelihood->getDegree()*intensityCorrection);
					Desirability* corrDesirability =
						new Desirability(desirability->getDegree()*intensityCorrection);
					character->addEEC(EEC::appraiseEvent(elicitor, corrDesirability, corrLikelihood));
					return;
				}
				if ((desirability != nullptr) && (likelihood == nullptr) && (liking != nullptr)) {
					Likin* corrLiking =
						new Likin(liking->getDegree()*intensityCorrection);
					Desirability* corrDesirability =
						new Desirability(desirability->getDegree()*intensityCorrection);
					character->addEEC(EEC::appraiseEvent(elicitor, corrDesirability, corrLiking));
					return;
				}

				// eecs for compound emotions (praiseworthiness & appealingness)
				if ((praiseworthiness != nullptr) && (agency != nullptr) && (appealingness != nullptr)) {
					Praiseworthiness* corrPraiseworthiness =
						new Praiseworthiness(praiseworthiness->getDegree()*intensityCorrection);
					character->addEEC(EEC::appraiseAction(elicitor, corrPraiseworthiness, agency));
					Appealingness* corrAppealingness =
						new Appealingness(appealingness->getDegree()*intensityCorrection);
					character->addEEC(EEC::appraiseObject(elicitor, corrAppealingness));
					return;
				}

				// eecs for action
				if ((praiseworthiness != nullptr) && (agency != nullptr)) {
					Praiseworthiness* corrPraiseworthiness =
						new Praiseworthiness(praiseworthiness->getDegree()*intensityCorrection);
					character->addEEC(EEC::appraiseAction(elicitor, corrPraiseworthiness, agency));
					return;
				}

				// eecs for object
				if ((appealingness != nullptr)) {
					Appealingness* corrAppealingness =
						new Appealingness(appealingness->getDegree()*intensityCorrection);
					character->addEEC(EEC::appraiseObject(elicitor, corrAppealingness));
					return;
				}
				return;
			}

			if (type == "Action") {
				//sLog.info("\tas action");
				if ((praiseworthiness != nullptr) && (agency != nullptr)) {
					Praiseworthiness* corrPraiseworthiness =
						new Praiseworthiness(praiseworthiness->getDegree()*intensityCorrection);
					character->addEEC(EEC::appraiseAction(elicitor, corrPraiseworthiness, agency));
				}
				return;
			}
			if (type == "Event") {
				//sLog.info("\tas event");
				if ((realization != nullptr)) {
					Realization* corrRealization =
						new Realization(realization->getDegree()*intensityCorrection);
					character->addEEC(EEC::appraiseEvent(elicitor, corrRealization));
					return;
				}
				if ((desirability != nullptr) && (likelihood == nullptr) && (liking == nullptr)) {
					Desirability* corrDesirability =
						new Desirability(desirability->getDegree()*intensityCorrection);
					character->addEEC(EEC::appraiseEvent(elicitor, corrDesirability));
					return;
				}
				if ((desirability != nullptr) && (likelihood != nullptr) && (liking == nullptr)) {
					Likelihood* corrLikelihood =
						new Likelihood(likelihood->getDegree()*intensityCorrection);
					Desirability* corrDesirability =
						new Desirability(desirability->getDegree()*intensityCorrection);
					character->addEEC(EEC::appraiseEvent(elicitor, corrDesirability, corrLikelihood));
					return;
				}
				if ((desirability != nullptr) && (likelihood == nullptr) && (liking != nullptr)) {
					Likin* corrLiking =
						new Likin(liking->getDegree()*intensityCorrection);
					Desirability* corrDesirability =
						new Desirability(desirability->getDegree()*intensityCorrection);
					character->addEEC(EEC::appraiseEvent(elicitor, corrDesirability, corrLiking));
					return;
				}
				return;
			}
			if (type == "Object") {
				//sLog.info("\tas object");
				if ((appealingness != nullptr)) {
					Appealingness* corrAppealingness =
						new Appealingness(appealingness->getDegree()*intensityCorrection);
					character->addEEC(EEC::appraiseObject(elicitor, corrAppealingness));
				}
				return;
			}
		}
		else {
			//sLog.info("Nothing to appraise!");
		}
	}




	AppraisalManager::AppraisalManager() {}

	void AppraisalManager::processBasicECC(AppraisalVariables* appVar, CharacterManager *performer, std::shared_ptr<Emotion::Elicit> elicitor)
	{
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		//sLog.info("Appraising eec for " + performer.getName());
		// intensity has to be 1.0d because eec value will be multiplied by this.
		appraise(performer, appVar, 1.0, elicitor);
		performer->inferEmotions();
	}


	void AppraisalManager::processPADInput(CharacterManager* performer, Mood m, double intensity, std::string description)
	{
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		performer->infuseBioSignalEmotions(std::make_shared<PADEmotion>(m, intensity, description));
	}

