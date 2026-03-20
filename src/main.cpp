// #include "Childbirth/GameManager.h"
// #include "Childbirth/EnemyFactory.h"
// #include "Childbirth/BiomeFactory.h"
// #include "Childbirth/HeroBuilder.h"
// #include "Childbirth/Prototype.h"

// using namespace std;

// void playBiome(const BiomeFactory &factory)
// {
// 	cout << "\nBiome: " << factory.getBiomeName() << endl;
// 	auto enemy = factory.createEnemy();
// 	auto weapon = factory.createWeapon();
// 	enemy->showInfo();
// 	weapon->showInfo();
// 	weapon->use();
// 	enemy->attack();
// }

// int main()
// {
// 	// Pattern 1: Singleton
// 	cout << "Pattern 1 — Singleton\n";
// 	GameManager &gm1 = GameManager::getInstance();
// 	GameManager &gm2 = GameManager::getInstance();
// 	cout << "gm1 == gm2 : "
// 	     << (&gm1 == &gm2 ? "true" : "false") << endl;

// 	gm1.startGame("adventure");
// 	gm1.addScore(1000);

// 	cout << "\tScore gm1: " << gm1.getScore() << endl;
// 	cout << "\tScore gm2: " << gm2.getScore() << endl;

// 	// Pattern 2: Factory Method
// 	cout << "\nPattern 2 — Factory Method\n";
// 	auto levelFactory = getFactoryForLevel("hard");
// 	auto boss = levelFactory->spawnEnemy();
// 	boss->attack();

// 	// Pattern 3: Abstract Factory
// 	cout << "\nPattern 3 — Abstract Factory\n";
// 	auto forestFactory = BiomeFactoryProvider::getFactory("forest");
// 	auto volcanoFactory = BiomeFactoryProvider::getFactory("volcano");
// 	playBiome(*forestFactory);
// 	playBiome(*volcanoFactory);

// 	// Pattern 4: Builder
// 	cout << "\nPattern 4 — Builder\n";
// 	HeroDirector director;
// 	WarriorBuilder warriorBuilder;
// 	MageBuilder mageBuilder;

// 	director.setBuilder(&warriorBuilder);
// 	director.buildFullHero("Hero");
// 	auto masterWarrior = warriorBuilder.getProduct();
// 	masterWarrior.showInfo();

// 	director.setBuilder(&mageBuilder);
// 	director.buildFullHero("Builder");
// 	auto masterMage = mageBuilder.getProduct();
// 	masterMage.showInfo();

// 	// Pattern 5: Prototype
// 	cout << "\nPattern 5 — Prototype\n";

// 	auto heroProto = make_unique<HeroPrototype>(
// 	    "Hero-Template", "Warrior", 250, 30,
// 	    "Plate armor", "Two-handed sword",
// 	    vector<string>{"Berserk", "Shield strike", "Iron skin"});

// 	auto orcProto = make_unique<EnemyPrototype>(
// 	    "Orc-Template", "Melee", 80, 20, "A piece of iron");

// 	auto eliteOrcProto = make_unique<EnemyPrototype>(
// 	    "Elite Orc Template", "Melee", 160, 40, "A rare sword");

// 	// Registry of prototypes
// 	PrototypeRegistry registry;
// 	registry.registerHero("warrior", std::move(heroProto));
// 	registry.registerEnemy("orc", std::move(orcProto));
// 	registry.registerEnemy("elite_orc", std::move(eliteOrcProto));
// 	registry.listPrototypes();

// 	auto hero1 = registry.cloneHero("warrior");
// 	hero1->withName("Player 1");
// 	hero1->showInfo();

// 	auto hero2 = registry.cloneHero("warrior");
// 	hero2->withName("Player 2")
// 	    .withHp(350)
// 	    .withWeapon("Legendary sword")
// 	    .addAbility("Boost speed");
// 	hero2->showInfo();

// 	// Spawn enemies
// 	for (int i = 1; i <= 3; ++i)
// 	{
// 		auto orc = registry.cloneEnemy("orc");
// 		orc->withName("Орк #" + to_string(i));
// 		orc->showInfo();
// 		orc->attack();
// 		gm1.addScore(-orc->getDamage());
// 	}

// 	for (int i = 1; i <= 2; ++i)
// 	{
// 		auto elite = registry.cloneEnemy("elite_orc");
// 		elite->withName("Елітний Орк #" + to_string(i))
// 		    .withDamage(40 + i * 5);
// 		elite->showInfo();
// 		elite->attack();
// 		gm1.addScore(-elite->getDamage());
// 	}

// 	gm1.addScore(hero1->getHp() + hero2->getHp());
// 	gm1.endGame();
// 	return 0;
// }

#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "Structural/Adapter.h"
#include "Structural/Composite.h"
#include "Structural/Proxy.h"
#include "Structural/Flyweight.h"
#include "Structural/Facade.h"
#include "Structural/Bridge.h"
#include "Structural/Decorator.h"

using namespace std;
int main()
{
	// Pattern 1 — Adapter
	// Old XML save system -> new ISaveSystem interface
	cout << " Pattern 1 — Adapter\n";
	{
		// The engine only knows ISaveSystem; we plug in the XML adapter
		unique_ptr<ISaveSystem> save = make_unique<XmlToJsonSaveAdapter>();

		save->save("PlayerOne", 4200);
		string data = save->load("PlayerOne");
		cout << "  Loaded: " << data << "\n";
	}

	// Pattern 2 — Composite
	// Scene tree: Level -> Zone -> individual game objects
	cout << "\nPattern 2 — Composite\n";
	{
		// Leaf nodes
		auto player = make_shared<GameObjectNode>("Hero", "Player");
		auto orc1 = make_shared<GameObjectNode>("Orc Guard", "Enemy");
		auto orc2 = make_shared<GameObjectNode>("Orc Shaman", "Enemy");
		auto chest = make_shared<GameObjectNode>("Chest", "Item");
		auto boss = make_shared<GameObjectNode>("Dragon Boss", "Boss");
		auto shrine = make_shared<GameObjectNode>("Shrine", "Object");

		// Groups
		auto enemyZone = make_shared<SceneGroup>("Enemy Zone");
		enemyZone->add(orc1);
		enemyZone->add(orc2);

		auto bossRoom = make_shared<SceneGroup>("Boss Room");
		bossRoom->add(boss);
		bossRoom->add(shrine);

		auto level = make_shared<SceneGroup>("Level 1 — Dark Forest");
		level->add(player);
		level->add(chest);
		level->add(enemyZone);
		level->add(bossRoom);

		// Render entire tree with one call
		level->render();
	}

	// Pattern 3 — Proxy
	// DatabaseProxy: logs requests, blocks unauthorised writes
	cout << "\nPattern 3 — Proxy\n";
	{
		DatabaseProxy guestProxy("guest");
		DatabaseProxy adminProxy("admin");

		cout << "  [guest] query:\n";
		string result = guestProxy.query("SELECT * FROM scores");
		cout << "  Result: " << result << "\n";

		cout << "  [guest] execute (should be denied):\n";
		guestProxy.execute("DELETE FROM scores WHERE id=1");

		cout << "  [admin] execute (should succeed):\n";
		adminProxy.execute("INSERT INTO scores VALUES ('Hero', 9999)");
	}

	// Pattern 4 — Flyweight
	// Thousands of particles share texture/color/size objects
	cout << "\nPattern 4 — Flyweight\n";
	{
		ParticleFactory factory;

		// Simulate spawning many particles of few types
		vector<Particle> scene;
		for (int i = 0; i < 3; ++i)
		{
			auto bulletType = factory.get("bullet.png", "yellow", 2.0f);
			scene.push_back({bulletType,
					 static_cast<float>(i * 10), 0.f,
					 5.f, 0.f});
		}
		for (int i = 0; i < 2; ++i)
		{
			auto sparkType = factory.get("spark.png", "orange", 1.0f);
			scene.push_back({sparkType,
					 static_cast<float>(i * 5), static_cast<float>(i * 5),
					 -1.f, 2.f});
		}
		// Another bullet — reuses existing flyweight (no new object created)
		auto bulletType = factory.get("bullet.png", "yellow", 2.0f);
		scene.push_back({bulletType, 99.f, 0.f, 5.f, 0.f});

		cout << "  Rendering " << scene.size()
		     << " particles using only " << factory.poolSize()
		     << " shared ParticleType objects:\n";
		for (auto &p : scene)
			p.render();
	}

	// Pattern 5 — Facade
	// One simple call hides codec/mixer/device complexity
	cout << "\nPattern 5 — Facade\n";
	{
		GameAudioFacade audio;

		audio.playMusic("dark_forest_theme.ogg", 70);
		audio.playSfx("sword_clash.wav");
		audio.playSfx("orc_death.wav");
		audio.stopMusic();
	}

	// Pattern 6 — Bridge
	// Switch render API without changing drawable objects
	cout << "\nPattern 6 — Bridge\n";
	{
		auto oglRenderer = make_shared<OpenGLRenderer>();
		auto dxRenderer = make_shared<DirectXRenderer>();

		// Same sprite — different backend
		Sprite heroSprite(oglRenderer, "hero.png", 100, 200);
		Sprite orcSprite(dxRenderer, "orc.png", 300, 150);

		// Same mesh — different backend
		Mesh3D bossMeshOGL(oglRenderer, "dragon.obj", 2.5f);
		Mesh3D bossMeshDX(dxRenderer, "dragon.obj", 2.5f);

		heroSprite.draw();
		orcSprite.draw();
		bossMeshOGL.draw();
		bossMeshDX.draw();
	}

	// Pattern 7 — Decorator
	// Stack power-up bonuses on top of a base attack at runtime
	cout << "\nPattern 7 — Decorator\n";
	{
		// Base attack
		shared_ptr<IAttack> attack = make_shared<BaseAttack>(50);
		cout << "  " << attack->description()
		     << "  ->  dmg=" << attack->damage() << "\n";

		// Wrap with FireBoost
		attack = make_shared<FireBoostDecorator>(attack);
		cout << "  " << attack->description()
		     << "  ->  dmg=" << attack->damage() << "\n";

		// Wrap with PoisonCoat
		attack = make_shared<PoisonCoatDecorator>(attack);
		cout << "  " << attack->description()
		     << "  ->  dmg=" << attack->damage() << "\n";

		// Wrap with CriticalStrike (multiplier applied last)
		attack = make_shared<CriticalStrikeDecorator>(attack);
		cout << "  " << attack->description()
		     << "  ->  dmg=" << attack->damage() << "\n";
	}

	return 0;
}
