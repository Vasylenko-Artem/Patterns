#include "Childbirth/BiomeFactory.h"
#include "Childbirth/Enemies.h"
#include "Childbirth/Weapon.h"
#include <stdexcept>

std::unique_ptr<Enemy> ForestBiomeFactory::createEnemy() const
{
	return std::make_unique<Goblin>();
}

std::unique_ptr<Weapon> ForestBiomeFactory::createWeapon() const
{
	return std::make_unique<WoodBow>();
}

std::unique_ptr<Enemy> DungeonBiomeFactory::createEnemy() const
{
	return std::make_unique<Orc>();
}

std::unique_ptr<Weapon> DungeonBiomeFactory::createWeapon() const
{
	return std::make_unique<IronSword>();
}

std::unique_ptr<Enemy> VolcanoBiomeFactory::createEnemy() const
{
	return std::make_unique<Dragon>();
}

std::unique_ptr<Weapon> VolcanoBiomeFactory::createWeapon() const
{
	return std::make_unique<FlameSword>();
}

std::unique_ptr<BiomeFactory> BiomeFactoryProvider::getFactory(const std::string &biome)
{
	if (biome == "forest")
		return std::make_unique<ForestBiomeFactory>();
	if (biome == "dungeon")
		return std::make_unique<DungeonBiomeFactory>();
	if (biome == "volcano")
		return std::make_unique<VolcanoBiomeFactory>();
	throw std::invalid_argument("Uknown biom: " + biome);
}
