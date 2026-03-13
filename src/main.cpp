#include "GameManager.h"
#include "EnemyFactory.h"
#include "BiomeFactory.h"
#include "HeroBuilder.h"
#include "Prototype.h"

using namespace std;

void playBiome(const BiomeFactory &factory)
{
	cout << "\n Biome: " << factory.getBiomeName() << endl;
	auto enemy = factory.createEnemy();
	auto weapon = factory.createWeapon();
	enemy->showInfo();
	weapon->showInfo();
	weapon->use();
	enemy->attack();
}

int main()
{
	// Pattern 1: Singleton
	cout << "Pattern 1 — Singleton\n";
	GameManager &gm1 = GameManager::getInstance();
	GameManager &gm2 = GameManager::getInstance();
	cout << "gm1 == gm2 : "
	     << (&gm1 == &gm2 ? "true" : "false") << endl;

	gm1.startGame("adventure");
	gm1.addScore(1000);

	cout << "\tScore gm1: " << gm1.getScore() << endl;
	cout << "\tScore gm2: " << gm2.getScore() << endl;

	// Pattern 2: Factory Method
	cout << "\nPattern 2 — Factory Method\n";
	auto levelFactory = getFactoryForLevel("hard");
	auto boss = levelFactory->spawnEnemy();
	boss->attack();

	// Pattern 3: Abstract Factory
	cout << "\nPattern 3 — Abstract Factory\n";
	auto forestFactory = BiomeFactoryProvider::getFactory("forest");
	auto volcanoFactory = BiomeFactoryProvider::getFactory("volcano");
	playBiome(*forestFactory);
	playBiome(*volcanoFactory);

	// Pattern 4: Builder
	cout << "\nPattern 4 — Builder\n";
	HeroDirector director;
	WarriorBuilder warriorBuilder;
	MageBuilder mageBuilder;

	director.setBuilder(&warriorBuilder);
	director.buildFullHero("Hero");
	auto masterWarrior = warriorBuilder.getProduct();
	masterWarrior.showInfo();

	director.setBuilder(&mageBuilder);
	director.buildFullHero("Builder");
	auto masterMage = mageBuilder.getProduct();
	masterMage.showInfo();

	// Pattern 5: Prototype
	cout << "\nPattern 5 — Prototype\n";

	auto heroProto = make_unique<HeroPrototype>(
	    "Hero-Template", "Warrior", 250, 30,
	    "Plate armor", "Two-handed sword",
	    vector<string>{"Berserk", "Shield strike", "Iron skin"});

	auto orcProto = make_unique<EnemyPrototype>(
	    "Orc-Template", "Melee", 80, 20, "A piece of iron");

	auto eliteOrcProto = make_unique<EnemyPrototype>(
	    "Elite Orc Template", "Melee", 160, 40, "A rare sword");

	// Registry of prototypes
	PrototypeRegistry registry;
	registry.registerHero("warrior", std::move(heroProto));
	registry.registerEnemy("orc", std::move(orcProto));
	registry.registerEnemy("elite_orc", std::move(eliteOrcProto));
	registry.listPrototypes();

	auto hero1 = registry.cloneHero("warrior");
	hero1->withName("Player 1");
	hero1->showInfo();

	auto hero2 = registry.cloneHero("warrior");
	hero2->withName("Player 2")
	    .withHp(350)
	    .withWeapon("Legendary sword")
	    .addAbility("Boost speed");
	hero2->showInfo();

	// Spawn enemies
	for (int i = 1; i <= 3; ++i)
	{
		auto orc = registry.cloneEnemy("orc");
		orc->withName("Орк #" + to_string(i));
		orc->showInfo();
		orc->attack();
		gm1.addScore(-orc->getDamage());
	}

	for (int i = 1; i <= 2; ++i)
	{
		auto elite = registry.cloneEnemy("elite_orc");
		elite->withName("Елітний Орк #" + to_string(i))
		    .withDamage(40 + i * 5);
		elite->showInfo();
		elite->attack();
		gm1.addScore(-elite->getDamage());
	}

	gm1.addScore(hero1->getHp() + hero2->getHp());
	gm1.endGame();
	return 0;
}
