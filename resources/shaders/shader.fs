#version 330 core

out vec4 FragColor;

struct Material{

  sampler2D diffuse[2];
  vec3 ambient[2];
  // vec3 diffuse[2];
  vec3 specular;
  float shininess;
};

struct Light{

  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

in vec3 FragPos;
in vec3 normal;

in VS_OUT {

  vec3 FragPos;
  vec3 Normal;
  vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D depthMap;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

vec3 blendNormal( vec3 normal ){

  vec3 blending = abs(normal);
  blending = normalize(max(blending, 0.00001));
  blending = blending / vec3(blending.x + blending.y + blending.z);
  return blending;

}

vec3 triplarMapping( sampler2D texture, vec3 normal, vec3 position, int shadow){

  vec3 normalBlend = blendNormal(normal);

  if( shadow == 1 ){

    float xColor = texture2D(texture, position.yz).r;
    float yColor = texture2D(texture, position.xz).r;
    float zColor = texture2D(texture, position.xy).r;

    return (vec3(xColor * normalBlend.x + yColor * normalBlend.y + zColor * normalBlend.z));

  }else{

    vec3 xColor = texture2D(texture, position.yz).rgb;
    vec3 yColor = texture2D(texture, position.xz).rgb;
    vec3 zColor = texture2D(texture, position.xy).rgb;

    return (xColor * normalBlend.x + yColor * normalBlend.y + zColor * normalBlend.z);
  }

}

float ShadowCalculation( vec4 fragPosLightSpace, vec3 lightDir, vec3 norm){

  //divide by perspective
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;
  
  //this has the depth of the topmost fragment
  float closestDepth = texture(depthMap, projCoords.xy).r;
  //this has the depth of current fragment
  float currentDepth = projCoords.z;

  //for smoother shadows
  float shadow = 0.0f;
  float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);

  //we will use percentage-closer-filtering for blur edges
  vec2 texelSize = 1.0 / textureSize(depthMap, 0);
  for( int i = -1; i <= 1; ++i ){

    for( int j = -1; j <= 1; ++j){

      float pcf = texture(depthMap, projCoords.xy + vec2(i, j) * texelSize).r;
      shadow += currentDepth - bias > pcf ? 1.0 : 0.0;
    }
  }

  shadow = shadow / 9.0;

  return shadow;
}


void main(){

  //diffuse
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(light.direction - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 tex1 = triplarMapping(material.diffuse[0], normal, FragPos, 0);
  vec3 tex2 = triplarMapping(material.diffuse[1], normal, FragPos, 0);
  // vec3 tex1 = material.diffuse[0];
  // vec3 tex2 = material.diffuse[1];
  vec3 diffTex;
  vec3 amb;

//texturing diffuse based on slope
  if(norm.y > 0.73){

    diffTex = tex2;
    amb = material.ambient[1];
  }else if( norm.y > 0.721){

    diffTex = mix(tex2, tex1, smoothstep(0.72f, 0.73f, norm.y));
    amb = mix(material.ambient[1], material.ambient[0], 0.5);
  }
  else{
    diffTex = tex1;
    amb = material.ambient[0];
  }


  vec3 diffuse = light.diffuse * (diff * diffTex);
  vec3 ambient = light.ambient * amb;

  //specular
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * ( spec * material.specular);

  //shadows
  float shadow = ShadowCalculation(fs_in.FragPosLightSpace, lightDir, norm);

  vec3 result = (ambient + (1 - shadow) * (diffuse + specular));

  FragColor = vec4(result, 1.0);
}
