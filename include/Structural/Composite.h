#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

class SceneNode
{
public:
	virtual void render(int depth = 0) const = 0;
	virtual ~SceneNode() = default;

protected:
	static std::string indent(int d);
};

// Leaf
class GameObjectNode : public SceneNode
{
	std::string name_;
	std::string type_;

public:
	GameObjectNode(std::string name, std::string type);

	void render(int depth = 0) const override;
};

// Composite
class SceneGroup : public SceneNode
{
	std::string name_;
	std::vector<std::shared_ptr<SceneNode>> children_;

public:
	explicit SceneGroup(std::string name);

	void add(std::shared_ptr<SceneNode> node);

	void render(int depth = 0) const override;
};
