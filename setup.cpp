#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>

#include "tree.h"
#include "glLogger.h"

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
