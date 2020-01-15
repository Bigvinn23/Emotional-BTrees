#include "PersonalityEmotionsRelations.h"
#include "Personality.h"

 double PersonalityEmotionsRelations::getPersonalityInfluence( const Personality& personality, EmotionType type) const {
	Influence influence = getInfluence(type);
	return  (fPersonalityEmotionInfluence *
		personality.getOpenness() * influence.opennessInfluence +
			personality.getConscientiousness() * influence.conscientiousnessInfluence +
			personality.getExtraversion() * influence.extraversionInfluence +
			personality.getAgreeableness() * influence.agreeablenessInfluence +
			personality.getNeurotism() * influence.neurotismInfluence);
}



static const std::unordered_map<EmotionType, Influence> Relations = {
	{ EmotionType::Undefined, Influence(Null, Null, Null, Null, Null) },
	{ EmotionType::Joy, Influence(Pos, Null, Pos, Null, Null) },
	{ EmotionType::Distress, Influence(Pos, Null, Null, Null, Pos) },
	{ EmotionType::HappyFor, Influence(Pos, Null, Pos, Pos, Null) },
	{ EmotionType::Gloating, Influence(Pos, Pos, Null, Neg, Pos) },
	{ EmotionType::Resentment, Influence(Pos, Pos, Null, Neg, Pos) },
	{ EmotionType::Pity, Influence(Pos, Null, Null, Pos, Pos) },
	{ EmotionType::Hope, Influence(Null, Null, Pos, Null, Null) },
	{ EmotionType::Fear, Influence(Neg, Null, Null, Null, Pos) },
	{ EmotionType::Satisfaction, Influence(Pos, Null, Pos, Null, Null) },
	{ EmotionType::Relief, Influence(Null, Null, Pos, Null, Null) },
	{ EmotionType::FearsConfirmed, Influence(Neg, Null, Null, Null, Pos) },
	{ EmotionType::Disappointment, Influence(Neg, Null, Null, Null, Pos) },
	{ EmotionType::Pride, Influence(Pos, Null, Pos, Null, Null) },
	{ EmotionType::Admiration, Influence(Pos, Null, Pos, Pos, Null) },
	{ EmotionType::Shame, Influence(Neg, Null, Null, Null, Pos) },
	{ EmotionType::Reproach, Influence(Null, Null, Null, Neg, Pos) },
	{ EmotionType::Liking, Influence(Pos, Null, Pos, Pos, Null) },
	{ EmotionType::Disliking, Influence(Null, Null, Null, Neg, Pos) },
	{ EmotionType::Gratitude, Influence(Pos, Null, Pos, Null, Null) },
	{ EmotionType::Anger, Influence(Null, Null, Null, Neg, Pos) },
	{ EmotionType::Gratification, Influence(Pos, Null, Pos, Null, Null) },
	{ EmotionType::Remorse, Influence(Pos, Null, Null, Null, Pos) },
	{ EmotionType::Love, Influence(Pos, Null, Pos, Pos, Null) },
	{ EmotionType::Hate, Influence(Pos, Null, Null, Neg, Pos) }
};

Influence getInfluence(EmotionType type)
{
	return Relations.at(type);
}