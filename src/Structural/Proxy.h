#pragma once

#include <iostream>
#include <string>

class IGameDatabase
{
public:
	virtual std::string query(const std::string &sql) = 0;
	virtual void execute(const std::string &sql) = 0;
	virtual ~IGameDatabase() = default;
};

// Real subject
class GameDatabase : public IGameDatabase
{
public:
	std::string query(const std::string &sql) override;
	void execute(const std::string &sql) override;
};

// Proxy — logging + access control
class DatabaseProxy : public IGameDatabase
{
	GameDatabase real_;
	std::string role_; // "admin" or "guest"

public:
	explicit DatabaseProxy(std::string role);

	std::string query(const std::string &sql) override;
	void execute(const std::string &sql) override;
};
