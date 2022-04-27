#version 330 core

out vec4 FragColor;

struct Material{

  sampler2D diffuse[2];
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

float ShadowCalculation( vec4 fragPosLightSpace ){

  //divide by perspective
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;
  
  //this has the depth of the topmost fragment
  float closestDepth = texture(depthMap, projCoords.xy).r;
  //this has the depth of current fragment
  float currentDepth = projCoords.z;

  float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

  return shadow;
}


void main(){

  //diffuse
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(light.direction - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 tex1 = triplarMapping(material.diffuse[0], normal, FragPos, 0);
  vec3 tex2 = triplarMapping(material.diffuse[1], normal, FragPos, 0);
  vec3 diffTex;

//texturing diffuse based on slope
  if(norm.y > 0.73){

    diffTex = tex2;
  }else if( norm.y > 0.721){

    diffTex = mix(tex2, tex1, 0.5);
  }
  else{
    diffTex = tex1;
  }


  vec3 diffuse = light.diffuse * diff;
  vec3 ambient = light.ambient;

  //specular
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * ( spec * material.specular);

  //shadows
  float shadow = ShadowCalculation(fs_in.FragPosLightSpace);

  vec3 result = (ambient + (1 - shadow) * (diffuse + specular)) * diffTex;

  FragColor = vec4(result, 1.0);
}
