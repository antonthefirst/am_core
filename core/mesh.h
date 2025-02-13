#pragma once
#include "container.h"

struct MeshVert
{
	float pos[3];
	float nor[3];
	float uv[2];
	float col[4];
	uint32_t data;
	uint32_t material;

	static size_t getStride();
	static size_t getOffPos();
	static size_t getOffNorm();
	static size_t getOffCol();
	static size_t getOffUv();
	static size_t getOffData();
	static size_t getOffMaterial();
};

struct Mesh {
	enum Type {
		TRIANGLES,
		LINES,
		POINTS,
	};
	Type                type = TRIANGLES;
	Bunch<MeshVert>     verts;
	Bunch<int> elems;

	Mesh() { }
	Mesh(Type _t) : type(_t) { }

	void addVert(const MeshVert& v);
	void addElem(unsigned int i);
	void clear();

	float*        getVertPtr() const; 
	size_t        getVertByteSize() const;
	unsigned int* getElemPtr() const;
	size_t        getElemByteSize() const;
	int           getElemCount() const;

};

inline void Mesh::addVert(const MeshVert& v)
{
	verts.push(v);
}
inline void Mesh::addElem(unsigned int i)
{
	elems.push(i);
}
inline void Mesh::clear() {
	verts.clear();
	elems.clear();
}
inline float* Mesh::getVertPtr() const
{
	return (float*)&verts[0];
}
inline size_t Mesh::getVertByteSize() const
{
	return verts.count * sizeof(MeshVert);
}
inline unsigned int* Mesh::getElemPtr() const
{
	return (unsigned int*)&elems[0];
}
inline size_t Mesh::getElemByteSize() const
{
	return elems.count * sizeof(unsigned int);
}
inline int Mesh::getElemCount() const
{
	return (int)elems.count;
}

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
inline size_t MeshVert::getStride() 
{
	return sizeof(MeshVert);
}
inline size_t MeshVert::getOffPos()
{
	return OFFSETOF(MeshVert, pos);
}
inline size_t MeshVert::getOffNorm()
{
	return OFFSETOF(MeshVert, nor);
}
inline size_t MeshVert::getOffCol()
{
	return OFFSETOF(MeshVert, col);
}
inline size_t MeshVert::getOffUv()
{
	return OFFSETOF(MeshVert, uv);
}
inline size_t MeshVert::getOffData()
{
	return OFFSETOF(MeshVert, data);
}
inline size_t MeshVert::getOffMaterial()
{
	return OFFSETOF(MeshVert, material);
}
#undef OFFSETOF