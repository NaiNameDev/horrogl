class collision_area {
public:
	mesh collision_mesh;

	int max_x;
	int max_y;
	int max_z;

	int min_x;
	int min_y;
	int min_z;

	collision_area(mesh ncm) {
		collision_mesh = ncm;
		
		max_x = collision_mesh.own_vertices[0];
		max_y = collision_mesh.own_vertices[1];
		max_z = collision_mesh.own_vertices[2];

		min_x = collision_mesh.own_vertices[0];
		min_y = collision_mesh.own_vertices[1];
		min_z = collision_mesh.own_vertices[2];
		
		for (int i = 0; i < (collision_mesh.vertices_cnt / 4) - 3; i += 3) {
			if (max_x < collision_mesh.own_vertices[i]) max_x = collision_mesh.own_vertices[i];
			if (max_y < collision_mesh.own_vertices[i + 1]) max_y = collision_mesh.own_vertices[i];
			if (max_z < collision_mesh.own_vertices[i + 2]) max_z = collision_mesh.own_vertices[i];
		
			if (min_x > collision_mesh.own_vertices[i]) min_x = collision_mesh.own_vertices[i];
			if (min_y > collision_mesh.own_vertices[i + 1]) min_y = collision_mesh.own_vertices[i];
			if (min_z > collision_mesh.own_vertices[i + 2]) min_z = collision_mesh.own_vertices[i];
		}
	}

	glm::vec3 is_aabb_colliding_with(collision_area area) {
		glm::vec3 ret = glm::vec3(0.0f, 0.0f, 0.0f);

		
		// AABB collision check lock ugly bruh
		if ((area.max_x + area.collision_mesh.position.x > min_x + collision_mesh.position.x && area.max_x + area.collision_mesh.position.x < max_x + collision_mesh.position.x) || (area.min_x + area.collision_mesh.position.x > min_x + collision_mesh.position.x && area.min_x + area.collision_mesh.position.x < max_x + collision_mesh.position.x)) ret.x = 1.0f;
		if ((area.max_y + area.collision_mesh.position.y > min_y + collision_mesh.position.y && area.max_y + area.collision_mesh.position.y < max_y + collision_mesh.position.y) || (area.min_y + area.collision_mesh.position.y > min_y + collision_mesh.position.y && area.min_y + area.collision_mesh.position.y < max_y + collision_mesh.position.y)) ret.y = 1.0f;
		if ((area.max_z + area.collision_mesh.position.z > min_z + collision_mesh.position.z && area.max_z + area.collision_mesh.position.z < max_z + collision_mesh.position.z) || (area.min_z + area.collision_mesh.position.z > min_z + collision_mesh.position.z && area.min_z + area.collision_mesh.position.z < max_z + collision_mesh.position.z)) ret.z = 1.0f;
		
		return ret;
	}
};

class physics_layer {
public:
	//std::vector<> layer;
};
