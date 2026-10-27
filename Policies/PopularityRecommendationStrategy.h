#ifndef POPULARITY_RECOMMENDATION_STRATEGY_H
#define POPULARITY_RECOMMENDATION_STRATEGY_H

#include "IRecommendationStrategy.h"

#include <utility>

class PopularityRecommendationStrategy : public IRecommendationStrategy
{
private:
	std::vector<std::pair<int, std::size_t> > popularity;

public:
	explicit PopularityRecommendationStrategy(
		const std::vector<std::pair<int, std::size_t> >& popularity);

	std::vector<int> recommend(const Menu& menu, const std::string& category,
							   std::size_t limit) const;
};

#endif
