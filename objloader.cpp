#include "objloader.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <GL/glut.h>
#include "texture.h"

vertex::vertex() : x(0), y(0), z(0) {}
vertex::vertex(double X, double Y, double Z) : x(X), y(Y), z(Z) {}

vertex& vertex::operator*(double num){
	x *= num;
	y *= num;
	z *= num;
	return *this;
}

void vertex::drawGL(){
	glVertex3d(x, y, z);
}

texCoord::texCoord() : u(0), v(0) {}
texCoord::texCoord(double U, double V) : u(U), v(V) {}

void texCoord::drawGL(){
	glTexCoord2d(u, v);
}

indexedTriangle::indexedTriangle(){
	vertexes[0] = 0;
	vertexes[1] = 0;
	vertexes[2] = 0;
	texCoords[0] = 0;
	texCoords[1] = 0;
	texCoords[2] = 0;
}

indexedTriangle::indexedTriangle(long v1, long v2, long v3){
	vertexes[0] = v1;
	vertexes[1] = v1;
	vertexes[2] = v3;
	texCoords[0] = 0;
	texCoords[1] = 0;
	texCoords[2] = 0;
}

indexedTriangle::indexedTriangle(long v1, long t1, long v2, long t2, long v3, long t3){
	vertexes[0] = v1;
	vertexes[1] = v1;
	vertexes[2] = v3;
	texCoords[0] = t1;
	texCoords[1] = t2;
	texCoords[2] = t3;
}

object::object(std::string& n, vertex* v, long vnum, texCoord* t, long tnum):
				name(n), vertexes(v), vertexnum(vnum), texCoords(t), texCoordnum(tnum){}
void object::print(){}
void object::draw(){}

vertex parseVertex(std::string line, int start = 2){
	vertex v;
	int beginning = start;
	int i = start;
	while(line[i++] != ' ');
	v.x = atof((line.substr(beginning,i - beginning)).c_str());
	beginning = i;
	while(line[i++] != ' ');
	v.y = atof(line.substr(beginning,i - beginning).c_str());
	beginning = i;
	while(line[i] != ' ' && line[i++] != '\n');
	v.z = atof(line.substr(beginning,i - beginning).c_str());
	return v;
}

texCoord parseTex(std::string& line){
	texCoord t;
	int beginning = 3;
	int i = 3;
	while(line[i++] != ' ');
	t.u = atof(line.substr(beginning,i - beginning).c_str());
	beginning = i;
	while(line[i] != ' ' && line[i++] != '\n');
	t.v = atof(line.substr(beginning,i - beginning).c_str());
	return t;
}

indexedTriangle parseTriangle(std::string& line){
	indexedTriangle t;
	int beginning = 2;
	int i = 2;
	for(int j = 0; j < 3; j++){
		while(line[i] != ' ' && line[i] != '/' && line[i++] != '\n');
		t.vertexes[j] = atoi(line.substr(beginning,i - beginning).c_str()) - 1;
		if(line[i] == '/'){
			beginning = ++i;
			while(line[i] != ' ' && line[i++] != '\n');
			t.texCoords[j] = atoi(line.substr(beginning,i - beginning).c_str()) - 1;
		i++;
		}
		beginning = i;
	}
	return t;

}

mesh::mesh(){
	initialised = false;
}

int mesh::getMaterialIndex(std::string name){
	for(int i =0; i < materialsnum; i++){
		if(!name.compare(materials[i].name))
			return i;
	}
	return -1;
}

void mesh::getMaterials(const char* name){
	int w,h;
	std::ifstream f(name);
	std::string line;
	materialsnum = 0;
	while(f.good()){
		getline(f,line);
		if(line.find("newmtl ") == 0) 	// got a material file
			materialsnum++;
	}

	f.clear() ;
	f.seekg(0, std::ios::beg);	
	materials = new material[materialsnum];

	int current = -1;

	while(f.good()){
		getline(f,line);
		if(line.find("newmtl ") == 0){ 	// got a new material
			materials[++current].name = line.substr(7);
		}else if(line.find("Ns ") == 0){
			materials[current].Ns = atof(line.substr(3).c_str());
		}else if(line.find("Ka ") == 0){
			materials[current].Ka = parseVertex(line.substr(3),3);
		}else if(line.find("Kd ") == 0){
			materials[current].Kd = parseVertex(line.substr(3),3);
		}else if(line.find("Ks ") == 0){
			materials[current].Ks = parseVertex(line.substr(3),3);
		}else if(line.find("Ni ") == 0){
			materials[current].Ni = atof(line.substr(3).c_str());
		}else if(line.find("d ") == 0){
			materials[current].d = atof(line.substr(2).c_str());
		}else if(line.find("illum ") == 0){
			materials[current].illum = atoi(line.substr(6).c_str());
		}else if(line.find("map_Kd ") == 0){ 	
//			materials[current].map_Kd = loadTexture(line.substr(7).c_str(),w,h);
			materials[current].map_Kd = loadTexture("textures/bud1.png",w,h);
			std::cout << line.substr(7) << "| " << materials[current].map_Kd << std::endl;
		}else if(line.find("map_Ka ") == 0){ 	
			materials[current].map_Ka = loadTexture(line.substr(7),w,h);
		}else if(line.find("map_Ks ") == 0){ 	
			materials[current].map_Ks = loadTexture(line.substr(7),w,h);
		}else if(line.find("map_d ") == 0){ 	
			materials[current].map_d = loadTexture(line.substr(6),w,h);
		}else if(line.find("map_bump ") == 0){ 	
			materials[current].map_bump = loadTexture(line.substr(9),w,h);
		}else if(line.find("bump ") == 0){ 	
			materials[current].map_bump = loadTexture(line.substr(5),w,h);
		}
	}
}

/* opens the passed file and parses it to extract all
 * data about the mesh.
 */
void mesh::init(const char* file, double scale){
	std::ifstream f(file);
	std::string line;
	vertexnum = texCoordnum = trianglenum = objectnum = 0;

	std::string mtllib; 	// there can be more tan one such file,
				// so this can cause problems, but who cares...
	// 2 runs are needed to first find how many
	// vertices, texCoords and polygons are present
	// and then to allocate the appropiate amount of 
	// memory to keep them
	while(f.good()){
		getline(f,line);
		if(line[0] == 'v'){
			if(line[1] == ' ')
				vertexnum++;
			else if(line[1] == 't')
				texCoordnum++;
		}else if(line[0] == 'f' && line[1] == ' '){
			trianglenum++;
		}else if(line[0] == 'o' && line[1] == ' '){
			objectnum++;
		}else if(line.find("mtllib ") == 0){ 	// got a material file
			getMaterials(line.substr(7).c_str());
		}
	}
	vertexes = new vertex[vertexnum];
	triangles = new indexedTriangle[trianglenum];
	texCoords = new texCoord[texCoordnum];
	f.clear() ;
	f.seekg(0, std::ios::beg);	

	//these 3 vars say how many vertexes etc. have been read 
	unsigned long vertexNo, texNo, triNo, objectNo;
	vertexNo = texNo = triNo = objectNo = 0;

	unsigned long beggining = 0;
	int material = -1;

	//this is where the file is actually parsed
	while(f.good()){
		getline(f,line);
		if(line[0] == '#'){
			// found a comment
		}else if(line[0] == 'v'){ 	
			if(line[1] == ' ') 	// found a vertex
				vertexes[vertexNo++] = parseVertex(line) * scale;
			else if(line[1] == 't') // found a texture value for this vertex
				texCoords[texNo++] = parseTex(line);
		}else if(line[0] == 'f' && line[1] == ' '){ // found a polygon
			triangles[triNo] = parseTriangle(line);
			triangles[triNo++].materialIndex = material;
		}else if(line.find("usemtl ") == 0){ 	// use a material 
			material = getMaterialIndex(line.substr(7));
		}else if(line[0] == 'o' && line[1] == ' '){ // found a polygon
			if(vertexNo - beggining > 0 ){
			//	objects[objectNo++] = object(line.substr(2),vertexes;
			}	
		}
	}
	f.close();
	makeList();
	initialised = true;
}

GLuint mesh::makeList(){
	listIndex = glGenLists(1);
	if(!listIndex)
		return 0;
	
	glNewList(listIndex, GL_COMPILE);
		glEnable(GL_TEXTURE_2D);
		glBegin (GL_TRIANGLES);
			glColor4f(0,0.5,0,1);
			for(int i = 0; i < trianglenum; i++){
				for(int j = 0; j < 3; j++){
					texCoords[triangles[i].texCoords[j]].drawGL();
					vertexes[triangles[i].vertexes[j]].drawGL();
				}
			}
		glEnd();
		// */
		glDisable(GL_TEXTURE_2D);
	glEndList();
	return listIndex;
}

void mesh::print(){}
void mesh::draw(){
	// this next part is totatlly rubbish - it only allows for 1 material, and
	// only uses 1 map from that material, and only works for this example
	if(materials){
//		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

//		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
//				                 GL_LINEAR_MIPMAP_NEAREST );
		// when texture area is large, bilinear filter the original
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		//
		// // the texture wraps over at the edges (repeat)
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);
		glBindTexture( GL_TEXTURE_2D,materials[1].map_Kd);
		glEnable(GL_TEXTURE_2D);
	}
	if(listIndex){
		glCallList(listIndex);
	}else{
		glPushMatrix();

		glEnable(GL_TEXTURE_2D);
		glBegin (GL_TRIANGLES);
			glColor4f(0,0.5,0,1);
			for(int i = 0; i < trianglenum; i++){
				for(int j = 0; j < 3; j++){
					texCoords[triangles[i].texCoords[j]].drawGL();
					vertexes[triangles[i].vertexes[j]].drawGL();
				}
			}
		glEnd();
		// */
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
}

mesh::~mesh(){
	glDeleteLists(listIndex, 1);
	delete [] vertexes;
	delete [] texCoords;
	delete [] triangles;
	delete [] materials;
}
