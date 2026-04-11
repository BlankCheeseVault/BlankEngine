#include <app.hpp>

#include <iostream>
#include <vector>

#include <stb/stb_image.h>

#include "shader.hlsl.hpp"

struct Vertex
{
    float x;
    float y;
    float z;

    float u;
    float v;
};

sg_buffer vertexBuffer = {};
sg_buffer indexBuffer = {};
sg_bindings bindings = {};

sg_view texture = {};
sg_sampler sampler = {};

sg_pipeline pipeline = {};

void init()
{
    App::screen_pass.action.colors[0].clear_value = {1, .5, .5, 1};

    std::vector<Vertex> vertices = {
        {-0.75, +.5, 0, 0, 0},
        {+0.75, +.5, 0, 1, 0},
        {+0.75, -.5, 0, 1, 1},
        {-0.75, -.5, 0, 0, 1}};

    std::vector<uint16_t> indices = {0, 1, 2, 0, 2, 3};

    { // vertex buffer
        sg_buffer_desc desc = {};
        desc.data.ptr = vertices.data();
        desc.data.size = vertices.size() * sizeof(Vertex);
        vertexBuffer = sg_make_buffer(desc);
    }
    { // index buffer
        sg_buffer_desc desc = {};
        desc.data.ptr = indices.data();
        desc.data.size = indices.size() * sizeof(uint16_t);
        desc.usage.index_buffer = true;
        indexBuffer = sg_make_buffer(desc);
    }

    {
        int w, h, c;
        unsigned char *data = stbi_load("res/Haha.png", &w, &h, &c, 4);

        if (!data)
        {
            std::cerr << "Failed to load image: " << " (" << stbi_failure_reason() << ")\n";
        }

        sg_image_desc img_desc = {};
        img_desc.width = w;
        img_desc.height = h;
        img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
        img_desc.data.mip_levels[0].ptr = data;
        img_desc.data.mip_levels[0].size = w * h * 4;

        sg_view_desc desc = {};
        desc.texture.image = sg_make_image(img_desc);
        texture = sg_make_view(desc);
    }

    {
        sg_sampler_desc desc = {};

        sampler = sg_make_sampler(desc);
    }

    bindings.vertex_buffers[0] = vertexBuffer;
    bindings.index_buffer = indexBuffer;
    bindings.views[0] = texture;
    bindings.samplers[0] = sampler;

    { // pipeline
        sg_shader shader = sg_make_shader(triangle_shader_desc(sg_query_backend()));

        sg_pipeline_desc desc = {};
        desc.shader = shader;
        desc.index_type = SG_INDEXTYPE_UINT16;
        desc.layout.buffers[0].stride = sizeof(Vertex);
        desc.layout.attrs[ATTR_triangle_v_position].format = SG_VERTEXFORMAT_FLOAT3;
        desc.layout.attrs[ATTR_triangle_v_uv].format = SG_VERTEXFORMAT_FLOAT2;

        pipeline = sg_make_pipeline(desc);
    }
}

void update()
{
}

void frame()
{
    App::debugPrint("Hello World", 0, 0);

    sg_apply_pipeline(pipeline);
    sg_apply_bindings(bindings);

    sg_draw(0, 6, 1);
}

void event(const sapp_event *event)
{
} 

void cleanup()
{
}

sapp_desc sokol_main(int argc, char *argv[])
{
    App::init_cb = init;
    App::update_cb = update;
    App::frame_cb = frame;
    App::event_cb = event;
    App::cleanup_cb = cleanup;

    App::debugScaleX = 1.5;
    App::debugScaleY = 1.5;
    return App::create("BlankEngine", 1920, 1080);
}
