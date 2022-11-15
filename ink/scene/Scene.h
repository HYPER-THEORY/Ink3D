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

#include <unordered_map>

#include "../objects/Material.h"
#include "../objects/Instance.h"
#include "../lights/PointLight.h"
#include "../lights/SpotLight.h"
#include "../lights/DirectionalLight.h"
#include "../lights/HemisphereLight.h"
#include "../lights/Exp2Fog.h"
#include "../lights/LinearFog.h"

namespace Ink {

class Scene : public Instance {
public:
	/**
	 * Creates a new Scene and initializes it with a name.
	 *
	 * \param n scene name
	 */
	explicit Scene(const std::string& n = "");
	
	/**
	 * Returns the material matching the specified name and linking with the
	 * specified mesh.
	 *
	 * \param s the specified mesh
	 * \param n material name
	 */
	const Material* get_material(const Mesh* s, const std::string& n) const;
	
	/**
	 * Returns the material matching the specified name.
	 *
	 * \param n material name
	 */
	const Material* get_material(const std::string& n) const;
	
	/**
	 * Returns all the materials in the material library.
	 */
	std::vector<const Material*> get_materials() const;
	
	/**
	 * Sets the specified material with name to the scene. Links it with the
	 * specified mesh.
	 *
	 * \param s specified mesh
	 * \param n material name
	 * \param m material
	 */
	void set_material(const Mesh* s, const std::string& n, const Material* m);
	
	/**
	 * Sets the specified material with name to the scene.
	 *
	 * \param n material name
	 * \param m material
	 */
	void set_material(const std::string& n, const Material* m);
	
	/**
	 * Removes all the materials from the scene.
	 */
	void clear_material();
	
	/**
	 * Returns the linear fog in the scene if there is, return nullptr
	 * otherwise.
	 */
	const LinearFog* get_linear_fog() const;
	
	/**
	 * Sets the specified linear fog to the scene. Only one fog can be set in a
	 * scene.
	 *
	 * \param f linear fog
	 */
	void set_fog(const LinearFog* f);
	
	/**
	 * Returns the exp square fog in the scene if there is, return nullptr
	 * otherwise.
	 */
	const Exp2Fog* get_exp2_fog() const;
	
	/**
	 * Sets the specified exp square fog to the scene. Only one fog can be set
	 * in a scene.
	 *
	 * \param f exp square fog
	 */
	void set_fog(const Exp2Fog* f);
	
	/**
	 * Adds the specified point light to the scene. The light number should not
	 * exceed the maximum number.
	 *
	 * \param l point light
	 */
	void add_light(const PointLight* l);
	
	/**
	 * Adds the specified spot light to the scene. The light number should not
	 * exceed the maximum number.
	 *
	 * \param l spot light
	 */
	void add_light(const SpotLight* l);
	
	/**
	 * Adds the specified directional light to the scene. The light number
	 * should not exceed the maximum number.
	 *
	 * \param l directional light
	 */
	void add_light(const DirectionalLight* l);
	
	/**
	 * Adds the specified hemisphere light to the scene. The light number should
	 * not exceed the maximum number.
	 *
	 * \param l hemisphere light
	 */
	void add_light(const HemisphereLight* l);
	
	/**
	 * Removes all the point & spot & directional & hemisphere lights from the
	 * scene.
	 */
	void clear_light();
	
	/**
	 * Returns the number of point lights in the scene.
	 */
	size_t get_point_light_count() const;
	
	/**
	 * Returns the point light at the specified index in the scene.
	 *
	 * \param i the index of light
	 */
	const PointLight* get_point_light(int i) const;
	
	/**
	 * Returns the number of spot lights in the scene.
	 */
	size_t get_spot_light_count() const;
	
	/**
	 * Returns the spot light at the specified index in the scene.
	 *
	 * \param i the index of light
	 */
	const SpotLight* get_spot_light(int i) const;
	
	/**
	 * Returns the number of directional lights in the scene.
	 */
	size_t get_directional_light_count() const;
	
	/**
	 * Returns the directional light at the specified index in the scene.
	 *
	 * \param i the index of light
	 */
	const DirectionalLight* get_directional_light(int i) const;
	
	/**
	 * Returns the number of hemisphere lights in the scene.
	 */
	size_t get_hemisphere_light_count() const;
	
	/**
	 * Returns the hemisphere light at the specified index in the scene.
	 *
	 * \param i the index of light
	 */
	const HemisphereLight* get_hemisphere_light(int i) const;
	
	/**
	 * Updates the local and global matrices of all descendant instances.
	 */
	void update_instances();
	
	/**
	 * Returns an instance vector of all descendant instances.
	 */
	std::vector<const Instance*> to_instances() const;
	
	/**
	 * Returns an instance vector of all descendant visible instances.
	 */
	std::vector<const Instance*> to_visible_instances() const;
	
private:
	std::unordered_map<std::string, const Material*> material_library;
	
	const LinearFog* linear_fog = nullptr;
	const Exp2Fog* exp2_fog = nullptr;
	
	std::vector<const PointLight*> point_lights;
	std::vector<const SpotLight*> spot_lights;
	std::vector<const DirectionalLight*> directional_lights;
	std::vector<const HemisphereLight*> hemisphere_lights;
};

}
