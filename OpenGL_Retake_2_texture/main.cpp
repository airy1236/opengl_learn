#include <iostream>
#include "application/application.h"
#include "glframework/shader.h"
#include "glframework/texture.h"

GLuint vao;
Shader* shader = nullptr;
Texture *wallTexture = nullptr, *landTexture = nullptr, *noiseTexture = nullptr;

void LoadData() {
    float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };
    unsigned int index[] = {0, 1, 3,
                            1, 2, 3};

    GLuint vbo, ebo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), &index, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)) );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void preparTexture() {
    wallTexture = new Texture("assets/textures/wall.jpg", 0);
}

void preparShader() {
    shader = new Shader("assets/shaders/vertexshader.vert", "assets/shaders/fragmentshader.frag");
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    shader->begin();

    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    shader->end();
}

int main() {
    app->init(800, 600);
    
    glViewport(0, 0, 800, 600);

    glClearColor(0.4f, 0.3f, 0.5f, 1.0f);

    preparTexture();
    preparShader();
    LoadData();

    while (app->update()) {

        render();

    }

    app->destory();
    delete shader;
    delete wallTexture;
    delete landTexture;
    delete noiseTexture;

	return 0;
}