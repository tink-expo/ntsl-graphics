#version 410 core

uniform float rad_pixel;
uniform vec3 i_force;

in vec4 undeformed_vpos;
in vec4 deformed_vpos;

out vec4 output_color;

const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.0001;
const float PI = 3.14159;
const float NATU_E = 2.71828;

vec3 center = vec3(0, 0, -20);

float normalDistribution(float sigma, float sq_x)
{
	return pow(NATU_E, -sq_x / (2 * sigma * sigma)) / (sigma * sqrt(2 * PI));
}

vec3 simpleBrush(vec3 pos, vec3 force)
{
	// vec3 diff = pos - center;
	// float factor = max(dot(diff, force), 0.0);
	// return pos + factor * force;
	vec3 diff = pos - center;
    float d_dot_f = dot(diff, force);
    if (d_dot_f <= 0) {
        return pos;
    }
    float sq_sin = pow(length(diff), 2) - 
            pow(d_dot_f, 2) / pow(length(force), 2);
    return pos + force * 3 * normalDistribution(0.4, sq_sin);
}

mat3 estimateJacobian(vec3 pos, vec3 force)
{
    vec3 diff_x = vec3(EPSILON, 0, 0);
    vec3 diff_y = vec3(0, EPSILON, 0);
    vec3 diff_z = vec3(0, 0, EPSILON);

    vec3 grad_x = (simpleBrush(pos + diff_x, force) - simpleBrush(pos - diff_x, force)) / 2 * EPSILON;
    vec3 grad_y = (simpleBrush(pos + diff_y, force) - simpleBrush(pos - diff_y, force)) / 2 * EPSILON;
    vec3 grad_z = (simpleBrush(pos + diff_z, force) - simpleBrush(pos - diff_z, force)) / 2 * EPSILON;

    return mat3(grad_x, grad_y, grad_z);
}

float sphereFunction(vec3 pos)
{
	float radius = 4.0;
	return length(pos - center) - radius;
}

float sceneSDF(vec3 pos)
{
    return sphereFunction(pos);
}

// vec3 ode23(vec3 ipos, float duration)
// {
//     // TODO: Check this
//     float h = duration / 5;
//     float t = h;
//     vec3 k1;
//     vec3 k2;
//     vec3 k3;
//     vec3 k4;
//     vec3 y = ipos;
//     while (t < duration) {

//         t += h;
//     }
// }

vec4 ntsl(vec3 ipos, vec3 w, float start, float end, vec3 force)
{
    float s_sum = start;
    vec3 pos = ipos;
    for (int i = 0; i < MAX_MARCHING_STEPS; ++i) {
        mat3 inv_jacobian = inverse(estimateJacobian(pos, force));
        float s_undeformed = sceneSDF(pos);
        s_sum += s_undeformed;
        float u_epsilon = s_sum * (rad_pixel * determinant(inv_jacobian));

        // Termination.
        if (s_undeformed < u_epsilon) {
            return vec4(pos, 1);
        }
        if (s_sum >= end) {
            return vec4(0);
        }

        if (s_undeformed < 3 * u_epsilon) {
            vec3 w_undeformed = normalize(inv_jacobian * w);
            pos = pos + s_undeformed * w_undeformed;
        } else {
            // pos = ode23(pos, s_undeformed);
            vec3 w_undeformed = normalize(inv_jacobian * w);
            pos = pos + s_undeformed * w_undeformed / 10;
        }
    }

    return vec4(0);
}

vec3 estimateNormal(vec3 p) {
    return normalize(vec3(
        sceneSDF(vec3(p.x + EPSILON, p.y, p.z)) - sceneSDF(vec3(p.x - EPSILON, p.y, p.z)),
        sceneSDF(vec3(p.x, p.y + EPSILON, p.z)) - sceneSDF(vec3(p.x, p.y - EPSILON, p.z)),
        sceneSDF(vec3(p.x, p.y, p.z  + EPSILON)) - sceneSDF(vec3(p.x, p.y, p.z - EPSILON))
    ));
}

vec3 sphereNormal(vec3 p) {
    return normalize(p - center);
}

vec3 phongContribForLight(vec3 k_d, vec3 k_s, float alpha, vec3 p, vec3 view_dir,
                          vec3 lightPos, vec3 lightIntensity, vec3 N) {
    vec3 L = normalize(lightPos - p);
    vec3 V = -view_dir;
    vec3 R = normalize(reflect(-L, N));
    
    float dotLN = dot(L, N);
    float dotRV = dot(R, V);
    
    if (dotLN < 0.0) {
        return vec3(0.0, 0.0, 0.0);
    } 
    
    if (dotRV < 0.0) {
        return lightIntensity * (k_d * dotLN);
    }
    return lightIntensity * (k_d * dotLN + k_s * pow(dotRV, alpha));
}

vec3 phongIllumination(vec3 k_a, vec3 k_d, vec3 k_s, float alpha, vec3 p, vec3 view_dir, vec3 normal) {
    const vec3 ambientLight = 0.5 * vec3(1.0, 1.0, 1.0);
    vec3 color = ambientLight * k_a;
    
    vec3 light1Pos = vec3(8, 20, 8);
    vec3 light1Intensity = vec3(0.8);
    
    color += phongContribForLight(k_d, k_s, alpha, p, view_dir,
                                  light1Pos,
                                  light1Intensity, normal);
    return color;
}

void main() 
{
    vec3 upos = undeformed_vpos.xyz;
    vec3 dpos = deformed_vpos.xyz;

    vec3 view_undeformed = normalize(upos);
    vec3 view_deformed = normalize(dpos);

    vec4 ntsl_res = ntsl(upos, view_deformed, MIN_DIST, MAX_DIST, i_force);
    if (ntsl_res.w == 0) {
        output_color = vec4(0, 0, 0, 1);
        output_color = vec4(1);

    } else {
        vec3 u_res = ntsl_res.xyz;
        vec3 d_res = simpleBrush(u_res, i_force);
        u_res = upos;
        d_res = dpos;

        mat3 inv_jacobian = inverse(estimateJacobian(u_res, i_force));
        vec3 normal = normalize(transpose(inv_jacobian) * sphereNormal(u_res));
        // vec3 normal = sphereNormal(d_res);
        
        vec3 K_a = vec3(0.2, 0.2, 0.2);
        vec3 K_d = vec3(0.2, 0.2, 0.7);
        vec3 K_s = vec3(1);
        float shininess = 10.0;
        vec3 color = phongIllumination(K_a, K_d, K_s, shininess, d_res, view_deformed, normal);

        output_color = vec4(color, 1.0);
    }
    // output_color = vec4(1);
}