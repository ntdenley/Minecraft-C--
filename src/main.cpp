#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800; // Screen width
const unsigned int SCR_HEIGHT = 600; // Screen height
bool wireframe = false;

// Enable wireframe mode
//#define DRAW_WIREFRAME

// Vertex shader code
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// Fragment shader code
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main() {

    // Initialize the GLFW window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Using OpenGL Major Version 3..
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // .. + Minor Version 3 = 3.3

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

    // Vertex Shader
    //===================================================================================

    // Create the vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); // Stores the ID of the shader object

    // Attach the shader source code to the shader object and compile the shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Create a success flag and info log buffer to check if the shader compilation was successful
    int  success; // Non-zero value indicates success
    char infoLog[512]; // Error info buffer

    // Get compilation status
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // Fill the info log buffer
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl; // Print the error message
    }

    // Fragment Shader
    //===================================================================================

    // Create the fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Stores the ID of the shader object

    // Attach the shader source code to the shader object and compile the shader
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Get compilation status (reuses the success and infoLog variables)
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog); // Fill the info log buffer
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl; // Print the error message
    }

    // Shader Program
    //===================================================================================
    
    // Create the shader program
    unsigned int shaderProgram = glCreateProgram();

    // Attach the vertex and fragment shaders to the shader program and link the program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Get linking status (reuses the success and infoLog variables)
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog); // Fill the info log buffer
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl; // Print the error message
    }

    // Delete the shader objects after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); 

    // Set the viewport size
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Set the callback function for when the window is resized
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

    // Vertex data
    //===================================================================================

    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // 0-indexed
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  

    // Get the vertex buffer object ID
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind the vertex array object
    glBindVertexArray(VAO);

    // Bind the vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind the element buffer object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Tell OpenGL how to interpret the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind the vertex array object
    glBindVertexArray(0);

    #ifdef DRAW_WIREFRAME
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    #endif

    // Render loop
    while(!glfwWindowShouldClose(window))
    {   
        // Process input
        processInput(window);

        // Render commands will go here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the rectangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); 

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwPollEvents();  
        glfwSwapBuffers(window);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}

bool mKeyReleased = true;

// Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && mKeyReleased)
    {
        wireframe = !wireframe;
        if(wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        mKeyReleased = false;
    }
    else if(glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE)
    {
        mKeyReleased = true;
    }
}

// Callback function for when the window is resized: adjust the viewport size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  