#include "shader.h"
#include "objects.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

void init(GLFWwindow* window);
void display(GLFWwindow* window, double currentTime);

glm::vec3 cameraLoc;

GLuint program;
GLuint vMatLoc, projMatLoc, mMatLoc;

GLuint vbo[2];
GLuint vao[1];

int w, h;
float aspect;
glm::mat4 pMat, vMat;

obj o;

int main(int, char**) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(600, 600, "glfw", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewInit();

    glfwSwapInterval(1);

    init(window);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        display(window, glfwGetTime());

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_D){
        cameraLoc.x++;
    }
    else if(key == GLFW_KEY_A){
        cameraLoc.x--;
    }
    else if(key == GLFW_KEY_W){
        cameraLoc.z--;
    }
    else if(key == GLFW_KEY_S){
        cameraLoc.z++;
    }
    else if(key == GLFW_KEY_SPACE){
        cameraLoc.y++;
    }
    else if(key == GLFW_KEY_LEFT_SHIFT){
        cameraLoc.y--;
    }
}

void drawModel(glm::mat4 model){
    glUniformMatrix4fv(mMatLoc, 1, GL_FALSE, glm::value_ptr(model));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glDrawElements(GL_TRIANGLES, o.getIndexBufferObjectData().size(), GL_UNSIGNED_INT, 0);
}

void display(GLFWwindow* window, double currentTime){
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glUseProgram(program);

    vMat = glm::translate(glm::mat4(1.0), -cameraLoc);

    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniformMatrix4fv(vMatLoc, 1, GL_FALSE, glm::value_ptr(vMat));

    drawModel(glm::translate(glm::mat4(1), glm::vec3(0, 0, 0)));
}

void resizeCallback(GLFWwindow* window, int width, int height){
    glfwSetWindowSize(window, width, height);
    glfwGetFramebufferSize(window, &w, &h);
    aspect = (float)w/(float)h;
    pMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);
}

void init(GLFWwindow* window){
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, resizeCallback);

    resizeCallback(window, 600, 600);

    program = createProgram("shaders/vert.glsl", "shaders/frag.glsl");

    o = obj("arrow.obj");

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    //gen vertex vuffer object
    glGenBuffers(2, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, o.getVertexBufferData().size() * sizeof(float), &o.getVertexBufferData()[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    //gen index buffer object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, o.getIndexBufferObjectData().size() * sizeof(int), &o.getIndexBufferObjectData()[0], GL_STATIC_DRAW);

    vMatLoc = glGetUniformLocation(program, "v_matrix");
    projMatLoc = glGetUniformLocation(program, "proj_matrix");
    mMatLoc = glGetUniformLocation(program, "m_matrix");

    cameraLoc = glm::vec3(0.0f, 0.0f, 2.0f);
}
