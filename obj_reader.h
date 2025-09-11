class obj_reader {
public:
	std::vector<float> vertices;

	void read_vertices(const char* path) {
		std::string	obj_string;
		std::ifstream obj_file;
        obj_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	
		try {
			obj_file.open(path);
			std::stringstream obj_stream;
			obj_stream << obj_file.rdbuf();
			obj_file.close();
			obj_string = obj_stream.str();
		}
        catch (std::ifstream::failure& e) {
            std::cout << "ERROR::OBJ_LOADER::CANT_READ_FILE: " << e.what() << std::endl;
        }

		std::vector<float> dots;
		std::vector<float> norms;

		std::vector<int> dpos;
		std::vector<int> npos;

		std::string dbuff = "";
		std::string nbuff = "";
		std::string fbuff = "";
		for (int i = 0; i < obj_string.length(); i++) {
			if (obj_string[i] == 'v') {
				if (obj_string[i + 1] == 'n') {
					i += 3;
					while(obj_string[i] != '\n') {
						if (obj_string[i] == ' ') {
							norms.push_back((float)std::atof(nbuff.c_str()));
							nbuff = "";
						}
						else {
							nbuff.push_back(obj_string[i]);
						}
						i++;
					}
					norms.push_back((float)std::atof(nbuff.c_str()));
					nbuff = "";
				}
				else {
					i += 2;
					while(obj_string[i] != '\n') {
						if (obj_string[i] == ' ') {
							dots.push_back((float)std::atof(dbuff.c_str()));
							dbuff = "";
						}
						else {
							dbuff.push_back(obj_string[i]);
						}
						i++;
					}
					dots.push_back((float)std::atof(dbuff.c_str()));
					dbuff = "";
				}
			}
			if (obj_string[i] == 'f') {
				i += 2;
				bool switcher = true;
				while(obj_string[i] != '\n') {
					if (obj_string[i] != ' ' && obj_string[i] != '/') {
						fbuff.push_back(obj_string[i]);
					}
					else if (fbuff != "") {
						if (switcher) {
							dpos.push_back((float)std::atoi(fbuff.c_str()));
							fbuff = "";
							switcher = !switcher;
						}
						else {
							npos.push_back((float)std::atoi(fbuff.c_str()));
							fbuff = "";
							switcher = !switcher;
						}
					}
					i++;
				}
				npos.push_back((float)std::atoi(fbuff.c_str()));
				fbuff = "";
			}
				
		}
		for (int i = 0; i < dpos.size(); i++) {
			vertices.push_back(dots[(dpos[i] - 1) * 3]);
			vertices.push_back(dots[(dpos[i] - 1) * 3 + 1]);
			vertices.push_back(dots[(dpos[i] - 1) * 3 + 2]);
			vertices.push_back(norms[(npos[i] - 1) * 3]);
			vertices.push_back(norms[(npos[i] - 1) * 3 + 1]);
			vertices.push_back(norms[(npos[i] - 1) * 3 + 2]);
		}
	}

	void clear_vertices() { 
		vertices.clear();
	}

	mesh create_mesh(Shader* shd) {
		//std::cout << vertices.size() << std::endl;
		//print_vertices();
		//exit(0);
		return mesh(&vertices[0], vertices.size() * sizeof(float), shd);
	}

	void print_vertices() {
		int cnt = 0;
		int cnt2 = 0;
		for (int i = 0; i < vertices.size(); i++) {
			std::cout << vertices[i] << " ";
			cnt++;
			cnt2++;
			if (cnt / 6 == 1) {
				std::cout << std::endl;
				cnt = 0;
			}
			if (cnt2 / (36) == 1) {
				std::cout << std::endl;
				cnt2 = 0;
			}
		}
	}
};
