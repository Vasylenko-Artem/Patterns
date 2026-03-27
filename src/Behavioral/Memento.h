#pragma once
#include <string>
#include <memory>
#include <vector>
#include <stack>

//  IMemento
class IMemento
{
public:
	virtual std::string label() const = 0;
	virtual std::string timestamp() const = 0;
	virtual ~IMemento() = default;
};

//  HeroState
class HeroState
{
	std::string name_;
	int hp_;
	int maxHp_;
	int mana_;
	int level_;
	int xp_;
	int gold_;
	int posX_, posY_;
	std::vector<std::string> inventory_;
	std::vector<std::string> completedQuests_;

	class Memento : public IMemento
	{
		friend class HeroState;

		std::string name_;
		int hp_;
		int maxHp_;
		int mana_;
		int level_;
		int xp_;
		int gold_;
		int posX_, posY_;
		std::vector<std::string> inventory_;
		std::vector<std::string> completedQuests_;
		std::string label_;
		std::string timestamp_;

		static std::string makeTimestamp();

	public:
		Memento(const HeroState &src, std::string label);

		std::string label() const override;
		std::string timestamp() const override;
	};

public:
	HeroState(std::string name, int maxHp, int startMana);

	std::unique_ptr<IMemento> save(const std::string &label);
	void restore(const IMemento &mem);

	void takeDamage(int dmg);
	void heal(int amount);
	void gainXp(int xp);
	void gainGold(int g);
	void move(int dx, int dy);
	void pickUpItem(const std::string &item);
	void completeQuest(const std::string &quest);

	void showStatus() const;

	bool isAlive() const;
	int getHp() const;
	const std::string &getName() const;
};

//  SaveManager
class SaveManager
{
	struct Slot
	{
		std::string slotName;
		std::unique_ptr<IMemento> memento;
	};

	std::vector<Slot> namedSlots_;
	std::stack<std::unique_ptr<IMemento>> undoHistory_;

public:
	void saveToSlot(const std::string &slotName,
			std::unique_ptr<IMemento> mem);

	void pushHistory(std::unique_ptr<IMemento> mem);

	const IMemento *getSlot(const std::string &slotName) const;

	std::unique_ptr<IMemento> popHistory();

	void listSlots() const;
};
