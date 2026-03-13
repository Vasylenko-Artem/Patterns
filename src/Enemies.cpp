#include "Enemies.h"
#include "GameManager.h"

// Goblin
Goblin::Goblin() : Enemy("Goblin", 30, 8) {}

void Goblin::attack() const
{
	std::cout << "  [Goblin] Damage: " << damage_ << std::endl;
	GameManager::getInstance().addScore(-damage_);
}

// Orc
Orc::Orc() : Enemy("Orc", 80, 20) {}

void Orc::attack() const
{
	std::cout << "  [Orc] Damage: " << damage_ << std::endl;
	GameManager::getInstance().addScore(-damage_);
}

// Dragon
Dragon::Dragon() : Enemy("Dragon", 300, 55) {}

void Dragon::attack() const
{
	std::cout << "  [Dragon] Damage: " << damage_ << std::endl;
	GameManager::getInstance().addScore(-damage_);
}
