#pragma once
#include"../Personality/Personality.h"
#include"../Mood/Mood.h"
#include<list>
#include"../Personality/PersonalityMoodRelations.h"
#include <unordered_map>
#include"PADEmotion.h"
#include"EmotionType.h"
#include<mutex>
#include"Emotion.h"

class EmotionVector  {//implements Comparator<Emotion>

	/** maps emotion types to emotions */
 protected:
	 
	 std::recursive_mutex _mutex;
	 std::unordered_map <Emotion::Type, std::shared_ptr<Emotion>> emotions;
 public:
	// static Logger log = Logger.getLogger(Logger.GLOBAL_LOGGER_NAME);

	/**
	* Creates a new <code>EmotionVector</code> for the specified
	* emotion types <br>
	* The initial intensity of the emotions in this collection is
	* the same as the baseline. The baseline is computed based on
	* the emotion type and the character's personality.
	*
	* @param personality a personality object
	* @param emotionTypes the predefined set of emotion types
	* @throws NullPointerException if one of the arguments is null or
	*                              if the list of emotion types is empty
	*/
	 EmotionVector(Personality& personality, Mood mood, const std::list<Emotion::Type>& emotionTypes) {
		if (emotionTypes.empty())
			throw std::invalid_argument("Invalid null (or empty) list of emotion types");
		/*if (mood == null)
			throw std::invalid_argument("Invalid null mood!");*/
		 if(personality == nullptr)
			throw std::invalid_argument("Invalid null personality!"); 
		// TODO INTRODUCE NEW EMOTION TYPES
		for (Emotion::Type type : emotionTypes) {
			if (type == Emotion::Type::Physical) {
				// do a separate initialisation of physical emotionsz
				Mood padValues =  Mood(0.0, 0.0, 0.0);
				std::shared_ptr<PADEmotion> pademotion = std::make_shared<PADEmotion>(padValues, 0.0, personality.toString());
				emotions.emplace(type, pademotion);
			}
			else {
				double baseline = personality.getEmotionBaseline(type);
				double moodyBaseline = personality.getEmotionBaseline(PersonalityMoodRelations::getInstance()->getMoodyPersonalityTraits(mood), type);
				baseline = (baseline + moodyBaseline) / 2.0;
				//log.info(type.toString() + " baseline= " + baseline + " mood influence= " + moodyBaseline);
				// intensity = baseline, elicitor = personality
				std::shared_ptr<Emotion> emotion = std::make_shared<Emotion>(type, baseline, baseline, personality);
				emotions.emplace(type, emotion);
			}
		}
	}

	/**
	* Compares the intensity of two emotions. Returns a negative integer,
	* zero, or a positive integer as the intensity of the first emotion is
	* less than, equal to, or greater than the intensity of the second emotion.
	*
	* @param o1 the first emotion to be compared
	* @param o2 the second emotion to be compared
	* @return a negative integer, zero, or a positive integer as the intensity
	*         of the first emotion is less than, equal to, or greater than the
	*         intensity of the second emotion
	*/
	 int compare(Emotion o1, Emotion o2) {
		// Just comparing the intensities is not enough! The emotional baseline of
		// some other emotion could be higher than an emotions intensity, which
		// baseline is 0.0.
		// Therefor the relative difference (intensity - baseline) will be used
		// for sorting emotions. The motivation is, that differences to the "normal"
		// emotional state are more "disturbing" than predefined emotional bias 
		// values
		// Old sorting:
		// if (e1.getIntensity() < e2.getIntensity()) return -1;
		// if (e1.getIntensity() > e2.getIntensity()) return 1;
		if ((o1.getIntensity() - o1.getBaseline()) <
			(o2.getIntensity() - o2.getBaseline())) return -1;
		if ((o1.getIntensity() - o1.getBaseline()) >
			(o2.getIntensity() - o2.getBaseline())) return 1;

		return 0;
	}

	/**
	* Returns the emotions sorted by their intensity in ascending order.
	*
	* @return the emotions sorted by their intensity
	* @see #compare(Object, Object)
	*/
	// synchronized
	 
	  std::list<std::shared_ptr<Emotion>> getEmotions() {
		  std::unique_lock<std::recursive_mutex> lock(_mutex);
		  std::list<std::shared_ptr<Emotion>> result;
		  for (auto kv : emotions) {
			 // keys.push_back(kv.first);      i need to sort this by intesity 
			  result.push_back(kv.second);
		  }
		  result.sort();
	 	return result;
	 }

	/**
	* Returns the emotion with the highest intensity.
	* This is a convenience method. It returns the last element
	* in the list of emotions sorted by their intensity.
	*
	* @return the emotion with the highest relative intensity.
	* @see #getEmotions()
	*/
	// synchronized 
	 std::shared_ptr<Emotion> getDominantEmotion() {
		 std::unique_lock<std::recursive_mutex> lock(_mutex);
		std::list<std::shared_ptr<Emotion>> sortedEmotions = getEmotions();
		int size = sortedEmotions.size();

		auto result = sortedEmotions.back();

		if (result->getIntensity() != result->getBaseline())
			return get(EmotionType::Undefined);

		return result;
	}

	/**
	* Returns the predefined set of emotion types as an unmodifiable list.
	*
	* @return the predefined set of emotion types.
	*/
	//	 synchronized
	  std::list<EmotionType> getEmotionTypes() {
		  std::unique_lock<std::recursive_mutex> lock(_mutex);
		  std::list<EmotionType> keys;
		  for (auto kv : emotions) {
			  keys.push_back(kv.first);
			  //vals.push_back(kv.second);
		  }
		  return keys;
	 }

	/**
	* Returns an emotion of that type or null if there is no emotion
	* with that type.
	*
	* @param type the emotion type
	* @return an emotion of that type or null
	*/
	// synchronized
	 std::shared_ptr<Emotion> get(Emotion::Type type) {
		 std::unique_lock<std::recursive_mutex> lock(_mutex);
		return emotions.at(type);
	}

	/**
	* Adds the specified emotion to the collection of emotions
	* if its type is one of the predefined emotion types.
	* The new emotion replaces the emotion with the same type.
	* The method returns true if the emotion is added to the
	* collection and false otherwise.
	*
	* @param emotion the new emotion
	* @return true if the emotion is added to the collection
	*         and false otherwise.
	* @see #getEmotionTypes()
	*/
	// synchronized
	 bool add(std::shared_ptr<Emotion> emotion) {
		 std::unique_lock<std::recursive_mutex> lock(_mutex);
		Emotion::Type type = emotion->getType();
		if (emotions.at(type)) {
			emotions.erase(type);
			emotions.insert(std::make_pair(type, (emotion)));
			return true;
		}
		else return false;
	}

	/**
	* Removes the specified emotiontype
	* @param type the emotion type to be removed
	*/
	// synchronized 
	 void remove(Emotion::Type type) {
		 std::unique_lock<std::recursive_mutex> lock(_mutex);
		 if (hasType(type))
			 emotions.erase(type);
	}

	/**
	* Queries if the emotion vector can store an emotion of the queried
	* emotion type.
	*
	* @param type type of emotion
	*
	* @return true if the emotion vector has the defined emotion type
	*/

	 bool hasType(Emotion::Type type) {
		 return emotions.find(type) != emotions.end();
	}

	/**
	* Merges a EmotionVector object with the current EmotionVector object.
	* In the case both vectors store the equal emotion(s), the old emotion(s)
	* will be replaced by the new emotion(s). Otherwise, the new emotion(s)
	* will be added to the collection.
	*
	* @param newEmotions an emotion vector
	*/
	//synchronized
	void merge(EmotionVector newEmotions) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		for (auto& newEmotion : newEmotions.emotions) {
			Emotion::Type newEmotionType = newEmotion.first;
			if (emotions.find(newEmotionType) == emotions.end()) {
				emotions.insert(std::make_pair(newEmotionType, newEmotion.second));
			}
		}
	}

	/**
	* Returns a concise representation of this collection
	* consisting of a formatted list of the sorted emotions.
	*
	* @return a string representation of the collection.
	* @see List getEmotions()
	*/
	//synchronized 
	//	std::string toString() {
	//	std::string newLine = System.getProperty("line.separator");
	//	std::string indent = "\t";
	//	StringBuffer sb = new StringBuffer(newLine);
	//	for (Iterator it = getEmotions().iterator(); it.hasNext();) {
	//		Emotion emotion = (Emotion)it.next();
	//		sb.append(indent);
	//		sb.append(emotion.toString());
	//		sb.append(newLine);
	//	}
	//	return sb.toString();
	//}
	const std::unordered_map<Emotion::Type, std::shared_ptr<Emotion>>& Getmap()
	{
		return emotions;
	}
};