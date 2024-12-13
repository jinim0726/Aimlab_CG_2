#pragma once
#include "crosshair.h"
#include "shaderSourceCode.h"

Crosshair::Crosshair() : color(glm::vec3(0.0f, 0.0f, 0.0f)), size(0.02f) {}

Crosshair::Crosshair(glm::vec3 color, GLfloat size) : color(color), size(size) {}

void Crosshair::Init(GLuint shaderProgramID) {
    // ũ�ν���� ���� ��ǥ (NDC ��ǥ��)
    float crosshairVertices[] = {
        -0.02f,  0.0f, 0.0f, // ���� ��
         0.02f,  0.0f, 0.0f, // ������ ��
         0.0f, -0.02f, 0.0f, // �Ʒ��� ��
         0.0f,  0.02f, 0.0f  // ���� ��
    };

    float crosshairColors[] = {
    0.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 
    0.0f, 0.0f, 0.0f, 1.0f, 
    0.0f, 0.0f, 0.0f, 1.0f  
    };


    // VAO, VBO ����
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbo);

    glBindVertexArray(vao);

    // ��ġ ������
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ���� ������
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairColors), crosshairColors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}



void Crosshair::SetColor(glm::vec3 newColor) {
    color = newColor;
}

void Crosshair::SetSize(GLfloat newSize) {
    size = newSize;

    // ������Ʈ�� ũ��� ���� �缳��
    vertices[0] = -size; vertices[1] = 0.0f; vertices[2] = 0.0f;
    vertices[3] = size; vertices[4] = 0.0f; vertices[5] = 0.0f;
    vertices[6] = 0.0f; vertices[7] = -size; vertices[8] = 0.0f;
    vertices[9] = 0.0f; vertices[10] = size; vertices[11] = 0.0f;

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Crosshair::Draw(GLuint shaderProgramID, int windowWidth, int windowHeight) {
    glUseProgram(shaderProgramID);

    // ���� ���
    float aspectRatio = (float)windowWidth / (float)windowHeight;

    // ��� ����
    glm::mat4 projection = glm::ortho(-1.0f * aspectRatio, 1.0f * aspectRatio, -1.0f, 1.0f);
    glm::mat4 view = glm::mat4(1.0f); // �⺻ ���� ���
    glm::mat4 model = glm::mat4(1.0f); // �⺻ ���� ���

    GLuint viewLoc = glGetUniformLocation(shaderProgramID, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgramID, "projection");
    GLuint modelLoc = glGetUniformLocation(shaderProgramID, "modelTransform");

    // ���� ��ķ� ����
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // ũ�ν���� �׸���
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);
}


