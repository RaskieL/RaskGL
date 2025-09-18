#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 uVP; // your normal view-projection matrix

void main() {
    gl_Position = uVP * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
layout(location = 0) out vec4 color;

void main() {
    color = vec4(1.0, 0.0, 0.0, 1.0); // solid red
}
