#pragma once
#include <memory>
#include <string>
#include "Enemy.h"
#include "Weapon.h"

class BiomeFactory
{
public:
	virtual ~BiomeFactory() = default;

	virtual std::unique_ptr<Enemy> createEnemy() const = 0;
	virtual std::unique_ptr<Weapon> createWeapon() const = 0;

	virtual std::string getBiomeName() const = 0;

	void describe() const
	{
		auto e = createEnemy();
		auto w = createWeapon();
		std::cout << "Biome [" << getBiomeName() << "]:\n";
		std::cout << "\tEnemy: ";
		e->showInfo();
		std::cout << "\tWeapon: ";
		w->showInfo();
	}
};

//  Конкретні фабрики біомів
class ForestBiomeFactory : public BiomeFactory
{
public:
	std::unique_ptr<Enemy> createEnemy() const override;
	std::unique_ptr<Weapon> createWeapon() const override;
	std::string getBiomeName() const override { return "Forest"; }
};

class DungeonBiomeFactory : public BiomeFactory
{
public:
	std::unique_ptr<Enemy> createEnemy() const override;
	std::unique_ptr<Weapon> createWeapon() const override;
	std::string getBiomeName() const override { return "Dungeon"; }
};

class VolcanoBiomeFactory : public BiomeFactory
{
public:
	std::unique_ptr<Enemy> createEnemy() const override;
	std::unique_ptr<Weapon> createWeapon() const override;
	std::string getBiomeName() const override { return "Volcano"; }
};

//  Abstract factory of factories
class BiomeFactoryProvider
{
public:
	// Це і є "фабрика що повертає фабрики"
	static std::unique_ptr<BiomeFactory> getFactory(const std::string &biome);
};
