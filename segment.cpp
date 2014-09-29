#include "segment.h"

#include <typeinfo>

int segment::budnum = 2; 	// the amount of buds at each node 
float segment::lenRate = 0; 	// how fast each segment grows longer in optimal conditions
float segment::radRate = 0; 	// how fast each segment grows wider in optimal conditions
int segment::growthTime = 0; 	// for how many units should a segment grow for before it sprouts a new segment

float segment::budAngle = 0; 
float segment::branchAngle = 0; 
supplies segment::growth(0,0,0); // how much supplies needed to grow
supplies segment::segmentUses(0,0,0); // how much supplies it uses
hormones segment::hormonesEffect(0,0,0);
hormones segment::hormonesMin(0,0,0); 	// the min amount of hormones for growth
hormones segment::hormonesProd(0,0,0);
GLuint segment::texture = 0;

void segment::setHormoneEffect(hormones h){
	hormonesEffect = h;
}

segment::segment(part* p, supplies u): part(p, u, segment::growth){
	radius = baseRadius;
	length = baseLen;
	buds = new part*[budnum];
	for(int i = 0; i < budnum; i++){
		buds[i] = 0;
	}
	bud* apexBud = new bud(this,bud::getUse());
	apexBud->dropLeaf();
	apex = apexBud;
	conductivity += apex->getConductivity();
}

segment::segment(part* p, supplies u, part** b, part* a): part(p, u, segment::growth){
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

	double origAux = h.auxin;
    h.auxin = bud::getEffect().auxin;
	supplies condSum; 	// is the sum of all partial conductivities. 
	supplies used; 		// how much supplies were used. 
	if(apex){
		supplies p = s.normalise((apex->getConductivity()/conductivity));
		supplies avail = p;
		apex = apex->transport(p, h);
		used = avail - p;
		if(apex)
			condSum = apex->getConductivity();
	}
	double apexAux = h.auxin;
	double currAux = h.auxin;
	for(int i = 0; i < budnum; i++){
		if(buds[i]){
            h.auxin = apexAux;
            supplies p = s.normalise((buds[i]->getConductivity()/conductivity));
			supplies avail = p;
			buds[i] = (buds[i])->transport(p, h);
			used += avail - p;
			if(buds[i])
				condSum += buds[i]->getConductivity();
			currAux += h.auxin - apexAux;
		}
	}
	h.auxin = currAux + origAux - bud::getEffect().auxin;
	conductivity = condSum;
	
	s -= used;
//	std::cout << ", " <<  s << ", " << used << std::endl;
	return grow(s,h);
}
	

part* segment::grow(supplies& s,hormones& h){
	if(s < segment::growth){
		return this;
	}

	double growthFactor = 1;

	if(togrow < growthTime){
		double auxin = h.auxin;
		h.auxin = hormonesEffect.auxin;
		growthFactor *= effect(h, segment::hormonesEffect, segment::hormonesProd);
		h.auxin = auxin;
	}else{
		growthFactor *= effect(h, segment::hormonesEffect, segment::hormonesProd);
	}
	if(growthFactor < segment::hormonesMin.auxin)
		return this;

	if(togrow < growthTime){
		length += segment::lenRate * growthFactor;
		s -= growth * growthFactor;

		togrow += 1;
		return this;
	}else if(togrow == growthTime){
		togrow++;
		for(int i = 0; i < budnum; i++){
			buds[i] = new bud(this,bud::getUse());
			conductivity += buds[i]->getConductivity();
		}
		delete apex;
		apex = new segment(this, segmentUses);
	}
	s -= segment::growth * growthFactor;
	radius += segment::radRate * growthFactor;
	return this;
}

void segment::draw(){
	/* makes sure this isn't rendered when it shouldn't be. renderRound 0 is
	 * for all static things */
	glRotatef((devAngle * budAngle), 0.0f, 0, 1.0f); 
	if(!part::renderRound){
		glBindTexture(GL_TEXTURE_2D,texture);
		double auxinFactor = (horm.auxin/segment::hormonesEffect.auxin)/2;
		glColor3f(0.647059 + auxinFactor*0.35,0.164706+auxinFactor*0.84,0.164706+auxinFactor*0.84);
		GLUquadric *quadric = gluNewQuadric(); 
		gluQuadricNormals(quadric, GLU_SMOOTH);   // Create Smooth Normals ( NEW )
		gluQuadricTexture(quadric, GL_TRUE); 
		gluCylinder(quadric, radius, radius, length, 10, 10);
		glTranslatef(0,0, length); 
		glColor3f(0.647059, 0.464706, 0.164706);
		gluCylinder(quadric, radius+0.001, radius+0.001, 0.05, 10, 10);
	}else{
		glTranslatef(0,0, length); 
	}
	if(apex){
		apex->draw();
	}
	float div = 360/(float)(budnum);
	for(int i = 0; i < budnum; i++){
		if(buds[i]){
			glRotatef(div*i, 0, 0, 1.0f); 
			glRotatef(branchAngle, 0, 1, 0.0f); 
			glRotatef(90, 0, 0, 1.0f); 	// otherwise the tree would be flat - it makes the buds change orientation:
			glTranslatef(0,0,-radius);
			buds[i]->draw();
			// undo the transformations - glPush (and pop) Matrix are problematic 
			// because the stack is limited
			glTranslatef(0,0,radius);
			glRotatef(-90, 0, 0, 1.0f); 	// otherwise the tree would be flat - it makes the buds change orientation:
			glRotatef(-branchAngle, 0, 1, 0.0f); 
			glRotatef(-div*i, 0, 0, 1.0f); 
			// end undo
		}
	}
	// undo the transformations - glPush (and pop) Matrix are problematic 
	// because the stack is limited
	glTranslatef(0,0, -length); 
	glRotatef(-(budAngle * budAngle), 0.0f, 0, 1.0f); 
	// end undo
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

GLuint segment::readTexture(const char* file){
	int w,h;
    return loadTexture(file,w,h);
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

