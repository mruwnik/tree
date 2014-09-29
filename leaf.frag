uniform sampler2D myTexture1;
uniform sampler2D myTexture2;

void main (void)
{
	vec4 texval1 = texture2D(myTexture1, vec2(gl_TexCoord[0]));
	vec4 texval2 = texture2D(myTexture2, vec2(gl_TexCoord[1]));
	         
	if(texval1[3] < 0.001){
		discard;
	}
	gl_FragColor = 0.5*(texval1 + texval2);
}
