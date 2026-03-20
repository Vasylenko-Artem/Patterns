#include "Childbirth/HeroBuilder.h"
#include <iostream>

using namespace std;

void Hero::setName(const string &name) { name_ = name; }
void Hero::setClass(const string &heroClass) { class_ = heroClass; }
void Hero::setHp(int hp) { hp_ = hp; }
void Hero::setMana(int mana) { mana_ = mana; }
void Hero::setArmor(const string &armor) { armor_ = armor; }
void Hero::setWeapon(const string &weapon) { weapon_ = weapon; }
void Hero::addAbility(const string &ability) { abilities_.push_back(ability); }

void Hero::showInfo() const
{
	cout << endl;
	cout << "\tName:     " << name_ << endl;
	cout << "\tClass:    " << class_ << endl;
	cout << "\tHP:       " << hp_ << endl;
	cout << "\tMana:     " << mana_ << endl;
	cout << "\tArmor:    " << armor_ << endl;
	cout << "\tWeapon:   " << weapon_ << endl;
	cout << "\tAbilities:\n";
	for (const auto &a : abilities_)
		cout << "\t  - " << a << endl;
	cout << endl;
}

// WarriorBuilder

WarriorBuilder::WarriorBuilder() { reset(); }

void WarriorBuilder::reset() { hero_ = Hero(); }
void WarriorBuilder::buildName(const string &name) { hero_.setName(name); }
void WarriorBuilder::buildClass() { hero_.setClass("Warrior"); }
void WarriorBuilder::buildStats()
{
	hero_.setHp(250);
	hero_.setMana(30);
}
void WarriorBuilder::buildArmor() { hero_.setArmor("Full plate armor"); }
void WarriorBuilder::buildWeapon() { hero_.setWeapon("Two-handed sword"); }

void WarriorBuilder::buildAbilities()
{
	hero_.addAbility("Berserk — +50% attack for 10 sec");
	hero_.addAbility("Shield Bash — stuns the enemy");
	hero_.addAbility("Iron Skin — -30% incoming damage");
}

Hero WarriorBuilder::getProduct()
{
	Hero result = hero_;
	reset();
	return result;
}

// MageBuilder

MageBuilder::MageBuilder() { reset(); }

void MageBuilder::reset() { hero_ = Hero(); }
void MageBuilder::buildName(const string &name) { hero_.setName(name); }
void MageBuilder::buildClass() { hero_.setClass("Mage"); }
void MageBuilder::buildStats()
{
	hero_.setHp(80);
	hero_.setMana(300);
}
void MageBuilder::buildArmor() { hero_.setArmor("Archmage robe"); }
void MageBuilder::buildWeapon() { hero_.setWeapon("Magic staff"); }

void MageBuilder::buildAbilities()
{
	hero_.addAbility("Fireball — 120 AoE damage");
	hero_.addAbility("Teleport — instant movement");
	hero_.addAbility("Freeze — stops all enemies");
	hero_.addAbility("Magic Shield — absorbs 200 damage");
}

Hero MageBuilder::getProduct()
{
	Hero result = hero_;
	reset();
	return result;
}

// RangerBuilder

RangerBuilder::RangerBuilder() { reset(); }

void RangerBuilder::reset() { hero_ = Hero(); }
void RangerBuilder::buildName(const string &name) { hero_.setName(name); }
void RangerBuilder::buildClass() { hero_.setClass("Ranger"); }
void RangerBuilder::buildStats()
{
	hero_.setHp(140);
	hero_.setMana(100);
}
void RangerBuilder::buildArmor() { hero_.setArmor("Scout leather armor"); }
void RangerBuilder::buildWeapon() { hero_.setWeapon("Elven bow"); }

void RangerBuilder::buildAbilities()
{
	hero_.addAbility("Precise Shot — x3 critical hit");
	hero_.addAbility("Trap — slows the enemy");
	hero_.addAbility("Stealth — becomes invisible");
}

Hero RangerBuilder::getProduct()
{
	Hero result = hero_;
	reset();
	return result;
}

// HeroDirector

void HeroDirector::setBuilder(HeroBuilder *builder)
{
	builder_ = builder;
}

void HeroDirector::buildFullHero(const string &name)
{
	builder_->reset();
	builder_->buildName(name);
	builder_->buildClass();
	builder_->buildStats();
	builder_->buildArmor();
	builder_->buildWeapon();
	builder_->buildAbilities();
}

void HeroDirector::buildBasicHero(const string &name)
{
	builder_->reset();
	builder_->buildName(name);
	builder_->buildClass();
	builder_->buildStats();
}
