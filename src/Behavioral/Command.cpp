#include "Behavioral/Command.h"
#include <algorithm>

//
// GameWorld
//

GameWorld::GameWorld(std::string heroName)
{
	hero.name = std::move(heroName);
}

void GameWorld::moveHero(int dx, int dy)
{
	hero.x += dx;
	hero.y += dy;
	std::cout << "    [World] " << hero.name << " moved to ("
			  << hero.x << ", " << hero.y << ")\n";
}

void GameWorld::attackEnemy(int dmg)
{
	if (!enemy.alive)
	{
		std::cout << "    [World] No enemy to attack.\n";
		return;
	}

	enemy.hp -= dmg;

	std::cout << "    [World] " << hero.name << " attacks "
			  << enemy.name << " for " << dmg
			  << " dmg (HP: " << enemy.hp << ")\n";

	if (enemy.hp <= 0)
	{
		enemy.alive = false;
		hero.gold += 30;
		std::cout << "    [World] Enemy defeated! +30 gold.\n";
	}
}

void GameWorld::undoAttack(int dmg)
{
	bool wasAlive = enemy.alive;

	enemy.hp += dmg;
	if (enemy.hp > 0)
		enemy.alive = true;

	if (!wasAlive && enemy.alive)
		hero.gold -= 30;

	std::cout << "    [World] Attack undone.\n";
}

void GameWorld::useItem(const std::string &item, int hpGain)
{
	auto it = std::find(hero.inventory.begin(), hero.inventory.end(), item);
	if (it == hero.inventory.end())
	{
		std::cout << "    [World] Item not found.\n";
		return;
	}

	hero.inventory.erase(it);
	hero.hp = std::min(hero.hp + hpGain, 200);

	std::cout << "    [World] Used " << item << "\n";
}

void GameWorld::undoItem(const std::string &item, int hpGain)
{
	hero.inventory.push_back(item);
	hero.hp = std::max(0, hero.hp - hpGain);

	std::cout << "    [World] Undo item.\n";
}

void GameWorld::addToInventory(const std::string &item)
{
	hero.inventory.push_back(item);
}

void GameWorld::removeFromInventory(const std::string &item)
{
	auto it = std::find(hero.inventory.begin(), hero.inventory.end(), item);
	if (it != hero.inventory.end())
		hero.inventory.erase(it);
}

void GameWorld::castSpell(const std::string &spell, int manaCost, int dmg)
{
	if (hero.mana < manaCost)
	{
		std::cout << "    Not enough mana.\n";
		return;
	}

	hero.mana -= manaCost;
	hero.activeEffects.push_back(spell);

	attackEnemy(dmg);
}

void GameWorld::undoCastSpell(const std::string &spell, int manaCost, int dmg)
{
	hero.mana = std::min(100, hero.mana + manaCost);

	auto it = std::find(hero.activeEffects.begin(),
						hero.activeEffects.end(), spell);
	if (it != hero.activeEffects.end())
		hero.activeEffects.erase(it);

	undoAttack(dmg);
}

void GameWorld::printStatus() const
{
	std::cout << "    HP=" << hero.hp << " MP=" << hero.mana << "\n";
}

//
// Commands
//

MoveCommand::MoveCommand(GameWorld &w, int dx, int dy)
	: world_(w), dx_(dx), dy_(dy) {}

void MoveCommand::execute() { world_.moveHero(dx_, dy_); }
void MoveCommand::undo() { world_.moveHero(-dx_, -dy_); }

std::string MoveCommand::description() const
{
	return "Move";
}

// AttackCommand
AttackCommand::AttackCommand(GameWorld &w, int dmg)
	: world_(w), damage_(dmg) {}

void AttackCommand::execute() { world_.attackEnemy(damage_); }
void AttackCommand::undo() { world_.undoAttack(damage_); }

std::string AttackCommand::description() const
{
	return "Attack";
}

// UseItemCommand
UseItemCommand::UseItemCommand(GameWorld &w, std::string item, int hpGain)
	: world_(w), item_(std::move(item)), hpGain_(hpGain) {}

void UseItemCommand::execute() { world_.useItem(item_, hpGain_); }
void UseItemCommand::undo() { world_.undoItem(item_, hpGain_); }

std::string UseItemCommand::description() const
{
	return "UseItem";
}

// CastSpellCommand
CastSpellCommand::CastSpellCommand(GameWorld &w, std::string spell, int mana, int dmg)
	: world_(w), spell_(std::move(spell)), manaCost_(mana), damage_(dmg) {}

void CastSpellCommand::execute()
{
	world_.castSpell(spell_, manaCost_, damage_);
}

void CastSpellCommand::undo()
{
	world_.undoCastSpell(spell_, manaCost_, damage_);
}

std::string CastSpellCommand::description() const
{
	return "CastSpell";
}

// MacroCommand
MacroCommand::MacroCommand(std::string label)
	: label_(std::move(label)) {}

void MacroCommand::add(std::shared_ptr<ICommand> cmd)
{
	commands_.push_back(std::move(cmd));
}

void MacroCommand::execute()
{
	for (auto &c : commands_)
		c->execute();
}

void MacroCommand::undo()
{
	for (auto it = commands_.rbegin(); it != commands_.rend(); ++it)
		(*it)->undo();
}

std::string MacroCommand::description() const
{
	return "Macro";
}

//
// InputHandler
//

void InputHandler::execute(std::shared_ptr<ICommand> cmd)
{
	cmd->execute();
	undoStack_.push(cmd);

	while (!redoStack_.empty())
		redoStack_.pop();
}

void InputHandler::undo()
{
	if (undoStack_.empty())
		return;

	auto cmd = undoStack_.top();
	undoStack_.pop();

	cmd->undo();
	redoStack_.push(cmd);
}

void InputHandler::redo()
{
	if (redoStack_.empty())
		return;

	auto cmd = redoStack_.top();
	redoStack_.pop();

	cmd->execute();
	undoStack_.push(cmd);
}

void InputHandler::printHistory() const
{
	std::cout << "Undo: " << undoStack_.size()
			  << " Redo: " << redoStack_.size() << "\n";
}
