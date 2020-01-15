#pragma once
/**
* The <code>Appealingness</code> class represents the appealingness
* of an object. The appealingness of an object is evaluated with
* respect to an character's attitudes.
*
* @author Patrick Gebhard (based on the work of Martin Klesen)
*
* @version 1.0
*/
#include <stdexcept>
class Appealingness {

private:
	 bool appealing;
	 double degree;

	/**
	* Returns a new <code>Appealingness</code> object using the
	* specified value to describe the degree of appealingness.
	*
	* @param degree the degree of appealingness. A negative degree denotes
	*        unapealingness.
	* @throws IllegalArgumentException if the degree is not a real number
	*         in the interval <code>[-1.0,1.0]</code>.
	*/
public:
	Appealingness(){}
		Appealingness(double Degree) {
		if (Degree < -1.0 || Degree > 1.0)
			throw std::invalid_argument("Invalid degree ");
		appealing = (Degree < 0.0) ? false : true;
		degree = Degree;
	}

	/**
	* Returns <code>true</code> if the object is appealing and
	* <code>false</code> if it is unappealing.
	*
	* @return <code>true</code> if the object is appealing;
	*         <code>false</code> if it is unappealing.
	*/
	 bool isAppealing() {
		return appealing;
	}

	/**
	* Returns the degree of appealingness.
	*
	* @return the degree of appealingness.
	*/
	 double getDegree() {
		return degree;
	}

	/**
	* Returns the absolut degree of appealingness.
	*
	* @return the absolut degree of appealingness.
	*/
	 double absDegree() {
		return abs(degree);
	}

	/**
	* Returns a string describing the appealingness.
	*
	* @return a string describing the appealingness.
	
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