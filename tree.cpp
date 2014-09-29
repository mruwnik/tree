#include "tree.h"
#include "weather.h"

double timedilation = 4;

/* nice hacky way of making sure everything is rendered by the proper program */
int tree::leafRenderRound = 1;
int tree::renderRounds = 2;

tree::tree(): s(780000,560,800), h(3,10,0){
    init();
    h.auxin = 3/timedilation;
    trunk = new segment(NULL, params.getBaseSegmentUse() / timedilation);
}

tree::tree(TreeParams param): s(780000,560,800), h(3,10,0), params(param){
	init();
    h.auxin = 3/timedilation;
    trunk = new segment(NULL, params.getBaseSegmentUse() / timedilation);
}

void tree::init(){
	std::cout << "tree init: ";
    params.init();
	std::cout << "done" << std::endl;
}

void tree::initGraphics(){
    leafTexture = leaf::readTexture("../resources/textures/leaf1.png");
    barkTexture = leaf::readTexture("../resources/textures/bark.png");

    segment::setTexture(barkTexture);

	leaf::setTexture(leafTexture);
    leaf::setTexture(leaf::readTexture("../resources/textures/leaf.png"));
	leaf::setRenderingRound(tree::leafRenderRound);

    leafShader = treeShader::shader("../resources/leaf.vert","../resources/leaf.frag");

//	bud::setModel("bud.obj");
}

void tree::setParams(TreeParams params){
    this->params = params;
    this->params.init();
}

TreeParams tree::getParams(){
    std::cout << "params: " << params.getBaseSegmentUse() << std::endl;
    return params;
}

int tree::grow(){
//	std::cout << "growing (" << s.water << "," <<  s.minerals << "," <<  s.sugar << ")" << std::endl;
	trunk = trunk->grow(s,h);
    return 0;
}

int tree::transport(int times){
    if(times < 0)
        times = 0;
    for(; times > 0; times--){
        trunk = trunk->transport(s,h);

        if(Weather::getTemp() < 5){
            h.abscisicAcid = 1;
        }else{
            h.abscisicAcid = 0;
            s.water = 300000000;
            s.minerals = 300000000;
            s.sugar = 3000000000;
        }
        h.auxin = 8;
   }
    return 0;
}

double tree::effect(hormones& h){
	this->h = h;
	return 0;
}

int tree::winter(){
	hormones h = hormones(0, 0, 1);
	trunk->transport(s, h);
    return 0;
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

tree::~tree(){
    if(trunk)
        delete trunk;
}
