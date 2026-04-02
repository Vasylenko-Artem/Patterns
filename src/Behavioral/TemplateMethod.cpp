#include "Behavioral/TemplateMethod.h"

//
// CombatUnit base
//

void CombatUnit::executeCombatRound(const std::string &enemyName, int enemyHp)
{
	std::cout << "\n  [" << unitName() << "]  Combat round start \n";
	prepareForBattle();
	selectTarget(enemyName);
	performAttack(enemyHp);
	applySpecialEffect();
	checkVictoryCondition(enemyHp);
	std::cout << "  [" << unitName() << "]  Combat round end \n";
}

void CombatUnit::prepareForBattle() {}
void CombatUnit::applySpecialEffect() {}

//
// Warrior
//

Warrior::Warrior(int strength, int armor)
    : strength_(strength), armor_(armor) {}

std::string Warrior::unitName() const
{
	return "Warrior";
}

void Warrior::prepareForBattle()
{
	std::cout << "    * Warrior raises shield (armor +" << armor_ << ")\n";
}

void Warrior::selectTarget(const std::string &enemyName)
{
	std::cout << "    * Warrior charges directly at: " << enemyName << "\n";
}

void Warrior::performAttack(int)
{
	std::cout << "    * Warrior swings two-handed sword for "
		  << strength_ << " damage\n";
}

void Warrior::applySpecialEffect()
{
	std::cout << "    * Warrior triggers Berserk — next hit +50%\n";
}

void Warrior::checkVictoryCondition(int enemyHp)
{
	if (enemyHp - strength_ <= 0)
		std::cout << "    * Enemy slain! Warrior claims the kill.\n";
	else
		std::cout << "    * Enemy survives with ~"
			  << (enemyHp - strength_) << " hp remaining.\n";
}

//
// Mage
//

Mage::Mage(int spellPower, std::vector<std::string> spells)
    : spellPower_(spellPower), spellBook_(std::move(spells)) {}

std::string Mage::unitName() const
{
	return "Mage";
}

void Mage::selectTarget(const std::string &enemyName)
{
	std::cout << "    * Mage scans arcane signature of: " << enemyName << "\n";
}

void Mage::performAttack(int)
{
	std::string spell = spellBook_.empty() ? "Magic Missile" : spellBook_[0];
	std::cout << "    * Mage casts [" << spell << "] for "
		  << spellPower_ << " magic damage\n";
}

void Mage::applySpecialEffect()
{
	std::cout << "    * Mage applies Arcane Weakness — enemy magic resistance -30%\n";
}

void Mage::checkVictoryCondition(int enemyHp)
{
	if (enemyHp - spellPower_ <= 0)
		std::cout << "    * Enemy disintegrated! Mage absorbs mana essence.\n";
	else
		std::cout << "    * Enemy resists partially, ~"
			  << (enemyHp - spellPower_) << " hp left.\n";
}

//
// Assassin
//

Assassin::Assassin(int baseDamage)
    : baseDamage_(baseDamage) {}

std::string Assassin::unitName() const
{
	return "Assassin";
}

void Assassin::prepareForBattle()
{
	stealthActive_ = true;
	std::cout << "    * Assassin vanishes into shadows (Stealth ON)\n";
}

void Assassin::selectTarget(const std::string &enemyName)
{
	std::cout << "    * Assassin silently marks weak spot on: " << enemyName << "\n";
}

void Assassin::performAttack(int)
{
	int dmg = stealthActive_ ? baseDamage_ * 3 : baseDamage_;
	std::cout << "    * Assassin strikes from shadows for "
		  << dmg << " damage"
		  << (stealthActive_ ? " [Backstab x3!]" : "") << "\n";
	stealthActive_ = false;
}

void Assassin::checkVictoryCondition(int enemyHp)
{
	int effectiveDmg = baseDamage_ * 3;
	if (enemyHp - effectiveDmg <= 0)
		std::cout << "    * One-shot kill! Assassin disappears.\n";
	else
		std::cout << "    * Target wounded (~"
			  << (enemyHp - effectiveDmg) << " hp), Assassin retreats.\n";
}
