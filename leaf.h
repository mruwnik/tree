#ifndef LEAF_H
#define LEAF_H
#include <iostream>
#include <cstdlib>
#include "glsl.h"
#include <GL/glut.h>
#include "parts.h"
#include "texture.h"

/* a single leaf */
class leaf : public  part{
protected:
	static supplies use; 	// defines how much supplies a leaf needs to survive
	static supplies growth; // defines how much supplies a leaf needs to grow
	static double sugarProduced; // defines how much sugar a fully developed leaf produces
	static double growBy;  	// defines by how much a leaf grows
	static double maxSize; 	// max size of a leaf
	double size; 		// size of leaf in %. 100% is the max. size of a leaf
	double remainingPetioleStr; // strength of the petiole in %. when it reaches 0 the leaf drops
	double colour[3];

	// GL stuff
	static GLuint texture[10]; 	// leaf textures
	static GLuint textures; 	// amount of leaf texture

	/* this I don't like. it's supposedly to stop swaping shader programs, but 
	 * it's too much like a hack for my likeing. */	 
	static int renderingRound; // defines on which render round leaves should be rendered
public:
	leaf(part* p,supplies u);
	leaf* transport(supplies& s, hormones& h);
	leaf* grow(supplies& s, hormones& h); 	// sends supplies to this leaf
	double effect(hormones& h); // used to change this part with a hormone
	void drop();
	void die();
	void draw();
	void print();
	static supplies getUses(){ return leaf::use; }
	static void setUses(supplies s){ leaf::use = s; }
	static void setGrowth(supplies s){ leaf::growth = s; }
	static void setSugarProduced(double s){leaf::sugarProduced = s;}
	static void setSize(double s, double g){ leaf::maxSize = s; leaf::growBy = s*g/100; }
	static int setTexture(GLuint t,signed int i = -1);
	static GLuint readTexture(const char* file);
	static int getTexturesAmount(){return leaf::textures;}
	static int setupShader(treeShader::shader& s);

	static int getRenderingRound(){return leaf::renderingRound;}
	static void setRenderingRound(int r){leaf::renderingRound = r;}
};
#endif //LEAF_H
