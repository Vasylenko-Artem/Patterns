#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

//  Event
struct HeroEvent
{
	std::string type;
	std::string heroName;
	int intValue = 0;
	std::string strValue;
};

//  Observer
class IObserver
{
public:
	virtual void update(const HeroEvent &event) = 0;
	virtual ~IObserver() = default;
};

//  Subject
class ISubject
{
public:
	virtual void attach(IObserver *observer) = 0;
	virtual void detach(IObserver *observer) = 0;
	virtual void notify(const HeroEvent &event) = 0;
	virtual ~ISubject() = default;
};

//  ObservableHero
class ObservableHero : public ISubject
{
	std::string name_;
	int hp_;
	int maxHp_;
	int level_;
	int xp_;
	std::string currentStatus_;

	std::vector<IObserver *> observers_;

public:
	ObservableHero(std::string name, int maxHp, int startLevel = 1);

	void attach(IObserver *o) override;
	void detach(IObserver *o) override;
	void notify(const HeroEvent &event) override;

	void takeDamage(int dmg);
	void heal(int amount);
	void gainXp(int xp);
	void applyStatus(const std::string &status);

	const std::string &getName() const;
	int getHp() const;
	int getMaxHp() const;
	int getLevel() const;
};

//  Observers
class UIHealthBar : public IObserver
{
	int barWidth_ = 20;

	std::string buildBar(int hp, int maxHp) const;

public:
	void update(const HeroEvent &event) override;
};

class AchievementSystem : public IObserver
{
	std::vector<std::string> unlocked_;

	void unlock(const std::string &badge);

public:
	void update(const HeroEvent &event) override;
	void listBadges() const;
};

class AutoSaveSystem : public IObserver
{
	int saveCount_ = 0;

public:
	void update(const HeroEvent &event) override;
};

class QuestTracker : public IObserver
{
	std::unordered_map<std::string, bool> objectives_;

public:
	QuestTracker();

	void update(const HeroEvent &event) override;
	void printStatus() const;
};
