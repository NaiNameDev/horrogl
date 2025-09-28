#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <iostream>
#include <vector>
#include <map>
#include <type_traits>

//for 2D
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
//fonts
#include <ft2build.h>
#include FT_FREETYPE_H

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

#include "shader.cpp"
#include "node.cpp"
#include "camera.cpp"
#include "mesh.cpp"
#include "obj_reader.cpp"
#include "aabb_phys_object.cpp"

#define SCR_WIDTH 1440
#define SCR_HEIGHT 720

#include "sprite.cpp"
#include "text_render.cpp"

//delta
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//camera
Camera main_camera;
float yaw = 90.0f, pitch = 0.0f;
bool firstMouse = true;
float lastX = 0, lastY = 0;

RigidBody player;

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
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {player. velocity.y = 5.0f;}
	
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {player. velocity.z = 0.0f;}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {player. velocity.z = 0.0f;}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {player. velocity.x = 0.0f;}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {player. velocity.x = 0.0f;}
	//if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASEdd) {player. velocity.y = 0.0f;}
	//if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE) {player. velocity.y = 0.0f;}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		player. velocity.x = main_camera.camera_front.x * player_speed;
		player. velocity.z = main_camera.camera_front.z * player_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		player. velocity.x = main_camera.camera_front.x * -player_speed;
		player. velocity.z = main_camera.camera_front.z * -player_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		player. velocity.x += glm::normalize(glm::cross(main_camera.camera_front, main_camera.camera_up)).x * -player_speed;
		player. velocity.z += glm::normalize(glm::cross(main_camera.camera_front, main_camera.camera_up)).z * -player_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		player. velocity.x += glm::normalize(glm::cross(main_camera.camera_front, main_camera.camera_up)).x * player_speed;
		player. velocity.z += glm::normalize(glm::cross(main_camera.camera_front, main_camera.camera_up)).z * player_speed;
	}
	//if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {player. velocity.y = 3.0f;}
	//if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {player. velocity.y = -3.0f;}
}

int main() {
	/* no glfw debug zone
		Node root("root");
		root.init_root();

		Node3D sigma(glm::vec3(3,3,3), "sigma1");
		root.add_child(&sigma);
		
		Node3D sigma2(glm::vec3(1,2,1), "sigma2");
		sigma.add_child(&sigma2);

		Node3D sigma3(glm::vec3(1,2,1), "sigma3");
		sigma2.add_child(&sigma3);

		glm::vec3 a = sigma3.get_global_position();

		std::cout << a.x << " " << a.y << " " << a.z << std::endl;

		root.show_tree_from_here();

		exit(0);
	*/
	
	stbi_set_flip_vertically_on_load(true);

	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "horrorgl", NULL, NULL);
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
	Shader sprite_shader("vimage_shader.glsl", "fimage_shader.glsl");

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

	player = RigidBody(glm::vec3(0,0,0), "Player");
	player.col_mesh = &rg1_mesh;
	player.add_child(&main_camera);
	
	StaticBody floorbd(glm::vec3(0.0f, -3.0f, 0.0f), "Floor");
	floorbd.col_mesh = &rg2_mesh;

	RigidBody wall_rig(glm::vec3(0.0f, -2.0f, 2.0f), "Cube");
	wall_rig.col_mesh = &wall_mesh;

	PhysicsLayer layer1;
	layer1.push_body(&player);
	layer1.push_body(&floorbd);
	layer1.push_body(&wall_rig);
	
	player.push_layer(&layer1);
	floorbd.push_layer(&layer1);
	wall_rig.push_layer(&layer1);

	//Sprite spr("media/sprites/test_image.png", &sprite_shader);

	//tree start here
	Node root;
	root.init_root();
	root.set_name("root");

	root.add_child(&player);
	root.add_child(&floorbd);
	root.add_child(&wall_rig);

	player.add_child(&rg1_mesh);
	floorbd.add_child(&rg2_mesh);
	wall_rig.add_child(&wall_mesh);

	root.show_tree_from_here();
	
	Shader text_shader("vtext_shader.glsl", "ftext_shader.glsl");
	Label hoi("media/open_sans/static/OpenSans-Italic.ttf", &text_shader, glm::vec2(100, 100));

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
		wall_rig.velocity = glm::vec3(0, sin(glfwGetTime()), sin(glfwGetTime()));
		wall_rig.move(deltaTime);
		wall_rig.col_mesh->drow();

		main_shader.setVec3("objectColor", glm::vec3(0.2f, 1.0f, 0.9f));
		floorbd.col_mesh->drow();
		
		player.apply_gravity(deltaTime);
		player.move_and_colide(deltaTime);
		
		//main_shader.setVec3("objectColor", glm::vec3(0.9f, 1.0f, 0.9f));
		//player.col_mesh->drow();
		//if (player.is_on_floor) std::cout << "sigma " << std::endl;
		//else std::cout << "not sigma :(" << std::endl;

		//2D zone
		glDisable(GL_DEPTH_TEST);
 		glDisable(GL_CULL_FACE);
		
		hoi.drow("HELLO WORLD!! SIGMA");

		//glBindTexture(GL_TEXTURE_2D, spr.texture);
		//sprite_shader.use();
		//spr.drow();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		//2D end
		main_shader.use();

		glfwSwapBuffers(window);
    	glfwPollEvents();
	}

	main_shader.kill_shader();
	glfwTerminate();

	return 0;
}
