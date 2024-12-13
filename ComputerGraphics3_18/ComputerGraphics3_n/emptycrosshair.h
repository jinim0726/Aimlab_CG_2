#pragma once
#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

class EmptyCrosshair {
private:
    GLuint vao, vbo[2];           // VAO�� VBO
    glm::vec3 color;           // ���ؼ� ����
    GLfloat size;              // ���ؼ� ũ��
    GLfloat gap;               // ��� ��� �ִ� ����

public:
    EmptyCrosshair();                                    // �⺻ ������
    EmptyCrosshair(glm::vec3 color, GLfloat size, GLfloat gap); // ����, ũ��, ���� ����

    void Init(GLuint shaderProgramID);                  // VAO, VBO �ʱ�ȭ
    void SetColor(glm::vec3 newColor);                  // ���� ����
    void SetSize(GLfloat newSize);                      // ũ�� ����
    void Draw(GLuint shaderProgramID, int windowWidth, int windowHeight);  // ���ؼ� �׸���
};