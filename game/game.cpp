#include <Windows.h>
#include <cstdlib> 
#include <ctime> 
#include "./game.h"
Game::Game() {
	catcher = Catcher();
	score = gameTime = 0;
	generateFallingItems();
	updateFallingItems(goodItems);
	updateFallingItems(badItems);
}
int Game::getScore() {
	return score;
}
void Game::generateFallingItems() {
	if (goodItems.size() + badItems.size() >= FallingItem::maxItemNum) return;
	for (int i = 0; i < 3; i++) {
		FallingItem tmp = FallingItem();
		if (tmp.isGood()) goodItems.push_back(tmp);
		else badItems.push_back(tmp);
	}
}
void Game::updateFallingItems(std::vector<FallingItem> &fallingItems) {
	if (fallingItems.empty()) return;
	//std::vector<std::vector<FallingItem>::iterator> toBeDeleted;
	for (int i = 0; i < fallingItems.size(); i++) {
		float x, y;
		fallingItems[i].getPos(x, y);
		// items fall
		fallingItems[i].setPos(x, y - 0.05f);
		if (y < -1.0f) { // touch the floor
			//toBeDeleted.push_back(fallingItems.begin() + i);
			fallingItems[i].toBeDeleted = true;
		} else if (abs(x - catcher.getX()) < 0.05f && y < -0.85f) { // collide with catcher
			if (fallingItems[i].isGood()) {
				score += 10;
				fallingItems[i].toBeDeleted = true;
				//std::cout << "catch\n";
			} else {
				gameover = true;
			}
		}
	}
	//std::cout << "deleted items: ";
	for (auto it = fallingItems.begin(); it < fallingItems.end(); it++) {
		float x, y;
		if (it->toBeDeleted == true) {
			// it->getPos(x, y);
			// std::cout << x << " " << y  << " " << it - fallingItems.begin() << std::endl;
			it = fallingItems.erase(it);
		}
	}
	//std::cout << std::endl;
	if (fallingItems.empty()) return;

	// update into vertices array
	if (fallingItems[0].isGood()) {
		memset(FallingItem::goodVertices, 0, sizeof(FallingItem::goodVertices));
		for (int i = 0; i < fallingItems.size(); i++) {
			memcpy(FallingItem::goodVertices + i * 12, fallingItems[i].vertexData, 12 * sizeof(float));
		}
	} else {
		memset(FallingItem::badVertices, 0, sizeof(FallingItem::badVertices));
		for (int i = 0; i < fallingItems.size(); i++) {
			memcpy(FallingItem::badVertices + i * 12, fallingItems[i].vertexData, 12 * sizeof(float));
		}
	}
}

void Game::print(std::vector<FallingItem> fallingItems) {
	std::cout << "==================================================\n";
	std::cout << "time: " << gameTime << std::endl << std::endl;
	if (fallingItems.empty()) return;
	if (fallingItems[0].isGood()) std::cout << "good items: \n";
	else std::cout << "bad items: \n";
	for (int i = 0; i < fallingItems.size(); i++) {
		float x, y;
		fallingItems[i].getPos(x, y);
		std::cout << "(" << x << ", " << y << ")\t";
		for (int j = 0; j < 4; j++) {
			std::cout << fallingItems[i].vertexData[3 * j] << " "
				<< fallingItems[i].vertexData[3 * j + 1] << " "
				<< fallingItems[i].vertexData[3 * j + 2] << " / ";
		}
		std::cout << std::endl;
	}
	/*std::cout << "vertices:\n";
	for (int i = 0; i < fallingItems.size(); i++) {
		std::cout << FallingItem::vertices[3 * i] << " "
			<< FallingItem::vertices[3 * i + 1] << " "
			<< FallingItem::vertices[3 * i + 2] << " / ";
	}
	std::cout << std::endl;*/
	catcher.print();
	std::cout << std::endl;
}