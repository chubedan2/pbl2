#include "PopularityRecommendationStrategy.h"

#include <algorithm>

PopularityRecommendationStrategy::PopularityRecommendationStrategy(
	const std::vector<std::pair<int, std::size_t> >& popularity)
	: popularity(popularity)
{
	std::stable_sort(this->popularity.begin(), this->popularity.end(),
					 [](const std::pair<int, std::size_t>& left,
						const std::pair<int, std::size_t>& right) {
						 return left.second > right.second;
					 });
}

std::vector<int> PopularityRecommendationStrategy::recommend(
	const Menu& menu, const std::string& category, std::size_t limit) const
{
	std::vector<int> result;
	for (std::size_t index = 0;
		 index < popularity.size() && result.size() < limit; ++index)
	{
		const MenuItem* item = menu.findById(popularity[index].first);
		if (item != nullptr && item->isAvailable() &&
			(category.empty() || item->getCategory() == category))
		{
			result.push_back(item->getId());
		}
	}
	return result;
}
