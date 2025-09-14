class Camera {
public:
	glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, -6.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
	int far = 4;
	float linear_t = 0.7f;

	glm::vec3 cameraDir = glm::vec3(0,0,0);

	void update_mouse(float yaw, float pitch) {
	    cameraDir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	    cameraDir.y = sin(glm::radians(pitch));
	    cameraDir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	    
		cameraFront = glm::normalize(cameraDir);
	}

	glm::mat4 get_view() {
		return glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
	}

};
