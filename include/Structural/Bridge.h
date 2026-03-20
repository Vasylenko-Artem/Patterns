#pragma once

#include <iostream>
#include <memory>
#include <string>

// Implementation interface — render API
class IRenderer
{
public:
	virtual void drawTexture(const std::string &tex, int x, int y) = 0;
	virtual void drawMesh(const std::string &mesh, float scale) = 0;
	virtual std::string apiName() const = 0;
	virtual ~IRenderer() = default;
};

// Concrete implementations
class OpenGLRenderer : public IRenderer
{
public:
	void drawTexture(const std::string &tex, int x, int y) override;
	void drawMesh(const std::string &mesh, float scale) override;
	std::string apiName() const override;
};

class DirectXRenderer : public IRenderer
{
public:
	void drawTexture(const std::string &tex, int x, int y) override;
	void drawMesh(const std::string &mesh, float scale) override;
	std::string apiName() const override;
};

// Abstraction — game drawable
class Drawable
{
protected:
	std::shared_ptr<IRenderer> renderer_;

public:
	explicit Drawable(std::shared_ptr<IRenderer> r);
	virtual void draw() = 0;
	virtual ~Drawable() = default;
};

// Refined Abstraction — 2D Sprite
class Sprite : public Drawable
{
	std::string texture_;
	int x_, y_;

public:
	Sprite(std::shared_ptr<IRenderer> r, std::string tex, int x, int y);

	void draw() override;
};

// Refined Abstraction — 3D Mesh
class Mesh3D : public Drawable
{
	std::string mesh_;
	float scale_;

public:
	Mesh3D(std::shared_ptr<IRenderer> r, std::string mesh, float scale);

	void draw() override;
};
