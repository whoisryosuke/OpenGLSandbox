#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

class UI {
private:

public:
    ImGuiContext* context;
    ImFont* font1;

	UI(GLFWwindow* window, const char* glsl_version) {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        context = ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);


        // Load fonts
        float size_pixels = 15.0;
        font1 = io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Regular.ttf", size_pixels);
	}

    void render(ImVec4 clear_color) {

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        bool show_another_window = false;

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            // Get window width
            const ImGuiStyle& style = ImGui::GetStyle();
            const ImVec2 p = ImVec2(24.0f, 24.0f);

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, p);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(56.0 / 255.0, 54.0 / 255.0, 50.0 / 255.0, 1.0f));
            
            ImGui::Begin("##1", &show_another_window, ImGuiWindowFlags_NoTitleBar);

            // Compute button size
            float margin = (style.WindowPadding.x) * 2.0f;
            float w = ImGui::GetWindowWidth() - margin;
            float h = 35.0f;

            ImGui::CenteredText("Do you want to save the changes", ImVec2(w, 20));
            ImGui::CenteredText("made to the document ?", ImVec2(w, 20));
            ImGui::CenteredText("\nYou can revert to undo the changes\n \n", ImVec2(w, 40));

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(199.0 / 255.0, 195.0 / 255.0, 190.0 / 255.0, 1.0f));

            ImGui::Button("Save", ImVec2(w, h));
            ImGui::PopStyleColor();

            const ImVec2 spc = ImVec2(7.0f, 16.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, spc);

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(116.0 / 255.0, 114.0 / 255.0, 109.0 / 255.0, 1.0f));
            ImGui::Button("Revert Changes", ImVec2(w, h));
            ImGui::Button("Cancel", ImVec2(w, h));
            ImGui::PopStyleColor();

            ImGui::PopStyleVar(1);

            ImGui::End();
            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor();
        }

        // Render Visuals!

        // Rendering
        ImGui::Render();
    }

    void destroy() {
        // Terminate lifecycle
        // Clean up memory and resources
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
};