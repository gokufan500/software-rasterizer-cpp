#include "window_manager.h"
#include <iostream>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void error_callback(int error, const char* description)
{
  std::cerr << "glfw error " << error << " : " << description << std::endl;
}
WindowManager::WindowManager(int width, int height, const char* title) : width(width), height(height)
{

  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    throw std::runtime_error("Failed to init GLFW");
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(width, height, title, nullptr, nullptr);

  if (!window)
  {
    glfwTerminate();
    throw std::runtime_error("Failed to create window");
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsDark();

  const char* glsl_version = "#version 330 core";
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);



}


WindowManager::~WindowManager()
{
  if (window)
  {
    glfwDestroyWindow(window);
  }
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
}

bool WindowManager::shouldClose() const 
{
  return glfwWindowShouldClose(window);
}

void WindowManager::pollEvents() const 
{
  glfwPollEvents();
  if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
  {
      ImGui_ImplGlfw_Sleep(10);
  }
}

void WindowManager::swapBuffers() const 
{
  glfwSwapBuffers(window);
}
void WindowManager::beginImGuiFrame() const  
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void WindowManager::renderImGui() const
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void WindowManager::framebufferSizeCallback(GLFWwindow* window, int width, int height) 
{
  handleFrameBufferResize(window, width, height);
}

void WindowManager::handleFrameBufferResize(GLFWwindow* window, int width, int height) 
{
  glViewport(0,0,width,height);
}
