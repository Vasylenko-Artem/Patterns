
#include "tasks.h"

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
int task_02()
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
