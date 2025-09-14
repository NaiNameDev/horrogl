#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <type_traits>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//bruh why glm does not have lerp bruh
float lerp(float a, float b, float t)
{
    return a * t + b * (1 - t);
}

glm::vec3 get_min(glm::vec3 v) {
	if (glm::abs(v.x) <= glm::abs(v.y) && glm::abs(v.x) <= glm::abs(v.z)) return glm::vec3(v.x, 0, 0);
	if (glm::abs(v.y) <= glm::abs(v.x) && glm::abs(v.y) <= glm::abs(v.z)) return glm::vec3(0, v.y, 0);
	return glm::vec3(0,0,v.z);
}

#include "shader.h"
#include "node.cpp"
#include "camera.h"
#include "mesh.h"
#include "obj_reader.h"
#include "aabb_phys_object.cpp"

#define SCR_WIDTH 1440
#define SCR_HEIGHT 720

//delta
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//camera
Camera main_camera;
float yaw = 90.0f, pitch = 0.0f;
bool firstMouse = true;
float lastX = 0, lastY = 0;

RigidBody rig1;

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

#define player_speed 5.0f
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {rig1. velocity.y = 5.0f;}
	
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {rig1. velocity.z = 0.0f;}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {rig1. velocity.z = 0.0f;}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {rig1. velocity.x = 0.0f;}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {rig1. velocity.x = 0.0f;}
	//if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASEdd) {rig1. velocity.y = 0.0f;}
	//if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE) {rig1. velocity.y = 0.0f;}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		rig1. velocity.x += main_camera.cameraFront.x * player_speed;
		rig1. velocity.z += main_camera.cameraFront.z * player_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		rig1. velocity.x += main_camera.cameraFront.x * -player_speed;
		rig1. velocity.z += main_camera.cameraFront.z * -player_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		rig1. velocity.x += glm::normalize(glm::cross(main_camera.cameraFront, main_camera.cameraUp)).x * -player_speed;
		rig1. velocity.z += glm::normalize(glm::cross(main_camera.cameraFront, main_camera.cameraUp)).z * -player_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		rig1. velocity.x += glm::normalize(glm::cross(main_camera.cameraFront, main_camera.cameraUp)).x * player_speed;
		rig1. velocity.z += glm::normalize(glm::cross(main_camera.cameraFront, main_camera.cameraUp)).z * player_speed;
	}
	//if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {rig1. velocity.y = 3.0f;}
	//if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {rig1. velocity.y = -3.0f;}
}

int main() {
	// no glfw debug zone
		Node root;
		root.init_root();
		root.set_name("root");

		Node child;
		child.set_name("child");
		child.move_to_child(&root);
		
		Node child2;
		child2.set_name("child2");
		child2.move_to_child(&root);

		Node child_of_child;
		child_of_child.set_name("child_of_child");
		child_of_child.move_to_child(&child);

		Node3D sigma(glm::vec3(1,1,1));
		sigma.set_name("3d_node");
		sigma.move_to_child(&child_of_child);
		
		Node3D sigma2(glm::vec3(1,2,1));
		sigma2.set_name("3d_node22");
		sigma2.move_to_child(&sigma);

		glm::vec3 a = sigma2.get_global_position();

		std::cout << a.x << " " << a.y << " " << a.z << std::endl;

		//root.show_tree_from_here();

		exit(0);
	//
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
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	ObjReader rdr;
	
	rdr.read_vertices("./media/obj/cube.obj");
	CollisionMesh rg1_mesh = CollisionMesh(rdr.create_mesh(&main_shader));
	
	CollisionMesh wall_mesh = CollisionMesh(rdr.create_mesh(&main_shader));
	
	rdr.clear_vertices();
	rdr.read_vertices("./media/obj/floor.obj");
	CollisionMesh rg2_mesh = CollisionMesh(rdr.create_mesh(&main_shader));

	rig1 = RigidBody();
	rig1.col_mesh = &rg1_mesh;
	
	RigidBody rig2(glm::vec3(0.0f, -3.0f, 0.0f));
	rig2.col_mesh = &rg2_mesh;

	RigidBody wall_rig(glm::vec3(0.0f, -2.0f, 2.0f));
	wall_rig.col_mesh = &wall_mesh;

	PhysicsLayer layer1;
	layer1.push_body(&rig1);
	layer1.push_body(&rig2);
	layer1.push_body(&wall_rig);

	rig1.layer = &layer1;
	rig2.layer = &layer1;
	wall_rig.layer = &layer1;

	while (!glfwWindowShouldClose(window)) {
		main_camera.update_mouse(yaw, pitch);
		processInput(window);
		
		main_shader.setVec3("objectColor", glm::vec3(0.9f, 1.0f, 0.9f));
        main_shader.setVec3("lightColor", glm::vec3(0.5f, 0.5f, 0.5f));
        main_shader.setVec3("lightPos", glm::vec3(0, 3.0f, -1));
        main_shader.setFloat("tran", 1.0f);
		main_shader.setFloat("u_time", glfwGetTime());
		main_shader.use();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		glClearColor(0,0,0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		glm::mat4 view = main_camera.get_view();
        glm::mat4 projection = glm::perspective(glm::radians(80.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		main_shader.setMat4("projection", projection);
        main_shader.setMat4("view", view);
		
		main_shader.setVec3("objectColor", glm::vec3(0.2f, 2.0f, 0.9f));
		wall_rig.velocity = glm::vec3(0, sin(glfwGetTime()), 0);
		wall_rig.move(deltaTime);
		wall_rig.col_mesh->position = wall_rig.position;
		wall_rig.col_mesh->drow();

		main_shader.setVec3("objectColor", glm::vec3(0.2f, 1.0f, 0.9f));
		rig2.col_mesh->drow();
		rig2.col_mesh->position = rig2.position;
		
		rig1.apply_gravity(deltaTime);
		rig1.move_and_colide(deltaTime);
		main_camera.cameraPos = rig1.position;
		
		//main_shader.setVec3("objectColor", glm::vec3(0.9f, 1.0f, 0.9f));
		//rig1.col_mesh->drow();

		glfwSwapBuffers(window);
    	glfwPollEvents();
	}

	main_shader.kill_shader();
	glfwTerminate();

	return 0;
}
