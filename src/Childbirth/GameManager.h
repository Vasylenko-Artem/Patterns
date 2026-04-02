#pragma once
#include <iostream>
#include <string>

class GameManager
{
public:
	static GameManager &getInstance();

	GameManager(const GameManager &) = delete;
	GameManager &operator=(const GameManager &) = delete;

	void startGame(const std::string &level);
	void addScore(int points);
	void endGame();

	int getScore() const { return score_; }
	std::string getLevel() const { return currentLevel_; }
	bool isRunning() const { return running_; }

private:
	GameManager() : score_(0), running_(false), currentLevel_("none") {}

	int score_;
	bool running_;
	std::string currentLevel_;
};
