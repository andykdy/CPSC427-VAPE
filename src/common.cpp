#include "common.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../ext/stb_image/stb_image.h"

#define PHYFSPP_IMPL
#include "physfs.hpp"

// stlib
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <physfs.h>

void gl_flush_errors()
{
	while (glGetError() != GL_NO_ERROR);
}

bool gl_has_errors()
{
	GLenum error = glGetError();

	if (error == GL_NO_ERROR) return false;

	while (error != GL_NO_ERROR)
	{
		const char* error_str = "";
		switch (error)
		{
			case GL_INVALID_OPERATION:
			error_str = "INVALID_OPERATION";
			break;
			case GL_INVALID_ENUM:
			error_str = "INVALID_ENUM";
			break;
			case GL_INVALID_VALUE:
			error_str = "INVALID_VALUE";
			break;
			case GL_OUT_OF_MEMORY:
			error_str = "OUT_OF_MEMORY";
			break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
			error_str = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		}

		fprintf(stderr, "OpenGL: %s", error_str);
		error = glGetError();
	}

	return true;
}

float dot(vec2 l, vec2 r)
{
	return l.x * r.x + l.y * r.y;
}

float dot(vec3 l, vec3 r)
{
	return l.x * r.x + l.y * r.y + l.z * r.z;
}

vec2 add(vec2 a, vec2 b) { return { a.x+b.x, a.y+b.y }; }
vec2 sub(vec2 a, vec2 b) { return { a.x-b.x, a.y-b.y }; }
vec2 mul(vec2 a, float b) { return { a.x*b, a.y*b }; }
vec3 mul(mat3 m, vec3 v) { return {
  dot(vec3{m.c0.x, m.c1.x, m.c2.x}, v),
  dot(vec3{m.c0.y, m.c1.y, m.c2.y}, v),
  dot(vec3{m.c0.z, m.c1.z, m.c2.z}, v)
}; }
float sq_len(vec2 a) { return dot(a, a); }
float len(vec2 a) { return std::sqrt(sq_len(a)); }
vec2 to_vec2(vec3 v) { return { v.x, v.y }; }

mat3 mul(const mat3 & l, const mat3 & r)
{
	mat3 l_t = { { l.c0.x, l.c1.x, l.c2.x},
	{ l.c0.y, l.c1.y, l.c2.y } ,
	{ l.c0.z, l.c1.z, l.c2.z } };

	mat3 ret;
	ret.c0.x = dot(l_t.c0, r.c0);
	ret.c0.y = dot(l_t.c1, r.c0);
	ret.c0.z = dot(l_t.c2, r.c0);

	ret.c1.x = dot(l_t.c0, r.c1);
	ret.c1.y = dot(l_t.c1, r.c1);
	ret.c1.z = dot(l_t.c2, r.c1);

	ret.c2.x = dot(l_t.c0, r.c2);
	ret.c2.y = dot(l_t.c1, r.c2);
	ret.c2.z = dot(l_t.c2, r.c2);
	return ret;
}

vec2 normalize(vec2 v)
{
	float m = sqrtf(dot(v, v));
	return { v.x / m, v.y / m };
}

// https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
typedef int OutCode;

const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

OutCode ComputeOutCode(const vec2& point, const vec2& tl, const vec2& br)
{
	OutCode code;

	code = INSIDE;          // initialised as being inside of [[clip window]]

	if (point.x < tl.x)           // to the left of clip window
		code |= LEFT;
	else if (point.x > br.x)      // to the right of clip window
		code |= RIGHT;
	if (point.y < tl.y)           // below the clip window
		code |= BOTTOM;
	else if (point.y > br.y)      // above the clip window
		code |= TOP;

	return code;
}

bool CohenSutherlandLineClipAndDraw(const vec2& p0, const vec2& p1, const vec2& tl, const vec2& br) {
	// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
	OutCode outcode0 = ComputeOutCode(p0, tl, br);
	OutCode outcode1 = ComputeOutCode(p1, tl, br);
	bool accept = false;

	float x0 = p0.x;
	float y0 = p0.y;
	float x1 = p1.x;
	float y1 = p1.y;

	while (true) {
		if (!(outcode0 | outcode1)) {
			// bitwise OR is 0: both points inside window; trivially accept and exit loop
			return true;
		} else if (outcode0 & outcode1) {
			// bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
			// or BOTTOM), so both must be outside window; exit loop (accept is false)
			return false;
		} else {
			// failed both tests, so calculate the line segment to clip
			// from an outside point to an intersection with clip edge
			float x, y = 0;

			// At least one endpoint is outside the clip rectangle; pick it.
			OutCode outcodeOut = outcode0 ? outcode0 : outcode1;

			// Now find the intersection point;
			// use formulas:
			//   slope = (y1 - y0) / (x1 - x0)
			//   x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
			//   y = y0 + slope * (xm - x0), where xm is xmin or xmax
			// No need to worry about divide-by-zero because, in each case, the
			// outcode bit being tested guarantees the denominator is non-zero
			if (outcodeOut & TOP) {           // point is above the clip window
				x = x0 + (x1 - x0) * (br.y - y0) / (y1 - y0);
				y = br.y;
			} else if (outcodeOut & BOTTOM) { // point is below the clip window
				x = x0 + (x1 - x0) * (tl.y - y0) / (y1 - y0);
				y = tl.y;
			} else if (outcodeOut & RIGHT) {  // point is to the right of clip window
				y = y0 + (y1 - y0) * (br.x - x0) / (x1 - x0);
				x = br.x;
			} else if (outcodeOut & LEFT) {   // point is to the left of clip window
				y = y0 + (y1 - y0) * (tl.x - x0) / (x1 - x0);
				x = tl.x;
			}

			// Now we move outside point to intersection point to clip
			// and get ready for next pass.
			if (outcodeOut == outcode0) {
				x0 = x;
				y0 = y;
				outcode0 = ComputeOutCode({x0,y0}, tl, br);
			} else {
				x1 = x;
				y1 = y;
				outcode1 = ComputeOutCode({x1,y1}, tl, br);
			}
		}
	}
}

Texture::Texture()
{

}

Texture::~Texture()
{
	if (id != 0) glDeleteTextures(1, &id);
	if (depth_render_buffer_id != 0) glDeleteRenderbuffers(1, &depth_render_buffer_id);
}

bool Texture::load_from_file(const char* path)
{
	if (path == nullptr) 
		return false;
	if (!PHYSFS_exists(path))
	{
		std::cout << "Unable to find " << path << std::endl;
		return false; //file doesn't exist
	}
	Texture::path = std::string(path);

	PHYSFS_file* myfile = PHYSFS_openRead(path);

	// Get the lenght of the file
	auto m_size = PHYSFS_fileLength(myfile);

	// Get the file data.
	auto m_data = new uint8_t[m_size];

	auto length_read = PHYSFS_readBytes(myfile, m_data, static_cast<PHYSFS_uint64>(m_size));

	if (length_read != m_size)
	{
		std::cout << PHYSFS_getLastErrorCode() << std::endl;
		delete [] m_data;
		m_data = nullptr;
		return false;
	}

	PHYSFS_close(myfile);

	stbi_uc* data = stbi_load_from_memory(m_data, static_cast<int>(m_size), &width, &height, nullptr, 4);
	if (data == nullptr)
		return false;
	delete [] m_data;

	gl_flush_errors();
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data);
	return !gl_has_errors();
}

// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
bool Texture::create_from_screen(GLFWwindow const * const window) {
	gl_flush_errors();
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glfwGetFramebufferSize(const_cast<GLFWwindow *>(window), &width, &height);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Generate the render buffer with the depth buffer
	glGenRenderbuffers(1, &depth_render_buffer_id);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_render_buffer_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_render_buffer_id);

	// Set id as colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, id, 0);

	// Set the list of draw buffers
	GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, draw_buffers); // "1" is the size of DrawBuffers

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	return !gl_has_errors();
}

bool Texture::is_valid()const
{
	return id != 0;
}

void Texture::invalidate() {
	id = 0;
}

namespace
{
	bool gl_compile_shader(GLuint shader)
	{
		glCompileShader(shader);
		GLint success = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLint log_len;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
			std::vector<char> log(log_len);
			glGetShaderInfoLog(shader, log_len, &log_len, log.data());
			glDeleteShader(shader);

			fprintf(stderr, "GLSL: %s", log.data());
			return false;
		}

		return true;
	}
}

bool EntityOld::Effect::load_from_file(const char* vs_path, const char* fs_path)
{
	gl_flush_errors();

	// Opening files
	auto* vs_is = new PhysFS::ifstream(vs_path);
	auto* fs_is = new PhysFS::ifstream(fs_path);

	if (!vs_is->good() || !fs_is->good())
	{
		fprintf(stderr, "Failed to load shader files %s, %s", vs_path, fs_path);
		return false;
	}

	// Reading sources
	std::stringstream vs_ss, fs_ss;
	vs_ss << vs_is->rdbuf();
	fs_ss << fs_is->rdbuf();
	delete vs_is;
	delete fs_is;
	std::string vs_str = vs_ss.str();
	std::string fs_str = fs_ss.str();
	const char* vs_src = vs_str.c_str();
	const char* fs_src = fs_str.c_str();
	GLsizei vs_len = (GLsizei)vs_str.size();
	GLsizei fs_len = (GLsizei)fs_str.size();

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vs_src, &vs_len);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fs_src, &fs_len);

	// Compiling
	// Shaders already delete if compilation fails
	if (!gl_compile_shader(vertex))
		return false;

	if (!gl_compile_shader(fragment))
	{
		glDeleteShader(vertex);
		return false;
	}

	// Linking
	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	{
		GLint is_linked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
		if (is_linked == GL_FALSE)
		{
			GLint log_len;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
			std::vector<char> log(log_len);
			glGetProgramInfoLog(program, log_len, &log_len, log.data());

			release();
			fprintf(stderr, "Link error: %s", log.data());
			return false;
		}
	}

	if (gl_has_errors())
	{
		release();
		fprintf(stderr, "OpenGL errors occured while compiling Effect");
		return false;
	}

    if (vertex != 0) {
        glDeleteShader(vertex);
        vertex = 0;
    }
    if (fragment != 0) {
        glDeleteShader(fragment);
        fragment = 0;
    }

	return true;
}

void EntityOld::Effect::release()
{
    if (program != 0) {
        glDeleteProgram(program);
        program = 0;
    }
    if (vertex != 0) {
        glDeleteShader(vertex);
        vertex = 0;
    }
    if (fragment != 0) {
        glDeleteShader(fragment);
        fragment = 0;
    }
}

void EntityOld::Transform::begin()
{
	out = { { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f}, { 0.f, 0.f, 1.f} };
}

void EntityOld::Transform::scale(vec2 scale)
{
	mat3 S = { { scale.x, 0.f, 0.f },{ 0.f, scale.y, 0.f },{ 0.f, 0.f, 1.f } };
	out = mul(out, S);
}

void EntityOld::Transform::rotate(float radians)
{
	float c = cosf(radians);
	float s = sinf(radians);
	mat3 R = { { c, s, 0.f },{ -s, c, 0.f },{ 0.f, 0.f, 1.f } };
	out = mul(out, R);
}

void EntityOld::Transform::translate(vec2 offset)
{
	mat3 T = { { 1.f, 0.f, 0.f },{ 0.f, 1.f, 0.f },{ offset.x, offset.y, 1.f } };
	out = mul(out, T);
}

void EntityOld::Transform::end()
{
	//
}
