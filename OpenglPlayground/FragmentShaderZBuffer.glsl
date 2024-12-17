#version 400 core

out vec4 FragColor;


float near = 0.1; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // transform depth value from [0,1] back to [-1,1] 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{

    float depth = LinearizeDepth(gl_FragCoord.z) / far;
	FragColor = vec4(vec3(depth), 1.0);

}