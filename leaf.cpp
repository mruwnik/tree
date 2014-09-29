#include "leaf.h"

supplies leaf::use = supplies(3,1,0);  	// defines how much supplies a leaf needs to survive
supplies leaf::growth = supplies(1,2,1); // defines how much supplies a leaf needs to grow
double leaf::sugarProduced = 2;
double leaf::growBy = 1; 	 	// defines by how much (in %) a leaf grows
double leaf::maxSize = 1;
GLuint leaf::texture[10];
GLuint leaf::textures = 0;

int leaf::renderingRound = 1;

int leaf::setTexture(GLuint t,signed int i){
	if(i < -1 || i > 10)
		return 0;
	if(i == -1){
		if(leaf::textures == 10)
			return 0;
		else
			leaf::texture[leaf::textures++] = t;
	}else{
		leaf::texture[i] = t;
	}
	return 1;
}

leaf::leaf(part* p, supplies u): part(p,u, leaf::growth), size(0.1), remainingPetioleStr(100){
	colour[0] = 0;
	colour[1] = 0.5;
	colour[2] = 0;
	togrow = maxSize;
}

leaf* leaf::transport(supplies& s, hormones& h){
	if(isDead()){
		if(remainingPetioleStr <= 0)
			return NULL;
		remainingPetioleStr -= (rand() % 100)/ 140.0;
	}
	effect(h);
	if(!part::transport(s,h))
		return this;
	leaf* leaf = grow(s,h);

	if(!isDead()){
		s.sugar += leaf::sugarProduced * size * getHealth()/1000;
	}
	return leaf;
}
  
double leaf::effect(hormones& h){
	if(h.abscisicAcid > 0)
		die();
}

leaf* leaf::grow(supplies& s, hormones& h){
	if(isGrown() || isDead())
		return this;
	if(s > growth){
		s -= growth;
		size += growBy;
	}
	togrow -= growBy;
	if(togrow <= 0)
		grown();
	return this;
}


void leaf::draw(){
    /* makes sure this isn't rendered if it's not it's turn. each shader program gets a
	 * different render round, so that only the things it should render are rendered
	 */
	if(part::renderRound != leaf::renderingRound)
		return;

	glRotatef(devAngle, 0, 0, 1.0f); // adds a bit of naturalism by rotating the bud by a certain
					  // seemingly random amount (in real plants it depends on a 
					  // few factors such as the amount of recieved light or whether
					  // it's pointing up or down)
//	glRotatef(-45,1, 0, 0.0f); 	// twists the leaf around so it sticks out straight from the branch. 
	glRotatef(90,0, 0, 1.0f); 	// rotates the leaf so it's over the bud. 
//	glRotatef(23,0, -1, 0); 	// rotates the leaf so it's over the bud. 
	glTranslatef(0,-0.5*size,0); 

    if(!isDead()){
		glEnable(GL_TEXTURE_2D);
		glBindTexture( GL_TEXTURE_2D, texture[0]);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ALPHA);
    }else{
        glDisable(GL_TEXTURE_2D);
        glDisable (GL_BLEND);
    }
	
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
//	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_DOT3_RGB_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
	glBegin (GL_QUADS);
		glColor4f(colour[0],colour[1],colour[2],0.2);
		glNormal3f(0.0,0.0,1);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0, 0.0);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0, 0.0); 
		glVertex3f (0, 0, 0);

		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0, 1.0);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0, 1.0); 
		glVertex3f (0, size, 0);

		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0, 1.0);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0, 1.0); 
		glVertex3f (size, size, 0.0); 

		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0, 0.0);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0, 0.0); 
		glVertex3f (size, 0, 0.0);
	glEnd();
	glDisable(GL_BLEND);
}

void leaf::drop(){}

void leaf::die(){
	colour[0] = 0.5;
	colour[1] = 0;
	colour[2] = 0.5;
    part::die();
}

void leaf::print(){
	std::cout << ">";
}

/* initialises and sends all needed info to the shader
 */
int leaf::setupShader(treeShader::shader& s){
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, texture[1]); 
	glEnable(GL_TEXTURE_2D);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_INCR);

	s.sendUniform1i("myTexture1", 0); // Texture Unit 0
	s.sendUniform1i("myTexture2", 1); // Texture Unit 1

	return 0;
}

GLuint leaf::readTexture(const char* file){
	int w,h;
    return loadTexture(file,w,h);
}
