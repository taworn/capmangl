#ifndef SHADERS_NORMAL_SHADER_HXX
#define SHADERS_NORMAL_SHADER_HXX

class NormalShader : public Shader
{
public:
	NormalShader() : position(-1), color(-1), mvpMatrix(-1)
	{
		const char vertexSourceCode[] = ""
			"uniform mat4 u_MVPMatrix;  \n"  // A constant representing the combined model/view/projection matrix.
			"attribute vec4 a_Position; \n"  // Per-vertex position information we will pass in.
			"attribute vec4 a_Color;    \n"  // Per-vertex color information we will pass in.
			"varying vec4 v_Color;      \n"  // This will be passed into the fragment shader.
			"void main() {              \n"  //
			"  v_Color = a_Color;       \n"  // Pass the color through to the fragment shader.  It will be interpolated across the triangle.
			"  gl_Position = u_MVPMatrix\n"  // gl_Position is a special variable used to store the final position.
			"              * a_Position;\n"  // Multiply the vertex by the matrix to get the final point in normalized screen coordinates.
			"}                          \n";
		const char fragmentSourceCode[] = ""
			"precision mediump float; \n"  // Set the default precision to medium.  We don't need as high of a precision in the fragment shader.
			"varying vec4 v_Color;    \n"  // This is the color from the vertex shader interpolated across the triangle per fragment.
			"void main() {            \n"  //
			"  gl_FragColor = v_Color;\n"  // Pass the color directly through the pipeline.
			"}                        \n";
		if (init(vertexSourceCode, fragmentSourceCode)) {
			position = getAttrib("a_Position");
			color = getAttrib("a_Color");
			mvpMatrix = getUniform("u_MVPMatrix");
		}
	}

	GLint getPosition() const { return position; }
	GLint getColor() const { return color; }
	GLint getMVPMatrix() const { return mvpMatrix; }

private:
	GLint position;
	GLint color;
	GLint mvpMatrix;
};

#endif // SHADERS_NORMAL_SHADER_HXX

