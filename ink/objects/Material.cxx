/**
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

#include "Material.h"

namespace Ink {

Material::Material(const std::string& n) : name(n) {}

Uniforms Material::get_uniforms() const {
	Uniforms vars = {
		{"alpha_test_f", &alpha_test},
		{"color_v3"    , &color     },
		{"color_map_i" , &color_map },
		{"alpha_f"     , &alpha     },
		{"emissive_v3" , &emissive  },
		{"metalness_f" , &metalness },
		{"roughness_f" , &roughness },
		{"specular_f"  , &specular  },
	};
	if (normal_map != nullptr) {
		vars.set("normal_scale_f", &normal_scale);
	}
	if (displacement_map != nullptr) {
		vars.set("displacement_scale_f", &displacement_scale);
	}
	return vars;
}

Defines Material::get_defines() const {
	Defines defines;
	
	/* check whether to use normal map */
	if (normal_map != nullptr) {
		defines.set("USE_NORMAL_MAP");
	}
	
	/* check whether to use tangent space */
	if (normal_map != nullptr && tangent_space) {
		defines.set("USE_TANGENT");
	}
	
	/* check whether to use displacement map */
	if (displacement_map != nullptr) {
		defines.set("USE_DISPLACEMENT_MAP");
	}
	
	/* check whether to use color map */
	if (color_map != nullptr) {
		defines.set("USE_COLOR_MAP");
	}
	
	/* check whether to use emissive map */
	if (emissive_map != nullptr) {
		defines.set("USE_EMISSIVE_MAP");
	}
	
	/* check whether to use metalness map */
	if (metalness_map != nullptr) {
		defines.set("USE_METALNESS_MAP");
	}
	
	/* check whether to use roughness map */
	if (roughness_map != nullptr) {
		defines.set("USE_ROUGHNESS_MAP");
	}
	
	/* check whether to use specular map */
	if (specular_map != nullptr) {
		defines.set("USE_SPECULAR_MAP");
	}
	
	/* check whether to use light probe */
	if (light_probe != nullptr) {
		defines.set("USE_LIGHT_PROBE");
	}
	
	/* check whether to use environment probe */
	if (env_probe != nullptr) {
		defines.set("USE_ENV_PROBE");
	}
	return defines;
}

size_t Material::get_image_count() const {
	return images.size();
}

void Material::add_image(const Image* i) {
	images.emplace_back(i);
}

void Material::add_images(const std::initializer_list<const Image*>& l) {
	images.insert(images.end(), l);
}

void Material::clear_image() {
	images.clear();
}

const Image* Material::get_image(int i) const {
	return images[i];
}

const void* Material::get_shader() const {
	return shader;
}

void Material::set_shader(const void* s) {
	shader = s;
}

const void* Material::get_light_probe() const {
	return light_probe;
}

void Material::set_light_probe(const void* p) {
	light_probe = p;
}

const void* Material::get_env_probe() const {
	return env_probe;
}

void Material::set_env_probe(const void* p) {
	env_probe = p;
}

}
