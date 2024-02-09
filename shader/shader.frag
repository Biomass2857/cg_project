#version 330 core
in vec3 color;
in vec3 normal;

out vec4 FragColor;

void main() {
    vec3 diffuse_light_color = vec3(0.248,0.264,0.610);
    vec3 specular_light_color = vec3(0.235,0.205,0.925);
    vec3 light_direction = normalize(vec3(0.830,0.512,0.266));
    vec3 light_reflection = normalize(-reflect(light_direction, normal));

    vec3 camera_vec = vec3(0.0, 0.0, 1.0);
    
    vec3 ambient_light = color * vec3(0.968,0.990,1.000);
    vec3 diffuse_light = diffuse_light_color * dot(light_direction, normal);
    vec3 specular_light = specular_light_color * pow(max(dot(light_reflection, camera_vec), 0.0), 32.0);
    
    vec3 result = vec3(0); 
	result += ambient_light;
    result += diffuse_light;
    result += specular_light;
    result = vec3(min(result.r, 1.0), min(result.g, 1.0), min(result.b, 1.0));
    
    FragColor = vec4(result, 1.0);
}