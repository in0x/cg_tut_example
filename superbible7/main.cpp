#include "stdafx.h"

#include "ShaderUtil.h"
#include "Mesh.h"

class color_change_app : public sb7::application
{
public:

	color_change_app() 
	{
	}

	void createProgram(std::string vertPath, std::string fragPath)
	{
		GLuint vert = ShaderUtil::createShader(vertPath, GL_VERTEX_SHADER);
		GLuint frag = ShaderUtil::createShader(fragPath, GL_FRAGMENT_SHADER);

		m_progHandle = glCreateProgram();

		glAttachShader(m_progHandle, vert);
		glAttachShader(m_progHandle, frag);

		glLinkProgram(m_progHandle);

		glDeleteShader(vert);
		glDeleteShader(frag);

		if (m_progHandle == 0)
		{
			throw std::exception("Failed to create program");
		}
	}

	void startup()
	{
		createProgram("lambert.vert", "lambert.frag");
		
		m_modelTf = glm::mat4{1.0};
		m_viewTf = glm::lookAt(glm::vec3{ 0, 0, 2 }, glm::vec3{ 0,0,0 }, glm::vec3{ 0,1,0 });
		m_projectionTf = glm::perspective(glm::radians(90.f), ((float)info.windowWidth / (float)info.windowHeight), 0.5f, 20.f);

		m_scene.load("female_bust_OBJ.obj");

		glEnable(GL_CULL_FACE);
	}

	void onResize(int w, int h)
	{
		sb7::application::onResize(w, h);

		m_projectionTf = glm::perspective(glm::radians(90.f), ((float)info.windowWidth / (float)info.windowHeight), -5.f, 20.f);
	}

	void render(double currentTime)
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		GLfloat color[] = {  1.f, 1.f, 1.f, 1.f  };
		glClearBufferfv(GL_COLOR, 0, color);

		glUseProgram(m_progHandle);

		vmath::mat4 rotMat = vmath::rotate(0.f, (float)currentTime * 45.f, 0.f);
		
		glUniformMatrix4fv(2, 1, GL_FALSE, (GLfloat*)&rotMat);
		glUniformMatrix4fv(3, 1, GL_FALSE, (GLfloat*)&m_viewTf);
		glUniformMatrix4fv(4, 1, GL_FALSE, (GLfloat*)&m_projectionTf);

		m_scene.draw();

		glDisable(GL_MULTISAMPLE);
		glDisable(GL_DEPTH_TEST);
		
	}

	void shutdown()
	{
		glDeleteProgram(m_progHandle);
	}

private:
	GLuint m_progHandle;

	glm::mat4 m_modelTf;
	glm::mat4 m_viewTf;
	glm::mat4 m_projectionTf;

	std::vector< GLuint > sceneVAOs;

	AssimpScene m_scene;
};

DECLARE_MAIN(color_change_app);

int main()
{
	return 0;
}
