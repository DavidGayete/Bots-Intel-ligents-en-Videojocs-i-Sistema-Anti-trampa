#include "Entity.h"
#include "animation.h"
#include "game.h"

Image floor_image;

EntityMesh::EntityMesh() {};
EntityMesh::EntityMesh(char* texture_name, char* mesh_name) {
	//__super::Entity();
	this->texture = Texture::Get(texture_name);

	// example of loading Mesh from Mesh Manager
	this->mesh = Mesh::Get(mesh_name);

	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
}
void EntityMesh::render(float time) {
	int window_width = 800;
	int window_height = 600;

	Matrix44 m;
	//m.rotate(DEG2RAD, Vector3(0, 1, 0));
	if (shader)
	{
		//enable shader
		shader->enable();

		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", Camera::current->viewprojection_matrix);
		shader->setUniform("u_texture", this->texture);
		shader->setUniform("u_model", this->model);
		shader->setUniform("u_time", time);

		//do the draw call
		mesh->render(GL_TRIANGLES);

		//disable shader
		shader->disable();
	}

}
void EntityMesh::update() {

}



Floor::Floor() {};

Floor::Floor(int size) {
	//this->texture = Texture::Get("data/flip_image.png");
	this->texture = Texture::Get("data/MAP_FLIP.png");
	//noise = Texture::Get("data/noisy_image.png");
	//floor_image.fromTexture(this->texture);
	//printf("%d %d", floor_image.width, floor_image.height);
	//floor_image.Image::loadPNG("data/mapa_1_zombies.png",false);
	//floor_image.Image::loadPNG("data/map2.png", false);
	this->mesh = new Mesh();
	this->mesh->createPlane(size);
	this->texture->bind();
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	floor_image.Image::loadPNG("data/MAP.png", false);

};
void Floor::render_floor(float time) {
	if (shader)
	{
		//enable shader
		shader->enable();
		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", Camera::current->viewprojection_matrix);
		shader->setUniform("u_texture", texture);
		//shader->setUniform("u_noise_texture", noise);
		shader->setUniform("u_model", this->model);
		shader->setUniform("u_camera_pos", Camera::current->eye);
		shader->setUniform("u_time", time);
		//do the draw call
		mesh->render(GL_TRIANGLES);
		//disable shader
		shader->disable();
	}



};



Wall::Wall() {};
Wall::Wall(int size) {
	this->texture = Texture::Get("data/wall.png");
	this->mesh = new Mesh();
	this->mesh->createQuad(0, 6.25f, 1024, 12.5f, false);
	this->texture->bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	Matrix44 m;
	m.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
	m.setTranslation(0, 0, size);
	Game::instance->wall_model.push_back(m);
	Matrix44 m2;
	m2.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
	m2.setTranslation(0, 0, -size);
	Game::instance->wall_model.push_back(m2);
	Matrix44 m3;
	m3.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
	m3.setTranslation(-size, 0, 0);
	m3.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
	Game::instance->wall_model.push_back(m3);
	Matrix44 m4;
	m4.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
	m4.setTranslation(size, 0, 0);
	m4.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
	Game::instance->wall_model.push_back(m4);
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");


};
void Wall::render_walls(float time) {
	if (Game::instance->wall_model.size() > 0) {
		shader->enable();
		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", Camera::current->viewprojection_matrix);
		shader->setUniform("u_texture", texture);
		shader->setUniform("u_time", time);
		for (int i = 0; i < Game::instance->wall_model.size(); i++) {
			shader->setUniform("u_model", Game::instance->wall_model[i]);
			this->mesh->render(GL_TRIANGLES);
		}
		shader->disable();
	}


};




Wall2::Wall2() {


};
Wall2::Wall2(bool aa) {
	this->texture = Texture::Get("data/wall.png");
	this->mesh = new Mesh();
	this->mesh = Mesh::Get("data/export_1.obj");


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);


};
void Wall2::render_walls2(float time) {
	if (Game::instance->wall2_model.size() > 0) {
		shader->enable();
		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", Camera::current->viewprojection_matrix);
		shader->setUniform("u_texture", texture);
		shader->setUniform("u_time", time);
		for (int i = 0; i < Game::instance->wall2_model.size(); i++) {
			shader->setUniform("u_model", Game::instance->wall2_model[i]);
			this->mesh->render(GL_TRIANGLES);
		}
		shader->disable();
	}


};




GameMap::GameMap() {

};
void GameMap::create_map(char* filename) {
	FILE* f;
	f = fopen(filename, "w");


};
void GameMap::load_map(char* filename, float half_size_x, float half_size_z) {
	FILE* f;
	f = fopen(filename, "r");
	int x;
	int z;
	float angle;
	int type;
};