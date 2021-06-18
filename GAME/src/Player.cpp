#include "Entity.h"
#include "game.h"
#include "utils.h"

#include "texture.h"
#include "fbo.h"

#include "input.h"
#include "animation.h"

#include <cmath>


bool moving1;
Animation* anim11;
float time_aux;
Shader* animacion1;


bool zona_plante_a1(Matrix44 model_player) {
	float pos_x = model_player.getTranslation().x;
	float pos_z = model_player.getTranslation().z;
	if (pos_x > Game::instance->zona_plante_a[0].x&& pos_z< Game::instance->zona_plante_a[0].z && pos_x>Game::instance->zona_plante_a[1].x&& pos_z > Game::instance->zona_plante_a[1].z
		&& pos_x < Game::instance->zona_plante_a[2].x && pos_z < Game::instance->zona_plante_a[2].z && pos_x<Game::instance->zona_plante_a[3].x && pos_z>Game::instance->zona_plante_a[3].z) {

		return true;


	}
	else {
		return false;
	}



}

bool zona_plante_b1(Matrix44 model_player) {
	float pos_x = model_player.getTranslation().x;
	float pos_z = model_player.getTranslation().z;
	if (pos_x > Game::instance->zona_plante_b[0].x&& pos_z< Game::instance->zona_plante_b[0].z && pos_x>Game::instance->zona_plante_b[1].x&& pos_z > Game::instance->zona_plante_b[1].z
		&& pos_x < Game::instance->zona_plante_b[2].x && pos_z < Game::instance->zona_plante_b[2].z && pos_x<Game::instance->zona_plante_b[3].x && pos_z>Game::instance->zona_plante_b[3].z) {

		return true;

	}
	else {
		return false;
	}




}

void renderQuieto1(Matrix44 m, Mesh* mesh, Texture* text) {
	Camera* camera = Camera::current;
	Vector3 position = m * mesh->box.center;

	BoundingBox global = transformBoundingBox(m, mesh->box);
	if (!camera->testBoxInFrustum(global.center, global.halfsize)) return;
	glPointSize(5);

	animacion1 = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");

	animacion1->enable();

	animacion1->setUniform("u_color", Vector4(1, 1, 1, 1));
	animacion1->setUniform("u_viewprojection", camera->viewprojection_matrix);

	if (text) {
		animacion1->setUniform("u_texture", text);


	}

	animacion1->setUniform("u_model", m);
	animacion1->setUniform("u_texture_tiling", 1.0f);
	animacion1->setUniform("u_time", Game::instance->time);
	animacion1->setUniform("u_camera_pos", camera->eye);
	animacion1->setUniform("u_camera_pos", camera->eye);
	//mesh->renderInstanced(GL_TRIANGLES, &zombies_in_frustum[0], zombies_in_frustum.size());
	//mesh->renderInstanced(GL_TRIANGLES, &zombies_in_frustum[0], zombies_in_frustum.size());
	//Shader::current = animacion;
	mesh->render(GL_TRIANGLES);
	animacion1->disable();
}

void shooting_player(Matrix44 orig, Matrix44 dest, int index) {
	//calculariamos la distancia
	
	Game::instance->vida_t[index] = Game::instance->vida_t[index] - 33;
	/*std::cout << "vida enemigo" << std::endl;
	std::cout << index << std::endl;
	std::cout << Game::instance->vida_t[index] << std::endl;*/
	if (Game::instance->vida_t[index] <= 0) {
		Game::instance->terrorists_alive[index] = false;
	}
	


}

bool enemigo_escondido_player(Matrix44 model_player, Matrix44 model_enemy, float ang) {
	for (int i = 0; i < Game::instance->wall2_model.size(); i++) {
		Vector3 target_direction = (Game::instance->wall2_model[i].getTranslation() - (model_player.getTranslation()));
		float ang_aux = dot(model_player.frontVector(), target_direction.normalize());
		if (ang + 0.05 >= ang_aux && ang - 0.05 <= ang_aux) {
			float dist1 = sqrt(pow(model_player.getTranslation().x - model_enemy.getTranslation().x, 2) + pow(model_player.getTranslation().z - model_enemy.getTranslation().z, 2));

			float dist2 = sqrt(pow(model_player.getTranslation().x - Game::instance->wall2_model[i].getTranslation().x, 2) + pow(model_player.getTranslation().z - Game::instance->wall2_model[i].getTranslation().z, 2));

			if (dist2 < dist1) {
				return false;
			}

		}

	}

	return true;
}

void renderAnimated1(Matrix44 m, Mesh* mesh, Texture* text, Skeleton* sk) {
	Camera* camera = Camera::current;
	Vector3 position = m * mesh->box.center;
	Shader* animacion;
	BoundingBox global = transformBoundingBox(m, mesh->box);
	if (!camera->testBoxInFrustum(global.center, global.halfsize)) return;
	glPointSize(5);

	animacion = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");

	animacion->enable();

	animacion->setUniform("u_color", Vector4(1, 1, 1, 1));
	animacion->setUniform("u_viewprojection", camera->viewprojection_matrix);

	if (text) {
		animacion->setUniform("u_texture", text);


	}

	animacion->setUniform("u_model", m);
	animacion->setUniform("u_texture_tiling", 1.0f);
	animacion->setUniform("u_time", Game::instance->time);
	animacion->setUniform("u_camera_pos", camera->eye);
	animacion->setUniform("u_camera_pos", camera->eye);
	//mesh->renderInstanced(GL_TRIANGLES, &zombies_in_frustum[0], zombies_in_frustum.size());
	//mesh->renderInstanced(GL_TRIANGLES, &zombies_in_frustum[0], zombies_in_frustum.size());
	//Shader::current = animacion;
	mesh->renderAnimated(GL_TRIANGLES, sk);



	animacion->disable();


}

void Arma::render(float time) {

	//if (shader && Game::instance->antiterrorists_alive[0])
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


Captain::Captain() {

}


Captain::Captain(char* texture_name, char* mesh_name) {
	//__super::EntityMesh(texture_name, mesh_name);
	angle = 0;
	this->texture = Texture::Get(texture_name);
	// example of loading Mesh from Mesh Manager
	this->mesh = Mesh::Get(mesh_name);
	this->model.setIdentity();
	//this->model.setScale(0.2f, 0.2f, 0.2f);
	this->model.setScale(2.0f, 2.0f,2.0f);
	this->position = Vector3(0, 0, 0);
	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");




}
void Captain::render(float time) {

	//if (shader && Game::instance->antiterrorists_alive[0])
	if(shader)
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
		/*shader->enable();
		glEnable(GL_DEPTH_TEST);
		anim11 = Animation::Get("data/people/terror/move/animations_walking.skanim");
		if (moving1 ==false) {
			//std::cout << "aaaaaaaaaaaaaaaa" << std::endl;
			anim11->assignTime(0);
		}
		else {
			//std::cout << "bbbbbbbbbbbbbbbbbbbbbbbbb" << std::endl;
			anim11->assignTime(0);
		}
		renderAnimated1(this->model, Mesh::Get("data/people/terror/move/character.mesh"), this->texture, &anim11->skeleton);

		Matrix44 hand = anim11->skeleton.getBoneMatrix("mixamorig_RightHand", false);

		hand = hand * Game::instance->antiterrorists[0];
		hand.rotate(45.0, Vector3(0, 0, 1));
		hand.rotate(180, Vector3(1, 0, 0));
		hand.scale(0.03, 0.03, 0.03);
		this->arma.model.scale(0.2, 0.2, 0.2);

		renderQuieto1(hand, this->arma.mesh, this->arma.texture);
		shader->disable();*/

	}


}
void Captain::update(float speed) {
	Matrix44 m;
	m = this->model;
	/*std::cout<<"posicion"<<std::endl;
	std::cout << m.getTranslation().x << std::endl;
	std::cout << m.getTranslation().z << std::endl;*/
	//mouse input to rotate the cam
	float ang_aux = Game::instance->ang;
	if (Game::instance->mouse_locked) //is left button pressed?
	{
		ang_aux -= Input::mouse_delta.x * 0.005f;
		m.rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		//camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		//camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
	}
	moving1 = false;
	//async input to move the camera around
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) {


		//target_pos = captain.model.getTranslation();//camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		m.translate(0, 0, speed*0.5);
		moving1 = true;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
		m.translate(0, 0, -speed*0.5);
		moving1 = true;

	}//camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
		m.translate(speed*0.5, 0, 0);
		moving1 = true;

	}//camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
		m.translate(-speed*0.5 , 0, 0);
		moving1 = true;

	}//camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);

	
	if (Input::isKeyPressed(SDL_SCANCODE_K)) {
		Game::instance->tiempo_ronda = 2;

	}

	if (Input::isKeyPressed(SDL_SCANCODE_M)) {
		Game::instance->antiterrorists_shoot[0] = true;
		Game::instance->antiterrorists_alive[0] = true;

	}
	if (Input::wasKeyPressed(SDL_SCANCODE_C)) {
		if (Game::instance->cheats) {
			Game::instance->cheats = false;
		}
		else {
			Game::instance->cheats = true;
		}


	}

	if (Input::isKeyPressed(SDL_SCANCODE_E)) {
		if (Game::instance->bomba_planted == true) {
			bool zona_A = zona_plante_a1(Game::instance->antiterrorists[0]);
			if (zona_A && Game::instance->a_planted == true && Game::instance->antiterrorists_alive[0]) {
				std::cout << "la lian aqui" << std::endl;
				Game::instance->bomba_planted = false;
				Game::instance->tiempo_ronda = 0;

			}
			bool zona_B = zona_plante_b1(Game::instance->antiterrorists[0]);
			if (zona_B && Game::instance->antiterrorists_alive[0] && Game::instance->b_planted == true) {
				std::cout << "la lian aqui   1" << std::endl;
				Game::instance->bomba_planted = false;
				Game::instance->tiempo_ronda = 0;
			}

			if (abs(Game::instance->antiterrorists[0].getTranslation().x - Game::instance->bomb_position.x) < 5 && abs(Game::instance->antiterrorists[0].getTranslation().z - Game::instance->bomb_position.z) < 5 && Game::instance->antiterrorists_alive[0]) {
				std::cout << "la lian aqui 222" << std::endl;
				Game::instance->bomba_planted = false;
				Game::instance->tiempo_ronda = 0;

			}


		}
	}


	

	/*if (Game::instance->antiterrorists_shoot_disp[0] == true) {
		std::cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" << std::endl; std::cout << "aaaaaaaaaaaaaaaaaaaaaaaaaa" << std::endl; std::cout << "enemigo_visto" << std::endl;
		std::cout << Game::instance->antiterrorists_shoot_disp[0] << std::endl;
	}*/
	
	if (Game::instance->antiterrorists_shoot_disp[0] == false && Game::instance->antiterrorists_shoot_time[0] <= Game::instance->tiempo_total) {
		Game::instance->antiterrorists_shoot_disp[0] = true;
		Game::instance->antiterrorists_shoot[0] = false;

		//std::cout << "recargaaaaa" << std::endl;

	}

	if ((Input::mouse_state & SDL_BUTTON_LEFT) && Game::instance->antiterrorists_shoot_disp[0] == true) //is left button pressed?
	{
		//printf("pium pium\n");
		Game::instance->antiterrorists_shoot[0] = true;
		Game::instance->antiterrorists_shoot_disp[0] = false;
		Game::instance->antiterrorists_shoot_time[0] = Game::instance->tiempo_total + 0.4;
		for (int j = 0; j < 5; j++) {
			Vector3 target_direction_2 = (Game::instance->terrorists[j].getTranslation() - (Game::instance->antiterrorists[0].getTranslation()));
			float ang = dot(Game::instance->antiterrorists[0].frontVector(), target_direction_2.normalize());
			

			if (ang > 0.99) {
				bool enemigo_visto = enemigo_escondido_player(Game::instance->antiterrorists[0], Game::instance->terrorists[j], ang);

				/*std::cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" << std::endl;std::cout << "aaaaaaaaaaaaaaaaaaaaaaaaaa" << std::endl;std::cout << "enemigo_visto" << std::endl;
				std::cout << enemigo_visto << std::endl;
				std::cout << j << std::endl;
				std::cout << ang << std::endl;*/
				if (enemigo_visto && Game::instance->terrorists_alive[j]) {
					//printf("pium pium");
					//Game::instance->antiterrorists_shoot_disp[0] = false;
					//std::cout << j << std::endl;
					shooting_player(Game::instance->antiterrorists[0], Game::instance->terrorists[j], j);
					break;
				}
			}
		}


	}


	bool posible_mov = true;
	Vector3 character_center = m.getTranslation() + Vector3(0, 1, 0);
	Vector3 coll;
	Vector3 collnorm;
	for (int i = 0; i < Game::instance->wall2_model.size(); i++) {
		if ((this->mesh->testSphereCollision(Game::instance->wall2_model[i], character_center, 1, coll, collnorm)) == true) {
			posible_mov = false;
			break;


		}



	}
	if (m.getTranslation().x > 128 || m.getTranslation().x < -128 || m.getTranslation().z>128 || m.getTranslation().z < -128) {
		posible_mov = false;


	}


	if (posible_mov) {
		this->model = m;
		Game::instance->antiterrorists[0] = m;
		Game::instance->ang = ang_aux;

	}
	//this->model = m;
	//Game::instance->antiterrorists[0] = m;
	
	Vector3 front(0, 0, -1);
	Vector3 eye = Game::instance->antiterrorists[0] * Vector3(0,1.75,0.3);
	
	Matrix44 R;
	R.setRotation(Game::instance->ang, Vector3(0, 1, 0));
	front = R.rotateVector(front);
	Vector3 center =eye +front;
	Vector3 up = Vector3(0.f, 1.f, 0.f);

	//Vector3 center =  this->model * Vector3();

	arma.model = this->model;
	arma.model.translate(0, 0.5, 0.1);

	arma.model.scale(0.2, 0.2, 0.2);
	

	Game::instance->camera->lookAt(eye, center, up);

	

	/*Vector3 eye = this->model * Vector3(0, 4, -7);
	Vector3 center = this->model * Vector3();
	Vector3 up = Vector3(0.f, 1.f, 0.f);
	Game::instance->camera->lookAt(eye, center, Vector3(0.f, 1.f, 0.f));*/
	bool aaa = false;
	
	//std::cout <<"aaaa" << std::endl;
	/*for (int i = 0; i < Game::instance->wall2_model.size(); i++) {
		Vector3 target_direction_3 = (Game::instance->wall2_model[i].getTranslation() - (this->model.getTranslation()));
		float ang2 = dot(this->model.frontVector(), target_direction_3.normalize());
		
		Vector3 target_direction_4 = (Game::instance->terrorists[0].getTranslation() - this->model.getTranslation());
		float ang3 = dot(this->model.frontVector(), target_direction_4.normalize());
		//if (ang3 == ang2 && ang2==0) {
		
		if (ang2+0.01 >= ang3 && ang2 - 0.01 <= ang3) {
			aaa = true;
			//std::cout << "escondido" << std::endl;
			std::cout << "ang222" << std::endl;
			std::cout << ang2 << std::endl;
			std::cout << "aaaaaaaaaaaaaaang3" << std::endl; std::cout << ang3 << std::endl;
			std::cout << ang3 << std::endl;
			

		}
		
		

	}*/

	/*Vector3 target_direction_4 = (Game::instance->terrorists[0].getTranslation() - this->model.getTranslation());
		float ang3 = dot(this->model.frontVector(), target_direction_4.normalize());

		std::cout << "ang222" << std::endl;
		std::cout << ang3 << std::endl;*/

	/*if (aaa) {
		std::cout << "escondido" << std::endl;
	}*/

	

}
