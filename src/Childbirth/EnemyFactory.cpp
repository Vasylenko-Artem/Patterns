#include "Childbirth/EnemyFactory.h"
#include "Childbirth/Enemies.h"
#include <stdexcept>

// EnemyFactory::spawnEnemy
std::unique_ptr<Enemy> EnemyFactory::spawnEnemy() const
{
	auto enemy = createEnemy();
	std::cout << "  Spawn: ";
	enemy->showInfo();
	return enemy;
}

// GoblinFactory
std::unique_ptr<Enemy> GoblinFactory::createEnemy() const
{
	return std::make_unique<Goblin>();
}

// OrcFactory
std::unique_ptr<Enemy> OrcFactory::createEnemy() const
{
	return std::make_unique<Orc>();
}

// DragonFactory
std::unique_ptr<Enemy> DragonFactory::createEnemy() const
{
	return std::make_unique<Dragon>();
}

// Utils
std::unique_ptr<EnemyFactory> getFactoryForLevel(const std::string &level)
{
	if (level == "easy")
		return std::make_unique<GoblinFactory>();
	if (level == "medium")
		return std::make_unique<OrcFactory>();
	if (level == "hard")
		return std::make_unique<DragonFactory>();
	throw std::invalid_argument("Uknown level: " + level);
}
