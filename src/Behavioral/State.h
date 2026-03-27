#pragma once
#include <iostream>
#include <string>
#include <memory>

// Forward
class BossEnemy;

//  State interface
class IState
{
public:
	virtual void onEnter(BossEnemy &boss) = 0;
	virtual void onExit(BossEnemy &boss) = 0;
	virtual void update(BossEnemy &boss) = 0;
	virtual void takeDamage(BossEnemy &boss, int dmg) = 0;
	virtual std::string stateName() const = 0;
	virtual ~IState() = default;
};

//  Context
class BossEnemy
{
	std::string name_;
	int hp_;
	int maxHp_;
	int baseDamage_;
	int armor_ = 0;
	bool shieldActive_ = false;
	int minionsSpawned_ = 0;
	std::shared_ptr<IState> state_;

public:
	BossEnemy(std::string name, int hp, int dmg);

	void setState(std::shared_ptr<IState> newState);

	void update();
	void takeDamage(int dmg);

	// getters
	const std::string &getName() const;
	int getHp() const;
	int getMaxHp() const;
	int getBaseDamage() const;
	float hpRatio() const;

	void setArmor(int a);
	int getArmor() const;

	void setShield(bool on);
	bool hasShield() const;

	void spawnMinion();
	int getMinionCount() const;

	void showStatus() const;
};

//  States
class IdleState : public IState
{
public:
	std::string stateName() const override;
	void onEnter(BossEnemy &boss) override;
	void onExit(BossEnemy &) override;
	void update(BossEnemy &boss) override;
	void takeDamage(BossEnemy &boss, int dmg) override;
};

class EnragedState : public IState
{
	int turnCount_ = 0;

public:
	std::string stateName() const override;
	void onEnter(BossEnemy &boss) override;
	void onExit(BossEnemy &boss) override;
	void update(BossEnemy &boss) override;
	void takeDamage(BossEnemy &boss, int dmg) override;
};

class WoundedState : public IState
{
	int turnCount_ = 0;

public:
	std::string stateName() const override;
	void onEnter(BossEnemy &boss) override;
	void onExit(BossEnemy &boss) override;
	void update(BossEnemy &boss) override;
	void takeDamage(BossEnemy &boss, int dmg) override;
};

class DespairState : public IState
{
	int turnCount_ = 0;

public:
	std::string stateName() const override;
	void onEnter(BossEnemy &boss) override;
	void onExit(BossEnemy &) override;
	void update(BossEnemy &boss) override;
	void takeDamage(BossEnemy &boss, int dmg) override;
};
