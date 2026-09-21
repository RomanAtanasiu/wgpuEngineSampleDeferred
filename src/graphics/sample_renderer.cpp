#include "sample_renderer.h"

#include "graphics/shader.h"
#include "graphics/renderer_storage.h"
#include "graphics/renderer.h"
#include "graphics/webgpu_context.h"

#include "shaders/blur_compute.wgsl.gen.h"
#include "shaders/black_and_white.wgsl.gen.h"
#include "shaders/contrast_compute.wgsl.gen.h"
#include "graphics/texture.h"

SampleRenderer::SampleRenderer(const sRendererConfiguration& config) : Renderer(config)
{

}

int SampleRenderer::pre_initialize(GLFWwindow* window, bool use_mirror_screen)
{
    return Renderer::pre_initialize(window, use_mirror_screen);
}

int SampleRenderer::initialize()
{
    int error_code = Renderer::initialize();

    clear_color = glm::vec4(0.22f, 0.22f, 0.22f, 1.0);

    return error_code;
}

int SampleRenderer::post_initialize()
{
    /*
    
    {

        red_in_screen.shader = RendererStorage::get_shader("data/test_shaders/screen_going_red.wgsl");


        Uniform u_time;
        time_buffer = webgpu_context->create_buffer(sizeof(time), WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst, &time, "Time");
        u_time.data = time_buffer;
        u_time.binding = 0;
        u_time.buffer_size = sizeof(time);
        std::vector<Uniform*> unifroms;
        unifroms.push_back(&u_time);
        time_bindgroup = webgpu_context->create_bind_group(unifroms, red_in_screen.shader, 1);

        red_in_screen.id = Renderer::post_process_add_render_pass(red_in_screen.shader, BEFORE_TRANSPARENTS_AND_TAA,{ time_bindgroup });
        red_in_screen.name = "low hp";
    }

    {
        black_and_white.shader = RendererStorage::get_shader("data/test_shaders/black_and_white.wgsl");
        black_and_white.id = post_process_add_render_pass(black_and_white.shader, BEFORE_TRANSPARENTS_AND_TAA);
        black_and_white.name = "black and white";
    }

    {
        contrast.shader = RendererStorage::get_shader("data/test_shaders/contrast_compute.wgsl");
        int workgroups[3] = { webgpu_context->gbuffer_format.width / 8, webgpu_context->gbuffer_format.height / 8 , 1};
        int workgroups2[3] = { webgpu_context->gbuffer_format.width / 16, webgpu_context->gbuffer_format.height / 16 , 1 };
        contrast.id = post_process_add_compute_pass(contrast.shader, BEFORE_TRANSPARENTS_AND_TAA, "contrast_compute", workgroups);
        contrast.name = "contrast";

    }
    {
        blur.shader = RendererStorage::get_shader("data/test_shaders/blur_compute.wgsl");
        int workgroups[3] = { webgpu_context->gbuffer_format.width / 8, webgpu_context->gbuffer_format.height / 8 , 1 };
        blur.id = post_process_add_compute_pass(blur.shader, AFTER_TRANSPARENTS,"box_blur", workgroups);
        blur.name = "blur_compute";


    }*/
    /*
    dst_texture_for_post_process = Renderer::webgpu_context->create_texture(
        WGPUTextureDimension_2D,
        webgpu_context->light_buffer_format,
        { webgpu_context->screen_width, webgpu_context->screen_height, 1u },
        WGPUTextureUsage_RenderAttachment | WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst,
        1u, 1u, "buffer to copy"
    );*/


    return Renderer::post_initialize();
}

void SampleRenderer::clean()
{
    /*
    delete contrast.shader;
    delete black_and_white.shader;
    delete red_in_screen.shader;
    wgpuBindGroupRelease(time_bindgroup);
    wgpuBufferRelease(time_buffer);*/
    Renderer::clean();
}

void SampleRenderer::update(float delta_time)
{
    time += delta_time;
    time = std::fmod(time, 1.0f);
    
    //webgpu_context->update_buffer(time_buffer, 0, &time, sizeof(time));
    Renderer::update(delta_time);
}

void SampleRenderer::render()
{

    /*
    
    if (ImGui::Button("switch post process")) {
        post_process_swap_passes(red_in_screen.id, black_and_white.id);
    }

    std::vector<tPostProcess> ordered_post_process = Renderer::post_process_get_ids_in_render_order(BEFORE_TRANSPARENTS_AND_TAA);
    ImGui::SeparatorText("AFTER LIGHT PASS");
    for (auto id : ordered_post_process) {
        bool enabled = Renderer::post_process_is_activated(id);
        std::string shader_name;
        if (id == red_in_screen.id) {
            shader_name = red_in_screen.name;
        } else if (id == black_and_white.id) {
            shader_name = black_and_white.name;
        } else if (id == contrast.id) {
            shader_name = contrast.name;
        } else if (id == blur.id) {
            shader_name = blur.name;
        }



        if (ImGui::Checkbox(shader_name.c_str(), &enabled)) {
            Renderer::post_process_set_active(id, enabled);
        }
        for (auto id_swap : ordered_post_process) {
            if (id != id_swap) {
                std::string shader_name_swap;
                if (id_swap == red_in_screen.id) {
                    shader_name_swap = red_in_screen.name;
                }
                else if (id_swap == black_and_white.id) {
                    shader_name_swap = black_and_white.name;
                }
                else if (id_swap == contrast.id) {
                    shader_name_swap = contrast.name;
                }
                else if (id_swap == blur.id) {
                    shader_name_swap = blur.name;
                }
                std::string button_id = shader_name_swap + "##swap_" + shader_name;

                if (ImGui::SmallButton(button_id.c_str())) {
                    Renderer::post_process_swap_passes(id, id_swap);
                }
            }
        }




    }

    ImGui::SeparatorText("AFTER_TAA");
    ordered_post_process = Renderer::post_process_get_ids_in_render_order(AFTER_TRANSPARENTS);

    for (auto id : ordered_post_process) {
        bool enabled = Renderer::post_process_is_activated(id);
        std::string shader_name;
        if (id == red_in_screen.id) {
            shader_name = red_in_screen.name;
        }
        else if (id == black_and_white.id) {
            shader_name = black_and_white.name;
        }
        else if (id == contrast.id) {
            shader_name = contrast.name;
        }
        else if (id == blur.id) {
            shader_name = blur.name;
        }



        if (ImGui::Checkbox(shader_name.c_str(), &enabled)) {
            Renderer::post_process_set_active(id, enabled);
        }
        for (auto id_swap : ordered_post_process) {
            if (id != id_swap) {
                std::string shader_name_swap;
                if (id_swap == red_in_screen.id) {
                    shader_name_swap = red_in_screen.name;
                }
                else if (id_swap == black_and_white.id) {
                    shader_name_swap = black_and_white.name;
                }
                else if (id_swap == contrast.id) {
                    shader_name_swap = contrast.name;
                }
                else if (id_swap == blur.id) {
                    shader_name_swap = blur.name;
                }
                std::string button_id = shader_name_swap + "##swap_" + shader_name;

                if (ImGui::SmallButton(button_id.c_str())) {
                    Renderer::post_process_swap_passes(id, id_swap);
                }
            }
        }
    }
    ImGui::Separator();
    */

    //Renderer::post_process_copy_post_process_to_texture(dst_texture_for_post_process, black_and_white.id);

/*
    ImGui::Checkbox("taa", &taa_activated);

    if (post_process_is_activated(temporal_AA_data.id) != taa_activated) {
        post_process_set_active(temporal_AA_data.id, taa_activated);
    }
    */
    Renderer::render();

}
