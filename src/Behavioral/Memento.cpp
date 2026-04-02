#include "Memento.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>

// HeroState::Memento

std::string HeroState::Memento::makeTimestamp()
{
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::ostringstream oss;
	oss << std::put_time(&tm, "%H:%M:%S");
	return oss.str();
}

HeroState::Memento::Memento(const HeroState &src, std::string label)
    : name_(src.name_), hp_(src.hp_), maxHp_(src.maxHp_),
      mana_(src.mana_), level_(src.level_), xp_(src.xp_),
      gold_(src.gold_), posX_(src.posX_), posY_(src.posY_),
      inventory_(src.inventory_),
      completedQuests_(src.completedQuests_),
      label_(std::move(label)),
      timestamp_(makeTimestamp()) {}

std::string HeroState::Memento::label() const { return label_; }
std::string HeroState::Memento::timestamp() const { return timestamp_; }

// HeroState

HeroState::HeroState(std::string name, int maxHp, int startMana)
    : name_(std::move(name)), hp_(maxHp), maxHp_(maxHp),
      mana_(startMana), level_(1), xp_(0), gold_(0),
      posX_(0), posY_(0)
{
}

std::unique_ptr<IMemento> HeroState::save(const std::string &label)
{
	std::cout << "    [Originator] Saving checkpoint \""
		  << label << "\"\n";
	return std::make_unique<Memento>(*this, label);
}

void HeroState::restore(const IMemento &mem)
{
	const Memento &m = static_cast<const Memento &>(mem);

	name_ = m.name_;
	hp_ = m.hp_;
	maxHp_ = m.maxHp_;
	mana_ = m.mana_;
	level_ = m.level_;
	xp_ = m.xp_;
	gold_ = m.gold_;
	posX_ = m.posX_;
	posY_ = m.posY_;
	inventory_ = m.inventory_;
	completedQuests_ = m.completedQuests_;

	std::cout << "    [Originator] State restored from \""
		  << m.label_ << "\" (" << m.timestamp_ << ")\n";
}

void HeroState::takeDamage(int dmg)
{
	hp_ = std::max(0, hp_ - dmg);
	std::cout << "    [Hero:" << name_ << "] took " << dmg
		  << " dmg  HP: " << hp_ << "/" << maxHp_ << "\n";
}

void HeroState::heal(int amount)
{
	hp_ = std::min(maxHp_, hp_ + amount);
	std::cout << "    [Hero:" << name_ << "] healed +"
		  << amount << "  HP: " << hp_ << "/" << maxHp_ << "\n";
}

void HeroState::gainXp(int xp)
{
	xp_ += xp;
	if (xp_ >= 100)
	{
		xp_ -= 100;
		++level_;
	}
	std::cout << "    [Hero:" << name_ << "] +"
		  << xp << " XP  Level: " << level_ << "\n";
}

void HeroState::gainGold(int g)
{
	gold_ += g;
	std::cout << "    [Hero:" << name_ << "] +"
		  << g << " gold  Total: " << gold_ << "\n";
}

void HeroState::move(int dx, int dy)
{
	posX_ += dx;
	posY_ += dy;
	std::cout << "    [Hero:" << name_ << "] moved to ("
		  << posX_ << ", " << posY_ << ")\n";
}

void HeroState::pickUpItem(const std::string &item)
{
	inventory_.push_back(item);
	std::cout << "    [Hero:" << name_ << "] picked up \""
		  << item << "\"\n";
}

void HeroState::completeQuest(const std::string &quest)
{
	completedQuests_.push_back(quest);
	std::cout << "    [Hero:" << name_ << "] completed quest \""
		  << quest << "\"\n";
}

void HeroState::showStatus() const
{
	std::cout << "    [Status] " << name_
		  << "  HP=" << hp_ << "/" << maxHp_
		  << "  MP=" << mana_
		  << "  Lv=" << level_
		  << "  XP=" << xp_
		  << "  Gold=" << gold_
		  << "  Pos=(" << posX_ << "," << posY_ << ")\n";
}

bool HeroState::isAlive() const { return hp_ > 0; }
int HeroState::getHp() const { return hp_; }
const std::string &HeroState::getName() const { return name_; }

// SaveManager

void SaveManager::saveToSlot(const std::string &slotName,
			     std::unique_ptr<IMemento> mem)
{
	for (auto &s : namedSlots_)
	{
		if (s.slotName == slotName)
		{
			s.memento = std::move(mem);
			std::cout << "    [SaveManager] Slot \"" << slotName
				  << "\" overwritten.\n";
			return;
		}
	}
	namedSlots_.push_back({slotName, std::move(mem)});
	std::cout << "    [SaveManager] Slot \"" << slotName
		  << "\" created.\n";
}

void SaveManager::pushHistory(std::unique_ptr<IMemento> mem)
{
	undoHistory_.push(std::move(mem));
}

const IMemento *SaveManager::getSlot(const std::string &slotName) const
{
	for (auto &s : namedSlots_)
		if (s.slotName == slotName)
			return s.memento.get();
	return nullptr;
}

std::unique_ptr<IMemento> SaveManager::popHistory()
{
	if (undoHistory_.empty())
		return nullptr;

	auto mem = std::move(const_cast<std::unique_ptr<IMemento> &>(
	    undoHistory_.top()));
	undoHistory_.pop();
	return mem;
}

void SaveManager::listSlots() const
{
	std::cout << "    [SaveManager] Named slots:\n";
	for (auto &s : namedSlots_)
		std::cout << "      [" << s.slotName << "] \""
			  << s.memento->label() << "\"  at "
			  << s.memento->timestamp() << "\n";
}
