#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "Shader.h"
#include "stb_image.h"
#include "UI.cpp"

#include <iostream>
#include <filesystem>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{

    std::string str = "Hello, world!";
    char* shaderTextBuffer = const_cast<char*>(str.c_str());

    //static char shaderTextBuffer[128] = "Hello, world!";
    //static char defaultText[128] = "Hello, world!";
    //static char *shaderTextBuffer;
    //shaderTextBuffer = &defaultText;

    // Initialize GLFW (our library for OpenGL)
    glfwInit();
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    // Set version of OpenGL (3.3 in this case)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Set profile - Core in this case
    // Selected from here: https://glad.dav1d.de/
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create Window using GLFW
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // Set a callback for when user resizes window
    // we resize our GL window to same size
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load GLAD pointers before doing any GL work
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    UI* ui = new UI(window, glsl_version);


    // Setup shaders (vertex and fragment)
    Shader cubeShader("Default.vs", "Default.ps", shaderTextBuffer);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // Load texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("assets/screenshot1.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Loaded texture" << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // Assign texture to uniform
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    cubeShader.use();
    cubeShader.setInt("texture1", 0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Our app state (used for Imgui)
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



    // ♻ RENDER LOOP
    while (!glfwWindowShouldClose(window))
    {
        // Input handling (e.g. ESC to close window)
        processInput(window);
        //glfwPollEvents();

        ui->render(clear_color);

        // Draw the clear (this clears previous render - in this case with a specific color)
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);


        // Draw vertex buffers
        cubeShader.use();
        // We can send uniform values (or vertex attributes) to the vertex shader
        // using the helper methods on Shader class
        // You can name them anything but the `_Name` convention comes from Unity
        cubeShader.setFloat("_Opacity", 0.2f);
        cubeShader.setFloat("_Offset", 0.5f);
        cubeShader.setVector3("_Color", 0.5, 0.0f, 0.5f);
        // Attach texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 
        // Render Imgui buffers
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // GLFW: Swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ui->destroy();

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(cubeShader.ID);

    // GLFW termination cycle - clears memory and resources
    glfwTerminate();
    return 0;
}


/// <summary>
/// Handles input inside GL window using GLFW
/// </summary>
/// <param name="window">GLFW Window object</param>
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

/// <summary>
/// GLFW: Callback when window size changes (by OS or user resize)
/// </summary>
/// <param name="window"></param>
/// <param name="width"></param>
/// <param name="height"></param>
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Set viewport to new size
    // Note that width/height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}