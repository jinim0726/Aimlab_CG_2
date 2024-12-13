#pragma once
#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

class Crosshair {
private:
    GLuint vao, vbo[2];         // VAO�� VBO
    glm::vec3 color;         // ���ؼ� ����
    GLfloat size;            // ���ؼ� ũ��
    float vertices[12];

    // ������ ���� ��ġ
    GLint projectionLoc;              // ���� ��� ��ġ
    GLint viewLoc;                    // �� ��� ��ġ
    GLint modelLoc;                   // �� ��ȯ ��� ��ġ
    GLint colorLoc;                   // ���� ������ ��ġ

public:
    Crosshair();                             // �⺻ ������
    Crosshair(glm::vec3 color, GLfloat size); // ����� ũ�⸦ �����ϴ� ������

    void Init(GLuint shaderID);              // VAO/VBO �ʱ�ȭ
    void SetColor(glm::vec3 newColor);       // ���� ����
    void SetSize(GLfloat newSize);           // ũ�� ����
    void Draw(GLuint shaderProgramID, int windowWidth, int windowHeight);  // ���ؼ� �׸���
};
