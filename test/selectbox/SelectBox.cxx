#include "../../ink/utils/Mainloop.h"
#include "../../ink/utils/Viewer.h"

#define HIGH_DPI 1

#define VP_WIDTH 960 * (HIGH_DPI + 1)
#define VP_HEIGHT 540 * (HIGH_DPI + 1)

const char* shader_vert = R"(
#include <common>

uniform mat4 model_view_proj;

in vec3 vertex;

void main() {
	gl_Position = model_view_proj * vec4(vertex, 1.);
}
)";

const char* shader_frag = R"(
#include <common>

uniform float index;

layout(location = 0) out float out_color;

void main() {
	out_color = index / 255.;
}
)";

std::unordered_map<std::string, Ink::Mesh> meshes;
std::unordered_map<std::string, Ink::Material> materials;
std::unordered_map<std::string, Ink::Instance*> instances;

struct ObjectInfo {
	float index = 0;
	Ink::Vec3 direction;
	Ink::Uniforms uniforms;
} objects[100];

Ink::Scene scene;
Ink::Scene another_scene;
Ink::Renderer renderer;
Ink::PerspCamera camera;

Ink::Image index_image;

Ink::Gpu::Texture* index_map = nullptr;
Ink::Gpu::RenderBuffer* index_buffer = nullptr;
Ink::Gpu::FrameBuffer* index_target = nullptr;

void conf(Settings& t) {
	t.title = "Select Box";
	t.width = 960;
	t.height = 540;
	t.highdpi = HIGH_DPI;
	t.borderless = true;
	t.background_color = Ink::Vec3(0, 0, 0);
}

void load() {
	meshes["Box"] = Ink::BoxMesh::create();
	
	materials["Box_Red"] = Ink::Material();
	materials["Box_Red"].color = Ink::Vec3(1, 0.5, 0.5);

	materials["Box_Green"] = Ink::Material();
	materials["Box_Green"].color = Ink::Vec3(0.5, 1, 0.5);
	
	Ink::Gpu::Shader* shader = new Ink::Gpu::Shader();
	shader->load_vert(shader_vert);
	shader->load_frag(shader_frag);
	shader->compile();
	
	for (int i = 0; i < 100; ++i) {
		std::string name = Ink::Format::format("Box_{}", i);
		
		objects[i].index = i + 1;
		objects[i].direction = Ink::Vec3::random() * 0.01;
		
		auto& uniforms = objects[i].uniforms;
		uniforms.set("index_f", &objects[i].index);
		materials[name].uniforms = &uniforms;
		materials[name].shader = shader;
		
		instances[name] = Ink::Instance::create(name);
		instances[name]->position.x = Ink::Random::random_f() * 10 - 5;
		instances[name]->position.y = Ink::Random::random_f() * 10 - 5;
		instances[name]->position.z = Ink::Random::random_f() * 10 - 5;
		instances[name]->mesh = &meshes["Box"];
		
		scene.add(instances[name]);
		scene.set_material("default", instances[name], &materials["Box_Red"]);
		
		another_scene.add(instances[name]);
		another_scene.set_material("default", instances[name], &materials[name]);
	}
	
	Ink::HemisphereLight* light = new Ink::HemisphereLight();
	light->ground_color = Ink::Vec3(0.5, 0.5, 0.5);
	scene.add_light(light);
	
	renderer.set_rendering_mode(Ink::FORWARD_RENDERING);
	renderer.set_tone_mapping(Ink::ACES_FILMIC_TONE_MAP, 1);
	
	renderer.load_scene(scene);
	renderer.set_viewport(Ink::Gpu::Rect(VP_WIDTH, VP_HEIGHT));
	
	camera = Ink::PerspCamera(75 * Ink::DEG_TO_RAD, 1.77, 0.05, 1000);
	camera.lookat(Ink::Vec3(0, 0, -8), Ink::Vec3(0, 0, -1), Ink::Vec3(0, 1, 0));
	
	index_map = new Ink::Gpu::Texture();
	index_map->init_2d(VP_WIDTH, VP_HEIGHT, Ink::TEXTURE_R8_UNORM);
	index_map->set_filters(Ink::TEXTURE_LINEAR, Ink::TEXTURE_LINEAR);
	
	index_buffer = new Ink::Gpu::RenderBuffer();
	index_buffer->init(VP_WIDTH, VP_HEIGHT, Ink::TEXTURE_D24_UNORM);
	
	index_target = new Ink::Gpu::FrameBuffer();
	index_target->set_attachment(*index_map, 0);
	index_target->set_depth_attachment(*index_buffer);
	
	index_image = Ink::Image(VP_WIDTH, VP_HEIGHT, 1);
}

void update(float dt) {
	for (int i = 0; i < 100; ++i) {
		std::string name = Ink::Format::format("Box_{}", i);
		Ink::Euler& rotation = instances[name]->rotation;
		rotation.x += objects[i].direction.x;
		rotation.y += objects[i].direction.y;
		rotation.z += objects[i].direction.z;
	}
	
	Ink::Renderer::update_scene(scene);
	Ink::Renderer::update_scene(another_scene);
	
	renderer.set_target(index_target);
	renderer.clear(false, true, false);
	renderer.render(another_scene, camera);
	
	index_map->copy_to_image(index_image);
	
	auto cursor_pos = Ink::Window::get_cursor_position();
	auto window_size = Ink::Window::get_size();
	float cursor_x = static_cast<float>(cursor_pos.first) / window_size.first;
	float cursor_y = 1 - static_cast<float>(cursor_pos.second) / window_size.second;
	int index = Ink::Soft::nearest_map(index_image, 0, cursor_x, cursor_y) * 255. - 1;
	
	for (int i = 0; i < 100; ++i) {
		scene.set_material("default", scene.get_child(i), &materials["Box_Red"]);
	}
	if (index >= 0) {
		scene.set_material("default", scene.get_child(index), &materials["Box_Green"]);
	}
	
	renderer.set_target(nullptr);
	renderer.render(scene, camera);
}

void quit() {}
