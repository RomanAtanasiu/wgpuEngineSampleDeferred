#include "sample_engine.h"

#include "framework/nodes/environment_3d.h"
#include "framework/parsers/parse_gltf.h"
//#include "framework/parsers/parse_obj.h"
#include "framework/input.h"

#include "graphics/sample_renderer.h"
#include "graphics/renderer_storage.h"
#include "graphics/primitives/quad_mesh.h"

#include "engine/scene.h"

#include "shaders/mesh_grid.wgsl.gen.h"
#include "framework/nodes/omni_light_3d.h"

#include "spdlog/spdlog.h"
#include "cmath"
#include <numbers>
int SampleEngine::initialize(Renderer* renderer, const sEngineConfiguration& configuration)
{
	return Engine::initialize(renderer, configuration);
}

int SampleEngine::post_initialize()
{
    Engine::post_initialize();

    // Create skybox
    {
        MeshInstance3D* skybox = new Environment3D();
        main_scene->add_node(skybox);
    }

    // Load Meta Quest Controllers and Controller pointer
    if (renderer->get_xr_available())
    {
        std::vector<Node*> entities_left;
        std::vector<Node*> entities_right;
        GltfParser parser;
        parser.parse("data/meshes/controllers/left_controller.glb", entities_left);
        parser.parse("data/meshes/controllers/right_controller.glb", entities_right);
        controller_mesh_left = static_cast<Node3D*>(entities_left[0]);
        controller_mesh_right = static_cast<Node3D*>(entities_right[0]);
    }

    // Create grid
    {
        MeshInstance3D* grid = new MeshInstance3D();
        grid->set_name("Grid");
        grid->set_mesh(new QuadMesh());
        grid->set_position(glm::vec3(0.0f));
        grid->rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        grid->scale(glm::vec3(10.f));
        grid->set_frustum_culling_enabled(false);

        // NOTE: first set the transparency and all types BEFORE loading the shader
        Material* grid_material = new Material();
        grid_material->set_transparency_type(ALPHA_BLEND);
        grid_material->set_cull_type(CULL_NONE);
        grid_material->set_type(MATERIAL_UNLIT);
        grid_material->set_shader(RendererStorage::get_shader_from_source(shaders::mesh_grid::source, shaders::mesh_grid::path, shaders::mesh_grid::libraries, grid_material));
        grid->set_surface_material_override(grid->get_surface(0), grid_material);

        //main_scene->add_node(grid);
    }
    //main_scene->parse(R"(C:\Users\u213937\Desktop\main_sponza\NewSponza_Main_glTF_003.gltf)");
    
//    main_scene->parse(R"(D:\Downloads\shoes_light\shoes_with_lights.gltf)");


    std::vector<Node*> parsed_entities;
    GltfParser parser;
    parser.parse(R"(C:\Users\u213937\Downloads\AlphaBlendModeTest.glb)", parsed_entities);
    //parser.parse(R"(C:\Users\u213937\Documents\sponza_tangent.glb)", parsed_entities);
    int x_lights = 5;
    int y_lights = 5;
    int z_lights = 5;

    /*
    
    for (int i = 0; i < x_lights;i++) {
        for (int j = 0; j < y_lights; j++) {
            for (int k = 0; k < z_lights; k++) {
                OmniLight3D* light = new OmniLight3D();
                light->set_intensity(10.0);
                light->set_range(3.0);
                
                light->set_position(glm::vec3(-10+ 4.0 * i, 4.0 * j, -6+2.2 * k));

                main_scene->add_node(light);
            }
        }
    }*/
    /*
    Node3D* root_entity = static_cast<Node3D*>(parsed_entities[0]);
    root_entity->scale(glm::vec3(0.5f));
    */
    main_scene->add_nodes(parsed_entities);
    
    root_entity = static_cast<Node3D*>(parsed_entities[0]);
    renderer->get_camera()->look_at({ 0.608711f, 0.749335f, 2.64692f }, { 1.15929f, 0.827252f, 1.81575f }, {0 ,1.0 ,0});
    


    return 0u;
}

void SampleEngine::clean()
{
    Engine::clean();
}

void SampleEngine::update(float delta_time)
{
    Engine::update(delta_time);

    main_scene->update(delta_time);

    if (renderer->get_xr_available()) {
        controller_mesh_left->set_transform(Transform::mat4_to_transform(Input::get_controller_pose(HAND_LEFT)));
        controller_mesh_right->set_transform(Transform::mat4_to_transform(Input::get_controller_pose(HAND_RIGHT)));
    }
    time += delta_time;
    if (time > 2) time -= 2;
   
    float t = 0;

    if (renderer->current_frame != 1) {
        t = (std::numbers::pi / 40.0) * float(renderer->current_frame);

    }
    float radio = 0.1;
    renderer->get_camera()->look_at({ 0.608711f , 0.749335f, 2.64692f  + float(renderer->current_frame) * 0.005}, {1.15929f + std::cos(t) * radio  , 0.827252f + std::sin(t) * radio , 1.81575f}, {0 ,1.0 ,0});

    /*
    if (time < 1) {
       root_entity->translate(glm::vec3(0.01*velocity, 0.005*velocity, 0.01*velocity));
    }
    else {
        root_entity->translate(glm::vec3(-0.01* velocity, -0.005* velocity, -0.01* velocity));
    }*/
}

void SampleEngine::render()
{
    if (show_imgui) {
        render_default_gui();
        ImGui::SliderFloat("velocity", &velocity, 0.1, 100);

    }

    main_scene->render();

    if (renderer->get_xr_available()) {
        controller_mesh_left->render();
        controller_mesh_right->render();
    }

    Engine::render();
}


void SampleEngine::append_glb(const std::string& filename)
{
    std::vector<Node*> entities;

    GltfParser gltf_parser;

    gltf_parser.parse(filename.c_str(), entities, PARSE_NO_FLAGS);

    if (!entities.empty()) {
        main_scene->add_nodes(entities);
    }
}
