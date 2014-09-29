#include "bud.h"

mesh bud::model;
int bud::growthTime = 192;
supplies bud::growth(1,2,2); 	// how much supplies needed to sprout
supplies bud::use = supplies(1,0,1); 	// how much supplies used
hormones bud::hormonesProd (1,0,0);
hormones bud::hormonesEffect(3,4,0);

void bud::setHormone(hormones h){
	hormonesEffect.auxin = h.auxin;
	hormonesEffect.growth = h.growth;
}

bud::bud(part* p, supplies u): part(p, u, bud::growth){
	l = new leaf(this,leaf::getUses());
	conductivity += l->getConductivity();
	togrow = -growthTime;
	devAngle = ((double)(random()%1000))/10.0f;
}

part* bud::transport(supplies& s, hormones& h){
	if(!part::transport(s,h)){
        return NULL;
//		return this;
	}
	if(l)
		l = l->transport(s,h);
	return grow(s,h);
}

double bud::effect(hormones& h){
	if(l)
		l->effect(h);
	return part::effect(h, hormonesEffect, hormonesProd);
}

part* bud::grow(supplies& s, hormones& h){
	double growthFactor = part::effect(h, hormonesEffect, hormonesProd);

	if(s < bud::growth){
		std::cout << "not enough supplies to grow\n";
		return this;
	}
    if(l != NULL && !l->isDead())
		return this;

	s -= growth * growthFactor;
	togrow += growthFactor;
	if(togrow >= 1){
		delete this;
		return new segment(part::parent,segment::getUses());
	}
	return this;
}

void bud::dropLeaf(){
	if(l){
		delete l;
		l = NULL;
	}
}

void bud::draw(){
	/* makes sure this isn't rendered when it shouldn't be. renderRound 0 is
	 * for all static things */
	if(!part::renderRound){
		if(bud::model.isInitialised()){
			bud::model.draw();
		}else{
			if(isDead())
				glColor3f(0,0,0);
			else
//				glColor3f(0.647059,0.164706,0.164706);
				glColor3f(0.647059 * getHealth()/1000,0.164706 * getHealth()/1000,0.164706 * getHealth()/1000);
//			glutSolidSphere(0.1,20,20);
		}
	}
	if(l){
		glPushMatrix();
		glTranslatef(0,0,-0.1);
		l->draw();
		glPopMatrix();
	}
}

void bud::print(){
	if(l)
		l->print();
}

bud::~bud(){
	if(l){
		delete l;
		l = NULL;
	}	
}

