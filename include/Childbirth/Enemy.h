#pragma once
#include <iostream>
#include <string>

class Enemy
{
public:
	Enemy(const std::string &name, int hp, int damage)
	    : name_(name), hp_(hp), damage_(damage) {}

	virtual ~Enemy() = default;

	virtual void attack() const = 0;
	virtual void showInfo() const;

	std::string getName() const { return name_; }
	int getDamage() const { return damage_; }
	int getHp() const { return hp_; }

protected:
	std::string name_;
	int hp_;
	int damage_;
};
