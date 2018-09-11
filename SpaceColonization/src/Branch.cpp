#include"Branch.h"

ci::Color Branch::color_s = ci::Color(160.f / 255.f, 82 / 255.f, 45 / 255.f);
extern float length_g = 5.0;

Branch::Branch(ci::vec2 position, ci::vec2 direction, std::shared_ptr<Branch> parent)
	:position_m(position), parent_m(parent),direction_m(direction)
{
}

Branch::~Branch()
{
}

ci::vec2 Branch::getPosition()
{
	return position_m;
}

ci::vec2 Branch::getDirection()
{
	return direction_m;
}

void Branch::setNextDirection(ci::vec2 newDirection)
{
	nextDirection_m += newDirection;
}

void Branch::show(ci::cairo::Context &ctx)
{
	if (parent_m)
	{
		ctx.newSubPath();
		ctx.setSource(color_s);
		ctx.line(position_m, parent_m->getPosition());
		ctx.stroke();
	}
}

std::shared_ptr<Branch> Branch::grow()
{
	ci::vec2 newDirection = direction_m;

	if (nearbyLeavesCount_m > 0)
	{
		newDirection = nextDirection_m / (float)nearbyLeavesCount_m;
		if(nearbyLeavesCount_m == 2)
			if(newDirection.y > 0)
				newDirection -= ci::vec2(3.f, 5.f);
			else
				newDirection -= ci::vec2(3.f, -5.f);
	}

	auto newDirectionNormalized = glm::normalize(newDirection);

	ci::vec2 nextPosition = position_m + newDirectionNormalized * length_g;
	auto nextBranch =  std::make_shared<Branch>(nextPosition, newDirectionNormalized, shared_from_this());
	return nextBranch;
}
