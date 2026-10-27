#ifndef I_RECOMMENDATION_STRATEGY_H
#define I_RECOMMENDATION_STRATEGY_H

#include <cstddef>
#include <string>
#include <vector>

#include "../Models/Menu.h"

class IRecommendationStrategy
{
public:
	virtual ~IRecommendationStrategy() {}
	virtual std::vector<int> recommend(const Menu& menu,
									  const std::string& category,
									  std::size_t limit) const = 0;
};

#endif
