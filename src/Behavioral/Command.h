#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <stack>

//  Receiver
class GameWorld
{
public:
	struct HeroState
	{
		std::string name;
		int x = 0, y = 0;
		int hp = 200;
		int mana = 100;
		int gold = 50;
		std::vector<std::string> inventory;
		std::vector<std::string> activeEffects;
	} hero;

	struct EnemyState
	{
		std::string name = "Orc Boss";
		int hp = 150;
		bool alive = true;
	} enemy;

	explicit GameWorld(std::string heroName);

	void moveHero(int dx, int dy);
	void attackEnemy(int dmg);
	void undoAttack(int dmg);

	void useItem(const std::string &item, int hpGain);
	void undoItem(const std::string &item, int hpGain);

	void addToInventory(const std::string &item);
	void removeFromInventory(const std::string &item);

	void castSpell(const std::string &spell, int manaCost, int dmg);
	void undoCastSpell(const std::string &spell, int manaCost, int dmg);

	void printStatus() const;
};

//  Command interface
class ICommand
{
public:
	virtual void execute() = 0;
	virtual void undo() = 0;
	virtual std::string description() const = 0;
	virtual ~ICommand() = default;
};

//  Commands
class MoveCommand : public ICommand
{
	GameWorld &world_;
	int dx_, dy_;

public:
	MoveCommand(GameWorld &w, int dx, int dy);

	void execute() override;
	void undo() override;
	std::string description() const override;
};

class AttackCommand : public ICommand
{
	GameWorld &world_;
	int damage_;

public:
	AttackCommand(GameWorld &w, int dmg);

	void execute() override;
	void undo() override;
	std::string description() const override;
};

class UseItemCommand : public ICommand
{
	GameWorld &world_;
	std::string item_;
	int hpGain_;

public:
	UseItemCommand(GameWorld &w, std::string item, int hpGain);

	void execute() override;
	void undo() override;
	std::string description() const override;
};

class CastSpellCommand : public ICommand
{
	GameWorld &world_;
	std::string spell_;
	int manaCost_;
	int damage_;

public:
	CastSpellCommand(GameWorld &w, std::string spell, int mana, int dmg);

	void execute() override;
	void undo() override;
	std::string description() const override;
};

class MacroCommand : public ICommand
{
	std::string label_;
	std::vector<std::shared_ptr<ICommand>> commands_;

public:
	explicit MacroCommand(std::string label);

	void add(std::shared_ptr<ICommand> cmd);
	void execute() override;
	void undo() override;
	std::string description() const override;
};

//  Invoker
class InputHandler
{
	std::stack<std::shared_ptr<ICommand>> undoStack_;
	std::stack<std::shared_ptr<ICommand>> redoStack_;

public:
	void execute(std::shared_ptr<ICommand> cmd);
	void undo();
	void redo();
	void printHistory() const;
};
