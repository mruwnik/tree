#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>

#include "tree.h"
#include "glLogger.h"
int w,h;
tree t;
int max = 700;
double x = 5;
double y = 5;
double g = 5;

// display settings
bool wireframe = false;
double hangle = 0;
double vangle = -90;
double zoom = 20;
int timeout = 20; 	// time to wait between rounds

/// weather settings 
// the temperature is calculated as a simple sinus of time, with an offest of avgTemperature 
// along the Y axix, and an amplitude of deltaTemp. it should nore or less align with summer and
// winter. temperature settings are in degrees.
const double avgTemperature = 10.0; // the avg temperature in this location
const double deltaTemp = 20.0; 	// the difference between hot summer and cold winter
double temperature = 10.0; 	// the current temperature

/// time settings
// the smallest time unit is 1h
int currentTime = 80*24; 		// the current time, in hours, counted since the beginning of the year
int speedup = 1; 		// how many times faster should the clock spin
bool paused = false;

hormones horms(6, 0, 0);

void renderScene(void) {
	glMatrixMode(GL_MODELVIEW);
	
	int n = 500;
	int m = 500;
	float n2 = -(n/2.0);
	float m2 = -(m/2.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	GLfloat mat_amb_diff[] = { 0,1.0,0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, 
			            mat_amb_diff);

	glDisable(GL_TEXTURE_2D);
	glBegin (GL_QUADS);
		if(temperature > 0)
			glColor4f(0,0.5,0,1);
		else
			glColor4f(1.0,1.0,1.0,1);
		glNormal3f(0.0,0.0,1.0);
		glVertex3f (-n, -n, 0);
		glVertex3f (-n, m, 0);
		glVertex3f (n, m, 0.0); 
		glVertex3f (n, -n, 0.0);
	glEnd();

	t.draw();

	glPopMatrix();

	glPushMatrix();
	glLoadIdentity();
	glTranslated(0, 0, -1);
	glLogger::render(!paused);
	glPopMatrix();
	glutSwapBuffers();
}

void setlight(){
//	GLfloat spot_direction[] = { 0.0, 0.0, 1.0};
	GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 0.4 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	GLfloat light_position[] = {0.0,0.0,15,1.0};
//	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 0.1};
	GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 0.1};
	GLfloat light_specular[] = { 0.2, 0.2, 0.2, 0.0 };
	GLfloat light_diffuse[] = { 0.2,0.2, 0.2, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
//	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
//	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);


	GLfloat light1_ambient[] = { 0, 0, 0, 1.0 };
	GLfloat light1_diffuse[] = { 0, 0, 0, 0.0 };
	GLfloat light1_specular[] = { 0.9, 0.9, 0.9, 1.0 };
	GLfloat light1_position[] = { 4, 4, 9, 1.0 };
	GLfloat spot_direction[] = { 0, 0, -1};

	
	glPushMatrix();
	glLoadIdentity();
//	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
//	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
//	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
//	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
//	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 6.0);

	glEnable(GL_LIGHT1);
	glPopMatrix();
	glEnable(GL_LIGHT0);

	std::cout << "Initalised lights\n";

	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = {50.0};
//	GLfloat high_shininess[] = { 100.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

//	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

}

void mouseMoved(int X, int Y){
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0,zoom,0, 		// punkt z ktorego sie patrzy
		  0.0,0.0,0.0,		// punkt (dowolny) lezacy na linie wzroku
		  0.0f,0.0f,1.0f);	// wektor ktory wskazuje gore
	vangle += ((double)(Y-y))/2;
	if(vangle > 0)
		vangle = -1;
	if(vangle < -90)
		vangle = -90;
	hangle += ((double)(X-x))/2;
	hangle = fmod(hangle,360);
	glRotated(vangle,1,0,0);
	glRotated(hangle,0,0,-1);
//	glutSwapBuffers();
	x = X;
	y = Y;

}

void mouse(int button,int state,int x1, int y1){
	std::cout << button << std::endl;
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		zoom++;
	else if(button == 4 && state == GLUT_DOWN)
		zoom += 3;
	else if(button == 3 && state == GLUT_DOWN)
		zoom -= 3;
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		zoom--;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0,zoom,0, 		// punkt z ktorego sie patrzy
		  0.0,0.0,0.0,		// punkt (dowolny) lezacy na linie wzroku
		  0.0f,0.0f,1.0f);	// wektor ktory wskazuje gore
	glRotated(vangle,1,0,0);
	glRotated(hangle,0,0,-1);
}
void changeSize(int w, int h){

	/* Prevent a divide by zero, when window is too short
	 (you cant make a window of zero width). */
	if(h == 0)
		h = 1;

	float ratio = 1.0* w / h;

	/* Reset the coordinate system before modifying*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	/* Set the viewport to be the entire window*/
	    glViewport(0, 0, w, h);

	/* Set the correct perspective.*/
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x,y,g, 		// punkt z ktorego sie patrzy
		  0.0,0.0,0.0,		// punkt (dowolny) lezacy na linie wzroku
		  0.0f,0.0f,1.0f);	// wektor ktory wskazuje gore
	setlight();

}

void showHelp(){
	glLogger::println("'w': wireframe\n'q': quit\n'a': +0.1 auxin");
	glLogger::println("'z': -0.1 auxin");
	glLogger::println("'p': pause");
	glLogger::println("'h': show help");
	glLogger::println("'s': forward 1 step");
	glLogger::println("'f': forward 1 year");
}

void keyPressed (unsigned char key, int x, int y) {  
	switch(key){
		case 'w': std::cout << key << "\n"; 
			if(wireframe){
				  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
				  wireframe = false;
			}else{
				  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
				  wireframe = true;
			}
			break;
		case 'q': exit(0); break;
		case 'a': horms.auxin += 0.1; t.effect(horms); break;
		case 'z': horms.auxin -= 0.1; t.effect(horms); break;
		case 'h': showHelp();
		case 'p': paused = !paused;
			  glLogger::println("paused");
			  break;
		case 's': t.effect(horms);
			  t.transport();
			  paused = true;
	temperature = avgTemperature + sin((currentTime / ((2 * 365.0)/4))) * deltaTemp;
			  break;
		case 'f': int year = (24*365)/speedup;
			  for(int i = 0; i < year; i++){
				temperature = avgTemperature + sin((((double)i)/year)*2*3.14) * deltaTemp;
				std::cout << "temp: " << temperature << std::endl;
				  t.effect(horms);
				  t.transport();
			 }
	}
}  
void init(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(820,820);
	glutCreateWindow("Tree");
	glutDisplayFunc(renderScene);
	glutIdleFunc(NULL);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(keyPressed);
//	glutSpecialFunc(keys::specialKeypress);
//	glutKeyboardUpFunc(keys::keyrelease);
//	glutIgnoreKeyRepeat(1);
//	glutSpecialUpFunc(keys::specialKeyRelease);
	glutPassiveMotionFunc(&mouseMoved);
	std::cout << "Initalised the window\n";

	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	//glCullFace(GL_FRONT);
	//glFrontFace(GL_CCW);
	glClearColor(135.0/255.0, 206.0/255.0, 250.0/255.0, 0.0); 
	glClear(GL_COLOR_BUFFER_BIT);

//	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

//	GLfloat mat_amb_diff[] = { 0, 0, 0, 1.0 };
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, 
//			            mat_amb_diff);

	std::cout << "Initalised colours\n";

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}


/**
 * the main simulation loop. 1 round is 1 hour
 *
 * @param i the current state
 */
void mainLoop(int i){
	if(paused){
		renderScene();
		glutTimerFunc(timeout, mainLoop, 1);
		return;
	}

	switch(i){
		case 1:
			glLogger::print("day "); glLogger::putNum(((long)(currentTime/24.0))%365); glLogger::putchar('\n');
			t.effect(horms);
			t.transport();	// jedna tura gry
			renderScene();
			break;
		case 2:
			std::cout << "winter" << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;
			t.winter();
//			currentTime = 0;
		case 3:
			renderScene();
			break;
	}
	
	currentTime++;
//	temperature = avgTemperature + sin((currentTime / ((2 * 365.0)/4))) * deltaTemp;

	if(!(currentTime % 500))
		temperature = 0;
	else if((currentTime % 500) == 3)
		temperature = 16;

	if(temperature < 10)
		horms.abscisicAcid = 1;
	else
		horms.abscisicAcid = 0;

	glLogger::print("Temperature: ");
	glLogger::putNum(temperature);
	glLogger::putchar('\n');
//	if(++currentTime < max)
		glutTimerFunc(timeout, mainLoop, 1);
//	else if(currentTime++ == max)
//		glutTimerFunc(timeout, mainLoop, 2);
//	else if(++currentTime < max*2)
//		glutTimerFunc(timeout, mainLoop, 3);
//	else
//		glutTimerFunc(timeout, mainLoop, (currentTime = 0));
}

int main(int argc, char** argv){
	init(argc, argv);
	glutMouseFunc(mouse);

	t.init();
	t.initGraphics();
	glutTimerFunc(timeout, mainLoop, 1);
	glutMainLoop();
	return 0;
}
