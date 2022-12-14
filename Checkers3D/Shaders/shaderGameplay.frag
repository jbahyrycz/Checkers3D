#version 450

in vec4 vCol;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

out vec4 color;

struct PointLight
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform PointLight pointLight;

uniform sampler2D theTexture;
uniform Material material;

uniform vec3 eyePosition;

vec4 CalcLightByDirection(PointLight light, vec3 direction)
{
	vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

	float diffuseFactor = max(dot(normalize(normal), normalize(direction)), 0.0f);
	vec4 diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f);

	vec4 specularColor = vec4(0, 0, 0, 0);

	if (diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - fragPos);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(normal)));

		float specularFactor = dot(fragToEye, reflectedVertex);
		if (specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	return (ambientColor + diffuseColor + specularColor); 
}

vec4 CalcPointLight()
{
	vec3 direction = fragPos - pointLight.position;
	float distance = length(direction);
	direction = normalize(direction);

	vec4 color = CalcLightByDirection(pointLight, direction);
	float attenuation = pointLight.exponent * distance * distance +
				pointLight.linear * distance +
				pointLight.constant;

	return (color/attenuation);
	//return color;
}

void main()
{
	vec4 finalColor = CalcPointLight();
	//vec4 finalColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	color = texture(theTexture, texCoord) * finalColor;
}