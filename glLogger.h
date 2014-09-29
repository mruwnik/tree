#ifndef GLLOGGER_H
#define GLLOGGER_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <sstream>
#include <sys/time.h>

class glLogger{
	static std::string contents;
	static int r;
	static int g;
	static int b;
	static void* font;
	static float lineHeight;
public:
	static void setColour(int r, int g, int b);
	static void setFont(void* font);
	static char putchar(const char c);
	static void putNum(long c);
	static void putNum(double c);
	static char* print(const char* str);
	static char* println(const char* str);
	static std::string& print(const std::string& str);
	static std::string& println(const std::string& str);
	static int render(bool clear = true);
	static void printText(float x, float y, const char* str);
	static void printText(float x, float y, int r, int g, int b, const char* str);
	static long getTimestampInms();
	static long getTimestampInus();
};
#endif // GLLOGGER_H
