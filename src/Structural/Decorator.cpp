#include "Structural/Decorator.h"

// BaseAttack
BaseAttack::BaseAttack(int dmg)
    : damage_(dmg)
{
}

int BaseAttack::damage() const
{
	return damage_;
}

std::string BaseAttack::description() const
{
	return "Base attack";
}

// AttackDecorator
AttackDecorator::AttackDecorator(std::shared_ptr<IAttack> a)
    : wrapped_(std::move(a))
{
}

int AttackDecorator::damage() const
{
	return wrapped_->damage();
}

std::string AttackDecorator::description() const
{
	return wrapped_->description();
}

// FireBoostDecorator
FireBoostDecorator::FireBoostDecorator(std::shared_ptr<IAttack> a)
    : AttackDecorator(std::move(a))
{
}

int FireBoostDecorator::damage() const
{
	return wrapped_->damage() + 20;
}

std::string FireBoostDecorator::description() const
{
	return wrapped_->description() + " + FireBoost(+20)";
}

// PoisonCoatDecorator
PoisonCoatDecorator::PoisonCoatDecorator(std::shared_ptr<IAttack> a)
    : AttackDecorator(std::move(a))
{
}

int PoisonCoatDecorator::damage() const
{
	return wrapped_->damage() + 15;
}

std::string PoisonCoatDecorator::description() const
{
	return wrapped_->description() + " + PoisonCoat(+15)";
}

// CriticalStrikeDecorator
CriticalStrikeDecorator::CriticalStrikeDecorator(std::shared_ptr<IAttack> a)
    : AttackDecorator(std::move(a))
{
}

int CriticalStrikeDecorator::damage() const
{
	return static_cast<int>(wrapped_->damage() * 1.5f);
}

std::string CriticalStrikeDecorator::description() const
{
	return wrapped_->description() + " + CriticalStrike(x1.5)";
}
