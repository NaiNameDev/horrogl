#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//bruh why glm does not have lerp bruh
float lerp(float a, float b, float t)
{
    return a * t + b * (1 - t);
}

#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "obj_reader.h"
#include "aabb_phys_object.cpp"

#define SCR_WIDTH 1920
#define SCR_HEIGHT 1080

//delta
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//camera
camera main_camera;
float yaw = 90.0f, pitch = 0.0f;
bool firstMouse = true;
float lastX = 0, lastY = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
	lastX = xpos;
    lastY = ypos;
	
	float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
	
    yaw   += xoffset;
    pitch += yoffset;
	
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		
	}
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
	
	}
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {}
}

int main() {
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "spheres and stars", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	Shader main_shader("vmain.glsl", "fmain.glsl");

	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	obj_reader rdr;
	rdr.read_vertices("./media/obj/cube.obj");
	mesh collision_mesh_1 = rdr.create_mesh(&main_shader);

	collision_area area1(collision_mesh_1);
	collision_area area2(collision_mesh_1);

	while (!glfwWindowShouldClose(window)) {
		main_camera.update_mouse(yaw, pitch);
		processInput(window);
		
		main_shader.setVec3("objectColor", glm::vec3(0.9f, 1.0f, 0.9f));
        main_shader.setVec3("lightColor", glm::vec3(0.5f ,0.5f, 0.5f));
        main_shader.setVec3("lightPos", main_camera.cameraPos);
        main_shader.setFloat("tran", 1.0f);
		main_shader.setFloat("u_time", glfwGetTime());
		main_shader.use();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		glm::mat4 view = main_camera.get_view();
        glm::mat4 projection = glm::perspective(glm::radians(80.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		main_shader.setMat4("projection", projection);
        main_shader.setMat4("view", view);

		area1.collision_mesh.drow();
		
		area2.collision_mesh.position = main_camera.cameraPos + (main_camera.cameraFront * glm::vec3(5));
		area2.collision_mesh.drow();

		glm::vec3 col = area1.is_aabb_colliding_with(area2);
	
		if (col.x > 0.1f && col.y > 0.1f && col.z > 0.1f) std::cout << "COLLISION!!" << std::endl;
		else std::cout << "nah" << std::endl;

		glfwSwapBuffers(window);
    	glfwPollEvents();
	}

	main_shader.kill_shader();
	glfwTerminate();

	return 0;
}
