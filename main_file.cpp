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
#include "myCube.h"
#include "myTeapot.h"
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

//#include "glm/ext.hpp"
//#include <models.h>


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


//Odkomentuj, żeby rysować czajnik
float* vertices = myTeapotVertices;
float* normals = myTeapotVertexNormals;
float* texCoords = myTeapotTexCoords;
float* colors = myTeapotColors;
int vertexCount = myTeapotVertexCount;

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

//glm::vec3 pos = glm::vec3(0, 22.2, -13.5);
//glm::vec3 pos = glm::vec3(0, 8, -13.5);
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
	/*if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_A) speed_y = 1;
		if (key == GLFW_KEY_D) speed_y = -1;
		if (key == GLFW_KEY_S) speed_x = 1;
		if (key == GLFW_KEY_W) speed_x = -1;
		if (key == GLFW_KEY_UP) walk_speed = 7;
		if (key == GLFW_KEY_DOWN) walk_speed = -7;

	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_A) speed_y = 0;
		if (key == GLFW_KEY_D) speed_y = 0;
		if (key == GLFW_KEY_S) speed_x = 0;
		if (key == GLFW_KEY_W) speed_x = 0;
		if (key == GLFW_KEY_UP) walk_speed = 0;
		if (key == GLFW_KEY_DOWN) walk_speed = 0;
	}*/
	if (key == GLFW_KEY_SPACE) {
		speed_z = speedRateOfSnake;
	}
	else if (key == GLFW_KEY_P) {
		speed_x = 0;
		speed_z = 0;
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
		//if (key == GLFW_KEY_UP) speed_z = 2;
		//if (key == GLFW_KEY_DOWN) speed_z = -2;
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
	//std::cout << "leftRigthDifference: " << leftRigtDifference << std::endl;
}

void loadModel(std::string plik) {
	using namespace std;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(plik,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals
	);
	std::cout << importer.GetErrorString() << std::endl;

	aiMesh* mesh = scene->mMeshes[0];
	for (int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D vertex = mesh->mVertices[i];
		verts.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1));

		aiVector3D normal = mesh->mNormals[i];
		norms.push_back(glm::vec4(normal.x, normal.y, normal.z, 0));

		aiVector3D texCoord = mesh->mTextureCoords[0][i];
		textureCoords.push_back(glm::vec2(texCoord.x, texCoord.y));

	}

	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace& face = mesh->mFaces[i];

		for (int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
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


/*void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action==GLFW_PRESS) {
        if (key==GLFW_KEY_LEFT) speed_x=-PI/2;
        if (key==GLFW_KEY_RIGHT) speed_x=PI/2;
        if (key==GLFW_KEY_UP) speed_y=PI/2;
        if (key==GLFW_KEY_DOWN) speed_y=-PI/2;
    }
    if (action==GLFW_RELEASE) {
        if (key==GLFW_KEY_LEFT) speed_x=0;
        if (key==GLFW_KEY_RIGHT) speed_x=0;
        if (key==GLFW_KEY_UP) speed_y=0;
        if (key==GLFW_KEY_DOWN) speed_y=0;
    }
}*/

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
	Tex5 = readTexture("gameOver2.png");
	Tex6 = readTexture("forest_top_down.png");
	
	//loadModel("object.obj"); //Wywołanie funkcji ładującej plik anvil.fbx
	//Model floor("floor.obj");
	//floor = floor.loadModel("snake_head.obj");
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
	
	//loadModel("snake_head.obj");
	//loadModel("Apple.obj");


}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
	//delete historySnakeDirection;
	//delete historyHeadSnakePosition;
    delete sp;
	delete sp2;
}

/*void drawSolid(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
	//M = glm::rotate(M, angle_y, glm::vec3(1.0f, 0.0f, 0.0f)); //Wylicz macierz modelu
	//M = glm::rotate(M, angle_x, glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz modelu

	//M = glm::scale(M, glm::vec3(0.02, 0.05, 0.03));

	sp->use();//Aktywacja programu cieniującego
	//Przeslij parametry programu cieniującego do karty graficznej
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glm::vec4 light = glm::vec4(-13.649, 16.3, -12.53, 1);
	//glm::vec4 light = glm::vec4(0, 21, -13, 1);
	glm::vec4 light2 = glm::vec4(13, 11, 12, 1);
	//glm::vec4 light2 = glm::vec4(1, 10, 5, 1);

	glUniform4fv(sp->u("lp"), 1, glm::value_ptr(light));
	glUniform4fv(sp->u("lp2"), 1, glm::value_ptr(light2));

	glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verts.data()); //Wskaż tablicę z danymi dla atrybutu vertex

	//glEnableVertexAttribArray(sp->a("color"));  //Włącz przesyłanie danych do atrybutu color
	//glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors); //Wskaż tablicę z danymi dla atrybutu color

	glEnableVertexAttribArray(sp->a("normal"));  //Włącz przesyłanie danych do atrybutu normal
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, norms.data()); //Wskaż tablicę z danymi dla atrybutu normal

	glEnableVertexAttribArray(sp->a("texCoord0"));  //Włącz przesyłanie danych do atrybutu normal
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, textureCoords.data()); //Wskaż tablicę z danymi dla atrybutu normal

	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Tex);

	//glDrawArrays(GL_TRIANGLES,0,vertexCount); //Narysuj obiekt
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

	glDisableVertexAttribArray(sp->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("texCoord0"));  //Wyłącz przesyłanie danych do atrybutu color
	glDisableVertexAttribArray(sp->a("normal"));  //Wyłącz przesyłanie danych do atrybutu normal
}*/

bool isEatApple(glm::vec3 applePosition) {
	bool eaten = false;
	float eatRangeX = 0.6f, eatRangeZ = 1.0f;;
	//std::cout << "Odlegloasc: " << applePosition << ", " << headSnakePosition << std::endl;
	//auto n = str(applePosition.begin(), applePosition.end());
	if (((pos_x >= 0 && applePosition.x >= 0) || (pos_x <= 0 && applePosition.x <= 0)) && abs(pos_x - applePosition.x) < eatRangeX) {
		if (((pos_z >= 0 && applePosition.z >= 0) || (pos_z >= 0 && applePosition.z <= 0) || (pos_z <= 0 && applePosition.z <= 0 && abs(pos_z - applePosition.z))) && abs(pos_z - applePosition.z) < eatRangeZ) {
			//std::cout << "Odleglosc: " << pos_x << ", " << pos_z << std::endl;
			//std::cout << "applePosition.x: " << applePosition.x << ", applePosition.z: " << applePosition.z << std::endl;
			eaten = true;
		}

		else if (pos_z <= 0 && applePosition.z >= 0 && abs(applePosition.z - pos_z) < eatRangeZ) {
			//std::cout << "Odleglosc: " << pos_x << ", " << pos_z << std::endl;
			//std::cout << "applePosition.x: " << applePosition.x << ", applePosition.z: " << applePosition.z << std::endl;
			eaten = true;
		}
	}
	else if (((pos_x >= 0 && applePosition.x <= 0 && abs(pos_x - applePosition.x) < eatRangeX) || (pos_x <= 0 && applePosition.x >= 0 && abs(applePosition.x - pos_x) < eatRangeX)) ) {
		if (((pos_z >= 0 && applePosition.z >= 0) || (pos_z >= 0 && applePosition.z <= 0) || (pos_z <= 0 && applePosition.z <= 0 && abs(pos_z - applePosition.z))) && abs(pos_z - applePosition.z) < eatRangeZ) {
			//std::cout << "Odleglosc: " << pos_x << ", " << pos_z << std::endl;
			//std::cout << "applePosition.x: " << applePosition.x << ", applePosition.z: " << applePosition.z << std::endl;
			eaten = true;
		}

		else if (pos_z <= 0 && applePosition.z >= 0 && abs(applePosition.z - pos_z) < eatRangeZ) {
			//std::cout << "Odleglosc: " << pos_x << ", " << pos_z << std::endl;
			//std::cout << "applePosition.x: " << applePosition.x << ", applePosition.z: " << applePosition.z << std::endl;
			eaten = true;
		}
	}
	//std::cout << "Odleglosc: " << pos_x << ", " << pos_z << std::endl;
	//std::cout << "applePosition.x: " << applePosition.x << ", applePosition.z: " << applePosition.z << std::endl;
	if (eaten)
		eatenApples++;
	return eaten;
}

glm::vec3 randApplePosition() {
	double appleX, appleZ;
	float appleRangeX = 0.6f, appleRangeZ = 0.6f;
	int tempPointsAmount = counterOfHistoryPoints;
	bool isCorrectPosition;
	do {
		isCorrectPosition = false;
		srand(time(NULL));
		appleX = (double)rand() / (RAND_MAX + 1) - 9 + (rand() % 19);

		appleZ = (double)rand() / (RAND_MAX + 1) - 9 + (rand() % 19);

		for (int i = eatenApples, j = tempPointsAmount; (tempPointsAmount - i) <= j && 0 < j; j--) {
			//std::cout << "Odleglosc: " << pos_x << ", " << pos_z << std::endl;
			//std::cout << "historyHeadSnakePosition.x: " << historyHeadSnakePosition[j].x << ", historyHeadSnakePosition.z: " << historyHeadSnakePosition[j].z << std::endl;

			if (((appleX >= 0 && historyHeadSnakePosition[j].x >= 0) || (appleX <= 0 && historyHeadSnakePosition[j].x <= 0)) && abs(appleX - historyHeadSnakePosition[j].x) < appleRangeX) {
				if (((appleZ >= 0 && historyHeadSnakePosition[j].z >= 0) || (appleZ >= 0 && historyHeadSnakePosition[j].z <= 0) || (appleZ <= 0 && historyHeadSnakePosition[j].z <= 0 && abs(appleZ - historyHeadSnakePosition[j].z))) && abs(appleZ - historyHeadSnakePosition[j].z) < appleRangeZ) {
					std::cout << "Odleglosc: " << appleX << ", " << appleZ << std::endl;
					std::cout << "historyHeadSnakePosition.x: " << historyHeadSnakePosition[j].x << ", historyHeadSnakePosition.z: " << historyHeadSnakePosition[j].z << std::endl;
					isCorrectPosition = true;
					break;
				}

				else if (appleZ <= 0 && historyHeadSnakePosition[j].z >= 0 && abs(historyHeadSnakePosition[j].z - appleZ) < appleRangeZ) {
					std::cout << "Odleglosc: " << appleX << ", " << appleZ << std::endl;
					std::cout << "historyHeadSnakePosition.x: " << historyHeadSnakePosition[j].x << ", historyHeadSnakePosition.z: " << historyHeadSnakePosition[j].z << std::endl;
					isCorrectPosition = true;
					break;
				}
			}
			else if (((appleX >= 0 && historyHeadSnakePosition[j].x <= 0 && abs(appleX - historyHeadSnakePosition[j].x) < appleRangeX) || (appleX <= 0 && historyHeadSnakePosition[j].x >= 0 && abs(historyHeadSnakePosition[j].x - appleX) < appleRangeX))) {
				if (((appleZ >= 0 && historyHeadSnakePosition[j].z >= 0) || (appleZ >= 0 && historyHeadSnakePosition[j].z <= 0) || (appleZ <= 0 && historyHeadSnakePosition[j].z <= 0 && abs(appleZ - historyHeadSnakePosition[j].z))) && abs(appleZ - historyHeadSnakePosition[j].z) < appleRangeZ) {
					std::cout << "Odleglosc: " << appleX << ", " << appleZ << std::endl;
					std::cout << "historyHeadSnakePosition.x: " << historyHeadSnakePosition[j].x << ", historyHeadSnakePosition.z: " << historyHeadSnakePosition[j].z << std::endl;
					isCorrectPosition = true;
					break;
				}

				else if (appleZ <= 0 && historyHeadSnakePosition[j].z >= 0 && abs(historyHeadSnakePosition[j].z - appleZ) < appleRangeZ) {
					std::cout << "Odleglosc: " << appleX << ", " << appleZ << std::endl;
					std::cout << "historyHeadSnakePosition.x: " << historyHeadSnakePosition[j].x << ", historyHeadSnakePosition.z: " << historyHeadSnakePosition[j].z << std::endl;
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
	float suicideRangeX = 0.25f, suicideRangeZ = 0.17f;
	int tempPointsAmount = counterOfHistoryPoints;
	std::cout << "Punktow pomiaru: " << tempPointsAmount << std::endl;
	for (int i = eatenApples, j = tempPointsAmount - 2; (tempPointsAmount - i) <= j && 0 < j; j--) {
		//std::cout << "Odleglosc: " << pos_x << ", " << pos_z << std::endl;
		//std::cout << "historyHeadSnakePosition.x: " << historyHeadSnakePosition[j].x << ", historyHeadSnakePosition.z: " << historyHeadSnakePosition[j].z << std::endl;

		if (((pos_x >= 0 && historyHeadSnakePosition[j].x >= 0) || (pos_x <= 0 && historyHeadSnakePosition[j].x <= 0)) && abs(pos_x - historyHeadSnakePosition[j].x) < suicideRangeX) {
			if (((pos_z >= 0 && historyHeadSnakePosition[j].z >= 0) || (pos_z >= 0 && historyHeadSnakePosition[j].z <= 0) || (pos_z <= 0 && historyHeadSnakePosition[j].z <= 0 && abs(pos_z - historyHeadSnakePosition[j].z))) && abs(pos_z - historyHeadSnakePosition[j].z) < suicideRangeZ) {
				std::cout << "Odleglosc: " << pos_x << ", " << pos_z << std::endl;
				std::cout << "historyHeadSnakePosition.x: " << historyHeadSnakePosition[j].x << ", historyHeadSnakePosition.z: " << historyHeadSnakePosition[j].z << std::endl;
				suicided = true;
				break;
			}

			else if (pos_z <= 0 && historyHeadSnakePosition[j].z >= 0 && abs(historyHeadSnakePosition[j].z - pos_z) < suicideRangeZ) {
				std::cout << "Odleglosc: " << pos_x << ", " << pos_z << std::endl;
				std::cout << "historyHeadSnakePosition.x: " << historyHeadSnakePosition[j].x << ", historyHeadSnakePosition.z: " << historyHeadSnakePosition[j].z << std::endl;
				suicided = true;
				break;
			}
		}
		else if (((pos_x >= 0 && historyHeadSnakePosition[j].x <= 0 && abs(pos_x - historyHeadSnakePosition[j].x) < suicideRangeX) || (pos_x <= 0 && historyHeadSnakePosition[j].x >= 0 && abs(historyHeadSnakePosition[j].x - pos_x) < suicideRangeX))) {
			if (((pos_z >= 0 && historyHeadSnakePosition[j].z >= 0) || (pos_z >= 0 && historyHeadSnakePosition[j].z <= 0) || (pos_z <= 0 && historyHeadSnakePosition[j].z <= 0 && abs(pos_z - historyHeadSnakePosition[j].z))) && abs(pos_z - historyHeadSnakePosition[j].z) < suicideRangeZ) {
				std::cout << "Odleglosc: " << pos_x << ", " << pos_z << std::endl;
				std::cout << "historyHeadSnakePosition.x: " << historyHeadSnakePosition[j].x << ", historyHeadSnakePosition.z: " << historyHeadSnakePosition[j].z << std::endl;
				suicided = true;
				break;
			}

			else if (pos_z <= 0 && historyHeadSnakePosition[j].z >= 0 && abs(historyHeadSnakePosition[j].z - pos_z) < suicideRangeZ) {
				std::cout << "Odleglosc: " << pos_x << ", " << pos_z << std::endl;
				std::cout << "historyHeadSnakePosition.x: " << historyHeadSnakePosition[j].x << ", historyHeadSnakePosition.z: " << historyHeadSnakePosition[j].z << std::endl;
				suicided = true;
				break;
			}
		}
	}
	return suicided;
}


//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float pos_x, float pos_z) {
//void drawScene(GLFWwindow * window, float kat_x, float kat_y) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Kamera
	/*glm::mat4 V = glm::lookAt(pos, pos + calcDir(kat_x, kat_y), glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku

	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f, 300.0f); //Wylicz macierz rzutowania
	glm::mat4 M = glm::mat4(1.0f);*/
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*glm::mat4 V = glm::lookAt(
         glm::vec3(0, 0, -2.5),
         glm::vec3(0,0,0),
         glm::vec3(0.0f,1.0f,0.0f)); //Wylicz macierz widoku*/
	//auto test = pos + calcDir(kat_x, kat_y);
	//glm::mat4 V = glm::lookAt(pos, pos + calcDir(kat_x, kat_y), glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku

	//bliższa kamera
	//glm::mat4 V = glm::lookAt(pos, glm::vec3(0, 7.5, -13), glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku 

	glm::mat4 V = glm::lookAt(pos, glm::vec3(0, 25.94, -15.95), glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku to było

	//glm::mat4 V = glm::lookAt(pos, glm::vec3(0, 22.6, 0), glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku

	//std::cout<<"x: "<<pos.x << " y: " << pos.y << " z: " << pos.z << " vec3x: " << test.x << " vec3y: " << test.y << " vec3z: " << test.z << std::endl;
	//std::cout << "x: " << pos_x  << " z: " << pos_z << std::endl;
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

		//showString();
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

	//M4 = glm::rotate(M4, 60 * PI / 180, glm::vec3(0.0f, 1.0f, 0.0f));
	head_direction = (leftRigtDifference * 90) % 360;
	//std::cout << "headSnakePosition.z: " << headSnakePosition.x <<", headSnakePosition.z: "<< headSnakePosition.z << std::endl;
	M4 = glm::translate(M4, headSnakePosition + glm::vec3(0, 0.069, 0.012));
	M4 = glm::scale(M4, glm::vec3(0.1, 0.1, 0.1));
	M4 = glm::rotate(M4, head_direction * PI / 180, glm::vec3(0.0f, 1.0f, 0.0f));
	headSnakeModel.drawSolid(P, V, M4);


	if (isEatApple(applePosition)) {
		applePosition = randApplePosition();
	}
	if (eatenApples > 0 && isSuicide()) {
		endGame = true;
		std::cout << "The end" << std::endl;
	}

	glm::mat4 M5 = M;
	M5 = glm::translate(M5, applePosition);
	M5 = glm::scale(M5, glm::vec3(0.01, 0.01, 0.01));

	appleModel.drawSolid(P, V, M5);

	
	//std::cout << "Eaten apples: " << eatenApples << std::endl;
	counterToSavePosition++;
	if (counterToSavePosition == 17) {
		counterToSavePosition = 0;
		historyHeadSnakePosition[counterOfHistoryPoints] = headSnakePosition - glm::vec3(0, 0, -0.1);
		historySnakeDirection[counterOfHistoryPoints] = head_direction;
		counterOfHistoryPoints++;
	}

	if (counterOfHistoryPoints == 1 && eatenApples > 0) {
		glm::mat4 M6 = M4;
		//M6 = glm::rotate(M6, -65 * PI / 180, glm::vec3(0.0f, 1.0f, 0.0f));
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
	
	//glm::mat4 M4 = M;
	//M4 = glm::translate(M4, glm::vec3(-1, 3, 2));
	//M4 = glm::scale(M4, glm::vec3(0.07, 0.07, 0.07));
	//drawSolid(P, V, M4);

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
	float angle = 0; //zadeklaruj zmienną przechowującą aktualny kąt obrotu
	float kat_x = 0, kat_y = 0;
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		pos_x += speed_x * glfwGetTime();
		pos_z += speed_z * glfwGetTime();
		pos += (float)(walk_speed * glfwGetTime());
		
		/*kat_x += speed_x * glfwGetTime();
		kat_y += speed_y * glfwGetTime();
		pos += (float)(walk_speed * glfwGetTime()) * calcDir(kat_x, kat_y);*/

		glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window, pos_x, pos_z); //Wykonaj procedurę rysującą
		//drawScene(window, kat_x, kat_y); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
		
		
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
