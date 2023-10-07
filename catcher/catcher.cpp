#include <cmath>
#include <cstring>
#include <iostream>
#include "./catcher.h"

float Catcher::speed = 0.02f;

Catcher::Catcher() {
	this->x = 0.0f;
	this->y = -0.95f;
	calculateVertex();
}
void Catcher::calculateVertex() {
	memset(boxVertexData, 0, sizeof(boxVertexData));
	boxVertexData[0] = x - 0.05f;
	boxVertexData[1] = y + 0.1f;
	boxVertexData[2] = 0.0f;
	boxVertexData[3] = x - 0.05f;
	boxVertexData[4] = y;
	boxVertexData[5] = 0.0f;
	boxVertexData[6] = x + 0.05f;
	boxVertexData[7] = y;
	boxVertexData[8] = 0.0f;
	boxVertexData[9] = x + 0.05f;
	boxVertexData[10] = y + 0.1f;
	boxVertexData[11] = 0.0f;

}
void Catcher::setX(float _x) {
	x = _x;
	calculateVertex();
}

float Catcher::getX() {
	return x;
}

void Catcher::print() {
	std::cout << "box vertex data" << std::endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << boxVertexData[j + i * 3] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}