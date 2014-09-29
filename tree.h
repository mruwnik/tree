#ifndef TREE_H
#define TREE_H
#include <iostream>
//#include "glsl.h"

#include "leaf.h"
#include "parts.h"
#include "segment.h"
#include "bud.h"

#include <GL/glut.h>
#include "glLogger.h"


class tree{
	part* trunk;
	supplies s;
	hormones h;
	static int budsnum; 		// number of buds per node
	static float budAngle; 		// angle of rotation between nodes (in degrees)
	static float branchAngle; 	// angle between child branch and main branch (in degrees)
	static float segRad; 		// by how much should the radius of each brach grow
	static float segLen; 		// by how much should each branch grow longer
	static supplies baseSegmentUse;
	static supplies segmentGrowthUse;
	static hormones segmentHormones; 		// the optimal amount of various hormones for growth
	static hormones segmentHormonesMin; 		// the min amount of various hormones for growth
	static hormones segmentHormonesProd; 		// the influence of various hormones
	static int meristemTime; 	// the amount of time (in hours) before a meristem produces a new section

	static int budGrowthTime; 	// the amount of time (in hours) before a bud is able to sprout
	static supplies budGrowthUse; 	// amount of supplies needed by a bud to sprout
	static supplies budUse; 	// the amount of supplies used by a bud to survive
	static hormones budHormonesProd; 		// how much auxin is produced by each bud
	static hormones budHormones; 		// the influence of various hormones

	static supplies leafUse; 	// amount of supplies used by each leaf
	static supplies leafGrowth; 	// amount of supplies needed by each leaf to grow
	static double leafSugarProduced;// how much sugar is produced by a fully grown, healthy leaf
	static double leafGrowBy; 	// by how much (in %) a leaf grows
	static double leafMaxSize; 	// max size of a leaf
	static int leafRenderRound; 	// round when leaves should be rendered
	GLuint leafTexture;
	GLuint barkTexture;

	static int renderRounds;
	treeShader::shader leafShader;
public:
	tree();
	void init();
	void initGraphics();
	int grow();
	int transport();
	double effect(hormones& h); // used to change this part with a hormone
	int winter();
	void print();
	void draw(bool textured = true);
	~tree();
};
#endif // TREE_H
