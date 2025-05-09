#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ManageShader.h"
#include "BlockPallete.h"
#include "BufferObjects.h"
#include "Camera.h"

#include "ChunkManager.h"
#include "World.h"

// Global var
unsigned int SCR_WIDTH = 1400;
unsigned int SCR_HEIGHT = 1400;
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//Resize Window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}  
void updateDTime() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

int main()
{
    initializeNoiseObject();
	// Initiallize glfw, and hint constants
	if (!glfwInit()) 
	{
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create window object
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Luv you diddy", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// Initiallize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}   

	// Shader setup
	Shader shaderProgram("shaderSource\\standard.vert", "shaderSource\\standard.frag");
	glEnable(GL_DEPTH_TEST);  // Allow opengl to ultilize z-buffer to order draw triangles call
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE); // Anti aliasing

	// Create object and load in image
	// ------------------------------------------------------------------------

    // float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f}; // When using GL_CLAMP_TO_BORDER
    // glTexParameterfv(type, GL_TEXTURE_BORDER_COLOR, flatColor);
    Texture planks_oak("textures/planks_oak.png", GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	planks_oak.texUnit(shaderProgram, "tex0", 0);

    // Player Camera Object
    Camera player(glm::vec3(8.0f,8.0f, 20.0f));
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

    // Chunking system
    // ------------------------------------------------------------------------
    World world;
    world.loadPerimeterRadius(6);



    double lastTime = 0.0;  // Time of the last frame
    int frameCount = 0;     // Frame counter
	while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime(); // Get current time
        frameCount++; // Increment frame counter
    
        // Calculate FPS every second
        if (currentTime - lastTime >= 1.0) { // If 1 second has passed
            std::cout << "FPS: " << frameCount << std::endl; // Print the FPS
            frameCount = 0; // Reset frame counter
            lastTime = currentTime; // Reset the last time
            // world.loadRenderDistance(player);
        }

        updateDTime();
        player.processInput(window, deltaTime);

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        world.render(shaderProgram, player);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clean up
    planks_oak.Delete();

	shaderProgram.Delete();

	glfwTerminate();
	return 0;
}