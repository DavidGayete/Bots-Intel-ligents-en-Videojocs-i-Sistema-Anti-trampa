#include "Stage.h"
#include "game.h"
Floor plane;

Camera* camera;

InitStage* init_stage = NULL;

Stage* current_stage = NULL;
Animation* anim ;


Wall wall;


GameMap g;
float menu = false;
int max_puntuacio;
bool load = true;
int nivel = 0;

int puntuacion_1;
int puntuacion_2;
int puntuacion_3;

void objeto_cercano(Mesh* mesh, Matrix44 m, Matrix44 m1) {
	Vector3 character_center = m.getTranslation() + Vector3(0, 1, 0);
	Vector3 coll;
	Vector3 collnorm;

	if (abs(m.getTranslation().x- m1.getTranslation().x)<100 && abs(m.getTranslation().z - m1.getTranslation().z) < 100) {
		Game::instance->objeto_cercano = true;
	}

}
void MenuStage::init(int window_width, int window_height) {
	
	
	
	camera = new Camera();
	camera->lookAt(Vector3(0.f, 10.f, 10.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f, window_width / (float)window_height, 0.1f, 500.f); //set the projection, we want to be perspective
	
}

void MenuStage::render(float time) {

	text_menu = Texture::Get("data/foto_menu_1_buena.png");
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	Mesh quad;
	quad.createQuad(0, 0, 2, 2, false);
	Shader* shader = Shader::Get("data/shaders/QUAD.vs", "data/shaders/texture.fs");
	shader->enable();
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture", text_menu);
	shader->setUniform("u_texture_tiling", 1.0f);
	quad.render(GL_TRIANGLES);
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	Mesh quad1;
	quad1.createQuad(-0.2, 0, 0.5, 0.25, false);
	Shader* shader1 = Shader::Get("data/shaders/QUAD.vs", "data/shaders/texture.fs");
	shader1->enable();
	shader1->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader1->setUniform("u_texture", text_menu2);
	shader1->setUniform("u_texture_tiling", 1.0f);
	quad1.render(GL_TRIANGLES);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);

	Mesh quad3;
	quad3.createQuad(-0.2, -0.3, 0.5, 0.25, false);
	Shader* shader3 = Shader::Get("data/shaders/QUAD.vs", "data/shaders/texture.fs");
	shader3->enable();
	shader3->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader3->setUniform("u_texture", text_menu4);
	shader3->setUniform("u_texture_tiling", 1.0f);
	quad3.render(GL_TRIANGLES);



	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	
	Mesh quad2;
	quad2.createQuad(-0.2, -0.6, 0.5, 0.25, false);
	Shader* shader2 = Shader::Get("data/shaders/QUAD.vs", "data/shaders/texture.fs");
	shader2->enable();
	shader2->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader2->setUniform("u_texture", text_menu3);
	shader2->setUniform("u_texture_tiling", 1.0f);
	quad2.render(GL_TRIANGLES);
	
	

}


void MenuStage::update(float seconds_elapsed) {
	
	if (Input::mouse_position.x < 418 && Input::mouse_position.x > 219 && Input::mouse_position.y>263 && Input::mouse_position.y<337) {
		text_menu2 = Texture::Get("data/new_game_2.png");
		if ((Input::mouse_state & SDL_BUTTON_LEFT)) {
			stop_sound("data/audios/IntroMusic.wav", true);
			play_sound("data/audios/ClickButton.wav", false);
			SDL_ShowCursor(Game::instance->mouse_locked); //hide or show the mouse
			Game::instance->zombie_speed = 25;
			nivel = 0;
			current_stage = load_stage;
			Game::instance->zombie_speed = 25;
		}
		
	}
	else {
		text_menu2 = Texture::Get("data/new_game_1.png");
	}
	
	
	if (Input::mouse_position.x < 418 && Input::mouse_position.x > 219 && Input::mouse_position.y > 443 && Input::mouse_position.y < 517) {
		text_menu3 = Texture::Get("data/controllers_2.png");
		if ((Input::mouse_state & SDL_BUTTON_LEFT)) {
			stop_sound("data/audios/IntroMusic.wav", true);
			play_sound("data/audios/ClickButton.wav", false);

			current_stage = control_stage;
		}
	}
	else {
		text_menu3 = Texture::Get("data/controllers_1.png");
	}

	if (Input::mouse_position.x < 418 && Input::mouse_position.x > 219 && Input::mouse_position.y > 352 && Input::mouse_position.y < 427) {
		text_menu4 = Texture::Get("data/load_game1.png");
		if ((Input::mouse_state & SDL_BUTTON_LEFT)) {
			stop_sound("data/audios/IntroMusic.wav", true);
			play_sound("data/audios/ClickButton.wav", false);
			nivel=nivel_aux;
			load = false;
			SDL_ShowCursor(Game::instance->mouse_locked); //hide or show the mouse
			current_stage = load_stage;
		}
	}
	else {
		text_menu4 = Texture::Get("data/load_game.png");
	}

}

void MenuStage2::render(float time) {

	text_menu = Texture::Get("data/foto_menu_1_buena.png");
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	Mesh quad;
	quad.createQuad(0, 0, 2, 2, false);
	Shader* shader = Shader::Get("data/shaders/QUAD.vs", "data/shaders/texture.fs");
	shader->enable();
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture", text_menu);
	shader->setUniform("u_texture_tiling", 1.0f);
	quad.render(GL_TRIANGLES);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	Mesh quad1;
	quad1.createQuad(-0.2, 0, 0.5, 0.25, false);
	Shader* shader1 = Shader::Get("data/shaders/QUAD.vs", "data/shaders/texture.fs");
	shader1->enable();
	shader1->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader1->setUniform("u_texture", text_menu2);
	shader1->setUniform("u_texture_tiling", 1.0f);
	quad1.render(GL_TRIANGLES);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);

	Mesh quad3;
	quad3.createQuad(-0.2, -0.3, 0.5, 0.25, false);
	Shader* shader3 = Shader::Get("data/shaders/QUAD.vs", "data/shaders/texture.fs");
	shader3->enable();
	shader3->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader3->setUniform("u_texture", text_menu4);
	shader3->setUniform("u_texture_tiling", 1.0f);
	quad3.render(GL_TRIANGLES);



	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);

	Mesh quad2;
	quad2.createQuad(-0.2, -0.6, 0.5, 0.25, false);
	Shader* shader2 = Shader::Get("data/shaders/QUAD.vs", "data/shaders/texture.fs");
	shader2->enable();
	shader2->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader2->setUniform("u_texture", text_menu3);
	shader2->setUniform("u_texture_tiling", 1.0f);
	quad2.render(GL_TRIANGLES);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);

	Mesh quad4;
	quad4.createQuad(-0.2, -0.9, 0.5, 0.25, false);
	Shader* shader4 = Shader::Get("data/shaders/QUAD.vs", "data/shaders/texture.fs");
	shader4->enable();
	shader4->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader4->setUniform("u_texture", text_menu5);
	shader4->setUniform("u_texture_tiling", 1.0f);
	quad4.render(GL_TRIANGLES);

}


void MenuStage2::update(float seconds_elapsed) {
	
	//continuar game
	if (Input::mouse_position.x < 418 && Input::mouse_position.x > 219 && Input::mouse_position.y > 263 && Input::mouse_position.y < 337) {
		text_menu2 = Texture::Get("data/continue2.png");
		if ((Input::mouse_state & SDL_BUTTON_LEFT)) {
			play_sound("data/audios/ClickButton.wav", false);
			SDL_ShowCursor(Game::instance->mouse_locked); //hide or show the mouse
			current_stage = play_stage;

		}

	}
	else {
		text_menu2 = Texture::Get("data/continue.png");
	}

	//restart
	if (Input::mouse_position.x < 418 && Input::mouse_position.x > 219 && Input::mouse_position.y > 352 && Input::mouse_position.y < 427) {
		text_menu4 = Texture::Get("data/restart2.png");
		
		if ((Input::mouse_state & SDL_BUTTON_LEFT)) {
			play_sound("data/audios/ClickButton.wav", false);
			SDL_ShowCursor(Game::instance->mouse_locked); //hide or show the mouse
			g.loading_game("data/new_game.txt");
			current_stage = play_stage;
			//g.saving_game("data/saved_game.txt");

		}
	}
	else {
		text_menu4 = Texture::Get("data/restart.png");
	}

	// save game
	

	
	if (Input::mouse_position.x < 418 && Input::mouse_position.x > 219 && Input::mouse_position.y > 443 && Input::mouse_position.y < 517) {
		text_menu3 = Texture::Get("data/savegame2.png");
		if ((Input::mouse_state & SDL_BUTTON_LEFT)) {
			play_sound("data/audios/ClickButton.wav", false);
			nivel_aux = nivel;
			g.saving_game("data/saved_game.txt");
			
		}
	}
	else {
		text_menu3 = Texture::Get("data/savegame.png");
	}
	//go back to menu
	if (Input::mouse_position.x < 418 && Input::mouse_position.x > 219 && Input::mouse_position.y > 533 && Input::mouse_position.y < 587) {
		text_menu5 = Texture::Get("data/menu2.png");
		
		if ((Input::mouse_state & SDL_BUTTON_LEFT)) {
			play_sound("data/audios/ClickButton.wav", false);
			carga = true;

			current_stage = init_stage;
		}
	}
	else {
		text_menu5 = Texture::Get("data/menu.png");
	}
	
}


void PlayStage::init(int window_width, int window_height) {
	anim = Animation::Get("data/assets/zombie/animations_walking.skanim");
	camera = new Camera();
	camera->lookAt(Vector3(0.f, 10.f, 10.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f, window_width / (float)window_height, 0.1f, 500.f); //set the projection, we want to be perspective
	plane = Floor(512);
	ic = icono();
	wall = Wall(512);
	zombies_base = House("data/assets/town/materials.tga", "data/assets/town/house.obj");
	
	chasis = Chasis("data/assets/coche/chasis/Car Uv.tga", "data/assets/coche/chasis/final_Car2.obj");

	motor = Motor("data/assets/coche/motor/bumpy normal abbrasion Map.png", "data/assets/coche/motor/engine.obj");
	
	neumatic = Neumatic("data/assets/coche/neumatico/wheel.png", "data/assets/coche/neumatico/tirenrim.obj");

	captain = Captain("data/assets/people/captain.tga", "data/assets/people/captain.ASE");
	tree = Tree("data/assets/trees/trees/trunk_3.tga", "data/assets/trees/trees/trunk.obj");
	captain.machete = Machete("data/assets/machete/21939_Machete_v1_diff.tga", "data/assets/machete/21939_Machete_v1.obj");
	aguila = Aguila("data/assets/aguila/Fogel_Mat_Diffuse_Color2.tga", "data/assets/aguila/Eagle2.obj");
	zombie = Zombie("data/assets/zombie/pacient_body.tga", "data/assets/zombie/dead_man.obj");
	g = GameMap();
	//g.create_map("data/map1.txt");
	g.read_highscore("data/highscore.txt");
	g.load_map("data/map2.txt", zombie.mesh->box.halfsize.x, zombie.mesh->box.halfsize.z);



	
}

void EagleStage::render(float time) {
	
	if (primera == true) {
		
		Game::instance->modo_aguila_act = true;
		tiempo_aguila = time;
		primera = false;
	}
	else {
		if (time>=tiempo_aguila+6) {
			tiempo_cumplido = true;
			primera = true;
			Game::instance->modo_aguila_act = false;
		}
	}
	float tiempo_restante = tiempo_aguila + 6 - time;
	
	
	Game::instance->tiempo_aguila_game = tiempo_restante;
	Vector3 eye = aguila.model * Vector3(0, 10,-20);
	Vector3 center = aguila.model * Vector3(0, 5,0);
	Vector3 up = Vector3(0.f, 0.f, 0.f);
	camera->lookAt(eye, center, Vector3(0.f, 1.f, 0.f));

	//set the clear color (the background color)
	glClearColor(0.6, 0.6, 0.9, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	if (Game::instance->modo_aguila_act) {
		drawText(300, 100, "tiempo restante:  " + std::to_string(int(Game::instance->tiempo_aguila_game)), Vector3(1, 1, 1), 2);
	}


	tree.render_forest(time);
	plane.render_floor(time);
	ic.render(time);
	captain.render(time);
	zombie.render_zombie(time);
	wall.render_walls(time);
	chasis.render(time);
	neumatic.render(time);
	motor.render(time);

	zombies_base.render_houses(time);

	aguila.render_aguila2(time);
	if (Game::instance->modo_aguila_act) {
		drawText(300, 100, "tiempo restante:  " + std::to_string(int(Game::instance->tiempo_aguila_game)), Vector3(1, 1, 1), 2);
	}
}

void PlayStage::render(float time) {
	


	
	captain.player_colision = false;
	Matrix44 R;
	R.setRotation(captain.angle, Vector3(0, 1, 0));
	//R.scale(2.0, 2.0, 2.0);
	captain.front = R.rotateVector(Vector3(0, 0, -1));
	captain.right = R.rotateVector(Vector3(1, 0, 0));



	Vector3 character_center = captain.model.getTranslation() + Vector3(0, 1, 0);
	Vector3 coll;
	Vector3 collnorm;
	for (int i = 0; i < trees_in_frustum.size(); i++) {
		if ((captain.mesh->testSphereCollision(trees_in_frustum[i], character_center, 1, coll, collnorm)) == true) {
			captain.player_colision = true;
			break;
		}
	}

	Vector3 eye = captain.model*Vector3(0, 4, -7);
	Vector3 center = captain.model*Vector3();
	Vector3 up = Vector3(0.f, 1.f, 0.f);
	camera->lookAt(eye, center, Vector3(0.f, 1.f, 0.f));

	//set the clear color (the background color)
	glClearColor(0.6, 0.6, 0.9, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	tree.render_forest(time);
	plane.render_floor(time);
	ic.render(time);
	captain.render(time);
	wall.render_walls(time);
	zombies_base.render_houses(time);
	
	chasis.render(time);
	if (!Game::instance->neumatico_cogido) {
		neumatic.render(time);
	}
	if (!Game::instance->motor_cogido) {
		motor.render(time);
	}
	
	zombie.render_zombie(time);
	
	if (Game::instance->objeto_cercano == false) {
		objeto_cercano(captain.mesh, captain.model, motor.model);
			
		objeto_cercano(captain.mesh, captain.model, neumatic.model);

		objeto_cercano(captain.mesh, captain.model, chasis.model);

	}


	if (Game::instance->primer_inmune) {
		Game::instance->tiempo_inmune = time;
		Game::instance->primer_inmune = false;
	}
	else {

		
		if (time >= Game::instance->tiempo_inmune + 4) {

			Game::instance->inmune = false;
		}
	}


	if (int(time) % 30 == 0 && int(time) > Game::instance->tiempo_aux) {
		Game::instance->tiempo_aux = int(time);
		Game::instance->puntuacion--;
	}

	drawText(600, 50, "Vidas Restantes:" + std::to_string(Game::instance->player_vidas), Vector3(1, 1, 1), 2);
	drawText(600, 70, "Puntuacion:" + std::to_string(Game::instance->puntuacion), Vector3(1, 1, 1), 2);
	drawText(600, 90, "Highscore:" + std::to_string(levels_puntuacion[nivel][1]), Vector3(1, 1, 1), 2);

	
	if (Game::instance->objeto_cercano == true && Game::instance->modo_aguila_act == false) {
		drawText(300, 100, "Cuidado, objeto cerca! ", Vector3(1, 1, 1), 2);
		Game::instance->objeto_cercano = false;

	}

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);

	if (Game::instance->modo_aguila_disponible) {
		Mesh quad;


		quad.createQuad(-0.8, 0.8, 0.15, 0.15 * (Game::instance->window_width / (float)Game::instance->window_width), false);
		Shader* shader = Shader::Get("data/shaders/QUAD.vs", "data/shaders/texture.fs");
		shader->enable();


		shader->setUniform("u_color", Vector4(1, 1, 1, 1));

		Texture* text = Texture::Get("data/assets/aa.png");

		shader->setUniform("u_texture", text);
		shader->setUniform("u_texture_tiling", 1.0f);
		quad.render(GL_TRIANGLES);
	}


	if (Game::instance->ataque_disponible) {
		Mesh quad;


		quad.createQuad(-0.5, 0.8, 0.15, 0.15 * (Game::instance->window_width / (float)Game::instance->window_width), false);
		Shader* shader = Shader::Get("data/shaders/QUAD.vs", "data/shaders/texture.fs");
		shader->enable();


		shader->setUniform("u_color", Vector4(1, 1, 1, 1));

		Texture* text = Texture::Get("data/assets/knife.tga");

		shader->setUniform("u_texture", text);
		shader->setUniform("u_texture_tiling", 1.0f);
		quad.render(GL_TRIANGLES);
	}


	if (Game::instance->motor_cogido) {
		Mesh quad;


		quad.createQuad(-0.2, 0.8, 0.15, 0.15 * (Game::instance->window_width / (float)Game::instance->window_width), false);
		Shader* shader = Shader::Get("data/shaders/QUAD.vs", "data/shaders/texture.fs");
		shader->enable();


		shader->setUniform("u_color", Vector4(1, 0, 0, 1));

		Texture* text = Texture::Get("data/assets/motor_dibujo.tga");

		shader->setUniform("u_texture", text);
		shader->setUniform("u_texture_tiling", 1.0f);
		quad.render(GL_TRIANGLES);
	}

	if (Game::instance->neumatico_cogido) {
		Mesh quad;


		quad.createQuad(0.1, 0.8, 0.15, 0.15 * (Game::instance->window_width / (float)Game::instance->window_width), false);
		Shader* shader = Shader::Get("data/shaders/QUAD.vs", "data/shaders/texture.fs");
		shader->enable();


		shader->setUniform("u_color", Vector4(1, 0, 0, 1));

		Texture* text = Texture::Get("data/assets/neumatico_dibujo.tga");

		shader->setUniform("u_texture", text);
		shader->setUniform("u_texture_tiling", 1.0f);
		quad.render(GL_TRIANGLES);
	}



}


void PlayStage::update(float seconds_elapsed) {
	float speed = seconds_elapsed * 5; //the speed is defined by the seconds_elapsed so it goes constant
	captain.update(speed,neumatic,motor,chasis);
	aguila.update(speed);
	zombie.update(seconds_elapsed, &captain);
	Game::instance->mouse_locked = true;
	if (Game::instance->mouse_locked)
		Input::centerMouse();
	if (Input::isKeyPressed(SDL_SCANCODE_1) && Game::instance->modo_aguila_disponible==true) {
			Game::instance->modo_aguila_disponible = false;
			aguila.model = captain.model;
			primera = true;
			aguila.model.rotate(0.7f, Vector3(-1.0f, 0.0f, 0.0f));
			Vector3 eye = aguila.model * Vector3(0, 10, -20);
			Vector3 center = aguila.model * Vector3();
			Vector3 up = Vector3(0.f, 0.f, 0.f);
			camera->lookAt(eye, center, Vector3(0.f, 1.f, 0.f));
			aguila.render_aguila(seconds_elapsed);
			current_stage = eagle_stage;
		
	
		
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_M) ) {
		Game::instance->mouse_locked = false;
		SDL_ShowCursor(!Game::instance->mouse_locked);
		current_stage = menu_stage2;


	}
	
	if (Input::wasKeyPressed(SDL_SCANCODE_K)) {
		
		current_stage = vic_stage;

	}
	


	Mesh points_mesh;
	if (Input::wasKeyPressed(SDL_SCANCODE_B)) {
		Vector3 character_center = captain.model.getTranslation() + Vector3(0, 1, 0);
		Vector3 coll;
		Vector3 collnorm;

		Vector3 target_direction = (motor.model.getTranslation() - (captain.model.getTranslation()));
		float ang =dot(captain.model.frontVector(), target_direction.normalize());
		
		
		if (abs(captain.model.getTranslation().x - motor.model.getTranslation().x) < 10 && abs(captain.model.getTranslation().z - motor.model.getTranslation().z) < 10 && ang >0.5) {
			Game::instance->modo_aguila_disponible = true;
			

			Game::instance->motor_cogido = true;
		
		}

		target_direction = (neumatic.model.getTranslation() - (captain.model.getTranslation()));
		ang = dot(captain.model.frontVector(), target_direction.normalize());
		if (abs(captain.model.getTranslation().x - neumatic.model.getTranslation().x) < 5 && abs(captain.model.getTranslation().z - neumatic.model.getTranslation().z) < 5 && ang > 0.5) {

			Game::instance->modo_aguila_disponible = true;
			Game::instance->neumatico_cogido = true;

		}
		target_direction = (chasis.model.getTranslation() - (captain.model.getTranslation()));
		ang = dot(captain.model.frontVector(), target_direction.normalize());
		
		if (abs(captain.model.getTranslation().x - chasis.model.getTranslation().x) < 13 && abs(captain.model.getTranslation().z - chasis.model.getTranslation().z) < 13 && ang > 0.5 && Game::instance->neumatico_cogido && Game::instance->motor_cogido) {
			
			current_stage = vic_stage;

		}


	}
	if (Game::instance->player_vidas == 0) {
		play_sound("data/audios/DunDunDunnn.wav", false);
		if (Game::instance->puntuacion > max_puntuacio) {
			
			if (Game::instance->puntuacion > levels_puntuacion[nivel][1]) {
				levels_puntuacion[nivel][1] = Game::instance->puntuacion;

				g.update_highscore("data/highscore.txt");
			}
	
		}
		current_stage = loser_stage;
	}

}

void EagleStage::update(float seconds_elapsed) {
	float speed = seconds_elapsed * 5; //the speed is defined by the seconds_elapsed so it goes constant
	aguila.update(speed);
	Game::instance->mouse_locked = true;
	if (Game::instance->mouse_locked)
		Input::centerMouse();
	if (tiempo_cumplido ==true) {
		current_stage = play_stage;
		primera = true;
		tiempo_cumplido = false;
	}
	
}
void InitStage::init(int window_width, int window_height) {
	nivel = 0;
	text_menu = Texture::Get("data/foto_menu_1_buena.png");
	text_menu2 = Texture::Get("data/welcome.png");
	text_menu3 = Texture::Get("data/foto_menu_1_buena.png");
	text_menu4 = Texture::Get("data/foto_menu_1_buena.png");
	text_menu5 = Texture::Get("data/foto_menu_1_buena.png");
	camera = new Camera();
	camera->lookAt(Vector3(0.f, 10.f, 10.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f, window_width / (float)window_height, 0.1f, 250.f); //set the projection, we want to be perspective
}

void InitStage::render(float time) {
	text_menu = Texture::Get("data/foto_menu_1_buena.png");
	text_menu2 = Texture::Get("data/welcome.png");
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	Mesh quad;
	play_sound("data/audios/IntroMusic.wav", true);
	quad.createQuad(0, 0, 2, 2, false);
	Shader* shader = Shader::Get("data/shaders/QUAD.vs", "data/shaders/texture.fs");
	shader->enable();
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture", text_menu);
	shader->setUniform("u_texture_tiling", 1.0f);
	quad.render(GL_TRIANGLES);



	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	Mesh quad2;
	quad2.createQuad(-0.1, -0.25, 0.75, 0.5, false);
	Shader* shader2 = Shader::Get("data/shaders/QUAD.vs", "data/shaders/texture.fs");
	shader2->enable();
	shader2->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader2->setUniform("u_texture", text_menu2);
	shader2->setUniform("u_texture_tiling", 1.0f);
	quad2.render(GL_TRIANGLES);
}

void InitStage::update(float seconds_elapsed) {
	if (Input::wasKeyPressed(SDL_SCANCODE_X)) {
		//stop_sound("data/audios/IntroMusic.wav", true);
		menu_stage->init(800, 600);
		current_stage = menu_stage;
		
	}
}

void LoadStage::init(int window_width, int window_height) {
	
	
}

void LoadStage::render(float time) {
	
	
	
	
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	Mesh quad;
	//play_sound("data/audios/IntroMusic.wav", true);
	
	

	quad.createQuad(0, 0, 2, 2, false);
	Shader* shader = Shader::Get("data/shaders/QUAD.vs", "data/shaders/flat.fs");
	shader->enable();


	shader->setUniform("u_color", Vector4(0.3, 0.3, 0.8, 1));




	
	shader->setUniform("u_texture_tiling", 1.0f);
	quad.render(GL_TRIANGLES);

	
	drawText(50, 50, "Deberas buscar una serie de componentes para formar un coche y " , Vector3(0, 0, 0), 2);
	drawText(50, 70, "escapar de unos zombies que te persiguen. Los zombies se mueven ", Vector3(0, 0, 0), 2);
	drawText(50, 90, "solo en la franja en la que han sido creados y una vez te alejas", Vector3(0, 0, 0), 2);
	drawText(50, 110, "de ellos se vuelven a su base. Puedes huir de ellos o atacarlos. ", Vector3(0, 0, 0), 2);
	drawText(50, 130, "Podras atacar cada ciertos segundos, asi que piensa en que momento " , Vector3(0, 0, 0), 2);
	drawText(50, 150, "atacar! Cada vez que un zombie te ataque, perderas 1 de vida ", Vector3(0, 0, 0), 2);
	drawText(50, 170, "(tienes 2 al principio) y te haras inmune durante 4 segundos. ", Vector3(0, 0, 0), 2);
	drawText(50, 190, "Empezaras con 100 puntos y recibiras 3 puntos por zombie muerto e ", Vector3(0, 0, 0), 2);
	drawText(50, 210, "iras perdiendo 1 punto cada 30 segundos. Empezaras con un modo", Vector3(0, 0, 0), 2);
	drawText(50, 230, "aguila. Al encontrar un componente del coche volveras a tenerlo,", Vector3(0, 0, 0), 2);
	drawText(50, 250, "el numero maximo de usos a la vez es 1, es decir, que si tienes", Vector3(0, 0, 0), 2);
	drawText(50, 270, "1 uso y encuentras un componente, seguira siendo de un uso, ", Vector3(0, 0, 0), 2);
	drawText(50, 290, "pero si tienes 0 usos, podras volverlo a utilizar.", Vector3(0, 0, 0), 2);
	drawText(50, 310, "El modo aguila dura 5 segundos", Vector3(0, 0, 0), 2);
	drawText(50, 330, "Hay en total 3 niveles que se tendras que pasar seguidos.", Vector3(0, 0, 0), 2);
	drawText(50, 350, "Cada nivel aumentara la velocidad de los zombies.", Vector3(0, 0, 0), 2);
	drawText(50, 370, "Para pasar el nivel, deberas formar el coche donde se situe el chasis.", Vector3(0, 0, 0), 2);
	drawText(50, 430, "PRESS N TO CONTINUE", Vector3(0, 0, 0), 2);
	
	
}
void LoadStage::update(float seconds_elapsed) {
	if (Input::wasKeyPressed(SDL_SCANCODE_N)) {
		//stop_sound("data/audios/IntroMusic.wav", true);
		current_stage = play_stage;
		
	}
	if (carga == true) {
		carga = false;
		play_stage->init(800, 600);
		g.loading_game("data/new_game.txt");

	}
	if (load == false) {
		load = true;
		g.loading_game("data/saved_game.txt");
	}
}

void LoseStage::render(float time) {

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	Mesh quad;
	//play_sound("data/audios/IntroMusic.wav", true);



	quad.createQuad(0, 0, 2, 2, false);
	Shader* shader = Shader::Get("data/shaders/QUAD.vs", "data/shaders/flat.fs");
	shader->enable();


	shader->setUniform("u_color", Vector4(1, 0, 0, 1));





	shader->setUniform("u_texture_tiling", 1.0f);
	quad.render(GL_TRIANGLES);


	drawText(50, 50, "You are a looser ", Vector3(1, 1, 1), 2);
	drawText(50, 90, "Puntos obtenidos: "+std::to_string(Game::instance->puntuacion), Vector3(1, 1, 1), 2);
	drawText(50, 130, "Intenta mejorar para la proxima vez!", Vector3(1, 1, 1), 2);
	
	drawText(50, 200, "PRESS R TO RETURN TO MENU", Vector3(1, 1, 1), 2);
	if (Input::isKeyPressed(SDL_SCANCODE_R)) {
		Game::instance->mouse_locked = false;
		SDL_ShowCursor(!Game::instance->mouse_locked);
		carga = true;
		current_stage = menu_stage;


	}

}


void ControlStage::render(float time) {





	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	Mesh quad;
	//play_sound("data/audios/IntroMusic.wav", true);



	quad.createQuad(0, 0, 2, 2, false);
	Shader* shader = Shader::Get("data/shaders/QUAD.vs", "data/shaders/flat.fs");
	shader->enable();


	shader->setUniform("u_color", Vector4(0.3, 0.3, 0.8, 1));





	shader->setUniform("u_texture_tiling", 1.0f);
	quad.render(GL_TRIANGLES);


	drawText(50, 50, "W --> Mover adelante ", Vector3(0, 0, 0), 2);
	drawText(50, 80, "S --> Mover atras ", Vector3(0, 0, 0), 2);
	drawText(50, 110, "D --> Mover derecha ", Vector3(0, 0, 0), 2);
	drawText(50, 140, "A --> Mover izquierda ", Vector3(0, 0, 0), 2);
	drawText(50, 170, "F --> Atacar ", Vector3(0, 0, 0), 2);
	drawText(50, 200, "B --> Coger Objetos ", Vector3(0, 0, 0), 2);
	drawText(50, 230, "1 --> Activar modo aguila ", Vector3(0, 0, 0), 2);
	drawText(50, 260, "M --> Menu ", Vector3(0, 0, 0), 2);

	drawText(50, 300, "PRESS R TO RETURN ", Vector3(0, 0, 0), 2);


}

void ControlStage::update(float seconds_elapsed) {
	if (Input::wasKeyPressed(SDL_SCANCODE_R)) {
		//stop_sound("data/audios/IntroMusic.wav", true);

		current_stage = menu_stage;

	}
	
}

void VictoryStage::render(float time) {





	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	Mesh quad;
	//play_sound("data/audios/IntroMusic.wav", true);


	
	quad.createQuad(0, 0, 2, 2, false);
	Shader* shader = Shader::Get("data/shaders/QUAD.vs", "data/shaders/flat.fs");
	shader->enable();


	shader->setUniform("u_color", Vector4(0, 1, 0, 1));





	shader->setUniform("u_texture_tiling", 1.0f);
	quad.render(GL_TRIANGLES);

		if (nivel < 2) {
			drawText(50, 50, "Nivel "+ std::to_string(nivel+1)+" superado", Vector3(1, 1, 1), 2);
			drawText(50, 90, "Puntos obtenidos: " + std::to_string(Game::instance->puntuacion), Vector3(1, 1, 1), 2);
			drawText(50, 130, "Intenta superar la puntuacion con un nivel de dificultad mas!", Vector3(1, 1, 1), 2);


			drawText(50, 200, "PRESS N FOR NEXT LEVEL", Vector3(1, 1, 1), 2);
			
			if (nivel == 0) {
				if (Game::instance->puntuacion > levels_puntuacion[nivel][1]) {
					levels_puntuacion[nivel][1] = Game::instance->puntuacion;
				}
				levels_puntuacion[nivel][0] = 1;
				
				g.update_highscore("data/highscore.txt");
				
				puntuacion_1 = Game::instance->puntuacion;
			}
			else if (nivel == 1) {
				if (Game::instance->puntuacion > levels_puntuacion[nivel][1]) {
					levels_puntuacion[nivel][1] = Game::instance->puntuacion;
				}
				levels_puntuacion[nivel][0] = 1;
				g.update_highscore("data/highscore.txt");
				puntuacion_2 = Game::instance->puntuacion;
			}

			if (Input::isKeyPressed(SDL_SCANCODE_N)) {
				
				Game::instance->zombie_speed += 10;
				nivel++;
				g.loading_game("data/new_game.txt");
				current_stage = play_stage;


			}
		}
		else {
			if (Game::instance->puntuacion > levels_puntuacion[nivel][1]) {
				levels_puntuacion[nivel][1] = Game::instance->puntuacion;
			}
			levels_puntuacion[nivel][0] = 1;
			g.update_highscore("data/highscore.txt");
			puntuacion_3 = Game::instance->puntuacion;
			drawText(50, 50, "Nivel ", Vector3(1, 1, 1), 2);
			drawText(50, 90, "Puntos obtenidos nivel 1: " + std::to_string(puntuacion_1), Vector3(1, 1, 1), 2);
			drawText(50, 130, "Puntos obtenidos nivel 2: " + std::to_string(puntuacion_2), Vector3(1, 1, 1), 2);
			drawText(50, 170, "Puntos obtenidos nivel 3: " + std::to_string(puntuacion_3), Vector3(1, 1, 1), 2);
			drawText(50, 210, "Intenta superar la puntuacion de los niveles la proxima!", Vector3(1, 1, 1), 2);


			drawText(50, 300, "PRESS N FOR GO TO MENU", Vector3(1, 1, 1), 2);
			if (Input::isKeyPressed(SDL_SCANCODE_N)) {
				
				Game::instance->mouse_locked = false;
				SDL_ShowCursor(!Game::instance->mouse_locked);
				carga = true;
				nivel = 0;
				current_stage = init_stage;


			}
		}
		

}


