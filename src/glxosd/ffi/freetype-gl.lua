--[[
Copyright (C) 2016 Nick Guletskii

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This is a combination of many header files from the freetype-gl project,
therefore the following license applies:

/* =========================================================================
 * Freetype GL - A C OpenGL Freetype engine
 * Platform:    Any
 * WWW:         https://github.com/rougier/freetype-gl
 * -------------------------------------------------------------------------
 * Copyright 2011,2012 Nicolas P. Rougier. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY NICOLAS P. ROUGIER ''AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL NICOLAS P. ROUGIER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of Nicolas P. Rougier.
 * ========================================================================= */
]]
local ffi = require("ffi"); ffi.cdef[[/**
 * Tuple of 4 ints.
 *
 * Each field can be addressed using several aliases:
 *  - First component:  <b>x</b>, <b>r</b>, <b>red</b> or <b>vstart</b>
 *  - Second component: <b>y</b>, <b>g</b>, <b>green</b> or <b>vcount</b>
 *  - Third component:  <b>z</b>, <b>b</b>, <b>blue</b>, <b>width</b> or <b>istart</b>
 *  - Fourth component: <b>w</b>, <b>a</b>, <b>alpha</b>, <b>height</b> or <b>icount</b>
 *
 */
typedef union {
	int data[4]; /**< All compoments at once     */
	struct {
		int x; /**< Alias for first component  */
		int y; /**< Alias for second component */
		int z; /**< Alias for third component  */
		int w; /**< Alias for fourht component */
	};
	struct {
		int x_; /**< Alias for first component  */
		int y_; /**< Alias for second component */
		int width; /**< Alias for third component  */
		int height; /**< Alias for fourth component */
	};
	struct {
		int r; /**< Alias for first component  */
		int g; /**< Alias for second component */
		int b; /**< Alias for third component  */
		int a; /**< Alias for fourth component */
	};
	struct {
		int red; /**< Alias for first component  */
		int green; /**< Alias for second component */
		int blue; /**< Alias for third component  */
		int alpha; /**< Alias for fourth component */
	};
	struct {
		int vstart; /**< Alias for first component  */
		int vcount; /**< Alias for second component */
		int istart; /**< Alias for third component  */
		int icount; /**< Alias for fourth component */
	};
} ivec4;
/**
 * Tuple of 3 ints.
 *
 * Each field can be addressed using several aliases:
 *  - First component:  <b>x</b>, <b>r</b> or <b>red</b>
 *  - Second component: <b>y</b>, <b>g</b> or <b>green</b>
 *  - Third component:  <b>z</b>, <b>b</b> or <b>blue</b>
 *
 */
typedef union {
	int data[3]; /**< All compoments at once     */
	struct {
		int x; /**< Alias for first component  */
		int y; /**< Alias for second component */
		int z; /**< Alias for third component  */
	};
	struct {
		int r; /**< Alias for first component  */
		int g; /**< Alias for second component */
		int b; /**< Alias for third component  */
	};
	struct {
		int red; /**< Alias for first component  */
		int green; /**< Alias for second component */
		int blue; /**< Alias for third component  */
	};
} ivec3;

/**
 *  Generic vector structure.
 *
 * @memberof vector
 */
typedef struct vector_t {
	/** Pointer to dynamically allocated items. */
	void * items;

	/** Number of items that can be held in currently allocated storage. */
	size_t capacity;

	/** Number of items. */
	size_t size;

	/** Size (in bytes) of a single item. */
	size_t item_size;
} vector_t;

/**
 *  Generic vertex attribute.
 */
typedef struct vertex_attribute_t {
	/**
	 *  atribute name
	 */
	GLchar * name;

	/**
	 * index of the generic vertex attribute to be modified.
	 */
	GLuint index;

	/**
	 * Number of components per generic vertex attribute.
	 *
	 * Must be 1, 2, 3, or 4. The initial value is 4.
	 */
	GLint size;

	/**
	 *  data type of each component in the array.
	 *
	 *  Symbolic constants GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT,
	 *  GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, or GL_DOUBLE are
	 *  accepted. The initial value is GL_FLOAT.
	 */
	GLenum type;

	/**
	 *  whether fixed-point data values should be normalized (GL_TRUE) or
	 *  converted directly as fixed-point values (GL_FALSE) when they are
	 *  accessed.
	 */
	GLboolean normalized;

	/**
	 *  byte offset between consecutive generic vertex attributes.
	 *
	 *  If stride is 0, the generic vertex attributes are understood to be
	 *  tightly packed in the array. The initial value is 0.
	 */
	GLsizei stride;

	/**
	 *  pointer to the first component of the first attribute element in the
	 *  array.
	 */
	GLvoid * pointer;

	/**
	 * pointer to the function that enable this attribute.
	 */
	void (*enable)(void *);

} vertex_attribute_t;

/**
 * Generic vertex buffer.
 */
typedef struct vertex_buffer_t {
	/** Format of the vertex buffer. */
	char * format;

	/** Vector of vertices. */
	vector_t * vertices;

	/** GL identity of the Vertex Array Object */
	GLuint VAO_id;

	/** GL identity of the vertices buffer. */
	GLuint vertices_id;

	/** Vector of indices. */
	vector_t * indices;

	/** GL identity of the indices buffer. */
	GLuint indices_id;

	/** Current size of the vertices buffer in GPU */
	size_t GPU_vsize;

	/** Current size of the indices buffer in GPU*/
	size_t GPU_isize;

	/** GL primitives to render. */
	GLenum mode;

	/** Whether the vertex buffer needs to be uploaded to GPU memory. */
	char state;

	/** Individual items */
	vector_t * items;

	/** Array of attributes. */
	vertex_attribute_t *attributes[16];
} vertex_buffer_t;

/**
 * Tuple of 2 ints.
 *
 * Each field can be addressed using several aliases:
 *  - First component: <b>x</b>, <b>s</b> or <b>start</b>
 *  - Second component: <b>y</b>, <b>t</b> or <b>end</b>
 *
 */
typedef union {
	int data[2]; /**< All compoments at once     */
	struct {
		int x; /**< Alias for first component  */
		int y; /**< Alias for second component */
	};
	struct {
		int s; /**< Alias for first component  */
		int t; /**< Alias for second component */
	};
	struct {
		int start; /**< Alias for first component  */
		int end; /**< Alias for second component */
	};
} ivec2;

/**
 * Tuple of 4 floats.
 *
 * Each field can be addressed using several aliases:
 *  - First component:  <b>x</b>, <b>left</b>, <b>r</b> or <b>red</b>
 *  - Second component: <b>y</b>, <b>top</b>, <b>g</b> or <b>green</b>
 *  - Third component:  <b>z</b>, <b>width</b>, <b>b</b> or <b>blue</b>
 *  - Fourth component: <b>w</b>, <b>height</b>, <b>a</b> or <b>alpha</b>
 */
typedef union {
	float data[4]; /**< All compoments at once    */
	struct {
		float x; /**< Alias for first component */
		float y; /**< Alias for second component */
		float z; /**< Alias for third component  */
		float w; /**< Alias for fourth component */
	};
	struct {
		float left; /**< Alias for first component */
		float top; /**< Alias for second component */
		float width; /**< Alias for third component  */
		float height; /**< Alias for fourth component */
	};
	struct {
		float r; /**< Alias for first component */
		float g; /**< Alias for second component */
		float b; /**< Alias for third component  */
		float a; /**< Alias for fourth component */
	};
	struct {
		float red; /**< Alias for first component */
		float green; /**< Alias for second component */
		float blue; /**< Alias for third component  */
		float alpha; /**< Alias for fourth component */
	};
} vec4;

/**
 * Tuple of 3 floats
 *
 * Each field can be addressed using several aliases:
 *  - First component:  <b>x</b>, <b>r</b> or <b>red</b>
 *  - Second component: <b>y</b>, <b>g</b> or <b>green</b>
 *  - Third component:  <b>z</b>, <b>b</b> or <b>blue</b>
 */
typedef union {
	float data[3]; /**< All compoments at once    */
	struct {
		float x; /**< Alias for first component */
		float y; /**< Alias fo second component */
		float z; /**< Alias fo third component  */
	};
	struct {
		float r; /**< Alias for first component */
		float g; /**< Alias fo second component */
		float b; /**< Alias fo third component  */
	};
	struct {
		float red; /**< Alias for first component */
		float green; /**< Alias fo second component */
		float blue; /**< Alias fo third component  */
	};
} vec3;

/**
 * Tuple of 2 floats
 *
 * Each field can be addressed using several aliases:
 *  - First component:  <b>x</b> or <b>s</b>
 *  - Second component: <b>y</b> or <b>t</b>
 */
typedef union {
	float data[2]; /**< All components at once     */
	struct {
		float x; /**< Alias for first component  */
		float y; /**< Alias for second component */
	};
	struct {
		float s; /**< Alias for first component  */
		float t; /**< Alias for second component */
	};
} vec2;

typedef union {
	float data[16]; /**< All compoments at once     */
	struct {
		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
		float m20, m21, m22, m23;
		float m30, m31, m32, m33;
	};
} mat4;

/**
 * A texture atlas is used to pack several small regions into a single texture.
 */
typedef struct texture_atlas_t {
	/**
	 * Allocated nodes
	 */
	vector_t * nodes;

	/**
	 *  Width (in pixels) of the underlying texture
	 */
	size_t width;

	/**
	 * Height (in pixels) of the underlying texture
	 */
	size_t height;

	/**
	 * Depth (in bytes) of the underlying texture
	 */
	size_t depth;

	/**
	 * Allocated surface size
	 */
	size_t used;

	/**
	 * Texture identity (OpenGL)
	 */
	unsigned int id;

	/**
	 * Atlas data
	 */
	unsigned char * data;

} texture_atlas_t;

/**
 * A structure that hold a kerning value relatively to a Unicode
 * codepoint.
 *
 * This structure cannot be used alone since the (necessary) right
 * Unicode codepoint is implicitely held by the owner of this structure.
 */
typedef struct kerning_t {
	/**
	 * Left Unicode codepoint in the kern pair in UTF-32 LE encoding.
	 */
	uint32_t codepoint;

	/**
	 * Kerning value (in fractional pixels).
	 */
	float kerning;

} kerning_t;

/*
 * Glyph metrics:
 * --------------
 *
 *                       xmin                     xmax
 *                        |                         |
 *                        |<-------- width -------->|
 *                        |                         |
 *              |         +-------------------------+----------------- ymax
 *              |         |    ggggggggg   ggggg    |     ^        ^
 *              |         |   g:::::::::ggg::::g    |     |        |
 *              |         |  g:::::::::::::::::g    |     |        |
 *              |         | g::::::ggggg::::::gg    |     |        |
 *              |         | g:::::g     g:::::g     |     |        |
 *    offset_x -|-------->| g:::::g     g:::::g     |  offset_y    |
 *              |         | g:::::g     g:::::g     |     |        |
 *              |         | g::::::g    g:::::g     |     |        |
 *              |         | g:::::::ggggg:::::g     |     |        |
 *              |         |  g::::::::::::::::g     |     |      height
 *              |         |   gg::::::::::::::g     |     |        |
 *  baseline ---*---------|---- gggggggg::::::g-----*--------      |
 *            / |         |             g:::::g     |              |
 *     origin   |         | gggggg      g:::::g     |              |
 *              |         | g:::::gg   gg:::::g     |              |
 *              |         |  g::::::ggg:::::::g     |              |
 *              |         |   gg:::::::::::::g      |              |
 *              |         |     ggg::::::ggg        |              |
 *              |         |         gggggg          |              v
 *              |         +-------------------------+----------------- ymin
 *              |                                   |
 *              |------------- advance_x ---------->|
 */

/**
 * A structure that describe a glyph.
 */
typedef struct texture_glyph_t {
	/**
	 * Unicode codepoint this glyph represents in UTF-32 LE encoding.
	 */
	uint32_t codepoint;

	/**
	 * Glyph's width in pixels.
	 */
	size_t width;

	/**
	 * Glyph's height in pixels.
	 */
	size_t height;

	/**
	 * Glyph's left bearing expressed in integer pixels.
	 */
	int offset_x;

	/**
	 * Glyphs's top bearing expressed in integer pixels.
	 *
	 * Remember that this is the distance from the baseline to the top-most
	 * glyph scanline, upwards y coordinates being positive.
	 */
	int offset_y;

	/**
	 * For horizontal text layouts, this is the horizontal distance (in
	 * fractional pixels) used to increment the pen position when the glyph is
	 * drawn as part of a string of text.
	 */
	float advance_x;

	/**
	 * For vertical text layouts, this is the vertical distance (in fractional
	 * pixels) used to increment the pen position when the glyph is drawn as
	 * part of a string of text.
	 */
	float advance_y;

	/**
	 * First normalized texture coordinate (x) of top-left corner
	 */
	float s0;

	/**
	 * Second normalized texture coordinate (y) of top-left corner
	 */
	float t0;

	/**
	 * First normalized texture coordinate (x) of bottom-right corner
	 */
	float s1;

	/**
	 * Second normalized texture coordinate (y) of bottom-right corner
	 */
	float t1;

	/**
	 * A vector of kerning pairs relative to this glyph.
	 */
	vector_t * kerning;

	/**
	 * Glyph outline type (0 = None, 1 = line, 2 = inner, 3 = outer)
	 */
	int outline_type;

	/**
	 * Glyph outline thickness
	 */
	float outline_thickness;

} texture_glyph_t;

/**
 *  Texture font structure.
 */
typedef struct texture_font_t {
	/**
	 * Vector of glyphs contained in this font.
	 */
	vector_t * glyphs;

	/**
	 * Atlas structure to store glyphs data.
	 */
	texture_atlas_t * atlas;

	/**
	 * font location
	 */
	enum {
		TEXTURE_FONT_FILE = 0, TEXTURE_FONT_MEMORY,
	} location;

	union {
		/**
		 * Font filename, for when location == TEXTURE_FONT_FILE
		 */
		char *filename;

		/**
		 * Font memory address, for when location == TEXTURE_FONT_MEMORY
		 */
		struct {
			const void *base;
			size_t size;
		} memory;
	};

	/**
	 * Font size
	 */
	float size;

	/**
	 * Whether to use autohint when rendering font
	 */
	int hinting;

	/**
	 * Outline type (0 = None, 1 = line, 2 = inner, 3 = outer)
	 */
	int outline_type;

	/**
	 * Outline thickness
	 */
	float outline_thickness;

	/**
	 * Whether to use our own lcd filter.
	 */
	int filtering;

	/**
	 * LCD filter weights
	 */
	unsigned char lcd_weights[5];

	/**
	 * Whether to use kerning if available
	 */
	int kerning;

	/**
	 * This field is simply used to compute a default line spacing (i.e., the
	 * baseline-to-baseline distance) when writing text with this font. Note
	 * that it usually is larger than the sum of the ascender and descender
	 * taken as absolute values. There is also no guarantee that no glyphs
	 * extend above or below subsequent baselines when using this distance.
	 */
	float height;

	/**
	 * This field is the distance that must be placed between two lines of
	 * text. The baseline-to-baseline distance should be computed as:
	 * ascender - descender + linegap
	 */
	float linegap;

	/**
	 * The ascender is the vertical distance from the horizontal baseline to
	 * the highest 'character' coordinate in a font face. Unfortunately, font
	 * formats define the ascender differently. For some, it represents the
	 * ascent of all capital latin characters (without accents), for others it
	 * is the ascent of the highest accented character, and finally, other
	 * formats define it as being equal to bbox.yMax.
	 */
	float ascender;

	/**
	 * The descender is the vertical distance from the horizontal baseline to
	 * the lowest 'character' coordinate in a font face. Unfortunately, font
	 * formats define the descender differently. For some, it represents the
	 * descent of all capital latin characters (without accents), for others it
	 * is the ascent of the lowest accented character, and finally, other
	 * formats define it as being equal to bbox.yMin. This field is negative
	 * for values below the baseline.
	 */
	float descender;

	/**
	 * The position of the underline line for this face. It is the center of
	 * the underlining stem. Only relevant for scalable formats.
	 */
	float underline_position;

	/**
	 * The thickness of the underline for this face. Only relevant for scalable
	 * formats.
	 */
	float underline_thickness;

} texture_font_t;

/**
 * Simple structure that describes text properties.
 */
typedef struct markup_t {
	/**
	 * A font family name such as "normal", "sans", "serif" or "monospace".
	 */
	char * family;

	/**
	 * Font size.
	 */
	float size;

	/**
	 * Whether text is bold.
	 */
	int bold;

	/**
	 * Whether text is italic.
	 */
	int italic;

	/**
	 * Vertical displacement from the baseline.
	 */
	float rise;

	/**
	 * Spacing between letters.
	 */
	float spacing;

	/**
	 * Gamma correction.
	 */
	float gamma;

	/**
	 * Text color.
	 */
	vec4 foreground_color;

	/**
	 * Background color.
	 */
	vec4 background_color;

	/**
	 * Whether outline is active.
	 */
	int outline;

	/**
	 * Outline color.
	 */
	vec4 outline_color;

	/**
	 * Whether underline is active.
	 */
	int underline;

	/**
	 * Underline color.
	 */
	vec4 underline_color;

	/**
	 * Whether overline is active.
	 */
	int overline;

	/**
	 * Overline color.
	 */
	vec4 overline_color;

	/**
	 * Whether strikethrough is active.
	 */
	int strikethrough;

	/**
	 * Strikethrough color.
	 */
	vec4 strikethrough_color;

	/**
	 * Pointer on the corresponding font (family/size/bold/italic)
	 */
	texture_font_t * font;

} markup_t;

/**
 * Structure in charge of caching fonts.
 */
typedef struct font_manager_t {
	/**
	 * Texture atlas to hold font glyphs.
	 */
	texture_atlas_t * atlas;

	/**
	 * Cached textures.
	 */
	vector_t * fonts;

	/**
	 * Default glyphs to be loaded when loading a new font.
	 */
	char * cache;

} font_manager_t;

/**
 * Text buffer structure
 */
typedef struct text_buffer_t {
	/**
	 * Vertex buffer
	 */
	vertex_buffer_t *buffer;

	/**
	 * Font manager
	 */
	font_manager_t *manager;

	/**
	 * Base color for text
	 */
	vec4 base_color;

	/**
	 * Pen origin
	 */
	vec2 origin;

	/**
	 * Last pen y location
	 */
	float last_pen_y;

	/**
	 * Total bounds
	 */
	vec4 bounds;

	/**
	 * Index (in the vertex buffer) of the current line start
	 */
	size_t line_start;

	/**
	 * Location of the start of the line
	 */
	float line_left;

	/**
	 * Vector of line information
	 */
	vector_t * lines;

	/**
	 * Current line ascender
	 */
	float line_ascender;

	/**
	 * Current line decender
	 */
	float line_descender;

	/**
	 * Shader handler
	 */
	GLuint shader;

	/**
	 * Shader "texture" location
	 */
	GLuint shader_texture;

	/**
	 * Shader "pixel" location
	 */
	GLuint shader_pixel;

} text_buffer_t;

/**
 * Glyph vertex structure
 */
typedef struct glyph_vertex_t {
	/**
	 * Vertex x coordinates
	 */
	float x;

	/**
	 * Vertex y coordinates
	 */
	float y;

	/**
	 * Vertex z coordinates
	 */
	float z;

	/**
	 * Texture first coordinate
	 */
	float u;

	/**
	 * Texture second coordinate
	 */
	float v;

	/**
	 * Color red component
	 */
	float r;

	/**
	 * Color green component
	 */
	float g;

	/**
	 * Color blue component
	 */
	float b;

	/**
	 * Color alpha component
	 */
	float a;

	/**
	 * Shift along x
	 */
	float shift;

	/**
	 * Color gamma correction
	 */
	float gamma;

} glyph_vertex_t;

mat4 * glxosd_ftgl_mat4_new(void);

void glxosd_ftgl_mat4_set_identity(mat4 *self);

void glxosd_ftgl_mat4_set_zero(mat4 *self);

void glxosd_ftgl_mat4_multiply(mat4 *self, mat4 *other);

void glxosd_ftgl_mat4_set_orthographic(mat4 *self, float left, float right, float bottom,
		float top, float znear, float zfar);

void glxosd_ftgl_mat4_set_perspective(mat4 *self, float fovy, float aspect, float zNear,
		float zFar);

void glxosd_ftgl_mat4_set_frustum(mat4 *self, float left, float right, float bottom, float top,
		float znear, float zfar);

void glxosd_ftgl_mat4_set_rotation(mat4 *self, float angle, float x, float y, float z);

void glxosd_ftgl_mat4_set_translation(mat4 *self, float x, float y, float z);

void glxosd_ftgl_mat4_set_scaling(mat4 *self, float x, float y, float z);

void glxosd_ftgl_mat4_rotate(mat4 *self, float angle, float x, float y, float z);

void glxosd_ftgl_mat4_translate(mat4 *self, float x, float y, float z);

void glxosd_ftgl_mat4_scale(mat4 *self, float x, float y, float z);

/**
 * Creates a new empty vector.
 *
 * @param   item_size    item size in bytes
 * @return               a new empty vector
 *
 */
vector_t * glxosd_ftgl_vector_new(size_t item_size);

/**
 *  Deletes a vector.
 *
 *  @param self a vector structure
 *
 */
void glxosd_ftgl_vector_delete(vector_t *self);

/**
 *  Returns a pointer to the item located at specified index.
 *
 *  @param  self  a vector structure
 *  @param  index the index of the item to be returned
 *  @return       pointer on the specified item
 */
const void * glxosd_ftgl_vector_get(const vector_t *self, size_t index);

/**
 *  Returns a pointer to the first item.
 *
 *  @param  self  a vector structure
 *  @return       pointer on the first item
 */
const void * glxosd_ftgl_vector_front(const vector_t *self);

/**
 *  Returns a pointer to the last item
 *
 *  @param  self  a vector structure
 *  @return pointer on the last item
 */
const void * glxosd_ftgl_vector_back(const vector_t *self);

/**
 *  Check if an item is contained within the vector.
 *
 *  @param  self  a vector structure
 *  @param  item  item to be searched in the vector
 *  @param  cmp   a pointer a comparison function
 *  @return       1 if item is contained within the vector, 0 otherwise
 */
int
vector_contains(const vector_t *self, const void *item,
		int (*cmp)(const void *, const void *));

/**
 *  Checks whether the vector is empty.
 *
 *  @param  self  a vector structure
 *  @return       1 if the vector is empty, 0 otherwise
 */
int glxosd_ftgl_vector_empty(const vector_t *self);

/**
 *  Returns the number of items
 *
 *  @param  self  a vector structure
 *  @return       number of items
 */
size_t glxosd_ftgl_vector_size(const vector_t *self);

/**
 *  Reserve storage such that it can hold at last size items.
 *
 *  @param  self  a vector structure
 *  @param  size  the new storage capacity
 */
void glxosd_ftgl_vector_reserve(vector_t *self, const size_t size);

/**
 *  Returns current storage capacity
 *
 *  @param  self  a vector structure
 *  @return       storage capacity
 */
size_t glxosd_ftgl_vector_capacity(const vector_t *self);

/**
 *  Decrease capacity to fit actual size.
 *
 *  @param  self  a vector structure
 */
void glxosd_ftgl_vector_shrink(vector_t *self);

/**
 *  Removes all items.
 *
 *  @param  self  a vector structure
 */
void glxosd_ftgl_vector_clear(vector_t *self);

/**
 *  Replace an item.
 *
 *  @param  self  a vector structure
 *  @param  index the index of the item to be replaced
 *  @param  item  the new item
 */
void glxosd_ftgl_vector_set(vector_t *self, const size_t index, const void *item);

/**
 *  Erase an item.
 *
 *  @param  self  a vector structure
 *  @param  index the index of the item to be erased
 */
void glxosd_ftgl_vector_erase(vector_t *self, const size_t index);

/**
 *  Erase a range of items.
 *
 *  @param  self  a vector structure
 *  @param  first the index of the first item to be erased
 *  @param  last  the index of the last item to be erased
 */
void glxosd_ftgl_vector_erase_range(vector_t *self, const size_t first, const size_t last);

/**
 *  Appends given item to the end of the vector.
 *
 *  @param  self a vector structure
 *  @param  item the item to be inserted
 */
void glxosd_ftgl_vector_push_back(vector_t *self, const void *item);

/**
 *  Removes the last item of the vector.
 *
 *  @param  self a vector structure
 */
void glxosd_ftgl_vector_pop_back(vector_t *self);

/**
 *  Resizes the vector to contain size items
 *
 *  If the current size is less than size, additional items are appended and
 *  initialized with value. If the current size is greater than size, the
 *  vector is reduced to its first size elements.
 *
 *  @param  self a vector structure
 *  @param  size the new size
 */
void glxosd_ftgl_vector_resize(vector_t *self, const size_t size);

/**
 *  Insert a single item at specified index.
 *
 *  @param  self  a vector structure
 *  @param  index location before which to insert item
 *  @param  item  the item to be inserted
 */
void glxosd_ftgl_vector_insert(vector_t *self, const size_t index, const void *item);

/**
 *  Insert raw data at specified index.
 *
 *  @param  self  a vector structure
 *  @param  index location before which to insert item
 *  @param  data  a pointer to the items to be inserted
 *  @param  count the number of items to be inserted
 */
void glxosd_ftgl_vector_insert_data(vector_t *self, const size_t index, const void * data,
		const size_t count);

/**
 *  Append raw data to the end of the vector.
 *
 *  @param  self  a vector structure
 *  @param  data  a pointer to the items to be inserted
 *  @param  count the number of items to be inserted
 */
void glxosd_ftgl_vector_push_back_data(vector_t *self, const void * data, const size_t count);

/**
 *  Sort vector items according to cmp function.
 *
 *  @param  self  a vector structure
 *  @param  cmp   a pointer a comparison function
 */
void glxosd_ftgl_vector_sort(vector_t *self, int (*cmp)(const void *, const void *));

/**
 * Creates a new empty texture atlas.
 *
 * @param   width   width of the atlas
 * @param   height  height of the atlas
 * @param   depth   bit depth of the atlas
 * @return          a new empty texture atlas.
 *
 */
texture_atlas_t * glxosd_ftgl_texture_atlas_new(const size_t width, const size_t height, const size_t depth);

/**
 *  Deletes a texture atlas.
 *
 *  @param self a texture atlas structure
 *
 */
void glxosd_ftgl_texture_atlas_delete(texture_atlas_t * self);

/**
 *  Upload atlas to video memory.
 *
 *  @param self a texture atlas structure
 *
 */
void glxosd_ftgl_texture_atlas_upload(texture_atlas_t * self);

/**
 *  Allocate a new region in the atlas.
 *
 *  @param self   a texture atlas structure
 *  @param width  width of the region to allocate
 *  @param height height of the region to allocate
 *  @return       Coordinates of the allocated region
 *
 */
ivec4 glxosd_ftgl_texture_atlas_get_region(texture_atlas_t * self, const size_t width,
		const size_t height);

/**
 *  Upload data to the specified atlas region.
 *
 *  @param self   a texture atlas structure
 *  @param x      x coordinate the region
 *  @param y      y coordinate the region
 *  @param width  width of the region
 *  @param height height of the region
 *  @param data   data to be uploaded into the specified region
 *  @param stride stride of the data
 *
 */
void glxosd_ftgl_texture_atlas_set_region(texture_atlas_t * self, const size_t x, const size_t y,
		const size_t width, const size_t height, const unsigned char *data,
		const size_t stride);

/**
 *  Remove all allocated regions from the atlas.
 *
 *  @param self   a texture atlas structure
 */
void glxosd_ftgl_texture_atlas_clear(texture_atlas_t * self);

/**
 * Creates a new empty font manager.
 *
 * @param   width   width of the underlying atlas
 * @param   height  height of the underlying atlas
 * @param   depth   bit depth of the underlying atlas
 *
 * @return          a new font manager.
 *
 */
font_manager_t * glxosd_ftgl_font_manager_new(size_t width, size_t height, size_t depth);

/**
 *  Deletes a font manager.
 *
 *  @param self a font manager.
 */
void glxosd_ftgl_font_manager_delete(font_manager_t *self);

/**
 *  Deletes a font from the font manager.
 *
 *  Note that font glyphs are not removed from the atlas.
 *
 *  @param self a font manager.
 *  @param font font to be deleted
 *
 */
void glxosd_ftgl_font_manager_delete_font(font_manager_t * self, texture_font_t * font);

/**
 *  Request for a font based on a filename.
 *
 *  @param self     a font manager.
 *  @param filename font filename
 *  @param size     font size
 *
 *  @return Requested font
 */
texture_font_t * glxosd_ftgl_font_manager_get_from_filename(font_manager_t * self, const char * filename,
		const float size);

/**
 *  Request for a font based on a description
 *
 *  @param self     a font manager
 *  @param family   font family
 *  @param size     font size
 *  @param bold     whether font is bold
 *  @param italic   whether font is italic
 *
 *  @return Requested font
 */
texture_font_t * glxosd_ftgl_font_manager_get_from_description(font_manager_t * self, const char * family,
		const float size, const int bold, const int italic);

/**
 *  Request for a font based on a markup
 *
 *  @param self    a font manager
 *  @param markup  Markup describing a font
 *
 *  @return Requested font
 */
texture_font_t * glxosd_ftgl_font_manager_get_from_markup(font_manager_t *self, const markup_t *markup);

/**
 *  Search for a font filename that match description.
 *
 *  @param self    a font manager
 *  @param family   font family
 *  @param size     font size
 *  @param bold     whether font is bold
 *  @param italic   whether font is italic
 *
 *  @return Requested font filename
 */
char * glxosd_ftgl_font_manager_match_description(font_manager_t * self, const char * family,
		const float size, const int bold, const int italic);
/*
 * TEXTURE FONT
 */

/**
 * This function creates a new texture font from given filename and size.  The
 * texture atlas is used to store glyph on demand. Note the depth of the atlas
 * will determine if the font is rendered as alpha channel only (depth = 1) or
 * RGB (depth = 3) that correspond to subpixel rendering (if available on your
 * freetype implementation).
 *
 * @param atlas     A texture atlas
 * @param pt_size   Size of font to be created (in points)
 * @param filename  A font filename
 *
 * @return A new empty font (no glyph inside yet)
 *
 */
texture_font_t * glxosd_ftgl_texture_font_new_from_file(texture_atlas_t * atlas, const float pt_size,
		const char * filename);

/**
 * This function creates a new texture font from a memory location and size.
 * The texture atlas is used to store glyph on demand. Note the depth of the
 * atlas will determine if the font is rendered as alpha channel only
 * (depth = 1) or RGB (depth = 3) that correspond to subpixel rendering (if
 * available on your freetype implementation).
 *
 * @param atlas       A texture atlas
 * @param pt_size     Size of font to be created (in points)
 * @param memory_base Start of the font file in memory
 * @param memory_size Size of the font file memory region, in bytes
 *
 * @return A new empty font (no glyph inside yet)
 *
 */
texture_font_t * glxosd_ftgl_texture_font_new_from_memory(texture_atlas_t *atlas, float pt_size,
		const void *memory_base, size_t memory_size);

/**
 * Delete a texture font. Note that this does not delete the glyph from the
 * texture atlas.
 *
 * @param self a valid texture font
 */
void glxosd_ftgl_texture_font_delete(texture_font_t * self);

/**
 * Request a new glyph from the font. If it has not been created yet, it will
 * be.
 *
 * @param self      A valid texture font
 * @param codepoint Character codepoint to be loaded in UTF-8 encoding.
 *
 * @return A pointer on the new glyph or 0 if the texture atlas is not big
 *         enough
 *
 */
texture_glyph_t * glxosd_ftgl_texture_font_get_glyph(texture_font_t * self, const char * codepoint);

/**
 * Request the loading of several glyphs at once.
 *
 * @param self       A valid texture font
 * @param codepoints Character codepoints to be loaded in UTF-8 encoding. May
 *                   contain duplicates.
 *
 * @return Number of missed glyph if the texture is not big enough to hold
 *         every glyphs.
 */
size_t glxosd_ftgl_texture_font_load_glyphs(texture_font_t * self, const char * codepoints);

/**
 * Get the kerning between two horizontal glyphs.
 *
 * @param self      A valid texture glyph
 * @param codepoint Character codepoint of the peceding character in UTF-8 encoding.
 *
 * @return x kerning value
 */
float glxosd_ftgl_texture_glyph_get_kerning(const texture_glyph_t * self, const char * codepoint);

/**
 * Creates a new empty glyph
 *
 * @return a new empty glyph (not valid)
 */
texture_glyph_t * glxosd_ftgl_texture_glyph_new(void);

/**
 * Line structure
 */
typedef struct line_info_t {
	/**
	 * Index (in the vertex buffer) where this line starts
	 */
	size_t line_start;

	/**
	 * bounds of this line
	 */
	vec4 bounds;

} line_info_t;

/**
 * Align enumeration
 */
typedef enum Align {
	/**
	 * Align text to the left hand side
	 */
	ALIGN_LEFT,

	/**
	 * Align text to the center
	 */
	ALIGN_CENTER,

	/**
	 * Align text to the right hand side
	 */
	ALIGN_RIGHT
} Align;

/**
 * Creates a new empty text buffer using custom shaders.
 *
 * @param depth          Underlying atlas bit depth (1 or 3)
 * @param vert_filename  Path to vertex shader
 * @param frag_filename  Path to fragment shader
 *
 * @return  a new empty text buffer.
 *
 */
text_buffer_t * glxosd_ftgl_text_buffer_new(size_t depth, const char * vert_filename,
		const char * frag_filename);

/**
 * Creates a new empty text buffer using custom shaders.
 *
 * @param depth          Underlying atlas bit depth (1 or 3)
 * @param program        Shader program
 *
 * @return  a new empty text buffer.
 *
 */
text_buffer_t * glxosd_ftgl_text_buffer_new_with_program(size_t depth, GLuint program);

/**
 * Deletes texture buffer and its associated shader and vertex buffer.
 *
 * @param  self  texture buffer to delete
 *
 */
void glxosd_ftgl_text_buffer_delete(text_buffer_t * self);

/**
 * Render a text buffer.
 *
 * @param self a text buffer
 *
 */
void glxosd_ftgl_text_buffer_render(text_buffer_t * self);

/**
 * Print some text to the text buffer
 *
 * @param self a text buffer
 * @param pen  position of text start
 * @param ...  a series of markup_t *, char * ended by NULL
 *
 */
void glxosd_ftgl_text_buffer_printf(text_buffer_t * self, vec2 * pen, ...);

/**
 * Add some text to the text buffer
 *
 * @param self   a text buffer
 * @param pen    position of text start
 * @param markup Markup to be used to add text
 * @param text   Text to be added
 * @param length Length of text to be added
 */
void glxosd_ftgl_text_buffer_add_text(text_buffer_t * self, vec2 * pen, markup_t * markup,
		const char * text, size_t length);

/**
 * Add a char to the text buffer
 *
 * @param self     a text buffer
 * @param pen      position of text start
 * @param markup   markup to be used to add text
 * @param current  charactr to be added
 * @param previous previous character (if any)
 */
void glxosd_ftgl_text_buffer_add_char(text_buffer_t * self, vec2 * pen, markup_t * markup,
		const char * current, const char * previous);

/**
 * Align all the lines of text already added to the buffer
 * This alignment will be relative to the overall bounds of the
 * text which can be queried by text_buffer_get_bounds
 *
 * @param self      a text buffer
 * @param pen       pen used in last call (must be unmodified)
 * @param alignment desired alignment of text
 */
void glxosd_ftgl_text_buffer_align(text_buffer_t * self, vec2 * pen, enum Align alignment);

/**
 * Get the rectangle surrounding the text
 *
 * @param self      a text buffer
 * @param pen       pen used in last call (must be unmodified)
 */
vec4 glxosd_ftgl_text_buffer_get_bounds(text_buffer_t * self, vec2 * pen);

/**
 * Clear text buffer
 *
 * @param self a text buffer
 */
void glxosd_ftgl_text_buffer_clear(text_buffer_t * self);

/**
 * Create an attribute from the given parameters.
 *
 * @param size       number of component
 * @param type       data type
 * @param normalized Whether fixed-point data values should be normalized
 (GL_TRUE) or converted directly as fixed-point values
 (GL_FALSE) when they are  accessed.
 * @param stride     byte offset between consecutive attributes.
 * @param pointer    pointer to the first component of the first attribute
 *                   element in the array.
 * @return           a new initialized vertex attribute.
 *
 * @private
 */
vertex_attribute_t * glxosd_ftgl_vertex_attribute_new(GLchar * name, GLint size, GLenum type,
		GLboolean normalized, GLsizei stride, GLvoid *pointer);

/**
 * Delete a vertex attribute.
 *
 * @param  self a vertex attribute
 *
 */
void glxosd_ftgl_vertex_attribute_delete(vertex_attribute_t * self);

/**
 * Create an attribute from the given description.
 *
 * @param  format Format string specifies the format of a vertex attribute.
 * @return        an initialized vertex attribute
 *
 * @private
 */
vertex_attribute_t * glxosd_ftgl_vertex_attribute_parse(char *format);

/**
 * Enable a vertex attribute.
 *
 * @param attr  a vertex attribute
 *
 * @private
 */
void glxosd_ftgl_vertex_attribute_enable(vertex_attribute_t *attr);

/**
 * Creates an empty vertex buffer.
 *
 * @param  format a string describing vertex format.
 * @return        an empty vertex buffer.
 */
vertex_buffer_t * glxosd_ftgl_vertex_buffer_new(const char *format);

/**
 * Deletes vertex buffer and releases GPU memory.
 *
 * @param  self  a vertex buffer
 */
void glxosd_ftgl_vertex_buffer_delete(vertex_buffer_t * self);

/**
 *  Returns the number of items in the vertex buffer
 *
 *  @param  self  a vertex buffer
 *  @return       number of items
 */
size_t glxosd_ftgl_vertex_buffer_size(const vertex_buffer_t *self);

/**
 *  Returns vertex format
 *
 *  @param  self  a vertex buffer
 *  @return       vertex format
 */
const char * glxosd_ftgl_vertex_buffer_format(const vertex_buffer_t *self);

/**
 * Print information about a vertex buffer
 *
 * @param  self  a vertex buffer
 */
void glxosd_ftgl_vertex_buffer_print(vertex_buffer_t * self);

/**
 * Prepare vertex buffer for render.
 *
 * @param  self  a vertex buffer
 * @param  mode  render mode
 */
void glxosd_ftgl_vertex_buffer_render_setup(vertex_buffer_t *self, GLenum mode);

/**
 * Finish rendering by setting back modified states
 *
 * @param  self  a vertex buffer
 */
void glxosd_ftgl_vertex_buffer_render_finish(vertex_buffer_t *self);

/**
 * Render vertex buffer.
 *
 * @param  self  a vertex buffer
 * @param  mode  render mode
 */
void glxosd_ftgl_vertex_buffer_render(vertex_buffer_t *self, GLenum mode);

/**
 * Render a specified item from the vertex buffer.
 *
 * @param  self   a vertex buffer
 * @param  index index of the item to be rendered
 */
void glxosd_ftgl_vertex_buffer_render_item(vertex_buffer_t *self, size_t index);

/**
 * Upload buffer to GPU memory.
 *
 * @param  self  a vertex buffer
 */
void glxosd_ftgl_vertex_buffer_upload(vertex_buffer_t *self);

/**
 * Clear all items.
 *
 * @param  self  a vertex buffer
 */
void glxosd_ftgl_vertex_buffer_clear(vertex_buffer_t *self);

/**
 * Appends indices at the end of the buffer.
 *
 * @param  self     a vertex buffer
 * @param  indices  indices to be appended
 * @param  icount   number of indices to be appended
 *
 * @private
 */
void glxosd_ftgl_vertex_buffer_push_back_indices(vertex_buffer_t *self, const GLuint * indices,
		const size_t icount);

/**
 * Appends vertices at the end of the buffer.
 *
 * @note Internal use
 *
 * @param  self     a vertex buffer
 * @param  vertices vertices to be appended
 * @param  vcount   number of vertices to be appended
 *
 * @private
 */
void glxosd_ftgl_vertex_buffer_push_back_vertices(vertex_buffer_t *self, const void * vertices,
		const size_t vcount);

/**
 * Insert indices in the buffer.
 *
 * @param  self    a vertex buffer
 * @param  index   location before which to insert indices
 * @param  indices indices to be appended
 * @param  icount  number of indices to be appended
 *
 * @private
 */
void glxosd_ftgl_vertex_buffer_insert_indices(vertex_buffer_t *self, const size_t index,
		const GLuint *indices, const size_t icount);

/**
 * Insert vertices in the buffer.
 *
 * @param  self     a vertex buffer
 * @param  index    location before which to insert vertices
 * @param  vertices vertices to be appended
 * @param  vcount   number of vertices to be appended
 *
 * @private
 */
void glxosd_ftgl_vertex_buffer_insert_vertices(vertex_buffer_t *self, const size_t index,
		const void *vertices, const size_t vcount);

/**
 * Erase indices in the buffer.
 *
 * @param  self   a vertex buffer
 * @param  first  the index of the first index to be erased
 * @param  last   the index of the last index to be erased
 *
 * @private
 */
void glxosd_ftgl_vertex_buffer_erase_indices(vertex_buffer_t *self, const size_t first,
		const size_t last);

/**
 * Erase vertices in the buffer.
 *
 * @param  self   a vertex buffer
 * @param  first  the index of the first vertex to be erased
 * @param  last   the index of the last vertex to be erased
 *
 * @private
 */
void glxosd_ftgl_vertex_buffer_erase_vertices(vertex_buffer_t *self, const size_t first,
		const size_t last);

/**
 * Append a new item to the collection.
 *
 * @param  self   a vertex buffer
 * @param  vcount   number of vertices
 * @param  vertices raw vertices data
 * @param  icount   number of indices
 * @param  indices  raw indices data
 */
size_t glxosd_ftgl_vertex_buffer_push_back(vertex_buffer_t * self, const void * vertices,
		const size_t vcount, const GLuint * indices, const size_t icount);

/**
 * Insert a new item into the vertex buffer.
 *
 * @param  self      a vertex buffer
 * @param  index     location before which to insert item
 * @param  vertices  raw vertices data
 * @param  vcount    number of vertices
 * @param  indices   raw indices data
 * @param  icount    number of indices
 */
size_t glxosd_ftgl_vertex_buffer_insert(vertex_buffer_t * self, const size_t index,
		const void * vertices, const size_t vcount, const GLuint * indices,
		const size_t icount);

/**
 * Erase an item from the vertex buffer.
 *
 * @param  self     a vertex buffer
 * @param  index    index of the item to be deleted
 */
void glxosd_ftgl_vertex_buffer_erase(vertex_buffer_t * self, const size_t index);

typedef struct {
	float x, y, z;
	float s, t;
	float r, g, b, a;
} vertex_t;


]]
return {}
