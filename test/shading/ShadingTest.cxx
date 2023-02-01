#include "../../ink/utils/Mainloop.h"
#include "../../ink/utils/Viewer.h"

#define USE_FORWARD_PATH 0
#define HIGH_DPI 1

#define VP_WIDTH (960 << HIGH_DPI)
#define VP_HEIGHT (540 << HIGH_DPI)

#define PATH "test/shading/DamagedHelmet/"
#define PATH_S "test/shading/Bridge2/"

std::unordered_map<std::string, Ink::Mesh> meshes;
std::unordered_map<std::string, Ink::Image> images;
std::unordered_map<std::string, Ink::Material> materials;

Ink::Scene scene;
Ink::Viewer viewer;
Ink::Renderer renderer;

Ink::Gpu::Texture* buffers = nullptr;
Ink::Gpu::RenderTarget* base_target = nullptr;

Ink::Gpu::Texture* post_map_0 = nullptr;
Ink::Gpu::RenderTarget* post_target_0 = nullptr;

Ink::Gpu::Texture* post_map_1 = nullptr;
Ink::Gpu::RenderTarget* post_target_1 = nullptr;

Ink::LightPass* light_pass = nullptr;
Ink::BloomPass* bloom_pass = nullptr;
Ink::ToneMapPass* tone_map_pass = nullptr;
Ink::FXAAPass* fxaa_pass = nullptr;

void conf(Settings& t) {
	t.title = "Shading Test";
	t.width = 960;
	t.height = 540;
	t.highdpi = HIGH_DPI;
	t.show_cursor = false;
	t.lock_cursor = true;
	t.borderless = true;
}

void load() {
	meshes["Helmet"] = Ink::Loader::load_obj(PATH "Helmet.obj")[0];
	meshes["Helmet"].create_tangents();
	
	images["Helmet_A"] = Ink::Loader::load_image(PATH "Default_albedo.jpg");
	images["Helmet_A"].flip_vertical();
	
	images["Helmet_N"] = Ink::Loader::load_image(PATH "Default_normal.jpg");
	images["Helmet_N"].flip_vertical();
	
	images["Helmet_AO"] = Ink::Loader::load_image(PATH "Default_AO.jpg");
	images["Helmet_AO"].flip_vertical();
	
	images["Helmet_E"] = Ink::Loader::load_image(PATH "Default_emissive.jpg");
	images["Helmet_E"].flip_vertical();
	
	images["Helmet_MR"] = Ink::Loader::load_image(PATH "Default_metalRoughness.jpg");
	images["Helmet_MR"].flip_vertical();
	
	images["Skybox_PX"] = Ink::Loader::load_image(PATH_S "posx.jpg");
	images["Skybox_NX"] = Ink::Loader::load_image(PATH_S "negx.jpg");
	images["Skybox_PY"] = Ink::Loader::load_image(PATH_S "posy.jpg");
	images["Skybox_NY"] = Ink::Loader::load_image(PATH_S "negy.jpg");
	images["Skybox_PZ"] = Ink::Loader::load_image(PATH_S "posz.jpg");
	images["Skybox_NZ"] = Ink::Loader::load_image(PATH_S "negz.jpg");
	
	auto helmet_mr = images["Helmet_MR"].split();
	images["Helmet_M"] = helmet_mr[2];
	images["Helmet_R"] = helmet_mr[1];
	
	Ink::ReflectionProbe* probe = new Ink::ReflectionProbe();
	probe->intensity = 2;
	probe->load_cubemap(images["Skybox_PX"], images["Skybox_NX"],
						images["Skybox_PY"], images["Skybox_NY"],
						images["Skybox_PZ"], images["Skybox_NZ"]);
	
	materials["Material_MR"] = Ink::Loader::load_mtl(PATH "Helmet.mtl")[0];
	materials["Material_MR"].emissive = Ink::Vec3(1, 1, 1);
	materials["Material_MR"].emissive_intensity = 2;
	materials["Material_MR"].roughness = 1;
	materials["Material_MR"].metalness = 1;
	materials["Material_MR"].color_map = &images["Helmet_A"];
	materials["Material_MR"].normal_map = &images["Helmet_N"];
	materials["Material_MR"].ao_map = &images["Helmet_AO"];
	materials["Material_MR"].emissive_map = &images["Helmet_E"];
	materials["Material_MR"].metalness_map = &images["Helmet_M"];
	materials["Material_MR"].roughness_map = &images["Helmet_R"];
	materials["Material_MR"].reflection_probe = probe;
	
	scene.set_material("Material_MR", &materials["Material_MR"]);
	
	Ink::Instance* helmet = Ink::Instance::create();
	helmet->mesh = &meshes["Helmet"];
	scene.add(helmet);
	
	viewer = Ink::Viewer(Ink::PerspCamera(75 * Ink::DEG_TO_RAD, 1.77, 0.05, 1000));
	viewer.set_position(Ink::Vec3(0, 0, 2));
	viewer.set_direction(Ink::Vec3(0, 0, 1));
	
#if USE_FORWARD_PATH
	renderer.set_rendering_mode(Ink::FORWARD_RENDERING);
	renderer.set_tone_map(Ink::ACES_FILMIC_TONE_MAP, 1);
#endif
	
	renderer.load_skybox(images["Skybox_PX"], images["Skybox_NX"],
						 images["Skybox_PY"], images["Skybox_NY"],
						 images["Skybox_PZ"], images["Skybox_NZ"]);
	
	renderer.load_scene(scene);
	renderer.set_viewport(Ink::Gpu::Rect(VP_WIDTH, VP_HEIGHT));
	
#if !USE_FORWARD_PATH
	buffers = new Ink::Gpu::Texture[5];
	
	buffers[0].init_2d(VP_WIDTH, VP_HEIGHT, Ink::TEXTURE_R8G8B8A8_UNORM);
	buffers[0].set_filters(Ink::TEXTURE_NEAREST, Ink::TEXTURE_NEAREST);
	
	buffers[1].init_2d(VP_WIDTH, VP_HEIGHT, Ink::TEXTURE_R10G10B10A2_UNORM);
	buffers[1].set_filters(Ink::TEXTURE_NEAREST, Ink::TEXTURE_NEAREST);
	
	buffers[2].init_2d(VP_WIDTH, VP_HEIGHT, Ink::TEXTURE_R8G8B8A8_UNORM);
	buffers[2].set_filters(Ink::TEXTURE_NEAREST, Ink::TEXTURE_NEAREST);
	
	buffers[3].init_2d(VP_WIDTH, VP_HEIGHT, Ink::TEXTURE_R16G16B16_SFLOAT);
	buffers[3].set_filters(Ink::TEXTURE_NEAREST, Ink::TEXTURE_NEAREST);
	
	buffers[4].init_2d(VP_WIDTH, VP_HEIGHT, Ink::TEXTURE_D24_UNORM);
	buffers[4].set_filters(Ink::TEXTURE_NEAREST, Ink::TEXTURE_NEAREST);
	
	base_target = new Ink::Gpu::RenderTarget();
	base_target->set_texture(buffers[0], 0);
	base_target->set_texture(buffers[1], 1);
	base_target->set_texture(buffers[2], 2);
	base_target->set_texture(buffers[3], 3);
	base_target->set_depth_texture(buffers[4]);
	base_target->set_target_number(4);
	
	post_map_0 = new Ink::Gpu::Texture();
	post_map_0->init_2d(VP_WIDTH, VP_HEIGHT, Ink::TEXTURE_R16G16B16_SFLOAT);
	post_map_0->set_filters(Ink::TEXTURE_LINEAR, Ink::TEXTURE_LINEAR);
	
	post_target_0 = new Ink::Gpu::RenderTarget();
	post_target_0->set_texture(*post_map_0, 0);
	
	post_map_1 = new Ink::Gpu::Texture();
	post_map_1->init_2d(VP_WIDTH, VP_HEIGHT, Ink::TEXTURE_R16G16B16_SFLOAT);
	post_map_1->set_filters(Ink::TEXTURE_LINEAR, Ink::TEXTURE_LINEAR);
	
	post_target_1 = new Ink::Gpu::RenderTarget();
	post_target_1->set_texture(*post_map_1, 0);
	
	renderer.set_target(base_target);
	
	light_pass = new Ink::LightPass();
	light_pass->init();
	light_pass->set_buffer_c(buffers + 0);
	light_pass->set_buffer_n(buffers + 1);
	light_pass->set_buffer_m(buffers + 2);
	light_pass->set_buffer_a(buffers + 3);
	light_pass->set_buffer_d(buffers + 4);
	light_pass->set_target(post_target_0);
	
	bloom_pass = new Ink::BloomPass(VP_WIDTH, VP_HEIGHT);
	bloom_pass->init();
	bloom_pass->threshold = 1.0;
	bloom_pass->radius = 0.5;
	bloom_pass->intensity = 2.5;
	bloom_pass->set_texture(post_map_0);
	bloom_pass->set_target(post_target_1);
	
	tone_map_pass = new Ink::ToneMapPass();
	tone_map_pass->init();
	tone_map_pass->mode = Ink::ACES_FILMIC_TONE_MAP;
	tone_map_pass->set_texture(post_map_1);
	tone_map_pass->set_target(post_target_0);
	
	fxaa_pass = new Ink::FXAAPass();
	fxaa_pass->init();
	fxaa_pass->set_texture(post_map_0);
#endif
}

void update(float dt) {
	auto time_ms = Ink::Date::get_time();
	
	viewer.update(dt);
	auto& camera = viewer.get_camera();
	
	Ink::Renderer::update_scene(scene);
	
	renderer.clear();
	renderer.render_skybox(camera);
	renderer.render(scene, camera);
	
#if !USE_FORWARD_PATH
	light_pass->set(&scene, &camera);
	light_pass->render();
	bloom_pass->render();
	tone_map_pass->render();
	fxaa_pass->render();
#endif
	
	Ink::Gpu::State::finish();
	auto delta_ms = Ink::Date::get_time() - time_ms;
	std::cout << Ink::Format::format("Time: {} ms\n", delta_ms);
}

void quit() {}
