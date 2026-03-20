#include "Structural/Composite.h"

// SceneNode
std::string SceneNode::indent(int d)
{
	return std::string(d * 2, ' ');
}

// GameObjectNode
GameObjectNode::GameObjectNode(std::string name, std::string type)
    : name_(std::move(name)), type_(std::move(type))
{
}

void GameObjectNode::render(int depth) const
{
	std::cout << indent(depth)
		  << "[" << type_ << "] " << name_ << "\n";
}

// SceneGroup
SceneGroup::SceneGroup(std::string name)
    : name_(std::move(name))
{
}

void SceneGroup::add(std::shared_ptr<SceneNode> node)
{
	children_.push_back(std::move(node));
}

void SceneGroup::render(int depth) const
{
	std::cout << indent(depth)
		  << "[Group] Group: " << name_ << "\n";

	for (const auto &child : children_)
		child->render(depth + 1);
}
