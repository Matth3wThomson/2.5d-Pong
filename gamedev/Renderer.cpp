#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{
	//modelMatrix = 	//Matrix4::Rotation(90,Vector3(1,0,0)) * 
	//	Matrix4::Scale(Vector3(100,100,100));

	viewMatrix = Matrix4::Rotation(70.0f, Vector3(1,0,0)) * 
		Matrix4::Rotation(-225.0f, Vector3(0,1,0)) * 
		Matrix4::Translation(Vector3(100,-400,100));

	projMatrix = Matrix4::Perspective(1.0f,1000.0f,
		(float)width/(float)height,45.0f);
}

Renderer::~Renderer(void){

}

GLuint Renderer::LoadTexture(string filename) {
	return 	SOIL_load_OGL_texture(filename.c_str(),SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS);
}

void	Renderer::RenderScene() {
	for(vector<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i ) {
		Render(*(*i));
	}
}

void	Renderer::Render(const RenderObject &o) {
	modelMatrix = o.GetWorldTransform();

	glPatchParameteri(GL_PATCH_VERTICES, 4);

	glDisable(GL_CULL_FACE);

	if(o.GetShader() && o.GetMesh()) {
		GLuint program = o.GetShader()->GetShaderProgram();
		glUseProgram(program);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, o.GetTexture());

		UpdateShaderMatrices(program);
		glUniform1i(glGetUniformLocation(program, "heightMap"), 0);

		o.Draw();
	}

	for(vector<RenderObject*>::const_iterator i = o.GetChildren().begin(); i != o.GetChildren().end(); ++i ) {
		Render(*(*i));
	}
}

void	Renderer::UpdateScene(float msec) {
	for(vector<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i ) {
		(*i)->Update(msec);
	}
}