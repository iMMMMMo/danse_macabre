/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "lodepng.h"
#include "shaderprogram.h"
#include "model.h"
#include "skeleton.h"
#include "skeletonmovable.h"
#include "wall.h"

float speed_x=0;
float aspectRatio=1;
glm::vec4 lightPos1 = glm::vec4(-5.0f, 0.0f, 0.0f, 1.0f);
glm::vec4 lightPos2 = glm::vec4(5.0f, 0.0f, 0.0f, 1.0f);
glm::vec4 lightColor1 = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
glm::vec4 lightColor2 = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

float playerY = 0;
float playerVelY = 0;

//Odkomentuj, żeby rysować kostkę
//float* vertices = myCubeVertices;
//float* normals = myCubeNormals;
//float* texCoords = myCubeTexCoords;
//float* colors = myCubeColors;
//int vertexCount = myCubeVertexCount;


glm::vec4 lightPos1 = glm::vec4(-60.0f, -50.0f, 20.0f, 1.0f);
glm::vec4 lightPos2 = glm::vec4(60.0f, -50.0f, 40.0f, 1.0f);
glm::vec4 lightColor1 = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
glm::vec4 lightColor2 = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

ShaderProgram *sp;
Skeleton korpus;
Skeleton czaszka;
SkeletonMovable reka_l;
SkeletonMovable reka_p;
Skeleton noga_l;
Skeleton noga_p;
Wall disco_floor;
Wall right_wall;
Wall left_wall;
Wall far_wall;

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods) {
    if (action==GLFW_PRESS) {
        if (key==GLFW_KEY_LEFT) speed_x=-3.14f/2;
        if (key==GLFW_KEY_RIGHT) speed_x=3.14f/2;
		if (key == GLFW_KEY_UP) {
			if (playerY <= 0) playerVelY = 20;
		}
		if (key == GLFW_KEY_1) {
			lightColor1 = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			lightColor2 = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		}
		if (key == GLFW_KEY_2) {
			lightColor1 = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			lightColor2 = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		}
		if (key == GLFW_KEY_3) {
			lightColor1 = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
			lightColor2 = glm::vec4(0.0f, 0.5f, 0.5f, 1.0f);
		}
		if (key == GLFW_KEY_4) {
			lightColor1 = glm::vec4(0.8f, 0.2f, 0.6f, 1.0f);
			lightColor2 = glm::vec4(0.4f, 0.6f, 0.9f, 1.0f);
		}
    }
	if (action == GLFW_REPEAT) {
		if (key == GLFW_KEY_UP) {
			if (playerY <= 0) playerVelY = 20;
		}
	}
    if (action==GLFW_RELEASE) {
        if (key==GLFW_KEY_LEFT) speed_x=0;
        if (key==GLFW_KEY_RIGHT) speed_x=0;
    }
}

void windowResizeCallback(GLFWwindow* window,int width,int height) {
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}

GLuint readTexture(const char* filename) { //Deklaracja globalna
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image; //Alokuj wektor do wczytania obrazka
	unsigned width, height; //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);
	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return tex;
}


//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);

	korpus.loadModel("korpus.fbx");
	czaszka.loadModel("czaszka.fbx");
	reka_l.loadModel("reka_l.fbx");
	reka_p.loadModel("reka_p.fbx");
	noga_l.loadModel("noga_l.fbx");
	noga_p.loadModel("noga_p.fbx");
	disco_floor.loadModel("wall.fbx");
	left_wall.loadModel("wall.fbx");
	right_wall.loadModel("wall.fbx");
	far_wall.loadModel("wall.fbx");

	reka_l.setRotationPoint(glm::vec4(6.5f, 0.0f, 59.5f, 1.0f));
	reka_p.setRotationPoint(glm::vec4(-6.5f, 0.0f, 59.5f, 1.0f));

	korpus.tex0 = readTexture("Skeleton_Body_AlbedoTransparency.png");
	czaszka.tex0 = readTexture("Skeleton_Body_AlbedoTransparency.png");
	reka_l.tex0 = readTexture("Skeleton_Body_AlbedoTransparency.png");
	reka_p.tex0 = readTexture("Skeleton_Body_AlbedoTransparency.png");
	noga_l.tex0 = readTexture("Skeleton_Body_AlbedoTransparency.png");
	noga_p.tex0 = readTexture("Skeleton_Body_AlbedoTransparency.png");
	disco_floor.tex0 = readTexture("parquet_BaseColor.png");
	left_wall.tex0 = readTexture("Square_BaseColor.png");
	right_wall.tex0 = readTexture("Square_BaseColor.png");
	far_wall.tex0 = readTexture("Square_BaseColor.png");

	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetKeyCallback(window,keyCallback);

	sp=new ShaderProgram("v_simplest.glsl",NULL,"f_simplest.glsl");
	sp_bg= new ShaderProgram("v_bg.glsl", NULL, "f_bg.glsl");
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    delete sp;
}


//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window,float angle_x) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V = glm::lookAt(
		glm::vec3(0.0f, -90.0f, 60.0f),
		glm::vec3(0.0f, 0.0f, 35.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)); //compute view matrix
	glm::mat4 P = glm::perspective(AI_DEG_TO_RAD(54), aspectRatio, 1.0f, 5000.0f); //compute projection matrix

	sp->use();//activate shading program
	//Send parameters to graphics card
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

	glUniform4fv(sp->u("lightPos1"), 1, glm::value_ptr(lightPos1));
	glUniform4fv(sp->u("lightPos2"), 1, glm::value_ptr(lightPos2));
	glUniform4fv(sp->u("lightColor1"), 1, glm::value_ptr(lightColor1));
	glUniform4fv(sp->u("lightColor2"), 1, glm::value_ptr(lightColor2));

	korpus.draw(playerY, sp);
	czaszka.draw(playerY, sp);
	reka_l.draw(playerY, angle_x, sp);
	reka_p.draw(playerY, -angle_x, sp);
	noga_l.draw(playerY, sp);
	noga_p.draw(playerY, sp);
	disco_floor.draw(glm::mat4(1), sp);

	glm::mat4 to_wall = glm::rotate(glm::mat4(1), AI_DEG_TO_RAD(90), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 to_side = glm::rotate(to_wall, AI_DEG_TO_RAD(90), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 flipped = glm::rotate(to_side, AI_DEG_TO_RAD(180), glm::vec3(1.0f, 0.0f, 0.0f));

	right_wall.draw(glm::translate(flipped, glm::vec3(0.0f, 0.0f, -50.0f)), sp);
	left_wall.draw(glm::translate(to_side, glm::vec3(0.0f, 0.0f, -50.0f)), sp);
	far_wall.draw(glm::translate(to_wall , glm::vec3(0.0f, 0.0f, -50.0f)), sp);

	glfwSwapBuffers(window); //Copy back buffer to front buffer
}

void calcJump() {
	double dt = glfwGetTime();
	playerY += playerVelY * dt;

	if (playerY < 0) {
		playerY = 0;
		playerVelY = 0;
	}

	playerVelY -= gravity * dt;
}

void calcRot() {
	if (angle_x < -AI_DEG_TO_RAD(145)) {
		if (speed_x < 0) speed_x = 0;
	}
	if (angle_x > AI_DEG_TO_RAD(25)) {
		if (speed_x > 0) speed_x = 0;
	}
	angle_x += speed_x * glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
}

int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	//Główna pętla
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		calcRot();
		calcJump();
        glfwSetTime(0); //Zeruj timer
		drawScene(window,angle_x); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
