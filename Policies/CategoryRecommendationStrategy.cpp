#include "CategoryRecommendationStrategy.h"

std::vector<int> CategoryRecommendationStrategy::recommend(
	const Menu& menu, const std::string& category, std::size_t limit) const
{
	std::vector<int> result;
	for (int index = 0; index < menu.size() && result.size() < limit; ++index)
	{
		const MenuItem* item = menu.getItemAt(index);
		if (item != nullptr && item->isAvailable() &&
			(category.empty() || item->getCategory() == category))
		{
			result.push_back(item->getId());
		}
	}
	return result;
}
