#version 410 core

in vec4 undeformed_vpos;
out vec4 output_color;

const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.0001;

vec3 SimpleBrush(vec3 pos, vec3 center, vec3 force)
{
	vec3 diff = pos - center;
	float factor = max(dot(diff, force), 0.0);
	return pos + factor * force;
}

float sphereFunction(vec3 pos)
{
	float radius = 1.0;
    vec3 center = vec3(0, 0, -20);
	return length(pos - center) - radius;
}

float sceneSDF(vec3 pos)
{
    return sphereFunction(pos);
}

float shortestDistanceToSurface(vec3 ipos, vec3 marchingDirection, float start, float end) {
    float depth = start;
    for (int i = 0; i < MAX_MARCHING_STEPS; i++) {
        float dist = sceneSDF(ipos + depth * marchingDirection);
        if (dist < EPSILON) {
			return depth;
        }
        depth += dist;
        if (depth >= end) {
            return end;
        }
    }
    return end;
}

vec3 estimateNormal(vec3 p) {
    return normalize(vec3(
        sceneSDF(vec3(p.x + EPSILON, p.y, p.z)) - sceneSDF(vec3(p.x - EPSILON, p.y, p.z)),
        sceneSDF(vec3(p.x, p.y + EPSILON, p.z)) - sceneSDF(vec3(p.x, p.y - EPSILON, p.z)),
        sceneSDF(vec3(p.x, p.y, p.z  + EPSILON)) - sceneSDF(vec3(p.x, p.y, p.z - EPSILON))
    ));
}

vec3 phongContribForLight(vec3 k_d, vec3 k_s, float alpha, vec3 p, vec3 view_dir,
                          vec3 lightPos, vec3 lightIntensity) {
    vec3 N = estimateNormal(p);
    vec3 L = normalize(lightPos - p);
    vec3 V = -view_dir;
    vec3 R = normalize(reflect(-L, N));
    
    float dotLN = dot(L, N);
    float dotRV = dot(R, V);
    
    if (dotLN < 0.0) {
        // Light not visible from this point on the surface
        return vec3(0.0, 0.0, 0.0);
    } 
    
    if (dotRV < 0.0) {
        // Light reflection in opposite direction as viewer, apply only diffuse
        // component
        return lightIntensity * (k_d * dotLN);
    }
    return lightIntensity * (k_d * dotLN + k_s * pow(dotRV, alpha));
}

vec3 phongIllumination(vec3 k_a, vec3 k_d, vec3 k_s, float alpha, vec3 p, vec3 view_dir) {
    const vec3 ambientLight = 0.5 * vec3(1.0, 1.0, 1.0);
    vec3 color = ambientLight * k_a;
    
    vec3 light1Pos = vec3(0, 0, 0);
    vec3 light1Intensity = vec3(0.8, 0.8, 0.8);
    
    color += phongContribForLight(k_d, k_s, alpha, p, view_dir,
                                  light1Pos,
                                  light1Intensity);
    return color;
}

void main() 
{
    vec3 view_dir = normalize(undeformed_vpos.xyz);
    float dist = shortestDistanceToSurface(undeformed_vpos.xyz, view_dir, MIN_DIST, MAX_DIST);
    
    vec3 K_a = vec3(0, 0, 0);
    vec3 K_d = vec3(0.7, 0.2, 0.2);
    vec3 K_s = vec3(0, 0, 0);
    float shininess = 10.0;

    vec3 p = undeformed_vpos.xyz + dist * view_dir;
    
    vec3 color = phongIllumination(K_a, K_d, K_s, shininess, p, view_dir);
    //color = vec3(1, 1, 1);
    
    vec4 ov = undeformed_vpos;
    ov.z *= (-1);

    output_color = vec4(color, 1.0);
}