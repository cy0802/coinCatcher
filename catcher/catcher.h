class Catcher {
public:
	Catcher();
	void setX(float x);
	float getX();
	void calculateVertex();
	float boxVertexData[12];
	void print();
	static float speed;
private:
	float x; 
	float y;
};