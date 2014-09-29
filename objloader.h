#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <GL/gl.h>
#include <string>

struct vertex{
	double x,y,z;
	vertex();
	vertex(double X, double Y, double Z);
	vertex& operator*(double num);
	void drawGL();
};

struct texCoord {
	double u,v;
	texCoord();
	texCoord(double U, double V);
	void drawGL();
};

struct material{
	std::string name;
	vertex Ka; 	// ambient colour
	vertex Kd; 	// difuse colour
	vertex Ks;  	// specular colour
	double Ns; 	// weight
	double Ni; 	// optical density
	double d; 	// transparency
	int illum;
	GLuint map_Ka; 	// the ambient texture map
	GLuint map_Kd; 	// the diffuse texture map (most of the time, it will
		                               // be the same as the ambient texture map)
	GLuint map_Ks; 	// the specular texture map
	GLuint map_d; 	// the alpha texture map
	GLuint map_bump; 	// the bump map
};

struct indexedTriangle{
	long vertexes[3];
	vertex normals[3];
	long texCoords[3];
	int materialIndex;
	indexedTriangle();
	indexedTriangle(long v1, long v2, long v3);
	indexedTriangle(long v1, long t1, long v2, long t2, long v3, long t3);
};

class object{
	vertex* vertexes;
	long vertexnum;
	texCoord* texCoords;
	long texCoordnum;
	std::string name;
public:
	object(std::string& n, vertex* v, long vnum, texCoord* t, long tnum);
	void print();
	void draw();
};

class mesh{
	vertex* vertexes;
	unsigned long vertexnum;
	texCoord* texCoords;
	unsigned long texCoordnum;
	object* objects;
	unsigned long objectnum;
	indexedTriangle* triangles;
	unsigned long trianglenum;
	material* materials;
	int materialsnum;
	GLuint listIndex;
	bool initialised;
public:
	mesh();
	void init(const char* file, double scale = 1);
	int getMaterialIndex(std::string name);
	void getMaterials(const char* name);
	bool isInitialised(){return initialised;}

	GLuint makeList();
	void print();
	void draw();
	~mesh();
};

#endif // OBJLOADER_H
