#include "Entity.h"
#include "game.h"

#include "IA.h"

#include "texture.h"
#include "fbo.h"

#include "input.h"
#include "animation.h"
Animation* anim1;
extern Mesh* machete_mesh;
extern Texture* machete_texture;

#include <cmath>
bool moving=false;

void renderQuieto(Matrix44 m, Mesh* mesh, Texture* text) {
	Shader* animacion;
	Camera* camera = Camera::current;
	Vector3 position = m * mesh->box.center;

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
	mesh->render(GL_TRIANGLES);
	animacion->disable();
}
// bool t==true --> ataca el terror
void shooting(Matrix44 orig, Matrix44 dest,int index,bool t) {
	//calculariamos la distancia
	float dist = sqrt(pow(orig.getTranslation().x - dest.getTranslation().x, 2) + pow(orig.getTranslation().z - dest.getTranslation().z, 2));
	int v1 = rand() % 10;
	bool acierto = false;
	if (dist < 10) {
		//90%
		if (v1 < 9) {
			acierto = true;
		}
	}
	else if (dist < 25) {
		//80
		if (v1 < 8) {
			acierto = true;
		}
	}
	else if (dist < 50) {
		//70
		if (v1 < 7) {
			acierto = true;
		}
	}
	else {
		//60 
		if (v1 < 6) {
			acierto = true;
		}

	}

		
	if (acierto) {
		if (t) {
			//std::cout << "yayayayayayayayayaya" << std::endl;
			//std::cout << "yayayayayayayayayayaya" << std::endl;
			Game::instance->vida_ct[index] = Game::instance->vida_ct[index] - 33;
			if (Game::instance->vida_ct[index] <= 0) {
				Game::instance->antiterrorists_alive[index] = false;
			}
		}
		else {
			//std::cout << "heeeeeeeeeeeeeeeyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy" << std::endl;
			//std::cout << "heeeeeeeeeeeeeeeyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy" << std::endl;
			//std::cout << index << std::endl;
			Game::instance->vida_t[index] = Game::instance->vida_t[index] - 33;
			if (Game::instance->vida_t[index] <= 0) {
				Game::instance->terrorists_alive[index] = false;
			}
		}
	}
	


}

bool esta_en_zona(Vector3 posicion, std::vector<Vector3> zona) {
	/*std::cout << "estamos" << std::endl;
	std::cout << posicion.x << std::endl;
	std::cout << posicion.z << std::endl;
	std::cout << zona[0].x << std::endl;
	std::cout << zona[0].z << std::endl;*/
	if (posicion.x > zona[0].x&& posicion.z > zona[0].z) {
		/*std::cout << "estamos2" << std::endl;
		std::cout << posicion.x << std::endl;
		std::cout << posicion.z << std::endl;
		std::cout << zona[1].x << std::endl;
		std::cout << zona[1].z << std::endl;*/
		if (posicion.x > zona[1].x&& posicion.z < zona[1].z) {
			/*std::cout << "estamos3" << std::endl;
			std::cout << posicion.x << std::endl;
			std::cout << posicion.z << std::endl;
			std::cout << zona[2].x << std::endl;
			std::cout << zona[2].z << std::endl;*/
			if (posicion.x < zona[2].x&& posicion.z > zona[2].z) {
				if (posicion.x < zona[3].x && posicion.z < zona[3].z) {
					return true;
				}

			}
		}

	}

	return false;
}


int zona_actual(Vector3 posicion) {
	if (esta_en_zona(posicion, Game::instance->zonas[0])) {
		return 0;

	}
	else if (esta_en_zona(posicion, Game::instance->zonas[1])) {

		return 1;
	}
	else {
		return 2;

	}


}

void renderAnimated(Matrix44 m, Mesh* mesh, Texture* text, Skeleton* sk) {
	Camera* camera = Camera::current;
	Vector3 position = m * mesh->box.center;
	Shader* animacion;
	BoundingBox global = transformBoundingBox(m, mesh->box);
	if (!camera->testBoxInFrustum(global.center, global.halfsize)) return;
	glPointSize(5);

	glEnable(GL_ALWAYS);
	
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
	glDisable(GL_ALWAYS);

}

bool zona_plante_a(Matrix44 model_player){
	float pos_x=model_player.getTranslation().x;
	float pos_z=model_player.getTranslation().z;
	if(pos_x> Game::instance->zona_plante_a[0].x && pos_z< Game::instance->zona_plante_a[0].z && pos_x>Game::instance->zona_plante_a[1].x && pos_z>Game::instance->zona_plante_a[1].z
	&& pos_x<Game::instance->zona_plante_a[2].x && pos_z<Game::instance->zona_plante_a[2].z && pos_x<Game::instance->zona_plante_a[3].x && pos_z>Game::instance->zona_plante_a[3].z ) {
		
		return true;
	
	
	}
	else{
		return false;
	}



}

bool zona_plante_b(Matrix44 model_player){
	float pos_x=model_player.getTranslation().x;
	float pos_z=model_player.getTranslation().z;
	if (pos_x > Game::instance->zona_plante_b[0].x&& pos_z< Game::instance->zona_plante_b[0].z && pos_x>Game::instance->zona_plante_b[1].x&& pos_z > Game::instance->zona_plante_b[1].z
		&& pos_x < Game::instance->zona_plante_b[2].x && pos_z < Game::instance->zona_plante_b[2].z && pos_x<Game::instance->zona_plante_b[3].x && pos_z>Game::instance->zona_plante_b[3].z) {
		
		return true;
	
	}
	else{
		return false;
	}




}
// terror --> true
bool enemigo_escondido(Matrix44 model_player, Matrix44 model_enemy, float ang,bool bando ){
				if (bando == false) {
					//std::cout << "entra en el if" << std::endl;
					}
					
					for(int i=0;i<Game::instance->wall2_model.size(); i++ ){
						Vector3 target_direction = (Game::instance->wall2_model[i].getTranslation() - (model_player.getTranslation()));
						float ang_aux =dot(model_player.frontVector(), target_direction.normalize());
						
						if (bando) {
							if (ang + 0.1 >= ang_aux && ang - 0.1 <= ang_aux) {
								float dist1 = sqrt(pow(model_player.getTranslation().x - model_enemy.getTranslation().x, 2) + pow(model_player.getTranslation().z - model_enemy.getTranslation().z, 2));

								float dist2 = sqrt(pow(model_player.getTranslation().x - Game::instance->wall2_model[i].getTranslation().x, 2) + pow(model_player.getTranslation().z - Game::instance->wall2_model[i].getTranslation().z, 2));

								if (dist2 < dist1) {
									return false;
								}

							}
						}
						else {
							
							//std::cout << enemigo_visto << std::endl;
							if (ang + 0.15 >= ang_aux && ang - 0.15 <= ang_aux) {
								//std::cout << "entra en el if" << std::endl;
								float dist1 = sqrt(pow(model_player.getTranslation().x - model_enemy.getTranslation().x, 2) + pow(model_player.getTranslation().z - model_enemy.getTranslation().z, 2));

								float dist2 = sqrt(pow(model_player.getTranslation().x - Game::instance->wall2_model[i].getTranslation().x, 2) + pow(model_player.getTranslation().z - Game::instance->wall2_model[i].getTranslation().z, 2));

								if (dist2 < dist1) {
									/*std::cout << "no se veeeee" << std::endl;
									std::cout << "ang" << std::endl;
									std::cout << ang << std::endl;
									std::cout << "ang_aux" << std::endl;
									std::cout << ang_aux << std::endl;
									std::cout << "dist" << std::endl;
									std::cout << dist1 << std::endl;
									std::cout << dist2 << std::endl;*/


									return false;
								}

							}
						}
						
					
					}
				
					return true;
				}


T_Players::T_Players() {

}


T_Players::T_Players(char* texture_name, char* mesh_name) {
	//__super::EntityMesh(texture_name, mesh_name);
	angle = 0;
	this->texture = Texture::Get(texture_name);
	// example of loading Mesh from Mesh Manager
	this->mesh = Mesh::Get(mesh_name);
	this->model.setIdentity();
	//this->model.setScale(25.0f, 25.0f, 25.0f);
	this->position = Vector3(0, 0, 0);
	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

}
void T_Players::render(float time) {

	/*if (shader)
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
	}*/
	/*if (Game::instance->terrorists.size() > 0) {
		shader->enable();
		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", Camera::current->viewprojection_matrix);
		shader->setUniform("u_texture", this->texture);
		shader->setUniform("u_model", this->model);
		shader->setUniform("u_time", time);

		//empieza en el 1 pq el 0 es mi player
		for (int i = 0; i < Game::instance->terrorists.size(); i++) {
			if(Game::instance->terrorists_alive[i]==true){
				shader->setUniform("u_model", Game::instance->terrorists[i]);
				this->mesh->render(GL_TRIANGLES);
			}
		}
		
		shader->disable();
	}*/



	for (int i = 0; i < Game::instance->terrorists.size(); i++) {
		if (Game::instance->terrorists_alive[i]) {
			shader->enable();
			glEnable(GL_DEPTH_TEST);
			anim1 = Animation::Get("data/people/terror/move/animations_walking.skanim");
			if (moving == false) {
				//std::cout << "aaaaaaaaaaaaaaaa" << std::endl;
				anim1->assignTime(0);
			}
			else {
				//std::cout << "bbbbbbbbbbbbbbbbbbbbbbbbb" << std::endl;
				anim1->assignTime(time);
			}
			//this->texture == Texture::Get("data/people/captain.tga");
			//renderAnimated(Game::instance->terrorists[i], Mesh::Get("data/people/terror/move/character.mesh"), Texture::Get("data/people/captain.tga"), &anim1->skeleton);
			
			if (punto_visible(Game::instance->terrorists[i], Game::instance->antiterrorists[0].getTranslation()) || Game::instance->cheats==false) {

				this->texture == Texture::Get("data/people/captain.tga");
				renderAnimated(Game::instance->terrorists[i], Mesh::Get("data/people/terror/move/character.mesh"), Texture::Get("data/people/captain.tga"), &anim1->skeleton);

			}
			else {
				glDisable(GL_DEPTH_TEST);
				this->texture == Texture::Get("data/people/captain_2.png");
				renderAnimated(Game::instance->terrorists[i], Mesh::Get("data/people/terror/move/character.mesh"), Texture::Get("data/people/captain_2.png"), &anim1->skeleton);
				glEnable(GL_DEPTH_TEST);

			}
			
			
			Matrix44 hand = anim1->skeleton.getBoneMatrix("mixamorig_LeftHand", false);

			hand = hand * Game::instance->terrorists[i];
			//hand.rotate(180.0, Vector3(1, 0, 0));
			hand.rotate(45.0 * DEG2RAD, Vector3(0, -1, 1));
			hand.rotate(45.0 * DEG2RAD, Vector3(0, 0, 1));
			
			hand.scale(0.03, 0.03, 0.03);
			this->arma.model.scale(0.2, 0.2, 0.2);
			//hand.translate(0, 8, 0);
			renderQuieto(hand, this->arma.mesh, this->arma.texture);
			shader->disable();
		}
		
		
		
	}

}



void T_Players::update(float speed) {
	if (Game::instance->empezamos) {
	
		for (int i=0; i< 5;i++){
		Vector3 posicionament_aux=Game::instance->terrorists[i].getTranslation();
		
		/*std::cout << "tiempoooo" << std::endl;
		std::cout << Game::instance->tiempo_ronda << std::endl;
		std::cout << "vidaaa" << std::endl;
		std::cout << Game::instance->vida_t[i] << std::endl;*/

		/*std::cout << Game::instance->terrorists_move_time[i] << std::endl;*/
		 
		
		
		if(Game::instance->terrorists_alive[i])  {
			Matrix44 m;
			m = Game::instance->terrorists[i];
			//provisional
			//Vector3 destino= Game::instance->primera_terror[i];
			
			/*std::cout << "DESTINOOOOOOO" << std::endl;
			std::cout << i << std::endl;
			std::cout << destino.x << std::endl;
			std::cout << destino.z << std::endl;
			std::cout << Game::instance->primera_terror[i].x << std::endl;
			std::cout << Game::instance->primera_terror[i].z << std::endl;
			std::cout << m.getTranslation().x << std::endl;
			std::cout << m.getTranslation().z << std::endl;
			std::cout << abs(m.getTranslation().x - Game::instance->primera_terror[i].x) << std::endl;
			std::cout << abs(m.getTranslation().z - Game::instance->primera_terror[i].z) << std::endl;*/

			
			int enemigos_vistos_total = 0;
			std::vector<int> enemigos_vistos_list;
			for (int j = 0; j < 5; j++) {
				Vector3 target_direction_2 = (Game::instance->antiterrorists[j].getTranslation() - (Game::instance->terrorists[i].getTranslation()));
				float ang = dot(Game::instance->terrorists[i].frontVector(), target_direction_2.normalize());
				/*if ((i == 3 || i == 4) && j == 0) {
					std::cout << "angaaaaaaaaaasssssssssssssssaaaaaaaaaa" << std::endl;
					std::cout << ang << std::endl;
				}*/

				if (ang > 0.8) {
					bool enemigo_visto = enemigo_escondido(Game::instance->terrorists[i], Game::instance->antiterrorists[j], ang,true);
					
					/*std::cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" << std::endl;std::cout << "aaaaaaaaaaaaaaaaaaaaaaaaaa" << std::endl;std::cout << "enemigo_visto" << std::endl;
					std::cout << enemigo_visto << std::endl;
					std::cout << j << std::endl;
					std::cout << ang << std::endl;*/
					if (enemigo_visto && Game::instance->antiterrorists_alive[j]) {
						//printf("lo hemos visto");
						//std::cout << j << std::endl;
						enemigos_vistos_list.push_back(j);
						enemigos_vistos_total++;

					}
				}
			}

			//std::cout << "enemigo disparando" << std::endl;
			//std::cout << Game::instance->terrorists_shoot[i] << std::endl;
			


			if (Game::instance->terrorists_shoot_disp[i] == false && Game::instance->terrorists_shoot_time[i] <= Game::instance->tiempo_total) {
				Game::instance->terrorists_shoot_disp[i] = true;
				//std::cout << "recargaaaaa" << std::endl;

			}
			



			Vector3 source = m.getTranslation();
			//std::cout << source.x << std::endl;
			//std::cout << source.z << std::endl;
			Vector3 destino = Game::instance->first_auxiliar_points[i][Game::instance->first_auxiliar_points[i].size() - 1];

			std::vector<int> num_disparando;
			for (int j = 0; j < 5; j++) {
				if (Game::instance->antiterrorists_shoot[j] && Game::instance->antiterrorists_alive[j]) {
					
					Vector3 target_direction_2 = (Game::instance->antiterrorists[j].getTranslation() - (Game::instance->terrorists[i].getTranslation()));
					float ang = dot(Game::instance->terrorists[i].frontVector(), target_direction_2.normalize());
						

					
					bool enemigo_visto = enemigo_escondido(Game::instance->terrorists[i], Game::instance->antiterrorists[j], ang,true);

					if (enemigo_visto && Game::instance->antiterrorists_alive[j]) {
								
						num_disparando.push_back(j);

					}
		
				}

			}

			if (enemigos_vistos_total > 1 && Game::instance->terrorists_shoot_disp[i] == true && Game::instance->bomba_planted) {
				float distancia = 999999999999;
				int indice = -1;
				//std::cout << "saleeee1" << std::endl;
				for (int j = 0; j < enemigos_vistos_list.size(); j++) {

					if (Game::instance->antiterrorists_alive[enemigos_vistos_list[j]] == true) {
						float dist1 = sqrt(pow(abs(Game::instance->terrorists[i].getTranslation().x - Game::instance->antiterrorists[enemigos_vistos_list[j]].getTranslation().x), 2) + pow(abs(Game::instance->terrorists[i].getTranslation().z - Game::instance->antiterrorists[enemigos_vistos_list[j]].getTranslation().z), 2));
						if (dist1 < distancia) {
							distancia = dist1;
							indice = enemigos_vistos_list[j];
						}

					}
				}

				if (indice != -1) {

					Vector3 target_direction;
					target_direction = (Game::instance->antiterrorists[indice].getTranslation() - (Game::instance->terrorists[i].getTranslation()));
					float target_angle = atan2(target_direction.z, target_direction.x);
					//float target_angle = atan2(target_direction.z, target_direction.x);
					float final_angle = (target_angle - Game::instance->angulo_t[i]);
					Game::instance->terrorists[i].rotate(final_angle, Vector3(0, 1, 0));

					Game::instance->terrorists_shoot_disp[i] = false;
					Game::instance->terrorists_shoot_time[i] = Game::instance->tiempo_total + 0.4;
					Game::instance->terrorists_move_time[i] = Game::instance->tiempo_total + 0.7;
					Game::instance->terrorists_shoot[i] = true;
					shooting(Game::instance->terrorists[i], Game::instance->antiterrorists[indice], indice, true);

					/*std::cout << "saleeee2" << std::endl;
					std::cout << i << std::endl;
					std::cout << distancia << std::endl;
					std::cout << indice << std::endl;
					std::cout << "disparamosss2222222222222" << std::endl;
					std::cout << i << std::endl;
					std::cout << Game::instance->vida_ct[0] << std::endl;*/
					Game::instance->angulo_t[i] = target_angle;
				}

			}
			else if (Game::instance->terrorists_shoot_disp[i] == true && num_disparando.size()>0) {
				float distancia = 999999999999;
				int indice = -1;
				//std::cout << "saleeee1" << std::endl;
				for (int j = 0; j < num_disparando.size(); j++) {
					
					if (Game::instance->antiterrorists_alive[num_disparando[j]] == true) {
						float dist1 = sqrt(pow(abs(Game::instance->terrorists[i].getTranslation().x - Game::instance->antiterrorists[num_disparando[j]].getTranslation().x), 2) + pow(abs(Game::instance->terrorists[i].getTranslation().z - Game::instance->antiterrorists[num_disparando[j]].getTranslation().z), 2));
						if (dist1 < distancia) {
							distancia = dist1;
							indice = num_disparando[j];
						}

					}
				}
			
				if (indice != -1) {
					
					Vector3 target_direction;
					target_direction = (Game::instance->antiterrorists[indice].getTranslation() - (Game::instance->terrorists[i].getTranslation()));
					float target_angle = atan2(target_direction.z, target_direction.x);
					//float target_angle = atan2(target_direction.z, target_direction.x);
					float final_angle = (target_angle - Game::instance->angulo_t[i]);
					Game::instance->terrorists[i].rotate(final_angle, Vector3(0, 1, 0));
					
					Game::instance->terrorists_shoot_disp[i] = false;
					Game::instance->terrorists_shoot_time[i] = Game::instance->tiempo_total + 0.4;
					Game::instance->terrorists_move_time[i] = Game::instance->tiempo_total + 0.7;
					Game::instance->terrorists_shoot[i] = true;
					shooting(Game::instance->terrorists[i], Game::instance->antiterrorists[indice], indice, true);

					/*std::cout << "saleeee2" << std::endl;
					std::cout << i << std::endl;
					std::cout << distancia << std::endl;
					std::cout << indice << std::endl;
					std::cout << "disparamosss2222222222222" << std::endl;
					std::cout << i << std::endl;
					std::cout << Game::instance->vida_ct[0] << std::endl;*/
					Game::instance->angulo_t[i] = target_angle;
				}

				



			}

			else if (enemigos_vistos_total == 1 && Game::instance->terrorists_shoot_disp[i]==true) {
				//std::cout << "saleeee33333333333" << std::endl;
				for (int j = 0; j < 5; j++) {
					Vector3 target_direction_2 = (Game::instance->antiterrorists[j].getTranslation() - (Game::instance->terrorists[i].getTranslation()));
					float ang = dot(Game::instance->terrorists[i].frontVector(), target_direction_2.normalize());
					

					if (ang > 0.8) {
						
						bool enemigo_visto = enemigo_escondido(Game::instance->terrorists[i], Game::instance->antiterrorists[j], ang,true);


						if (Game::instance->antiterrorists_alive[j] && enemigo_visto) {
							
							/*if (Game::instance->antiterrorists_alive[0]) {
								std::cout << "muertooo" << std::endl;
							}*/
							
							Vector3 target_direction;
							target_direction = (Game::instance->antiterrorists[j].getTranslation() - (Game::instance->terrorists[i].getTranslation()));






							float target_angle = atan2(target_direction.z, target_direction.x);
							//float target_angle = atan2(target_direction.z, target_direction.x);
							float final_angle = (target_angle - Game::instance->angulo_t[i]);
							Game::instance->terrorists[i].rotate(final_angle, Vector3(0, 1, 0));
							Game::instance->terrorists_shoot_disp[i] = false;
							Game::instance->terrorists_shoot_time[i] = Game::instance->tiempo_total + 0.4;
							Game::instance->terrorists_move_time[i] = Game::instance->tiempo_total + 0.7;
							Game::instance->terrorists_shoot[i] = true;
							shooting(Game::instance->terrorists[i], Game::instance->antiterrorists[j], j, true);
							/*std::cout << " " << std::endl;
							std::cout << "disparamosss" << std::endl;
							std::cout << i << std::endl;
							std::cout << Game::instance->vida_ct[0] << std::endl;*/
							Game::instance->angulo_t[i] = target_angle;
							break;
						}
						
						
					}
				}


			}
			else if( (abs(m.getTranslation().x - destino.x) > 5 || abs(m.getTranslation().z - destino.z) > 5)&& Game::instance->terrorists_move_time[i]< Game::instance->tiempo_total){
				/*std::cout << "saleeee44444444" << std::endl;
				std::cout << "DESTINOOOOOOO" << std::endl;
				std::cout << i << std::endl;
				std::cout << destino.x << std::endl;
				std::cout << destino.z << std::endl;
				std::cout << Game::instance->primera_terror[i].x << std::endl;
				std::cout << Game::instance->primera_terror[i].z << std::endl;
				/*std::cout << m.getTranslation().x << std::endl;
				std::cout << m.getTranslation().z << std::endl;
				std::cout << abs(m.getTranslation().x - Game::instance->primera_terror[i].x) << std::endl;
				std::cout << abs(m.getTranslation().z - Game::instance->primera_terror[i].z) << std::endl;*/
				//std::vector<Vector3> shortest_path = aStarSearch(mapa_general,m.getTranslation(),destino);
				//std::cout << "AAAAAAAAAAAAAAAAAAAA" << std::endl;
				//std::cout << i<< std::endl;
				Vector3 shortest_path = new_move(Game::instance->first_auxiliar_points[i], source, destino,i,true);
				
				

				Vector3 target_direction;
				target_direction = (shortest_path - (m.getTranslation()));
				
				//target_direction = (Game::instance->antiterrorists[0].getTranslation() - (m.getTranslation()));

				/*std::cout << "AAAAAAAAAAAAAAAAAAAA" << std::endl;
				std::cout << Game::instance->antiterrorists[0].getTranslation().x << std::endl;
				std::cout << Game::instance->antiterrorists[0].getTranslation().z << std::endl;*/


				
				
				float target_angle = atan2(target_direction.z, target_direction.x);
				//float target_angle = atan2(target_direction.z, target_direction.x);
				float final_angle = (target_angle - Game::instance->angulo_t[i]);
				m.rotate(final_angle, Vector3(0, 1, 0));
			
				
				/*std::cout << "angulos" << std::endl;
				std::cout << target_angle << std::endl;
				std::cout << Game::instance->angulo_t[i] << std::endl;
				std::cout << final_angle << std::endl;*/
				

				m.translate(0, 0, speed*0.1);
				
				
				bool posible_mov = true;
				Vector3 character_center = m.getTranslation() + Vector3(0, 1, 0);
				Vector3 coll;
				Vector3 collnorm;
				

				Game::instance->angulo_t[i] = target_angle;
				Game::instance->terrorists[i] = m;
				if (posible_mov) {
					//this->model = m;
					/*if (destino.x != shortest_path.x && destino.z != shortest_path.z) {
						Game::instance->terrorists[i] = m;
					}*/
					Game::instance->terrorists[i] = m;
					
					moving = true;

				}
			
			}
			


			int zona = zona_actual(Game::instance->terrorists[i].getTranslation());//necesito saber en que zona está
			/*std::cout << "zona actual" << std::endl;
			std::cout << zona << std::endl;
			std::cout << i << std::endl;*/

			if(Game::instance->enemigos_zonas_terror[zona]< enemigos_vistos_total){
				Game::instance->enemigos_zonas_terror[zona]=enemigos_vistos_total;		
				
				//std::cout <<"sumamosss enemigos vistos" << std::endl;
				//std::cout << Game::instance->enemigos_zonas_terror[zona] << std::endl;

			}
			//a = 0 ,,,,, mid = 1,,,, b = 2
			//rotaciones
			if(Game::instance->ataque_bomba_a ==false && Game::instance->ataque_bomba_b==false && Game::instance->rondas_seguidas_perdidas_t != 3){
				if(zona==0 && Game::instance->enemigos_zonas_terror[zona]>1 && Game::instance->zona_correspondiente_t[i]==0){
					//me voy para la zona 2 de manera segura
					Game::instance->zona_correspondiente_t[i]=2;
					Vector3 pos_aux = Game::instance->terrorists[i].getTranslation();
					pos_aux.x = pos_aux.x + 128;
					pos_aux.z = pos_aux.z + 128;

					Vector3 dest_aux = Game::instance->primera_terror[4];
					dest_aux.x = dest_aux.x + 128;
					dest_aux.z = dest_aux.z + 128;


					std::vector<Vector3> recorregut = aStarSearch(Game::instance->list_of_matrix[1], pos_aux, dest_aux);
				
					Game::instance->terror_auxiliar[i]=1;
					Game::instance->first_auxiliar_points[i]=recorregut;

			
				}
				else if(zona==1 && Game::instance->enemigos_zonas_terror[0]>1){
					//me voy para la zona 2 de manera segura
					//Game::instance->zona_correspondiente_t[i]=6;
					//Game::instance->primera_terror[i]=Game::instance->primera_terror[4];


			
				}
				/*else if(zona==1 && Game::instance->enemigos_zonas_terror[2]>0){
					Game::instance->zona_correspondiente_t[i] = 0;
					Vector3 pos_aux = Game::instance->terrorists[i].getTranslation();
					pos_aux.x = pos_aux.x + 128;
					pos_aux.z = pos_aux.z + 128;

					Vector3 dest_aux =  Vector3(-101,0,-12);
					dest_aux.x = dest_aux.x + 128;
					dest_aux.z = dest_aux.z + 128;


					std::vector<Vector3> recorregut = aStarSearch(Game::instance->list_of_matrix[1], pos_aux, dest_aux);

					Game::instance->terror_auxiliar[i] = 1;
					Game::instance->first_auxiliar_points[i] = recorregut;
				}*/
				else if(zona==2 && Game::instance->enemigos_zonas_terror[zona]>5 && Game::instance->zona_correspondiente_t[i] == 2){
				
					printf("heeeeeeeeeeeyyyyyyyyyyy");

					Game::instance->zona_correspondiente_t[i] = 0;
					Vector3 pos_aux = Game::instance->terrorists[i].getTranslation();
					pos_aux.x = pos_aux.x + 128;
					pos_aux.z = pos_aux.z + 128;

					Vector3 dest_aux = Game::instance->primera_terror[1];
					dest_aux.x = dest_aux.x + 128;
					dest_aux.z = dest_aux.z + 128;


					std::vector<Vector3> recorregut = aStarSearch(Game::instance->list_of_matrix[2], pos_aux, dest_aux);

					Game::instance->terror_auxiliar[i] = 1;
					Game::instance->first_auxiliar_points[i] = recorregut;

				}
				/*else if(zona==1 && Game::instance->enemigos_zonas_terror[1]>1){
					if(Game::instance->enemigos_zonas_terror[0]>Game::instance->enemigos_zonas_terror[2]){
						Game::instance->zona_correspondiente_t[i]=6;
						Game::instance->primera_terror[i]=Game::instance->primera_terror[4];
					}
					else if(Game::instance->enemigos_zonas_terror[0]<Game::instance->enemigos_zonas_terror[2]){
						Game::instance->zona_correspondiente_t[i]=5;
						Game::instance->primera_terror[i]=Game::instance->primera_terror[1];
					}
					else{ //si hay el mismo numero de vistos, nos la jugamos a la bomba a
						Game::instance->zona_correspondiente_t[i]=5;
						Game::instance->primera_terror[i]=Game::instance->primera_terror[1];
					}
			
				}*/
				else if (abs(m.getTranslation().x - Game::instance->primera_terror[i].x) <= 6 && abs(m.getTranslation().z - Game::instance->primera_terror[i].z) <= 6 && zona== Game::instance->zona_correspondiente_t[i] && Game::instance->primera_terror_hecha[i]==false) {
					printf("pasamos la primera");
					Game::instance->primera_terror_hecha[i] = true;
					Vector3 pos_aux = Game::instance->terrorists[i].getTranslation();
					pos_aux.x = pos_aux.x + 128;
					pos_aux.z = pos_aux.z + 128;

					Vector3 dest_aux = Game::instance->segunda_terror[i];
					dest_aux.x = dest_aux.x + 128;
					dest_aux.z = dest_aux.z + 128;


					std::vector<Vector3> recorregut = aStarSearch(Game::instance->list_of_matrix[0], pos_aux, dest_aux);

					Game::instance->terror_auxiliar[i] = 1;
					Game::instance->first_auxiliar_points[i] = recorregut;

				}
				else if (abs(m.getTranslation().x - Game::instance->segunda_terror[i].x) <= 6 && abs(m.getTranslation().z - Game::instance->segunda_terror[i].z) <= 6 && zona == Game::instance->zona_correspondiente_t[i] && Game::instance->segunda_terror_hecha[i] == false) {
					printf("\npasamos la segunda\n");
					Game::instance->segunda_terror_hecha[i] = true;
					Vector3 pos_aux = Game::instance->terrorists[i].getTranslation();
					pos_aux.x = pos_aux.x + 128;
					pos_aux.z = pos_aux.z + 128;

					Vector3 dest_aux;
					
					if (zona == 0) {
						std::cout << "el avisador" << std::endl;
						std::cout << i << std::endl;
						Game::instance->ataque_bomba_a = true;
						dest_aux = Game::instance->bomba_a_posicion[i];

					}
					else if (zona == 2) {
						Game::instance->ataque_bomba_b = true;
						dest_aux = Game::instance->bomba_b_posicion[i];
					}
					dest_aux.x = dest_aux.x + 128;
					dest_aux.z = dest_aux.z + 128;


					std::vector<Vector3> recorregut = aStarSearch(Game::instance->list_of_matrix[0], pos_aux, dest_aux);

					Game::instance->terror_auxiliar[i] = 1;
					Game::instance->first_auxiliar_points[i] = recorregut;
					Game::instance->avisados_terror[i] = true;

				}
			}
			
			else if (Game::instance->avisados_terror[i] == false) {
				/*std::cout << "avisados" << std::endl;
				std::cout << i << std::endl;*/
				Game::instance->avisados_terror[i] = true;
				if (Game::instance->ataque_bomba_a) {
					Vector3 pos_aux = Game::instance->terrorists[i].getTranslation();
					pos_aux.x = pos_aux.x + 128;
					pos_aux.z = pos_aux.z + 128;

					Vector3 dest_aux = Game::instance->bomba_a_posicion[i];;

					
					dest_aux.x = dest_aux.x + 128;
					dest_aux.z = dest_aux.z + 128;


					std::vector<Vector3> recorregut = aStarSearch(Game::instance->list_of_matrix[1], pos_aux, dest_aux);

					Game::instance->terror_auxiliar[i] = 1;
					Game::instance->first_auxiliar_points[i] = recorregut;
				}
				else if (Game::instance->ataque_bomba_b) {
					/*std::cout << "entraaaaqui" << std::endl;
					std::cout << i << std::endl;*/
					Vector3 pos_aux = Game::instance->terrorists[i].getTranslation();
					pos_aux.x = pos_aux.x + 128;
					pos_aux.z = pos_aux.z + 128;

					Vector3 dest_aux = Game::instance->bomba_b_posicion[i];;


					dest_aux.x = dest_aux.x + 128;
					dest_aux.z = dest_aux.z + 128;


					std::vector<Vector3> recorregut = aStarSearch(Game::instance->list_of_matrix[0], pos_aux, dest_aux);

					Game::instance->terror_auxiliar[i] = 1;
					Game::instance->first_auxiliar_points[i] = recorregut;
				}

			}
			
			//miramos si esta en la zona de plante
			if(Game::instance->bomba_planted==false){
				bool zona_A=zona_plante_a(Game::instance->terrorists[i]);
				if(zona_A){
					//std::cout << "esta plantada en a" << std::endl;
					Game::instance->a_planted=true;
					Game::instance->bomb_position=Game::instance->terrorists[i].getTranslation();
					Game::instance->tiempo_ronda=30.0;
					Game::instance->bomba_planted = true;
				}
				bool zona_B=zona_plante_b(Game::instance->terrorists[i]);
				if(zona_B){
					
					//std::cout << "esta plantada en b" << std::endl;
					Game::instance->b_planted=true;
					Game::instance->bomb_position=Game::instance->terrorists[i].getTranslation();
					Game::instance->tiempo_ronda = 30;
					Game::instance->bomba_planted = true;
				}
			
			
			}
					
	
		}
		
	
	
	}

	moving = true;
	}


}




CT_Players::CT_Players() {

}


CT_Players::CT_Players(char* texture_name, char* mesh_name) {
	//__super::EntityMesh(texture_name, mesh_name);
	angle = 0;
	this->texture = Texture::Get(texture_name);
	// example of loading Mesh from Mesh Manager
	this->mesh = Mesh::Get(mesh_name);
	this->model.setIdentity();
	this->model.setScale(2.0f, 2.0f, 2.0f);
	this->position = Vector3(0, 0, 0);
	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

}
void CT_Players::render(float time) {

	/*if (shader)
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
	}*/
	/*if (Game::instance->antiterrorists.size() > 0) {

		shader->enable();
		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", Camera::current->viewprojection_matrix);
		shader->setUniform("u_texture", this->texture);
		shader->setUniform("u_model", this->model);
		shader->setUniform("u_time", time);

		//empieza en el 1 pq el 0 es mi player
		for (int i = 1; i < Game::instance->antiterrorists.size(); i++) {
			if(Game::instance->antiterrorists_alive[i]==true){
				shader->setUniform("u_model", Game::instance->antiterrorists[i]);
				this->mesh->render(GL_TRIANGLES);
			}
		}
		
		shader->disable();
	}*/

	for (int i = 1; i < Game::instance->antiterrorists.size(); i++) {
		if (Game::instance->antiterrorists_alive[i]) {
			shader->enable();
			glEnable(GL_DEPTH_TEST);
			anim1 = Animation::Get("data/people/terror/move/animations_walking.skanim");
			if (moving == false) {
				//std::cout << "aaaaaaaaaaaaaaaa" << std::endl;
				anim1->assignTime(0);
			}
			else {
				//std::cout << "bbbbbbbbbbbbbbbbbbbbbbbbb" << std::endl;
				anim1->assignTime(time);
			}
			

				this->texture == Texture::Get("data/people/captain.tga");
				renderAnimated(Game::instance->antiterrorists[i], Mesh::Get("data/people/terror/move/character.mesh"), Texture::Get("data/people/captain_2_skin.png"), &anim1->skeleton);

			
				Matrix44 hand = anim1->skeleton.getBoneMatrix("mixamorig_LeftHand", false);

				hand = hand * Game::instance->antiterrorists[i];
				//hand.rotate(180.0, Vector3(1, 0, 0));
				hand.rotate(45.0 * DEG2RAD, Vector3(0, -1, 1));
				hand.rotate(45.0 * DEG2RAD, Vector3(0, 0, 1));

				hand.scale(0.03, 0.03, 0.03);
				this->arma.model.scale(0.2, 0.2, 0.2);
				//hand.translate(0, 8, 0);
				renderQuieto(hand, this->arma.mesh, this->arma.texture);
				shader->disable();
		}



	}

	


	

	




}









void CT_Players::update(float speed) {
	//std::cout << "enemigos en zona 0" << std::endl;
	//std::cout << Game::instance->enemigos_zonas_ct[0] << std::endl;
	

	if (Game::instance->empezamos) {

		for (int i = 1; i <5; i++) {
			Vector3 posicionament_aux = Game::instance->antiterrorists[i].getTranslation();

			if (Game::instance->antiterrorists_alive[i]) {
				Matrix44 m;
				m = Game::instance->antiterrorists[i];
				//provisional
				//Vector3 destino= Game::instance->primera_terror[i];
				Vector3 destino = Game::instance->first_auxiliar_points_ct[i][Game::instance->first_auxiliar_points_ct[i].size() - 1];
				/*std::cout << "DESTINOOOOOOO" << std::endl;
				std::cout << Game::instance->primera_terror[0].x << std::endl;
				std::cout << Game::instance->primera_terror[0].z << std::endl;*/



				Vector3 source = m.getTranslation();
				//std::cout << source.x << std::endl;
				//std::cout << source.z << std::endl;



				int enemigos_vistos_total = 0;
				for (int j = 0; j < 5; j++) {
					Vector3 target_direction_2 = (Game::instance->terrorists[j].getTranslation() - (Game::instance->antiterrorists[i].getTranslation()));
					float ang = dot(Game::instance->antiterrorists[i].frontVector(), target_direction_2.normalize());
					/*if (i == 4  && j == 4) {
						std::cout << "angaaaaaaaaaasssssssssssssssaaaaaaaaaa" << std::endl;
						std::cout << ang << std::endl;
					}
					*/
					if (ang > 0.8) {
						bool enemigo_visto = enemigo_escondido(Game::instance->antiterrorists[i], Game::instance->terrorists[j], ang,false);

						/*std::cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" << std::endl;std::cout << "aaaaaaaaaaaaaaaaaaaaaaaaaa" << std::endl;std::cout << "enemigo_visto" << std::endl;
						std::cout << enemigo_visto << std::endl;
						std::cout << j << std::endl;
						std::cout << ang << std::endl;*/
						if (enemigo_visto && Game::instance->terrorists_alive[j]) {
							//printf("lo hemos visto");
							//std::cout << j << std::endl;
							enemigos_vistos_total++;

						}
					}
				}
				//std::cout << "enemigos vistos 0" << std::endl;
				//std::cout << enemigos_vistos_total << std::endl;

			
				if (Game::instance->antiterrorists_shoot_disp[i] == false && Game::instance->antiterrorists_shoot_time[i] <= Game::instance->tiempo_total) {
					Game::instance->antiterrorists_shoot_disp[i] = true;
					Game::instance->antiterrorists_shoot[i] = true;
					/*std::cout << "recargaaaaa" << std::endl;
					std::cout << Game::instance->terrorists_shoot[4] << std::endl;
					std::cout << Game::instance->terrorists_alive[4] << std::endl;*/
					


				}


				/*std::cout << "enemigo 4" << std::endl;
				std::cout << Game::instance->terrorists_shoot[4] << std::endl;
				std::cout << Game::instance->terrorists_alive[4] << std::endl;*/

				std::vector<int> num_disparando;
				for (int j = 0; j < 5; j++) {
					if (Game::instance->terrorists_shoot[j] && Game::instance->terrorists_alive[j]) {
						/*std::cout << "entraaaaaaaaaaaaaaaaaaaaaa" << std::endl;
						std::cout << j << std::endl;
						std::cout << Game::instance->terrorists_alive[j] << std::endl;*/

						Vector3 target_direction_2 = (Game::instance->terrorists[j].getTranslation() - (Game::instance->antiterrorists[i].getTranslation()));
						float ang = dot(Game::instance->antiterrorists[i].frontVector(), target_direction_2.normalize());
						//std::cout << ang << std::endl;


						bool enemigo_visto = enemigo_escondido(Game::instance->antiterrorists[i], Game::instance->terrorists[j], ang,false);
						/*if (i == 4 && j == 4) {
							std::cout << "enemigo disparando" << std::endl;
							std::cout << enemigo_visto << std::endl;

						}*/

						if (enemigo_visto && Game::instance->terrorists_alive[j]) {
							//std::cout << "suuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu" << std::endl;
							num_disparando.push_back(j);

						}

					}

				}


				/*std::cout << "disparandooo" << std::endl;
				std::cout << num_disparando.size() << std::endl;
				std::cout << "enemigos_vistos_total" << std::endl;
				std::cout << enemigos_vistos_total << std::endl;*/

				/*if (num_disparando.size() > 0) {
					std::cout << "num_disparando.size()" << std::endl;

				}*/
				
				/*std::cout << "disparandooo" << std::endl;
				std::cout << Game::instance->terrorists_shoot[0] << std::endl;
				std::cout << Game::instance->terrorists_shoot[1] << std::endl;*/
				
				
				if (Game::instance->antiterrorists_shoot_disp[i] == true && num_disparando.size() > 0) {
					float distancia = 999999999999;
					int indice = -1;
					/*std::cout << "saleeee1" << std::endl;
					std::cout << "saleeee1" << std::endl;
					std::cout << "saleeee1" << std::endl;
					std::cout << "saleeee1" << std::endl;*/
					for (int j = 0; j < num_disparando.size(); j++) {

						if (Game::instance->terrorists_alive[num_disparando[j]] == true) {
							float dist1 = sqrt(pow(abs(Game::instance->antiterrorists[i].getTranslation().x - Game::instance->terrorists[num_disparando[j]].getTranslation().x), 2) + pow(abs(Game::instance->antiterrorists[i].getTranslation().z - Game::instance->terrorists[num_disparando[j]].getTranslation().z), 2));
							if (dist1 < distancia) {
								distancia = dist1;
								indice = num_disparando[j];
							}

						}
					}

					if (indice != -1) {

						Vector3 target_direction;
						target_direction = (Game::instance->terrorists[indice].getTranslation() - (Game::instance->antiterrorists[i].getTranslation()));
						float target_angle = atan2(target_direction.z, target_direction.x);
						//float target_angle = atan2(target_direction.z, target_direction.x);
						float final_angle = (target_angle - Game::instance->angulo_ct[i]);
						Game::instance->antiterrorists[i].rotate(final_angle, Vector3(0, 1, 0));

						Game::instance->antiterrorists_shoot_disp[i] = false;
						Game::instance->antiterrorists_shoot_time[i] = Game::instance->tiempo_total + 0.4;
						Game::instance->antiterrorists_move_time[i] = Game::instance->tiempo_total + 0.7;
						Game::instance->antiterrorists_shoot[i] = true;
						//std::cout << "dispara 111111111111" << std::endl;
						shooting(Game::instance->antiterrorists[i], Game::instance->terrorists[indice], indice, false);

						/*std::cout << "saleeee2" << std::endl;
						std::cout << i << std::endl;
						std::cout << distancia << std::endl;
						std::cout << indice << std::endl;
						std::cout << "disparamosss2222222222222" << std::endl;
						std::cout << i << std::endl;
						std::cout << Game::instance->vida_t[4] << std::endl;*/
						Game::instance->angulo_ct[i] = target_angle;
					}





				}

				else if (enemigos_vistos_total >= 1 && Game::instance->antiterrorists_shoot_disp[i] == true) {
					/*std::cout << "saleeee33333333333" << std::endl;
					std::cout << "saleeee33333333333" << std::endl;
					std::cout << "saleeee33333333333" << std::endl;
					std::cout << "saleeee33333333333" << std::endl;*/
					for (int j = 0; j < 5; j++) {
						Vector3 target_direction_2 = (Game::instance->terrorists[j].getTranslation() - (Game::instance->antiterrorists[i].getTranslation()));
						float ang = dot(Game::instance->antiterrorists[i].frontVector(), target_direction_2.normalize());


						if (ang > 0.8) {

							bool enemigo_visto = enemigo_escondido(Game::instance->antiterrorists[i], Game::instance->terrorists[j], ang,false);


							if (Game::instance->terrorists_alive[j] && enemigo_visto) {

								/*if (Game::instance->antiterrorists_alive[0]) {
									std::cout << "muertooo" << std::endl;
								}*/

								Vector3 target_direction;
								target_direction = (Game::instance->terrorists[j].getTranslation() - (Game::instance->antiterrorists[i].getTranslation()));






								float target_angle = atan2(target_direction.z, target_direction.x);
								//float target_angle = atan2(target_direction.z, target_direction.x);
								float final_angle = (target_angle - Game::instance->angulo_ct[i]);
								Game::instance->antiterrorists[i].rotate(final_angle, Vector3(0, 1, 0));
								Game::instance->antiterrorists_shoot_disp[i] = false;
								Game::instance->antiterrorists_shoot_time[i] = Game::instance->tiempo_total + 0.4;
								Game::instance->antiterrorists_move_time[i] = Game::instance->tiempo_total + 0.7;
								Game::instance->antiterrorists_shoot[i] = true;
								//std::cout << "dispara 22222222" << std::endl;
								shooting(Game::instance->antiterrorists[i], Game::instance->terrorists[j], j, false);
								/*std::cout << " " << std::endl;
								std::cout << "disparamosss" << std::endl;
								std::cout << i << std::endl;
								std::cout << Game::instance->vida_ct[0] << std::endl;*/
								Game::instance->angulo_ct[i] = target_angle;
								break;
							}


						}
					}


				}


				else if ((abs(m.getTranslation().x - destino.x) > 5 || abs(m.getTranslation().z - destino.z) > 5) && Game::instance->antiterrorists_move_time[i] < Game::instance->tiempo_total) {
					/*std::cout << "saleeee4444444" << std::endl;
					std::cout << "saleeee4444444" << std::endl;
					std::cout << "saleeee4444444" << std::endl;
					std::cout << "saleeee4444444" << std::endl;*/
					//std::vector<Vector3> shortest_path = aStarSearch(mapa_general,m.getTranslation(),destino);
					//std::cout << "AAAAAAAAAAAAAAAAAAAA" << std::endl;
					//std::cout << i<< std::endl;
					Vector3 shortest_path = new_move(Game::instance->first_auxiliar_points_ct[i], source, destino, i,false);

					

					Vector3 target_direction;
					target_direction = (shortest_path - (m.getTranslation()));




					float target_angle = atan2(target_direction.z, target_direction.x);
					//float target_angle = atan2(target_direction.z, target_direction.x);
					float final_angle = (target_angle - Game::instance->angulo_ct[i]);
					m.rotate(final_angle, Vector3(0, 1, 0));


					/*std::cout << "angulos" << std::endl;
					std::cout << target_angle << std::endl;
					std::cout << Game::instance->angulo_t[i] << std::endl;
					std::cout << final_angle << std::endl;*/


					m.translate(0, 0, speed * 0.1);


					bool posible_mov = true;
					
					

					Game::instance->angulo_ct[i] = target_angle;
					Game::instance->antiterrorists[i] = m;
					if (posible_mov) {
						//this->model = m;
						/*if (destino.x != shortest_path.x && destino.z != shortest_path.z) {
							Game::instance->terrorists[i] = m;
						}*/
						Game::instance->antiterrorists[i] = m;

						moving = true;

					}
				

				}

				int zona = zona_actual(Game::instance->antiterrorists[i].getTranslation());//necesito saber en que zona está
			/*std::cout << "zona actual" << std::endl;
			std::cout << zona << std::endl;
			std::cout << i << std::endl;*/

				if (Game::instance->enemigos_zonas_ct[zona] < enemigos_vistos_total) {
					Game::instance->enemigos_zonas_ct[zona] = enemigos_vistos_total;


				}
				if (Game::instance->bomba_planted == false) {
					if (i==2 && Game::instance->enemigos_zonas_ct[0] > 1) {
						Game::instance->zona_correspondiente_ct[i] = 0;
						Vector3 pos_aux = Game::instance->antiterrorists[i].getTranslation();
						pos_aux.x = pos_aux.x + 128;
						pos_aux.z = pos_aux.z + 128;

						Vector3 dest_aux = Vector3(-46, 0, -115);
						dest_aux.x = dest_aux.x + 128;
						dest_aux.z = dest_aux.z + 128;


						std::vector<Vector3> recorregut = aStarSearch(Game::instance->list_of_matrix[1], pos_aux, dest_aux);

						Game::instance->ct_auxiliar[i] = 1;
						Game::instance->first_auxiliar_points_ct[i] = recorregut;

					}
					else if (zona == 1 && Game::instance->enemigos_zonas_ct[2] > 1) {
						Game::instance->zona_correspondiente_ct[i] = 2;
						Vector3 pos_aux = Game::instance->antiterrorists[i].getTranslation();
						pos_aux.x = pos_aux.x + 128;
						pos_aux.z = pos_aux.z + 128;

						Vector3 dest_aux = Vector3(64, 0, -115);
						dest_aux.x = dest_aux.x + 128;
						dest_aux.z = dest_aux.z + 128;


						std::vector<Vector3> recorregut = aStarSearch(Game::instance->list_of_matrix[1], pos_aux, dest_aux);

						Game::instance->ct_auxiliar[i] = 1;
						Game::instance->first_auxiliar_points_ct[i] = recorregut;

					}


				}
				else if (Game::instance->avisados_ct[i]==false && Game::instance->bomba_planted) {
					Game::instance->avisados_ct[i] = true;
					Vector3 pos_aux = Game::instance->antiterrorists[i].getTranslation();
					pos_aux.x = pos_aux.x + 128;
					pos_aux.z = pos_aux.z + 128;

					Vector3 dest_aux = Game::instance->bomb_position;
					dest_aux.x = dest_aux.x + 128;
					dest_aux.z = dest_aux.z + 128;


					std::vector<Vector3> recorregut = aStarSearch(Game::instance->list_of_matrix[0], pos_aux, dest_aux);

					Game::instance->ct_auxiliar[i] = 1;
					Game::instance->first_auxiliar_points_ct[i] = recorregut;

				}
				
				if (Game::instance->bomba_planted == true) {
					bool zona_A = zona_plante_a(Game::instance->antiterrorists[i]);
					if (zona_A && Game::instance->a_planted == true && Game::instance->antiterrorists_alive[i]) {
						std::cout << "la lian aqui" << std::endl;
						Game::instance->bomba_planted = false;
						Game::instance->tiempo_ronda = 0;
						
					}
					bool zona_B = zona_plante_b(Game::instance->antiterrorists[i]);
					if (zona_B && Game::instance->antiterrorists_alive[i] && Game::instance->b_planted == true) {
						std::cout << "la lian aqui   1" << std::endl;
						Game::instance->bomba_planted = false;
						Game::instance->tiempo_ronda = 0;
					}
					
					if (abs(Game::instance->antiterrorists[i].getTranslation().x - Game::instance->bomb_position.x) < 5 && abs(Game::instance->antiterrorists[i].getTranslation().z - Game::instance->bomb_position.z) < 5 && Game::instance->antiterrorists_alive[i]) {
						std::cout << "la lian aqui 222" << std::endl;
						Game::instance->bomba_planted = false;
						Game::instance->tiempo_ronda = 0;

					}


				}

			}


		}

		moving = true;
	}
}
