#include "State.h"

//
// BossEnemy
//

BossEnemy::BossEnemy(std::string name, int hp, int dmg)
    : name_(std::move(name)), hp_(hp), maxHp_(hp), baseDamage_(dmg) {}

void BossEnemy::setState(std::shared_ptr<IState> newState)
{
	if (state_)
	{
		std::cout << "    Transition: " << state_->stateName()
			  << " -> " << newState->stateName() << "\n";
		state_->onExit(*this);
	}
	state_ = std::move(newState);
	state_->onEnter(*this);
}

void BossEnemy::update()
{
	if (state_)
		state_->update(*this);
}

void BossEnemy::takeDamage(int dmg)
{
	int effective = std::max(0, dmg - armor_);
	hp_ = std::max(0, hp_ - effective);

	std::cout << "    Boss took " << effective << " dmg\n";

	if (state_)
		state_->takeDamage(*this, effective);
}

// getters
const std::string &BossEnemy::getName() const { return name_; }
int BossEnemy::getHp() const { return hp_; }
int BossEnemy::getMaxHp() const { return maxHp_; }
int BossEnemy::getBaseDamage() const { return baseDamage_; }

float BossEnemy::hpRatio() const
{
	return static_cast<float>(hp_) / maxHp_;
}

void BossEnemy::setArmor(int a) { armor_ = a; }
int BossEnemy::getArmor() const { return armor_; }

void BossEnemy::setShield(bool on) { shieldActive_ = on; }
bool BossEnemy::hasShield() const { return shieldActive_; }

void BossEnemy::spawnMinion()
{
	++minionsSpawned_;
	std::cout << "    Minion spawned #" << minionsSpawned_ << "\n";
}

int BossEnemy::getMinionCount() const { return minionsSpawned_; }

void BossEnemy::showStatus() const
{
	std::cout << "    HP=" << hp_
		  << " State=" << (state_ ? state_->stateName() : "none")
		  << "\n";
}

//
// IdleState
//

std::string IdleState::stateName() const { return "Idle"; }

void IdleState::onEnter(BossEnemy &boss)
{
	std::cout << "    Idle...\n";
	boss.setArmor(0);
}

void IdleState::onExit(BossEnemy &) {}

void IdleState::update(BossEnemy &) {}

void IdleState::takeDamage(BossEnemy &boss, int)
{
	boss.setState(std::make_shared<EnragedState>());
}

//
// EnragedState
//

std::string EnragedState::stateName() const { return "Enraged"; }

void EnragedState::onEnter(BossEnemy &boss)
{
	boss.setArmor(5);
	turnCount_ = 0;
}

void EnragedState::onExit(BossEnemy &) {}

void EnragedState::update(BossEnemy &boss)
{
	++turnCount_;
	std::cout << "    Attack: "
		  << boss.getBaseDamage() + turnCount_ * 5 << "\n";
}

void EnragedState::takeDamage(BossEnemy &boss, int)
{
	if (boss.hpRatio() < 0.6f)
		boss.setState(std::make_shared<WoundedState>());
}

//
// WoundedState
//

std::string WoundedState::stateName() const { return "Wounded"; }

void WoundedState::onEnter(BossEnemy &boss)
{
	boss.setArmor(15);
}

void WoundedState::onExit(BossEnemy &boss)
{
	boss.setShield(false);
}

void WoundedState::update(BossEnemy &boss)
{
	++turnCount_;
	if (turnCount_ % 2 == 0)
		std::cout << "    Attack\n";
	else
		boss.setShield(true);
}

void WoundedState::takeDamage(BossEnemy &boss, int)
{
	if (boss.hpRatio() < 0.3f)
		boss.setState(std::make_shared<DespairState>());
}

//
// DespairState
//

std::string DespairState::stateName() const { return "Despair"; }

void DespairState::onEnter(BossEnemy &boss)
{
	boss.setArmor(0);
	boss.spawnMinion();
}

void DespairState::onExit(BossEnemy &) {}

void DespairState::update(BossEnemy &boss)
{
	std::cout << "    Frenzy attack: "
		  << boss.getBaseDamage() * 2 << "\n";
}

void DespairState::takeDamage(BossEnemy &boss, int)
{
	if (boss.getHp() <= 0)
		std::cout << "    Boss defeated\n";
}
