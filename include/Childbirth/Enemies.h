#pragma once
#include "Childbirth/Enemy.h"

class Goblin : public Enemy
{
public:
	Goblin();
	void attack() const override;
};

class Orc : public Enemy
{
public:
	Orc();
	void attack() const override;
};

class Dragon : public Enemy
{
public:
	Dragon();
	void attack() const override;
};
