#include "ink/utils/Mainloop.h"

#define A "\0\0\0"
#define B "\xEE\xBB\x66"
#define C "\xFF\xDD\xBB"

using namespace Ink;

const char* block =
A B B B B B B B B B B A B B B B B A A A B B B B B B B B C C C A B B B B
B B B B C C C B B B B B B B B B B A A A B B B B B B B B C C C A A A B B
B B B B C C C C C A A B B B A A A B B C C C A B B C C C A A A C C C A B
B C C C C C C C C C B B B B C C C C C C C B B B A B B B B B B B B B B A;

Scene scene;
Viewer viewer;
Renderer renderer;

void conf(Settings& t) {
	t.title = "Ink3D Example";
	t.show_cursor = false;
	t.lock_cursor = true;
	t.background_color = Vec3(1, 0.93, 0.8);
}

void load() {
	auto* cube = Instance::create();
	cube->mesh = new Mesh(BoxMesh::create());
	scene.add(cube);
	
	auto* image = new Image(12, 12, 3);
	std::copy_n(block, 12 * 12 * 3, &image->data[0]);
	
	auto* material = new Material();
	material->color_map = image;
	scene.set_material("default", material);
	
	auto* light = new HemisphereLight(Vec3(1), Vec3(0.5));
	light->direction = Vec3(0, 0, -1);
	scene.add_light(light);
	
	viewer = Viewer(PerspCamera(75 * DEG_TO_RAD, 1.77, 0.05, 500));
	viewer.set_position(Vec3(0, 0, -2));
	
	renderer.set_rendering_mode(FORWARD_RENDERING);
	renderer.set_texture_callback([](Gpu::Texture& t) -> void {
		t.set_filters(TEXTURE_NEAREST, TEXTURE_NEAREST);
	});
	renderer.load_scene(scene);
	renderer.set_viewport(Gpu::Rect(960, 540));
}

void update(float dt) {
	Renderer::update_scene(scene);
	viewer.update(dt);
	renderer.render(scene, viewer.get_camera());
}

void quit() {}
