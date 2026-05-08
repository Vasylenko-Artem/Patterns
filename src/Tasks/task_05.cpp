#include "tasks.h"

#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>

using std::cout;
using std::string;

//  Functional paradigm re-implementation of OOP patterns
namespace fp
{

	//  1. STRATEGY — functions instead of class hierarchy

	struct BattleContext
	{
		string unitName;
		int unitHp;
		int unitMaxHp;
		int unitBaseDamage;

		struct Target
		{
			string name;
			int hp;
			int maxHp;
		};
		std::vector<Target> enemies;
		std::vector<Target> allies;
	};

	// Strategy is just a callable
	using CombatStrategy = std::function<string(const BattleContext &)>;

	// --- Strategy factories (plain functions, no classes) -------

	CombatStrategy makeAggressive()
	{
		return [](const BattleContext &ctx) -> string
		{
			if (ctx.enemies.empty())
				return ctx.unitName + " finds no targets.";
			auto it = std::min_element(ctx.enemies.begin(), ctx.enemies.end(),
						   [](const auto &a, const auto &b)
						   { return a.hp < b.hp; });
			return ctx.unitName + " [Aggressive] strikes \"" + it->name +
			       "\" for " + std::to_string(ctx.unitBaseDamage) + " dmg.";
		};
	}

	CombatStrategy makeDefensive(int healAmount = 30)
	{
		return [healAmount](const BattleContext &ctx) -> string
		{
			if (ctx.unitHp < ctx.unitMaxHp / 2)
				return ctx.unitName + " heals for " + std::to_string(healAmount);
			if (ctx.enemies.empty())
				return ctx.unitName + " holds position.";
			auto it = std::max_element(ctx.enemies.begin(), ctx.enemies.end(),
						   [](const auto &a, const auto &b)
						   { return a.hp < b.hp; });
			return ctx.unitName + " attacks strongest \"" + it->name + "\".";
		};
	}

	CombatStrategy makeBerserker()
	{
		return [](const BattleContext &ctx) -> string
		{
			if (ctx.enemies.empty())
				return ctx.unitName + " rages at nobody.";
			float ratio = static_cast<float>(ctx.unitHp) / ctx.unitMaxHp;
			float mult = (ratio <= 0.25f)	? 3.0f
				     : (ratio <= 0.50f) ? 2.0f
				     : (ratio <= 0.75f) ? 1.5f
							: 1.0f;
			int dmg = static_cast<int>(ctx.unitBaseDamage * mult);
			return ctx.unitName + " [Berserker] hits \"" +
			       ctx.enemies.front().name + "\" for " + std::to_string(dmg);
		};
	}

	CombatStrategy makeSupport(int buffAmount = 20)
	{
		return [buffAmount](const BattleContext &ctx) -> string
		{
			if (!ctx.allies.empty())
			{
				auto it = std::max_element(ctx.allies.begin(), ctx.allies.end(),
							   [](const auto &a, const auto &b)
							   { return a.hp < b.hp; });
				return ctx.unitName + " buffs \"" + it->name + "\" by " +
				       std::to_string(buffAmount);
			}
			if (ctx.enemies.empty())
				return ctx.unitName + " stands by.";
			return ctx.unitName + " attacks \"" + ctx.enemies.front().name + "\".";
		};
	}

	//  2. FACTORY METHOD — map of creator functions

	struct EnemyData
	{
		string name;
		int hp;
		int damage;
	};
	struct WeaponData
	{
		string name;
		int damage;
		string type;
	};

	using EnemyCreator = std::function<EnemyData()>;
	using WeaponCreator = std::function<WeaponData()>;

	struct BiomeProducts
	{
		EnemyCreator createEnemy;
		WeaponCreator createWeapon;
	};

	// Registry: biome name -> product creators
	const std::unordered_map<string, BiomeProducts> biomeRegistry{
	    {"forest", {[]
			{ return EnemyData{"Goblin", 40, 8}; }, []
			{ return WeaponData{"WoodBow", 12, "ranged"}; }}},
	    {"dungeon", {[]
			 { return EnemyData{"Orc", 80, 15}; }, []
			 { return WeaponData{"IronSword", 20, "melee"}; }}},
	    {"volcano", {[]
			 { return EnemyData{"Dragon", 200, 35}; }, []
			 { return WeaponData{"FlameSword", 40, "melee"}; }}},
	};

	// "Factory provider" — returns a BiomeProducts by key
	const BiomeProducts &getBiomeFactory(const string &biome)
	{
		auto it = biomeRegistry.find(biome);
		if (it == biomeRegistry.end())
			throw std::invalid_argument("Unknown biome: " + biome);
		return it->second;
	}

	void describeBiome(const string &biome)
	{
		const auto &f = getBiomeFactory(biome);
		auto e = f.createEnemy();
		auto w = f.createWeapon();
		cout << "Biome [" << biome << "]:\n"
		     << "  Enemy : " << e.name << " HP=" << e.hp << " DMG=" << e.damage << "\n"
		     << "  Weapon: " << w.name << " DMG=" << w.damage << " type=" << w.type << "\n";
	}

	//  3. DECORATOR — function composition instead of wrappers

	struct AttackResult
	{
		int damage;
		string description;
	};

	// Base attack — plain function / lambda
	using AttackFn = std::function<AttackResult()>;

	AttackFn makeBaseAttack(int dmg)
	{
		return [dmg]() -> AttackResult
		{
			return {dmg, "Base attack"};
		};
	}

	// Decorators are higher-order functions
	AttackFn withFireBoost(AttackFn inner)
	{
		return [inner]() -> AttackResult
		{
			auto r = inner();
			return {r.damage + 20, r.description + " + FireBoost(+20)"};
		};
	}

	AttackFn withPoisonCoat(AttackFn inner)
	{
		return [inner]() -> AttackResult
		{
			auto r = inner();
			return {r.damage + 15, r.description + " + PoisonCoat(+15)"};
		};
	}

	AttackFn withCriticalStrike(AttackFn inner)
	{
		return [inner]() -> AttackResult
		{
			auto r = inner();
			return {static_cast<int>(r.damage * 1.5f),
				r.description + " + CriticalStrike(x1.5)"};
		};
	}

	//  4. EXECUTE AROUND — resource management via higher-order fn

	// Generic execute-around: setup -> body(resource) -> teardown
	template <typename Resource, typename Body>
	auto executeAround(
	    std::function<Resource()> acquire,
	    std::function<void(Resource &)> release,
	    Body body)
	{
		Resource res = acquire();
		// RAII-style guard so release fires even on exception
		struct Guard
		{
			Resource &r;
			std::function<void(Resource &)> &rel;
			~Guard() { rel(r); }
		} guard{res, release};

		return body(res);
	}

	// Convenience: execute-around for a "battle session"
	struct BattleSession
	{
		string name;
		int round = 0;
	};

	template <typename Body>
	auto withBattleSession(const string &sessionName, Body body)
	{
		return executeAround<BattleSession>(
		    [&sessionName]() -> BattleSession
		    {
			    cout << "  [Session] Opening: " << sessionName << "\n";
			    return {sessionName, 0};
		    },
		    [](BattleSession &s)
		    {
			    cout << "  [Session] Closing: " << s.name
				 << " (rounds played: " << s.round << ")\n";
		    },
		    body);
	}

} // namespace fp

int task_05()
{
	using namespace fp;
	cout << "task_05: Functional Paradigm Patterns\n";

	// 1. Strategy — swap functions at runtime
	cout << "Pattern 1 : Strategy(functional)\n";

	BattleContext ctx;
	ctx.unitName = "Soldier";
	ctx.unitHp = 40;
	ctx.unitMaxHp = 100;
	ctx.unitBaseDamage = 25;
	ctx.enemies = {{"Goblin", 30, 30}, {"Orc", 80, 80}};
	ctx.allies = {{"Archer", 60, 60}};

	// Strategy is just a std::function — swap without inheritance
	CombatStrategy strategy = makeAggressive();
	cout << "  " << strategy(ctx) << "\n";

	strategy = makeDefensive(30);
	cout << "  " << strategy(ctx) << "\n"; // hp < half -> heals

	ctx.unitHp = 20;
	strategy = makeBerserker();
	cout << "  " << strategy(ctx) << "\n"; // rage mult applies

	strategy = makeSupport(20);
	cout << "  " << strategy(ctx) << "\n";

	// 2. Factory Method — registry lookup
	cout << "Pattern 2: Factory Method (functional)\n";

	for (const auto &biome : {"forest", "dungeon", "volcano"})
		describeBiome(biome);

	try
	{
		getBiomeFactory("swamp");
	}
	catch (const std::invalid_argument &ex)
	{
		cout << "  Exception: " << ex.what() << "\n";
	}

	// 3. Decorator — function composition
	cout << "Pattern 3: Decorator (functional)\n";

	// Compose decorators left-to-right
	AttackFn atk = makeBaseAttack(30);
	cout << "  Base:   " << atk().description << " = " << atk().damage << "\n";

	atk = withFireBoost(makeBaseAttack(30));
	cout << "  +Fire:  " << atk().description << " = " << atk().damage << "\n";

	atk = withCriticalStrike(withPoisonCoat(withFireBoost(makeBaseAttack(30))));
	cout << "  Full:   " << atk().description << " = " << atk().damage << "\n";

	// 4. Execute Around — resource bracketing
	cout << "Pattern 4: Execute Around\n";

	int totalDmg = withBattleSession("Arena Round 1", [](BattleSession &s)
					 {
		AttackFn combo = withCriticalStrike(withFireBoost(makeBaseAttack(30)));
		int dmg = 0;
		for (int i = 0; i < 3; ++i) {
			++s.round;
			auto r = combo();
			cout << "  Round " << s.round << ": " << r.description
				<< " -> " << r.damage << "\n";
			dmg += r.damage;       
		}
		return dmg; });
	cout << "  Total damage dealt: " << totalDmg << "\n";

	return 0;
}
