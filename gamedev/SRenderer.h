#pragma once

#include "RenderObject.h"
#include "Matrix4.h"
#include "Matrix3.h"

struct Light {
	Vector3 position;
	float radius;
	Vector3 colour;
};

/**
* A Custom renderer class to handle the rendering of render objects
*/
class SRenderer
{
public:
	SRenderer();

	/**
	* Constructor for the Renderer Creation.
	* @param orthographic Whether or not this renderer is for orthographic elements
	*/
	SRenderer(int width, int height, bool orthgraphic = false);
	~SRenderer(void);

	/**
	* Renders all objects this renderer holds references to
	*/
	virtual void RenderScene();

	/**
	* Render a given renderObject
	*/
	virtual void Render(const RenderObject &o);

	/**
	* Update RenderObjects
	*/
	virtual void UpdateScene(float msec);

	inline void SetProjectionMatrix(Matrix4 m){	projMatrix = m;	}
	inline void SetViewMatrix(Matrix4 m){ viewMatrix = m; cameraMoved = true; }

	inline int GetWidth(){ return width; }
	inline int GetHeight(){ return height; }
	inline void SetSize(const int& x, const int& y){
		width = x;
		height = y;
		cameraMoved = true;

		if (ortho){
			projMatrix = Matrix4::Orthographic(-1,1, (float) width ,0,0, (float) height);
			viewMatrix.ToIdentity();
		} else {
			projMatrix = Matrix4::Perspective(1.0f,1000.0f,
				(float)width/(float)height,45.0f);
			viewMatrix = Matrix4::Translation(Vector3(0, -7, -20));
		}
	}

	void AddRenderObject(RenderObject &r){
		renderObjects.push_back(&r);
	}

	void RemoveRenderObject(const RenderObject &r){
		for (vector<RenderObject*>::const_iterator i; i != renderObjects.end(); ++i){
			if ((*i) == &r){
				renderObjects.erase(i);
			}
		}
	};

	void RemoveAllRenderObjects(){
		while (!renderObjects.empty()){
			renderObjects.pop_back();
		}
	}

	void SetShaderLight(const Vector3& position, const Vector3& colour, const float& radius){
		light.position = position;
		light.colour = colour;
		light.radius = radius;
	}

	void UploadUniforms(GLuint program){
		if (cameraMoved){
			//Calculates the cameraPos, only if the camera gets moved!
			Matrix3 rotation = Matrix3(viewMatrix);
			Vector3 invCamPos = viewMatrix.GetPositionVector();

			Vector3 camPos	= rotation * -invCamPos;

			//Sends the camera Pos to the shader
			glUniform3fv(glGetUniformLocation(program, "cameraPos"), 1, (float*) &camPos);

			cameraMoved = false;
		}

		//Send specularity to GPU!
		glUniform1f(glGetUniformLocation(program, "specularity"), specularity);
	};

protected:
	void UpdateShaderMatrices(GLuint program);

	void ApplyShaderLight(GLuint program){
		glUniform3fv(glGetUniformLocation(program, "lightColour"),
			1, (float*) &light.colour);
		glUniform3fv(glGetUniformLocation(program, "lightPos"),
			1, (float*) &light.position);
		glUniform1f(glGetUniformLocation(program, "lightRadius"),
			light.radius);
	}

	bool cameraMoved;

	int width, height;
	bool ortho;

	float specularity;

	Light light;

	vector<RenderObject*> renderObjects;
	Matrix4 modelMatrix;
	Matrix4 viewMatrix;
	Matrix4 projMatrix;
	Matrix4 textureMatrix;
};

