#include "Childbirth/Prototype.h"

// HeroPrototype implementation

HeroPrototype::HeroPrototype(
	const std::string &name,
	const std::string &heroClass,
	int hp, int mana,
	const std::string &armor,
	const std::string &weapon,
	const std::vector<std::string> &abilities)
	: name_(name), class_(heroClass),
	  hp_(hp), mana_(mana),
	  armor_(armor), weapon_(weapon),
	  abilities_(abilities)
{
	std::cout << "  [HeroPrototype] Original created: "
			  << name_ << " (" << class_ << ")\n";
}

HeroPrototype::HeroPrototype(const HeroPrototype &) = default;

std::unique_ptr<HeroPrototype> HeroPrototype::clone() const
{
	return std::make_unique<HeroPrototype>(*this);
}

HeroPrototype &HeroPrototype::withName(const std::string &name)
{
	name_ = name;
	return *this;
}

HeroPrototype &HeroPrototype::withHp(int hp)
{
	hp_ = hp;
	return *this;
}

HeroPrototype &HeroPrototype::withWeapon(const std::string &weapon)
{
	weapon_ = weapon;
	return *this;
}

HeroPrototype &HeroPrototype::addAbility(const std::string &ability)
{
	abilities_.push_back(ability);
	return *this;
}

void HeroPrototype::showInfo() const
{
	std::cout << "\tHero (Prototype)\n";
	std::cout << "\tName:   " << name_ << std::endl;
	std::cout << "\tClass:  " << class_ << std::endl;
	std::cout << "\tHP:     " << hp_ << std::endl;
	std::cout << "\tMana:   " << mana_ << std::endl;
	std::cout << "\tArmor:  " << armor_ << std::endl;
	std::cout << "\tWeapon: " << weapon_ << std::endl;
	std::cout << "\tAbilities:\n";

	for (const auto &a : abilities_)
		std::cout << "\t  - " << a << std::endl;
}

std::string HeroPrototype::getName() const { return name_; }
std::string HeroPrototype::getClass() const { return class_; }
int HeroPrototype::getHp() const { return hp_; }

// EnemyPrototype implementation

EnemyPrototype::EnemyPrototype(
	const std::string &name,
	const std::string &type,
	int hp, int damage,
	const std::string &drop)
	: name_(name), type_(type),
	  hp_(hp), damage_(damage), drop_(drop)
{
	std::cout << "  [EnemyPrototype] Original created: "
			  << name_ << " (HP:" << hp_ << " DMG:" << damage_ << ")\n";
}

EnemyPrototype::EnemyPrototype(const EnemyPrototype &) = default;

std::unique_ptr<EnemyPrototype> EnemyPrototype::clone() const
{
	return std::make_unique<EnemyPrototype>(*this);
}

EnemyPrototype &EnemyPrototype::withName(const std::string &name)
{
	name_ = name;
	return *this;
}

EnemyPrototype &EnemyPrototype::withHp(int hp)
{
	hp_ = hp;
	return *this;
}

EnemyPrototype &EnemyPrototype::withDamage(int dmg)
{
	damage_ = dmg;
	return *this;
}

EnemyPrototype &EnemyPrototype::withDrop(const std::string &drop)
{
	drop_ = drop;
	return *this;
}

void EnemyPrototype::attack() const
{
	std::cout << "  [" << name_ << "] attacks! Damage: "
			  << damage_ << std::endl;
}

void EnemyPrototype::showInfo() const
{
	std::cout << "  [" << name_ << "] type:" << type_
			  << " HP:" << hp_
			  << " DMG:" << damage_
			  << " Drop: " << drop_
			  << std::endl;
}

std::string EnemyPrototype::getName() const { return name_; }
int EnemyPrototype::getHp() const { return hp_; }
int EnemyPrototype::getDamage() const { return damage_; }

// PrototypeRegistry implementation

void PrototypeRegistry::registerHero(
	const std::string &key,
	std::unique_ptr<HeroPrototype> proto)
{
	heroes_[key] = std::move(proto);
}

void PrototypeRegistry::registerEnemy(
	const std::string &key,
	std::unique_ptr<EnemyPrototype> proto)
{
	enemies_[key] = std::move(proto);
}

std::unique_ptr<HeroPrototype> PrototypeRegistry::cloneHero(
	const std::string &key) const
{
	auto it = heroes_.find(key);

	if (it == heroes_.end())
		throw std::invalid_argument("Hero prototype not found: " + key);

	return it->second->clone();
}

std::unique_ptr<EnemyPrototype> PrototypeRegistry::cloneEnemy(
	const std::string &key) const
{
	auto it = enemies_.find(key);

	if (it == enemies_.end())
		throw std::invalid_argument("Enemy prototype not found: " + key);

	return it->second->clone();
}

void PrototypeRegistry::listPrototypes() const
{
	std::cout << "  Registered hero prototypes:  ";
	for (const auto &[k, _] : heroes_)
		std::cout << "[" << k << "] ";

	std::cout << std::endl;

	std::cout << "  Registered enemy prototypes: ";
	for (const auto &[k, _] : enemies_)
		std::cout << "[" << k << "] ";

	std::cout << std::endl;
}
