@vs vs
in vec3 v_position;
in vec2 v_uv;

out vec2 f_uv;

void main() {
    gl_Position = vec4(v_position, 1);
    f_uv = v_uv;
}
@end

@fs fs
in vec2 f_uv;

out vec4 FragColor;

layout(binding=0) uniform texture2D u_texture;
layout(binding=0) uniform sampler u_sampler;

void main() {
    vec4 tex = texture(sampler2D(u_texture, u_sampler), f_uv);
    FragColor = tex;
}
@end

@program triangle vs fs

// ..\tools\sokol-shdc.exe -i ..\projects\demo\shader.hlsl -o ../projects/demo/shader.hlsl.hpp -l glsl430