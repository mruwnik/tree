#include "leaf.h"

supplies leaf::use = supplies(3,1,0);  	// defines how much supplies a leaf needs to survive
supplies leaf::growth = supplies(1,2,1); // defines how much supplies a leaf needs to grow
double leaf::growBy = 1; 	 	// defines by how much (in %) a leaf grows
double leaf::maxSize = 1;

leaf::leaf(part* p, supplies u): part(p,u), size(0.1){
	colour[0] = 0;
	colour[1] = 0.5;
	colour[2] = 0;
	togrow = maxSize;
}

leaf* leaf::transport(supplies& s, hormones& h){
	if(!part::transport(s,h))
		return this;
	return grow(s,h);
}

leaf* leaf::grow(supplies& s, hormones& h){
	if(isGrown())
		return this;
	if(s > growth){
		s -= growth;
		size += growBy;
	}
	togrow -= growBy;
	if(togrow <= 0)
		grown();
	return this;
}

void leaf::draw(){
	glRotatef(devAngle, 0, 0, 1.0f); // adds a bit of naturalism by rotating the bud by a certain
					  // seemingly random amount (in real plants it depends on a 
					  // few factors such as the amount of recieved light or whether
					  // it's pointing up or down)
	glRotatef(-45,1, 0, 0.0f); 	// twists the leaf around so it sticks out straight from the branch. 
	glTranslatef(0,-0.5*size,0); 
	glBegin (GL_QUADS);
		glColor4f(colour[0],colour[1],colour[2],1);
		glNormal3f(0.0,0.0,1.0);
		glVertex3f (0, 0, 0);
		glVertex3f (0, size, 0);
		glVertex3f (size, size, 0.0); 
		glVertex3f (size, 0, 0.0);
	glEnd();
}

void leaf::die(){
	colour[0] = 0.5;
	colour[1] = 0;
	colour[2] = 0.5;
}

void leaf::print(){
	std::cout << ">";
}
