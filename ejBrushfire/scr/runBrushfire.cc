//#include <libplayerc++/playerc++.h>

#include <player-3.0/libplayerc++/playerc++.h>

#include <iostream>
#include <string>
#include <vector>
//#include <stdlib.h>

#include "brushfireMap.h"
#include "distanceMap.h"
#include "potentialMap.h"

#include <GL/glut.h>
#include <GL/gl.h>

#include <thread>

using namespace PlayerCc;

obstacleMap om;
brushfireMap bm;
distanceMap dm;
potentialMap pm;

vector<pos*> path;

int WindowHeight = 400;
int WindowWidth = 400;

int metaX = 360;
int metaY = 100;

int inX = 8;
int inY = -5;

double scale = 20;

enum Entorno { primero, segundo, tercero, cuarto, quinto };

Entorno mapa = quinto;

int win1, win2, win3, win4;
bool getHeatMapColor(float value, float *red, float *green, float *blue);
void init2D(float r, float g, float b);
void display(void);

void abrirPlayer(){



	switch(mapa)
			  {
			  case primero:
				  system("player /home/nico/workspace/ejBrushfire/scr/primero.cfg");
				  break;
			  case segundo:
				  system("player /home/nico/workspace/ejBrushfire/scr/segundo.cfg");
				  break;
			  case tercero:
				  system("player /home/nico/workspace/ejBrushfire/scr/tercero.cfg");
				  break;
			  case cuarto:
				  system("player /home/nico/workspace/ejBrushfire/scr/cuartoB.cfg");
				  break;
			  case quinto:
				  system("player /home/nico/workspace/ejBrushfire/scr/quinto.cfg");
				  break;
			  default:
				  system("player /home/nico/workspace/ejBrushfire/scr/success.cfg");
				  break;
			  }


}


int main(int argc, char **argv) {

	std::thread Player (abrirPlayer);
	sleep(5);
	switch(mapa)
			  {
			  case primero:
				  inX = 8;
				  inY = -5;
				  break;
			  case segundo:
				  inX = 8;
				  inY = -7;
				  break;
			  case tercero:
				  inX = -8;
				  inY = 8;
				  break;
			  case cuarto:
				  inX = -8;
				  inY = -9;
				  break;
			  case quinto:
				  inX = 8;
				  inY = 9;
				  break;
			  default:
				  inX = 8;
				  inY = -5;
				  break;
			  }

	metaX =  scale * ((scale / 2) + inX);
	metaY =  scale * ((scale / 2) + inY);

	try {
		/* Connect to Player server */
		printf("Generando cliente de Player\n");
		playerc_client_t *client = playerc_client_create(NULL, "localhost",
				6665);

		if (0 != playerc_client_connect(client)) {
			printf("Error conectando con el cliente\n");
			return -1;
		}

		playerc_simulation_t* simproxy = playerc_simulation_create(client, 0);
		if (simproxy == NULL) {
			printf("simproxy es NULL\n");
			return 0;
		}

		if (playerc_simulation_subscribe(simproxy, PLAYERC_OPEN_MODE) != 0) {
			fprintf(stderr, "Error al suscribirse: %s\n", playerc_error_str());
			return 0;
		}

		int** map = om.getMap(client);

		double** bMap = bm.generate(om);

		printf("Generated brushfire Map\n");

		double* goalx = new double;
		double* goaly = new double;



		//Goal is at co-ordinates 360x100
		printf("Meta  = %d, %d\n", metaX, metaY);
		double** dMap = dm.generate(metaX, metaY, om.resolution(), om.width(), om.height());
		printf("Generated distance Map\n");
		double** pMap = pm.generate(dm, bm);
		printf("Generated potential Map\n");

		// Create and subscribe to a position2d device.
		playerc_position2d_t* position2d = playerc_position2d_create(client, 0);
		if (playerc_position2d_subscribe(position2d, PLAYER_OPEN_MODE))
			return -1;
		// Wait for new data from server
		playerc_client_read(client);
		// Print current robot poseu
		printf("position2d : %f %f %f\n", position2d->px, position2d->py,
				position2d->pa);

		///*
//		double ipos = abs(position2d->py);
//		double jpos = abs(position2d->px);
//		if (position2d->py > 0)
//			ipos += (scale / 2);
//		if (position2d->px > 0)
//			jpos += (scale / 2);

		double ipos = (scale / 2) + (position2d->py);
		double jpos = (scale / 2) + (position2d->px);

		if (ipos < 0)
			ipos = 0;
		if (jpos < 0)
			jpos = 0;


		printf("Distance at goal = %f\n", dm.map()[metaX][metaY]);
		printf("Brushfire at goal = %f\n", bm.map()[metaX][metaY]);
		printf("Potential at goal = %f\n", pm.map()[metaX][metaY]);

		printf("Robot Start Position = %f %f\n", scale * ipos, scale * jpos);
		printf("Pos X = %f , Pos Y =  %f\n", scale * jpos, scale * ipos);

		path = pm.findPath(scale * ipos, scale * jpos);
		vector<pos*>::iterator iter = path.begin();

		double foo;
		while (iter != path.end()) {
			double x = (*iter)->x; //printf("pathX = %f\n", x);
			double y = (*iter)->y; //printf("pathY = %f\n", y);

			playerc_simulation_set_pose2d(simproxy, "r0", (x / scale) - 10,
					(y / scale) - 10, foo);

			++iter;
		}
		// Shutdown
		playerc_position2d_unsubscribe(position2d);
		playerc_position2d_destroy(position2d);
		playerc_client_disconnect(client);
		playerc_client_destroy(client);

	} catch (PlayerError& e) {
		std::cerr << e << std::endl;
		return 1;
	}

	int xInit = 50,
		yInit = 600,
		sep = 430;

	glutInit(&argc, argv);
	//glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutInitWindowPosition(xInit, yInit);
	win1 = glutCreateWindow("points and lines");
	init2D(0.0, 0.0, 0.0);
	glutInitWindowPosition(xInit+sep, yInit);
	win2 = glutCreateWindow("Ventana 2 ");
	init2D(0.0, 0.0, 0.0);
	glutInitWindowPosition(xInit+2*sep, yInit);
	win3 = glutCreateWindow("Ventana 3 ");
	init2D(0.0, 0.0, 0.0);
	glutInitWindowPosition(xInit+3*sep, yInit);
	win4 = glutCreateWindow("Ventana 4: ");
	init2D(0.0, 0.0, 0.0);
	glutDisplayFunc(display);
	glutMainLoop();

	return (0);
}



void init2D(float r, float g, float b) {
	glClearColor(r, g, b, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 400.0, 0.0, 400.0);
	glMatrixMode(GL_MODELVIEW);
}

void display(void) {
	float color;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);

	/////////////////////////////// dibujo del mapa de obstaculos//////////////////////////////////////////////
	glutSetWindow(win1);
	glBegin(GL_POINTS);
	for (int i = 0; i < WindowHeight; i++) {
		for (int j = 0; j < WindowWidth; j++) {
				color = om.map()[j][i];
				glColor3f(color,color,color);
				glVertex2i(i, j);
			}
	}
	vector<pos*>::iterator iterA = path.begin();

	//glEnd();
	//glBegin(GL_POINT);
	glEnable(GL_POINT_SMOOTH); //glEnable
	glPointSize(3.0);   //gl_PointSize = 10.0;
	while (iterA != path.end()) {
		double x = (*iterA)->x; //printf("pathX = %f\n", x);
		double y = (*iterA)->y; //printf("pathY = %f\n", y);
		glColor3f(0.54, 0.16, 0.88);  //0.541176 0.168627 0.886275
		glVertex2i(x, y);
		++iterA;
	}
	glDisable(GL_POINT_SMOOTH);
	glEnd();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////// dibujo del mapa de BrushFire/////////////////////////////////////////
	glutSetWindow(win2);
	float r, g, b, inpVal;
	//int iter = 0;
	glLoadIdentity();
	// Multiply in translation matrix
	//glTranslatef(0, 0, -10);
	// Multiply in rotation matrix
	//glRotatef(45.0, 0, 0, 1);

	glBegin(GL_POINTS);
	for (int i = 0; i < WindowHeight; i++) {
			for (int j = 0; j < WindowWidth; j++) {
					color = bm.map()[j][i];
					inpVal = color/150;
					getHeatMapColor(inpVal, &r, &g, &b);
					glColor3f(r, g, b);
					glVertex2i(i,j);
				}
		}
    iterA = path.begin();
	while (iterA != path.end()) {
		double x = (*iterA)->x; //printf("pathX = %f\n", x);
		double y = (*iterA)->y; //printf("pathY = %f\n", y);
		glColor3f(1,1,1);  //0.541176 0.168627 0.886275
		glVertex2i(x, y);
		++iterA;
	}

	glEnd();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glutSetWindow(win3);
	glBegin(GL_POINTS);
		for (int i = 0; i < WindowHeight; i++) {
				for (int j = 0; j < WindowWidth; j++) {
						color = dm.map()[j][i];
						inpVal = color/50;
						getHeatMapColor(inpVal, &r, &g, &b);
						glColor3f(r, g, b);
						glVertex2i(i,j);
					}
			}

	    iterA = path.begin();
		while (iterA != path.end()) {
			double x = (*iterA)->x; //printf("pathX = %f\n", x);
			double y = (*iterA)->y; //printf("pathY = %f\n", y);
			glColor3f(1,1,1);   //0.541176 0.168627 0.886275
			glVertex2i(x, y);
			++iterA;
		}

	glEnd();

	glutSetWindow(win4);
	glBegin(GL_POINTS);

	for (int i = 0; i < WindowHeight; i++) {
			for (int j = 0; j < WindowWidth; j++) {
					color = pm.map()[j][i];
					inpVal = color/900;
					getHeatMapColor(inpVal, &r, &g, &b);
					glColor3f(r, g, b);
					glVertex2i(i,j);
				}
		}

	    iterA = path.begin();
		while (iterA != path.end()) {
			double x = (*iterA)->x; //printf("pathX = %f\n", x);
			double y = (*iterA)->y; //printf("pathY = %f\n", y);
			glColor3f(1,1,1);  //0.541176 0.168627 0.886275
			glVertex2i(x, y);
			++iterA;
		}
	glEnd();

	glFlush();
}






bool getHeatMapColor(float value, float *red, float *green, float *blue) {
	const int NUM_COLORS = 4;
	static float color[NUM_COLORS][3] = { { 0, 0, 1 }, { 0, 1, 0 }, { 1, 1, 0 },
			{ 1, 0, 0 } };
	// A static array of 4 colors:  (blue,   green,  yellow,  red) using {r,g,b} for each.

	int idx1; // |-- Our desired color will be between these two indexes in "color".
	int idx2; // |
	float fractBetween = 0; // Fraction between "idx1" and "idx2" where our value is.

	if (value <= 0) {
		idx1 = idx2 = 0;
	} // accounts for an input <=0
	else if (value >= 1) {
		idx1 = idx2 = NUM_COLORS - 1;
	} // accounts for an input >=0
	else {
		value = value * (NUM_COLORS - 1); // Will multiply value by 3.
		idx1 = floor(value); // Our desired color will be after this index.
		idx2 = idx1 + 1; // ... and before this index (inclusive).
		fractBetween = value - float(idx1); // Distance between the two indexes (0-1).
	}

	*red = (color[idx2][0] - color[idx1][0]) * fractBetween + color[idx1][0];
	*green = (color[idx2][1] - color[idx1][1]) * fractBetween + color[idx1][1];
	*blue = (color[idx2][2] - color[idx1][2]) * fractBetween + color[idx1][2];
}
