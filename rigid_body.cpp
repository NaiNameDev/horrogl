#define gravity (9.8 * delta)

class rigid_body {
public:
	glm::vec3 velocity = glm::vec3(0);
	glm::vec3 position;
	collision_area area;

	bool is_on_floor = false;

	rigid_body(){}

	rigid_body(collision_area narea, glm::vec3 pos = glm::vec3(0,0,0)) {
		area = narea;
		position = pos;
	}

	void move(float delta) {
		position += velocity * delta;
		std::cout << velocity.x * delta << " " << velocity.y * delta << " " << velocity.z * delta << std::endl;
	}
	void apply_gravity(float delta) {
		if (!is_on_floor) {
			velocity.y -= gravity;
		}
	}
	void check_floor(glm::vec3 aabb) {
		if (aabb.y > 0.01f) {
			//is_on_floor = true;
			velocity.y = 0;
		}
		else is_on_floor = false;
	}
};
