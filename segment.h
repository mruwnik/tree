#ifndef SEGMENT_H
#define SEGMENT_H
#include "parts.h"
#include "bud.h"
#include <GL/glu.h>
#include "texture.h"

const double baseRadius = 0.1;
const double baseLen = 0.2;
/* a branch segment. a segment in this case is defined as the segment of a branch between 2
 * nodes
 */
class segment : public part{
protected:
	static int budnum; 		// the amount of buds at each node 
	static float lenRate; 		// how fast each segment grows longer in optimal conditions
	static float radRate; 		// how fast each segment grows wider in optimal conditions
	static int growthTime; 		// for how many units should a segment grow for before it sprouts a new segment
	static float budAngle; 		// at what angle do the buds pop out relative to the previous node
	static float branchAngle; 	// at what angle do branches go out relative the main branch
	static supplies growth; 	// how much supplies needed to grow
	static supplies segmentUses; 	// how much supplies it uses 
	static hormones hormonesEffect; // the optimal amount of hormones for growth
	static hormones hormonesMin; 	// the min amount of hormones for growth
	static hormones hormonesProd; 	// how much hormones are produced
	part** buds; 			// a list of buds at the end of this node
	part* apex; 			// the apex - can be anything really
	float radius;
	float length;
	static GLuint texture;
public:
	segment(part* p,supplies u);
	segment(part* p,supplies u, part** b, part* a);
	void draw();
	void print();
	part* transport(supplies& s, hormones& h);
	part* grow(supplies& u, hormones& h);
	void die();
	void drop(){};

	static supplies getUses(){return segment::segmentUses;}
	static void setUses(supplies u){segment::segmentUses = u;}
	static void setBudNum(int i){segment::budnum = i;}
	static void setBudAngle(float a){segment::budAngle = a;}
	static void setBranchAngle(float a){segment::branchAngle = a;}
	static void setRate(float d, float r, int g){segment::lenRate = d;  segment::radRate = r; segment::growthTime = g;}
	static void setGrowth(supplies g){segment::growth = g;}
	static void setHormoneProd(hormones h){segment::hormonesProd = h;}
	static void setHormoneEffect(hormones h);
	static void setHormoneMin(hormones h){segment::hormonesMin = h;}
	static GLuint readTexture(const char* file);
	static void setTexture(GLuint tex){segment::texture = tex;}
	~segment();
};
#endif //SEGMENT_H
