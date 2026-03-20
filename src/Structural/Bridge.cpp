#include "Structural/Bridge.h"

// OpenGL
void OpenGLRenderer::drawTexture(const std::string &tex, int x, int y)
{
	std::cout << "    [OpenGL] glDrawTexture(\"" << tex << "\", " << x << ", " << y << ")\n";
}

void OpenGLRenderer::drawMesh(const std::string &mesh, float scale)
{
	std::cout << "    [OpenGL] glDrawMesh(\"" << mesh << "\", scale=" << scale << ")\n";
}

std::string OpenGLRenderer::apiName() const
{
	return "OpenGL";
}

// DirectX
void DirectXRenderer::drawTexture(const std::string &tex, int x, int y)
{
	std::cout << "    [DirectX] DrawSprite(\"" << tex << "\", {" << x << ", " << y << "})\n";
}

void DirectXRenderer::drawMesh(const std::string &mesh, float scale)
{
	std::cout << "    [DirectX] DrawMesh(\"" << mesh << "\", scale=" << scale << ")\n";
}

std::string DirectXRenderer::apiName() const
{
	return "DirectX";
}

// Drawable
Drawable::Drawable(std::shared_ptr<IRenderer> r)
    : renderer_(std::move(r))
{
}

// Sprite
Sprite::Sprite(std::shared_ptr<IRenderer> r, std::string tex, int x, int y)
    : Drawable(std::move(r)),
      texture_(std::move(tex)),
      x_(x),
      y_(y)
{
}

void Sprite::draw()
{
	std::cout << "  [Sprite] Drawing via " << renderer_->apiName() << "\n";
	renderer_->drawTexture(texture_, x_, y_);
}

// Mesh3D
Mesh3D::Mesh3D(std::shared_ptr<IRenderer> r, std::string mesh, float scale)
    : Drawable(std::move(r)),
      mesh_(std::move(mesh)),
      scale_(scale)
{
}

void Mesh3D::draw()
{
	std::cout << "  [Mesh3D] Drawing via " << renderer_->apiName() << "\n";
	renderer_->drawMesh(mesh_, scale_);
}
