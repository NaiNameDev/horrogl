enum node_type {DNODE, NODE3D, NODE2D, UINODE};
typedef struct {
	void* ptr;
	node_type type;
} node_ptr;

class Node3D;

class Node {
public:
	node_ptr parant;
	std::vector<node_ptr> childs;
	enum node_type own_type = DNODE;

	std::string name = "Node";

	Node() {}
	
	node_ptr form_node_prt() {
		return (node_ptr){this, own_type};
	}

	void init_root() {
		parant = form_node_prt();
	}
	void add_child(node_ptr child_ptr) {
		childs.push_back(child_ptr);
		static_cast<Node*>(child_ptr.ptr)->parant = form_node_prt();
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
			std::cout << static_cast<Node*>(childs[i].ptr)->name << " type:" << childs[i].type << '\n';
			static_cast<Node*>(childs[i].ptr)->show_tree_from_here(deep + 1);
		}
	}
	
};

class Node3D : public Node {
public:
	glm::vec3 position;
	std::string name = "Node3D";
	enum node_type own_type = NODE3D;

	Node3D(glm::vec3 npos = glm::vec3(0,0,0)) { position = npos; own_type = NODE3D; }
	
	node_ptr form_node_prt() {
		return (node_ptr){this, own_type};
	}

	glm::vec3 get_global_position() {
		glm::vec3 ret = position;

		node_ptr tmp = parant;
		while(static_cast<Node3D*>(tmp.ptr)->own_type == NODE3D) {
			//std::cout << static_cast<Node3D*>(tmp.ptr)->own_type << std::endl;
			ret += static_cast<Node3D*>(tmp.ptr)->position;
			tmp = static_cast<Node3D*>(tmp.ptr)->parant;
		}

		return ret;
	}
};
