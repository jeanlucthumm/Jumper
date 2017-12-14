//
// Created by Jean-Luc Thumm on 11/25/17.
//

#ifndef PA4_NWINDOW_H
#define PA4_NWINDOW_H

#include "GLinclude.h"
#include "Transform.h"
#include "Camera.h"
#include "CubeMap.h"
#include "Ray.h"
#include "EventListener.h"
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <memory>
#include <unordered_map>

class Window {
public:
    Window(int width, int height);
    ~Window();
    Window(const Window &) = delete;
    Window &operator=(Window &) = delete;

    void start();
    void subscribe(int key, EventListener *listener);

    static Window &Instance();
    const glm::mat4 &View();
    const glm::mat4 &Projection();
    GLuint SkyboxTexture();
    glm::vec3 CameraPos();

    constexpr static float ROT_SCALE = 4.0f;
    constexpr static float TRANS_SCALE = 0.03f;
    constexpr static float SCROLL_SCALE = 0.1f;
    constexpr static float SCALE_SCALE = 0.1f;
private:
    void handleKey(GLFWwindow *w, int key, int scancode, int action, int mods);
    void handleMouseButton(GLFWwindow *w, int button, int action, int mods);
    void handleMouseMove(GLFWwindow *w, double xpos, double ypos);
    void handleMouseScroll(GLFWwindow *w, double xoffset, double yoffset);
    void handleResize(GLFWwindow *w, int width, int height);

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    static void mouse_move_callback(GLFWwindow *window, double xpos, double ypos);
    static void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
    static void resize_callback(GLFWwindow *window, int width, int height);
    static void error_callback(int error, const char *description);

    GLFWwindow *makeGLFWWindow(int width, int height);
    void setOpenGLPrefs();
    void registerHandlers(GLFWwindow *w);
    void instantiateSingletons();
    void setupScene();

    void display();
    void idle();

    glm::vec3 trackBallMapping(float x, float y);
    Ray raycast(float mouseX, float mouseY);

    int width, height;
    Transform graph;
    GLFWwindow *glfwWindow;
    Camera cam;
    bool orbitFlag, transFlag;
    bool dragFlag;
    bool started;
    glm::vec2 lastTransPoint;
    glm::vec3 lastOrbitPoint;
    glm::mat4 projection;
    std::unique_ptr<CubeMap> skybox;
    EventListener *selected;
    std::unordered_map<int, std::vector<EventListener *>> keyListeners;
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    bool renderStarted;
};


#endif //PA4_NWINDOW_H
