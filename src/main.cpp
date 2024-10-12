#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800; // Screen width
const unsigned int SCR_HEIGHT = 600; // Screen height

int main() {

    // Initialize the GLFW window
    glfwInit();

    // Using OpenGL Major Version 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // Minor Version 3 -> 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Use the core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create the window, 
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Tell GLFW to make the context of the window the main context on the current thread
    glfwMakeContextCurrent(window);

    // Initialize GLAD. GLAD manages function pointers for OpenGL to ensure that the correct function pointers are loaded
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set the viewport size
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Set the callback function for when the window is resized
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

    // Render loop
    while(!glfwWindowShouldClose(window))
    {   
        // Process input
        processInput(window);

        // Render commands will go here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Check and call events and swap the buffers
        glfwPollEvents();  
        glfwSwapBuffers(window);
    }

    // Terminate GLFW
    glfwTerminate();
    return 0;
}

// Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Callback function for when the window is resized: adjust the viewport size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  