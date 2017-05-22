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

using namespace PlayerCc;

obstacleMap om;
brushfireMap bm;
distanceMap dm;
potentialMap pm;

int WindowHeight = 400;
int WindowWidth = 400;


float findHighest(int A[][400], int n, int m) {
	int max;
	for (int i = 0; i < n; i++) {
		max = A[i][0];
		for (int j = 1; j < m; j++) {
			if (A[i][j] > max) {
				max = A[i][j];
			}
		}
		// do something with max

	}
	return max;
}

float findLowest(int A[][400], int n, int m) {
	int min = A[0][0];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (A[i][j] < min) {
				min = A[i][j];
			}
		}

	}
	return min;
}

void init2D(float r, float g, float b) {
	glClearColor(r, g, b, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 400.0, 0.0, 400.0);
}

void display(void) {
	float color;
//	printf("Distance at goal = %f\n", dm.map()[360][100]);
//	printf("Brushfire at goal = %f\n", bm.map()[360][100]);
//	printf("Potential at goal = %f\n", pm.map()[360][100]);


	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);

	//draw two points
	glBegin(GL_POINTS);

	for (int i = 0; i < WindowHeight; i++) {
		for (int j = 0; j < WindowWidth; j++) {
				color = om.map()[j][i];
				glColor3f(color, color, color); //blue color
				glVertex2i(j, i);
			}
	}


	glEnd();

	//draw a line
//	glBegin(GL_LINES);
//	glVertex2i(10, 10);
//	glVertex2i(100, 100);
//	glEnd();

	glFlush();
}





int main(int argc, char **argv) {

	try {
		/* Connect to Player server */
		printf("Creating Player Client\n");
		playerc_client_t *client = playerc_client_create(NULL, "localhost",
				6665);

		if (0 != playerc_client_connect(client)) {
			printf("Error connecting client\n");
			return -1;
		}

		playerc_simulation_t* simproxy = playerc_simulation_create(client, 0);
		if (simproxy == NULL) {
			printf("simproxy is NULL\n");
			return 0;
		}

		if (playerc_simulation_subscribe(simproxy, PLAYERC_OPEN_MODE) != 0) {
			fprintf(stderr, "Error subscribing: %s\n", playerc_error_str());
			return 0;
		}

		int** map = om.getMap(client);

		double** bMap = bm.generate(om);

		printf("Generated brushfire Map\n");

		double* goalx = new double;
		double* goaly = new double;

		int metax = 360;
		int metay = 100;

		//Goal is at co-ordinates 360x100
		double** dMap = dm.generate(360, 100, om.resolution(), om.width(),
				om.height());

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

		double scale = 20;

		/*
		 double ipos = scale/2 + position2d->py;
		 double jpos = scale/2 + position2d->px;
		 */

		///*
		double ipos = abs(position2d->py);
		double jpos = abs(position2d->px);
		if (position2d->py > 0)
			ipos += (scale / 2);
		//else abs(ipos = (scale / 2) - ipos);

		if (position2d->px > 0)
			jpos += (scale / 2);
		//else abs(jpos = (scale / 2) - jpos);
		//*/

		printf("Distance at goal = %f\n", dm.map()[360][100]);
		printf("Brushfire at goal = %f\n", bm.map()[360][100]);
		printf("Potential at goal = %f\n", pm.map()[360][100]);

		printf("Robot Start Position = %f %f\n", scale * ipos, scale * jpos);
		printf("Pos X = %f , Pos Y =  %f\n", scale * jpos, scale * ipos);

		vector<pos*> path = pm.findPath(scale * ipos, scale * jpos);
		vector<pos*>::iterator iter = path.begin();

		double foo;
		while (iter != path.end()) {
			double x = (*iter)->x;
			double y = (*iter)->y;

			playerc_simulation_set_pose2d(simproxy, "r0", (x / scale) - 10,
					(y / scale) - 10, foo);

			++iter;
		}

		printf("Distance at goal = %f\n", dm.map()[360][100]);
		printf("Brushfire at goal = %f\n", bm.map()[360][100]);
		printf("Potential at goal = %f\n", pm.map()[360][100]);

		printf("Robot Start Position = %f %f\n", scale * ipos, scale * jpos);
		printf("Pos X = %f , Pos Y =  %f\n", scale * jpos, scale * ipos);

		// Shutdown
		playerc_position2d_unsubscribe(position2d);
		playerc_position2d_destroy(position2d);
		playerc_client_disconnect(client);
		playerc_client_destroy(client);

	} catch (PlayerError& e) {
		std::cerr << e << std::endl;
		return 1;
	}


	int maxPot, minPot;
	int n = 400;
	int m = 400;

	//maxPot = findHighest(pm.map(), 400, 400);

	double min = pm.map()[0][0];
	double max = pm.map()[0][0];
	int aux_i = 0;
	int aux_j = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (pm.map()[i][j] < pm.map()[aux_i][aux_j] ) {
				//min = pm.map()[i][j];
				aux_i = i;
				aux_j = j;

			}
		}
	}

	printf("Potential minimoooooooooo = %f\n", pm.map()[aux_i][aux_j]);
	printf("Potential minimoooooooooo = %d, %d\n",aux_i,aux_j);
	printf("Potential minimoooooooooo = %f\n", pm.map()[aux_i-1][aux_j-1]);
	printf("Potential minimoooooooooo = %f\n", pm.map()[aux_i-2][aux_j-2]);
//
//	for (int i = 0; i < n; i++) {
//		max = pm.map()[i][0];
//		for (int j = 1; j < m; j++) {
//			if (pm.map()[i][j] > max) {
//				max = pm.map()[i][j];
//			}
//		}
//
//	}
//	printf("Potential maximo = %f\n", max);






	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("points and lines");
	init2D(0.0, 0.0, 0.0);
	glutDisplayFunc(display);
	glutMainLoop();

	return (0);
}

