#include "ringScar.h"

#include <typeinfo>

GLuint ringScar::texture = 0;

void ringScar::setHormoneEffect(hormones h){
	hormonesEffect = h;
}

ringScar::ringScar(part* p, part* a): part(p, u, NULL){
	radius = baseRadius;
	apex = a;
}

/**
 * passes supplies on (it doesn't really use enough resource to play any major part)
 *
 * @returns part* - the address of this ringScar 
 */
part* ringScar::transport(supplies& s, hormones& h){
	if(part::isDead())
		return this;
	part* current = grow(s,h);
	if(!current)
		return NULL;

	if(apex){
		apex = apex->transport(s, h);
	}
	return current;
}
	

part* ringScar::grow(supplies& s,hormones& h){
	if(s < segment::growth){
		return this;
	}

	hormones tmp = h;
	double growthFactor = effect(tmp, segment::hormonesEffect, segment::hormonesProd);

	if(growthFactor < segment::hormonesMin.auxin)
		return this;

	radius += segment::radRate * growthFactor;
	return this;
}

void ringScar::draw(){
	/* makes sure this isn't rendered when it shouldn't be. renderRound 0 is
	 * for all static things */
	glRotatef(devAngle + turnDegrees, 0.0f, 0, 1.0f); 
	glRotatef(budAngle, 0.0f, 0, 1.0f); 
	if(!part::renderRound){
		glBindTexture(GL_TEXTURE_2D,texture);
		glColor3f(0.647059,0.164706,0.164706);
		GLUquadric *quadric = gluNewQuadric(); 
		gluQuadricNormals(quadric, GLU_SMOOTH);   // Create Smooth Normals ( NEW )
		gluQuadricTexture(quadric, GL_TRUE); 
		gluCylinder(quadric, radius, radius, baseLen, 10, 10);
		glTranslatef(0,0, baseLen); 
		glColor3f(0.347059,0.264706,0.064706);
		gluCylinder(quadric, radius+0.001, radius+0.001, 0.05, 10, 10);
	}else{
		glTranslatef(0,0, baseLen); 
	}
	if(apex){
		glPushMatrix();
		apex->draw();
		glPopMatrix();
	}
}

void ringScar::print(){
	std::cout << "|"  << devAngle << std::endl;
	if(apex){
		std::cout << "apex" << std::endl;
		apex->print();
	}
	for(int i = 0; i < budnum; i++){
		if(buds[i]){
			buds[i]->print();
		}
	}
}

void ringScar::die(){
	if(apex)
		apex->die();
	for(int i = 0; i < budnum; i++){
		if(buds[i])
			buds[i]->die();
	}
	part::die();
}

GLuint ringScar::readTexture(const char* file){
	int w,h;
	return loadTexture(file,w,h);
}

ringScar::~ringScar(){
	if(apex){
		delete apex;
		apex = 0;
	}
	if(buds){
		delete [] buds;
		buds = 0;
	}
	std::cout << "deleting" << std::endl;
}

