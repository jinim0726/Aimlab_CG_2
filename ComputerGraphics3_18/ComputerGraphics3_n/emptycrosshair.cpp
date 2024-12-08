#include "emptycrosshair.h"

// �⺻ ������
EmptyCrosshair::EmptyCrosshair()
    : color(glm::vec3(1.0f, 1.0f, 1.0f)), size(0.1f), gap(0.02f), vao(0), vbo(0) {}

// ����� ���� ������
EmptyCrosshair::EmptyCrosshair(glm::vec3 color, GLfloat size, GLfloat gap)
    : color(color), size(size), gap(gap), vao(0), vbo(0) {}

// �ʱ�ȭ �Լ�
void EmptyCrosshair::Init(GLuint shaderProgramID) {
    // ������ �̷���� ������ ���ؼ��� ��ǥ
    GLfloat crosshairVertices[] = {
        // ���� (����)
        -size - gap, 0.0f, 0.0f,
        -gap,        0.0f, 0.0f,
        // ���� (������)
         gap,        0.0f, 0.0f,
         size + gap, 0.0f, 0.0f,
         // ������ (�Ʒ���)
           0.0f, -size - gap, 0.0f,
           0.0f, -gap,        0.0f,
           // ������ (����)
             0.0f,  gap,        0.0f,
             0.0f,  size + gap, 0.0f
    };

    // �� ������ ���� (��: ������ RGBA)
    GLfloat crosshairColors[] = {
        0.0f, 0.0f, 0.0f, 1.0f, // ���� ��
        0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 1.0f, // ������ ��
        0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 1.0f, // �Ʒ��� ��
        0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 1.0f, // ���� ��
        0.0f, 0.0f, 0.0f, 1.0f
    };

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


// ���� ����
void EmptyCrosshair::SetColor(glm::vec3 newColor) {
    color = newColor;
}

// ũ�� ����
void EmptyCrosshair::SetSize(GLfloat newSize) {
    size = newSize;
}

// �׸��� �Լ�
void EmptyCrosshair::Draw(GLuint shaderProgramID, int windowWidth, int windowHeight) {
    glUseProgram(shaderProgramID);

    // ȭ�� ���� ���
    float aspectRatio = (float)windowWidth / (float)windowHeight;

    // ��� ����
    glm::mat4 projection = glm::ortho(-1.0f * aspectRatio, 1.0f * aspectRatio, -1.0f, 1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    GLuint viewLoc = glGetUniformLocation(shaderProgramID, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgramID, "projection");
    GLuint modelLoc = glGetUniformLocation(shaderProgramID, "modelTransform");
    GLuint colorLoc = glGetUniformLocation(shaderProgramID, "color");

    // ��� ����
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    

    // ���ؼ� �׸���
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 8); 
    glBindVertexArray(0);
}
