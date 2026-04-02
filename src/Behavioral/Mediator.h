#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

namespace med
{

	class BattleUnit;

	//  Mediator interface
	class IMediator
	{
	public:
		virtual void notify(BattleUnit *sender,
							const std::string &event,
							int value = 0) = 0;
		virtual ~IMediator() = default;
	};

	//  Abstract Colleague
	class BattleUnit
	{
	protected:
		IMediator *mediator_ = nullptr;
		std::string name_;
		int hp_;
		int maxHp_;

	public:
		BattleUnit(std::string name, int hp);

		void setMediator(IMediator *m);

		const std::string &getName() const;
		int getHp() const;
		bool isAlive() const;

		void takeDamage(int dmg);
		void receiveHeal(int amount);

		virtual void attack(BattleUnit *target) = 0;
		virtual void onEvent(const std::string &event,
							 BattleUnit *source,
							 int value) = 0;

		virtual ~BattleUnit() = default;
	};

	//  MedHero
	class MedHero : public BattleUnit
	{
		int attackPower_;

	public:
		MedHero(std::string name, int hp, int atk);

		void attack(BattleUnit *target) override;
		void onEvent(const std::string &event, BattleUnit *source, int value) override;
	};

	//  MedEnemy
	class MedEnemy : public BattleUnit
	{
		int attackPower_;

	public:
		MedEnemy(std::string name, int hp, int atk);

		void attack(BattleUnit *target) override;
		void onEvent(const std::string &event, BattleUnit *source, int value) override;
	};

	//  HealBot
	class HealBot : public BattleUnit
	{
		int healPower_;
		std::vector<BattleUnit *> allies_;

	public:
		HealBot(std::string name, int hp, int heal);

		void registerAlly(BattleUnit *ally);

		void attack(BattleUnit *target) override;
		void onEvent(const std::string &event, BattleUnit *source, int value) override;
	};

	//  UILogger
	class UILogger : public BattleUnit
	{
	public:
		explicit UILogger();

		void attack(BattleUnit *target) override;
		void onEvent(const std::string &event, BattleUnit *source, int value) override;
	};

	//  Mediator
	class BattleMediator : public IMediator
	{
		std::vector<BattleUnit *> heroes_;
		std::vector<BattleUnit *> enemies_;
		UILogger *logger_ = nullptr;

	public:
		void addHero(BattleUnit *u);
		void addEnemy(BattleUnit *u);
		void setLogger(UILogger *l);

		void notify(BattleUnit *sender,
					const std::string &event,
					int value = 0) override;

	private:
		bool isHero(BattleUnit *u) const;
	};

} // namespace med
