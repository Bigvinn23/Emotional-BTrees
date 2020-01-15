#pragma once
#include <array>
#include"Emotions/Emotion.h"
#include"Emotions/EmotionVector.h"
#include"Mood/Mood.h"
#include"PErsonality/Personality.h"
#include"Emotions/EmotionType.h"
#include"Emotions/EmotionsPADRelation.h"
#include"Emotions/EmotionPADCentroid.h"
#include<mutex>
#include"Matrix.h"
#include"glm/glm.hpp"
#include <random>

//import de.affect.manage.AffectManager;


/**
* The <code>MoodEngine</code> class is used to compute the current mood of a
* character based on it's personality and current active emotions.
*
* @author Patrick Gebhard
*
* @version 1.0
*/
 class MoodEngine {

 private: 
	 std::recursive_mutex _mutex;
	 Personality fPersonality;
	  Mood fDefaultMood ;
	  Mood fCurrentMoodTendency ;
	 EmotionPADCentroid* fEmotionsCenter = nullptr;
	 bool fNeurotismMoodChanges = false;
	 int fNeurotismMoodChangeTime = 1;
	 int fRelativeMoodOctant = 0;
	 int fLastRelativeMoodOctant = 0;
	 double fLowestValue = 0.0001;
	 long fOverallMoodReturnPeriods = 1800000; // 30 minutes
	 double fMoodReturnFactor = 1.0;
	 long fOverallMoodReturnCnt = 0;
	 int sMOODSTEPS = 10; //better 300
	 double sDEFAULTMOODIMPACT = 0.25;
	 int sNeurotismMoodChangeMaxTime = 8;
	 //Logger log = AffectManager.sLog;

	 int signum(float x){return (x > 0) ? 1 : ((x < 0) ? -1 : 0);}

	 float random() 
	 { 
		 std::random_device r;
		 std::default_random_engine random_engine(r());
		 std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
		 srand(time(NULL));

		 return distribution(random_engine);
	 }

	 Mood moveMoodLinear(Mood currentMood, double strength,
		 double pValue, double aValue, double dValue) {
		 Mood resultMood;
		 double currentPleasure = currentMood.getPleasure();
		 double currentArousal = currentMood.getArousal();
		 double currentDominance = currentMood.getDominance();
		 // get the subcube boundaries if fEmotionCenter is in the current mood octant cube
		 double newPValue = currentPleasure + (pValue / sMOODSTEPS * strength);
		 newPValue = checkValue(newPValue);
		 double newAValue = currentArousal + (aValue / sMOODSTEPS * strength);
		 newAValue = checkValue(newAValue);
		 double newDValue = currentDominance + (dValue / sMOODSTEPS * strength);
		 newDValue = checkValue(newDValue);
		 fCurrentMoodTendency = Mood(checkValue(currentPleasure + pValue),
			 checkValue(currentArousal + aValue),
			 checkValue(currentDominance + dValue));
		 if (isEmotionCenterInCurrentMoodOctantCube(currentMood)) {
			 //debug log.info("EmotionCenter is in mood cube - mood will not leave cube!");
			 double pleasureEnd = signum(currentPleasure);
			 newPValue = (pleasureEnd < 0.0) ? (newPValue > 0.0) ? 0.0 : newPValue : (newPValue < 0.0) ? 0.0 : newPValue;
			 double arousalEnd = signum(currentArousal);
			 newAValue = (arousalEnd < 0.0) ? (newAValue > 0.0) ? 0.0 : newAValue : (newAValue < 0.0) ? 0.0 : newAValue;
			 double dominanceEnd = signum(currentDominance);
			 newDValue = (dominanceEnd < 0.0) ? (newDValue > 0.0) ? 0.0 : newDValue : (newDValue < 0.0) ? 0.0 : newDValue;
		 }
		 resultMood =  Mood (newPValue, newAValue, newDValue);
		 return resultMood;
	 }


	 Mood randomMoodChange(Mood currentMood, double strength,
		 double pValue, double aValue, double dValue) {
		 // to which octant the current mood should move	
		 fRelativeMoodOctant = (random() * 8.0);
		 double newPValue = currentMood.getPleasure();
		 double newAValue = currentMood.getArousal();
		 double newDValue = currentMood.getDominance();
		 //     log.info("relative mood octant " + fRelativeMoodOctant + 
		 //  	     " P:" + ((fRelativeMoodOctant >> 2) % 2) +
		 //  	     " A:" + ((fRelativeMoodOctant >> 1) % 2) +
		 //  	     " D:" + ((fRelativeMoodOctant >> 0) % 2));
		 // allow only random values that are not opposite to the current mood 
		 // move direction
		 newPValue = ((((fRelativeMoodOctant >> 2) % 2) > 0) && (signum(pValue) >= 0)) ? newPValue + strength : newPValue - strength;
		 newPValue = checkValue(newPValue);

		 newAValue = ((((fRelativeMoodOctant >> 1) % 2) > 0) && (signum(aValue) >= 0)) ? newAValue + strength : newAValue - strength;
		 newAValue = checkValue(newAValue);

		 newDValue = ((((fRelativeMoodOctant >> 0) % 2) > 0) && (signum(dValue) >= 0)) ? newDValue + strength : newDValue - strength;
		 newDValue = checkValue(newDValue);
		 return  Mood(newPValue, newAValue, newDValue);
	 }

	  double checkValue(double value) {
		 double result = value;
		 result = (result > 1.0) ? 1.0 : result;
		 result = (result < -1.0) ? -1.0 : result;
		 return result;
	 }

	  Mood getPADPlanePoint(EmotionPADCentroid* eCenter, Mood currentMood ) {
		  Mood result =  Mood(0., 0., 0.);
		  double curMoodPValue = currentMood.getPleasure();
		  double curMoodAValue = currentMood.getArousal();
		  double curMoodDValue = currentMood.getDominance();
		  double pMoodToECenter = (eCenter->getPValue() - curMoodPValue);
		  double aMoodToECenter = (eCenter->getAValue() - curMoodAValue);
		  double dMoodToECenter = (eCenter->getDValue() - curMoodDValue);
		  //log.info("pMoodToECenter " + pMoodToECenter);
		  //log.info("aMoodToECenter " + aMoodToECenter);
		  //log.info("dMoodToECenter " + dMoodToECenter);
		  if ((abs(pMoodToECenter) <= fLowestValue) &&
			  (abs(aMoodToECenter) <= fLowestValue) &&
			  (abs(dMoodToECenter) <= fLowestValue)) {
			  return Mood();//suppose to be null
		  }
	//	  using mat3 = glm::tmat3x3<double, glm::highp>;
		//  using vec3 = glm::tvec3<double, glm::highp>;
		  std::vector<std::vector<std::vector<double>>> inputVals = {
			  { { pMoodToECenter, -1., 0. }, {aMoodToECenter, 0., 0.}, {dMoodToECenter, 0., -1. }},
			  { { pMoodToECenter, -1., 0.},  {aMoodToECenter, 0., 0.}, {dMoodToECenter, 0., -1. }},
			  { { pMoodToECenter, 0., 0.},   {aMoodToECenter, -1., 0.}, {dMoodToECenter, 0., -1. }},
			  { { pMoodToECenter, 0., 0. },  {aMoodToECenter, -1., 0.} ,{dMoodToECenter, 0., -1.  }},
			  { { pMoodToECenter, -1., 0.},  {aMoodToECenter, 0., -1.}, {dMoodToECenter, 0., 0.  }},
			  { { pMoodToECenter, -1., 0.},  {aMoodToECenter, 0., -1.}, {dMoodToECenter, 0., 0.  }}
		  };
		  
		  std::vector<std::vector<std::array<double, 3>>> bVals = {
			  { {{ 0 }, { -1. - curMoodAValue }, { 0 }} },
			  { {{ 0 }, {  1. - curMoodAValue }, { 0 }} },
			  { {{ -1. - curMoodPValue }, { 0 }, { 0 }} },
			  { {{  1. - curMoodPValue }, { 0 }, { 0 }} },
			  { {{ 0 }, { 0 }, { -1. - curMoodDValue }} },
			  { {{ 0 }, { 0 }, {  1. - curMoodDValue }} }
		  };
		  for (int i = 0; i < 6; i++) {
			  // check if end point is in neagtive pleasure plane
			  Matrix A = Matrix(inputVals[i]);
			  Matrix b = Matrix(bVals[i]);

			  double fac = 0.0;
			  if (A.det() != 0.0) {
				  Matrix x = A.solve(b);
				  fac = x.get(0, 0);
			  }
			  else {
				  // special case, if only one part differs from zero. Than the current
				  // mood should move according to that part.
				  return  Mood(signum(pMoodToECenter), signum(aMoodToECenter), signum(dMoodToECenter));
			  }
			  double PPoint = curMoodPValue + fac * pMoodToECenter;
			  double APoint = curMoodAValue + fac * aMoodToECenter;
			  double DPoint = curMoodDValue + fac * dMoodToECenter;
			  if ((PPoint <= 1.0) && (PPoint >= -1.0) &&
				  (APoint <= 1.0) && (APoint >= -1.0) &&
				  (DPoint <= 1.0) && (DPoint >= -1.0)) {
				  result = (fac > 0.0) ?  Mood(PPoint, APoint, DPoint) :  Mood(-PPoint, -APoint, -DPoint);
			  }
		  }
		  return result;
	  }

	   bool isEmotionCenterInCurrentMoodOctantCube(Mood currentMood) {
		   bool isInCurrentMoodOctantCube = false;
		   double pleasureEnd = signum(currentMood.getPleasure());
		   double arousalEnd = signum(currentMood.getArousal());
		   double dominanceEnd = signum(currentMood.getDominance());
		   bool isInPleasurePart = false;
		   if ((pleasureEnd > 0.0) && (fEmotionsCenter->getPValue() > 0.0)) {
			   isInPleasurePart = true;
		   }
		   if ((pleasureEnd < 0.0) && (fEmotionsCenter->getPValue() < 0.0)) {
			   isInPleasurePart = true;
		   }
		   bool isInArousalPart = false;
		   if ((arousalEnd > 0.0) && (fEmotionsCenter->getAValue() > 0.0)) {
			   isInArousalPart = true;
		   }
		   if ((arousalEnd < 0.0) && (fEmotionsCenter->getAValue() < 0.0)) {
			   isInArousalPart = true;
		   }
		   bool isInDominancePart = false;
		   if ((dominanceEnd > 0.0) && (fEmotionsCenter->getDValue() > 0.0)) {
			   isInDominancePart = true;
		   }
		   if ((dominanceEnd < 0.0) && (fEmotionsCenter->getDValue() < 0.0)) {
			   isInDominancePart = true;
		   }
		   if (isInPleasurePart && isInArousalPart && isInDominancePart) {
			   isInCurrentMoodOctantCube = true;
		   }
		   return isInCurrentMoodOctantCube;
	   }

	   bool isMoodAtEmotionsCenter(Mood currentMood) {
		   bool isAtEmotionCenter = false;
		   double pRange = (fEmotionsCenter->getPValue() - currentMood.getPleasure());
		   double aRange = (fEmotionsCenter->getAValue() - currentMood.getArousal());
		   double dRange = (fEmotionsCenter->getDValue() - currentMood.getDominance());
		   if ((abs(pRange) < 0.001) && (abs(aRange) < 0.001) && (abs(dRange) < 0.001)) {
			   isAtEmotionCenter = true;
		   }
		   return isAtEmotionCenter;
	   }

	   bool isMoodInEmotionsCenterSubCube(Mood currentMood) {
		   bool isInEmotionsCenterSubCube = false;
		   if ((((fEmotionsCenter->getPValue() >= 0.0) &&
			   (currentMood.getPleasure() >= fEmotionsCenter->getPValue())) ||
			   ((fEmotionsCenter->getPValue() < 0.0) &&
			   (currentMood.getPleasure() <= fEmotionsCenter->getPValue()))) &&
				   (((fEmotionsCenter->getAValue() >= 0.0) &&
			   (currentMood.getArousal() >= fEmotionsCenter->getAValue())) ||
					   ((fEmotionsCenter->getAValue() < 0.0) &&
					   (currentMood.getArousal() <= fEmotionsCenter->getAValue()))) &&
						   (((fEmotionsCenter->getDValue() >= 0.0) &&
			   (currentMood.getDominance() >= fEmotionsCenter->getDValue())) ||
							   ((fEmotionsCenter->getDValue() < 0.0) &&
							   (currentMood.getDominance() <= fEmotionsCenter->getDValue())))) {
			   isInEmotionsCenterSubCube = true;
		   }
		   return isInEmotionsCenterSubCube;
	   }
 public:
	 MoodEngine(Personality personality, bool neuroticMoodChanges, Mood defaultMood, double compPeriod) {
		fPersonality = personality;
		fDefaultMood = defaultMood;
		fEmotionsCenter =  new EmotionPADCentroid();
		fNeurotismMoodChanges = neuroticMoodChanges;
		// fNeurotismMoodChangeTime is reciproc to the neurotism value.
		// The higher neurotsim is, the lower is fNeurotismMoodChangeTime.
		fNeurotismMoodChangeTime = 1 + sNeurotismMoodChangeMaxTime -((fPersonality.getNeurotism() + 1.0) * sNeurotismMoodChangeMaxTime / 2);

		fMoodReturnFactor = (fOverallMoodReturnPeriods) / compPeriod;
	}

	// synchronized
		 EmotionPADCentroid* getEmotionsCenter() {
			 std::unique_lock<std::recursive_mutex> lock(_mutex);
		return fEmotionsCenter;
	}

	

	/**
	* <code>isMoodAtEmotionsCenter</code> checks is the current mood is at the
	* emotion(s) center postion.
	*
	* @param currentMood the current mood that will be checked if it is at the
	*                    emotion(s) center position.
	*
	* @return bool true if the current mood is near (&lt; 0.004d) the
	*                 emotion(s) center postion, false otherwise
	*/
	

	 

		 Mood getCurrentMoodTendency() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		return (fCurrentMoodTendency.getEmpty() == false) ? fCurrentMoodTendency : fDefaultMood;
	}

	/**
	* <code>compute</code> computes the new current mood based on the actual
	* current mood and the active emotion(s) of a character.
	*
	* @param currentMood the current mood that will be checked if it is at the
	*                    emotion(s) center position.
	* @param emotions the active emotion(s) of a character
	*
	* @return Mood the new current mood
	*/
	 Mood compute(Mood currentMood, std::shared_ptr<EmotionVector> emotions) {
		 std::unique_lock<std::recursive_mutex> lock(_mutex);
		// increment mood return counter to 0
		fOverallMoodReturnCnt++;
		fEmotionsCenter->clear();		
		for (auto& it : emotions->getEmotions()) {
			EmotionType eType = it->getType();
			double eIntensity = it->getIntensity();
			if (eIntensity > it->getBaseline()) {
				// log.info("Active emotion " + eType + " influences current mood");
				Mood ePAD = (eType == (EmotionType::Physical)) ? it->getPADValues() :EmotionsPADRelation::getEmotionPADMapping(eType);
				//log.info(eType + " PAD representation is (" + ePAD.getPleasure() + ", " +  ePAD.getArousal() + " , " + ePAD.getDominance() + ")");
				fEmotionsCenter->add(ePAD.getPleasure(), ePAD.getArousal(), ePAD.getDominance(), eIntensity);
				//log.info(fEmotionsCenter->toString());
			}
		}
		if (fEmotionsCenter->isActive()) {
			// reset mood return counter to 0
			fOverallMoodReturnCnt = 0;
			double emotionImpact = fEmotionsCenter->getWeight();
			//log.info(fPersonality + " Emotion(s) impact is " + prettyPrint(emotionImpact));
			if (isMoodAtEmotionsCenter(currentMood)) {
				//log.info(fPersonality + " Current mood " + currentMood + " is at emotion(s) center");
				currentMood = Mood(fEmotionsCenter->getPValue(), fEmotionsCenter->getAValue(),
					fEmotionsCenter->getDValue());
			}
			if ((isEmotionCenterInCurrentMoodOctantCube(currentMood)) && (fEmotionsCenter->getIntensity() <= currentMood.getIntensity())) {
				//log.info(fPersonality + " current emotion(s) pushing mood deeper in mood octant");
				Mood planePoint = getPADPlanePoint(fEmotionsCenter,  Mood(0.0, 0.0, 0.0));
				currentMood = (planePoint) ? moveMoodLinear(currentMood, emotionImpact,
					planePoint.getPleasure(),
					planePoint.getArousal(),
					planePoint.getDominance()) : currentMood;

			}
			else if (isMoodInEmotionsCenterSubCube(currentMood)) { // PG 11.3.11: this case is obsolete
																   //log.info(fPersonality + " Moving mood deeper in emotion(s) center sub cube");
				Mood planePoint = getPADPlanePoint(fEmotionsCenter,  Mood(0.0, 0.0, 0.0));
				currentMood = (planePoint) ? moveMoodLinear(currentMood, emotionImpact,
					planePoint.getPleasure(),
					planePoint.getArousal(),
					planePoint.getDominance()) : currentMood;
			}
			else {
				Mood planePoint = getPADPlanePoint(fEmotionsCenter, currentMood);
				//log.info(fPersonality + " Using intersection point (" + prettyPrint(planePoint.getPleasure()) + ", " +  prettyPrint(planePoint.getArousal()) + ", " +  prettyPrint(planePoint.getDominance()) + ")");
				
				currentMood = (planePoint) ? moveMoodLinear(currentMood, emotionImpact,
					planePoint.getPleasure(),
					planePoint.getArousal(),
					planePoint.getDominance()) : currentMood;
			}
			//Mood planePoint = getPADPlanePoint(fEmotionsCenter, currentMood);
		}
		else { // back to default mood
			if (currentMood != fDefaultMood) {
				double pMoodToDefMood = (fDefaultMood.getPleasure() - currentMood.getPleasure());
				double aMoodToDefMood = (fDefaultMood.getArousal() - currentMood.getArousal());
				double dMoodToDefMood = (fDefaultMood.getDominance() - currentMood.getDominance());
				// if current mood is very close to default mood, stop the move back process
				if ((abs(pMoodToDefMood) < 0.006) && (abs(aMoodToDefMood) < 0.006) &&
					(abs(dMoodToDefMood) < 0.006)) {
					currentMood = fDefaultMood;
				}
				else {
					//debug log.info ("Current mood drifting back to default mood");
					//debug log.info (currentMood + " -> " + fDefaultMood);
					currentMood = moveMoodLinear(currentMood, sDEFAULTMOODIMPACT * fMoodReturnFactor,
						pMoodToDefMood, aMoodToDefMood, dMoodToDefMood);
					if (fNeurotismMoodChanges) {
						currentMood = randomMoodChange(currentMood, 0.005 * fMoodReturnFactor,
							pMoodToDefMood, aMoodToDefMood, dMoodToDefMood);
					}
				}
			}
			else {
				// move mood away from default mood, ...
				if (fNeurotismMoodChanges) {
					//debug log.info("Crazy mood change");
					currentMood = randomMoodChange(currentMood, 0.05, //initial "jump" from default mood
						random() * 2.0 - 1.0, random() * 2.0 - 1.0, random() * 2.0 - 1.0);
				}
			}
		}
		return currentMood;
	}
};