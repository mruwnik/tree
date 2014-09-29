#ifndef MERISTEM_H
#define MERISTEM_H
#include "segment.h"
#include <GL/glut.h>

/* a branch meristem. a meristem in this case is defined as the meristem of a branch between 2
 * nodes
 */
class meristem : public segment{
protected:
	static float lenRate; 		// how fast each meristem grows longer in optimal conditions
	static int growthTime; 		// how long (in hours) before a new segment is produced
	static supplies growth; 	// how much supplies needed to grow
public:
	meristem(part* p,supplies u);
	part* transport(supplies& s, hormones& h);
	part* grow(supplies& u, hormones& h);

	static void setLenRate(float l){ meristem::lenRate = l;}
	static void setGrowthTime(int t){ meristem::growthTime = t;}
	static void setGrowth(supplies s){ meristem::growth = s;}

	~meristem();
};
#endif //MERISTEM_H
