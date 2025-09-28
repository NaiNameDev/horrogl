class CollisionMesh : public Mesh {
public:
	int max_x;
	int max_y;
	int max_z;

	int min_x;
	int min_y;
	int min_z;

	CollisionMesh(Mesh from, glm::vec3 npos = glm::vec3(0,0,0), std::string new_name = "CollisionMesh")
		: Mesh(from.own_vertices, from.vertices_cnt, from.shader, npos, new_name) {
		max_x = own_vertices[0];
		max_y = own_vertices[1];
		max_z = own_vertices[2];

		min_x = own_vertices[0];
		min_y = own_vertices[1];
		min_z = own_vertices[2];
		
		for (int i = 0; i < (vertices_cnt / 4) - 3; i += 3) {
			if (max_x < own_vertices[i]) max_x = own_vertices[i];
			if (max_y < own_vertices[i + 1]) max_y = own_vertices[i + 1];
			if (max_z < own_vertices[i + 2]) max_z = own_vertices[i + 2];
		
			if (min_x > own_vertices[i]) min_x = own_vertices[i];
			if (min_y > own_vertices[i + 1]) min_y = own_vertices[i + 1];
			if (min_z > own_vertices[i + 2]) min_z = own_vertices[i + 2];
		}
	}
	CollisionMesh(float* vertices, int vertices_size, Shader* new_shader, glm::vec3 npos = glm::vec3(0,0,0))
		: Mesh(vertices, vertices_size, new_shader, npos) {
		max_x = own_vertices[0];
		max_y = own_vertices[1];
		max_z = own_vertices[2];

		min_x = own_vertices[0];
		min_y = own_vertices[1];
		min_z = own_vertices[2];
		
		for (int i = 0; i < (vertices_cnt / 4) - 3; i += 3) {
			if (max_x < own_vertices[i]) max_x = own_vertices[i];
			if (max_y < own_vertices[i + 1]) max_y = own_vertices[i];
			if (max_z < own_vertices[i + 2]) max_z = own_vertices[i];
		
			if (min_x > own_vertices[i]) min_x = own_vertices[i];
			if (min_y > own_vertices[i + 1]) min_y = own_vertices[i];
			if (min_z > own_vertices[i + 2]) min_z = own_vertices[i];
		}
	}
	
	glm::vec3 is_aabb_colliding_with(CollisionMesh area) {
		glm::vec3 ret = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::vec3 my_global = get_global_position();
		glm::vec3 area_global = area.get_global_position();

		glm::vec3 my_min = glm::vec3(min_x + my_global.x, min_y + my_global.y, min_z + my_global.z);
		glm::vec3 my_max = glm::vec3(max_x + my_global.x, max_y + my_global.y, max_z + my_global.z);
	
		glm::vec3 area_min = glm::vec3(area.min_x + area_global.x, area.min_y + area_global.y, area.min_z + area_global.z);
		glm::vec3 area_max = glm::vec3(area.max_x + area_global.x, area.max_y + area_global.y, area.max_z + area_global.z);
		
		if (my_max.x >= area_min.x && my_max.x <= area_max.x) ret.x = area_min.x - my_max.x;
		if (my_min.x >= area_min.x && my_min.x <= area_max.x) ret.x = std::abs(my_min.x - area_max.x);

		if (my_max.y >= area_min.y && my_max.y <= area_max.y) ret.y = area_min.y - my_max.y;
		if (my_min.y >= area_min.y && my_min.y <= area_max.y) ret.y = std::abs(my_min.y - area_max.y);

		if (my_max.z >= area_min.z && my_max.z <= area_max.z) ret.z = area_min.z - my_max.z;
		if (my_min.z >= area_min.z && my_min.z <= area_max.z) ret.z = std::abs(my_min.z - area_max.z);

		return ret;
	}
};

class StaticBody;
class PhysicsLayer {
public:
	std::vector<StaticBody*> layer;

	void push_body(StaticBody* body) {
		layer.push_back(body);
	}
};

class StaticBody : public Node3D {
public:
	CollisionMesh* col_mesh;
	std::vector<PhysicsLayer*> layers;

	StaticBody(glm::vec3 npos = glm::vec3(0,0,0), std::string new_name = "StaticBody") 
	: Node3D(npos, new_name) { }

	void push_layer(PhysicsLayer* new_layer) {
		layers.push_back(new_layer);
	}
};

#define gravity (9.8 * delta)

class RigidBody : public StaticBody {
public:
	glm::vec3 velocity;
	bool is_on_floor = false;
	
	RigidBody(glm::vec3 npos = glm::vec3(0,0,0), std::string new_name = "RigidBody") 
	: StaticBody(npos, new_name) { }

	void apply_gravity(float delta) {
		velocity.y -= gravity;
	}
	void move(float delta) {
		position += velocity * delta;
	}
	void move_and_colide(float delta) {
		move(delta);
		
		for (int i = 0; i < layers.size(); i++) {
			for (int j = 0; j < layers[i]->layer.size(); j++) {
				if (layers[i]->layer[j] != this) {
					glm::vec3 aabb = col_mesh->is_aabb_colliding_with(*layers[i]->layer[j]->col_mesh);
					glm::vec3 maabb = get_min(aabb);
			
					if (glm::abs(aabb.x) > 0 && glm::abs(aabb.y) > 0 && glm::abs(aabb.z) > 0) {
						if (maabb.y > 0.0f) {
							is_on_floor = true;
							velocity.y = 0;
						}
						position += maabb; 
					}
				}
			}
		}
	}
};
