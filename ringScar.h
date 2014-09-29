#ifndef SEGMENT_H
#define SEGMENT_H
#include "parts.h"
#include "segment.h"
#include "bud.h"
#include "texture.h"

const double turnDegrees = 90.0; // by how much any segments sprouting from this should be turned
/* 
 * a ring scar as is found each year between the previous years meristem and this years one
 */
class ringScar : public part{
protected:
	part* apex; 			// the apex - can be anything really
	float radius;
	static GLuint texture;
public:
	ringScar(part* p, part* a);
	void draw();
	void print();
	part* transport(supplies& s, hormones& h);
	part* grow(supplies& u, hormones& h);
	void die();

	static GLuint readTexture(const char* file);
	static void setTexture(GLuint tex){ringScar::texture = tex;}
	~ringScar();
};
#endif //SEGMENT_H
