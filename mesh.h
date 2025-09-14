class Mesh : public Node3D {
public:
	unsigned int VBO, VAO;
	unsigned int vertices_cnt;
	float* own_vertices;
	Shader* shader;

	std::string name = "Mesh";

	Mesh() {}

	/*Mesh(float* vertices, int vertices_size, glm::vec3 npos = glm::vec3(0,0,0)) {
		shader = new_shader;
		vertices_cnt = vertices_size;
		position = npos;
		own_vertices = vertices;
	}*/
	Mesh(float* vertices, int vertices_size, Shader* new_shader, glm::vec3 npos = glm::vec3(0,0,0)) 
		: Node3D(npos) {
		shader = new_shader;
		vertices_cnt = vertices_size;
		position = npos;
		own_vertices = vertices;
	
    	glGenVertexArrays(1, &VAO);
	    glGenBuffers(1, &VBO);

	    glBindVertexArray(VAO);

	    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

    	// position attribute
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	    glEnableVertexAttribArray(0);
	    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	    glEnableVertexAttribArray(1);
	}
	void drow() {
        glBindVertexArray(VAO);
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model = glm::translate(model, position);
		shader->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, vertices_cnt);
		
	}
	void kill() {
    	glDeleteVertexArrays(1, &VAO);
    	glDeleteBuffers(1, &VBO);
		//shader->kill_shader();
	}
};
