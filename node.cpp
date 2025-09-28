enum node_type {DNODE, NODE3D, NODE2D, UINODE};
/*typedef struct {
	void* ptr;
	node_type type;
} node_ptr;

class Node3D;
*/
class Node {
public:
	Node* parant;
	std::vector<Node*> childs;
	enum node_type own_type;

	std::string name;

	Node(std::string new_name = "Node") { name = new_name; own_type = DNODE;}
	
	/*node_ptr form_node_prt() {
		return (node_ptr){this, own_type};
	}*/

	void init_root() {
		parant = this;
	}
	void add_child(Node* child_ptr) {
		childs.push_back(child_ptr);
		child_ptr->parant = this;
	}
	//void move_to_child(node_ptr new_parant) {    //this shit doesnt work bruh
	//	static_cast<Node*>(new_parant.ptr)->add_child(form_node_prt());
	//	parant = new_parant;
	//}
	void set_name(std::string nname) {
		name = nname;
	}
	void show_tree_from_here(int deep = 0) {
		if (childs.size() == 0) { return; }

		for (int i = 0; i < childs.size(); i++) {
			for (int t = 0; t < deep; t++) std::cout << "  ";
			std::cout << childs[i]->name << '\n';
			childs[i]->show_tree_from_here(deep + 1);
		}
	}
	
};

class Node3D : public Node {
public:
	glm::vec3 position;
	std::string name;
	enum node_type own_type;

	Node3D(glm::vec3 npos = glm::vec3(0,0,0), std::string new_name = "Node3D") : Node(new_name) { position = npos; own_type = NODE3D;}

	glm::vec3 get_global_position() {
		glm::vec3 ret = position;

		Node* tmp = parant;
		while(static_cast<Node3D*>(tmp)->own_type == NODE3D && tmp->parant != tmp) {
			ret += static_cast<Node3D*>(tmp)->position;
			tmp = static_cast<Node3D*>(tmp)->parant;
		}

		return ret;
	}
};
class Node2D : public Node {
public:
	glm::vec2 position;
	std::string name;
	enum node_type own_type;

	Node2D(glm::vec2 npos = glm::vec2(0,0), std::string new_name = "Node2D") : Node(new_name) { position = npos; own_type = NODE2D;}

	glm::vec2 get_global_position() {
		glm::vec2 ret = position;

		Node* tmp = parant;
		while(tmp->own_type == NODE2D) {
			ret += static_cast<Node2D*>(tmp)->position;
			tmp = static_cast<Node2D*>(tmp)->parant;
		}

		return ret;
	}
};
