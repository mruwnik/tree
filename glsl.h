#ifndef GLSLLOADER_H
#define GLSLOADER_H

#include <iostream>
#include <GL/glew.h> 
/* loads shaders 
 * */
namespace treeShader{
	class shader{
		GLuint program;
		bool ready; 	// says whether this shader has a program ready
		GLuint vertexShader;
		GLuint fragmentShader;
		// displays info about how well the shaders were loaded
		void show_info_log(GLuint object,
				   PFNGLGETSHADERIVPROC glGet__iv,
				   PFNGLGETSHADERINFOLOGPROC glGet__InfoLog);
	public:
		shader();
		shader(const char* vShader, const char* pShader);
		GLuint load(GLenum type, const char *filename); 	// loads a shader. type says what kind (vertex or fragment)
		GLuint loadVertexShader(const char* filename, bool link = false);
		GLuint loadPixelShader(const char* filename, bool link = false);
		GLuint getVertexShader(){return vertexShader;}
		GLuint getPixelShader(){return fragmentShader;}
		GLuint makeProgram(GLuint vShader, GLuint pShader);
		GLuint getProgram(){return program;}

		void sendUniform1i(const char* s, GLuint i);
		void sendUniform1f(const char* s, float i);
		void begin();
		void end();
		~shader();
	};
};

#endif //GLSLLOADER_H

