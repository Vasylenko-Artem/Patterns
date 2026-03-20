#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>

template <typename T>
class Prototype
{
public:
	virtual ~Prototype() = default;

	// Main method of the pattern — returns deep copy
	virtual std::unique_ptr<T> clone() const = 0;
};

// HeroPrototype — clonable hero

class HeroPrototype : public Prototype<HeroPrototype>
{
public:
	HeroPrototype(
	    const std::string &name,
	    const std::string &heroClass,
	    int hp, int mana,
	    const std::string &armor,
	    const std::string &weapon,
	    const std::vector<std::string> &abilities);

	HeroPrototype(const HeroPrototype &);

	std::unique_ptr<HeroPrototype> clone() const override;

	HeroPrototype &withName(const std::string &name);
	HeroPrototype &withHp(int hp);
	HeroPrototype &withWeapon(const std::string &weapon);
	HeroPrototype &addAbility(const std::string &ability);

	void showInfo() const;

	std::string getName() const;
	std::string getClass() const;
	int getHp() const;

private:
	std::string name_;
	std::string class_;
	int hp_;
	int mana_;
	std::string armor_;
	std::string weapon_;
	std::vector<std::string> abilities_;
};

// EnemyPrototype — clonable enemy

class EnemyPrototype : public Prototype<EnemyPrototype>
{
public:
	EnemyPrototype(
	    const std::string &name,
	    const std::string &type,
	    int hp, int damage,
	    const std::string &drop);

	EnemyPrototype(const EnemyPrototype &);

	std::unique_ptr<EnemyPrototype> clone() const override;

	EnemyPrototype &withName(const std::string &name);
	EnemyPrototype &withHp(int hp);
	EnemyPrototype &withDamage(int dmg);
	EnemyPrototype &withDrop(const std::string &drop);

	void attack() const;
	void showInfo() const;

	std::string getName() const;
	int getHp() const;
	int getDamage() const;

private:
	std::string name_;
	std::string type_;
	int hp_;
	int damage_;
	std::string drop_;
};

// PrototypeRegistry — registry of prototypes

class PrototypeRegistry
{
public:
	void registerHero(const std::string &key,
			  std::unique_ptr<HeroPrototype> proto);

	void registerEnemy(const std::string &key,
			   std::unique_ptr<EnemyPrototype> proto);

	std::unique_ptr<HeroPrototype> cloneHero(const std::string &key) const;
	std::unique_ptr<EnemyPrototype> cloneEnemy(const std::string &key) const;

	void listPrototypes() const;

private:
	std::map<std::string, std::unique_ptr<HeroPrototype>> heroes_;
	std::map<std::string, std::unique_ptr<EnemyPrototype>> enemies_;
};
