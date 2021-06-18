#pragma once
#include "mesh.h"
#include "shader.h"

#include "texture.h"



enum captain_direction { FRONT,BACK };
class Entity {
public:
	std::string name;
	Matrix44 model;
	Entity() {

	};
	
	void render(float time);
	void update();
};


class EntityMesh : public Entity{
public:
	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	Vector4 color;
	EntityMesh();
	EntityMesh(char* texture_name, char* mesh_name);

	virtual void render(float time);
	void update();
};

class Arma : public EntityMesh {
public:
	float angle;
	Arma() {};
	Arma(char* texture_name, char* mesh_name) {
		//__super::EntityMesh(texture_name, mesh_name);
		angle = 1;
		this->texture = Texture::Get(texture_name);
		// example of loading Mesh from Mesh Manager
		this->mesh = Mesh::Get(mesh_name);
		// example of shader loading using the shaders manager

		shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	};
	
	void render(float time);

	//void render_zombie(float time);


};
class Captain : public EntityMesh {
public:
	float angle;
	Vector3 position;
	Captain();
	Captain(char* texture_name, char* mesh_name);
	Arma arma;
	void render(float time);
	void update(float speed);
	
};
class T_Players : public EntityMesh {
public:
	float angle;
	Vector3 position;
	Arma arma;
	T_Players();
	T_Players(char* texture_name, char* mesh_name);
	void render(float time);
	void update(float speed);
	
};
class CT_Players : public EntityMesh {
public:
	float angle;
	Vector3 position;
	Arma arma;
	CT_Players();
	CT_Players(char* texture_name, char* mesh_name);
	void render(float time);
	void update(float speed);
	
};


class Floor : public EntityMesh {
public:
	
	Floor() ;

	Floor(int size) ;
	void render_floor(float time) ;
};

class Wall : public EntityMesh {
public:
	Wall() ;
	Wall(int size) ;
	void render_walls(float time) ;
};






class Wall2 : public EntityMesh {
public:
	Wall2() ;
	Wall2(bool aa);
	void render_walls2(float time);
};





class GameMap {
public:
	enum object_types { TREES, ZOMBIES };
	GameMap();
	void create_map(char* filename);
	void load_map(char* filename, float half_size_x, float half_size_z);
	
};

