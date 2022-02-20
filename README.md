# OpenGL Sandbox

Playground for experimenting with OpenGL (preferably on Windows). [Based on this OpenGL book / tutorial.](https://learnopengl.com/Getting-started/Creating-a-window)

## Getting Started

1. Clone repo.
2. Download compiled binaries and include files of GLFW for your version of Visual Studio (e.g. 2022): [http://www.glfw.org/download.html](http://www.glfw.org/download.html)
   1. You can also use CMake to manually compile the binaries yourself from source code.
3. Extract the binaries to a local folder, we’ll share this between projects: `C:\Development\C++\OpenGL\Libraries\lib-vc2022`
4. Open the solution and go to Project > Properties.
5. Under **Configuration Properties > VC++ Directories**, edit the **Library Directories** and add where you put the shared GLFW binaries: `C:\Development\C++\OpenGL\Libraries\lib-vc2022`
6. Try running the debugger!

## Examples

There's a bunch of examples that are inside the project source (e.g. `01-Window.cpp`). These are different progressions of OpenGL configuration - from initially creating a blank window - to rendering complex scenes.

To use an example:

1. Go to the Solution Explorer and exclude (don't delete) whatever file has the `main()` function inside
1. Go to **Show All Files** _(button on top of Solution Explorer)_ and select an example (one or multiple files depending).
1. Right click and select **Include in Project**.

## Initial Setup

<aside>
⚠️ This guide assumes you’re running Windows with Visual Studio 2022. If you want to use Mac or Linux, [check this guide](https://learnopengl.com/Getting-started/Creating-a-window) above for special instructions.
</aside>

1. Create a new C++ Project.
   1. Make sure it’s an “Empty Project” template (no Universal Windows or WinRT stuff).
2. Download compiled binaries and include files of GLFW for your version of Visual Studio (e.g. 2022): [http://www.glfw.org/download.html](http://www.glfw.org/download.html)
   1. You can also use CMake to manually compile the binaries yourself from source code.
3. Extract the binaries to a local folder, we’ll share this between projects: `C:\Development\C++\OpenGL\Libraries\lib-vc2022`
4. Extract the `includes` folder from the GLFW zip into your project: `C:\Development\YourOpenGLProject\include\GLFW`
5. Go to the GLAD website and fill out this form to download the correct version: [http://glad.dav1d.de/](http://glad.dav1d.de/)
   1. Language is **C++**
   2. **GL version** is set to **3.3**
   3. Set **Profile** to **Core**
   4. Make sure “**Generate a Loader**” is **checked**.
   5. Click Generate and download the `glad.zip`
6. Extract the `includes` folder from the GLAD zip into your project - there should be 2 folders (`glad` and `KHR`): `C:\Development\YourOpenGLProject\include\KHR`
7. Now lets link the libraries to the project. Open the project in Visual Studio.
8. Select your project, then go to **Project > Properties**.
9. Under **Configuration Properties > VC++ Directories**, let’s change 2 things:
   1. Edit the **Include Directories** and add your project’s include folder `C:\Development\YourOpenGLProject\include\`
   2. Edit the **Library Directories** and add where you put the shared GLFW binaries: `C:\Development\C++\OpenGL\Libraries\lib-vc2022`
10. Then go to **Linker > Input** in the sidebar, and add the GLFW library - as well as the OpenGL library (built into Windows usually) to your **Additional Dependencies**:

    ```bash
    glfw3.lib
    opengl32.lib
    ```

11. Create a new C++ file and name it anything - like `App.cpp` - and add the following to create a new OpenGL window:

    ```cpp
    #include <glad/glad.h>
    #include <GLFW/glfw3.h>

    #include <iostream>

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void processInput(GLFWwindow* window);

    // settings
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    int main()
    {
        // Initialize GLFW (our library for OpenGL)
        glfwInit();
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

        // ♻ RENDER LOOP
        while (!glfwWindowShouldClose(window))
        {
            // Input handling (e.g. ESC to close window)
            processInput(window);

            // Render Visuals!
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // GLFW: Swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

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
    ```

12. **Start the debugger** to open a new blank window that should look colored teal.
