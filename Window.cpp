//
// Created by Jean-Luc Thumm on 11/25/17.
//

#include "Window.h"
#include "util.h"
#include "MeshBank.h"
#include "Geometry.h"
#include "DirLight.hpp"
#include "PointLight.hpp"
#include "Mover.hpp"
#include "MaterialBank.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <glm/ext.hpp>


using std::cerr;
using std::endl;
using std::cout;

static Window *instance;

Window &Window::Instance() {
    if (!instance) {
        throw std::runtime_error{"Tried to get Window instance when none instantiated"};
    }
    return *instance;
}

void Window::resize_callback(GLFWwindow *window, int width, int height) {
    instance->handleResize(window, width, height);
}

void Window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    instance->handleKey(window, key, scancode, action, mods);
}

void Window::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    instance->handleMouseButton(window, button, action, mods);
}

void Window::mouse_move_callback(GLFWwindow *window, double xpos, double ypos) {
    instance->handleMouseMove(window, xpos, ypos);
}

void Window::mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    instance->handleMouseScroll(window, xoffset, yoffset);
}

void Window::error_callback(int error, const char *description) {
    cerr << description << endl;
}

void Window::handleKey(GLFWwindow *w, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        auto result = keyListeners.find(key);
        if (result != keyListeners.end()) {
            for (EventListener *listener : result->second) {
                listener->receive(key);
            }
        }

        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(w, GL_TRUE);
                break;
            case GLFW_KEY_S:
                float factor;
                if (mods == GLFW_MOD_SHIFT) {
                    factor = 1.0f - SCALE_SCALE;
                }
                else {
                    factor = 1.0f + SCALE_SCALE;
                }
                graph.scale(glm::vec3{factor});
                break;
            case GLFW_KEY_R:
                cam.reset();
                break;
            case GLFW_KEY_C:
                orbitFlag = false;
                transFlag = false;
                break;
            default:
                break;
        }
    }
}

void Window::handleMouseButton(GLFWwindow *w, int button, int action, int mods) {
    double xpos, ypos;
    glfwGetCursorPos(w, &xpos, &ypos);
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS && mods != GLFW_MOD_SHIFT) {
            Ray ray = raycast(static_cast<float>(xpos), static_cast<float>(ypos));
            std::list<Node *> hits = graph.hit(ray);
            if (hits.empty()) {
                orbitFlag = true;
                lastOrbitPoint = trackBallMapping(static_cast<float>(xpos),
                                                  static_cast<float>(ypos));
                cam.save(); // use current state as reference for transformations
            }
            else {
                // TODO pick the closest event listener instead of first
                selected = dynamic_cast<EventListener *>(*(hits.begin()));
                if (selected != nullptr) {
                    selected->receive(Event{Event::PRESS, ray});
                }
            }
        }
        else if (action == GLFW_RELEASE && mods != GLFW_MOD_SHIFT) {
            if (selected != nullptr) {
                Ray ray = raycast(static_cast<float>(xpos), static_cast<float>(ypos));
                selected->receive(Event{Event::RELEASE, ray});
                selected = nullptr;
            }
            orbitFlag = false;
            dragFlag = false;
        }
        else if (action == GLFW_PRESS) {
            transFlag = true;
            lastTransPoint = glm::vec2{xpos, ypos};
        }
        else if (action == GLFW_RELEASE) {
            transFlag = false;
        }
    }
}

void Window::handleMouseMove(GLFWwindow *w, double xpos, double ypos) {
    if (selected != nullptr) {
        Ray ray = raycast(static_cast<float>(xpos), static_cast<float>(ypos));
        selected->receive(Event{Event::DRAG, ray});
        return;
    }

    if (orbitFlag) {
        dragFlag = true;
        glm::vec3 curPoint = trackBallMapping(static_cast<float>(xpos), static_cast<float>(ypos));
        glm::vec3 direction = curPoint - lastOrbitPoint;

        float velocity = glm::length(direction);
        if (velocity < 0.0001) return;  // only act when movement is great enough

        glm::vec3 rotAxis = glm::cross(lastOrbitPoint, curPoint);
        float rotAngle = velocity * ROT_SCALE;
        cam.statefulRotate(-rotAngle, rotAxis);
    }

    if (transFlag) {
        auto deltaX = static_cast<float>(xpos - lastTransPoint.x);
        auto deltaY = static_cast<float>(lastTransPoint.y - ypos);
        glm::vec3 transVec{deltaX, deltaY, 0};
    }
}

void Window::handleMouseScroll(GLFWwindow *w, double xoffset, double yoffset) {
    glm::vec3 transVec = {0, 0, SCROLL_SCALE * yoffset};
}

void Window::handleResize(GLFWwindow *w, int width, int height) {
#ifdef __APPLE__
    glfwGetFramebufferSize(w, &width,
                           &height); // In case your Mac has a retina display
#endif
    this->width = width;
    this->height = height;
    glViewport(0, 0, width, height);

    if (height > 0) {
        projection = glm::perspective(45.0f, (float) width / (float) height, 0.1f,
                                      1000.0f);
    }
}

glm::vec3 Window::trackBallMapping(float x, float y) {
    int w = width / 2;
    int h = height / 2;
    glm::vec3 v;
#ifdef __APPLE__ // retina bullshit
    v.x = (2.0f * x - w) / w;
    v.y = (h - 2.0f * y) / h;
    v.z = 0.0;
#else
    v.x = (x - w) / w;
    v.y = (h - y) / h;
    v.z = 0.0;
#endif
    float d = glm::length(v);
    d = (d < 1.0f) ? d : 1.0f;
    v.z = std::sqrt(1.001f - d * d);
    return glm::normalize(v);
}

Ray Window::raycast(float mouseX, float mouseY) {
#ifdef __APPLE__ // retina bullshit
    float x = (2.0f * mouseX) / (width * 0.5f) - 1.0f;
    float y = (2.0f * mouseY) / (height * 0.5f) - 1.0f;
#else
    float x = mouseX / (width * 0.5f) - 1.0f;
    float y = mouseY / (height * 0.5f) - 1.0f;
#endif

    glm::mat4 invVP = glm::inverse(projection * cam.viewMatrix());
    glm::vec4 screenPos = glm::vec4(x, -y, 1.0f, 1.0f);
    glm::vec4 worldPos = invVP * screenPos;

    auto dir = glm::vec3{worldPos};

    return Ray{cam.position(), dir};
}

void Window::registerHandlers(GLFWwindow *w) {
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(w, key_callback);
    glfwSetFramebufferSizeCallback(w, resize_callback);
    glfwSetMouseButtonCallback(w, mouse_button_callback);
    glfwSetCursorPosCallback(w, mouse_move_callback);
    glfwSetScrollCallback(w, mouse_scroll_callback);
}

GLFWwindow *Window::makeGLFWWindow(int width, int height) {
    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW" << endl;
        return nullptr;
    }

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow *window = glfwCreateWindow(width, height, "Window", nullptr,
                                          nullptr);

    if (!window) {
        cerr << "Failed to open GLFW window" << endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwGetFramebufferSize(window, &width, &height);

    // print info
    std::printf("Renderer: %s\n", (char *) glGetString(GL_RENDERER));
    std::printf("OpenGL version: %s\n", (char *) glGetString(GL_VERSION));
#ifdef GL_SHADING_LANGUAGE_VERSION
    std::printf("GLSL version: %s\n",
                (char *) glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif

    handleResize(window, width, height); // sets values for current size

#ifndef __APPLE__
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        cerr << "Error: " << glewGetErrorString(err) << endl;
        glfwTerminate();
    }
    cout << "Current GLEW version: " << glewGetString(GLEW_VERSION) << endl;
#endif
    glGetString(GL_RENDERER);


    return window;
}

void Window::setOpenGLPrefs() {
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

//    glClearColor(0.05f, 0.8f, 0.85f, 1.0f);
    glClearColor(0, 0, 0, 1);

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.0);

    glEnable(GL_PROGRAM_POINT_SIZE);
}

void Window::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    skybox->draw(cam.viewMatrix(), projection);
    graph.draw(glm::mat4{}, cam.viewMatrix(), projection);

    glfwPollEvents();
    glfwSwapBuffers(glfwWindow);
}

void Window::idle() {
    graph.update();
}

void Window::start() {
    if (started) {
        throw std::runtime_error{"Cannot start already started Window"};
    }
    started = true;

    while (!glfwWindowShouldClose(glfwWindow)) {
        idle();
        display();

        GLenum error = glGetError();

        if (error != GL_NO_ERROR) {
            util::print_error(error);
        }
    }
}

Window::~Window() {
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

const glm::mat4 &Window::View() {
    return cam.viewMatrix();
}

const glm::mat4 &Window::Projection() {
    return projection;
}

GLuint Window::SkyboxTexture() {
    return skybox->TextureID();
}

glm::vec3 Window::CameraPos() {
    return cam.position();
}

void Window::subscribe(int key, EventListener *listener) {
    auto result = keyListeners.insert(std::make_pair(key, std::vector<EventListener *>{listener}));
    if (!result.second) {
        // key already exists
        (result.first)->second.push_back(listener);
    }
}

Window::Window(int width, int height)
        : width{width}, height{height}, orbitFlag{false}, transFlag{false}, dragFlag{false},
          started{false}, selected{nullptr},
          cam{
                  glm::vec3{0.0f, 0.0f, 5.0f},
                  glm::vec3{0.0f, 0.0f, 0.0f},
                  glm::vec3{0.0f, 1.0f, 0.0f}
          } {
    instance = this;
    glfwWindow = makeGLFWWindow(width, height);
    if (!glfwWindow) throw std::runtime_error{"Could not make GLFW window"};
    setOpenGLPrefs();
    registerHandlers(glfwWindow);
    instantiateSingletons();
    setupScene();
}

void Window::instantiateSingletons() {
    MaterialBank::instantiate();
    MeshBank::instantiate();
}

void Window::setupScene() {
    // Shaders
    auto cubeMapShader = std::make_shared<Shader>("shader/sky.vert", "shader/sky.frag");
    auto lightShader = std::make_shared<Shader>("shader/light.vert", "shader/light.frag");
    auto materialOnlyShader = std::make_shared<Shader>("shader/material_only.vert",
                                                       "shader/material_only.frag");
    auto noLightShader = std::make_shared<Shader>("shader/no_lighting.vert",
                                                  "shader/no_lighting.frag");

    // Meshes
    MeshBank::refID cubeID = MeshBank::I()->load("obj/cube.obj");
    MeshBank::refID carID = MeshBank::I()->load("obj/porsche/mod.obj");
    MeshBank::refID grassID = MeshBank::I()->load("obj/grass.obj");
    MeshBank::refID roadID = MeshBank::I()->load("obj/road.obj");

    skybox = std::make_unique<CubeMap>(
            std::vector<std::string>{
                    "right.jpg",
                    "left.jpg",
                    "top.jpg",
                    "bottom.jpg",
                    "back.jpg",
                    "front.jpg"
            },
            "skybox/",
            cubeMapShader
    );


    auto *dirLight = new DirLight{
            glm::vec3{1.0, -1.0, -0.2},
            glm::vec3{0.2, 0.2, 0.2},
            0
    };
    dirLight->attach(materialOnlyShader);
    graph.addChild(dirLight);

    auto *pointLight = new PointLight{
            0, 0.0, 0.25, 0.0,
            glm::vec3{0.2, 0.2, 0.2},
            glm::vec3{0.2, 0.2, 0.2},
            glm::vec3{0.2, 0.2, 0.2},
    };
    pointLight->attach(materialOnlyShader);

    auto trans = new Mover;
    trans->scale(glm::vec3{0.06});
    trans->translate(glm::vec3{10, 1, 0});
    trans->addChild(pointLight);
    trans->addChild(new Geometry{cubeID, lightShader});
    graph.addChild(trans);

    Geometry *car = new Geometry{carID, noLightShader};
    graph.addChild(car);
}
