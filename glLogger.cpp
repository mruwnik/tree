#include "glLogger.h"
#include <string.h>

std::string glLogger::contents = std::string();
int glLogger::r = 0;
int glLogger::g = 0;
int glLogger::b = 0;
void* glLogger::font = GLUT_BITMAP_TIMES_ROMAN_10;
float glLogger::lineHeight = 10;

void glLogger::setColour(int r, int g, int b){
	glLogger::r = r;
	glLogger::g = g;
	glLogger::b = b;
}
void glLogger::setFont(void* font){
	glLogger::font = font;
//	glLogger::lineHeight = glutBitmapHeight(font);
}

void glLogger::putNum(long number){
	std::stringstream ss;//create a stringstream
	ss << number;//add number to the stream
	glLogger::contents += ss.str();
}

void glLogger::putNum(double number){
	std::stringstream ss;//create a stringstream
	ss << number;//add number to the stream
	glLogger::contents += ss.str();
}

char glLogger::putchar(const char c){
	glLogger::contents += c;
	return c;
}
char* glLogger::print(const char* str){
	glLogger::contents.append(str);
	return (char*)str;
}
char* glLogger::println(const char* str){
	glLogger::contents.append(str);
	glLogger::contents.append("\n");
	return (char*)str;
}
std::string& glLogger::print(const std::string& str){
	glLogger::contents.append(str);
	return (std::string&)str;
}
std::string& glLogger::println(const std::string& str){
	glLogger::contents.append(str);
	glLogger::contents.append("\n");
	return (std::string&)str;
}

void glLogger::printText(float x, float y, const char* str){
	glLogger::printText(x, y, glLogger::r, glLogger::g, glLogger::b, str);
}
void glLogger::printText(float x, float y, int r, int g, int b, const char* str){
	GLint m_viewport[4];
	glGetIntegerv( GL_VIEWPORT, m_viewport );
	float lineHeight = glLogger::lineHeight/m_viewport[3];

	int j = strlen(str);
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	for( int i = 0; i < j; i++ ) {
		if(str[i] == '\n'){
			y -= lineHeight;
			glRasterPos2f(x, y);
		}else{
			glutBitmapCharacter(glLogger::font, str[i]);
		}
	}
}
int glLogger::render(bool clear){
	glLogger::printText(-0.4, 0.38, glLogger::contents.c_str());
	if(clear)
		glLogger::contents.clear();
}

long glLogger::getTimestampInms(){
	struct timeval  tv;
	gettimeofday(&tv, NULL);

	return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
}

long glLogger::getTimestampInus(){
	struct timeval  tv;
	gettimeofday(&tv, NULL);

	return (tv.tv_sec) * 1000000 + (tv.tv_usec) ;
}
