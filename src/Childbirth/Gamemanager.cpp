#include "Childbirth/GameManager.h"

using namespace std;

GameManager &GameManager::getInstance()
{
	static GameManager instance;
	return instance;
}

void GameManager::startGame(const string &level)
{
	currentLevel_ = level;
	score_ = 0;
	running_ = true;
	cout << "[GameManager] The game has started. Level: " << currentLevel_ << endl;
}

void GameManager::addScore(int points)
{
	score_ += points;
	cout << "[GameManager] " << (points >= 0 ? "+" : "")
		 << points << " points Score: " << score_ << endl;
}

void GameManager::endGame()
{
	running_ = false;
	cout << "[GameManager] The game is over. Final score: " << score_ << endl;
}
