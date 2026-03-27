#include "Visitor.h"

namespace vis
{
	//
	// Weapon
	//

	Weapon::Weapon(std::string name, int damage, float weight, int gold)
	    : name_(std::move(name)), damage_(damage),
	      weight_(weight), goldValue_(gold) {}

	void Weapon::accept(IVisitor &visitor) { visitor.visit(*this); }

	std::string Weapon::name() const { return name_; }
	int Weapon::getDamage() const { return damage_ + upgradeLevel_ * 5; }
	float Weapon::getWeight() const { return weight_; }
	int Weapon::getGold() const { return goldValue_ + upgradeLevel_ * 20; }
	int Weapon::getUpgrade() const { return upgradeLevel_; }
	void Weapon::upgrade() { ++upgradeLevel_; }

	//
	// Armor
	//

	Armor::Armor(std::string name, int defense, float weight, int gold)
	    : name_(std::move(name)), defense_(defense),
	      weight_(weight), goldValue_(gold) {}

	void Armor::accept(IVisitor &visitor) { visitor.visit(*this); }

	std::string Armor::name() const { return name_; }
	int Armor::getDefense() const { return defense_ + upgradeLevel_ * 3; }
	float Armor::getWeight() const { return weight_; }
	int Armor::getGold() const { return goldValue_ + upgradeLevel_ * 15; }
	int Armor::getUpgrade() const { return upgradeLevel_; }
	void Armor::upgrade() { ++upgradeLevel_; }

	//
	// Potion
	//

	Potion::Potion(std::string name, std::string effect, int potency,
		       float weight, int gold)
	    : name_(std::move(name)), effect_(std::move(effect)),
	      potency_(potency), weight_(weight), goldValue_(gold) {}

	void Potion::accept(IVisitor &visitor) { visitor.visit(*this); }

	std::string Potion::name() const { return name_; }
	std::string Potion::getEffect() const { return effect_; }
	int Potion::getPotency() const { return potency_; }
	float Potion::getWeight() const { return weight_; }
	int Potion::getGold() const { return goldValue_; }

	//
	// QuestItem
	//

	QuestItem::QuestItem(std::string name, std::string quest, bool key, float weight)
	    : name_(std::move(name)), quest_(std::move(quest)),
	      isKeyItem_(key), weight_(weight) {}

	void QuestItem::accept(IVisitor &visitor) { visitor.visit(*this); }

	std::string QuestItem::name() const { return name_; }
	std::string QuestItem::getQuest() const { return quest_; }
	bool QuestItem::isKey() const { return isKeyItem_; }
	float QuestItem::getWeight() const { return weight_; }

	//
	// Visitors
	//

	void StatsPrinter::visit(Weapon &w)
	{
		std::cout << "[Weapon] " << w.name()
			  << " dmg=" << w.getDamage() << "\n";
	}

	void StatsPrinter::visit(Armor &a)
	{
		std::cout << "[Armor] " << a.name()
			  << " def=" << a.getDefense() << "\n";
	}

	void StatsPrinter::visit(Potion &p)
	{
		std::cout << "[Potion] " << p.name()
			  << " effect=" << p.getEffect() << "\n";
	}

	void StatsPrinter::visit(QuestItem &q)
	{
		std::cout << "[QuestItem] " << q.name() << "\n";
	}

	// WeightCalculator
	void WeightCalculator::visit(Weapon &w) { total_ += w.getWeight(); }
	void WeightCalculator::visit(Armor &a) { total_ += a.getWeight(); }
	void WeightCalculator::visit(Potion &p) { total_ += p.getWeight(); }
	void WeightCalculator::visit(QuestItem &q) { total_ += q.getWeight(); }

	float WeightCalculator::total() const { return total_; }
	void WeightCalculator::reset() { total_ = 0.0f; }

	// LootEvaluator
	void LootEvaluator::visit(Weapon &w) { total_ += w.getGold(); }
	void LootEvaluator::visit(Armor &a) { total_ += a.getGold(); }
	void LootEvaluator::visit(Potion &p) { total_ += p.getGold(); }

	void LootEvaluator::visit(QuestItem &q)
	{
		if (q.isKey())
			++unsellable_;
	}

	int LootEvaluator::total() const { return total_; }
	int LootEvaluator::unsellable() const { return unsellable_; }
	void LootEvaluator::reset()
	{
		total_ = 0;
		unsellable_ = 0;
	}

	// UpgradeVisitor
	void UpgradeVisitor::visit(Weapon &w)
	{
		w.upgrade();
		++upgraded_;
	}

	void UpgradeVisitor::visit(Armor &a)
	{
		a.upgrade();
		++upgraded_;
	}

	void UpgradeVisitor::visit(Potion &)
	{
		++skipped_;
	}

	void UpgradeVisitor::visit(QuestItem &)
	{
		++skipped_;
	}

	int UpgradeVisitor::upgraded() const { return upgraded_; }
	int UpgradeVisitor::skipped() const { return skipped_; }
}
