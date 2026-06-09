#include "addons/Mainloop.h"
#include "addons/utils/ImageUtils.h"
#include "addons/window/Viewer.h"

#define HIGH_DPI 0

#define VP_WIDTH 960 * (HIGH_DPI + 1)
#define VP_HEIGHT 540 * (HIGH_DPI + 1)

const char* shader_vert = R"(
uniform mat4 model_view_proj;

in vec3 vertex;

void main() {
	gl_Position = model_view_proj * vec4(vertex, 1.);
}
)";

const char* shader_frag = R"(
uniform float index;

layout(location = 0) out float out_color;

void main() {
	out_color = index / 255.;
}
)";

std::unordered_map<std::string, ink::Mesh> meshes;
std::unordered_map<std::string, ink::Material> materials;
std::unordered_map<std::string, ink::Instance*> instances;

struct ObjectInfo {
	ink::Vec3 direction;
	ink::Uniforms uniforms;
} objects[100];

ink::Scene scene;
ink::Scene another_scene;
ink::Renderer renderer;
ink::PerspCamera camera;

ink::Image index_image;

ink::gpu::Texture* index_map = nullptr;
ink::gpu::Renderbuffer* index_buffer = nullptr;
ink::gpu::RenderTarget* index_target = nullptr;

void conf(Settings& t) {
	t.title = "Select Box";
	t.width = 960;
	t.height = 540;
	t.highdpi = HIGH_DPI;
	t.borderless = true;
	t.background_color = ink::Vec3(0, 0, 0);
}

void load() {
	meshes["Box"] = ink::BoxMesh::create();
	
	materials["Box_Red"] = ink::Material();
	materials["Box_Red"].color = ink::Vec3(1, 0.5, 0.5);

	materials["Box_Wire"] = ink::Material();
	materials["Box_Wire"].depth_test = false;
	materials["Box_Wire"].blending = true;
	materials["Box_Wire"].alpha = 0.4;
	materials["Box_Wire"].emissive = ink::Vec3(1, 1, 0);
	
	ink::gpu::Shader* shader = new ink::gpu::Shader();
	shader->load_vert(shader_vert);
	shader->load_frag(shader_frag);
	shader->compile();
	
	for (int i = 0; i < 100; ++i) {
		std::string name = "Box_" + std::to_string(i);
		
		auto& direction = objects[i].direction;
		do {
			direction.x = ink::Random::random_f() * 2 - 1;
			direction.y = ink::Random::random_f() * 2 - 1;
			direction.z = ink::Random::random_f() * 2 - 1;
		} while(direction.magnitude() > 1);
		direction = direction.normalize() * 0.01;
		
		auto& uniforms = objects[i].uniforms;
		uniforms.set_f("index", i + 1);
		materials[name].uniforms = &uniforms;
		materials[name].shader = shader;
		
		instances[name] = new ink::Instance(name);
		instances[name]->position.x = ink::Random::random_f() * 10 - 5;
		instances[name]->position.y = ink::Random::random_f() * 10 - 5;
		instances[name]->position.z = ink::Random::random_f() * 10 - 5;
		instances[name]->mesh = &meshes["Box"];
		
		scene.add(instances[name]);
		scene.set_material("default", *instances[name], &materials["Box_Red"]);
		
		another_scene.add(instances[name]);
		another_scene.set_material("default", *instances[name], &materials[name]);
	}
	
	ink::HemisphereLight* light = new ink::HemisphereLight();
	light->ground_color = ink::Vec3(0.5, 0.5, 0.5);
	scene.add_light(light);
	
	materials["line"] = ink::Material();
	materials["line"].color = ink::Vec3(1, 1, 1);
	materials["line"].wireframe = true;
	
	scene.set_material("line", &materials["line"]);
	
	renderer.set_rendering_mode(ink::FORWARD_RENDERING);
	renderer.load_scene(scene);
	renderer.set_viewport(ink::gpu::Rect(VP_WIDTH, VP_HEIGHT));
	
	camera = ink::PerspCamera(75 * ink::DEG_TO_RAD, 1.77, 0.05, 1000);
	camera.lookat(ink::Vec3(0, 0, -8), ink::Vec3(0, 0, -1), ink::Vec3(0, 1, 0));
	
	index_map = new ink::gpu::Texture();
	index_map->init_2d(VP_WIDTH, VP_HEIGHT, ink::TEXTURE_R8_UNORM);
	index_map->set_filters(ink::TEXTURE_LINEAR, ink::TEXTURE_LINEAR);
	
	index_buffer = new ink::gpu::Renderbuffer();
	index_buffer->init(VP_WIDTH, VP_HEIGHT, ink::TEXTURE_D24_UNORM);
	
	index_target = new ink::gpu::RenderTarget();
	index_target->set_texture(*index_map, 0);
	index_target->set_depth_buffer(*index_buffer);
	
	index_image = ink::Image(VP_WIDTH, VP_HEIGHT, 1);
}

void update(float dt) {
	for (int i = 0; i < 100; ++i) {
		std::string name = "Box_" + std::to_string(i);
		ink::Euler& rotation = instances[name]->rotation;
		rotation.x += objects[i].direction.x;
		rotation.y += objects[i].direction.y;
		rotation.z += objects[i].direction.z;
	}
	
	ink::Renderer::update_scene(scene);
	ink::Renderer::update_scene(another_scene);
	
	renderer.set_target(index_target);
	renderer.clear(true, true, false);
	renderer.render(another_scene, camera);
	
	index_map->copy_to_image(index_image);
	
	auto cursor_pos = ink::Window::get_cursor_position();
	auto window_size = ink::Window::get_size();
	float cursor_x = static_cast<float>(cursor_pos.first) / window_size.first;
	float cursor_y = 1 - static_cast<float>(cursor_pos.second) / window_size.second;
	int index = ink::ImageUtils::nearest_sample(index_image, 0, cursor_x, cursor_y) * 255. - 1;
	
	renderer.set_target(nullptr);
	renderer.render(scene, camera);
	
	ink::Scene highlight_scene;
	if (index >= 0) {
		highlight_scene.add(scene.get_child(index));
		highlight_scene.set_material("default", &materials["Box_Wire"]);
	}
	renderer.load_scene(highlight_scene);
	renderer.render_transparent(highlight_scene, camera);
}

void quit() {}
