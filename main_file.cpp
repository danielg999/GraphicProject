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
#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include <iostream>
#include "lodepng.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <models.h>
#include <models.cpp>
#include <glm/gtx/io.hpp>
#include "glm/gtx/string_cast.hpp"
#include <time.h> 
#include <vector>
#include <cmath>


float aspectRatio=1;

ShaderProgram *sp, *sp2;

std::vector<glm::vec4> verts;
std::vector<glm::vec4> norms;
std::vector<glm::vec2> textureCoords;
std::vector<unsigned int> indices;

Model floorModel = Model();
Model headSnakeModel = Model();
Model bodySnakeModel = Model();
Model appleModel = Model();
Model wallModel = Model();
Model cubeInfoModel = Model();
Model baseModel = Model();



GLuint Tex, Tex2, Tex3, Tex4, Tex5, Tex6;

float speed_x = 0; //[radiany/s]
float speed_z = 0; //[radiany/s]
float speed_y = 0;
float walk_speed = 0;
float pos_x = 0;
float pos_z = 0;

int leftRigtDifference = 0;
float head_direction = 0;
std::vector<glm::vec3> headSnakePosition;
glm::vec3 applePosition = glm::vec3(0, 0.35, 4);
bool startFlag = true;
int eatenApples = 0;
glm::vec3 historyHeadSnakePosition[10000];
int historySnakeDirection[10000];
int counterOfHistoryPoints = 0;
int counterToSavePosition = 0;
bool endGame = false;
float speedRateOfSnake = 2;
//Model floor;

glm::vec3 pos = glm::vec3(0, 26.83, -16.41);


glm::vec3 calcDir(float kat_x, float kat_y) {
	glm::vec4 dir = glm::vec4(0, 0, 1, 0);
	glm::mat4 M = glm::rotate(glm::mat4(1.0f), kat_y, glm::vec3(0, 1, 0));
	M = glm::rotate(M, kat_x, glm::vec3(1, 0, 0));
	dir = M * dir;
	return glm::vec3(dir);
}


void key_callback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mod
) {
	if (key == GLFW_KEY_SPACE) {
		speed_z = speedRateOfSnake;
	}
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) {
			leftRigtDifference += 1;
			if (leftRigtDifference % 4 == 1 || leftRigtDifference % 4 == -3) {
				speed_x = speedRateOfSnake;
				speed_z = 0;
			}
			else if (leftRigtDifference % 4 == -1 || leftRigtDifference % 4 == 3) {
				speed_x = -speedRateOfSnake;
				speed_z = 0;
			}
			else if (leftRigtDifference % 4 == 0) {
				speed_x = 0;
				speed_z = speedRateOfSnake;
			}
			else if (leftRigtDifference % 4 == 2 || leftRigtDifference % 4 == -2) {
				speed_x = 0;
				speed_z = -speedRateOfSnake;
			}
		}
		if (key == GLFW_KEY_RIGHT) {
			leftRigtDifference -= 1;
			if (leftRigtDifference % 4 == -1 || leftRigtDifference % 4 == 3) {
				speed_x = -speedRateOfSnake;
				speed_z = 0;
			}
			else if (leftRigtDifference % 4 == -3 || leftRigtDifference % 4 == 1) {
				speed_x = speedRateOfSnake;
				speed_z = 0;
			}
			else if (leftRigtDifference % 4 == 0) {
				speed_x = 0;
				speed_z = speedRateOfSnake;
			}
			else if (leftRigtDifference % 4 == 2 || leftRigtDifference % 4 == -2) {
				speed_x = 0;
				speed_z = -speedRateOfSnake;
			}
		}
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT) {
			if (leftRigtDifference % 4 == 1 || leftRigtDifference % 4 == -3) {
				speed_x = speedRateOfSnake;
				speed_z = 0;
			}
			else if (leftRigtDifference % 4 == -1 || leftRigtDifference % 4 == 3) {
				speed_x = -speedRateOfSnake;
				speed_z = 0;
			}
			else if (leftRigtDifference % 4 == 0) {
				speed_x = 0;
				speed_z = speedRateOfSnake;
			}
			else if (leftRigtDifference % 4 == 2 || leftRigtDifference % 4 == -2) {
				speed_x = 0;
				speed_z = -speedRateOfSnake;
			}
		}
		if (key == GLFW_KEY_RIGHT) {
			if (leftRigtDifference % 4 == -1) {
				speed_x = -speedRateOfSnake;
				speed_z = 0;
			}
			else if (leftRigtDifference % 4 == -3 || leftRigtDifference % 4 == 1) {
				speed_x = speedRateOfSnake;
				speed_z = 0;
			}
			else if (leftRigtDifference % 4 == 0) {
				speed_x = 0;
				speed_z = speedRateOfSnake;
			}
			else if (leftRigtDifference % 4 == 2 || leftRigtDifference % 4 == -2) {
				speed_x = 0;
				speed_z = -speedRateOfSnake;
			}
		}
		
	}
}

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void windowResizeCallback(GLFWwindow* window,int width,int height) {
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0.5,1,0.5,1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetKeyCallback(window, key_callback);

	sp=new ShaderProgram("v_simplest.glsl",NULL,"f_simplest.glsl");
	sp2 = new ShaderProgram("v_cube.glsl", NULL, "f_cube.glsl");
	Tex = readTexture("grass.png");
	Tex2 = readTexture("bricks3b_specular.png");
	Tex3 = readTexture("appleTex.png");
	Tex4 = readTexture("stone-wall.png");
	Tex5 = readTexture("gameOver.png");
	Tex6 = readTexture("forest_top_down.png");

	baseModel.loadModel("floor.obj");
	baseModel.sp = sp2;
	baseModel.Tex = Tex6;

	floorModel.loadModel("floor.obj");
	floorModel.sp = sp;
	floorModel.Tex = Tex;

	wallModel.loadModel("walls.obj");
	wallModel.sp = sp;
	wallModel.Tex = Tex4;

	headSnakeModel.loadModel("snake_head4.obj");
	headSnakeModel.sp = sp;
	headSnakeModel.Tex = Tex2;

	appleModel.loadModel("Apple.obj");
	appleModel.sp = sp;
	appleModel.Tex = Tex3;
	
	bodySnakeModel.loadModel("bodySnake2.obj");
	bodySnakeModel.sp = sp;
	bodySnakeModel.Tex = Tex2;
	
	cubeInfoModel.loadModel("kostka.obj");
	cubeInfoModel.sp = sp2;
	cubeInfoModel.Tex = Tex5;
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
    delete sp;
	delete sp2;
}

bool isEatApple(glm::vec3 applePosition) {
	bool eaten = false;
	float eatRangeX = 0.6f, eatRangeZ = 1.0f;;
	if (((pos_x >= 0 && applePosition.x >= 0) || (pos_x <= 0 && applePosition.x <= 0)) && abs(pos_x - applePosition.x) < eatRangeX) {
		if (((pos_z >= 0 && applePosition.z >= 0) || (pos_z >= 0 && applePosition.z <= 0) || (pos_z <= 0 && applePosition.z <= 0 && abs(pos_z - applePosition.z))) && abs(pos_z - applePosition.z) < eatRangeZ) {
			eaten = true;
		}
		else if (pos_z <= 0 && applePosition.z >= 0 && abs(applePosition.z - pos_z) < eatRangeZ) {
			eaten = true;
		}
	}
	else if (((pos_x >= 0 && applePosition.x <= 0 && abs(pos_x - applePosition.x) < eatRangeX) || (pos_x <= 0 && applePosition.x >= 0 && abs(applePosition.x - pos_x) < eatRangeX)) ) {
		if (((pos_z >= 0 && applePosition.z >= 0) || (pos_z >= 0 && applePosition.z <= 0) || (pos_z <= 0 && applePosition.z <= 0 && abs(pos_z - applePosition.z))) && abs(pos_z - applePosition.z) < eatRangeZ) {
			eaten = true;
		}
		else if (pos_z <= 0 && applePosition.z >= 0 && abs(applePosition.z - pos_z) < eatRangeZ) {
			eaten = true;
		}
	}

	if (eaten)
		eatenApples++;
	return eaten;
}

glm::vec3 randApplePosition() {
	double appleX, appleZ;
	float appleRangeX = 0.6f, appleRangeZ = 0.6f;
	int tempPointsAmount = counterOfHistoryPoints;
	bool isCorrectPosition;
	srand(time(NULL));
	do {
		isCorrectPosition = false;
		appleX = (double)rand() / (RAND_MAX + 1) - 9 + (rand() % 19);
		appleZ = (double)rand() / (RAND_MAX + 1) - 9 + (rand() % 19);
		for (int i = eatenApples, j = tempPointsAmount; (tempPointsAmount - i) <= j && 0 < j; j--) {

			if (((appleX >= 0 && historyHeadSnakePosition[j].x >= 0) || (appleX <= 0 && historyHeadSnakePosition[j].x <= 0)) && abs(appleX - historyHeadSnakePosition[j].x) < appleRangeX) {
				if (((appleZ >= 0 && historyHeadSnakePosition[j].z >= 0) || (appleZ >= 0 && historyHeadSnakePosition[j].z <= 0) || (appleZ <= 0 && historyHeadSnakePosition[j].z <= 0 && abs(appleZ - historyHeadSnakePosition[j].z))) && abs(appleZ - historyHeadSnakePosition[j].z) < appleRangeZ) {
					isCorrectPosition = true;
					break;
				}
				else if (appleZ <= 0 && historyHeadSnakePosition[j].z >= 0 && abs(historyHeadSnakePosition[j].z - appleZ) < appleRangeZ) {
					isCorrectPosition = true;
					break;
				}
			}
			else if (((appleX >= 0 && historyHeadSnakePosition[j].x <= 0 && abs(appleX - historyHeadSnakePosition[j].x) < appleRangeX) || (appleX <= 0 && historyHeadSnakePosition[j].x >= 0 && abs(historyHeadSnakePosition[j].x - appleX) < appleRangeX))) {
				if (((appleZ >= 0 && historyHeadSnakePosition[j].z >= 0) || (appleZ >= 0 && historyHeadSnakePosition[j].z <= 0) || (appleZ <= 0 && historyHeadSnakePosition[j].z <= 0 && abs(appleZ - historyHeadSnakePosition[j].z))) && abs(appleZ - historyHeadSnakePosition[j].z) < appleRangeZ) {
					isCorrectPosition = true;
					break;
				}
				else if (appleZ <= 0 && historyHeadSnakePosition[j].z >= 0 && abs(historyHeadSnakePosition[j].z - appleZ) < appleRangeZ) {
					isCorrectPosition = true;
					break;
				}
			}
		}
	} while (isCorrectPosition);
	

	glm::vec3 arr = glm::vec3(appleX, 0.35, appleZ);
	return arr;
}

bool isSuicide() {
	bool suicided = false;
	float suicideRangeX = 0.3f, suicideRangeZ = 0.22f;
	int tempPointsAmount = counterOfHistoryPoints;

	for (int i = eatenApples, j = tempPointsAmount - 2; (tempPointsAmount - i) <= j && 0 < j; j--) {
		if (((pos_x >= 0 && historyHeadSnakePosition[j].x >= 0) || (pos_x <= 0 && historyHeadSnakePosition[j].x <= 0)) && abs(pos_x - historyHeadSnakePosition[j].x) < suicideRangeX) {
			if (((pos_z >= 0 && historyHeadSnakePosition[j].z >= 0) || (pos_z >= 0 && historyHeadSnakePosition[j].z <= 0) || (pos_z <= 0 && historyHeadSnakePosition[j].z <= 0 && abs(pos_z - historyHeadSnakePosition[j].z))) && abs(pos_z - historyHeadSnakePosition[j].z) < suicideRangeZ) {
				suicided = true;
				break;
			}
			else if (pos_z <= 0 && historyHeadSnakePosition[j].z >= 0 && abs(historyHeadSnakePosition[j].z - pos_z) < suicideRangeZ) {
				suicided = true;
				break;
			}
		}
		else if (((pos_x >= 0 && historyHeadSnakePosition[j].x <= 0 && abs(pos_x - historyHeadSnakePosition[j].x) < suicideRangeX) || (pos_x <= 0 && historyHeadSnakePosition[j].x >= 0 && abs(historyHeadSnakePosition[j].x - pos_x) < suicideRangeX))) {
			if (((pos_z >= 0 && historyHeadSnakePosition[j].z >= 0) || (pos_z >= 0 && historyHeadSnakePosition[j].z <= 0) || (pos_z <= 0 && historyHeadSnakePosition[j].z <= 0 && abs(pos_z - historyHeadSnakePosition[j].z))) && abs(pos_z - historyHeadSnakePosition[j].z) < suicideRangeZ) {
				suicided = true;
				break;
			}
			else if (pos_z <= 0 && historyHeadSnakePosition[j].z >= 0 && abs(historyHeadSnakePosition[j].z - pos_z) < suicideRangeZ) {
				suicided = true;
				break;
			}
		}
	}
	return suicided;
}


//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float pos_x, float pos_z) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V = glm::lookAt(pos, glm::vec3(0, 25.94, -15.95), glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku to było
    glm::mat4 P=glm::perspective(50.0f*PI/180.0f, aspectRatio, 0.01f, 50.0f); //Wylicz macierz rzutowania od tad bylo
    glm::mat4 M=glm::mat4(1.0f);
	glm::mat4 M1 = M;
	M1 = glm::translate(M1, glm::vec3(0, -5, 0));
	M1 = glm::scale(M1, glm::vec3(23, 0.2, 25));
	baseModel.drawSolid(P, V, M1);

	if (endGame) {
		glm::mat4 M7 = M;
		M7 = glm::translate(M7, glm::vec3(0, 16.2, -9.5));
		M7 = glm::scale(M7, glm::vec3(11, 5.8, 11));
		M7 = glm::rotate(M7, -25 * PI / 180, glm::vec3(1.0f, 0.0f, 0.0f));
		cubeInfoModel.drawSolid(P, V, M7);
	}

	glm::mat4 M2 = M;
	M2 = glm::translate(M2, glm::vec3(-11.7, 0, 0.3));
	M2 = glm::scale(M2, glm::vec3(1.07, 0.9, 1.1));
	wallModel.drawSolid(P, V, M2);

	glm::mat4 M3 = M;
	M3 = glm::translate(M3, glm::vec3(0, 0, 0));
	M3 = glm::scale(M3, glm::vec3(10.5, 0.4, 11.35));
	floorModel.drawSolid(P, V, M3);

	glm::vec3 headSnakePosition = glm::vec3(0 + pos_x, 0.3, 0 + pos_z);
	glm::mat4 M4 = M;


	head_direction = (leftRigtDifference * 90) % 360;
	M4 = glm::translate(M4, headSnakePosition + glm::vec3(0, 0.069, 0.012));
	M4 = glm::scale(M4, glm::vec3(0.1, 0.1, 0.1));
	M4 = glm::rotate(M4, head_direction * PI / 180, glm::vec3(0.0f, 1.0f, 0.0f));
	headSnakeModel.drawSolid(P, V, M4);


	if (isEatApple(applePosition)) {
		applePosition = randApplePosition();
	}
	if (eatenApples > 0 && isSuicide()) {
		endGame = true;
	}

	glm::mat4 M5 = M;
	M5 = glm::translate(M5, applePosition);
	M5 = glm::scale(M5, glm::vec3(0.01, 0.01, 0.01));

	appleModel.drawSolid(P, V, M5);

	counterToSavePosition++;
	if (counterToSavePosition == 17) {
		counterToSavePosition = 0;
		historyHeadSnakePosition[counterOfHistoryPoints] = headSnakePosition - glm::vec3(0, 0, -0.1); // do poprawy przy kierunku
		historySnakeDirection[counterOfHistoryPoints] = head_direction;
		counterOfHistoryPoints++;
	}

	if (counterOfHistoryPoints == 1 && eatenApples > 0) {
		glm::mat4 M6 = M4;
		M6 = glm::translate(M6, glm::vec3(0, 0, -8.5));
		M6 = glm::scale(M6, glm::vec3(4, 3.5, 3));

		bodySnakeModel.drawSolid(P, V, M6);
	}
	else if(eatenApples> 0){
		glm::mat4 MX;
		for (int i = eatenApples, j = counterOfHistoryPoints - 1; 0 < i; i--, j--) {
			MX = M;
			MX = glm::translate(MX, historyHeadSnakePosition[j]);
			MX = glm::rotate(MX, historySnakeDirection[j] * PI / 180, glm::vec3(0.0f, 1.0f, 0.0f));
			MX = glm::scale(MX, glm::vec3(0.37, 0.35, 0.3));
			bodySnakeModel.drawSolid(P, V, MX);
		}
	}

    glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}





int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1000, 1000, "Snake 3D", NULL, NULL);  

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
		pos_x += speed_x * glfwGetTime();
		pos_z += speed_z * glfwGetTime();
		pos += (float)(walk_speed * glfwGetTime());

		glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window, pos_x, pos_z); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
