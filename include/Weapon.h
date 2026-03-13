#pragma once
#include <iostream>
#include <string>

//  Weapon — second type of objects in a biome
//  (first type — enemies, already implemented via Factory Method)

class Weapon
{
public:
	Weapon(const std::string &name, int power, const std::string &type);
	virtual ~Weapon();

	virtual void use() const = 0;
	virtual void showInfo() const;

	std::string getName() const;
	int getPower() const;
	std::string getType() const;

protected:
	std::string name_;
	int power_;
	std::string type_;
};

//  Concrete weapon types

// Forest weapons
class WoodBow : public Weapon
{
public:
	WoodBow();
	void use() const override;
};

class HunterKnife : public Weapon
{
public:
	HunterKnife();
	void use() const override;
};

// Dungeon weapons
class IronSword : public Weapon
{
public:
	IronSword();
	void use() const override;
};

class CrossBow : public Weapon
{
public:
	CrossBow();
	void use() const override;
};

// Volcano weapons
class FlameSword : public Weapon
{
public:
	FlameSword();
	void use() const override;
};

class LavaCannon : public Weapon
{
public:
	LavaCannon();
	void use() const override;
};
