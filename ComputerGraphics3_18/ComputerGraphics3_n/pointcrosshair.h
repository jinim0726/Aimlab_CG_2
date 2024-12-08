#pragma once
#include <iostream>
#include <gl/glew.h> 
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

class PointCrosshair {
private:
    GLuint vao, vbo;         // VAO�� VBO
    glm::vec3 color;         // ���ؼ� ����
    GLfloat size;            // ���ؼ� ũ��

    // ������ ���� ��ġ
    GLint projectionLoc;              // ���� ��� ��ġ
    GLint viewLoc;                    // �� ��� ��ġ
    GLint modelLoc;                   // �� ��ȯ ��� ��ġ
    GLint colorLoc;                   // ���� ������ ��ġ

public:
    PointCrosshair(); // �⺻ ������
    PointCrosshair(glm::vec3 color, GLfloat size);

    void Init(GLuint shaderProgramID);
    void SetColor(glm::vec3 newColor);
    void SetSize(GLfloat newSize);
    void Draw(GLuint shaderProgramID, int windowWidth, int windowHeight);
};
