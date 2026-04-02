#include "tasks.h"

#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "Behavioral/TemplateMethod.h"
#include "Behavioral/Mediator.h"
#include "Behavioral/ChainOfResponsibility.h"
#include "Behavioral/Observer.h"
#include "Behavioral/Strategy.h"
#include "Behavioral/Command.h"
#include "Behavioral/State.h"
#include "Behavioral/Visitor.h"
#include "Behavioral/Iterator.h"
#include "Behavioral/Memento.h"

using namespace std;

int task_03()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout << "Pattern 1 — Template Method\n";

	// Enemy we're fighting this round
	const string enemy = "Orc Warchief";
	const int enemyHp = 90;

	cout << "  Enemy: " << enemy << "  |  HP: " << enemyHp << "\n";

	// Warrior — overrides all abstract steps + both hooks
	cout << "\nWarrior (all hooks active) \n";
	Warrior warrior(60, 20);
	warrior.executeCombatRound(enemy, enemyHp);

	// Mage — overrides abstract steps + applySpecialEffect
	cout << "\nMage (no prepareForBattle hook) \n";
	Mage mage(110, {"Fireball", "Ice Lance", "Thunder"});
	mage.executeCombatRound(enemy, enemyHp);

	// Assassin — overrides abstract steps + prepareForBattle
	cout << "\nAssassin (no applySpecialEffect hook) \n";
	Assassin assassin(35);
	assassin.executeCombatRound(enemy, enemyHp);

	// Polymorphic call through base pointer
	cout << "\nPolymorphic dispatch (same call, different behaviour) \n";
	vector<unique_ptr<CombatUnit>> squad;
	squad.push_back(make_unique<Warrior>(45, 15));
	squad.push_back(make_unique<Mage>(80, vector<string>{"Lightning"}));
	squad.push_back(make_unique<Assassin>(30));

	for (auto &unit : squad)
		unit->executeCombatRound("Skeleton Knight", 70);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout << "Pattern 2 — Mediator\n";

	// Create concrete mediator
	med::BattleMediator mediator;

	// Colleagues
	med::MedHero hero1("Artur", 120, 35);
	med::MedHero hero2("Lena", 80, 25);
	med::MedEnemy orc("Orc Boss", 100, 40);
	med::MedEnemy troll("Troll", 60, 20);
	med::HealBot bot("MediBot", 999, 30);
	med::UILogger ui;

	// HealBot knows which allies to watch
	bot.registerAlly(&hero1);
	bot.registerAlly(&hero2);

	// Register everyone with the mediator
	mediator.addHero(&hero1);
	mediator.addHero(&hero2);
	mediator.addHero(&bot);
	mediator.addEnemy(&orc);
	mediator.addEnemy(&troll);
	mediator.setLogger(&ui);

	// Scene 1: Hero attacks Orc -> mediator routes event
	cout << "\nScene 1: Artur attacks Orc Boss \n";
	hero1.attack(&orc);

	// Scene 2: Orc retaliates, HealBot should react
	cout << "\nScene 2: Orc Boss retaliates against Lena \n";
	orc.attack(&hero2);

	// Scene 3: Orc is killed -> mediator notifies all
	cout << "\nScene 3: Artur finishes off Orc Boss \n";
	hero1.attack(&orc); // orc has ~25 hp left, 35 dmg kills it
	hero1.attack(&orc); // extra strike to ensure death

	// Scene 4: Hero falls -> mediator triggers rally
	cout << "\nScene 4: Troll one-shots Lena (simulate low HP) \n";
	hero2.takeDamage(999); // force death through takeDamage directly

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout << "Pattern 3 — Chain of Responsibility\n";

	int heroHp = 200;
	cout << "  Hero HP: " << heroHp << "\n";

	// Build chain: dodge(25%) | block(15) | armor(30%) | shield(40) | HP
	auto defenseChain = buildDefenseChain(25, 15, 30, 40, heroHp);

	// Hit 1: physical 30 dmg -> not dodged, block+armor, leaks to HP
	cout << "\nHit 1: Physical 30 dmg (not dodged -> block + armor -> HP) \n";
	{
		DamageRequest req("Orc", "physical", 30);
		defenseChain->handle(req);
		cout << "  Hero HP after: " << heroHp << "\n";
	}

	// Hit 2: physical 50 dmg -> block(15) + armor(30%) -> HP
	cout << "\nHit 2: Physical 50 dmg (not dodged -> block + armor -> HP) \n";
	{
		DamageRequest req("Troll", "physical", 50);
		defenseChain->handle(req);
		cout << "  Hero HP after: " << heroHp << "\n";
	}

	// Hit 3: magic 55 dmg -> skip dodge/block/armor -> shield
	cout << "\nHit 3: Magic 55 dmg (skips physical handlers -> shield + HP) \n";
	{
		DamageRequest req("Dark Mage", "magic", 55);
		defenseChain->handle(req);
		cout << "  Hero HP after: " << heroHp << "\n";
	}

	// Hit 4: fire 20 dmg -> shield depleted -> armor(30%) -> HP
	cout << "\nHit 4: Fire 20 dmg (shield depleted -> armor -> HP) \n";
	{
		DamageRequest req("Dragon", "fire", 20);
		defenseChain->handle(req);
		cout << "  Hero HP after: " << heroHp << "\n";
	}

	// Hit 5: physical 9 dmg -> block absorbs entirely
	cout << "\nHit 5: Physical 9 dmg (block absorbs entirely, stops chain) \n";
	{
		DamageRequest req("Goblin", "physical", 9);
		defenseChain->handle(req);
		cout << "  Hero HP after: " << heroHp << "\n";
	}

	// Hit 6: physical 35 dmg (35 % 7 == 0) -> DODGE
	cout << "\nHit 6: Physical 35 dmg (35 % 7 == 0 -> DODGE, stops chain) \n";
	{
		DamageRequest req("Elite Orc", "physical", 35);
		defenseChain->handle(req);
		cout << "  Hero HP after: " << heroHp << "\n";
	}

	cout << "\n Final Hero HP: " << heroHp << " / 200\n";

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout << "Pattern 4 — Observer\n";

	// Create subject
	ObservableHero hero("Roland", 200, 1);

	// Create observers
	UIHealthBar hpBar;
	AchievementSystem achievements;
	AutoSaveSystem autoSave;
	QuestTracker quests;

	// Attach all observers
	cout << "\nAttaching observers \n";
	hero.attach(&hpBar);
	hero.attach(&achievements);
	hero.attach(&autoSave);
	hero.attach(&quests);

	// Scene 1: Hero takes heavy damage
	cout << "\nScene 1: Roland takes 80 dmg \n";
	hero.takeDamage(80);

	// Scene 2: Enemy poisons hero
	cout << "\nScene 2: Poison applied \n";
	hero.applyStatus("poison");

	// Scene 3: Hero heals
	cout << "\nScene 3: Roland heals 40 HP \n";
	hero.heal(40);

	// Scene 4: Detach UIHealthBar (e.g. UI closed)
	cout << "\nScene 4: Detach UIHealthBar (minimized UI) \n";
	hero.detach(&hpBar);

	// Scene 5: XP gain -> Level Up (AutoSave + Achievement fire)
	cout << "\nScene 5: Gain 100 XP -> Level Up \n";
	hero.gainXp(100); // triggers level_up + hp_changed (UIHealthBar detached)

	// Scene 6: Re-attach UIHealthBar and take fatal damage
	cout << "\nScene 6: Reattach UIHealthBar, fatal hit \n";
	hero.attach(&hpBar);
	hero.takeDamage(999);

	// Summary
	cout << "\nFinal state \n";
	achievements.listBadges();
	quests.printStatus();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout << "Pattern 5 — Strategy\n";

	// Shared battlefield state (targets visible to AI units)
	using Target = BattleContext::Target;
	vector<Target> playerParty = {{"Artur", 120, 200}, {"Lena", 40, 80}};
	vector<Target> enemyAllies = {{"Troll", 60, 100}};

	// Scene 1: Each unit uses its own strategy
	cout << " Scene 1: Each enemy takes a turn with its own strategy \n";

	EnemyAI aggressor("Orc Warrior", 100, 35, make_shared<AggressiveStrategy>());
	EnemyAI guardian("Stone Golem", 200, 25, make_shared<DefensiveStrategy>(40));
	EnemyAI berserker("Werewolf", 100, 30, make_shared<BerserkerStrategy>());
	EnemyAI shaman("Orc Shaman", 80, 15, make_shared<SupportStrategy>(25));

	aggressor.showStatus();
	aggressor.takeTurn(playerParty, enemyAllies);

	guardian.showStatus();
	guardian.takeTurn(playerParty, enemyAllies);

	berserker.showStatus();
	berserker.takeTurn(playerParty, enemyAllies);

	shaman.showStatus();
	shaman.takeTurn(playerParty, enemyAllies);

	// Scene 2: Berserker takes damage -> strategy output changes
	cout << "\nScene 2: Werewolf drops to 30% HP -> Berserker rage escalates \n";
	berserker.takeDamage(70); // 100 -> 30 HP
	berserker.showStatus();
	berserker.takeTurn(playerParty, enemyAllies);

	// Scene 3: Boss phase change — swap strategy at runtime
	cout << "\nScene 3: Boss phase change (HP < 50%) -> strategy swap \n";
	EnemyAI boss("Dragon Boss", 300, 50, make_shared<DefensiveStrategy>(60));
	boss.showStatus();
	cout << "  Turn 1 (Defensive, HP full):\n";
	boss.takeTurn(playerParty, {});

	boss.takeDamage(200); // simulate heavy damage
	boss.showStatus();
	cout << "  HP dropped below 50% -> switching to Berserker!\n";
	boss.setStrategy(make_shared<BerserkerStrategy>());
	cout << "  Turn 2 (Berserker, low HP):\n";
	boss.takeTurn(playerParty, {});

	// Scene 4: Same context, four strategies in sequence
	cout << "\nScene 4: One unit cycles through all strategies \n";
	EnemyAI unit("Test Unit", 50, 20, make_shared<AggressiveStrategy>());
	vector<shared_ptr<ICombatStrategy>> strategies = {
	    make_shared<AggressiveStrategy>(),
	    make_shared<DefensiveStrategy>(),
	    make_shared<BerserkerStrategy>(),
	    make_shared<SupportStrategy>()};
	for (auto &s : strategies)
	{
		unit.setStrategy(s);
		unit.takeTurn(playerParty, enemyAllies);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout << "Pattern 6 — Command\n";

	// Setup world and invoker
	GameWorld world("Artur");
	world.addToInventory("Health Potion");
	world.addToInventory("Elixir");
	InputHandler input;

	// Scene 1: Basic commands + undo
	cout << "\nScene 1: Basic actions \n";
	input.execute(make_shared<MoveCommand>(world, 3, 1));
	input.execute(make_shared<MoveCommand>(world, 0, 2));
	input.execute(make_shared<AttackCommand>(world, 40));
	input.execute(make_shared<AttackCommand>(world, 35));
	world.printStatus();

	// Scene 2: Undo last two attacks
	cout << "\nScene 2: Undo last 2 attacks \n";
	input.undo(); // undo Attack(35)
	input.undo(); // undo Attack(40)
	world.printStatus();

	// Scene 3: Redo one attack
	cout << "\nScene 3: Redo first attack \n";
	input.redo(); // redo Attack(40)
	world.printStatus();

	// Scene 4: UseItem command + undo
	cout << "\nScene 4: Use Health Potion then undo \n";
	world.hero.hp = 90; // simulate damage taken
	cout << "  (HP manually set to 90 to show healing)\n";
	input.execute(make_shared<UseItemCommand>(world, "Health Potion", 60));
	world.printStatus();
	input.undo(); // potion returns to inventory, HP reverts
	world.printStatus();

	// Scene 5: CastSpell command
	cout << "\nScene 5: Cast Fireball \n";
	input.execute(make_shared<CastSpellCommand>(world, "Fireball", 30, 55));
	world.printStatus();

	// Scene 6: MacroCommand — combo attack
	cout << "\nScene 6: Macro command - \"Blitz Combo\" \n";
	auto blitz = make_shared<MacroCommand>("Blitz Combo");
	blitz->add(make_shared<MoveCommand>(world, 1, 0));
	blitz->add(make_shared<AttackCommand>(world, 25));
	blitz->add(make_shared<CastSpellCommand>(world, "IceLance", 20, 30));
	blitz->add(make_shared<MoveCommand>(world, -1, 0));
	input.execute(blitz);
	world.printStatus();

	// Scene 7: Undo the entire macro in one step
	cout << "\nScene 7: Undo entire Blitz Combo macro \n";
	input.undo();
	world.printStatus();

	input.printHistory();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout << "Pattern 7 — State\n";

	// Create boss and set initial state
	BossEnemy dragon("Dragon Overlord", 300, 45);
	dragon.setState(make_shared<IdleState>());
	dragon.showStatus();

	// Scene 1: Boss is idle, takes a turn (patrols)
	cout << "\nScene 1: Boss in Idle — patrol turn \n";
	dragon.update();

	// Scene 2: Player hits boss -> Idle transitions to Enraged
	cout << "\nScene 2: Player attacks boss (30 dmg) -> Idle exits \n";
	dragon.takeDamage(30);
	dragon.showStatus();

	// Scene 3: Boss in Enraged phase, 3 turns
	cout << "\nScene 3: Enraged phase — 3 attack turns \n";
	dragon.update();
	dragon.update();
	dragon.update();

	// Scene 4: Drop HP below 60 % -> transition to Wounded
	cout << "\nScene 4: Heavy damage -> drops below 60% HP \n";
	dragon.takeDamage(100); // 300-30-100 = 170 HP = 56%
	dragon.showStatus();

	// Scene 5: Wounded phase — 4 turns (shield alternates)
	cout << "\nScene 5: Wounded phase — 4 turns (shield alternates) \n";
	dragon.update(); // turn 1 -> shield ON
	dragon.update(); // turn 2 -> attack
	dragon.update(); // turn 3 -> shield ON
	dragon.update(); // turn 4 -> attack

	// Scene 6: Drop HP below 30 % -> transition to Despair
	cout << "\nScene 6: Critical hit -> drops below 30% HP \n";
	dragon.takeDamage(95); // 170-95 = 75 HP = 25%
	dragon.showStatus();

	// Scene 7: Despair phase — turns (minions spawn)
	cout << "\nScene 7: Despair phase — 3 turns + final blow \n";
	dragon.update();       // turn 1 — double dmg
	dragon.update();       // turn 2 — double dmg + minion
	dragon.update();       // turn 3 — double dmg
	dragon.takeDamage(80); // final blow
	dragon.showStatus();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout << "Pattern 8 — Visitor\n";

	// Build inventory (mix of all element types)
	cout << "\nBuilding inventory \n";
	vector<shared_ptr<vis::IItem>> inventory;
	inventory.push_back(make_shared<vis::Weapon>("Iron Sword", 45, 3.5f, 120));
	inventory.push_back(make_shared<vis::Weapon>("Enchanted Bow", 38, 1.8f, 200));
	inventory.push_back(make_shared<vis::Armor>("Plate Chest", 30, 8.0f, 250));
	inventory.push_back(make_shared<vis::Armor>("Shadow Cloak", 12, 1.2f, 90));
	inventory.push_back(make_shared<vis::Potion>("Health Potion", "heal", 60, 0.5f, 30));
	inventory.push_back(make_shared<vis::Potion>("Mana Elixir", "mana", 50, 0.5f, 40));
	inventory.push_back(make_shared<vis::QuestItem>("Dragon Seal", "Slay the Dragon", true, 0.1f));
	inventory.push_back(make_shared<vis::QuestItem>("Old Map", "Lost Ruins", false, 0.3f));
	cout << "  Inventory: " << inventory.size() << " items\n";

	// Helper: run a visitor over all items
	auto runVisitor = [&](vis::IVisitor &v)
	{
		for (auto &item : inventory)
			item->accept(v);
	};

	// Visitor 1: StatsPrinter
	cout << "\nVisitor 1: StatsPrinter (full item details) \n";
	vis::StatsPrinter printer;
	runVisitor(printer);

	// Visitor 2: WeightCalculator
	cout << "\nVisitor 2: WeightCalculator (total carry weight) \n";
	vis::WeightCalculator weightCalc;
	runVisitor(weightCalc);
	cout << "  Total carry weight: " << weightCalc.total() << " kg\n";

	// Visitor 3: LootEvaluator
	cout << "\nVisitor 3: LootEvaluator (sell value) \n";
	vis::LootEvaluator loot;
	runVisitor(loot);
	cout << "  Total loot value:   " << loot.total() << " gold"
	     << "  (unsellable items: " << loot.unsellable() << ")\n";

	// Visitor 4: UpgradeVisitor
	cout << "\nVisitor 4: UpgradeVisitor (blacksmith forges gear) \n";
	vis::UpgradeVisitor forge;
	runVisitor(forge);
	cout << "  Upgraded: " << forge.upgraded()
	     << "  Skipped: " << forge.skipped() << "\n";

	// Show stats again after upgrade
	cout << "\nStats after upgrade \n";
	vis::StatsPrinter printer2;
	runVisitor(printer2);

	// Re-evaluate loot value after upgrade
	cout << "\nLoot value after upgrade \n";
	vis::LootEvaluator loot2;
	runVisitor(loot2);
	cout << "  Total loot value after upgrade: " << loot2.total()
	     << " gold  (was " << loot.total() << ")\n";

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout << "Pattern 9 — Iterator\n";

	// Collection 1: Inventory
	cout << "\nCollection 1: Inventory \n";
	Inventory inv;
	inv.addItem("Iron Sword");
	inv.addItem("Health Potion");
	inv.addItem("Plate Armor");
	inv.addItem("Mana Potion");
	inv.addItem("Enchanted Shield");
	inv.addItem("Strength Potion");
	cout << "  Items in inventory: " << inv.size() << "\n";

	// Forward traversal
	cout << "\n [ForwardIterator] All items:\n";
	auto fwdIt = inv.createIterator();
	while (fwdIt->hasNext())
		cout << "    -> " << fwdIt->next() << "\n";

	// Filter traversal — only Potions
	cout << "\n [FilterIterator] keyword=\"Potion\":\n";
	auto filterIt = inv.createFilterIterator("Potion");
	while (filterIt->hasNext())
		cout << "    -> " << filterIt->next() << "\n";

	// Reset and iterate again
	cout << "\n [ForwardIterator] After reset — first 3 items:\n";
	fwdIt->reset();
	for (int i = 0; i < 3 && fwdIt->hasNext(); ++i)
		cout << "    -> " << fwdIt->next() << "\n";

	// Collection 2: EnemyWave
	cout << "\nCollection 2: EnemyWave \n";
	EnemyWave wave;
	wave.addEnemy({"Goblin Scout", 40, 8});
	wave.addEnemy({"Orc Warrior", 100, 25});
	wave.addEnemy({"Dark Mage", 70, 35});
	wave.addEnemy({"Troll Berserker", 150, 40});
	wave.addEnemy({"Dragon Boss", 300, 60});
	cout << "  Enemies in wave: " << wave.size() << "\n";

	// Forward — spawn order
	cout << "\n [ForwardIterator] Spawn order (first to engage):\n";
	auto waveIt = wave.createIterator();
	while (waveIt->hasNext())
	{
		auto e = waveIt->next();
		cout << "    -> " << e.name
		     << "  HP=" << e.hp << "  DMG=" << e.damage << "\n";
	}

	// Reverse — kill priority (strongest last encountered = boss first)
	cout << "\n [ReverseIterator] Kill priority (boss first):\n";
	auto revIt = wave.createReverseIterator();
	int priority = 1;
	while (revIt->hasNext())
	{
		auto e = revIt->next();
		cout << "    #" << priority++ << " " << e.name
		     << "  HP=" << e.hp << "  DMG=" << e.damage << "\n";
	}

	// Collection 3: SkillTree
	cout << "\nCollection 3: SkillTree (DFS pre-order) \n";

	// Build tree:
	//  Combat(1)
	//    Swordsmanship(2)
	//      Parry(3)
	//      Riposte(3)
	//    Shield Mastery(2)
	//      Block(3)
	//  Magic(1)
	//    Fireball(2)
	//    Ice Lance(2)
	//      Blizzard(3)
	auto root = make_shared<SkillNode>("Combat", 1);
	auto sword = make_shared<SkillNode>("Swordsmanship", 2);
	auto shield = make_shared<SkillNode>("Shield Mastery", 2);
	auto parry = make_shared<SkillNode>("Parry", 3);
	auto riposte = make_shared<SkillNode>("Riposte", 3);
	auto block = make_shared<SkillNode>("Block", 3);
	auto magic = make_shared<SkillNode>("Magic", 1);
	auto fire = make_shared<SkillNode>("Fireball", 2);
	auto ice = make_shared<SkillNode>("Ice Lance", 2);
	auto blizz = make_shared<SkillNode>("Blizzard", 3);

	sword->addChild(parry);
	sword->addChild(riposte);
	shield->addChild(block);
	root->addChild(sword);
	root->addChild(shield);
	magic->addChild(fire);
	ice->addChild(blizz);
	magic->addChild(ice);

	// Root with two top-level branches — wrap in a virtual root
	auto treeRoot = make_shared<SkillNode>("[SkillTree]", 0);
	treeRoot->addChild(root);
	treeRoot->addChild(magic);

	SkillTree skillTree(treeRoot);

	cout << "  DFS pre-order traversal:\n";
	auto treeIt = skillTree.createIterator();
	while (treeIt->hasNext())
	{
		SkillNode *node = treeIt->next();
		// Indent by tier level
		std::string indent(node->level * 2, ' ');
		cout << "    " << indent << node->skillName
		     << "  (tier " << node->level << ")\n";
	}

	// Demonstrate reset — count total nodes
	treeIt->reset();
	int nodeCount = 0;
	while (treeIt->hasNext())
	{
		treeIt->next();
		++nodeCount;
	}
	cout << "  Total skill nodes in tree: " << nodeCount << "\n";

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout << "Pattern 10 — Memento\n";

	// Setup originator and caretaker
	HeroState roland("Roland", 200, 100);
	SaveManager saves;

	// Scene 1: Initial state — save to slot "start"
	cout << "\nScene 1: Initial state saved to slot \"start\" \n";
	roland.showStatus();
	saves.saveToSlot("start", roland.save("Game Start"));

	// Scene 2: Play through the first area
	cout << "\nScene 2: Playing through Forest area \n";
	roland.move(5, 3);
	roland.gainXp(40);
	roland.gainGold(30);
	roland.pickUpItem("Iron Key");
	roland.pickUpItem("Health Potion");
	roland.takeDamage(35);

	cout << "\n Reached checkpoint — saving to slot \"forest\"\n";
	saves.saveToSlot("forest", roland.save("Forest Checkpoint"));
	saves.pushHistory(roland.save("before-dungeon"));

	// Scene 3: Enter dungeon — dangerous area
	cout << "\nScene 3: Entering dungeon (push history before each risk) \n";
	saves.pushHistory(roland.save("dungeon-entrance"));

	roland.move(2, -1);
	roland.takeDamage(60);
	roland.gainXp(60); // triggers level up
	roland.completeQuest("Find the Ancient Seal");
	roland.gainGold(120);
	roland.pickUpItem("Ancient Seal");

	cout << "\n Before boss fight — snapshot pushed to history\n";
	saves.pushHistory(roland.save("before-boss"));
	roland.showStatus();

	// Scene 4: Boss fight — roland dies
	cout << "\nScene 4: Boss fight — roland takes fatal damage \n";
	roland.takeDamage(999);
	cout << "  Hero is dead: " << (roland.isAlive() ? "no" : "YES") << "\n";

	// Scene 5: Restore from most recent history snapshot
	cout << "\nScene 5: Undo — restore from history (before-boss) \n";
	auto lastSnap = saves.popHistory();
	if (lastSnap)
		roland.restore(*lastSnap);
	roland.showStatus();

	// Scene 6: Try again, lose again — restore from slot
	cout << "\nScene 6: Try boss again — fail again \n";
	roland.takeDamage(150);
	roland.takeDamage(120);
	cout << "  Hero HP: " << roland.getHp() << "\n";

	cout << "\n Loading named slot \"forest\" (further back) \n";
	const IMemento *forestSave = saves.getSlot("forest");
	if (forestSave)
		roland.restore(*forestSave);
	roland.showStatus();

	// Scene 7: Overwrite a slot
	cout << "\nScene 7: Progress further and overwrite \"forest\" slot \n";
	roland.gainGold(50);
	roland.pickUpItem("Magic Staff");
	saves.saveToSlot("forest", roland.save("Forest (Updated)"));

	// Scene 8: Restore from start — shows full rollback
	cout << "\nScene 8: Restore from very first save \"start\" \n";
	const IMemento *startSave = saves.getSlot("start");
	if (startSave)
		roland.restore(*startSave);
	roland.showStatus();

	// Final: list all slots
	cout << "\nSave Manager state \n";
	saves.listSlots();

	return 0;
}
