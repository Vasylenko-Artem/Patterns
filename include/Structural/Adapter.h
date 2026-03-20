#pragma once

#include <iostream>
#include <string>

// Target interface expected by the game engine
class ISaveSystem
{
public:
	virtual void save(const std::string &playerName, int score) = 0;
	virtual std::string load(const std::string &playerName) = 0;
	virtual ~ISaveSystem() = default;
};

// Adaptee — legacy XML save system we cannot modify
class LegacyXmlSave
{
public:
	void writeXml(const std::string &tag, const std::string &value);
	std::string readXml(const std::string &tag);
};

// Adapter — makes LegacyXmlSave look like ISaveSystem
class XmlToJsonSaveAdapter : public ISaveSystem
{
	LegacyXmlSave legacy_;

public:
	void save(const std::string &playerName, int score) override;
	std::string load(const std::string &playerName) override;
};
