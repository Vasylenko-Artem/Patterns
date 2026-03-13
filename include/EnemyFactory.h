#pragma once
#include <memory>
#include "Enemy.h"

// abstract class
class EnemyFactory
{
public:
	virtual ~EnemyFactory() = default;

	virtual std::unique_ptr<Enemy> createEnemy() const = 0;

	std::unique_ptr<Enemy> spawnEnemy() const;
};

class GoblinFactory : public EnemyFactory
{
public:
	std::unique_ptr<Enemy> createEnemy() const override;
};

class OrcFactory : public EnemyFactory
{
public:
	std::unique_ptr<Enemy> createEnemy() const override;
};

class DragonFactory : public EnemyFactory
{
public:
	std::unique_ptr<Enemy> createEnemy() const override;
};

std::unique_ptr<EnemyFactory> getFactoryForLevel(const std::string &level);
