#include <fstream>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "BSPViewer/GLFW.hpp"
#include "BSPViewer/IO.hpp"
#include "BSPViewer/ImGuiContext.hpp"
#include "BSPViewer/Window.hpp"
#include "BSPViewer/BSP/BSP.hpp"
#include "BSPViewer/BSP/Decoder.hpp"
#include "BSPViewer/Graphics/DepthStencilBuffer.hpp"
#include "BSPViewer/Graphics/FragmentShaderStage.hpp"
#include "BSPViewer/Graphics/FrameBuffer.hpp"
#include "BSPViewer/Graphics/OpenGLType.hpp"
#include "BSPViewer/Graphics/Shader.hpp"
#include "BSPViewer/Graphics/Texture2D.hpp"
#include "BSPViewer/Graphics/TextureFilter.hpp"
#include "BSPViewer/Graphics/TextureWrap.hpp"
#include "BSPViewer/Graphics/VertexArray.hpp"
#include "BSPViewer/Graphics/VertexBuffer.hpp"
#include "BSPViewer/Graphics/VertexBufferElement.hpp"
#include "BSPViewer/Graphics/VertexBufferLayout.hpp"
#include "BSPViewer/Graphics/VertexShaderStage.hpp"

int main() {
	GLFW glfw;

    glfw.setContextVersion(3, 3);
    glfw.setOpenGLProfile(GLFW_OPENGL_CORE_PROFILE);
    glfw.enableOpenGLForwardCompatibility();

    Window window("BSP Viewer", 800, 600);

    window.makeContextCurrent();

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		return -1;
	}

    glfw.setSwapInterval(1);

	IMGUI_CHECKVERSION();
    
    ImGuiContext imGuiContext;
    
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    Shader shader = Shader(
        VertexShaderStage(readFileToString("vertex_shader.glsl")),
        FragmentShaderStage(readFileToString("fragment_shader.glsl"))
    );

    FrameBuffer frameBuffer = FrameBuffer(
        std::move(Texture2D(200, 200, TextureFilter::Linear, TextureFilter::Linear, TextureWrap::ClampToEdge, TextureWrap::ClampToEdge)), 
        std::move(DepthStencilBuffer(200, 200))
    );

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    BSPDecoder decoder;

    // TODO: The file should be chosen by a user through some file dialog
    MemoryStream stream = MemoryStream::fromFile("boo.bsp");

    auto bsp = decoder.read<BSP>(stream);

    std::vector<VertexArray> vertexArrays;
    vertexArrays.reserve(bsp.modelParts.size());

    for (const auto& modelPart : bsp.modelParts) {
        VertexArray vertexArray;
        vertexArray.bind();

        std::vector<Vector3> vertices;

        vertices.reserve(modelPart.vertices.size());

        for (const auto& vertex : modelPart.vertices) {
            if (vertex.vertex) {
                Vector3 position = vertex.vertex.value();

                // TODO: Make a proper camera system and use it to move the object, instead of doing... this...
                position.y -= 1.0;
                position.z -= 1.0;

                vertices.push_back(position);
            }
        }

        VertexBufferLayout vertexBufferLayout { VertexBufferElement(OpenGLType::Float, 3) };
        VertexBuffer vertexBuffer(std::move(vertices), vertexBufferLayout);

        std::vector<uint32_t> indices;

        indices.reserve(modelPart.indices.size() * 3);

        for (const auto& index : modelPart.indices) {
            indices.push_back(index.index0);
            indices.push_back(index.index1);
            indices.push_back(index.index2);
        }

        IndexBuffer indexBuffer(std::move(indices));

        vertexArray.attachVertexBuffer(std::move(vertexBuffer));
        vertexArray.attachIndexBuffer(std::move(indexBuffer));

        vertexArrays.emplace_back(std::move(vertexArray));
    }

    // TODO: Replace with a custom implementation
    ImGui_ImplGlfw_InitForOpenGL(window.window.get(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

	while (window.isOpen()) {
		glfw.pollEvents();

        {
            shader.bind();
            frameBuffer.bind();

            glViewport(0, 0, 200, 200);

            glEnable(GL_DEPTH_TEST);

            glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            for (auto& vertexArray : vertexArrays) {
                vertexArray.bind();

                glDrawElements(GL_TRIANGLES, vertexArray.getAttachedIndexBuffer().value().getIndicesCount(), GL_UNSIGNED_INT, nullptr);
            }

            glDisable(GL_DEPTH_TEST);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, 800, 600);

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        // TODO: Replace with a custom implementation
		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();

        //application.draw();

        {
            if (ImGui::BeginMainMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    ImGui::EndMenu();
                }

                ImGui::EndMainMenuBar();
            }

            ImGui::Begin("Tree");

            ImGui::End();

            ImGui::Begin("Viewer");

            ImGui::Image(
                reinterpret_cast<void*>(frameBuffer.getColorAttachment().getId()), 
                ImVec2(200, 200), 
                ImVec2(0, 1), 
                ImVec2(1, 0)
            );

            ImGui::End();

            ImGui::Begin("Properties");

            ImGui::End();
        }

        ImGui::Render();

        // TODO: Replace with a custom implementation
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.swapBuffers();
	}

    // TODO: Replace with a custom implementation
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

	return 0;
}
