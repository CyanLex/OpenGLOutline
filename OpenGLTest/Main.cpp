#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Shader/Shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <vector>
#include <map>
#include <utility>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// cube vertex data: position (x,y,z) + color (r,g,b)
float cubeVertices[] = {
    // Front
    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,

    // Back
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.5f,
};


unsigned int cubeIndices[] = {
    0, 1, 2, 2, 3, 0, // front
    4, 5, 6, 6, 7, 4, // back
    0, 4, 7, 7, 3, 0, // left
    1, 5, 6, 6, 2, 1, // right
    3, 2, 6, 6, 7, 3, // top
    0, 1, 5, 5, 4, 0  // bottom
};

// function that gives you the adjacent verices of given vertices

std::vector<unsigned int> buildAdjacency(const std::vector<unsigned int>& indices)
{
    using Edge = std::pair<unsigned int, unsigned int>;
    struct EdgeData { unsigned int triIndex, opp; };

    auto make_edge = [](unsigned int a, unsigned int b) {
        return (a < b) ? std::make_pair(a, b) : std::make_pair(b, a);
        };

    std::map<Edge, std::vector<EdgeData>> edgeMap;

    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int a = indices[i];
        unsigned int b = indices[i + 1];
        unsigned int c = indices[i + 2];
        edgeMap[make_edge(a, b)].push_back({ (unsigned int)i, c });
        edgeMap[make_edge(b, c)].push_back({ (unsigned int)i, a });
        edgeMap[make_edge(c, a)].push_back({ (unsigned int)i, b });
    }

    std::vector<unsigned int> adj;
    adj.reserve(indices.size() * 2);

    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int a = indices[i];
        unsigned int b = indices[i + 1];
        unsigned int c = indices[i + 2];

        auto getOpp = [&](unsigned int v0, unsigned int v1, unsigned int oppDefault) {
            auto key = make_edge(v0, v1);
            auto& list = edgeMap[key];
            for (auto& ed : list) {
                if (ed.triIndex != i) return ed.opp;
            }
            return oppDefault; // no adjacent triangle found
            };

        unsigned int adj0 = getOpp(a, b, c);
        unsigned int adj1 = getOpp(b, c, a); 
        unsigned int adj2 = getOpp(c, a, b); 

        adj.push_back(a);
        adj.push_back(adj0);
        adj.push_back(b);
        adj.push_back(adj1);
        adj.push_back(c);
        adj.push_back(adj2);
    }

    return adj;
}


int main()
{
    // glfw: initialize
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Outlined Cube", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // build and compile shaders
    Shader ourShader("3.3.shader.vert", "3.3.shader.frag", "3.3.shader.geom"); // no more , 

    std::vector<unsigned int> indices(cubeIndices, cubeIndices + 36);
    std::vector<unsigned int> adjIndices = buildAdjacency(indices); // adjacent indices

    // set up vertex data and buffers
    //VBO Vertex Buffer Objects
    //VAO Vertex Array Object
    //EBO Element Buffer Object
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO); 

    glBindVertexArray(VAO);
    

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // buffer with adjacency info
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,adjIndices.size() * sizeof(unsigned int),adjIndices.data(),GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    // TODO:everythng is black 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        ourShader.use();
        

        // i have no idea of how this work, something somthing transformation matrix for 3D
        // ohh matrix rotation
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(),glm::vec3(0.5f, 1.0f, 0.0f));
        glm::mat4 view = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)SCR_WIDTH / (float)SCR_HEIGHT,0.1f, 100.0f);

        // send to shaders
        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        // send window scale for outline
        ourShader.setVec2("uWinScale", 1.0f / SCR_WIDTH, 1.0f / SCR_HEIGHT);

        // draw cube but WHERE IS THE CUBE
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36); // with no adjacency
        glDrawElements(GL_TRIANGLES_ADJACENCY, (GLsizei)adjIndices.size(), GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
