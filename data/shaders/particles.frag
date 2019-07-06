#version 330

//out color
out vec4 fragColor;

uniform sampler2D u_diffuse_map;
uniform vec3 u_color;
//varying lowp vec4 fragmentColor;

void main(){
	vec4 tex = vec4(texture(u_diffuse_map, gl_PointCoord));
    fragColor = vec4(u_color,1.0) * tex;
}