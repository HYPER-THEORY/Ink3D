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

#pragma once

#include <initializer_list>
#include <unordered_map>

#include "../objects/Material.h"
#include "../objects/Instance.h"
#include "../lights/PointLight.h"
#include "../lights/SpotLight.h"
#include "../lights/DirectionalLight.h"
#include "../lights/AmbientLight.h"
#include "../lights/HemisphereLight.h"

namespace Ink {

class Scene : public Instance {
public:
	/**
	 * Create a new Scene with its name.
	 *
	 * \param n scene name
	 */
	Scene(const std::string& n = "");
	
	/**
	 * Returns the materials linked with the specified mesh matching the name.
	 *
	 * \param s the specified mesh
	 * \param n material name
	 */
	const Material* get_material(const Mesh* s, const std::string& n) const;
	
	/**
	 * Returns the materials linked with all meshes matching the name.
	 *
	 * \param n material name
	 */
	const Material* get_material(const std::string& n) const;
	
	/**
	 * Returns all the materials in the material library.
	 */
	std::vector<const Material*> get_materials() const;
	
	/**
	 * Link materials with the specified mesh matching the name.
	 *
	 * \param s the specified mesh
	 * \param n material name
	 * \param m material
	 */
	void set_material(const Mesh* s, const std::string& n, const Material* m);
	
	/**
	 * Link materials with all meshes matching the name.
	 *
	 * \param n material name
	 * \param m material
	 */
	void set_material(const std::string& n, const Material* m);
	
	/**
	 * Clear all the materials in the scene.
	 */
	void clear_material();
	
	/**
	 * Add a point light to the scene.
	 *
	 * \param l point light
	 */
	void add_light(const PointLight* l);
	
	/**
	 * Add a spot light to the scene.
	 *
	 * \param l spot light
	 */
	void add_light(const SpotLight* l);
	
	/**
	 * Add a directional light to the scene.
	 *
	 * \param l directional light
	 */
	void add_light(const DirectionalLight* l);
	
	/**
	 * Add an ambient light to the scene.
	 *
	 * \param l ambient light
	 */
	void add_light(const AmbientLight* l);
	
	/**
	 * Add an ambient light to the scene.
	 *
	 * \param l ambient light
	 */
	void add_light(const HemisphereLight* l);
	
	/**
	 * Returns the number of point lights.
	 */
	size_t get_point_light_count() const;
	
	/**
	 * Returns the point light at the index.
	 *
	 * \param i the index of light
	 */
	const PointLight* get_point_light(int i) const;
	
	/**
	 * Returns the number of spot lights.
	 */
	size_t get_spot_light_count() const;
	
	/**
	 * Returns the spot light at the index.
	 *
	 * \param i the index of light
	 */
	const SpotLight* get_spot_light(int i) const;
	
	/**
	 * Returns the number of directional lights.
	 */
	size_t get_directional_light_count() const;
	
	/**
	 * Returns the directional light at the index.
	 *
	 * \param i the index of light
	 */
	const DirectionalLight* get_directional_light(int i) const;
	
	/**
	 * Returns the number of ambient lights.
	 */
	size_t get_ambient_light_count() const;
	
	/**
	 * Returns the ambient light at the index.
	 *
	 * \param i the index of light
	 */
	const AmbientLight* get_ambient_light(int i) const;
	
	/**
	 * Returns the number of hemisphere lights.
	 */
	size_t get_hemisphere_light_count() const;
	
	/**
	 * Returns the hemisphere light at the index.
	 *
	 * \param i the index of light
	 */
	const HemisphereLight* get_hemisphere_light(int i) const;
	
	/**
	 * Clear all the lights in the scene.
	 */
	void clear_light();
	
	/**
	 * Update the local and global matrices of descendant instances.
	 */
	void update_instances();
	
	/**
	 * Returns all descendant instances to instance array.
	 */
	std::vector<const Instance*> to_instances() const;
	
private:
	std::unordered_map<std::string, const Material*> material_library;
	
	std::vector<const PointLight*> point_lights;
	std::vector<const SpotLight*> spot_lights;
	std::vector<const DirectionalLight*> directional_lights;
	std::vector<const AmbientLight*> ambient_lights;
	std::vector<const HemisphereLight*> hemisphere_lights;
};

}
