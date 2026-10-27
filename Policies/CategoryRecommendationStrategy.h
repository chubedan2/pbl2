#ifndef CATEGORY_RECOMMENDATION_STRATEGY_H
#define CATEGORY_RECOMMENDATION_STRATEGY_H

#include "IRecommendationStrategy.h"

class CategoryRecommendationStrategy : public IRecommendationStrategy
{
public:
	std::vector<int> recommend(const Menu& menu, const std::string& category,
							   std::size_t limit) const;
};

#endif
