#include "tree.h"

int tree::budsnum = 2; 		// number of buds per node
float tree::budAngle = 180; 	// angle of rotation between nodes (in degrees)
float tree::branchAngle = 45; 	// angle between child branch and main branch (in degrees)
float tree::segRad = 0.00002; 	// by how much should the radius of each brach grow
float tree::segLen = 0.006; 		// by how much should each branch grow longer
supplies tree::baseSegmentUse = supplies(1,0,1);
supplies tree::segmentGrowthUse = supplies(1,0,1);
hormones tree::segmentHormones = hormones(4,4);
int tree::meristemTime = 194; 	// the amount of time (in hours) before a meristem produces a new section

int tree::budGrowthTime = 194; 	// the amount of time (in hours) before a bud is able to sprout
supplies tree::budGrowthUse = supplies(1,2,2);
supplies tree::budUse = supplies(1,0,1);
hormones tree::budHormonesProd(1,0);
hormones tree::budHormones = hormones(4,4);

supplies tree::leafUse; 	// amount of supplies used by each leaf
supplies tree::leafGrowth; 	// amount of supplies needed by each leaf to grow
double tree::leafGrowBy = 1; 	// by how much (in %) a leaf grows
double tree::leafMaxSize = 1; 	// max leaf size

tree::tree(): s(180,160,200), h(0,10){
	init();
	trunk = new meristem(NULL,baseSegmentUse);
}

void tree::init(){
	segment::setGrowth(segmentGrowthUse);
	segment::setBudNum(budsnum);
	segment::setBudAngle(budAngle);
	segment::setBranchAngle(branchAngle);
	segment::setRate(segLen,segRad);
	segment::setUses(baseSegmentUse);
	segment::setHormone(segmentHormones);
	meristem::setGrowthTime(meristemTime);
	meristem::setLenRate(segLen);

	bud::setGrowthTime(budGrowthTime);
	bud::setGrowth(budGrowthUse);
	bud::setUse(budUse);
	bud::setHormonesProd(budHormonesProd);
	bud::setHormone(budHormones);

	leaf::setUses(tree::leafUse);
	leaf::setSize(tree::leafMaxSize,tree::leafGrowBy);
}

int tree::grow(){
//	std::cout << "growing (" << s.water << "," <<  s.minerals << "," <<  s.sugar << ")" << std::endl;
	trunk = trunk->grow(s,h);
}

int tree::transport(){
//	std::cout << "supplies: " << s << std::endl;
	h = hormones(0,10);
	s = supplies(180,160,200);
	trunk = trunk->transport(s,h);
}

void tree::draw(){
	int n = 5;
	int m = 5;
	float n2 = -(n/2.0);
	float m2 = -(m/2.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
//	GLfloat mat_amb_diff[] = { 0,1.0,0, 1.0 };
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, 
//			            mat_amb_diff);
//	glColor3f(0,1.0,0);
//	glRectf(-(n2),-(m2),n2,m2);
//	glColor3f(1.0,1.0,1.0);
//	mat_amb_diff[0]= 1.0;
//	mat_amb_diff[2]= 1.0;
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, 
//			            mat_amb_diff);

	glEnable(GL_BLEND);		// Enable Blending
	glBegin (GL_QUADS);
		glColor4f(0,0.5,0,1);
		glNormal3f(0.0,0.0,1.0);
		glVertex3f (-5, -5, 0);
		glVertex3f (-5, m, 0);
		glVertex3f (n, m, 0.0); 
		glVertex3f (n, -5, 0.0);
	glEnd();
	
//	glRotatef(-branchAngle, 0, 1, 0.0f); 
	trunk->draw();
	glDisable(GL_BLEND);
	glPopMatrix();
	glutSwapBuffers();

//	std::cout << "current tree:" << std::endl;
}

void tree::print(){
	std::cout << trunk << std::endl;
	trunk->print();
}

tree::~tree(){}
