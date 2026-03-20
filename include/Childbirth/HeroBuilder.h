#pragma once
#include <string>
#include <vector>

class Hero
{
public:
	void setName(const std::string &name);
	void setClass(const std::string &heroClass);
	void setHp(int hp);
	void setMana(int mana);
	void setArmor(const std::string &armor);
	void setWeapon(const std::string &weapon);
	void addAbility(const std::string &ability);

	void showInfo() const;

	std::string getName() const { return name_; }
	int getHp() const { return hp_; }
	int getMana() const { return mana_; }

private:
	std::string name_ = "???";
	std::string class_ = "???";
	int hp_ = 0;
	int mana_ = 0;
	std::string armor_ = "none";
	std::string weapon_ = "none";
	std::vector<std::string> abilities_;
};

//  BUILDER — абстрактний інтерфейс

class HeroBuilder
{
public:
	virtual ~HeroBuilder() = default;

	virtual void reset() = 0;
	virtual void buildName(const std::string &name) = 0;
	virtual void buildClass() = 0;
	virtual void buildStats() = 0;
	virtual void buildArmor() = 0;
	virtual void buildWeapon() = 0;
	virtual void buildAbilities() = 0;
	virtual Hero getProduct() = 0;
};

//  CONCRETE BUILDER 1 — Warrior

class WarriorBuilder : public HeroBuilder
{
public:
	WarriorBuilder();
	void reset() override;
	void buildName(const std::string &name) override;
	void buildClass() override;
	void buildStats() override;
	void buildArmor() override;
	void buildWeapon() override;
	void buildAbilities() override;
	Hero getProduct() override;

private:
	Hero hero_;
};

//  CONCRETE BUILDER 2 — Mage

class MageBuilder : public HeroBuilder
{
public:
	MageBuilder();
	void reset() override;
	void buildName(const std::string &name) override;
	void buildClass() override;
	void buildStats() override;
	void buildArmor() override;
	void buildWeapon() override;
	void buildAbilities() override;
	Hero getProduct() override;

private:
	Hero hero_;
};

//  CONCRETE BUILDER 3 — Ranger

class RangerBuilder : public HeroBuilder
{
public:
	RangerBuilder();
	void reset() override;
	void buildName(const std::string &name) override;
	void buildClass() override;
	void buildStats() override;
	void buildArmor() override;
	void buildWeapon() override;
	void buildAbilities() override;
	Hero getProduct() override;

private:
	Hero hero_;
};

//  DIRECTOR

class HeroDirector
{
public:
	void setBuilder(HeroBuilder *builder);
	void buildFullHero(const std::string &name);
	void buildBasicHero(const std::string &name);

private:
	HeroBuilder *builder_ = nullptr;
};
