#pragma once

#include <iostream>
#include <memory>
#include <string>

class IAttack
{
public:
	virtual int damage() const = 0;
	virtual std::string description() const = 0;
	virtual ~IAttack() = default;
};

// Concrete component — base hero attack
class BaseAttack : public IAttack
{
	int damage_;

public:
	explicit BaseAttack(int dmg);

	int damage() const override;
	std::string description() const override;
};

// Base decorator
class AttackDecorator : public IAttack
{
protected:
	std::shared_ptr<IAttack> wrapped_;

public:
	explicit AttackDecorator(std::shared_ptr<IAttack> a);

	int damage() const override;
	std::string description() const override;
};

// Concrete decorator — Fire Boost (+20 dmg)
class FireBoostDecorator : public AttackDecorator
{
public:
	explicit FireBoostDecorator(std::shared_ptr<IAttack> a);

	int damage() const override;
	std::string description() const override;
};

// Concrete decorator — Poison Coat (+15 dmg)
class PoisonCoatDecorator : public AttackDecorator
{
public:
	explicit PoisonCoatDecorator(std::shared_ptr<IAttack> a);

	int damage() const override;
	std::string description() const override;
};

// Concrete decorator — Critical Strike (x1.5)
class CriticalStrikeDecorator : public AttackDecorator
{
public:
	explicit CriticalStrikeDecorator(std::shared_ptr<IAttack> a);

	int damage() const override;
	std::string description() const override;
};
