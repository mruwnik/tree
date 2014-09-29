#include "meristem.h"

#include <typeinfo>

float meristem::lenRate = 0.26; 	// how fast each meristem grows longer in optimal conditions
int meristem::growthTime = 192; 	// how fast each meristem grows longer in optimal conditions
supplies meristem::growth = supplies(1,1,1); // how much supplies needed to grow

meristem::meristem(part* p, supplies u): segment(p, u){
	delete apex;
	apex = NULL;
}

part* meristem::transport(supplies& s, hormones& h){
	return segment::transport(s,h);
}
	

part* meristem::grow(supplies& s,hormones& h){
	length += meristem::lenRate;
	if(s < growth)
		return this;
	s -= growth;
	if(togrow < growthTime){
		togrow += 1;
		return this;
	}else{
		apex = this;
		segment* newSegment = new segment(*this);
		parent = newSegment;
		radius = baseRadius;
		length = baseLen;
		buds = new part*[budnum];
		for(int i = 0; i < budnum; i++)
			buds[i] = new bud(this,bud::getUse());
		apex = NULL;
		togrow = 0;
		return newSegment;
	}

}

meristem::~meristem(){
}

