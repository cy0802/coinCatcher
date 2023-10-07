#include "../fallingItem/fallingItem.h"
#include "../catcher/catcher.h"
#include <vector>
#include <iostream>

class Game {
public:
	std::vector<FallingItem> goodItems;
	std::vector<FallingItem> badItems;
	Catcher catcher;
	Game(); // create window, initialize fallingItems and catcher
	void updateFallingItems(std::vector<FallingItem>&); // update fallingItems, catcher, and score
	void generateFallingItems(); // generate 3 fallingItems one time
	void print(std::vector<FallingItem>);
	int getScore();
	int gameTime = 0;
	bool gameover = false;
private:
	int score = 0;
};