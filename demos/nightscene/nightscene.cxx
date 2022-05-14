/*
 * Copyright (C) 2021-2022 Hypertheory
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "nightscene.h"

void nightscene::init() {
	int material_size;
	simple_shading = std::make_unique<gpu::shader>();
	simple_shading->read("../demos/nightscene/shaders/simpleshading.vert.glsl",
						 "../demos/nightscene/shaders/simpleshading.frag.glsl");
	/* road */
	loader::load_obj("../demos/nightscene/models/road_straight.obj", &road);
	material_size = loader::load_mtl("../demos/nightscene/models/road_straight.mtl", road_material);
	for (int i = 0; i < material_size; ++i) {
		road_material[i].set_shader(simple_shading.get());
		night_scene.set_material(&road, road_material[i].name, road_material + i);
	}
	/* barrier */
	loader::load_obj("../demos/nightscene/models/construction_barrier.obj", &barrier);
	material_size = loader::load_mtl("../demos/nightscene/models/construction_barrier.mtl", barrier_material);
	for (int i = 0; i < material_size; ++i) {
		barrier_material[i].set_shader(simple_shading.get());
		night_scene.set_material(&barrier, barrier_material[i].name, barrier_material + i);
	}
	/* light */
	loader::load_obj("../demos/nightscene/models/light_curved.obj", &light);
	material_size = loader::load_mtl("../demos/nightscene/models/light_curved.mtl", light_material);
	for (int i = 0; i < material_size; ++i) {
		light_material[i].set_shader(simple_shading.get());
		night_scene.set_material(&light, light_material[i].name, light_material + i);
	}
	/* police */
	loader::load_obj("../demos/nightscene/models/police.obj", &police);
	material_size = loader::load_mtl("../demos/nightscene/models/police.mtl", police_material);
	for (int i = 0; i < material_size; ++i) {
		police_material[i].set_shader(simple_shading.get());
		night_scene.set_material(&police, police_material[i].name, police_material + i);
	}
	/* suv */
	loader::load_obj("../demos/nightscene/models/suv.obj", &suv);
	material_size = loader::load_mtl("../demos/nightscene/models/suv.mtl", suv_material);
	for (int i = 0; i < material_size; ++i) {
		suv_material[i].set_shader(simple_shading.get());
		night_scene.set_material(&suv, suv_material[i].name, suv_material + i);
	}
	/* night scene */
	night_scene.add_instance({&road,	{0.0, 0.0, 0.0},		{0.0, 0.0, 0.0},	{4.0, 1.0, 2.0}});
	night_scene.add_instance({&barrier,	{-0.15, 0.05, -0.1},	{0.0, 1.8, 0.5},	{1.0, 1.0, 1.0}});
	night_scene.add_instance({&barrier,	{-0.1, 0.05, 0.35},		{0.3, -1.2, 0.3},	{1.0, 1.0, 1.0}});
	night_scene.add_instance({&barrier,	{0.0, 0.0, 0.6},		{0.0, 0.0, 0.0},	{1.0, 1.0, 1.0}});
	night_scene.add_instance({&barrier,	{0.0, 0.0, -0.3},		{0.0, 0.0, 0.0},	{1.0, 1.0, 1.0}});
	night_scene.add_instance({&barrier,	{0.0, 0.0, -0.6},		{0.0, 0.0, 0.0},	{1.0, 1.0, 1.0}});
	night_scene.add_instance({&light,	{1.0, 0.0, 0.9},		{0.0, 0.0, 0.0},	{1.0, 1.0, 1.0}});
	night_scene.add_instance({&light,	{1.0, 0.0, -0.9},		{0.0, 3.14, 0.0},	{1.0, 1.0, 1.0}});
	night_scene.add_instance({&light,	{-0.5, 0.0, 0.9},		{0.0, 0.0, 0.0},	{1.0, 1.0, 1.0}});
	night_scene.add_instance({&light,	{-0.5, 0.0, -0.9},		{0.0, 3.14, 0.0},	{1.0, 1.0, 1.0}});
	night_scene.add_instance({&police,	{0.3, 0.0, -0.4},		{0.0, 0.4, 0.0},	{0.2, 0.2, 0.2}});
	night_scene.add_instance({&police,	{1.0, 0.0, 0.4},		{0.0, 1.07, 0.0},	{0.2, 0.2, 0.2}});
	night_scene.add_instance({&police,	{-1.4, 0.0, -0.45},		{0.0, -1.0, 0.0},	{0.2, 0.2, 0.2}});
	night_scene.add_instance({&suv,		{-0.4, 0.0, 0.0},		{0.0, 1.8, 0.0},	{0.2, 0.2, 0.2}});
}

scene nightscene::get_scene() {
	return night_scene;
}

scene nightscene::night_scene;
mesh nightscene::road;
mesh nightscene::barrier;
mesh nightscene::light;
mesh nightscene::police;
mesh nightscene::suv;
material nightscene::road_material[8];
material nightscene::barrier_material[8];
material nightscene::light_material[8];
material nightscene::police_material[8];
material nightscene::suv_material[8];
std::unique_ptr<gpu::shader> nightscene::simple_shading;
