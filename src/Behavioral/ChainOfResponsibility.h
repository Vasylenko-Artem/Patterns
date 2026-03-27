#pragma once
#include <iostream>
#include <string>
#include <memory>

//  Request
struct DamageRequest
{
	std::string source;
	std::string damageType;
	int amount;
	bool handled = false;

	DamageRequest(std::string src, std::string type, int dmg);
};

//  Abstract Handler
class DamageHandler
{
protected:
	std::unique_ptr<DamageHandler> next_;
	std::string handlerName_;

public:
	explicit DamageHandler(std::string name);

	DamageHandler *setNext(std::unique_ptr<DamageHandler> next);

	virtual void handle(DamageRequest &req);
	virtual ~DamageHandler() = default;

protected:
	virtual void process(DamageRequest &req) = 0;
	void log(const std::string &msg) const;
};

//  Concrete Handlers
class DodgeHandler : public DamageHandler
{
	int dodgeChance_;

public:
	explicit DodgeHandler(int chance);

protected:
	void process(DamageRequest &req) override;
};

class BlockHandler : public DamageHandler
{
	int blockValue_;

public:
	explicit BlockHandler(int block);

protected:
	void process(DamageRequest &req) override;
};

class ArmorHandler : public DamageHandler
{
	int reductionPct_;

public:
	explicit ArmorHandler(int reductionPct);

protected:
	void process(DamageRequest &req) override;
};

class ShieldSpellHandler : public DamageHandler
{
	int barrier_;

public:
	explicit ShieldSpellHandler(int barrier);

protected:
	void process(DamageRequest &req) override;
};

class HealthHandler : public DamageHandler
{
	int &hp_;

public:
	explicit HealthHandler(int &hp);

protected:
	void process(DamageRequest &req) override;
};

//  Builder
std::unique_ptr<DamageHandler> buildDefenseChain(
	int dodgeChance,
	int blockValue,
	int armorPct,
	int spellBarrier,
	int &heroHp);
