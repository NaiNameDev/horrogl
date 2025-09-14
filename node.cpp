class Node {
public:
	Node* parant;
	std::vector<Node*> childs;

	std::string name = "Node";

	Node() {}
	
	void init_root() {
		parant = this;
	}
	void add_child(Node* new_child) {
		childs.push_back(new_child);
		new_child->parant = this;
	}
	void move_to_child(Node* new_parant) {
		new_parant->add_child(this);
		parant = new_parant;
	}
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
	std::string name = "Node3D";

	Node3D(glm::vec3 npos = glm::vec3(0,0,0)) { position = npos; }

	glm::vec3 get_global_position() {
		glm::vec3 ret = position;

		Node* tmp = this;
		while(tmp->parant != tmp) {
			if (std::is_base_of< std::decay_t<decltype(*tmp)>, Node3D>::value) {
				std::cout << tmp->name <<std::endl;
				ret += static_cast<Node3D*>(tmp->parant)->position;
			}
			tmp = tmp->parant;
		}

		return ret;
	}
};
