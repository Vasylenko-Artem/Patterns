#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <vector>

//  Context Data
struct BattleContext
{
	std::string unitName;
	int unitHp;
	int unitMaxHp;
	int unitBaseDamage;

	struct Target
	{
		std::string name;
		int hp;
		int maxHp;
	};

	std::vector<Target> enemies;
	std::vector<Target> allies;
};

//  Strategy Interface
class ICombatStrategy
{
public:
	virtual std::string execute(const BattleContext &ctx) = 0;
	virtual std::string name() const = 0;
	virtual ~ICombatStrategy() = default;
};

//  Strategies
class AggressiveStrategy : public ICombatStrategy
{
public:
	std::string name() const override;
	std::string execute(const BattleContext &ctx) override;
};

class DefensiveStrategy : public ICombatStrategy
{
	int healAmount_;

public:
	explicit DefensiveStrategy(int heal = 30);

	std::string name() const override;
	std::string execute(const BattleContext &ctx) override;
};

class BerserkerStrategy : public ICombatStrategy
{
public:
	std::string name() const override;
	std::string execute(const BattleContext &ctx) override;
};

class SupportStrategy : public ICombatStrategy
{
	int buffAmount_;

public:
	explicit SupportStrategy(int buff = 20);

	std::string name() const override;
	std::string execute(const BattleContext &ctx) override;
};

//  Context
class EnemyAI
{
	std::string name_;
	int hp_;
	int maxHp_;
	int baseDamage_;
	std::shared_ptr<ICombatStrategy> strategy_;

public:
	EnemyAI(std::string name, int hp, int dmg,
			std::shared_ptr<ICombatStrategy> strategy);

	void setStrategy(std::shared_ptr<ICombatStrategy> s);
	void takeDamage(int dmg);

	void takeTurn(const std::vector<BattleContext::Target> &enemies,
				  const std::vector<BattleContext::Target> &allies);

	void showStatus() const;

	int getHp() const;
	int getMaxHp() const;
	const std::string &getName() const;
};
