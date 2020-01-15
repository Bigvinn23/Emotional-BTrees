#pragma once
#pragma once
#include <stdexcept>
class Likin {

private:
	bool liking;
	double degree;

	/**
	* Returns a new <code>Likelihood</code> object using the specified
	* value to describe the degree of belief that an anticipated event
	* will occur.
	*
	* @param degree the degree of belief that the event will occur. A negative
	*        value denotes unlikliness.
	*
	* @throws IllegalArgumentException if the degree is not a real number
	*         in the interval <code>[-1.0,1.0]</code>.
	*/
public:
	Likin(double Degree) {
		if (Degree < -1.0 || Degree > 1.0)
			throw std::invalid_argument("Invalid degree ");
		liking = (Degree < 0.0) ? false : true;
		degree = Degree;
	}
	Likin(){}
	/**
	* Returns <code>true</code> if the event is likely and
	* <code>false</code> if it is unlikely.
	*
	* @return <code>true</code> if the event is likely;
	*         <code>false</code> if it is unlikely.
	*/
	bool isLikely() {
		return liking;
	}

	/**
	* Returns the degree of belief that the event will occur.
	*
	* @return the degree of belief that the event will occur.
	*/
	double getDegree() {
		return degree;
	}

	/**
	* Returns the absolut degree of belief that the event will occur.
	*
	* @return the absolut degree of belief that the event will occur.
	*/
	double absDegree() {
		return abs(degree);
	}

	/**
	* Returns a string describing the desirability.
	*
	* @return a string describing the desirability.

	public String toString() {
	DecimalFormat form = new DecimalFormat("#.##");
	return form.format(degree);
	}*/
	void setDegree(double x)
	{
		degree = x;
		if (degree > 1)
			degree = 1;
		if (degree < -1)
			degree = -1;
	}
};