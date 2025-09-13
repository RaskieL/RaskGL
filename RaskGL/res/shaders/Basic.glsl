#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in mat4 instanceMatrix;

out vec2 v_TexCoord;
out vec3 v_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
	gl_Position = u_Projection * u_View * instanceMatrix * vec4(position, 1.0);
	
    v_TexCoord = texCoord;
	v_Normal = mat3(instanceMatrix) * normal;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;

uniform vec3 lightDir;
uniform vec4 u_Color;
uniform sampler2D u_Texture;

float ambient = 0.1;

void main() {
	float diff = max(dot(normalize(v_Normal), normalize(lightDir)), 0.0);
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = vec4(texColor.rgb * (ambient + diff), texColor.a);
};