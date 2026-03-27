#include "Behavioral/ChainOfResponsibility.h"
#include <algorithm>

//
// DamageRequest
//

DamageRequest::DamageRequest(std::string src, std::string type, int dmg)
	: source(std::move(src)), damageType(std::move(type)), amount(dmg) {}

//
// DamageHandler base
//

DamageHandler::DamageHandler(std::string name)
	: handlerName_(std::move(name)) {}

DamageHandler *DamageHandler::setNext(std::unique_ptr<DamageHandler> next)
{
	next_ = std::move(next);
	return next_.get();
}

void DamageHandler::handle(DamageRequest &req)
{
	if (req.amount <= 0)
	{
		std::cout << "    [" << handlerName_
				  << "] damage already nullified — skipping\n";
		req.handled = true;
		return;
	}

	process(req);

	if (!req.handled && next_)
		next_->handle(req);
}

void DamageHandler::log(const std::string &msg) const
{
	std::cout << "    [" << handlerName_ << "] " << msg << "\n";
}

//
// DodgeHandler
//

DodgeHandler::DodgeHandler(int chance)
	: DamageHandler("DodgeHandler"), dodgeChance_(chance) {}

void DodgeHandler::process(DamageRequest &req)
{
	if (req.damageType != "physical")
	{
		log("not a physical hit — cannot dodge, passing on");
		return;
	}

	bool dodged = (req.amount % 7 == 0);

	if (dodged)
	{
		log("DODGE! " + req.source + "'s attack of " +
			std::to_string(req.amount) + " missed completely");
		req.amount = 0;
		req.handled = true;
	}
	else
	{
		log("failed to dodge (" + std::to_string(dodgeChance_) +
			"% chance), passing " + std::to_string(req.amount) + " dmg on");
	}
}

//
// BlockHandler
//

BlockHandler::BlockHandler(int block)
	: DamageHandler("BlockHandler"), blockValue_(block) {}

void BlockHandler::process(DamageRequest &req)
{
	if (req.damageType != "physical")
	{
		log("shield only blocks physical — passing on");
		return;
	}

	int blocked = std::min(req.amount, blockValue_);
	req.amount -= blocked;

	log("shield blocks " + std::to_string(blocked) +
		" dmg  →  " + std::to_string(req.amount) + " remaining");

	if (req.amount <= 0)
		req.handled = true;
}

//
// ArmorHandler
//

ArmorHandler::ArmorHandler(int reductionPct)
	: DamageHandler("ArmorHandler"), reductionPct_(reductionPct) {}

void ArmorHandler::process(DamageRequest &req)
{
	if (req.damageType == "magic")
	{
		log("armor useless vs magic — passing on");
		return;
	}

	int reduced = req.amount * reductionPct_ / 100;
	req.amount -= reduced;

	log("armor absorbs " + std::to_string(reduced) + " dmg (" +
		std::to_string(reductionPct_) + "%)  →  " +
		std::to_string(req.amount) + " remaining");

	if (req.amount <= 0)
		req.handled = true;
}

//
// ShieldSpellHandler
//

ShieldSpellHandler::ShieldSpellHandler(int barrier)
	: DamageHandler("ShieldSpellHandler"), barrier_(barrier) {}

void ShieldSpellHandler::process(DamageRequest &req)
{
	if (req.damageType == "physical")
	{
		log("spell shield ignores physical — passing on");
		return;
	}

	if (barrier_ <= 0)
	{
		log("spell shield depleted — passing on");
		return;
	}

	int absorbed = std::min(req.amount, barrier_);
	barrier_ -= absorbed;
	req.amount -= absorbed;

	log("spell shield absorbs " + std::to_string(absorbed) +
		" dmg  →  barrier=" + std::to_string(barrier_) +
		"  remaining dmg=" + std::to_string(req.amount));

	if (req.amount <= 0)
		req.handled = true;
}

//
// HealthHandler
//

HealthHandler::HealthHandler(int &hp)
	: DamageHandler("HealthHandler"), hp_(hp) {}

void HealthHandler::process(DamageRequest &req)
{
	hp_ -= req.amount;
	if (hp_ < 0)
		hp_ = 0;

	log(std::to_string(req.amount) +
		" dmg hits HP directly  →  HP now = " +
		std::to_string(hp_));

	req.handled = true;
}

//
// Builder
//

std::unique_ptr<DamageHandler> buildDefenseChain(
	int dodgeChance,
	int blockValue,
	int armorPct,
	int spellBarrier,
	int &heroHp)
{
	auto dodge = std::make_unique<DodgeHandler>(dodgeChance);
	auto block = std::make_unique<BlockHandler>(blockValue);
	auto armor = std::make_unique<ArmorHandler>(armorPct);
	auto shield = std::make_unique<ShieldSpellHandler>(spellBarrier);
	auto health = std::make_unique<HealthHandler>(heroHp);

	armor->setNext(std::move(shield))->setNext(std::move(health));
	block->setNext(std::move(armor));
	dodge->setNext(std::move(block));

	return dodge;
}
