#include "glsl.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>

using namespace treeShader;

void shader::show_info_log(GLuint object,
			   PFNGLGETSHADERIVPROC glGet__iv,
			   PFNGLGETSHADERINFOLOGPROC glGet__InfoLog){
	GLint log_length;
	char *log;

	glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
	log = new char[log_length];
	glGet__InfoLog(object, log_length, NULL, log);
	std::cerr << log << std::endl;
	delete [] log;
}

shader::shader(): ready(false){}

shader::shader(const char* vShader, const char* pShader): ready(false){
	if(glewInit())
		std::cerr << "could not initialize GLEW\n";
	vertexShader = loadVertexShader(vShader);
	fragmentShader = loadPixelShader(pShader,true);
//	program = makeProgram(vertexShader, fragmentShader);
}


GLuint shader::load(GLenum type, const char *filename){ 	// loads a shader. type says what kind (vertex or fragment)
	std::cout << "loading " << filename << "\n";
	GLint length;

	std::ifstream t(filename);
	std::string source;

    t.seekg(0, std::ios::end);
    length = t.tellg();
	source.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	source.assign((std::istreambuf_iterator<char>(t)),
			            std::istreambuf_iterator<char>());


	GLuint shader;
	GLint shader_ok;

	if(source.empty())
		return 0;


	shader = glCreateShader(type);
	const char* sourceString = source.c_str();
	glShaderSource(shader, 1, (const GLchar**)&sourceString, &length);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
	if (!shader_ok) {
		std::cerr << "Failed to compile " << filename << ":\n";
		show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

GLuint shader::loadVertexShader(const char* filename, bool link){
	vertexShader = load(GL_VERTEX_SHADER,filename);
	if(link)
		program = makeProgram(vertexShader, fragmentShader);
	return vertexShader;
}

GLuint shader::loadPixelShader(const char* filename, bool link){
	fragmentShader = load(GL_FRAGMENT_SHADER,filename);
	if(link)
		program = makeProgram(vertexShader, fragmentShader);
	return fragmentShader;
}

GLuint shader::makeProgram(GLuint vShader, GLuint pShader){
	GLint program_ok;
	
	if(ready)
		glDeleteProgram(program);
	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, pShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
	if (!program_ok) {
		std::cerr << "Failed to link shader program:\n";
		show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
		glDeleteProgram(program);
		ready = false;
		return 0;
	}
//	ready = true;
	return program;
}

void shader::sendUniform1i(const char* s, GLuint i){
	glUniform1i(glGetUniformLocation(program, s),i);
}

void shader::sendUniform1f(const char* s, float i){
	glUniform1f(glGetUniformLocation(program, s),i);
}

void shader::begin(){
	glUseProgram(program);
}

void shader::end(){
	glUseProgram(0);
}

shader::~shader(){
	if(ready)
		glDeleteProgram(program);
}
