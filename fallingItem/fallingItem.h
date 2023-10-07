class FallingItem {
public:
	static int speed;
	static int goodRatio; // goodRatio out of 10
	static float goodVertices[150 * 12];
	static float badVertices[150 * 12];
	static const int maxItemNum = 150;
	float vertexData[12];
	bool toBeDeleted = false;
	FallingItem();
	bool isGood();
	void getPos(float &x, float &y);
	void setPos(float x, float y);
	static void setSpeed(float _speed);
	void calculateVertex();
	
	// shader source code
	static char* vertexShaderSource;
	static char* goodShaderSource;
	static char* badShaderSource;
private:
	float x;
	float y;
	bool good;
};