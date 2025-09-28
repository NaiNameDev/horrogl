class Camera : public Node3D {
public:

	Camera(glm::vec3 new_pos = glm::vec3(0,0,0), std::string new_name = "Camera") : Node3D(new_pos, new_name) {}

	glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 camera_dir = glm::vec3(0,0,0);

	void update_mouse(float yaw, float pitch) {
	    camera_dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	    camera_dir.y = sin(glm::radians(pitch));
	    camera_dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	    
		camera_front = glm::normalize(camera_dir);
	}

	glm::mat4 get_view() {
		return glm::lookAt(get_global_position(), camera_front + get_global_position(), camera_up);
	}

};
