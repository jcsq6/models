#version 430

layout (location = 0) in vec3 position;

out vec4 varyingColor;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;

void main(void){
    mat4 mv_matrix = v_matrix * m_matrix;

    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
    varyingColor = vec4(clamp(position, 0.0, 1.0), 1.0);
}