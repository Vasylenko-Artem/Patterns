#include "Childbirth/Enemy.h"

void Enemy::showInfo() const
{
	std::cout << "  [" << name_ << "] HP: " << hp_
			  << " | Damage: " << damage_ << std::endl;
}
