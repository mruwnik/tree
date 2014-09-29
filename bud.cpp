#include "bud.h"
#include "meristem.h"

int bud::growthTime = 192;
supplies bud::growth(1,2,2); 	// how much supplies needed to sprout
supplies bud::use(1,0,1); 	// how much supplies used
hormones bud::hormonesProd (1,0);
hormones bud::hormonesEffect(3,4);

void bud::setHormone(hormones h){
	hormonesEffect.auxin = h.auxin;
	hormonesEffect.growth = h.growth;
}

bud::bud(part* p, supplies u): part(p, u){
	l = new leaf(this,leaf::getUses());
	togrow = -growthTime;
	devAngle = ((double)(random()%1000))/10.0f;
}

part* bud::transport(supplies& s, hormones& h){
	if(!part::transport(s,h))
		return NULL;
	if(l)
		l = l->transport(s,h);
	return grow(s,h);
}



part* bud::grow(supplies& s, hormones& h){
	if(s < bud::growth){
		std::cout << "not enough supplies to grow\n";
		return this;
	}else if(togrow < 0){
		s -= growth;
		togrow += 1;
	}else if(h.auxin > hormonesEffect.auxin){
//		std::cout << h.auxin << " " << hormonesEffect.auxin << std::endl;
		return this;

 	}else{
		h.auxin++;
		s -= bud::growth;
		togrow += 1 - (h.auxin/hormonesEffect.auxin);
//		std::cout << h.auxin << " " << hormonesEffect.auxin << " " << togrow << " " << (h.auxin/hormonesEffect.auxin)<< std::endl;
		if(togrow >= 1)
			return new meristem(part::parent,segment::getUses());
		else if(togrow < 0)
			togrow = 0;
	}
	return this;
}

void bud::draw(){
	if(l)
		l->draw();
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

