#pragma once
#include <iostream>
#include <string>
#include <vector>

//
// Pattern 1 — Template Method
//

class CombatUnit
{
public:
	void executeCombatRound(const std::string &enemyName, int enemyHp);
	virtual ~CombatUnit() = default;

protected:
	virtual std::string unitName() const = 0;
	virtual void selectTarget(const std::string &enemyName) = 0;
	virtual void performAttack(int enemyHp) = 0;
	virtual void checkVictoryCondition(int enemyHp) = 0;

	// Hooks
	virtual void prepareForBattle();
	virtual void applySpecialEffect();
};

//  Warrior
class Warrior : public CombatUnit
{
	int strength_;
	int armor_;

public:
	Warrior(int strength, int armor);

protected:
	std::string unitName() const override;
	void prepareForBattle() override;
	void selectTarget(const std::string &enemyName) override;
	void performAttack(int enemyHp) override;
	void applySpecialEffect() override;
	void checkVictoryCondition(int enemyHp) override;
};

//  Mage
class Mage : public CombatUnit
{
	int spellPower_;
	std::vector<std::string> spellBook_;

public:
	Mage(int spellPower, std::vector<std::string> spells);

protected:
	std::string unitName() const override;
	void selectTarget(const std::string &enemyName) override;
	void performAttack(int enemyHp) override;
	void applySpecialEffect() override;
	void checkVictoryCondition(int enemyHp) override;
};

//  Assassin
class Assassin : public CombatUnit
{
	int baseDamage_;
	bool stealthActive_ = false;

public:
	explicit Assassin(int baseDamage);

protected:
	std::string unitName() const override;
	void prepareForBattle() override;
	void selectTarget(const std::string &enemyName) override;
	void performAttack(int enemyHp) override;
	void checkVictoryCondition(int enemyHp) override;
};
