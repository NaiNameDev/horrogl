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

glm::vec3 get_min(glm::vec3 v) {
	glm::vec3 ret = 	 glm::vec3(v.x, 0, 0);

	if (glm::abs(v.z) < glm::abs(v.z)) ret = glm::vec3(0, 0, v.z);
	if (glm::abs(v.y) < glm::abs(v.x)) ret = glm::vec3(0, v.y, 0);
	
	return ret;
}

#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "obj_reader.h"
#include "aabb_phys_object.cpp"
#include "rigid_body.cpp"

#define SCR_WIDTH 1440
#define SCR_HEIGHT 720

//delta
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//camera
camera main_camera;
float yaw = 90.0f, pitch = 0.0f;
bool firstMouse = true;
float lastX = 0, lastY = 0;

rigid_body rig1;

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
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {rig1. velocity.y = 5.0f;}
	
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {rig1. velocity.z = 0.0f;}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {rig1. velocity.z = 0.0f;}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {rig1. velocity.x = 0.0f;}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {rig1. velocity.x = 0.0f;}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {rig1. velocity.z = 3.0f;}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {rig1. velocity.z = -3.0f;}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {rig1. velocity.x = 3.0f;}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {rig1. velocity.x = -3.0f;}
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

	rig1 = rigid_body(area1, area1.collision_mesh.position);
	rigid_body rig2(area2, area2.collision_mesh.position - glm::vec3(0.0f, 5, 0.0f));

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

		//rig1.apply_gravity(deltaTime);
		rig1.apply_gravity(deltaTime);
		rig1.area.collision_mesh.position = rig1.position;

		main_shader.setVec3("objectColor", glm::vec3(0.2f, 1.0f, 0.9f));
		rig2.area.collision_mesh.drow();
		rig2.area.collision_mesh.position = rig2.position;
		
		glm::vec3 aabb = rig1.area.is_aabb_colliding_with(rig2.area);
		glm::vec3 maabb = get_min(aabb);

		if (glm::abs(aabb.x) > 0 && glm::abs(aabb.y) > 0 && glm::abs(aabb.z) > 0) {
			rig1.position += maabb; 
			rig1.check_floor(aabb);
		}
		rig1.move(deltaTime);
		rig1.area.collision_mesh.position = rig1.position;
		
		main_camera.cameraPos = rig1.position;
		rig1.area.collision_mesh.drow();

		//std::cout << maabb.x << ", " << maabb.y << ", " << maabb.z << std::endl;
		if (rig1.is_on_floor) std::cout << "on floor" << std::endl;

		glfwSwapBuffers(window);
    	glfwPollEvents();
	}

	main_shader.kill_shader();
	glfwTerminate();

	return 0;
}
