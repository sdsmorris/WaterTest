#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Model/Model.h"
#include "Camera.h"
#include "Texture/TextureLoader.h"
#include "Water/FBO.h"

//OpenGL
GLFWwindow* window;

Shader shader;
Shader ws;
Shader quadShader;
Model mdl, tree;

unsigned int stripvao;

glm::mat4 model, view, projection;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 1280 / 2.0f;
float lastY = 720/ 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//clipping plane
float clipPlaneDirection, clipPlaneDist;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void renderScene()
{
    glClearColor(0.5f, 0.5f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    shader.setVec4("u_ClipPlane", glm::vec4(0, clipPlaneDirection, 0, clipPlaneDist));

    //Camera update
    projection = glm::perspective(glm::radians(camera.Zoom), (float)1280 / (float)720, 0.1f, 100.0f);
    view = camera.GetViewMatrix();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    //Render map model
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    //model = glm::scale(model, glm::vec3(4.0f, 1.0f, 4.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setMat4("model", model);
    mdl.draw(shader);

    //Render trees
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(5.0f, 3.0f, 5.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(1, 1, 1));
    shader.setMat4("model", model);
    tree.draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.0f, 3.0f, 5.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(1, 1, 1));
    shader.setMat4("model", model);
    tree.draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(5.0f, 3.0f, -5.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(1, 1, 1));
    shader.setMat4("model", model);
    tree.draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(5.0f, 3.0f, -2.5f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(1, 1, 1));
    shader.setMat4("model", model);
    tree.draw(shader);

}

int main()
{	
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(1280, 720, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CLIP_DISTANCE0);
    glViewport(0, 0, 1280, 720);

    mdl = Model("C:/Users/sammo/Documents/Blender/My Models/test.obj");
    tree = Model("C:/Users/sammo/Documents/Blender/My Models/tree.dae");
    //Model mdl("res/skyplatform.dae");
    shader = Shader("C:/Users/sammo/source/repos/WaterRenderingTest/WaterRenderingTest/src/Model/staticModelVertex.glsl", "C:/Users/sammo/source/repos/WaterRenderingTest/WaterRenderingTest/src/Model/staticModelFragment.glsl");
    quadShader = Shader("C:/Users/sammo/source/repos/WaterRenderingTest/WaterRenderingTest/src/Water/texturedQuadVertex.glsl", "C:/Users/sammo/source/repos/WaterRenderingTest/WaterRenderingTest/src/Water/texturedQuadFragment.glsl");
	
    //Water
    ws = Shader("C:/Users/sammo/source/repos/WaterRenderingTest/WaterRenderingTest/src/Water/waterVertex.glsl", "C:/Users/sammo/source/repos/WaterRenderingTest/WaterRenderingTest/src/Water/waterFragment.glsl");
    unsigned int vao, vbo, ebo;

    /*float vertices[] = { -0.5, 0.5, 0,
                         0.5, 0.5, 0,
                         0.5, -0.5, 0,
                        -0.5, -0.5, 0 }; 

    unsigned short indices[] = { 0, 1, 2, 0, 2, 3 };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);*/

    //Textured GUI quad
    float quadverts[] = { -0.35, 0.95, 0, 1.0, 1.0,
        -0.95, 0.95, 0, 0, 1.0,
        -0.35, 0.35, 0, 1.0, 0,
        -0.95, 0.35, 0, 0, 0 };

    unsigned int quadvao, quadvbo;
    glGenVertexArrays(1, &quadvao);
    glBindVertexArray(quadvao);
    glGenBuffers(1, &quadvbo);
    glBindBuffer(GL_ARRAY_BUFFER, quadvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadverts), quadverts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    float quadverts2[] = { 0.35, 0.95, 0, 0, 1.0,
        0.95, 0.95, 0, 1.0, 1.0,
        0.35, 0.35, 0, 0, 0,
        0.95, 0.35, 0, 1.0, 0 };

    unsigned int quadvao2, quadvbo2;
    glGenVertexArrays(1, &quadvao2);
    glBindVertexArray(quadvao2);
    glGenBuffers(1, &quadvbo2);
    glBindBuffer(GL_ARRAY_BUFFER, quadvbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadverts2), quadverts2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    unsigned int tex = TextureLoader::textureFromFileToTarget("C:/Users/sammo/Documents/Resources/Textures/Rips/Super Mario Galaxy 2/ThrowBackGalaxy/OldFSS20.png", GL_TEXTURE_2D);

    //Water tile
    float stripverts[] = { 0.5, 0.5, 0, 1.0, 0,
        0.5, -0.5, 0, 1.0, 1,
        -0.5, 0.5, 0, 0, 0,
        -0.5, -0.5, 0, 0, 1 };

    unsigned int stripvbo;
    glGenVertexArrays(1, &stripvao);
    glBindVertexArray(stripvao);
    glGenBuffers(1, &stripvbo);
    glBindBuffer(GL_ARRAY_BUFFER, stripvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(stripverts), stripverts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    //Refraction FBO (both color buffers)
    FBO fbo(300, 200);
    fbo.BindFramebuffer();
    fbo.CreateColorTextureAttachment();
    fbo.CreateDepthTextureAttachment();
    fbo.UnbindFramebuffer();

    //Reflection FBO (color buffer and render buffer)
    FBO fbo2(300, 200);
    fbo2.BindFramebuffer();
    fbo2.CreateColorTextureAttachment();
    fbo2.CreateDepthBufferAttachment();
    fbo2.UnbindFramebuffer();

    //Load DuDv map
    int dudv = TextureLoader::textureFromFileToTarget("C:/Users/sammo/Documents/Resources/Tutorials/Water/waterDUDV.png", GL_TEXTURE_2D);

    //Debug info
    const GLubyte* renderer = glGetString(GL_RENDERER); // Returns a hint to the model
    std::cout << "GPU: " << renderer << "\n";
    const GLubyte* vendor = glGetString(GL_VENDOR);
    std::cout << "Vendor: " << vendor << "\n";
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << version << "\n";

    //Water scroll stuff
    const float WAVE_SPEED = 0.03f;
    float moveFactor = 0;

    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        //Refraction (Everything below)
        clipPlaneDirection = -1;
        clipPlaneDist = 0.95;
        fbo.BindFramebuffer();
        renderScene();
        fbo.UnbindFramebuffer();

        //Refelction (Everything above)
        clipPlaneDirection = 1;
        clipPlaneDist = -0.95;
        fbo2.BindFramebuffer();
        renderScene();
        fbo2.UnbindFramebuffer();
        
        //Render regular scene
        clipPlaneDirection = 0;
        clipPlaneDist = 0;
        renderScene();

        //Render water
        moveFactor += WAVE_SPEED * deltaTime;
        //moveFactor = fmod(moveFactor, 1);
        ws.use();
        ws.setInt("reflectionTexture", 0);
        ws.setInt("refractionTexture", 1);
        ws.setInt("dudvMap", 2);
        ws.setFloat("moveFactor", moveFactor);
        ws.setMat4("projection", projection);
        ws.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
        model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));
        shader.setMat4("model", model);

        //glBindVertexArray(vao);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fbo2.m_ColorBufferID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, fbo.m_ColorBufferID);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, dudv);
        glBindVertexArray(stripvao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        /*quadShader.use();
        glBindVertexArray(quadvao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fbo.m_ColorBufferID);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(quadvao2);
        glBindTexture(GL_TEXTURE_2D, fbo2.m_ColorBufferID);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);*/
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}