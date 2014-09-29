#ifndef BUD_H
#define BUD_H
#include <stdlib.h>
#include "parts.h"
#include "leaf.h"

#include <iostream>
/* a branch bud. a bud in this case is defined as the bud of a branch between 2
 * nodes
 */
class bud : public part{
protected:
	static int growthTime; 	// how long (in hours) before the bud is ready to sprout (that is is 
	static supplies growth; // how much supplies needed to sprout
	static supplies use; 	// how much supplies used by a bud
	static hormones hormonesProd;// how much auxin produced by each bud
	static hormones hormonesEffect;
	leaf* l;
public:
	bud(part* p,supplies u);
	part* transport(supplies&, hormones& h);
	part* grow(supplies& u, hormones& h);
	void drop(){};
	void draw();
	void print();

	static void setGrowthTime(int t){bud::growthTime = t;}
	static void setGrowth(supplies s){bud::growth = s;}
	static void setUse(supplies s){bud::use = s;}
	static void setHormonesProd(hormones a){bud::hormonesProd = a;}
	static void setHormone(hormones h);
	static supplies getUse(){return bud::use;}
	~bud();
};
#endif //BUD_H
