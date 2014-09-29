#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <stdio.h>

#include "tree.h"
tree t;
int count = 0;
int max = 1400;
int timeout = 20;
double x = 5;
double y = 5;
double g = 5;
double hangle = 0;
double vangle = -90;
double zoom = 20;

void renderScene(void) {
	glMatrixMode(GL_MODELVIEW);
	t.draw();
}

void setlight(){
//	GLfloat spot_direction[] = { 0.0, 0.0, 1.0};
	GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 0.4 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	GLfloat light_position[] = {0.0,0.0,15,1.0};
	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 0.1};
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

//	glEnable(GL_LIGHT1);
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

void init(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(820,820);
	glutCreateWindow("Tree");
	glutDisplayFunc(renderScene);
	glutIdleFunc(NULL);
	glutReshapeFunc(changeSize);
//	glutKeyboardFunc(keys::keypress);
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
	glClearColor(0.0, 0.0, 0.0, 0.0); 
	glClear(GL_COLOR_BUFFER_BIT);

//	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

//	GLfloat mat_amb_diff[] = { 0, 0, 0, 1.0 };
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, 
//			            mat_amb_diff);

	std::cout << "Initalised colours\n";

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

}

void mainLoop(int i){
	switch(i){
		case 1:
		t.transport();	// jedna tura gry
		t.draw();	// wyswietla obecny stan gry
		break;
		case 2: t.draw(); break;
	}
	if(count++ < max)
		glutTimerFunc(timeout, mainLoop, 1);
	else
		glutTimerFunc(timeout, mainLoop, 2);
}

int main(int argc, char** argv){
//	t.init();
#define WITH_GL
#ifdef WITH_GL
	init(argc, argv);
	glutMouseFunc(mouse);

	glutTimerFunc(1000/60, mainLoop, 1);
	glutMainLoop();
#endif
	for(int i = 0; i < 1195; i++){
//		t.grow();
#ifdef WITH_GL
		t.draw();
#else
		t.transport();	// jedna tura gry
		if(i%192 == 0){
			std::cout << "round " << i/192 << "\n\n";
			t.print();
			std::cout << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;
		}
//		getchar();
#endif
	}
	return 0;
}
