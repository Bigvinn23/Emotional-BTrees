#pragma once
#include"eec/Desirability.h"
#include"eec/Praiseworthiness.h"
#include"eec/Appealingness.h"
#include"eec/Likelihood.h"
#include"eec/Liking.h"
#include"eec/Realization.h"
#include"eec/Agency.h"
#include<string>

/**
* The class<code> AppraisalVariables</code> holds all posssible variables
* and their values, which are the result of an subjective appraisal. Based
* on these values emotions will be generated.
*
* @author Patrick Gebhard
*
* @version 1.0
*/
class AppraisalVariables {

private:
	 std::string type;
	 std::string signal;
	 Desirability *desirability;
	 Praiseworthiness *praiseworthiness;
	 Appealingness *appealingness;
	 Likelihood *likelihood;
	 Realization *realization;
	 Agency *agency;
	 Likin *liking;
	 std::string elicitor;

public: 
	AppraisalVariables(){}
		AppraisalVariables(std::string Type,
		std::string Signal,
		Desirability *Desirability,
		Praiseworthiness *Praiseworthiness,
		Appealingness *Appealingness,
		Likelihood *Likelihood,
		Realization *Realization,
		Agency *Agency,
		Likin *Liking,
			std::string Elicitor) {
		type = Type;
		signal = Signal;
		desirability = Desirability;
		praiseworthiness = Praiseworthiness;	
		appealingness = Appealingness;
		likelihood = Likelihood;
		realization = Realization;
		agency = Agency;
		liking = Liking;
		elicitor = Elicitor;
	}

	 std::string getType() {
		return type;
	}

	 std::string getSignal() {
		return signal;
	}

	Desirability* getDesirability() {
		return desirability;
	}

	 Praiseworthiness* getPraiseworthiness() {
		return praiseworthiness;
	}

	 Appealingness* getAppealingness() {
		return appealingness;
	}

	 Likelihood* getLikelihood() {
		return likelihood;
	}

	 Realization* getRealization() {
		return realization;
	}

	 Agency* getAgency() {
		return agency;
	}

	 Likin* getLiking() {
		return liking;
	}

	 void setLiking(double x)
	 {
		 getLiking()->setDegree(getLiking()->getDegree() + x);
	 }

	 void setRealization(double x)
	 {
		 getRealization()->setDegree(getRealization()->getDegree() + x);
	 }

	 void setLikelihood(double x)
	 {
		 getLikelihood()->setDegree(getLikelihood()->getDegree() + x);
	 }

	 void setAppealingness(double x)
	 {
		 getAppealingness()->setDegree(getAppealingness()->getDegree() + x);
	 }

	 void setPraiseworthiness(double x)
	 {
		 getPraiseworthiness()->setDegree(getPraiseworthiness()->getDegree() + x);
	 }

	 void setDesirability(double x)
	 {
		 getDesirability()->setDegree(getDesirability()->getDegree() + x);
	 }
	 std::string getElicitor() {
		return elicitor;
	}




	 /*
	public String toString() {
		StringBuffer sb = new StringBuffer();
		sb.append("AppraisalVariable: Type=").append(type).append("\n\tDesirability: ");
		sb = (desirability != null) ?
			sb.append(desirability).append(" Praiseworthiness: ") :
			sb.append("- Praiseworthiness: ");
		sb = (praiseworthiness != null) ?
			sb.append(praiseworthiness).append(" Appealingness: ") :
			sb.append("- Appealingness: ");
		sb = (appealingness != null) ?
			sb.append(appealingness).append("\n\tLikelihood: ") :
			sb.append("-\n\tLikelihood: ");
		sb = (likelihood != null) ?
			sb.append(likelihood).append(" Realization: ") :
			sb.append("- Realization: ");
		sb = (realization != null) ?
			sb.append(realization).append(" Liking: ") :
			sb.append("- Liking: ");
		sb = (liking != null) ?
			sb.append(liking).append(" Agency: ") :
			sb.append("- Agency: ");
		sb = (agency != null) ?
			sb.append(agency) :
			sb.append("-");
		return sb.toString();
	}*/

};