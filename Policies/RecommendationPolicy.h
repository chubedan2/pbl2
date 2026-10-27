#ifndef RECOMMENDATION_POLICY_H
#define RECOMMENDATION_POLICY_H

#include "IRecommendationStrategy.h"

class RecommendationPolicy
{
private:
	const IRecommendationStrategy* strategy;

public:
	explicit RecommendationPolicy(const IRecommendationStrategy& strategy);
	void setStrategy(const IRecommendationStrategy& strategy);
	std::vector<int> recommend(const Menu& menu, const std::string& category,
							   std::size_t limit) const;
};

#endif
