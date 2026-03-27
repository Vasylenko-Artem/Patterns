#include "Behavioral/Strategy.h"
#include <algorithm>

//
// AggressiveStrategy
//

std::string AggressiveStrategy::name() const
{
	return "Aggressive";
}

std::string AggressiveStrategy::execute(const BattleContext &ctx)
{
	if (ctx.enemies.empty())
		return ctx.unitName + " finds no targets.";

	auto weakest = std::min_element(
		ctx.enemies.begin(), ctx.enemies.end(),
		[](const BattleContext::Target &a, const BattleContext::Target &b)
		{ return a.hp < b.hp; });

	int dmg = ctx.unitBaseDamage;

	return ctx.unitName + " [Aggressive] strikes \"" +
		   weakest->name + "\" for " + std::to_string(dmg) + " dmg.";
}

//
// DefensiveStrategy
//

DefensiveStrategy::DefensiveStrategy(int heal)
	: healAmount_(heal) {}

std::string DefensiveStrategy::name() const
{
	return "Defensive";
}

std::string DefensiveStrategy::execute(const BattleContext &ctx)
{
	bool lowHp = ctx.unitHp < ctx.unitMaxHp / 2;

	if (lowHp)
	{
		return ctx.unitName + " heals for " +
			   std::to_string(healAmount_);
	}

	if (ctx.enemies.empty())
		return ctx.unitName + " holds position.";

	auto strongest = std::max_element(
		ctx.enemies.begin(), ctx.enemies.end(),
		[](const BattleContext::Target &a, const BattleContext::Target &b)
		{ return a.hp < b.hp; });

	return ctx.unitName + " attacks strongest \"" +
		   strongest->name + "\".";
}

//
// BerserkerStrategy
//

std::string BerserkerStrategy::name() const
{
	return "Berserker";
}

std::string BerserkerStrategy::execute(const BattleContext &ctx)
{
	if (ctx.enemies.empty())
		return ctx.unitName + " rages at nobody.";

	float hpRatio = static_cast<float>(ctx.unitHp) / ctx.unitMaxHp;
	float mult = 1.0f;

	if (hpRatio <= 0.75f)
		mult = 1.5f;
	if (hpRatio <= 0.50f)
		mult = 2.0f;
	if (hpRatio <= 0.25f)
		mult = 3.0f;

	const auto &target = ctx.enemies.front();
	int dmg = static_cast<int>(ctx.unitBaseDamage * mult);

	return ctx.unitName + " [Berserker] hits \"" +
		   target.name + "\" for " + std::to_string(dmg);
}

//
// SupportStrategy
//

SupportStrategy::SupportStrategy(int buff)
	: buffAmount_(buff) {}

std::string SupportStrategy::name() const
{
	return "Support";
}

std::string SupportStrategy::execute(const BattleContext &ctx)
{
	if (!ctx.allies.empty())
	{
		auto strongest = std::max_element(
			ctx.allies.begin(), ctx.allies.end(),
			[](const BattleContext::Target &a, const BattleContext::Target &b)
			{ return a.hp < b.hp; });

		return ctx.unitName + " buffs \"" +
			   strongest->name + "\"";
	}

	if (ctx.enemies.empty())
		return ctx.unitName + " stands by.";

	const auto &target = ctx.enemies.front();
	return ctx.unitName + " attacks \"" + target.name + "\".";
}

//
// EnemyAI
//

EnemyAI::EnemyAI(std::string name, int hp, int dmg,
				 std::shared_ptr<ICombatStrategy> strategy)
	: name_(std::move(name)),
	  hp_(hp),
	  maxHp_(hp),
	  baseDamage_(dmg),
	  strategy_(std::move(strategy)) {}

void EnemyAI::setStrategy(std::shared_ptr<ICombatStrategy> s)
{
	std::cout << "  [" << name_ << "] Strategy: "
			  << strategy_->name() << " -> " << s->name() << "\n";
	strategy_ = std::move(s);
}

void EnemyAI::takeDamage(int dmg)
{
	hp_ = std::max(0, hp_ - dmg);
}

void EnemyAI::takeTurn(
	const std::vector<BattleContext::Target> &enemies,
	const std::vector<BattleContext::Target> &allies)
{
	BattleContext ctx;
	ctx.unitName = name_;
	ctx.unitHp = hp_;
	ctx.unitMaxHp = maxHp_;
	ctx.unitBaseDamage = baseDamage_;
	ctx.enemies = enemies;
	ctx.allies = allies;

	std::string result = strategy_->execute(ctx);
	std::cout << "  " << result << "\n";
}

void EnemyAI::showStatus() const
{
	std::cout << "  [" << name_ << "] HP:" << hp_
			  << "/" << maxHp_
			  << " DMG:" << baseDamage_
			  << " Strategy:" << strategy_->name() << "\n";
}

int EnemyAI::getHp() const { return hp_; }
int EnemyAI::getMaxHp() const { return maxHp_; }
const std::string &EnemyAI::getName() const { return name_; }
