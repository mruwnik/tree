#include "segment.h"

#include <typeinfo>

int segment::budnum = 2; 	// the amount of buds at each node 
float segment::lenRate = 0.26; 	// how fast each segment grows longer in optimal conditions
float segment::radRate = 0.1; 	// how fast each segment grows wider in optimal conditions
float segment::budAngle = 0; 
float segment::branchAngle = 0; 
supplies segment::growth(1,1,1); // how much supplies needed to grow
supplies segment::segmentUses(1,0,1); // how much supplies it uses
hormones segment::hormonesEffect(4,4);

void segment::setHormone(hormones h){
	hormonesEffect.auxin = h.auxin;
	hormonesEffect.growth = h.growth;
}

segment::segment(part* p, supplies u): part(p, u){
	radius = baseRadius;
	length = baseLen;
	buds = new part*[budnum];
	apex = new bud(this,bud::getUse());
	conductivity = apex->getConductivity();
	for(int i = 0; i < budnum; i++){
		buds[i] = new bud(this,bud::getUse());
		conductivity += buds[i]->getConductivity();
	}
}

segment::segment(part* p, supplies u, part** b, part* a): part(p, u){
	radius = baseRadius;
	length = baseLen;
	buds = b;
	apex = a;
}

/* passes supplies on, but beforehand deducts the amount of supplies needed for
 * it to survive, and, if growing, to grow. the supplies are divided between the
 * various parts by thier conductivity. parts which can conduct more are deemed
 * to be larger and get more supplies. they get devided acording to:
 * [supplies] * [conductivity of given part]/[conductivity of the whole segment]
 *
 * supplies& s - the supplies to pass on
 * hormones& h - the current level of hormones in this part
 *
 * returns part* - the address of this segment (if it's transformed into something
 * 		   else, then that's returned)
 */
part* segment::transport(supplies& s, hormones& h){
	if(part::isDead())
		return this;
	if(!part::transport(s,h)){
		return this;
	}
	part* current = grow(s,h);
	if(!current)
		return NULL;

	double condSum = 0; 	// is the sum of all partial conductivities. 
	if(apex){
		apex = apex->transport(s*(apex->getConductivity()/conductivity), h);
		condSum = apex->getConductivity();
	}
	for(int i = 0; i < budnum; i++){
		if(buds[i]){
			buds[i] = (buds[i])->transport(s*(buds[i]->getConductivity()/conductivity), h);
			condSum += budnum[i]->getConductivity();
		}
	}
	conductivity = condSum;
	return current;
}
	

part* segment::grow(supplies& s,hormones& h){
	if(s < segment::growth){
		return this;
 	}else{
		s -= segment::growth;
		radius += segment::radRate;
	}
	return this;
}

void segment::draw(){
 	glRotatef(devAngle, 0.0f, 0, 1.0f); 
 	glRotatef(budAngle, 0.0f, 0, 1.0f); 
	glColor3f(0.647059,0.164706,0.164706);
	GLUquadric *quadric = gluNewQuadric(); 
	gluCylinder(quadric, radius, radius, length, 40, 40);
	glTranslatef(0,0, length); 
	glColor3f(0.647059,0.464706,0.164706);
	gluCylinder(quadric, radius+0.001, radius+0.001, 0.05, 40, 40);
	if(apex){
		glPushMatrix();
		apex->draw();
		glPopMatrix();
	}
	float div = 360/(float)(budnum);
	for(int i = 0; i < budnum; i++){
		if(buds[i]){
			glPushMatrix();
			glRotatef(div*i, 0, 0, 1.0f); 
			glRotatef(branchAngle, 0, 1, 0.0f); 
			glRotatef(90, 0, 0, 1.0f); 	// otherwise the tree would be flat - it makes the buds change orientation:
			buds[i]->draw();
			glPopMatrix();
		}
	}
}

void segment::print(){
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

void segment::die(){
	if(apex)
		apex->die();
	for(int i = 0; i < budnum; i++){
		if(buds[i])
			buds[i]->die();
	}
	part::die();
}

segment::~segment(){
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

