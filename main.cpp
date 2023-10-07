#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "./game/game.h"
#define STB_IMAGE_IMPLEMENTATION
#include "./Includes/stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void init();
unsigned int buildShaders(char*, char*);
// settings
const unsigned int SCR_WIDTH = 900;
const unsigned int SCR_HEIGHT = 900;
GLFWwindow* window;
Game coinCatcher;
char* textureVertexShader = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"void main(){\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"    TexCoord = aTexCoord;\n"
"}";
char* textureFragShader = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture;\n"
"void main(){\n"
"    FragColor = texture(ourTexture, TexCoord);\n"
"}\n";

int main() {
    init();
    glDebugMessageCallback(MessageCallback, 0);
    unsigned int goodShaderProgram = buildShaders(FallingItem::vertexShaderSource, FallingItem::goodShaderSource);
    unsigned int badShaderProgram = buildShaders(FallingItem::vertexShaderSource, FallingItem::badShaderSource);
    coinCatcher = Game();
    
    GLuint VBO_GOODITEM, VBO_BADITEM, VBO_CATCHER, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_GOODITEM);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_GOODITEM);
    /*glBufferData(GL_ARRAY_BUFFER, sizeof(FallingItem::vertices), 
        FallingItem::vertices, GL_STREAM_DRAW);*/
    glBufferStorage(GL_ARRAY_BUFFER, sizeof(FallingItem::goodVertices), 
        nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
    void* goodVerticesMapping = glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(FallingItem::goodVertices), 
        GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glGenBuffers(1, &VBO_BADITEM);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_BADITEM);
    /*glBufferData(GL_ARRAY_BUFFER, sizeof(coinCatcher.catcher.boxVertexData),
        coinCatcher.catcher.boxVertexData, GL_STREAM_DRAW);*/
    glBufferStorage(GL_ARRAY_BUFFER, sizeof(FallingItem::badVertices),
        nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
    void* badVerticesMapping = glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(FallingItem::badVertices),
        GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glGenBuffers(1, &VBO_CATCHER);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_CATCHER);
    /*glBufferData(GL_ARRAY_BUFFER, sizeof(coinCatcher.catcher.boxVertexData),
        coinCatcher.catcher.boxVertexData, GL_STREAM_DRAW);*/
    glBufferStorage(GL_ARRAY_BUFFER, sizeof(coinCatcher.catcher.boxVertexData), 
        nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
    void* boxMapping = glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(coinCatcher.catcher.boxVertexData), 
        GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);

    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window)) {
        if (coinCatcher.gameover) break;
        processInput(window);
        
        // updata falling items' position
        if (coinCatcher.gameTime % FallingItem::speed == 0) {
            coinCatcher.generateFallingItems();
            coinCatcher.updateFallingItems(coinCatcher.goodItems);
            coinCatcher.updateFallingItems(coinCatcher.badItems);
        }
        if (coinCatcher.gameTime % 10 == 0) {
            std::cout << "current gametime: " << coinCatcher.gameTime << std::endl;
            std::cout << "current speed: " << FallingItem::speed << std::endl;
            std::cout << "score: " << coinCatcher.getScore() << std::endl << std::endl;
        }
        //coinCatcher.print();
        memcpy(goodVerticesMapping, FallingItem::goodVertices, 
            sizeof(float) * 12 * coinCatcher.goodItems.size());

        memcpy(badVerticesMapping, FallingItem::badVertices,
            sizeof(float) * 12 * coinCatcher.badItems.size());
        
        // update catcher's position
        memcpy(boxMapping, coinCatcher.catcher.boxVertexData, 
            sizeof(coinCatcher.catcher.boxVertexData));

        // draw background
        glClearColor(0.164706, 0.239216, 0.270588, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw catcher
        glUseProgram(goodShaderProgram);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_CATCHER);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);
        glLineWidth(4.0f);
        glDrawArrays(GL_LINE_STRIP, 0, 4); 

        //draw good items
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_GOODITEM);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        for (int i = 0; i < coinCatcher.goodItems.size(); i++) {
            //for (int j = 0; j < 12; j++) std::cout << floatData[12 * i + j] << " ";
            //std::cout << std::endl;
            glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
        }

        //draw bad items
        glUseProgram(badShaderProgram);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_BADITEM);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        //std::cout << "Vertices:\n";
        //float* floatData = static_cast<float*>(verticesMapping);
        for (int i = 0; i < coinCatcher.badItems.size(); i++) {
            //for (int j = 0; j < 12; j++) std::cout << floatData[12 * i + j] << " ";
            //std::cout << std::endl;
            glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
        }
        //std::cout << std::endl;
        coinCatcher.gameTime++;
        if (coinCatcher.gameTime % 200 == 0 && FallingItem::speed > 15)
            FallingItem::speed = FallingItem::speed - 1;
        if (coinCatcher.gameTime % 750 == 0 && FallingItem::goodRatio < 8)
            FallingItem::goodRatio++;

        char newtitle[100] = "Coin Catcher / score: ";
        std::string tmp = std::to_string(coinCatcher.getScore());
        strcat_s(newtitle, tmp.c_str());
        glfwSetWindowTitle(window, newtitle);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // game already over
    // display a gameover picture
    unsigned int textureShaderProgram = buildShaders(textureVertexShader, textureFragShader);
    //float vertices[] = {
    //    // position           texture cood
    //    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,  // ¥ª¤U¨¤
    //     1.0f, -1.0f, 0.0f,   1.0f, 0.0f,  // ¥k¤U¨¤
    //     1.0f,  1.0f, 0.0f,   1.0f, 1.0f,  // ¥k¤W¨¤
    //    -1.0f,  1.0f, 0.0f,   0.0f, 1.0f   // ¥ª¤W¨¤
    //};
    float vertices[] = {
        // position           texture coord
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,  // ¥ª¤U¨¤
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f,  // ¥k¤U¨¤
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f,  // ¥k¤W¨¤

         -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,  // ¥ª¤U¨¤
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f,  // ¥k¤W¨¤
         -1.0f,  1.0f, 0.0f,   0.0f, 1.0f   // ¥ª¤W¨¤
    };
    unsigned int VBO, VAO_TEXTURE;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO_TEXTURE);
    glBindVertexArray(VAO_TEXTURE);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("..\\gameover.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "successfully load texture\n";
        std::cout << "width: " << width << " / height: " << height << " / channel: " << nrChannels << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
        std::cout << stbi_failure_reason() << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(textureShaderProgram, "ourTexture"), 0);
    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUseProgram(textureShaderProgram);
        glBindVertexArray(VAO_TEXTURE);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO_TEXTURE);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO_GOODITEM);
    glDeleteBuffers(1, &VBO_BADITEM);
    glDeleteBuffers(1, &VBO_CATCHER);
    glDeleteProgram(goodShaderProgram);
    glDeleteProgram(badShaderProgram);
}

void init() {
    // glfw: initialize and configure
    // ------------------------------
    srand(time(NULL));
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Coin Catcher", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, keyCallback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
}

unsigned int buildShaders(char* vertexShaderSource, char* fragmentShaderSource) {
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    std::cout << "keyCallback()\n";
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_LEFT) 
            coinCatcher.catcher.setX(coinCatcher.catcher.getX() - Catcher::speed);
        if (key == GLFW_KEY_RIGHT)
            coinCatcher.catcher.setX(coinCatcher.catcher.getX() + Catcher::speed);
    }
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, 
        GLenum severity, GLsizei length, const GLchar* message, const void* userParam){
    std::cerr << "GL CALLBACK: " << message << std::endl;
}