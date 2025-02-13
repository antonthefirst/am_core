#pragma once
#include "math/maths.h"

// I feel like I'd like to isolate raw OpenGL definitions out of this, but maybe see how it goes.
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

struct TexDesc {
	ivec2 res                    = ivec2(0);
	unsigned int internal_format = GL_RGBA8;
	unsigned int data_format     = GL_RGBA;
	unsigned int data_type       = GL_UNSIGNED_BYTE;
	unsigned int filter_min      = GL_LINEAR;
	unsigned int filter_mag      = GL_LINEAR;
	unsigned int wrap_s          = GL_CLAMP_TO_EDGE;
	unsigned int wrap_t          = GL_CLAMP_TO_EDGE;
	bool operator==(const TexDesc& rhs) const;
	bool operator!=(const TexDesc& rhs) const;

	TexDesc() { }
	TexDesc(ivec2 _res) : res(_res.x, _res.y) { }

	TexDesc& setFormat(unsigned int _internal_format) {
		internal_format = _internal_format;
		return *this;
	}
	TexDesc& setFormat(unsigned int _internal_format, unsigned int _data_format, unsigned int _data_type) {
		internal_format = _internal_format;
		data_format = _data_format;
		data_type = _data_type;
		return *this;
	}
	TexDesc& setFilterMin(unsigned int f) { filter_min = f; return *this; }
	TexDesc& setFilterMag(unsigned int f) { filter_mag = f; return *this; }
};

// Set the parameters of the descriptor to the currently bound texture and fill it with data.
inline void setTexImageParams(TexDesc desc, void* data) {
	glTexImage2D(GL_TEXTURE_2D, 0, desc.internal_format, desc.res.x, desc.res.y, 0, desc.data_format, desc.data_type, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, desc.filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, desc.filter_mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, desc.wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, desc.wrap_t);
}
// Set the parameters of the descriptor to the currently bound texture.
inline void setTexStorageParams(TexDesc desc) {
	glTexStorage2D(GL_TEXTURE_2D, 1, desc.internal_format, desc.res.x, desc.res.y);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, desc.filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, desc.filter_mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, desc.wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, desc.wrap_t);
}

// If we want to do ownership and sizing etc, maybe do it here.
struct TexResource {

};