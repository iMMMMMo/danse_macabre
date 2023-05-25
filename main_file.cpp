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
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"
#include "myTeapot.h"
#include "model.h"

float speed_x=0;
float speed_y=0;
float aspectRatio=1;
glm::vec4 lightPos1 = glm::vec4(-5.0f, 0.0f, 0.0f, 1.0f);
glm::vec4 lightPos2 = glm::vec4(5.0f, 0.0f, 0.0f, 1.0f);
glm::vec4 lightColor1 = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
glm::vec4 lightColor2 = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

ShaderProgram *sp;
ShaderProgram *sp_bg;
Model skeleton;

GLuint tex;

//Odkomentuj, żeby rysować kostkę
float* vertices = myCubeVertices;
float* normals = myCubeNormals;
float* texCoords = myCubeTexCoords;
float* colors = myCubeColors;
int vertexCount = myCubeVertexCount;


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


//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}


void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods) {
    if (action==GLFW_PRESS) {
        if (key==GLFW_KEY_LEFT) speed_x=-PI/2;
        if (key==GLFW_KEY_RIGHT) speed_x=PI/2;
        if (key==GLFW_KEY_UP) speed_y=PI/2;
        if (key==GLFW_KEY_DOWN) speed_y=-PI/2;
		// sterowanie swiatlami 
		if (action == GLFW_KEY_1) {
			lightColor1 = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			lightColor2 = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		}
		if (action == GLFW_KEY_2) {
			lightColor1 = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			lightColor2 = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		}
		if (action == GLFW_KEY_3) {
			lightColor1 = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
			lightColor2 = glm::vec4(0.0f, 0.5f, 0.5f, 1.0f);
		}
		if (action == GLFW_KEY_4) {
			lightColor1 = glm::vec4(0.8f, 0.2f, 0.6f, 1.0f);
			lightColor2 = glm::vec4(0.4f, 0.6f, 0.9f, 1.0f);
		}
    }
    if (action==GLFW_RELEASE) {
        if (key==GLFW_KEY_LEFT) speed_x=0;
        if (key==GLFW_KEY_RIGHT) speed_x=0;
        if (key==GLFW_KEY_UP) speed_y=0;
        if (key==GLFW_KEY_DOWN) speed_y=0;
    }
	
}

void windowResizeCallback(GLFWwindow* window,int width,int height) {
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}


//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	skeleton.loadModel("uploads_files_600310_skeleton_animated.FBX");
	tex = readTexture("Skeleton_Body_AlbedoTransparency.png");
	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetKeyCallback(window,keyCallback);

	sp=new ShaderProgram("v_simplest.glsl",NULL,"f_simplest.glsl");
	sp_bg= new ShaderProgram("v_bg.glsl", NULL, "f_bg.glsl");
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    delete sp;
}
void drawSkeleton(float angle_x, float angle_y) {
	glUniform4fv(sp->u("lightPos1"), 1, glm::value_ptr(lightPos1));
	glUniform4fv(sp->u("lightPos2"), 1, glm::value_ptr(lightPos2));
	glUniform4fv(sp->u("lightColor1"), 1, glm::value_ptr(lightColor1));
	glUniform4fv(sp->u("lightColor2"), 1, glm::value_ptr(lightColor2));

	glm::mat4 M = glm::mat4(1.0f);
	M = glm::scale(M, glm::vec3(0.3f, 0.3f, 0.3f));
	M = glm::translate(M, glm::vec3(0.0f, -30.0f, 0.0f));
	M = glm::rotate(M, AI_DEG_TO_RAD(-90), glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::rotate(M, AI_DEG_TO_RAD(180), glm::vec3(0.0f, 0.0f, 1.0f));
	M = glm::rotate(M, angle_y, glm::vec3(5.0f, 0.0f, 0.0f)); //Compute model matrix
	M = glm::rotate(M, angle_x, glm::vec3(0.0f, 0.0f, 5.0f)); //Compute model matrix
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(sp->a("vertex")); //Enable sending data to the attribute vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, skeleton.verts.data()); //Specify source of the data for the attribute vertex

	glEnableVertexAttribArray(sp->a("normal")); //Enable sending data to the attribute color
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, skeleton.norms.data()); //Specify source of the data for the attribute normal

	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, skeleton.texCoords.data());

	glUniform1i(sp->u("textureMap0"), 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glDrawElements(GL_TRIANGLES, skeleton.indices.size(), GL_UNSIGNED_INT, skeleton.indices.data()); //Draw the object


	glDisableVertexAttribArray(sp->a("vertex")); //Disable sending data to the attribute vertex
	glDisableVertexAttribArray(sp->a("normal")); //Disable sending data to the attribute normal
	glDisableVertexAttribArray(sp->a("texCoord0"));
	
}

void drawBackground(float angle_x, float angle_y) {
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(0.0f, 0.0f, 19.9f));
	M = glm::scale(M, glm::vec3(25.0f, 25.0f, 25.0f));
	
	//Przeslij parametry programu cieniującego do karty graficznej
	glUniformMatrix4fv(sp_bg->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(sp_bg->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(sp_bg->a("vertex"), 4, GL_FLOAT, false, 0, vertices); //Wskaż tablicę z danymi dla atrybutu vertex

	glDrawArrays(GL_TRIANGLES, 0, vertexCount); //Narysuj obiekt

	glDisableVertexAttribArray(sp_bg->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window,float angle_x,float angle_y) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V = glm::lookAt(
		glm::vec3(0.0f, 0.0f, -30.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)); //compute view matrix
	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, 1.0f, 1.0f, 50.0f); //compute projection matrix

	sp->use();//activate shading program
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	drawSkeleton(angle_x, angle_y);
	
	sp_bg->use();//Aktywacja programu cieniującego
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	drawBackground(angle_x, angle_y);

	glfwSwapBuffers(window); //Copy back buffer to front buffer
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
	float angle_x=0; //Aktualny kąt obrotu obiektu
	float angle_y=0; //Aktualny kąt obrotu obiektu
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
        angle_x+=speed_x*glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
        angle_y+=speed_y*glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
        glfwSetTime(0); //Zeruj timer
		drawScene(window,angle_x,angle_y); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
