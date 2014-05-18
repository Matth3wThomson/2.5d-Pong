#pragma once

#include "RenderObject.h"


RenderObject::RenderObject(void)	{
	mesh	= NULL;
	shader	= NULL;
	texture = NULL;
	parent  = NULL;
}

RenderObject::RenderObject(Mesh*m, Shader*s, GLuint t) {
	mesh	= m;
	shader	= s;
	texture = t;
	parent  = NULL;
}


RenderObject::~RenderObject(void)
{

}

RenderObject::RenderObject(const RenderObject& rhs) :
texture(rhs.texture), modelMatrix(rhs.modelMatrix), worldTransform(rhs.worldTransform),
	children(rhs.children)
{
	this->mesh = rhs.mesh;
	this->shader = rhs.shader;
	this->parent = rhs.parent;
}


void RenderObject::Update(float msec) {
	if(parent) {
		worldTransform = parent->modelMatrix * modelMatrix;
	}
	else {
		worldTransform = modelMatrix;
	}

	for(vector<RenderObject*>::const_iterator i = children.begin(); i != children.end(); ++i ) {
		(*i)->Update(msec);
	}
}

void RenderObject::Draw() const {
	if(mesh) {
		mesh->Draw();
	}
}