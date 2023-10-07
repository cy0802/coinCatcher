#include <cstdlib> 
#include <ctime> 
#include "./fallingItem.h"

int FallingItem::speed = 25; // frame updating speed
int FallingItem::goodRatio = 2;
float FallingItem::goodVertices[150 * 12] = {0.0f};
float FallingItem::badVertices[150 * 12] = { 0.0f };
char* FallingItem::vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
char* FallingItem::goodShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.867f, 0.788f, 0.706f, 1.0f);\n"
"}\n\0";
char* FallingItem::badShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.757f, 0.486f, 0.455f, 1.0f);\n"
"}\n\0";

FallingItem::FallingItem() {
	y = 0.99f;
	x = (rand() % 20 - 10) / 10.0f;
	good = rand() % 10 > goodRatio ? true : false;
	calculateVertex();
}
void FallingItem::calculateVertex() {
	vertexData[0] = this->x - 0.01; 
	vertexData[1] = this->y + 0.01;
	vertexData[2] = 0.0f;
	vertexData[3] = this->x + 0.01;
	vertexData[4] = this->y + 0.01;
	vertexData[5] = 0.0f;
	vertexData[6] = this->x + 0.01;
	vertexData[7] = this->y - 0.01;
	vertexData[8] = 0.0f;
	vertexData[9] = this->x - 0.01;
	vertexData[10] = this->y - 0.01;
	vertexData[11] = 0.0f;
}
bool FallingItem::isGood() {
	return good;
}

void FallingItem::getPos(float& x, float& y) {
	x = this->x;
	y = this->y;
}
void FallingItem::setPos(float x, float y) {
	this->x = x;
	this->y = y;
	calculateVertex();
}
void FallingItem::setSpeed(float _speed) {
	FallingItem::speed = _speed;
}
