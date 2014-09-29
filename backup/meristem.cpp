#include "meristem.h"

#include <typeinfo>

float meristem::lenRate = 0.26; 	// how fast each meristem grows longer in optimal conditions
int meristem::growthTime = 192; 	// how fast each meristem grows longer in optimal conditions
supplies meristem::growth = supplies(1,1,1); // how much supplies needed to grow

meristem::meristem(part* p, supplies u): segment(p, u){
	conductivity += meristem::growth;
	((bud*)apex)->dropLeaf();
}

part* meristem::transport(supplies& s, hormones& h){
//	bud* top = (bud*)apex;
//	top->effect(h);
//	apex = NULL;
	
	// #### this bit should really be in the growth part, but it's here because I
	part* bit = segment::transport(s,h);

//	apex = top;
	return bit;
}
	

part* meristem::grow(supplies& s,hormones& h){
	return segment::grow(s, h);
	if(s < segment::growth)
		return this;

	double growthFactor = effect(h, hormonesEffect, hormonesProd);
	std::cout << "meristem: " << h.auxin << " " << growthFactor <<  std::endl;
	length += meristem::lenRate * growthFactor;
	s -= growth * growthFactor;

	if(togrow < growthTime){
		togrow += growthFactor;
		return this;
	}else{
		part* realApex = apex;
		apex = this;
		segment* newSegment = new segment(*this);
		parent = newSegment;
		radius = baseRadius;
		length = baseLen;
		buds = new part*[budnum];
		for(int i = 0; i < budnum; i++)
			buds[i] = new bud(this,bud::getUse());
		togrow = 0;
		apex = realApex;
		return newSegment;
	}

}

meristem::~meristem(){
}

