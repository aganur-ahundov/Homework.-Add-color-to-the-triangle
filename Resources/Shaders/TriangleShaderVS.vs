
attribute vec3 a_posL;
attribute vec2 a_uv;
uniform float u_WVP;


varying vec2 v_uv;
uniform sampler2D u_s_texture;


void main()
{

	gl_Position = vec4( a_posL, 1.0 );
	v_uv = a_uv;
}
   