#ifndef _ML_SHADER_API_HPP_
#define _ML_SHADER_API_HPP_

// Vertex Uniforms
/* * * * * * * * * * * * * * * * * * * * */
#define ML_VERT_PROJ		"Vert.proj"
#define ML_VERT_VIEW		"Vert.view"
#define ML_VERT_MODEL		"Vert.model"

// Fragment Uniforms
/* * * * * * * * * * * * * * * * * * * * */
#define ML_FRAG_MAIN_COL	"Frag.mainCol"
#define ML_FRAG_MAIN_TEX	"Frag.mainTex"


/* * * * * * * * * * * * * * * * * * * * */

#define ML_VERT_EXAMPLE \
"#shader vertex\n" \
"#version 410 core\n" \
"\n" \
"layout(location = 0) in vec3 a_Position;\n" \
"layout(location = 1) in vec4 a_Normal;\n" \
"layout(location = 2) in vec2 a_Texcoord;\n" \
"\n" \
"out VertexData\n" \
"{\n" \
"	vec3 Position;\n" \
"	vec4 Normal;\n" \
"	vec2 Texcoord;\n" \
"} Out;\n" \
"\n" \
"struct Vert_Uniforms\n" \
"{\n" \
"	mat4 proj;\n" \
"	mat4 view;\n" \
"	mat4 model;\n" \
"};\n" \
"\n" \
"uniform Vert_Uniforms Vert;\n" \
"\n" \
"void main()\n" \
"{\n" \
"	Out.Position = a_Position;\n" \
"	Out.Normal = a_Normal;\n" \
"	Out.Texcoord = a_Texcoord;\n" \
"	\n" \
"	mat4 MVP = (Vert.proj * Vert.view * Vert.model);\n" \
"	\n" \
"	gl_Position = MVP * vec4(Out.Position, 1.0);\n" \
"}\n" \
""

#define ML_FRAG_EXAMPLE \
"#shader fragment\n" \
"#version 410 core\n" \
"\n" \
"in VertexData\n" \
"{\n" \
"	vec3 Position;\n" \
"	vec4 Normal;\n" \
"	vec2 Texcoord;\n" \
"} In;\n" \
"\n" \
"out vec4 gl_Color;\n" \
"\n" \
"struct Frag_Uniforms\n" \
"{\n" \
"	vec4 mainCol;\n" \
"	sampler2D mainTex;\n" \
"};\n" \
"\n" \
"uniform Frag_Uniforms Frag;\n" \
"\n" \
"void main()\n" \
"{\n" \
"	gl_Color = Frag.mainCol * texture(Frag.mainTex, In.Texcoord);\n" \
"}\n" \
""

#define ML_GEOM_EXAMPLE ""

#endif // !_ML_SHADER_API_HPP_