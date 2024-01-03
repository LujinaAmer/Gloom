#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <ctime>
#include <vector>
#include <iostream>
#include <irrKlang.h>
using namespace irrklang;

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

// Define a custom struct for three elements
struct Triple {
	double first;
	double second;
	double third;

	// Add any additional members or functions as needed
};

//Tombstones
std::vector<double> SpikesTombs = { -5, 0, 5 };
std::vector<double> SpearsTombs = { -3.5, 3.5 };


int WIDTH = 1280;
int HEIGHT = 720;

int moonx = 0;//motions of the moon 
bool backup=false;
int moony = 0;
bool backright = false;

GLuint tex;
GLuint moon;
char title[] = "3D Model Loader Sample";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 200;


///Moon Variables

double MoonTrans=40;
double MoonLightTrans=40;
double MoonX = -20;
double LightX = -20;
boolean TimeUp = false;

//Player
double PlayerX = 0;
double PlayerZ = 0;
double PlayerY = 0;
double RotY = 0;
double Speed = 0.3;
boolean SpaceFlag = false;
boolean SpaceFlag2 = false;
boolean FPS = false;
boolean TPS = false;
int Lives = 5;
int Score = 0;

//Level2
double PlayerCollideZ = 37;
double PlayerCollideX = 5;

//Ghost Variables
boolean PlayerStarted = false;
boolean PlayerMoved = false;
boolean GhostStop = false;
double PlayerPrevRotation = 0;
double PlayerPrevTranslationX = 0;
double PlayerPrevTranslationZ = 0;
double PlayerCurrRotation = 0;
double PlayerCurrTranslationX = 0;
double PlayerCurrTranslationZ = 0;
double GhostMovementX = 0;
double GhostMovementZ = 0.5;
double GhostRotation = 0;
boolean Running = false;
double Distance = 0.5;

//Collectable 
double Collectable1X = 20;
double Collectable1Z = 25;
double Collectable1Y = 0;
double Collectable2X = -30;
double Collectable2Z = -30;
double Collectable2Y = 0;
double Collectable3X = 0;
double Collectable3Z = -25;
double Collectable3Y = 0;
boolean Collectable1 = true;
boolean Collectable2 = true;
boolean Collectable3 = true;
boolean CollectableFlag = true;

double Obstacle1X = 0;
double Obstacle1Z = -5;
double Obstacle2X = 0;
double Obstacle2Z = 30;
boolean ObstacleFlag1 = true;
boolean ObstacleFlag2 = true;

double Arrow1X = 0;
boolean ArrowFlag = true;
double LightIntensity = 0;
boolean IntensityFlag = true;
int health = 5;
int Time = 100;
boolean Lost = false;
boolean Won = false;

//Spears
double spearsX = 0;
boolean Spears = false;

//Spikes
double spikesY = -2.5;
boolean Spikes = false;

//Torch
double rotationAngle = 0;
boolean Torch1 = true;
boolean Torch2 = true;
boolean Torch3 = true;

//Goal
boolean GoalReached = false;
boolean Level1 = true;
boolean Level2 = false;

//Sound
ISoundEngine* engine = createIrrKlangDevice();

// Define camera parameters
GLfloat cameraRadius = 10.0f;
GLfloat cameraTheta = 0.0f;
GLfloat cameraPhi = 0.0f;

class Vector {
public:
	float x, y, z;

	Vector(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector operator+(Vector& v) {
		return Vector(x + v.x, y + v.y, z + v.z);
	}

	Vector operator-(Vector& v) {
		return Vector(x - v.x, y - v.y, z - v.z);
	}

	Vector operator*(float n) {
		return Vector(x * n, y * n, z * n);
	}

	Vector operator/(float n) {
		return Vector(x / n, y / n, z / n);
	}

	Vector unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector cross(Vector v) {
		return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

Vector Eye(-0.37, 7.52, 54.52);
Vector Center(-0.37, 7.43, 53.52);
Vector Up(0, 1, 0);

int prevMouseX = 0;
int prevMouseY = 0;
Vector LookAt = Center; // Add this line
clock_t lastMouseTime = 0;
const int mouseResetDelay = 0; // Adjust the delay as needed (in milliseconds)
boolean isRecentering = false;

int cameraZoom = 0;

// Model Variables
Model_3DS model_moon;
Model_3DS model_Gate_Exit;
Model_3DS model_Gate_Entrance;
Model_3DS model_lego;
Model_3DS model_Spear;
Model_3DS model_Tomb;
Model_3DS model_Wall;
Model_3DS model_Spikes;
Model_3DS model_Torch;
Model_3DS model_slender;
Model_3DS model_heart;
Model_3DS model_ghosts;
Model_3DS model_ghost;
Model_3DS model_arrow;
Model_3DS model_wall;

// Textures
GLTexture tex1_ground;
GLTexture tex_ground;
GLTexture tex_moon;

//Light
GLfloat lightIntensity = 1.0f;  // Initial light intensity

////////////////////////////////////////////////////////////////Camera Movement and Rotation Start

void print(int x, int y, const char* string)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glRasterPos2i(x, y);  // Set the position of the text in window coordinates
	int len = (int)strlen(string);

	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void moveX(float d) {
	Vector right = Up.cross(Center - Eye).unit();
	Eye = Eye + right * d;
	Center = Center + right * d;
}

void moveY(float d) {
	Eye = Eye + Up.unit() * d;
	Center = Center + Up.unit() * d;
}

void moveZ(float d) {
	Vector view = (Center - Eye).unit();
	Eye = Eye + view * d;
	Center = Center + view * d;
}

void rotateX(float a) {
	Vector view = (Center - Eye).unit();
	Vector right = Up.cross(view).unit();
	view = view * cos(DEG2RAD(a)) + Up * sin(DEG2RAD(a));
	Up = view.cross(right);
	Center = Eye + view;
}

void rotateY(float a) {
	Vector view = (Center - Eye).unit();
	Vector right = Up.cross(view).unit();
	view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
	right = view.cross(Up);
	Center = Eye + view;
}

//=======================================================================
// Lighting Configuration Function
//=======================================================================


//mouse rotation
//void updateCameraPosition() {
//	const float radius = 10.0f;

//	Eye.x = LookAt.x + radius * std::cos(cameraTheta * M_PI / 180.0) * std::cos(cameraPhi * M_PI / 180.0);
//	Eye.y = LookAt.y + radius * std::sin(cameraPhi * M_PI / 180.0);
//	Eye.z = LookAt.z + radius * std::sin(cameraTheta * M_PI / 180.0) * std::cos(cameraPhi * M_PI / 180.0);
//
//
//	Center.x = LookAt.x;
//	Center.y = LookAt.y;
//	Center.z = LookAt.z;
//
//	Up.x = 0.0f;
//	Up.y = 1.0f;
//	Up.z = 0.0f;
//}
//
//void rotateCamera(int deltaX, int deltaY) {
//	const float sensitivity = 0.05f;
//
//	if (!isRecentering) {
//		cameraTheta -= deltaX * sensitivity;
//		cameraPhi -= deltaY * sensitivity;
//
//		// Clamp the vertical rotation to avoid flipping
//		if (cameraPhi > 89.0f) {
//			cameraPhi = 89.0f;
//		}
//		else if (cameraPhi < -89.0f) {
//			cameraPhi = -89.0f;
//		}
//
//		updateCameraPosition();
//	}
//	else {
//		isRecentering = false;
//	}
//}

void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	glEnable(GL_LIGHT0);

	// Define Moonlight ambient light (cool bluish tint)
	GLfloat ambient[] = { lightIntensity, lightIntensity, lightIntensity, 0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Moonlight diffuse light (cool bluish tint)
	GLfloat diffuse[] = { lightIntensity * 0.5f, lightIntensity * 0.5f, lightIntensity * 0.6f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Moonlight specular light (white)
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define Moonlight position in World Space
	GLfloat moonlight_position[] = { LightX, -MoonLightTrans, -60, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, moonlight_position);
}

void InitLightSource1()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT7);


	// Define Moonlight ambient light (cool bluish tint)
	GLfloat ambient[] = { LightIntensity, LightIntensity , LightIntensity, 0 };
	glLightfv(GL_LIGHT7, GL_AMBIENT, ambient);

	GLfloat diffuse[] = { 1, 1, 1, 1.0f };
	glLightfv(GL_LIGHT7, GL_DIFFUSE, diffuse);

	// Define Moonlight specular light (white)
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT7, GL_SPECULAR, specular);

	// Finally, define Moonlight position in World Space
	GLfloat moonlight_position[] = { 5, 8, 10, 1 };
	glLightfv(GL_LIGHT7, GL_POSITION, moonlight_position);
}

void time(int val)//timer animation function, allows the user to pass an integer valu to the timer function.
{
	if (Time == 00) {
		Lost = true;

	}
	else {
		Time--;
	}

	glutPostRedisplay();
	glutTimerFunc(1000, time, 0);


}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, Center.x, Center.y, Center.z, Up.x, Up.y, Up.z);
	//*******************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
	//glutSetCursor(GLUT_CURSOR_NONE);
	//glutFullScreen();

	engine->play2D("media/Background.mp3", true);

	LookAt = Center;

	PlayerX = 0;
	PlayerZ = 35;

}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	glColor3f(1, 1, 1);
}

void GhostMovement(int value) {

	if (PlayerStarted && !GhostStop) {
		if (!(PlayerPrevTranslationX == GhostMovementX && PlayerPrevTranslationZ == GhostMovementZ && GhostRotation == PlayerPrevRotation)) {
			GhostMovementX = PlayerPrevTranslationX;
			GhostMovementZ = PlayerPrevTranslationZ;
			GhostRotation = PlayerPrevRotation;
		}
		else {
			PlayerMoved = false;
		}

		if (PlayerMoved) {
			Distance = 0.5;
		}
		else {
			if (RotY == 0) {
				GhostMovementX = PlayerX;
				GhostMovementZ = PlayerZ + Distance;
				GhostRotation = RotY;
			}
			if (RotY == 180) {
				GhostMovementX = PlayerX;
				GhostMovementZ = PlayerZ - Distance;
				GhostRotation = RotY;
			}
			if (RotY == -90) {
				GhostMovementZ = PlayerZ;
				GhostMovementX = PlayerX - Distance;
				GhostRotation = RotY;
			}
			if (RotY == 90) {
				GhostMovementZ = PlayerZ;
				GhostMovementX = PlayerX + Distance;
				GhostRotation = RotY;
			}

			Distance -= 0.5;
		}

		if (Distance <= 0) {
			health--;
			if (health <= 0) {
				GhostStop = true;
			}
		}

	}

	glutPostRedisplay();
	glutTimerFunc(500, GhostMovement, 0);
}

void Timer(int value) {
		
	double MoonMovemnet = (0.00001+0.1)/20;
	double MoonMovementX = (0.6+0.1)/20;

	if (MoonTrans <= 17) 
	{
		TimeUp = true;
	}

	else

	{
		MoonTrans = MoonTrans - MoonMovemnet;
		MoonLightTrans = MoonLightTrans - MoonMovemnet;
		MoonX = MoonX + MoonMovementX;
	}

	// Decrease light intensity over time
	if (lightIntensity > 0) {
		lightIntensity -= 0.001;
	}

	InitLightSource();
	
	float humanJump = 0.05;

	if (SpaceFlag) {
		if (PlayerY < 2.5 && SpaceFlag2) {
			PlayerY = humanJump + PlayerY;
			if (PlayerY >= 2.5) {
				SpaceFlag2 = false;
			}
		}
		else {
			PlayerY = PlayerY-humanJump;
			if (PlayerY == 0) {
				SpaceFlag = false;
			}
		}
	}

	if (Spears && PlayerZ <= -10 + 4 && PlayerZ >= -10 - 3) {
		if (PlayerX <= spearsX + 18 + 5 && PlayerX >= spearsX + 18 - 5) {
			if (engine) {
				irrklang::ISound* sound = engine->play2D("media/Thump.wav", false, false, true);
			}
			PlayerX = 0;
			PlayerZ = 0;
			Lives--;
		}
	}

	if (Spears && spearsX >=-35) {
		spearsX -= 0.3;
		if (spearsX <= -35) {
			spearsX = 0;
		}
	}
	
	if (Spikes && spikesY <= 0) {
		spikesY += 0.05;
	}

	rotationAngle += 2;
	if (rotationAngle >= 360.0f) {
		rotationAngle -= 360.0f;
	}

	if (CollectableFlag)
	{
		Collectable1Y += 0.004;
		Collectable2Y += 0.004;
		Collectable3Y += 0.004;
		if (Collectable1Y >= 0.1) {
			CollectableFlag = false;
		}
	}
	else {
		Collectable1Y -= 0.004;
		Collectable2Y += 0.004;
		Collectable3Y += 0.004;
		if (Collectable1Y <= 0) {
			CollectableFlag = true;
		}
	}


	if (ObstacleFlag1)
	{
		Obstacle1X += 0.05;
		if (Obstacle1X >= 20) {
			ObstacleFlag1 = false;
		}
	}
	else {
		Obstacle1X -= 0.05;
		if (Obstacle1X <= 0) {
			ObstacleFlag1 = true;
		}
	}

	if (ObstacleFlag2)
	{
		Obstacle2X -= 0.05;
		if (Obstacle2X <= -20) {
			ObstacleFlag2 = false;
		}
	}
	else {
		Obstacle2X += 0.05;
		if (Obstacle2X >= 0) {
			ObstacleFlag2 = true;
		}
	}

	if (ArrowFlag)
	{
		Arrow1X += 0.05;
		if (Arrow1X >= 2) {
			ArrowFlag = false;
		}
	}
	else {
		Arrow1X -= 0.05;
		if (Arrow1X <= 0) {
			ArrowFlag = true;
		}
	}

	if (IntensityFlag)
	{
		LightIntensity += 0.005;
		if (LightIntensity >= 0.5) {
			IntensityFlag = false;
		}
	}
	else {
		LightIntensity -= 0.005;
		if (LightIntensity <= 0) {
			IntensityFlag = true;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(16, Timer, 0);


}

//void checkMouseMovement(int value) {
//	// Check if there's been no mouse movement for the specified delay
//	if (clock() - lastMouseTime > mouseResetDelay) {
//		// Move the mouse pointer to the center of the window
//		isRecentering = true;
//		glutWarpPointer(WIDTH / 2, HEIGHT / 2);
//	}
//
//	// Set the timer to call the function again
//	glutTimerFunc(16, checkMouseMovement, 0);
//}

void Health() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	char* p0s[20];
	sprintf((char*)p0s, "Heath:");
	glPushMatrix();
	glColor3f(0, 1, 0);
	print(2, 700, (char*)p0s);
	glTranslatef(80, 0, 0);
	glScaled(50, 1, 1);
	glLineWidth(10);
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(health, 705, 0.0f);
	glVertex3f(0, 705, 0.0f);
	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void TimeText() {
	glDisable(GL_LIGHTING);
	char* p0s[20];
	sprintf((char*)p0s, "Time: %d", Time);
	glPushMatrix();
	glColor3f(1, 1, 1);
	print(1, 2, (char*)p0s);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}
void ScoreText() {
	glDisable(GL_LIGHTING);
	char* p0s[20];
	sprintf((char*)p0s, "Score: %d", Score);
	glPushMatrix();
	glColor3f(0, 1, 1);
	glScalef(2, 2, 2);
	print(1200, 700, (char*)p0s);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}
void LoseText() {
	glDisable(GL_LIGHTING);
	char* p0s[20];
	sprintf((char*)p0s, "YOU LOSE");
	glPushMatrix();
	glColor3f(1, 0, 0);
	glScalef(2, 2, 2);
	print(450, 350, (char*)p0s);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void WinText() {
	glDisable(GL_LIGHTING);
	char* p0s[20];
	sprintf((char*)p0s, "YOU WIN");
	glPushMatrix();
	glColor3f(0, 1, 0);
	glScalef(2, 2, 2);
	print(450, 350, (char*)p0s);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	if (Level2 && PlayerZ <= -70) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (engine) {
			irrklang::ISound* sound = engine->play2D("media/Win.wav", false, false, true);
		}
		glPushMatrix();
		WinText();
		glPopMatrix();
		glutSwapBuffers();
	}



	if (Lost || health <= 0 || Lives == 0) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (engine) {
			irrklang::ISound* sound = engine->play2D("media/Lose.wav", false, false, true);
		}
		glPushMatrix();
		LoseText();
		glPopMatrix();
		glutSwapBuffers();
	}
	

	if (!(Lost || health == 0 || Lives == 0) && Level1) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GLfloat emission1[] = { 0.5,0.5,0.5, 1 };
		GLfloat no_emission1[] = { 0.0f, 0.0f, 0.0f, 1.0f };


		glPushMatrix();
		ScoreText();
		glPopMatrix();
		// Draw Ground
		glPushMatrix();
		glScalef(0.5, 2, 2);
		RenderGround();
		glPopMatrix();

		glColor3f(1.0f, 1.0f, 1.0f);

		// Draw hell gate Model
		glPushMatrix();
		glScalef(3.5, 2, 1);
		glTranslated(-3.35, 0, -43.2);
		glRotated(-90, 0, 1, 0);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission1);
		model_Gate_Exit.Draw();
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_emission1);
		glPopMatrix();

		//Wall
		glPushMatrix();
		glTranslatef(9, 0, 0);
		glScalef(5, 5, 142);
		glRotated(90, 0, 1, 0);
		model_Wall.Draw();
		glPopMatrix();

		//Wall
		glPushMatrix();
		glTranslatef(-9, 0, 0);
		glScalef(5, 5, 142);
		glRotated(90, 0, 1, 0);
		model_Wall.Draw();
		glPopMatrix();

		//SpikesComp

		//TombStone
		if (!Spikes) {
			glPushMatrix();
			glTranslatef(-5, 1.1, 25);
			glScalef(0.02, 0.02, 0.02);
			model_Tomb.Draw();
			glPopMatrix();
			//TombStone
			glPushMatrix();
			glTranslatef(0, 1.1, 25);
			glScalef(0.02, 0.02, 0.02);
			model_Tomb.Draw();
			glPopMatrix();
			//TombStone
			glPushMatrix();
			glTranslatef(5, 1.1, 25);
			glScalef(0.02, 0.02, 0.02);
			model_Tomb.Draw();
			glPopMatrix();
		}

		//Spikes
		if (Spikes) {
			glPushMatrix();
			glTranslatef(0, spikesY, 15);
			glScalef(2, 0.5, 1);
			glRotatef(90, 0, 1, 0);
			model_Spikes.Draw();
			glTranslatef(0, 0, -4);
			model_Spikes.Draw();
			glPopMatrix();
		}

		//SpearsComp

		if (!Spears) {
			//TombStone
			glPushMatrix();
			glTranslatef(-3.5, 1.1, 0);
			glScalef(0.02, 0.02, 0.02);
			model_Tomb.Draw();
			glPopMatrix();
			//TombStone
			glPushMatrix();
			glTranslatef(3.5, 1.1, 0);
			glScalef(0.02, 0.02, 0.02);
			model_Tomb.Draw();
			glPopMatrix();
		}

		//Spears
		if (Spears) {
			glPushMatrix();
			glTranslatef(spearsX, 0, -10);
			for (int y = 0; y < 5; y++) {
				for (int i = 0; i < 5; i++) {
					glPushMatrix();
					glTranslatef(23, y + 2, i - 1);
					glRotatef(90, 1, 0, 0);
					glRotatef(90, 0, 0, 1);
					glScalef(0.2, 0.1, 0.2);
					model_Spear.Draw();
					glPopMatrix();
				}
			}
			glPopMatrix();
		}

		//Torch
		if (Torch1) {
			glPushMatrix();
			glTranslatef(2.5, 0, 25);
			glTranslatef(0, 0, 0);
			glRotatef(rotationAngle, 0, 1, 0);
			glTranslatef(0, 0, 0);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission1);
			model_Torch.Draw();
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_emission1);
			glPopMatrix();
		}

		if (Torch2) {
			glPushMatrix();
			glTranslatef(-2.5, 0, 25);
			glTranslatef(0, 0, 0);
			glRotatef(rotationAngle, 0, 1, 0);
			glTranslatef(0, 0, 0);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission1);
			model_Torch.Draw();
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_emission1);
			glPopMatrix();
		}

		if (Torch3) {
			glPushMatrix();
			glTranslatef(0, 0, 0);
			glTranslatef(0, 0, 0);
			glRotatef(rotationAngle, 0, 1, 0);
			glTranslatef(0, 0, 0);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission1);
			model_Torch.Draw();
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_emission1);
			glPopMatrix();
		}


		//player
		glPushMatrix();
		glTranslatef(PlayerX, PlayerY, PlayerZ);
		glRotatef(RotY, 0, 1, 0);
		glRotatef(180, 0, 1, 0);
		glScalef(0.15, 0.15, 0.15);
		model_lego.Draw();
		glPopMatrix();

		// Draw hell gate Model
		//glPushMatrix();
		//glRotated(90, 0, 1, 0);
		//glTranslated(-38,0, 3);
		//model_Gate_Entrance.Draw();
		//glPopMatrix();

		// Draw moon Model
		glPushMatrix();
		glTranslated(MoonX, MoonTrans, -60);
		glScalef(4, 4, 4);
		GLfloat emission[] = { 0.5,0.5,0.5, 1 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
		model_moon.Draw();
		GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_emission);
		glPopMatrix();

		//sky box
		glPushMatrix();
		GLUquadricObj* qobj;
		qobj = gluNewQuadric();
		glTranslated(50, 0, 0);
		glRotated(90, 1, 0, 1);
		glBindTexture(GL_TEXTURE_2D, tex);
		gluQuadricTexture(qobj, true);
		gluQuadricNormals(qobj, GL_SMOOTH);
		gluSphere(qobj, 100, 100, 100);
		gluDeleteQuadric(qobj);
		glPopMatrix();
		glutSwapBuffers();
	}
	if (!(Lost || Health == 0 || Lives == 0) && !(Level2 && PlayerZ <= -80) && Level2) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw Ground
		glPushMatrix();
		glScalef(2, 2, 2);
		RenderGround();
		glPopMatrix();

		glPushMatrix();
		Health();
		glPopMatrix();

		glPushMatrix();
		ScoreText();
		glPopMatrix();

		glPushMatrix();
		TimeText();
		glPopMatrix();

		glColor3f(1.0f, 1.0f, 1.0f);

		// Draw heaven gate Model
		glPushMatrix();
		glRotated(180, 0, 1, 0);
		glTranslated(0, 2.5, 32);
		model_Gate_Exit.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(GhostMovementX, 0, GhostMovementZ);
		glTranslatef(5, 0, 37);
		glRotatef(GhostRotation, 0, 1, 0);
		glRotatef(180, 0, 1, 0);
		glScalef(0.05, 0.05, 0.05);
		model_ghosts.Draw();
		glPopMatrix();


		glPushMatrix();
		glTranslatef(PlayerX, PlayerY, PlayerZ);
		glTranslatef(5, 0, 37);
		glRotatef(RotY, 0, 1, 0);
		glRotatef(180, 0, 1, 0);
		glScalef(0.15, 0.15, 0.15);
		model_lego.Draw();
		glPopMatrix();

		// Draw moon Model
		glPushMatrix();
		glTranslated(MoonX, MoonTrans, -60);
		glScalef(4, 4, 4);
		GLfloat emission[] = { 0.5,0.5,0.5, 1 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
		model_moon.Draw();
		GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_emission);
		glPopMatrix();

		//sky box
		glPushMatrix();
		GLUquadricObj* qobj;
		qobj = gluNewQuadric();
		glTranslated(50, 0, 0);
		glRotated(90, 1, 0, 1);
		glBindTexture(GL_TEXTURE_2D, tex);
		gluQuadricTexture(qobj, true);
		gluQuadricNormals(qobj, GL_SMOOTH);
		gluSphere(qobj, 100, 100, 100);
		gluDeleteQuadric(qobj);
		glPopMatrix();

		if (Collectable1) {
			glPushMatrix();
			glTranslatef(Collectable1X, Collectable1Y, Collectable1Z);
			glScalef(0.05, 0.05, 0.05);
			model_heart.Draw();
			glPopMatrix();
		}

		if (Collectable2) {
			glPushMatrix();
			glTranslatef(Collectable2X, Collectable2Y, Collectable2Z);
			glScalef(0.05, 0.05, 0.05);
			model_heart.Draw();
			glPopMatrix();
		}

		if (Collectable3) {
			glPushMatrix();
			glTranslatef(Collectable3X, Collectable3Y, Collectable3Z);
			glScalef(0.05, 0.05, 0.05);
			model_heart.Draw();
			glPopMatrix();
		}

		glPushMatrix();
		glTranslatef(Obstacle1X, 0, Obstacle1Z);
		glScalef(50, 50, 50);
		model_ghost.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(Obstacle2X, 0, Obstacle2Z);
		glScalef(50, 50, 50);
		model_ghost.Draw();
		glPopMatrix();


		glPushMatrix();
		glColor3f(0, 1, 1);
		glTranslatef(Arrow1X, 0, 0);
		glTranslatef(5, 8, 10);
		glScalef(1, 1, 1);
		glRotatef(180, 0, 0, 1);
		glRotatef(90, 1, 0, 0);
		GLfloat emission1[] = { 0,0,1,1 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission1);
		model_arrow.Draw();
		GLfloat no_emission1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_emission1);
		glPopMatrix();

		//the walls of the Maze-------------------------------------------------------------------------------------------------------------------->


		for (int i = 0; i < 4; i++) {

			glPushMatrix();
			glTranslated(-39, 0, 30 - (i * 20));
			glRotated(90, 0, 1, 0);
			glScalef(10, 10, 10);
			model_wall.Draw();
			glPopMatrix();

			glPushMatrix();
			glTranslated(39, 0, 30 - (i * 20));
			glRotated(90, 0, 1, 0);
			glScalef(10, 10, 10);
			model_wall.Draw();
			glPopMatrix();

			if (i != 1) {
				glPushMatrix();
				glTranslated(30 - (i * 20), 0, 39);
				glScalef(10, 10, 10);
				model_wall.Draw();
				glPopMatrix();

				glPushMatrix();
				glTranslated(-30 + (i * 20), 0, -39);
				glScalef(10, 10, 10);
				model_wall.Draw();
				glPopMatrix();
			}
		}
		glPushMatrix();
		glScaled(0.5, 1, 1);
		glTranslated(-30 + (1 * 20), 0, -39);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();


		glPushMatrix();
		glScaled(1.6, 1, 1);
		glTranslated(2.4, 0, -28.5);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glScaled(0.5, 1, 1);
		glTranslated(-50, 0, -26);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glScaled(0.5, 1, 1);
		glTranslated(-65, 0, -15);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glScaled(0.5, 1, 1);
		glTranslated(-50, 0, 4);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glScaled(0.5, 1, 1);
		glTranslated(-65, 0, 15);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glScaled(1.3, 1, 1);
		glTranslated(-14.5, 0, 27);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glScaled(0.5, 1, 1);
		glTranslated(50 - (1 * 20), 0, 39);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(31 - (1 * 20), 0, 30);
		glRotated(90, 0, 1, 0);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(31 - (1 * 20), 0, 15);
		glRotated(90, 0, 1, 0);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glScaled(1, 1, 1.3);
		glTranslated(25, 0, 20);
		glRotated(90, 0, 1, 0);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(48 - (1 * 20), 0, -20);
		glRotated(90, 0, 1, 0);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(38 - (1 * 20), 0, -10);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(22 - (1 * 20), 0, -10);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(10 - (1 * 20), 0, -10);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(38 - (1 * 20), 0, 5);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(22 - (1 * 20), 0, 5);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(20 - (1 * 20), 0, 30);
		glRotated(90, 0, 1, 0);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-20 + (1 * 20), 0, -20);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-10 + (1 * 20), 0, -20);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-20, 0, -30 + (0 * 20));
		glRotated(90, 0, 1, 0);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-20, 0, -30 + (1 * 20));
		glRotated(90, 0, 1, 0);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-20, 0, -30 + (2 * 20));
		glRotated(90, 0, 1, 0);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-10, 0, -30 + (0 * 20));
		glRotated(90, 0, 1, 0);
		glScalef(10, 10, 10);
		model_wall.Draw();
		glPopMatrix();


		//-------------------------------------------------Finally End of Maze Sketch-------------------------------------------------------------->

		glutSwapBuffers();
	}
}

void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
	{
		Eye.x += -0.1;
		Eye.z += -0.1;
	}
	else
	{
		Eye.x += 0.1;
		Eye.z += 0.1;
	}

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(Eye.x, Eye.y, Eye.z, Center.x, Center.y, Center.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters


	glutPostRedisplay();	//Re-draw scene 
}

void myMouse(int button, int state, int x, int y)
{
	y = HEIGHT - y;

	if (state == GLUT_DOWN)
	{
		cameraZoom = y;
	}
}

//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboard(unsigned char button, int x, int y)
{
	switch (button)
	{
	case 'W':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'R':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
	float d = 0.5;
	float a = 2.0;

	switch (button) {
	case 'w':
		moveY(d);
		break;
	case 's':
		moveY(-d);
		break;
	case 'a':
		moveX(d);
		break;
	case 'd':
		moveX(-d);
		break;
	case 'q':
		moveZ(d);
		break;
	case 'e':
		moveZ(-d);
		break;
	case 'W':
		rotateX(a);
		break;
	case 'S':
		rotateX(-a);
		break;
	case 'A':
		rotateY(a);
		break;
	case 'D':
		rotateY(-a);
		break;
	case ' ': {
		switch (button) {
		case 'w':
			moveY(d);
			break;
		case 's':
			moveY(-d);
			break;
		case 'a':
			moveX(d);
			break;
		case 'd':
			moveX(-d);
			break;
		}
		SpaceFlag = true;
		SpaceFlag2 = true;
		break;
	}
	case'c': {
		if (TPS) {
			TPS = false;
		}
		else if (FPS) {
			FPS = false;
			TPS = true;
		}
		else {
			FPS = true;
			TPS = false;
		}
		break;
	}
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(Eye.x, Eye.y, Eye.z, Center.x, Center.y, Center.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	printf(" centerx:  % f", Center.x);
	printf(" centery:  % f", Center.y);
	printf(" centerz:  % f\n", Center.z);
	printf(" eyex:  % f", Eye.x);
	printf(" eyey:  % f", Eye.y);
	printf(" eyez:  % f\n", Eye.z);
	printf(" upx:  % f", Up.x);
	printf(" upy:  % f", Up.y);
	printf(" upz:  % f\n", Up.z);
	printf(" PlayerX:  % f", PlayerX);
	printf(" PlayerY:  % f", PlayerY);
	printf(" PlayerZ:  % f\n", PlayerZ);

	glutPostRedisplay();	//Re-draw scene 

}

//=======================================================================
// Motion Function
//=======================================================================

//void myMotion(int x, int y) {
//	int deltaX = x - prevMouseX;
//	int deltaY = y - prevMouseY;
//
//	rotateCamera(deltaX, deltaY);
//
//	prevMouseX = x;
//	prevMouseY = y;
//
//	lastMouseTime = clock();  // Record the time of the last mouse movement
//
//	glLoadIdentity();
//	gluLookAt(Eye.x, Eye.y, Eye.z, LookAt.x, LookAt.y, LookAt.z, Up.x, Up.y, Up.z);
//
//	glutPostRedisplay();
//	printf(" centerx:  % f", Center.x);
//	printf(" centery:  % f", Center.y);
//	printf(" centerz:  % f\n", Center.z);
//	printf(" eyex:  % f", Eye.x);
//	printf(" eyey:  % f", Eye.y);
//	printf(" eyez:  % f\n", Eye.z);
//	printf(" upx:  % f", Up.x);
//	printf(" upy:  % f", Up.y);
//	printf(" upz:  % f\n", Up.z);
//}



////////////////////////////////////////////////////////////////Camera Movement and Rotation End


//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, Center.x, Center.y, Center.z, Up.x, Up.y, Up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	model_Gate_Exit.Load("Models/gates/gate to hell.3ds");
	model_Gate_Entrance.Load("Models/gates/gate to hell.3ds");
	model_moon.Load("models/moon/Moon.3ds");
	model_lego.Load("models/lego/luke.3ds");
	model_Spear.Load("models/Spear/Spear.3ds");
	model_Tomb.Load("models/TombStone/Stone.3ds");
	model_Wall.Load("models/wall/walls.3ds");
	model_Spikes.Load("models/Spikes/Spikes.3ds");
	model_Torch.Load("models/Torch/Torch.3ds");
	model_slender.Load("models/slender/slender_man.3ds");
	model_heart.Load("models/heart/Love.3ds");
	model_ghosts.Load("models/Ghosts/ghost1.3ds");
	model_ghost.Load("models/Ghost/ghost1.3ds");
	model_arrow.Load("models/Arrow/arrow.3ds");
	model_wall.Load("models/wall/wall1.3ds");
	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	//loadBMP(&moon, "models/moon/Textures/Mo.bmp", true);
	loadBMP(&tex, "Textures/night_sky.bmp", true);
}

bool collide2(int x, int z)
{
	if (x > 3.7 && z<2.3 && z>-31 && x < 7.4)
		return true;
	if (x < -2.5 && z < 0.6 && z >= -21.5 && x >-7)
		return true;
	if (x > -14 && x < 24.7 && z<-28.9 && z>-33.9)
		return true;
	if (x > 18 && x<22 && z >-25.5 && z < -0.5)
		return true;
	if (x > 31.5 && x<35.5 && z>-73.6 && z < 2.5)
		return true;
	if (x > -41.8 && x < 35.7 && z>0.2 && z < 4)
		return true;
	if (x < -41.6 && z > -73.6 && z < 2.5)
		return true;
	if (x > -41.8 && x < 35.7 && z < -73.6)
		return true;
	if (x > -36 && x < -12 && z<-7.7 && z>-11.7)
		return true;
	if (x > -41.8 && x<-32.9 && z>-24.5 && z < -19.7)
		return true;
	if (x > -27.2 && x<-22.4 && z>-73 && z < -17.8)
		return true;
	if (x > -34.6 && x<-28.1 && z>-35.3 && z < -30.5)
		return true;
	if (x > -41.8 && x<-33 && z>-53.5 && z < -49.8)
		return true;
	if (x > -34.4 && x<-28.1 && z>-65.4 && z < -60.6)
		return true;
	if (x > -17.3 && x<-13 && z>-72 && z < -56)
		return true;
	if (x > -13 && x<15.7 && z>-58.4 && z < -55.2)
		return true;
	if (x > -12.1 && x<13.5 && z>-67.1 && z < -63.1)
		return true;
	if (x > 20.8 && x<24.9 && z>-67.5 && z < -47.2)//not working
		return true;
	if (x > -22.4 && x<23.2 && z>-48.8 && z < -44.8)
		return true;
	return false;
}

bool Collide(double x, double z) {
	if(!Spikes && z <= 25 + 0.5 && z >= 25 - 0.5){
		if ((x <= -5 + 2 && x >= -5 - 2) || (x <= 0 + 2 && x >= 0 - 2) || (x <= 5 + 2 && x >= 5 - 2)) {
			if (engine) {
				irrklang::ISound* sound = engine->play2D("media/Thump.wav", false, false, true);
			}
			Spikes = true;
			Score += 100;
			return true;	
		}
	}

	if (Spikes && z <= 15 + 2 && z >= 15) {
		if (PlayerY <= spikesY + 1.5 && PlayerY >= spikesY - 1.5) {
			if (engine) {
				irrklang::ISound* sound = engine->play2D("media/Thump.wav", false, false, true);
			}
			PlayerX = 0;
			PlayerZ = 35;
			Lives--;
		}
	}

	if (!Spears && z <= 0 + 0.5 && z >= 0 - 0.5) {
		if ((x <= -3.5 + 2 && x >= -3.5 - 2) || (x <= 3.5 + 2 && x >= 3.5 - 2)) {
			if (engine) {
				irrklang::ISound* sound = engine->play2D("media/Thump.wav", false, false, true);
			}
			Spears = true;
			Score += 100;
			return true;
		}
	}

	if (Spears && PlayerZ <= -10 + 4 && PlayerZ >= -10 - 3) {
		if (PlayerX <= spearsX + 18 + 5 && PlayerX >= spearsX + 18 - 5) {
			if (engine) {
				irrklang::ISound* sound = engine->play2D("media/Thump.wav", false, false, true);
			}
			PlayerX = 0;
			PlayerZ = 0;
			Lives --;
		}
	}

	return false;

}

bool Collect(double x, double z) {
	if (z <= 25 + 1 && z >= 25 - 1) {
		if (Torch1 && x <= 2.5 + 1 && x >= 2.5 - 1) {
			if (engine) {
				irrklang::ISound* sound = engine->play2D("media/Kaching.mp3", false, false, true);
			}
			Torch1 = false;
			return true;
		}
		if (Torch2 && x <= -2.5 + 1 && x >= -2.5 - 1) {
			if (engine) {
				irrklang::ISound* sound = engine->play2D("media/Kaching.mp3", false, false, true);
			}
			Torch2 = false;
			return true;
		}
	}
	if (Torch3 && z <= 0 + 1 && z >= 0 - 1) {
		if (x <= 0 + 1 && x >= 0 - 1) {
			if (engine) {
				irrklang::ISound* sound = engine->play2D("media/Kaching.mp3", false, false, true);
			}
			Torch3 = false;
			return true;
		}
	}
	return false;
}

void Loc(int k, int x, int y)
{
	if (Level1) {
		if (k == GLUT_KEY_RIGHT) {
			if (PlayerX + Speed < 9 - 1.3 && !Collide(PlayerX + Speed, PlayerZ)) {
				PlayerX += Speed;
				if (Collect(PlayerX, PlayerZ)) {
					Score += 100;
					lightIntensity += 0.5;
				}
				RotY = -90;
				if (FPS) {
					Eye.x = PlayerX + 0.4;
					Eye.y = PlayerY + 3.2;
					Eye.z = PlayerZ + 0.15;
					Center.z = PlayerZ;
					Center.y = -0.2;
					Center.x = (PlayerX + 200);
					gluPerspective(150, aspectRatio, zNear, zFar);
				}
				if (TPS) {
					Eye.x = PlayerX - 20;
					Eye.y = PlayerY + 7;
					Eye.z = PlayerZ - 0.15;
					Center.x = (PlayerX + 100);
					Center.y = -2;
					Center.z = PlayerZ;
					gluPerspective(0, aspectRatio, zNear, zFar);
				}
			}
			else {
				if (engine) {
					irrklang::ISound* sound = engine->play2D("media/Thump.wav", false, false, true);
				}
			}
		}
		if (k == GLUT_KEY_LEFT)
		{
			if (PlayerX - Speed > -9 + 1.3 && !Collide(PlayerX - Speed, PlayerZ)) {
				PlayerX -= Speed;
				if (Collect(PlayerX, PlayerZ)) {
					Score += 100;
					lightIntensity += 0.5;
				}
				RotY = 90;
				if (FPS) {
					Eye.x = PlayerX - 0.4;
					Eye.y = PlayerY + 3.2;
					Eye.z = PlayerZ - 0.15;
					Center.z = PlayerZ;
					Center.y = -0.2;
					Center.x = -(PlayerX + 200);
					gluPerspective(150, aspectRatio, zNear, zFar);
				}
				if (TPS) {
					Eye.x = PlayerX + 20;
					Eye.y = PlayerY + 7;
					Eye.z = PlayerZ + 0.15;
					Center.x = -(PlayerX + 100);
					Center.y = -2;
					Center.z = PlayerZ;
					gluPerspective(0, aspectRatio, zNear, zFar);
				}
			}
			else {
				if (engine) {
					irrklang::ISound* sound = engine->play2D("media/Thump.wav", false, false, true);
				}
			}
		}
		if (k == GLUT_KEY_UP)
		{
			if (PlayerZ - Speed > -40 + 1.3 && !Collide(PlayerX, PlayerZ - Speed)) {
				if (PlayerZ - Speed < -31.9) {
					PlayerX = 0;
					PlayerZ = 0;
					GoalReached = true;
					Level1 = false;
					Level2 = true;
				}
				PlayerZ -= Speed;
				if (Collect(PlayerX, PlayerZ)) {
					Score += 100;
					lightIntensity += 0.5;
				}
				RotY = 0;
				if (FPS) {
					Eye.x = PlayerX + 0.15;
					Eye.y = PlayerY + 3.2;
					Eye.z = PlayerZ - 0.4;
					Center.z = -(PlayerZ + 200);
					Center.y = -0.2;
					Center.x = PlayerX;
					gluPerspective(150, aspectRatio, zNear, zFar);
				}
				if (TPS) {
					Eye.x = PlayerX + 0.15;
					Eye.y = PlayerY + 7;
					Eye.z = PlayerZ + 15;
					Center.z = -(PlayerZ + 200);
					Center.y = -2;
					Center.x = PlayerX;
					gluPerspective(150, aspectRatio, zNear, zFar);
				}
			}
			else {
				if (engine) {
					irrklang::ISound* sound = engine->play2D("media/Thump.wav", false, false, true);
				}
			}
		}
		if (k == GLUT_KEY_DOWN)
		{
			if (PlayerZ + Speed < 40 - 1.3 && !Collide(PlayerX, PlayerZ + Speed)) {
				PlayerZ += Speed;
				if (Collect(PlayerX, PlayerZ)) {
					engine->drop();
					Score += 100;
					lightIntensity += 0.5;
					PlayerStarted = false;
				}
				RotY = 180;
				if (FPS) {
					Eye.x = PlayerX - 0.15;
					Eye.y = PlayerY + 3.2;
					Eye.z = PlayerZ + 0.4;
					Center.z = (PlayerZ + 200);
					Center.y = -0.2;
					Center.x = PlayerX;
					gluPerspective(150, aspectRatio, zNear, zFar);
				}
				if (TPS) {
					Eye.x = PlayerX - 0.15;
					Eye.y = PlayerY + 7;
					Eye.z = PlayerZ - 15;
					Center.z = (PlayerZ + 200);
					Center.y = -2;
					Center.x = PlayerX;
					gluPerspective(150, aspectRatio, zNear, zFar);
				}
			}
			else {
				if (engine) {
					irrklang::ISound* sound = engine->play2D("media/Thump.wav", false, false, true);
				}
			}
		}

		printf(" centerx:  % f", Center.x);
		printf(" centery:  % f", Center.y);
		printf(" centerz:  % f\n", Center.z);
		printf(" eyex:  % f", Eye.x);
		printf(" eyey:  % f", Eye.y);
		printf(" eyez:  % f\n", Eye.z);
		printf(" upx:  % f", Up.x);
		printf(" upy:  % f", Up.y);
		printf(" upz:  % f\n", Up.z);
		printf(" PlayerX:  % f", PlayerX);
		printf(" PlayerY:  % f", PlayerY);
		printf(" PlayerZ:  % f\n", PlayerZ);

		glLoadIdentity();	//Clear Model_View Matrix

		gluLookAt(Eye.x, Eye.y, Eye.z, Center.x, Center.y, Center.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

		glutPostRedisplay();
	}
	if (Level2) {
		PlayerPrevTranslationX = PlayerX;
		PlayerPrevTranslationZ = PlayerZ;
		PlayerPrevRotation = RotY;


		PlayerStarted = true;
		PlayerMoved = true;


		if (collide2(PlayerX, PlayerZ) == false) {

			if (k == GLUT_KEY_RIGHT) {
				PlayerX += Speed;
				PlayerCollideX += Speed;
				RotY = -90;
				if (FPS) {
					Eye.x = PlayerX + 0.4;
					Eye.y = PlayerY + 3.2;
					Eye.z = PlayerZ + 0.15;
					Center.z = PlayerZ;
					Center.y = -0.2;
					Center.x = (PlayerX + 200);
					gluPerspective(150, aspectRatio, zNear, zFar);
				}
				if (TPS) {
					Eye.x = PlayerX - 20;
					Eye.y = PlayerY + 7;
					Eye.z = PlayerZ - 0.15;
					Center.x = (PlayerX + 100);
					Center.y = -2;
					Center.z = PlayerZ;
					gluPerspective(0, aspectRatio, zNear, zFar);
				}
			}
			if (k == GLUT_KEY_LEFT)
			{
				PlayerX -= Speed;
				PlayerCollideX -= Speed;
				RotY = 90;
				if (FPS) {
					Eye.x = PlayerX - 0.4;
					Eye.y = PlayerY + 3.2;
					Eye.z = PlayerZ - 0.15;
					Center.z = PlayerZ;
					Center.y = -0.2;
					Center.x = -(PlayerX + 200);
					gluPerspective(150, aspectRatio, zNear, zFar);
				}
				if (TPS) {
					Eye.x = PlayerX + 20;
					Eye.y = PlayerY + 7;
					Eye.z = PlayerZ + 0.15;
					Center.x = -(PlayerX + 100);
					Center.y = -2;
					Center.z = PlayerZ;
					gluPerspective(0, aspectRatio, zNear, zFar);
				}
			}
			if (k == GLUT_KEY_UP)
			{
				PlayerZ -= Speed;
				PlayerCollideZ -= Speed;
				RotY = 0;
				if (FPS) {
					Eye.x = PlayerX + 0.15;
					Eye.y = PlayerY + 3.2;
					Eye.z = PlayerZ - 0.4;
					Center.z = -(PlayerZ + 200);
					Center.y = -0.2;
					Center.x = PlayerX;
					gluPerspective(150, aspectRatio, zNear, zFar);
				}
				if (TPS) {
					Eye.x = PlayerX + 0.15;
					Eye.y = PlayerY + 7;
					Eye.z = PlayerZ + 15;
					Center.z = -(PlayerZ + 200);
					Center.y = -2;
					Center.x = PlayerX;
					gluPerspective(150, aspectRatio, zNear, zFar);
				}
			}
			if (k == GLUT_KEY_DOWN)
			{
				PlayerZ += Speed;
				PlayerCollideZ += Speed;
				RotY = 180;
				if (FPS) {
					Eye.x = PlayerX - 0.15;
					Eye.y = PlayerY + 3.2;
					Eye.z = PlayerZ + 0.4;
					Center.z = (PlayerZ + 200);
					Center.y = -0.2;
					Center.x = PlayerX;
					gluPerspective(150, aspectRatio, zNear, zFar);
				}
				if (TPS) {
					Eye.x = PlayerX = 0.15;
					Eye.y = PlayerY + 7;
					Eye.z = PlayerZ - 15;
					Center.z = (PlayerZ + 200);
					Center.y = -2;
					Center.x = PlayerX;
					gluPerspective(150, aspectRatio, zNear, zFar);
				}
			}
		}

		if (Collectable1) {
			if (PlayerX + 5 >= Collectable1X - 1 && PlayerX + 5 <= Collectable1X + 1 && PlayerZ + 37 >= Collectable1Z - 1 && PlayerZ + 37 <= Collectable1Z + 1) {

				if (engine) {
					irrklang::ISound* sound = engine->play2D("media/Katching.wav", false, false, true);
				}
				Collectable1 = false;
				Score++;
				if (health < 4)health++;
			}
		}
		if (Collectable2) {
			if (PlayerX + 5 >= Collectable2X - 1 && PlayerX + 5 <= Collectable2X + 1 && PlayerZ + 37 >= Collectable2Z - 1 && PlayerZ + 37 <= Collectable2Z + 1) {
				if (engine) {
					irrklang::ISound* sound = engine->play2D("media/Katching.wav", false, false, true);
				}
				Collectable2 = false;
				Score++;
				if (health < 4)health++;
			}
		}
		if (Collectable3) {
			if (PlayerX + 5 >= Collectable3X - 1 && PlayerX + 5 <= Collectable3X + 1 && PlayerZ + 37 >= Collectable3Z - 1 && PlayerZ + 37 <= Collectable3Z + 1) {
				if (engine) {
					irrklang::ISound* sound = engine->play2D("media/Katching.wav", false, false, true);
				}
				Collectable3 = false;
				Score++;
				if (health < 4)health++;
			}
		}

		if (PlayerX + 5 >= Obstacle1X - 1 && PlayerX + 5 <= Obstacle1X + 1 && PlayerZ + 37 >= Obstacle1Z - 1 && PlayerZ + 37 <= Obstacle1Z + 1) {
			if (health > 0)health--;
		}

		if (PlayerX + 5 >= Obstacle2X - 1 && PlayerX + 5 <= Obstacle2X + 1 && PlayerZ + 37 >= Obstacle2Z - 1 && PlayerZ + 37 <= Obstacle2Z + 1) {
			if (health > 0)health--;
		}


		printf(" CollideX:  % f", PlayerCollideX);
		printf(" CollideZ:  % f", PlayerCollideX);
		printf(" Collectable1X:  % f", Collectable1X);
		printf(" Collectable1Z:  % f", Collectable1Z);

		//printf(" centerx:  % f", Center.x);
		//printf(" centery:  % f", Center.y);
		//printf(" centerz:  % f\n", Center.z);
		//printf(" eyex:  % f", Eye.x);
		//printf(" eyey:  % f", Eye.y);
		//printf(" eyez:  % f\n", Eye.z);
		//printf(" upx:  % f", Up.x);
		//printf(" upy:  % f", Up.y);
		//printf(" upz:  % f\n", Up.z);

		printf(" PlayerX:  % f", PlayerX);
		printf(" PlayerY:  % f", PlayerY);
		printf(" PlayerZ:  % f\n", PlayerZ);

		if (collide2(PlayerX, PlayerZ) == true) {
			if (engine) {
				irrklang::ISound* sound = engine->play2D("media/Thump.wav", false, false, true);
			}
			if (RotY == 0) {
				PlayerZ += 0.3;
			}
			if (RotY == -90) {
				PlayerX -= 0.3;
			}
			if (RotY == 90) {
				PlayerX += 0.3;
			}
			if (RotY == 180) {
				PlayerZ -= 0.3;
			}

		}

		glLoadIdentity();	//Clear Model_View Matrix

		gluLookAt(Eye.x, Eye.y, Eye.z, Center.x, Center.y, Center.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

		glutPostRedisplay();
	}
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);
	glutCreateWindow(title);
	myInit();

	glutDisplayFunc(myDisplay);

	glutTimerFunc(25, Timer, 0);
	//glutTimerFunc(16, checkMouseMovement, 0);

	glutTimerFunc(25, GhostMovement, 0);


	glutTimerFunc(25, time, 0);

	glutKeyboardFunc(myKeyboard);

	//glutPassiveMotionFunc(myMotion);

	glutReshapeFunc(myReshape);

	glutSpecialFunc(Loc);


	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}