#version 330 core

out vec4 FragColor;

struct Material{

  sampler2D diffuse[2];
  vec3 ambient;
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

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

vec3 blendNormal( vec3 normal ){

  vec3 blending = abs(normal);
  blending = normalize(max(blending, 0.00001));
  blending = blending / vec3(blending.x + blending.y + blending.z);
  return blending;

}

vec3 triplarMapping( sampler2D texture, vec3 normal, vec3 position){

  vec3 normalBlend = blendNormal(normal);
  vec3 xColor = texture2D(texture, position.yz).rgb;
  vec3 yColor = texture2D(texture, position.xz).rgb;
  vec3 zColor = texture2D(texture, position.xy).rgb;

  return (xColor * normalBlend.x + yColor * normalBlend.y + zColor * normalBlend.z);
}



void main(){

  //ambient 
  vec3 ambient = light.ambient * material.ambient;

  //diffuse
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(light.direction);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * ( diff * triplarMapping(material.diffuse[1], normal, FragPos));

  //specular
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * ( spec * material.specular);

  vec3 result = ambient + diffuse + specular;
  FragColor = vec4(result, 1.0);
}
