#include "Structural/Adapter.h"

// LegacyXmlSave
void LegacyXmlSave::writeXml(const std::string &tag, const std::string &value)
{
	std::cout << "  [LegacyXmlSave] Writing XML: <" << tag << ">" << value
		  << "</" << tag << ">\n";
}

std::string LegacyXmlSave::readXml(const std::string &tag)
{
	return "<" + tag + ">SavedData</" + tag + ">";
}

// XmlToJsonSaveAdapter
void XmlToJsonSaveAdapter::save(const std::string &playerName, int score)
{
	std::cout << "  [Adapter] Converting JSON save -> XML format\n";

	legacy_.writeXml("player", playerName);
	legacy_.writeXml("score", std::to_string(score));
}

std::string XmlToJsonSaveAdapter::load(const std::string &playerName)
{
	std::cout << "  [Adapter] Converting XML load -> JSON format\n";

	std::string raw = legacy_.readXml("player_" + playerName);

	return "{ \"player\": \"" + playerName + "\", \"raw\": \"" + raw + "\" }";
}
