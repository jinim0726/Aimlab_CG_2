#pragma once
#include "shape.h"

#define SCREENWIDTH 1980
#define SCREENHEIGHT 1080

using namespace shape;

void Shape::SetIsActive(bool is)
{
	isActive = is;
}

void Shape::SetSpeed(GLfloat s_speed)
{
	speed = s_speed;
}

void Shape::SetDic(glm::mat3 dic)
{
	direcTionV3 = dic;
}

Camera::Camera()
{
	position = glm::vec3(0.0f, 0.0f, 1.0f); // ī�޶� ��ġ
	front = glm::vec3(0.0f, 0.0f, -1.0f);   // ī�޶� �ٶ󺸴� ����
	up = glm::vec3(0.0f, 1.0f, 0.0f);       // ���� �� ����
	fov = 108.0f;
	sensitivity = 0.255f;        // ���콺 ����
	yaw = -90.0f; // ī�޶��� Y�� ȸ�� (����) // �⺻������ -Z���� �ٶ󺸵��� �ʱ�ȭ (-90��)
	pitch = 0.0f; // ī�޶��� X�� ȸ�� (����) // ���Ʒ� ������ �ʱ⿡�� ���� ����
	isMouseLocked = true;		// ���콺 ���� ����
	firstMouse = true;          // ���콺 �ʱ� �̵� ����
	ignoreMouseEvent = false; // ���콺 �߾� ���� �̺�Ʈ ���� �÷���
}

void Camera::calculateCameraFront()
{
	glm::vec3 temp;

	// Yaw�� Pitch�� ����� ���� ���� ���
	temp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	temp.y = sin(glm::radians(pitch)); // pitch�� ���Ʒ� ����
	temp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch)); // yaw�� �¿� ȸ��

	// �� ���� ���͸� ����ȭ
	front = glm::normalize(temp);
	std::cout << "front : " << front.x << ", " << front.y << ", " << front.z << std::endl;

	// Right ���� ��� (ī�޶��� ������ ����)
	glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
	// Up ���� ��� (ī�޶��� �� ����)
	up = glm::normalize(glm::cross(right, front));
}

void Camera::lockMouse() {
	if (isMouseLocked) {
		ignoreMouseEvent = true;
	}

	glutWarpPointer(SCREENWIDTH / 2, SCREENHEIGHT / 2); // Ŀ���� ȭ�� �߾����� �̵�
}

void Camera::setIsMouseLocked(bool temp) {
	isMouseLocked = temp;
}

void Camera::mouseCallback(GLfloat xPos, GLfloat yPos)
{
	if (isMouseLocked) {
		glutSetCursor(GLUT_CURSOR_NONE);

		// ȭ�� �߾��� �������� �̵��� ���
		float xoffset = xPos - (SCREENWIDTH / 2);
		float yoffset = (SCREENHEIGHT / 2) - yPos; // ȭ���� Y���� �Ʒ��� ����

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		// Yaw�� �¿� ȸ��, Pitch�� ��/�Ʒ� ȸ��
		yaw += xoffset;  // yaw: �¿� ȸ�� (���� ȸ��)
		pitch += yoffset; // pitch: ��/�Ʒ� ȸ�� (���� ȸ��)

		std::cout << yaw << ", " << pitch << std::endl;

		// Pitch ���� (���Ʒ� ȸ�� ����)
		if (pitch > 80.0f)
			pitch = 80.0f;
		if (pitch < -80.0f)
			pitch = -80.0f;
		// ���� ���� ����
		calculateCameraFront();

		// ���콺�� �ٽ� ȭ�� �߾����� ����
		lockMouse(); // lockMouse()�� glutWarpPointer ȣ��
	}
}