#include "Structural/Flyweight.h"

// ParticleType
ParticleType::ParticleType(std::string texture,
			   std::string color,
			   float size)
    : texture_(std::move(texture)),
      color_(std::move(color)),
      size_(size)
{
}

void ParticleType::render(float x, float y, float vx, float vy) const
{
	std::cout << "  [Particle] tex=" << texture_
		  << " col=" << color_
		  << " size=" << size_
		  << " | pos=(" << x << "," << y << ")"
		  << " vel=(" << vx << "," << vy << ")\n";
}

const std::string &ParticleType::texture() const
{
	return texture_;
}

// ParticleFactory
std::shared_ptr<ParticleType> ParticleFactory::get(
    const std::string &texture,
    const std::string &color,
    float size)
{
	std::string key = texture + "_" + color;

	auto it = pool_.find(key);
	if (it == pool_.end())
	{
		std::cout << "  [FlyweightFactory] Creating new ParticleType: "
			  << key << "\n";

		pool_[key] = std::make_shared<ParticleType>(texture, color, size);
	}

	return pool_[key];
}

int ParticleFactory::poolSize() const
{
	return static_cast<int>(pool_.size());
}

// Particle
void Particle::render() const
{
	type->render(x, y, vx, vy);
}
