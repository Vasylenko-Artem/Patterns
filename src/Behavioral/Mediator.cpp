#include "Behavioral/Mediator.h"
#include <algorithm>

namespace med
{

	//
	// BattleUnit
	//

	BattleUnit::BattleUnit(std::string name, int hp)
		: name_(std::move(name)), hp_(hp), maxHp_(hp) {}

	void BattleUnit::setMediator(IMediator *m)
	{
		mediator_ = m;
	}

	const std::string &BattleUnit::getName() const
	{
		return name_;
	}

	int BattleUnit::getHp() const
	{
		return hp_;
	}

	bool BattleUnit::isAlive() const
	{
		return hp_ > 0;
	}

	void BattleUnit::takeDamage(int dmg)
	{
		if (!isAlive())
			return;

		hp_ -= dmg;
		if (hp_ < 0)
			hp_ = 0;

		std::cout << "    [" << name_ << "] took " << dmg
				  << " damage  ->  HP: " << hp_ << "/" << maxHp_ << "\n";

		if (mediator_)
		{
			mediator_->notify(this, "damaged", dmg);
			if (!isAlive())
				mediator_->notify(this, "died", 0);
		}
	}

	void BattleUnit::receiveHeal(int amount)
	{
		if (!isAlive())
			return;

		hp_ = std::min(hp_ + amount, maxHp_);

		std::cout << "    [" << name_ << "] healed +" << amount
				  << "  ->  HP: " << hp_ << "/" << maxHp_ << "\n";
	}

	//
	// MedHero
	//

	MedHero::MedHero(std::string name, int hp, int atk)
		: BattleUnit(std::move(name), hp), attackPower_(atk) {}

	void MedHero::attack(BattleUnit *target)
	{
		if (!isAlive() || !target)
			return;

		std::cout << "    [" << name_ << "] attacks ["
				  << target->getName() << "] for " << attackPower_ << "\n";

		target->takeDamage(attackPower_);

		if (mediator_)
			mediator_->notify(this, "attacked", attackPower_);
	}

	void MedHero::onEvent(const std::string &event, BattleUnit *source, int)
	{
		if (event == "ally_died")
			std::cout << "    [" << name_ << "] \"" << source->getName()
					  << " fell! I must fight harder!\"\n";
		else if (event == "enemy_died")
			std::cout << "    [" << name_ << "] \"Enemy down! +XP!\"\n";
	}

	//
	// MedEnemy
	//

	MedEnemy::MedEnemy(std::string name, int hp, int atk)
		: BattleUnit(std::move(name), hp), attackPower_(atk) {}

	void MedEnemy::attack(BattleUnit *target)
	{
		if (!isAlive() || !target)
			return;

		std::cout << "    [" << name_ << "] attacks ["
				  << target->getName() << "] for " << attackPower_ << "\n";

		target->takeDamage(attackPower_);

		if (mediator_)
			mediator_->notify(this, "attacked", attackPower_);
	}

	void MedEnemy::onEvent(const std::string &event, BattleUnit *, int)
	{
		if (event == "ally_died")
			std::cout << "    [" << name_ << "] *becomes enraged!*\n";
	}

	//
	// HealBot
	//

	HealBot::HealBot(std::string name, int hp, int heal)
		: BattleUnit(std::move(name), hp), healPower_(heal) {}

	void HealBot::registerAlly(BattleUnit *ally)
	{
		allies_.push_back(ally);
	}

	void HealBot::attack(BattleUnit *)
	{
		std::cout << "    [" << name_ << "] HealBot cannot attack!\n";
	}

	void HealBot::onEvent(const std::string &event, BattleUnit *source, int)
	{
		if (event != "damaged" || source == this)
			return;

		BattleUnit *neediest = nullptr;

		for (auto *a : allies_)
		{
			if (a->isAlive() && a != this)
			{
				if (!neediest || a->getHp() < neediest->getHp())
					neediest = a;
			}
		}

		if (neediest)
		{
			std::cout << "    [" << name_ << "] auto-heals ["
					  << neediest->getName() << "]\n";
			neediest->receiveHeal(healPower_);
		}
	}

	//
	// UILogger
	//

	UILogger::UILogger() : BattleUnit("UI-Logger", 9999) {}

	void UILogger::attack(BattleUnit *) {}

	void UILogger::onEvent(const std::string &event, BattleUnit *source, int value)
	{
		std::cout << "    [UI] EVENT \"" << event << "\" from ["
				  << source->getName() << "]";

		if (value)
			std::cout << " val=" << value;

		std::cout << "\n";
	}

	//
	// BattleMediator
	//

	void BattleMediator::addHero(BattleUnit *u)
	{
		heroes_.push_back(u);
		u->setMediator(this);
	}

	void BattleMediator::addEnemy(BattleUnit *u)
	{
		enemies_.push_back(u);
		u->setMediator(this);
	}

	void BattleMediator::setLogger(UILogger *l)
	{
		logger_ = l;
		l->setMediator(this);
	}

	void BattleMediator::notify(BattleUnit *sender,
								const std::string &event,
								int value)
	{
		if (logger_)
			logger_->onEvent(event, sender, value);

		bool hero = isHero(sender);

		if (event == "damaged")
		{
			for (auto *h : heroes_)
				h->onEvent(event, sender, value);
			for (auto *e : enemies_)
				e->onEvent(event, sender, value);
		}
		else if (event == "died")
		{
			if (hero)
			{
				for (auto *h : heroes_)
					if (h != sender)
						h->onEvent("ally_died", sender, 0);

				for (auto *e : enemies_)
					e->onEvent("ally_died", sender, 0);
			}
			else
			{
				for (auto *h : heroes_)
					h->onEvent("enemy_died", sender, 0);

				for (auto *e : enemies_)
					if (e != sender)
						e->onEvent("ally_died", sender, 0);
			}
		}
	}

	bool BattleMediator::isHero(BattleUnit *u) const
	{
		for (auto *h : heroes_)
			if (h == u)
				return true;
		return false;
	}

} // namespace med
