#include "Tree.h"

Tree::Tree()
{
	leaves_m.reserve(leavesNo_g);
	
	for (size_t i = 0; i < leavesNo_g; i++)
	{
		leaves_m.emplace_back(std::make_unique<Leaf>());
	}
	
	auto trunk = std::make_shared<Branch>(ci::vec2(screenWidth_g / 2, screenHeight_g), ci::vec2(0, -1), nullptr);
	branches_m.emplace_back(trunk);

	bool found = false;
	auto current = trunk;

	while (!found)
	{
		for (auto& leaf : leaves_m)
		{
			float distance = glm::distance(current->getPosition(), leaf->getPosition());

			if (distance < maxDistance_g)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			auto branch = current->grow();
			branches_m.emplace_back(branch);
			current = branch;
		}
	}
}


void Tree::show(ci::cairo::Context &ctx)
{
	for (auto& leaf : leaves_m)
		leaf->show(ctx);
	
	for (auto& branch : branches_m)
		branch->show(ctx);
}

void Tree::grow()
{
	for (auto& leaf : leaves_m)
	{
		std::shared_ptr<Branch> closestBranch;
		float recordDistance = -1;

		for (auto& branch : branches_m)
		{
			auto distance = glm::abs(glm::distance(leaf->getPosition(), branch->getPosition()));
			if (distance < minDistance_g)
			{
				leaf->reached_m = true;
				closestBranch = nullptr;
				break;
			}
			else if (distance > maxDistance_g)
			{
			}
			else if (!closestBranch || distance < recordDistance)
			{
				closestBranch = branch;
				recordDistance = distance;
			}
		}

		if (closestBranch)
		{
			auto newDirection = leaf->getPosition() - closestBranch->getPosition();
			auto distance = glm::abs(glm::distance(leaf->getPosition(), closestBranch->getPosition()));
			
			closestBranch->setNextDirection(newDirection);
			closestBranch->nearbyLeavesCount_m++;
		}
	}

	if (leaves_m.size())
	{
		//remove reached leaves
		leaves_m.erase(std::remove_if(leaves_m.begin(), leaves_m.end(), [](const std::unique_ptr<Leaf>& leaf) {return leaf->reached_m == true; }), leaves_m.end());

		size_t originalSize = branches_m.size();
		for (size_t i = 0; i < originalSize; i++)
		{
			if (branches_m[i]->nearbyLeavesCount_m > 0)
			{
				branches_m.emplace_back(branches_m[i]->grow());
			}
			branches_m[i]->nearbyLeavesCount_m = 0;
		}

		if (branches_m.size() == originalSize) // we didn't grow, then none of the remaning will be reachable.
		{
			leaves_m.clear();
		}
	}
}

Tree::~Tree()
{
}
