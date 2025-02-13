#pragma once

#include "mesh.h"
#include "math/pose3.h"
#include "container.h"

struct MeshMaker {
	Mesh* m = nullptr;
	unsigned int elem_offset = 0;
	Bunch<pose3> stack;

	MeshMaker(Mesh* _m) : m(_m) { }

	void shape();

	void push(pose3 p);
	void pop();
	pose3 top(); // Return the matrix at the top of the stack, or identity if none.

	void vert(float x, float y, float r, float g, float b, float a, float u, float v);
	void vert(float x, float y, float z, float nx, float ny, float nz, float r, float g, float b, float a, float u, float v);
	void vert(vec3 p, vec3 n, vec4 col, vec2 uv);
	void vert(vec3 p, vec3 n, vec4 col, vec2 uv, uint32_t data, uint32_t material);

	void elem(unsigned int a);
	void elem(unsigned int a, unsigned int b);
	void elem(unsigned int a, unsigned int b, unsigned int c);

	void clear();
};

inline void MeshMaker::shape() {
	elem_offset = int(m->verts.count);
}

inline void MeshMaker::push(pose3 p) {
	if (stack.count)
		stack.push(stack.back() * p);
	else
		stack.push(p);
}

inline void MeshMaker::pop() {
	ASSERT(stack.count);
	stack.pop();
}
pose3 MeshMaker::top() {
	if (stack.count)
		return stack.back();
	else
		return pose3::I();
}

inline void MeshMaker::vert(float x, float y, float r, float g, float b, float a, float u, float v) {
	vec3 p = vec3(x,y,0.0f);
	if (stack.count)
		p = xfmVec(stack.back(), p);
	uint32_t data = 0;
	uint32_t material = 0;
	m->addVert({ { p.x,p.y,p.z }, {0,0,1}, { u,v },{ r,g,b,a }, data, material });
}

inline void MeshMaker::vert(float x, float y, float z, float nx, float ny, float nz, float r, float g, float b, float a, float u, float v) {
	vec3 p = vec3(x,y,z);
	if (stack.count)
		p = xfmVec(stack.back(), p);	
	uint32_t data = 0;
	uint32_t material = 0;
	m->addVert({ { p.x,p.y,p.z },{ nx,ny,nz },{ u,v },{ r,g,b,a }, data, material });
}

inline void MeshMaker::vert(vec3 p, vec3 n, vec4 col, vec2 uv) {
	if (stack.count) {
		p = xfmVec(stack.back(), p);	
		// #TODO! n = xfmNor(stack.back(), n);
	}
	uint32_t data = 0;
	uint32_t material = 0;
	m->addVert({ { p.x,p.y,p.z },{ n.x,n.y,n.z },{ uv.x,uv.y },{ col.x,col.y,col.z,col.w }, data, material });
}

inline void MeshMaker::vert(vec3 p, vec3 n, vec4 col, vec2 uv, uint32_t data, uint32_t material) {
	if (stack.count) {
		p = xfmVec(stack.back(), p);	
		// #TODO! n = xfmNor(stack.back(), n);
	}
	m->addVert({ { p.x,p.y,p.z },{ n.x,n.y,n.z },{ uv.x,uv.y },{ col.x,col.y,col.z,col.w }, data, material });
}

inline void MeshMaker::elem(unsigned int a) {
	m->addElem(elem_offset + a);
}

inline void MeshMaker::elem(unsigned int a, unsigned int b) {
	m->addElem(elem_offset + a);
	m->addElem(elem_offset + b);
}

inline void MeshMaker::elem(unsigned int a, unsigned int b, unsigned int c) {
	m->addElem(elem_offset + a);
	m->addElem(elem_offset + b);
	m->addElem(elem_offset + c);
}

inline void MeshMaker::clear() {
	m->clear();
}