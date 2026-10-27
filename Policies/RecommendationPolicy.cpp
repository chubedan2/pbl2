#include "RecommendationPolicy.h"

RecommendationPolicy::RecommendationPolicy(
	const IRecommendationStrategy& strategy)
	: strategy(&strategy)
{
}

void RecommendationPolicy::setStrategy(const IRecommendationStrategy& strategy)
{
	this->strategy = &strategy;
}

std::vector<int> RecommendationPolicy::recommend(
	const Menu& menu, const std::string& category, std::size_t limit) const
{
	return strategy->recommend(menu, category, limit);
}
