#pragma once

#include "SRenderer.h"


SRenderer::SRenderer(){
	::SRenderer(720, 1280, false);
}

SRenderer::SRenderer(int width, int height, bool orthographic)
{
	this->width = width;
	this->height = height;
	this->ortho = orthographic;

	if (orthographic){
		projMatrix = Matrix4::Orthographic(-1,1, (float) width ,0,0, (float) height);
		viewMatrix.ToIdentity();
	} else {
		projMatrix = Matrix4::Perspective(1.0f,1000.0f,
			(float)width/(float)height,45.0f);
		viewMatrix = Matrix4::Translation(Vector3(0, -7, -20));
	}

	cameraMoved = true;
	specularity = 50.0f;
}

SRenderer::~SRenderer(void)
{
}

void SRenderer::RenderScene() {
	for(vector<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i ) {
		Render(*(*i));
	}
}

void	SRenderer::Render(const RenderObject &o) {
	modelMatrix = o.GetWorldTransform();

	glPatchParameteri(GL_PATCH_VERTICES, 4);

	glDisable(GL_CULL_FACE);

	if(o.GetShader() && o.GetMesh()) {
		GLuint program = o.GetShader()->GetShaderProgram();
		glUseProgram(program);

		UpdateShaderMatrices(program);

		ApplyShaderLight(program);

		UploadUniforms(program);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, o.GetTexture());

		o.Draw();
	}



	for(vector<RenderObject*>::const_iterator i = o.GetChildren().begin(); i != o.GetChildren().end(); ++i ) {
		Render(*(*i));
	}
}



void	SRenderer::UpdateScene(float msec) {
	for(vector<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i ) {
		(*i)->Update(msec);
	}
}

void SRenderer::UpdateShaderMatrices(GLuint program)	{
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix")  , 1, false, (float*)&modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix")   , 1, false, (float*)&viewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix")   , 1, false, (float*)&projMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "textureMatrix"), 1, false, (float*)&textureMatrix);
}
