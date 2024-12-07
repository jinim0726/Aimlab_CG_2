#define _CRT_SECURE_NO_WARNINGS //--- ���α׷� �� �տ� ������ ��
#include <iostream>
#include <print>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shaderSourceCode.h"

#define SCREENWIDTH 1980
#define SCREENHEIGHT 1080
#define MAX_LINE_LENGTH 256

using namespace shape;

char* filetobuf(const char* file) {
	FILE* fptr;
	long length;
	char* buf;

	fptr = fopen(file, "rb"); // Open file for reading
	if (!fptr) // Return NULL on failure
		return NULL;

	// Seek to the end of the file and get the length
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);

	// Handle empty or invalid file
	if (length < 0) {
		fclose(fptr);
		return NULL; // Return NULL for negative length
	}

	// Allocate buffer for the entire file plus a null terminator
	buf = (char*)malloc(length + 1);
	if (!buf) {
		fclose(fptr);
		return NULL; // Handle memory allocation failure
	}

	// Go back to the beginning of the file
	fseek(fptr, 0, SEEK_SET);

	// Read the contents of the file into the buffer
	size_t result = fread(buf, 1, length, fptr);
	if (result != length) {
		free(buf); // Free allocated buffer
		fclose(fptr); // Close the file
		return NULL; // Return NULL on read error
	}

	fclose(fptr); // Close the file
	buf[length] = '\0'; // Null terminator
	return buf; // Return the buffer
}

void free_model(shape::Model* model) {
	free(model->vertices);
	free(model->normals);
	free(model->faces);
	model->vertices = NULL;
	model->normals = NULL;
	model->faces = NULL;
	model->vertex_count = 0;
	model->normal_count = 0;
	model->face_count = 0;
}

void read_obj_file(const char* filename, shape::Model* model) {
	FILE* file = fopen(filename, "r");
	if (!file) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << filename << " �ε� ����" << std::endl;
	}

	char line[MAX_LINE_LENGTH];
	model->vertex_count = 0;
	model->normal_count = 0;
	model->face_count = 0;

	// 1st pass: count vertices, normals, and faces
	while (fgets(line, sizeof(line), file)) {
		if (line[0] == 'v' && line[1] == ' ')
			model->vertex_count++;
		else if (line[0] == 'v' && line[1] == 'n')
			model->normal_count++;
		else if (line[0] == 'f' && line[1] == ' ')
			model->face_count++;
	}


	// Allocate memory
	if (model->vertex_count > 0) {
		std::cout << "���ؽ� ���� : " << model->vertex_count << std::endl;
		model->vertices = (Vertex*)malloc(model->vertex_count * sizeof(Vertex));
	}
	if (model->normal_count > 0) {
		std::cout << "��� ���� : " << model->normal_count << std::endl;
		model->normals = (Vertex*)malloc(model->normal_count * sizeof(Vertex));
	}
	if (model->face_count > 0) {
		std::cout << "�� ���� ���� : " << model->face_count << std::endl;
		model->faces = (Face*)malloc(model->face_count * sizeof(Face));
	}
	if(!model->vertices || !model->normals || !model->faces) {
		perror("Error allocating memory");
	}

	// 2nd pass: parse vertices, normals, and faces
	fseek(file, 0, SEEK_SET);
	size_t vertex_index = 0, normal_index = 0, face_index = 0;
	while (fgets(line, sizeof(line), file)) {
		if (line[0] == 'v' && line[1] == ' ') {
			if (sscanf(line + 2, "%f %f %f", &model->vertices[vertex_index].x,
				&model->vertices[vertex_index].y,
				&model->vertices[vertex_index].z) == 3) {
				std::println("vertex {}: {} {} {}", vertex_index, model->vertices[vertex_index].x, model->vertices[vertex_index].y, model->vertices[vertex_index].z);
				vertex_index++;
			}
		}
		else if (line[0] == 'v' && line[1] == 'n') {
			if (sscanf(line + 3, "%f %f %f", &model->normals[normal_index].x,
				&model->normals[normal_index].y,
				&model->normals[normal_index].z) == 3) {
				normal_index++;
			}
		}
		else if (line[0] == 'f' && line[1] == ' ') {
			unsigned int v1, v2, v3, n1, n2, n3, t1, t2, t3;
			if (sscanf(line + 2, "%u//%u %u//%u %u//%u", &v1, &n1, &v2, &n2, &v3, &n3) == 6) {
				std::println("face {} : {} {} {}", face_index, v1, v2, v3);
				model->faces[face_index].v1 = v1 - 1;  // OBJ indices are 1-based, subtract 1
				model->faces[face_index].v2 = v2 - 1;
				model->faces[face_index].v3 = v3 - 1;
				//model->faces[face_index].n1 = n1 - 1;
				//model->faces[face_index].n2 = n2 - 1;
				//model->faces[face_index].n3 = n3 - 1;
				face_index++;
			}
			else if (sscanf(line + 2, "%u %u %u", &v1, &v2, &v3) == 3) {
				std::println("face {} : {} {} {}", face_index, v1, v2, v3);
				model->faces[face_index].v1 = v1 - 1;  // OBJ indices are 1-based, subtract 1
				model->faces[face_index].v2 = v2 - 1;
				model->faces[face_index].v3 = v3 - 1;
				//model->faces[face_index].n1 = n1 - 1;
				//model->faces[face_index].n2 = n2 - 1;
				//model->faces[face_index].n3 = n3 - 1;
				face_index++;
			}
			else if (sscanf(line + 2, "%u/%u/%u %u/%u/%u %u/%u/%u", &v1, &n1, &t1, &v2, &n2, &t2, &v3, &n3, &t3) == 9) {
				std::println("face {} : {} {} {}", face_index, v1, v2, v3);
				model->faces[face_index].v1 = v1 - 1;  // OBJ indices are 1-based, subtract 1
				model->faces[face_index].v2 = v2 - 1;
				model->faces[face_index].v3 = v3 - 1;
				//model->faces[face_index].n1 = n1 - 1;
				//model->faces[face_index].n2 = n2 - 1;
				//model->faces[face_index].n3 = n3 - 1;
				face_index++;
			}
		}
	}

	fclose(file);
}

void make_vertexShaders(GLuint& vertexShader) // ���ؽ� ���̴� ��ü)
{
	GLchar* vertexSource;
	//--- ���ؽ� ���̴� �о� �����ϰ� ������ �ϱ�
	//--- filetobuf: ��������� �Լ��� �ؽ�Ʈ�� �о ���ڿ��� �����ϴ� �Լ�
	vertexSource = filetobuf("vertex.glsl");

	// ���ؽ� ���̴� ��ü �����
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, NULL);

	// ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexShader);

	// ����üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders(GLuint& fragmentShader)
{
	GLchar* fragmentSource;

	fragmentSource = filetobuf("fragment.glsl"); // �����׼��̴� �о����

	//--- �����׸�Ʈ ���̴� �о� �����ϰ� �������ϱ�

	// �����׸�Ʈ ���̴� ��ü �����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, NULL);

	// �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentShader);

	// ����üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: frag_shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void make_shaderProgram(GLuint& vertexShader, GLuint& fragmentShader, GLuint& shaderID)
{
	make_vertexShaders(vertexShader);
	make_fragmentShaders(fragmentShader);

	shaderID = glCreateProgram(); //--- ���̴� ���α׷� �����

	glAttachShader(shaderID, vertexShader); //--- ���̴� ���α׷��� ���ؽ� ���̴� ���̱�
	glAttachShader(shaderID, fragmentShader); //--- ���̴� ���α׷��� �����׸�Ʈ ���̴� ���̱�

	glLinkProgram(shaderID); //--- ���̴� ���α׷� ��ũ�ϱ�

	glDeleteShader(vertexShader); //--- ���̴� ��ü�� ���̴� ���α׷��� ��ũ��������, ���̴� ��ü ��ü�� ���� ����
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result); // ---���̴��� �� ����Ǿ����� üũ�ϱ�
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
	}

	glUseProgram(shaderID);
	//--- ������� ���̴� ���α׷� ����ϱ�
	//--- ���� ���� ���̴����α׷� ���� �� �ְ�, �� �� �Ѱ��� ���α׷��� ����Ϸ���
	//--- glUseProgram �Լ��� ȣ���Ͽ� ��� �� Ư�� ���α׷��� �����Ѵ�.
	//--- ����ϱ� ������ ȣ���� �� �ִ�.
}

void InitBuffer(GLuint& shaderID, GLuint& vao, GLuint* vbo, shape::DefaultShape defaultShape)
{
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�

	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�

	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 10, defaultShape.vertex, GL_STATIC_DRAW);
	//--- ��ǥ���� attribute �ε��� 0���� �����Ѵ�
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);
	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 10, defaultShape.color, GL_STATIC_DRAW);
	//--- ������ attribute �ε��� 1���� �����Ѵ�
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);
}

void InitBuffer(GLuint& shaderID, GLuint& vao, GLuint* vbo, GLuint* ebo, shape::Cube defaultShape)
{
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�

	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�

	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(1, ebo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Face) * defaultShape.model.face_count, defaultShape.model.faces, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * defaultShape.model.vertex_count, defaultShape.model.vertices,GL_STATIC_DRAW);
	//--- ��ǥ���� attribute �ε��� 0���� �����Ѵ�
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);
	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * defaultShape.model.vertex_count, defaultShape.color, GL_STATIC_DRAW);
	//--- ������ attribute �ε��� 1���� �����Ѵ�
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);
}

void SetCamera(shape::Camera camera, GLuint& shaderID, bool isOrtho)
{
	// �� �� �������� �ʱ�ȭ
	glm::mat4 view = glm::lookAt(
		camera.position, // ī�޶� ��ġ (x, y, z)
		camera.position + camera.front, // ī�޶� �ٶ󺸴� �� (x, y, z)
		camera.up // ���� �� ���� (���⼭�� y��)
	);

	glm::mat4 projection;

	if (isOrtho) {
		projection = glm::ortho(
			-1.0f, 1.0f, // ���ʰ� ������ ���
			-1.0f, 1.0f, // �Ʒ��� �� ���
			0.1f, 100.0f // ����� Ŭ�� ���� �� Ŭ�� ���
		);
	}
	else {
		projection = glm::perspective(
			glm::radians(camera.fov), // �þ߰� (FOV)
			(float)SCREENWIDTH / (float)SCREENHEIGHT, // ��Ⱦ��
			0.1f, // ����� Ŭ�� ���
			100.0f // �� Ŭ�� ���
		);
	}

	GLint viewLoc = glGetUniformLocation(shaderID, "view");
	GLint projectionLoc = glGetUniformLocation(shaderID, "projection");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void DrawShape(shape::Camera camera, GLuint& shaderID, GLuint& vao, GLuint* vbo, shape::DefaultShape defaultShape, bool isOrtho)
{
	InitBuffer(shaderID, vao, vbo, defaultShape);
	SetCamera(camera, shaderID, isOrtho);

	unsigned int modelLocation = glGetUniformLocation(shaderID, "modelTransform"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
	glm::mat4 temp = glm::mat4(1.0f);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(temp)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glBindVertexArray(vao);

	switch (defaultShape.cnt) {
	case 1:
		glDrawArrays(GL_POINTS, 0, 1);
		break;
	case 2:
		glDrawArrays(GL_LINES, 0, 2);
		break;
	case 3:
		glDrawArrays(GL_TRIANGLES, 0, 3);
		break;
	case 4:
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		break;
	case 5:
		glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
		break;
	case 6:
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		break;
	case 7:
		glDrawArrays(GL_TRIANGLE_FAN, 0, 7);
		break;
	case 8:
		glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
		break;
	case 9:
		glDrawArrays(GL_TRIANGLE_FAN, 0, 9);
		break;
	case 10:
		glDrawArrays(GL_TRIANGLE_FAN, 0, 10);
		break;
	}
}

void DrawShape(shape::Camera camera, GLuint& shaderID, GLuint& vao, GLuint* vbo, GLuint* ebo, shape::Cube defaultShape, bool isOrtho)
{
	SetCamera(camera, shaderID, isOrtho);

	InitBuffer(shaderID, vao, vbo, ebo, defaultShape);
	// �� �� �������� �ʱ�ȭ

	unsigned int modelLocation = glGetUniformLocation(shaderID, "modelTransform"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
	glm::mat4 temp = defaultShape.transform_change * defaultShape.rotation_mom * defaultShape.rotation_world * defaultShape.transform_world * defaultShape.rotation_self * defaultShape.transform_self * defaultShape.scale;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(temp)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 3 * defaultShape.model.face_count, GL_UNSIGNED_INT, 0);
}

void DrawShape_Face(shape::Camera camera, GLuint& shaderID, GLuint& vao, GLuint* vbo, GLuint* ebo, shape::Cube defaultShape, int faceNum, bool isOrtho)
{
	SetCamera(camera, shaderID, isOrtho);
	InitBuffer(shaderID, vao, vbo, ebo, defaultShape);
	// �� �� �������� �ʱ�ȭ

	unsigned int modelLocation = glGetUniformLocation(shaderID, "modelTransform"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
	glm::mat4 temp = defaultShape.transform_change * defaultShape.rotation_mom * defaultShape.rotation_world * defaultShape.transform_world * defaultShape.rotation_self * defaultShape.transform_self * defaultShape.scale;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(temp)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	int faceIndex = faceNum; // 0���� �����Ͽ�, �� ��° ���� ����
	int offset = faceIndex * defaultShape.faceVertexCnt * sizeof(GLuint); // ���� �ε����� ����Ʈ ������ ���

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, defaultShape.faceVertexCnt, GL_UNSIGNED_INT, (void*)offset);
}

void DrawShape_Line(shape::Camera camera, GLuint& shaderID, GLuint& vao, GLuint* vbo, GLuint* ebo, shape::Cube defaultShape, bool isOrtho)
{
	SetCamera(camera, shaderID, isOrtho);

	InitBuffer(shaderID, vao, vbo, ebo, defaultShape);
	// �� �� �������� �ʱ�ȭ

	unsigned int modelLocation = glGetUniformLocation(shaderID, "modelTransform"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
	glm::mat4 temp = defaultShape.transform_change * defaultShape.rotation_mom * defaultShape.rotation_world * 
		defaultShape.transform_world * defaultShape.rotation_self * defaultShape.transform_self * defaultShape.scale;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(temp)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glBindVertexArray(vao);
	glDrawElements(GL_LINE_STRIP, 3 * defaultShape.model.face_count, GL_UNSIGNED_INT, 0);
}

void DrawShape_Face_Line(shape::Camera camera, GLuint& shaderID, GLuint& vao, GLuint* vbo, GLuint* ebo, shape::Cube defaultShape, int faceNum, bool isOrtho)
{
	SetCamera(camera, shaderID, isOrtho);

	InitBuffer(shaderID, vao, vbo, ebo, defaultShape);
	// �� �� �������� �ʱ�ȭ

	unsigned int modelLocation = glGetUniformLocation(shaderID, "modelTransform"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
	glm::mat4 temp = defaultShape.transform_change * defaultShape.rotation_mom * defaultShape.rotation_world * 
		defaultShape.transform_world * defaultShape.rotation_self * defaultShape.transform_self * defaultShape.scale;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(temp)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	int faceIndex = faceNum; // 0���� �����Ͽ�, �� ��° ���� ����
	int offset = faceIndex * defaultShape.faceVertexCnt * sizeof(GLuint); // ���� �ε����� ����Ʈ ������ ���

	glBindVertexArray(vao);
	glDrawElements(GL_LINE_STRIP, defaultShape.faceVertexCnt, GL_UNSIGNED_INT, (void*)offset);
}

void DrawShape_NoneCamera(shape::Camera camera, GLuint& shaderID, GLuint& vao, GLuint* vbo, shape::DefaultShape defaultShape, bool isOrtho)
{
	InitBuffer(shaderID, vao, vbo, defaultShape);

	unsigned int modelLocation = glGetUniformLocation(shaderID, "modelTransform"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
	glm::mat4 temp = glm::mat4(1.0f);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(temp)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glBindVertexArray(vao);

	switch (defaultShape.cnt) {
	case 1:
		glDrawArrays(GL_POINTS, 0, 1);
		break;
	case 2:
		glDrawArrays(GL_LINES, 0, 2);
		break;
	case 3:
		glDrawArrays(GL_TRIANGLES, 0, 3);
		break;
	case 4:
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		break;
	case 5:
		glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
		break;
	case 6:
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		break;
	case 7:
		glDrawArrays(GL_TRIANGLE_FAN, 0, 7);
		break;
	case 8:
		glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
		break;
	case 9:
		glDrawArrays(GL_TRIANGLE_FAN, 0, 9);
		break;
	case 10:
		glDrawArrays(GL_TRIANGLE_FAN, 0, 10);
		break;
	}
}