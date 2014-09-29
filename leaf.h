#ifndef LEAF_H
#define LEAF_H
#include <iostream>
#include <GL/glut.h>
#include "parts.h"

/* a single leaf */
class leaf : part{
protected:
	static supplies use; 	// defines how much supplies a leaf needs to survive
	static supplies growth; // defines how much supplies a leaf needs to grow
	static double growBy;  	// defines by how much a leaf grows
	static double maxSize; 	// max size of a leaf
	double size; 		// size of leaf in %. 100% is the max. size of a leaf
	double colour[3];
public:
	leaf(part* p,supplies u);
	leaf* transport(supplies& s, hormones& h);
	leaf* grow(supplies& s, hormones& h); 	// sends supplies to this leaf
	void drop(){}
	void die();
	void draw();
	void print();
	static supplies getUses(){ return leaf::use; }
	static void setUses(supplies s){ leaf::use = s; }
	static void setGrowth(supplies s){ leaf::growth = s; }
	static void setSize(double s, double g){ leaf::maxSize = s; leaf::growBy = s*g/100; }
};
#endif //LEAF_H
