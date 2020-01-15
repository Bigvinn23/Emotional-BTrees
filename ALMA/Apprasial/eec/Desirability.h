#pragma once
/**
* The <code>Desirability</code> class describes the desirability
* of an event. The desirability of an event is appraised in terms
* of how it facilitates or interferes with the character's goals.
*
* @author Patrick Gebhard (based on the work of Martin Klesen)
* @version 1.0
*/
#include <stdexcept>
class Desirability {

private:
	bool desirable;
	double degree;

	/**
	* Returns a new <code>Desirability</code> object using the
	* specified value to describe the degree of desirability.
	*
	* @param degree the degree of desirability.  A negative degress denotes
	*        undesirablility.
	* @throws IllegalArgumentException if the degree is not a real number
	*         in the interval <code>[-1.0,1.0]</code>.
	*/
public:
	 Desirability(double Degree) {
		if (Degree < -1.0 || Degree > 1.0)
			throw std::invalid_argument("Invalid degree ");
		desirable = (Degree < 0.0) ? false : true;
		degree = Degree;
	}
	 Desirability(){}
	/**
	* Returns <code>true</code> if the event is desirable and
	* <code>false</code> if it is undesirable.
	*
	* @return <code>true</code> if the event is desirable;
	*         <code>false</code> if it is undesirable.
	*/
	bool isDesirable() {
		return desirable;
	}

	/**
	* Returns the degree of desirability.
	*
	* @return the degree of desirability.
	*/
	 double getDegree() {
		return degree;
	}

	/**
	* Returns the absolut degree of desirability.
	*
	* @return the absolut degree of desirability.
	*/
	 double absDegree() {
		return abs(degree);
	}

	/**
	* Returns a string describing the desirability.
	*
	* @return a string describing the desirability.
	
	public String toString() {
		DecimalFormat form = new DecimalFormat("##.##");
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