#include "Childbirth/Weapon.h"

//  Weapon base class

Weapon::Weapon(const std::string &name, int power, const std::string &type)
	: name_(name), power_(power), type_(type) {}

Weapon::~Weapon() = default;

void Weapon::showInfo() const
{
	std::cout << "\t[Weapon: " << name_ << "] Type: " << type_
			  << " | Power: " << power_ << std::endl;
}

std::string Weapon::getName() const { return name_; }
int Weapon::getPower() const { return power_; }
std::string Weapon::getType() const { return type_; }

//  Forest weapons

WoodBow::WoodBow() : Weapon("Wooden Bow", 15, "Ranged") {}

void WoodBow::use() const
{
	std::cout << "\t[Wooden Bow] Shoots an arrow! Damage: " << power_ << std::endl;
}

HunterKnife::HunterKnife() : Weapon("Hunter Knife", 10, "Melee") {}

void HunterKnife::use() const
{
	std::cout << "\t[Hunter Knife] Quick slash! Damage: " << power_ << std::endl;
}

//  Dungeon weapons

IronSword::IronSword() : Weapon("Iron Sword", 30, "Melee") {}

void IronSword::use() const
{
	std::cout << "\t[Iron Sword] Powerful strike! Damage: " << power_ << std::endl;
}

CrossBow::CrossBow() : Weapon("Crossbow", 25, "Ranged") {}

void CrossBow::use() const
{
	std::cout << "\t[Crossbow] Bolt pierces armor! Damage: " << power_ << std::endl;
}

//  Volcano weapons

FlameSword::FlameSword() : Weapon("Flame Sword", 60, "Melee") {}

void FlameSword::use() const
{
	std::cout << "\t[Flame Sword] Burning strike! Damage: " << power_ << std::endl;
}

LavaCannon::LavaCannon() : Weapon("Lava Cannon", 80, "Ranged") {}

void LavaCannon::use() const
{
	std::cout << "\t[Lava Cannon] Lava explosion! Damage: " << power_ << std::endl;
}
