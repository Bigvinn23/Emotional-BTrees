#pragma once
#include<string>
 class Agency {
 private:
	 std::string name;
	/** The character itself is responsible for the action. */
 public:
	 static  Agency SELF ;
	/** Another person is responsible for the action. */
	 static  Agency OTHER ;
	

	/**
	* Hide default constructor.
	*/
	 Agency(std::string Name) {
		name = Name;
	//	SELF = Agency("self");
		/** Another person is responsible for the action. */
		//OTHER = Agency("other");
	}
	 std::string getAgency()
	 {
		 return name;
	 }
 };
 Agency Agency::SELF = Agency("self");
 Agency Agency::OTHER = Agency("other");