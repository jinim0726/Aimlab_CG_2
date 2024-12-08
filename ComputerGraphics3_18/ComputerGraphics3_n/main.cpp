#define _CRT_SECURE_NO_WARNINGS //--- ���α׷� �� �տ� ������ ��
#include <iostream>
#include <vector>
#include "random.h"
#include "shaderSourceCode.h"
#include "crosshair.h"	//--- ũ�ν���� ��� �߰�
#include "pointcrosshair.h"
#include "emptycrosshair.h"

#define SCREENWIDTH 1920
#define SCREENHEIGHT 1080

// ����� ���� �Լ�
GLvoid Reshape(int w, int h); //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
GLvoid drawScene(GLvoid);
void Keyboard(unsigned char, int, int); // Ű �Է� �޴� �Լ�
void SpecialKeyboard(int key, int x, int y);
void TimerFunction(int); // Ÿ�̸� �Լ�
void Mouse(int, int, int, int);
void PassiveMotion(int x, int y);
void Motion(int x, int y);

// ����� ���� ���� ( �ʼ� )
GLuint shaderProgramID; // ���̴� ���α׷� �̸�
GLuint vertexShader; // ���ؽ� ���̴� ��ü
GLuint fragmentShader; // �����׸�Ʈ ���̴� ��ü
shape::Camera camera;
bool setTimer = true;
bool isLeftButtonClick;
bool isRightButtonClick;
GLuint vao, vbo[2], ebo;

// ����� ���� ���� ( �ʼ��ƴ� )
shape::DefaultShape line_x{};
shape::DefaultShape line_y{};
shape::DefaultShape line_z{};
shape::DefaultShape line_camera{};
std::vector<shape::Cube*> cubes;
bool isDepth = true;
bool isOrtho = false;
bool isSolid{};

//--- ũ�ν���� ��ü ����
Crosshair crosshair;	
PointCrosshair pointCrosshair(glm::vec3(0.0f, 1.0f, 0.0f), 5.0f); // ���, ũ�� 5.0
EmptyCrosshair emptyCrosshair(glm::vec3(0.0f, 1.0f, 0.0f), 0.02f, 0.005f);
int crosshairType = 1;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(SCREENWIDTH, SCREENHEIGHT); // �������� ũ�� ����
	glutCreateWindow("Example1"); // ������ ���� (������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	// ���� ����
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	// ���̴� �о�ͼ� ���̴� ���α׷� �����
	make_shaderProgram(vertexShader, fragmentShader, shaderProgramID);
	glPointSize(1.f);
	glLineWidth(5.f);

	// ���ؼ� �ʱ�ȭ
	crosshair = Crosshair(glm::vec3(1.0f, 1.0f, 1.0f), 0.3f); 
	crosshair.Init(shaderProgramID); 
	emptyCrosshair.Init(shaderProgramID);
	//-------------
	pointCrosshair.Init(shaderProgramID);

	camera.lockMouse();

	line_x.SetVertex(glm::vec3(1.0f, 0.0f, 0.0f), 0);
	line_x.SetVertex(glm::vec3(-1.0f, 0.0f, 0.0f), 1);
	line_y.SetVertex(glm::vec3(0.0f, 1.0f, 0.0f), 0);
	line_y.SetVertex(glm::vec3(0.0f, -1.0f, 0.0f), 1);
	line_z.SetVertex(glm::vec3(0.0f, 0.0f, 1.0f), 0);
	line_z.SetVertex(glm::vec3(0.0f, 0.0f, -1.0f), 1);
	line_camera.SetVertex(camera.front - camera.position, 0);
	line_camera.SetVertex(camera.position, 1);
	line_x.SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 0.5f), 0);
	line_x.SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 0.5f), 1);
	line_y.SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.5f), 0);
	line_y.SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.5f), 1);
	line_z.SetColor(glm::vec4(0.0f, 0.0f, 1.0f, 0.5f), 0);
	line_z.SetColor(glm::vec4(0.0f, 0.0f, 1.0f, 0.5f), 1);
	line_camera.SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0);
	line_camera.SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 1);
	line_x.cnt = 2;
	line_y.cnt = 2;
	line_z.cnt = 2;

	for (int i{}; i < 10; ++i) {
		for (int j{}; j < 10; ++j) {
			shape::Cube* cube = new shape::Cube{ "sphere.txt", 0 };
			cube->Transform_World(glm::vec3(-1.0f + (i * 0.2f), -1.0f + (j * 0.2f), 0.0f));
			cube->Scale(glm::vec3(0.02f, 0.02f, 0.02));
			cubes.push_back(cube);
		}
	}

	glutTimerFunc(60, TimerFunction, 1); // Ÿ�̸��Լ� �� ����

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutMouseFunc(Mouse);
	glutPassiveMotionFunc(PassiveMotion);
	glutMotionFunc(Motion);
	glClearColor(1.f, 1.f, 1.f, 1.f); // �������� ��backGround���� ����
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	glClearColor(1.f, 1.f, 1.f, 1.f); // �������� ��backGround���� ����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.

	// ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderProgramID);

	// �������------------------------------------------------

	// x y z ��
	DrawShape(camera, shaderProgramID, vao, vbo, line_x, isOrtho);
	DrawShape(camera, shaderProgramID, vao, vbo, line_y, isOrtho);
	DrawShape(camera, shaderProgramID, vao, vbo, line_z, isOrtho);
	DrawShape_NoneCamera(camera, shaderProgramID, vao, vbo, line_camera, isOrtho);

	for(shape::Cube* cube : cubes)
		DrawShape(camera, shaderProgramID, vao, vbo, &ebo, *cube, isOrtho);

	// ���� ���õ� ���ؼ� ������
	if (crosshairType == 1) {
		crosshair.Draw(shaderProgramID, SCREENWIDTH, SCREENHEIGHT); // �⺻ ���� ���ؼ�
	}
	else if (crosshairType == 2) {
		pointCrosshair.Draw(shaderProgramID, SCREENWIDTH, SCREENHEIGHT); // �� ���ؼ�
	}
	else if (crosshairType == 3) {
		emptyCrosshair.Draw(shaderProgramID, SCREENWIDTH, SCREENHEIGHT);

	}

	// �������-----------------------------------------------

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1': // �⺻ ���� ���ؼ�
		crosshairType = 1;
		break;
	case '2': // �� ���ؼ�
		crosshairType = 2;
		break;
	case '3': // ����� ���ؼ�
		crosshairType = 3;
		break;
	case 'i':
		camera.setIsMouseLocked(false);
		break;
	case 'I':
		camera.setIsMouseLocked(true);
		break;
	case 'a':
		camera.position.x -= 0.1f;
		camera.front.x -= 0.1f;
		camera.up.x -= 0.1f;
		break;
	case 'd':
		camera.position.x += 0.1f;
		camera.front.x += 0.1f;
		camera.up.x += 0.1f;
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}

	glutPostRedisplay();
}

void SpecialKeyboard(int key, int x, int y) {
	switch (key) {
	}
	glutPostRedisplay();
}


void Mouse(int button, int state, int x, int y) // ���콺 ��ǥ�� ���� ���� 0,0 ������ �Ʒ��� 800,800 (â ũ��)
{
	GLfloat xPos = (2.0f * x) / SCREENWIDTH - 1.0f;
	GLfloat yPos = 1.0f - (2.0f * y) / SCREENHEIGHT;

	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			isLeftButtonClick = true;
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			isRightButtonClick = true;
		}
	}
	else if (state == GLUT_UP)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			isLeftButtonClick = false;
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			isRightButtonClick = false;
		}
	}

	glutPostRedisplay();
}

void PassiveMotion(int x, int y)
{
	GLfloat xPos = (2.0f * x) / SCREENWIDTH - 1.0f;
	GLfloat yPos = 1.0f - (2.0f * y) / SCREENHEIGHT;

	camera.mouseCallback(x, y);
	line_camera.SetVertex(glm::normalize(camera.front - camera.position), 0);
	line_camera.SetVertex(camera.position, 1);

	glutPostRedisplay();
}

void Motion(int x, int y)
{
	GLfloat xPos = (2.0f * x) / SCREENWIDTH - 1.0f;	
	GLfloat yPos = 1.0f - (2.0f * y) / SCREENHEIGHT;

	camera.mouseCallback(x, y);	
	line_camera.SetVertex(glm::normalize(camera.front - camera.position), 0);
	line_camera.SetVertex(camera.position, 1);

	if (isLeftButtonClick == true)
	{
	}
	else if (isRightButtonClick == true)
	{
	}

	glutPostRedisplay();
}

void TimerFunction(int value)
{
	if (setTimer)
	{
		switch (value) {
		case 1:
			break;
		}
		glutPostRedisplay(); // ȭ�� �� ���
		glutTimerFunc(60, TimerFunction, 1); // Ÿ�̸��Լ� �� ����
	}
}