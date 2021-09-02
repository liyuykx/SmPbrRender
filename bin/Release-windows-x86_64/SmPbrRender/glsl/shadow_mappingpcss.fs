#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float lightSourceSize;
uniform int blockerKernelSize;
uniform bool usePossion ;
vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);
float getRandom(int i) {
	float dot_product = dot(vec4(gl_FragCoord.xyy, i), vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
}
float CalcShadowPCSS(vec4 fragPosLightSpace){
	    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
   // float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float bias = 0.005;
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

	float blockDepth = 0.0;
	float depthCount = 0.0;
	for(int x = -blockerKernelSize; x <= blockerKernelSize; ++x)
	{
		for(int y = -blockerKernelSize; y <= blockerKernelSize; ++y)
		{
			float mapDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			if(currentDepth - bias > mapDepth) {
				blockDepth += mapDepth;
				depthCount++;
			}     
		}    
	}
	float av_blockDepth = blockDepth/depthCount;
	int W_p = av_blockDepth != 0 ? int((currentDepth  - av_blockDepth)*lightSourceSize/av_blockDepth):0;

    float shadowFactor = 1.0;
    if( usePossion ) {
        for (int i=0 ;i<4;i++){
	   // if (texture( depthmap, projCoords.xy + poissonDisk[i]/700.0 ).r  <  currentDepth - bias ){
         //    shadowFactor-=0.2;
		//}
	    int index = int((2*W_p)*getRandom(i))%((2*W_p));
	    if(texture( shadowMap, projCoords.xy + poissonDisk[index]* texelSize ).r  <  currentDepth - bias ){
        shadowFactor -= 0.25;
      }
    }
	return shadowFactor;
    }
	
	for(int x = -W_p; x <= W_p; ++x)
	{
		for(int y = -W_p; y <= W_p; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias < pcfDepth ? 1.0 : 0.0;        
		}    
	}
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 1.0;

	return shadow /= (2*W_p + 1)*(2*W_p + 1);
}
 
void main()
{           
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    // ambient
    vec3 ambient = 0.3 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
    float shadow = CalcShadowPCSS(fs_in.FragPosLightSpace);    
    //float shadow =CalcShadowPoission(fs_in.FragPosLightSpace);                      
    vec3 lighting = (ambient + shadow* (diffuse + specular)) * color;    
    FragColor = vec4(lighting, 1.0);
}