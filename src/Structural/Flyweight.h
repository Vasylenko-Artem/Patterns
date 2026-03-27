#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

// Flyweight — shared intrinsic state
class ParticleType
{
	std::string texture_;
	std::string color_;
	float size_;

public:
	ParticleType(std::string texture, std::string color, float size);

	void render(float x, float y, float vx, float vy) const;

	const std::string &texture() const;
};

// Flyweight Factory
class ParticleFactory
{
	std::unordered_map<std::string, std::shared_ptr<ParticleType>> pool_;

public:
	std::shared_ptr<ParticleType> get(const std::string &texture,
					  const std::string &color,
					  float size);

	int poolSize() const;
};

// Context — holds extrinsic state + reference to flyweight
struct Particle
{
	std::shared_ptr<ParticleType> type; // shared (flyweight)
	float x, y, vx, vy;

	void render() const;
};
