#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f); // ī�޶� ��ġ
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);   // ī�޶� �ٶ󺸴� ����
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);       // ���� �� ����
GLfloat cameraYaw = -90.0f; // ī�޶��� Y�� ȸ�� (����)
GLfloat cameraPitch = 0.0f; // ī�޶��� X�� ȸ�� (����)
GLfloat lastX = 450.f, lastY = 450.f;  // ���콺 �ʱ� ��ġ (������ �߽ɰ����� ����)
GLfloat sensitivity = 0.1f;        // ���콺 ����
GLfloat yaw = -90.0f; // �⺻������ -Z���� �ٶ󺸵��� �ʱ�ȭ (-90��)
GLfloat pitch = 0.0f; // ���Ʒ� ������ �ʱ⿡�� ���� ����
bool isMouseLocked = true; // ���콺 ���� ����
bool firstMouse = true;          // ���콺 �ʱ� �̵� ����
bool ignoreMouseEvent = false; // ���콺 �߾� ���� �̺�Ʈ ���� �÷���