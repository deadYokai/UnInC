#version 330 core
out vec4 frag_color;

uniform vec3 rectColor;
void main(){
    frag_color = vec4(rectColor, 1.0);
}