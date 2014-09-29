#include "tree.h"

double timedilation = 0.5;

int tree::budsnum = 2; 		// number of buds per node
float tree::budAngle = 0; 	// angle of rotation between nodes (in degrees)
float tree::branchAngle = 30; 	// angle between child branch and main branch (in degrees)
float tree::segRad = 0.00005; 	// by how much should the radius of each brach grow
float tree::segLen = 0.010; 		// by how much should each branch grow longer
supplies tree::baseSegmentUse = supplies(1,0,0);
supplies tree::segmentGrowthUse = supplies(1,0,1);
hormones tree::segmentHormones = hormones(8,4,0);
hormones tree::segmentHormonesProd = hormones(0,0,0);
hormones tree::segmentHormonesMin = hormones(0.2,0,0);
int tree::meristemTime = 194; 	// the amount of time before a meristem grows a new segment

int tree::budGrowthTime = 194; 	// the amount of time (in hours) before a bud is able to sprout
supplies tree::budGrowthUse = supplies(1,2,1);
supplies tree::budUse = supplies(1,0,0);
hormones tree::budHormonesProd(0.5,0,0);
hormones tree::budHormones = hormones(4,4,0);

supplies tree::leafUse = supplies(4, 1, 0); 	// amount of supplies used by each leaf
supplies tree::leafGrowth = supplies(1, 1, 1); 	// amount of supplies needed by each leaf to grow
double tree::leafSugarProduced = 2.5;
double tree::leafGrowBy = 1; 	// by how much (in %) a leaf grows
double tree::leafMaxSize = 1; 	// max leaf size

/* nice hacky way of making sure everything is rendered by the proper program */
int tree::leafRenderRound = 1;
int tree::renderRounds = 2;

tree::tree(): s(780000,560,800), h(3,10,0){
	init();
	h.auxin = 3/timedilation;
	trunk = new segment(NULL,baseSegmentUse / timedilation);
}

void tree::init(){
	std::cout << "tree init: ";
	segment::setGrowth(segmentGrowthUse / timedilation);
	segment::setBudNum(budsnum);
	segment::setBudAngle(budAngle);
	segment::setBranchAngle(branchAngle);
	segment::setRate(segLen / timedilation, segRad / timedilation, meristemTime * timedilation);
	segment::setUses(baseSegmentUse / timedilation);
	segment::setHormoneProd(segmentHormonesProd / timedilation);
	segment::setHormoneEffect(segmentHormones / timedilation);
	segment::setHormoneMin(segmentHormonesMin);
	segment::setTexture(barkTexture);

	bud::setGrowthTime(budGrowthTime * timedilation);
	bud::setGrowth(budGrowthUse / timedilation);
	bud::setUse(budUse / timedilation);
	bud::setHormonesProd(budHormonesProd / timedilation);
	bud::setHormone(budHormones / timedilation);

	leaf::setUses(tree::leafUse / timedilation);
	leaf::setGrowth(tree::leafGrowth / timedilation);
	leaf::setSugarProduced(tree::leafSugarProduced);
	leaf::setSize(tree::leafMaxSize,tree::leafGrowBy / timedilation);
	std::cout << "done" << std::endl;
}

void tree::initGraphics(){
	leafTexture = leaf::readTexture("textures/leaf1.png");
	barkTexture = leaf::readTexture("textures/bark.png");
	leaf::setTexture(leafTexture);
	leaf::setTexture(leaf::readTexture("textures/leaf.png"));
	leaf::setRenderingRound(tree::leafRenderRound);

	leafShader = treeShader::shader("leaf.vert","leaf.frag");

//	bud::setModel("bud.obj");
}

int tree::grow(){
//	std::cout << "growing (" << s.water << "," <<  s.minerals << "," <<  s.sugar << ")" << std::endl;
	trunk = trunk->grow(s,h);
}

int tree::transport(){
//	std::cout << "supplies before: " << s << std::endl;
	supplies diff = s;
//	long start = glLogger::getTimestampInus();
	trunk = trunk->transport(s,h);
//	std::cout << "transport time: " <<  (glLogger::getTimestampInus() - start) << std::endl;
	diff = s - diff;
//	std::cout << "supplies after: " << s << std::endl;

	glLogger::print("water: "); 	glLogger::putNum(s.water); glLogger::print(" ("); 
					glLogger::putNum(diff.water); glLogger::println(")");
	glLogger::print("minerals: "); glLogger::putNum(s.minerals); glLogger::print("(");
					glLogger::putNum(diff.minerals); glLogger::println(")");
	glLogger::print("sugar: "); glLogger::putNum(s.sugar); glLogger::print("(");
					glLogger::putNum(diff.sugar); glLogger::println(")");
	glLogger::print("auxin: "); glLogger::putNum(h.auxin); glLogger::putchar('\n');
	glLogger::print("abscisicAcid: "); glLogger::putNum(h.abscisicAcid); glLogger::putchar('\n');


	if(temperature < 10){
		h.abscisicAcid = 1;
	}else{
		h.abscisicAcid = 0;
		s.water = 300000000;
//		s.water = 25 * h.auxin;
		s.minerals = 300000000;
		s.sugar = 3000000000;
	}
//	h.auxin = 3/timedilation;
}

double tree::effect(hormones& h){
	this->h = h;
	return 0;
}

int tree::winter(){
	hormones h = hormones(0, 0, 1);
	trunk->transport(s, h);
}

void tree::draw(bool textured){
//	glRotatef(-branchAngle, 0, 1, 0.0f); 
	for(int i = 0; i < tree::renderRounds; i++){
		glPushMatrix();
		part::setRenderRound(i);
		if(i){
			leafShader.begin();
			leaf::setupShader(leafShader);
			trunk->draw();
			leafShader.end();
			glActiveTextureARB(GL_TEXTURE0_ARB);
			glDisable(GL_TEXTURE_2D);

	//		glActiveTextureARB(GL_TEXTURE1_ARB);
	//		glDisable(GL_TEXTURE_2D);

		}else{
			trunk->draw();
		}
		glPopMatrix();
	}

//	std::cout << "current tree:" << std::endl;
}

void tree::print(){
	std::cout << trunk << std::endl;
	trunk->print();
}

tree::~tree(){}
