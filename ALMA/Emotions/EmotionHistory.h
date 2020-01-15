#pragma once
#include<string>
#include<vector>
#include"EmotionVector.h"
#include<mutex>
class EmotionHistory {

private:
	std::vector<std::shared_ptr<EmotionVector>> history;
	// Logger log = Logger.getLogger(Logger.GLOBAL_LOGGER_NAME);

	/**
	* Constructs an empty history.
	*/
	std::recursive_mutex _mutex;
public:
	EmotionHistory() {
		history = std::vector<std::shared_ptr<EmotionVector>>();
	}

	/**
	* Removes all elements from the emotion history.

	synchronized*/ void clear() { std::unique_lock<std::recursive_mutex> lock(_mutex); history.clear(); }

	/**
	* Appends the specified element to the end of the history.
	* If the history is full (size() == length()) then the first element
	* (index = 0) is removed thereby shifting any subsequent elements to
	* the left (subtracting one from their indices) before adding the new
	* element to the end of the history.
	*
	* @param emotions the emotions to be appended to this history
	*/
	//synchronized 
	void add(std::shared_ptr<EmotionVector> emotions) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		history.push_back(emotions);
	}

	/**
	* Returns the number of elements in this history.
	* The size specifies the current number of elements in
	* the emotion history.
	*
	*/
	//synchronized
	int size() { std::unique_lock<std::recursive_mutex> lock(_mutex); return history.size(); }

	/**
	* Returns the element at the specified position in this history.
	*
	* @param index index of element to return
	* @return the element at the specified position in this history
	* @throws IndexOutOfBoundsException if index is out of range
	*                                   (index < 0 || index >= size())
	*/
	// synchronized 
	std::shared_ptr<EmotionVector> get(int index) {
		if (index < 0 || index >= size())
			throw std::invalid_argument("Index out of range: " + index);
		return history.at(index);
	}

	/**
	* Removes the element at the specified position in this history.
	* Shifts any subsequent elements to the left (subtracts one from
	* their indices).
	*
	* @param index index of element to remove
	* @throws IndexOutOfBoundsException if index is out of range
	*                                   (index < 0 || index >= size())
	*/
	//synchronized 
	void remove(int index) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		if (index < 0 || index >= size())
			throw std::invalid_argument("Index out of range: " + index);
		history.erase(history.begin() + index - 1);
	}

	/**
	* Removes the first EmotionVector element of the history, if the element
	* contains no active emotion.
	*/
	//synchronized 
	bool removeFirstIfInactive() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		if (size() > 0) {
			std::shared_ptr<EmotionVector> firstEmotionVector = history.front();
			bool remove = true;
			for (std::shared_ptr<Emotion> emotion : firstEmotionVector->getEmotions())
			{
				if (emotion->getIntensity() > emotion->getBaseline()) {
					remove = false;
				}
			}
			if (remove) {
				history.erase(history.begin());
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}

	/**
	* Returns the emotion that has the queried elicitor or null. The first
	* occurence of an emotion whoses elicitor matches the queried elicitor
	* will be returend.
	*
	* @param queryEmotionType type of emotion
	*        queryElicitor the elicitor of an emotion
	*
	* @return emotion or null
	*/
	// synchronized 
	bool getEmotionByElicitor(Emotion::Type queryEmotionType,
		std::shared_ptr<Emotion> queryElicitor, std::shared_ptr<Emotion> emotion) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		if (size() == 0) return false;
		for (int i = size() - 1; i >= 0; i--) {
			std::shared_ptr<EmotionVector> emotions = get(i);
			for (std::shared_ptr<Emotion> histEmotion : emotions->getEmotions()) {
				//debug log.info("History emotion " + histEmotion.getType() + " has " + histEmotion.getElicitor().toString() + " elicitor context queried has " + queryElicitor.toString());
				if ((histEmotion->getType() == queryEmotionType) &&
					queryElicitor->operator== (histEmotion->getElicitor()->Emo))
				{
					if (emotion == nullptr)
					{
						emotion = histEmotion;
						return true;
					}
				}
			}
		}
		return false;
	}

	std::shared_ptr<Emotion> getEmotionByElicitor(Emotion::Type queryEmotionType,
		std::shared_ptr<Emotion> queryElicitor) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		if (size() == 0) return false;
		for (int i = size() - 1; i >= 0; i--) {
			std::shared_ptr<EmotionVector> emotions = get(i);
			for (std::shared_ptr<Emotion> histEmotion : emotions->getEmotions()) {
				//debug log.info("History emotion " + histEmotion.getType() + " has " + histEmotion.getElicitor().toString() + " elicitor context queried has " + queryElicitor.toString());
				if ((histEmotion->getType() == queryEmotionType) &&
					queryElicitor == (histEmotion->getElicitor()->Emo))
				{
					return histEmotion;
				}
			}
		}
		return nullptr;
	}

	/**
	* Removes the emotion that has the queried elicitor or null.
	*
	* @param queryEmotion emotion
	*        queryElicitor the elicitor of an emotion
	*/
	// synchronized
	void removeEmotionByElicitor( std::shared_ptr<Emotion> queryEmotion,
		std::shared_ptr<Emotion::Elicit> queryElicitor) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		if (size() == 0) return;
		for (int i = size() - 1; i >= 0; i--) {
			std::shared_ptr<EmotionVector> emotions = get(i);
			for (std::shared_ptr<Emotion> histEmotion : emotions->getEmotions()) {
				if ((histEmotion == queryEmotion) &&
					(queryElicitor->operator==(histEmotion->getElicitor()))) {
					// log.fine("History emotion " + histEmotion + " has " + histEmotion.getElicitor().toString() + " will be removed");
					emotions->remove(queryEmotion->getType());
				}
			}
		}
	}


	/**
	* Returns the last element of the history or null if the history is empty.
	*
	* @return the last element or null
	*/
	//synchronized 
	std::shared_ptr<EmotionVector> lastElement() {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		if (size() == 0)
			throw std::runtime_error("Empty EmotionVector");
		const size_t lastIndex = size() - 1;
		
		return history.at(lastIndex);
	}

	/**
	* Returns the emotional state of an agent by collecting the emotions
	* with the highest intensity for each type in the history and by adding
	* them to the specified collection.
	*
	* @param emotions the collection used to store the emotional state
	* @return the new emotional state or the unmodified collection
	*         if the history is empty (size() == 0).
	*/
	// synchronized 
	std::shared_ptr<EmotionVector> getEmotionalState(std::shared_ptr<EmotionVector>& emotions) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		if (size() == 0) return emotions;
		for (const Emotion::Type type : emotions->getEmotionTypes()) {
			double max = 0.0;
			// traverse history in reverse order, latest element first
			for (int i = size() - 1; i >= 0; i--) {
				std::shared_ptr<Emotion> e = get(i)->get(type);
				if (e != nullptr && e->getIntensity() > max) {
					max = e->getIntensity();
					emotions->add(std::move(e));
				}
			}
		}
		return emotions;
	}

	/**
	* Returns a concise representation of the emotion history.
	*
	* @return a string representation of the emotion history.
	*/
	// synchronized 
	/*std::string toString() {
		std::string newLine = System.getProperty("line.separator");
		StringBuffer sb = new StringBuffer();
		sb.append("[EmotionHistory: size=" + size() + ",");
		sb.append(newLine);
		for (int i = 0; i<size(); i++) {
			EmotionVector emotions = get(i);
			sb.append("\t");
			sb.append(i + ". ");
			sb.append(emotions.getClass().getName() + '@' +
				Integer.toHexString(emotions.hashCode()));
			//sb.append(emotions);
			sb.append(newLine);
		}
		sb.append("]");
		return sb.toString();
	}*/

};