#include "Behavioral/Observer.h"
#include <algorithm>

//
// ObservableHero
//

ObservableHero::ObservableHero(std::string name, int maxHp, int startLevel)
	: name_(std::move(name)),
	  hp_(maxHp),
	  maxHp_(maxHp),
	  level_(startLevel),
	  xp_(0)
{
}

void ObservableHero::attach(IObserver *o)
{
	observers_.push_back(o);
	std::cout << "  [Subject] Observer attached (total="
			  << observers_.size() << ")\n";
}

void ObservableHero::detach(IObserver *o)
{
	observers_.erase(
		std::remove(observers_.begin(), observers_.end(), o),
		observers_.end());

	std::cout << "  [Subject] Observer detached (total="
			  << observers_.size() << ")\n";
}

void ObservableHero::notify(const HeroEvent &event)
{
	for (auto *obs : observers_)
		obs->update(event);
}

void ObservableHero::takeDamage(int dmg)
{
	int prev = hp_;
	hp_ = std::max(0, hp_ - dmg);

	std::cout << "  [Hero:" << name_ << "] takes " << dmg
			  << " dmg  HP: " << prev << " -> " << hp_ << "\n";

	notify({hp_ > 0 ? "hp_changed" : "died",
			name_, hp_, std::to_string(dmg) + " dmg"});
}

void ObservableHero::heal(int amount)
{
	int prev = hp_;
	hp_ = std::min(maxHp_, hp_ + amount);

	std::cout << "  [Hero:" << name_ << "] healed +" << amount
			  << "  HP: " << prev << " -> " << hp_ << "\n";

	notify({"hp_changed", name_, hp_, "heal"});
}

void ObservableHero::gainXp(int xp)
{
	xp_ += xp;

	std::cout << "  [Hero:" << name_ << "] gained " << xp
			  << " XP (total=" << xp_ << ")\n";

	if (xp_ >= 100)
	{
		xp_ -= 100;
		++level_;
		hp_ = maxHp_;

		std::cout << "  [Hero:" << name_ << "] *** LEVEL UP -> "
				  << level_ << " ***\n";

		notify({"level_up", name_, level_, ""});
		notify({"hp_changed", name_, hp_, "level_up_heal"});
	}
}

void ObservableHero::applyStatus(const std::string &status)
{
	currentStatus_ = status;

	std::cout << "  [Hero:" << name_ << "] status applied: "
			  << status << "\n";

	notify({"status_applied", name_, 0, status});
}

const std::string &ObservableHero::getName() const { return name_; }
int ObservableHero::getHp() const { return hp_; }
int ObservableHero::getMaxHp() const { return maxHp_; }
int ObservableHero::getLevel() const { return level_; }

//
// UIHealthBar
//

std::string UIHealthBar::buildBar(int hp, int maxHp) const
{
	int filled = (barWidth_ * hp) / maxHp;
	std::string bar = "[";

	for (int i = 0; i < barWidth_; ++i)
		bar += (i < filled ? '#' : '.');

	bar += "] " + std::to_string(hp) + "/" + std::to_string(maxHp);
	return bar;
}

void UIHealthBar::update(const HeroEvent &event)
{
	if (event.type != "hp_changed" && event.type != "died")
		return;

	if (event.type == "died")
	{
		std::cout << "    [UIHealthBar] " << event.heroName
				  << " HP: [" << std::string(barWidth_, 'X') << "] DEAD\n";
	}
	else
	{
		std::cout << "    [UIHealthBar] " << event.heroName
				  << " HP updated → " << event.intValue << "\n";
	}
}

//
// AchievementSystem
//

void AchievementSystem::unlock(const std::string &badge)
{
	if (std::find(unlocked_.begin(), unlocked_.end(), badge) != unlocked_.end())
		return;

	unlocked_.push_back(badge);

	std::cout << "    [Achievements] ★ BADGE UNLOCKED: \""
			  << badge << "\"\n";
}

void AchievementSystem::update(const HeroEvent &event)
{
	if (event.type == "level_up" && event.intValue >= 2)
		unlock("First Level-Up");

	if (event.type == "level_up" && event.intValue >= 5)
		unlock("Veteran");

	if (event.type == "status_applied" && event.strValue == "poison")
		unlock("Poisoned for the First Time");

	if (event.type == "hp_changed" && event.intValue == 0)
		unlock("Near Death");

	if (event.type == "died")
		unlock("You Died");
}

void AchievementSystem::listBadges() const
{
	std::cout << "    [Achievements] Unlocked (" << unlocked_.size() << "):\n";
	for (auto &b : unlocked_)
		std::cout << "      • " << b << "\n";
}

//
// AutoSaveSystem
//

void AutoSaveSystem::update(const HeroEvent &event)
{
	if (event.type != "level_up")
		return;

	++saveCount_;

	std::cout << "    [AutoSave] Checkpoint #" << saveCount_
			  << " saved — \"" << event.heroName
			  << "\" lvl " << event.intValue << "\n";
}

//
// QuestTracker
//

QuestTracker::QuestTracker()
{
	objectives_["survive_poison"] = false;
	objectives_["reach_level_2"] = false;
	objectives_["take_100_dmg"] = false;
}

void QuestTracker::update(const HeroEvent &event)
{
	if (event.type == "status_applied" && event.strValue == "poison")
	{
		std::cout << "    [QuestTracker] poison detected\n";
	}

	if (event.type == "hp_changed" && !objectives_["take_100_dmg"])
	{
		if (!event.strValue.empty() && std::stoi(event.strValue) >= 50)
		{
			objectives_["take_100_dmg"] = true;
			std::cout << "    [QuestTracker] heavy damage objective complete\n";
		}
	}

	if (event.type == "level_up" && event.intValue >= 2)
	{
		objectives_["reach_level_2"] = true;
		std::cout << "    [QuestTracker] reached level 2\n";
	}

	if (event.type == "status_applied" && event.strValue != "poison")
	{
		objectives_["survive_poison"] = true;
		std::cout << "    [QuestTracker] survived status\n";
	}
}

void QuestTracker::printStatus() const
{
	std::cout << "    [QuestTracker] Objectives:\n";
	for (auto &[name, done] : objectives_)
		std::cout << "      " << (done ? "[x]" : "[ ]")
				  << " " << name << "\n";
}
