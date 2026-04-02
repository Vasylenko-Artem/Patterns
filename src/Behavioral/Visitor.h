#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <vector>

namespace vis
{
	//  Forward declarations
	class Weapon;
	class Armor;
	class Potion;
	class QuestItem;

	//  Visitor interface
	class IVisitor
	{
	public:
		virtual void visit(Weapon &item) = 0;
		virtual void visit(Armor &item) = 0;
		virtual void visit(Potion &item) = 0;
		virtual void visit(QuestItem &item) = 0;
		virtual ~IVisitor() = default;
	};

	//  Element interface
	class IItem
	{
	public:
		virtual void accept(IVisitor &visitor) = 0;
		virtual std::string name() const = 0;
		virtual ~IItem() = default;
	};

	//
	// Elements
	//

	class Weapon : public IItem
	{
		std::string name_;
		int damage_;
		float weight_;
		int goldValue_;
		int upgradeLevel_ = 0;

	public:
		Weapon(std::string name, int damage, float weight, int gold);

		void accept(IVisitor &visitor) override;

		std::string name() const override;
		int getDamage() const;
		float getWeight() const;
		int getGold() const;
		int getUpgrade() const;
		void upgrade();
	};

	class Armor : public IItem
	{
		std::string name_;
		int defense_;
		float weight_;
		int goldValue_;
		int upgradeLevel_ = 0;

	public:
		Armor(std::string name, int defense, float weight, int gold);

		void accept(IVisitor &visitor) override;

		std::string name() const override;
		int getDefense() const;
		float getWeight() const;
		int getGold() const;
		int getUpgrade() const;
		void upgrade();
	};

	class Potion : public IItem
	{
		std::string name_;
		std::string effect_;
		int potency_;
		float weight_;
		int goldValue_;

	public:
		Potion(std::string name, std::string effect, int potency,
		       float weight, int gold);

		void accept(IVisitor &visitor) override;

		std::string name() const override;
		std::string getEffect() const;
		int getPotency() const;
		float getWeight() const;
		int getGold() const;
	};

	class QuestItem : public IItem
	{
		std::string name_;
		std::string quest_;
		bool isKeyItem_;
		float weight_;

	public:
		QuestItem(std::string name, std::string quest, bool key, float weight);

		void accept(IVisitor &visitor) override;

		std::string name() const override;
		std::string getQuest() const;
		bool isKey() const;
		float getWeight() const;
	};

	//
	// Visitors
	//

	class StatsPrinter : public IVisitor
	{
	public:
		void visit(Weapon &w) override;
		void visit(Armor &a) override;
		void visit(Potion &p) override;
		void visit(QuestItem &q) override;
	};

	class WeightCalculator : public IVisitor
	{
		float total_ = 0.0f;

	public:
		void visit(Weapon &w) override;
		void visit(Armor &a) override;
		void visit(Potion &p) override;
		void visit(QuestItem &q) override;

		float total() const;
		void reset();
	};

	class LootEvaluator : public IVisitor
	{
		int total_ = 0;
		int unsellable_ = 0;

	public:
		void visit(Weapon &w) override;
		void visit(Armor &a) override;
		void visit(Potion &p) override;
		void visit(QuestItem &q) override;

		int total() const;
		int unsellable() const;
		void reset();
	};

	class UpgradeVisitor : public IVisitor
	{
		int upgraded_ = 0;
		int skipped_ = 0;

	public:
		void visit(Weapon &w) override;
		void visit(Armor &a) override;
		void visit(Potion &p) override;
		void visit(QuestItem &q) override;

		int upgraded() const;
		int skipped() const;
	};
} // namespace vis
