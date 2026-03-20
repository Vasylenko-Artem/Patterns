#include "Structural/Proxy.h"

// GameDatabase
std::string GameDatabase::query(const std::string &sql)
{
	return "[DB] Result for: " + sql;
}

void GameDatabase::execute(const std::string &sql)
{
	std::cout << "  [DB] Executing: " << sql << "\n";
}

// DatabaseProxy
DatabaseProxy::DatabaseProxy(std::string role)
    : role_(std::move(role))
{
}

std::string DatabaseProxy::query(const std::string &sql)
{
	std::cout << "  [Proxy] LOG query | role=" << role_ << " | sql=" << sql << "\n";
	return real_.query(sql);
}

void DatabaseProxy::execute(const std::string &sql)
{
	if (role_ != "admin")
	{
		std::cout << "  [Proxy] ACCESS DENIED for role=" << role_ << "\n";
		return;
	}

	std::cout << "  [Proxy] LOG execute | role=" << role_ << "\n";
	real_.execute(sql);
}
