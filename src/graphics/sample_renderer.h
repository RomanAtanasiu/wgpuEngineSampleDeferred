#pragma once

#include "includes.h"

#include "graphics/renderer.h"
#include "string.h"

#include "graphics/texture.h"

class SampleRenderer : public Renderer {

public:

    struct s_pass {
        Shader* shader = nullptr;
        tPostProcess id = 0;
        std::string name = "";
    };


    s_pass red_in_screen;
    s_pass black_and_white;
    s_pass contrast;
    s_pass blur;

    float time = 0;
    WGPUBindGroup time_bindgroup = nullptr;
    WGPUBuffer time_buffer = nullptr;

    int actual_requested_timestamps = 0;
    int max_timestamps = 500;
    bool activated_request_timestamps = false;

    bool taa_activated = true;

    //WGPUTexture dst_texture_for_post_process = nullptr;
    Texture* dst_texture_for_post_process;

    SampleRenderer(const sRendererConfiguration& config = {});

    int pre_initialize(GLFWwindow* window, bool use_mirror_screen = false) override;
    int initialize() override;
    int post_initialize() override;

    void clean() override;

    void update(float delta_time) override;
    void render() override;
    
    
};
