#include <iostream>
#include <string>

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Utilities.h"
#include "Texture.h"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 960
#define CAM_SPEED_FACTOR 10;
#define MOUSE_DRAG_SENSITIVITY_FACTOR 0.01f;
#define MOUSE_SCROLL_SENSITIVITY_FACTOR 2.0f;

float xpos_prev = WINDOW_WIDTH / 2;
float ypos_prev = WINDOW_HEIGHT / 2;
glm::vec3 camFront(0.0f, 0.0f, -1.0f);
float fov = 45.0f;
float pitch = 0.0f;
float yaw = -90.0f;
bool firstMouse = true;

void mouseCursorCallback(GLFWwindow* window, double xpos, double ypos);
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
        std::cout << "Error: Glew init not ok" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;

    auto [vertices, sizeOfVertices]  = parseFloatArrayFromString("resources/data/hard_coded_data.txt", "vertices");
    unsigned int elements[6] = { 0, 1, 2, 0, 1, 3 };

    unsigned int vertexBufferID, vertexArrayID, elementArrayID;   // vertex array object
    glGenVertexArrays(1, &vertexArrayID);
    glGenBuffers(1, &vertexBufferID);
    glGenBuffers(1, &elementArrayID);

    glBindVertexArray(vertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayID);

    glBufferData(GL_ARRAY_BUFFER        , sizeOfVertices  , vertices, GL_STATIC_DRAW);
    delete[] vertices;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 2) * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (2 + 3) * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int shader = createShader("resources/shaders/vertex_and_fragment.shader");
    glUseProgram(shader);
    Texture container("resources/images/container.jpg", 0);
    container.bindToShader(shader, "texture0"); 
    Texture smileyFace("resources/images/awesomeface.png", 1);
    smileyFace.bindToShader(shader, "texture1");
    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    glEnable(GL_DEPTH_TEST);
    glm::vec3 camPos(0.0f, 0.0f, 15.0f);
    glm::vec3 camUp(0.0f, 1.0f, 0.0f);
    glm::vec3 upVector = camUp;
    view = glm::lookAt(camPos, glm::vec3(0.0f, 0.0f, 0.0f), camUp);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    float camSpeed = 0.01f;
    float lastFrame, currentFrame = 0.0f, frameDuration;


    /*glm::vec3 cubePositions[] = 
    {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(3.0f,  0.0f,  0.0f),
        glm::vec3(0.0f,  3.0f,  0.0f),
        glm::vec3(0.0f,  0.0f,  3.0f),
        glm::vec3(3.0f,  3.0f,  3.0f)
    };*/


    auto [cubePositions, sizeOfCubePositions] = parseFloatArrayFromString("resources/data/hard_coded_data.txt", "cubePositions");
    glfwSetCursorPosCallback(window, mouseCursorCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        lastFrame = currentFrame;
        currentFrame = glfwGetTime();
        frameDuration = currentFrame - lastFrame;
        camSpeed = frameDuration*CAM_SPEED_FACTOR;

        // Read keyboard input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose (window, true);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            camPos += camSpeed * camFront; 
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            camPos -= camSpeed * camFront;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            camPos -= camSpeed * glm::normalize(glm::cross(camFront, camUp));
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            camPos += camSpeed * glm::normalize(glm::cross(camFront, camUp));
        view = glm::lookAt(camPos, camPos+camFront, upVector);
        glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
               
        for (unsigned int i = 0; i < sizeOfCubePositions/sizeof(cubePositions[0]); i++)
        {
            model = glm::translate(glm::mat4(1.0f), glm::vec3(cubePositions[i*3], cubePositions[i*3+1], cubePositions[i*3+2]));
            glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model)); 
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        projection = glm::perspective(glm::radians(fov), float(WINDOW_WIDTH / WINDOW_HEIGHT), 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    delete[] cubePositions;
    glfwTerminate();
    return 0;
}

void mouseCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        xpos_prev = xpos;
        ypos_prev = ypos;
        firstMouse = false;
    }
    float xpos_delta = xpos - xpos_prev;
    float ypos_delta = ypos - ypos_prev;
    xpos_prev = xpos;
    ypos_prev = ypos;
    yaw += xpos_delta * MOUSE_DRAG_SENSITIVITY_FACTOR;
    pitch += ypos_delta * MOUSE_DRAG_SENSITIVITY_FACTOR;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    camFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    camFront.y = sin(glm::radians(pitch));
    camFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
}

void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= yoffset * MOUSE_SCROLL_SENSITIVITY_FACTOR;
    if (fov > 45.0f)
        fov = 45.0f;
    if (fov < 1.0f)
        fov = 1.0f;
}

