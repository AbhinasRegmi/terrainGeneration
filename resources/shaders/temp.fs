// #version 330 core

// out vec4 FragColor;

// in vec3 FragPos;
// in vec3 normal;

// struct Material{
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
//     float shininess;
// };

// struct DirLight{
//     vec3 direction;

//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };

// struct SpotLight{
//     vec3 position;
//     vec3 direction;
//     float cutOff;
//     float outerCutOff;

//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };

// #define constant 1.0f
// #define linear 0.09f
// #define quadratic 0.0034f

// uniform vec3 viewPos;
// uniform DirLight dirLight;
// uniform SpotLight spotLight;
// uniform Material material;

// vec3 dirLightCalculation(DirLight light, vec3 normal, vec3 viewDir);
// vec3 spotLightCalculation(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir);

// void main(){

//     vec3 norm = normalize(normal);
//     vec3 viewDir = normalize( viewPos - FragPos);

//     vec3 result = dirLightCalculation(dirLight, norm, viewDir);
//     result += spotLightCalculation(spotLight, norm, FragPos, viewDir);

//     FragColor = vec4(result, 1.0f);
// }

// vec3 dirLightCalculation(DirLight light, vec3 normal, vec3 viewDir){

//     vec3 lightDir = normalize(-light.direction);

//     //diffuse
//     float diff = max(dot(normal, lightDir), 0.0);

//     //specular
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

//     //combine
//     vec3 ambient = light.ambient * material.ambient * 2.0;
//     vec3 diffuse = light.diffuse * ( diff * material.diffuse) * 4.0;
//     vec3 specular = light.specular * (spec * material.specular);

//     return (ambient + diffuse + specular);
// }

// vec3 spotLightCalculation(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir){

//     vec3 lightDir = normalize(light.position - FragPos);

//     //diffuse
//     float diff = max(dot(normal, lightDir), 0.0);

//     //specular
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

//     //attenuation
//     float distance = length(light.position - FragPos);
//     float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);

//     //spotlight intensity
//     float theta = dot(lightDir, normalize(-light.direction));
//     float epsilon = light.cutOff - light.outerCutOff;
//     float intensity = clamp( (theta - light.outerCutOff) / epsilon, 0.0, 1.0);

//     //combine
//     vec3 ambient = light.ambient * material.ambient;
//     vec3 diffuse = light.diffuse * (diff * material.diffuse);
//     vec3 specular = light.specular * (spec * material.specular);

//     ambient *= attenuation * intensity;
//     diffuse *= attenuation * intensity;
//     specular *= attenuation * intensity;

//     return (ambient + diffuse + specular);
// }



