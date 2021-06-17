#include "game.h"
//#include "Stage.h"
#include "Entity.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "IA.h"
#include <cmath>



//some globals
Floor plane;
Wall wall;
Wall2 wall2;
Captain captain;
T_Players t_players;
CT_Players ct_players;

Arma arma;
Mesh* mesh = NULL;
Texture* texture = NULL;
Shader* shader = NULL;
Animation* anim = NULL;
float angle = 0;
float mouse_speed = 100.0f;
FBO* fbo = NULL;
Vector3 target_pos;
Game* Game::instance = NULL;

bool bomb_aux=true;
extern Mesh* machete_mesh = NULL;
extern Texture* machete_texture = NULL;

void Game::change_spawn(){
	int size_ct=spawn_ct.size();
	int size_terror=spawn_terror.size();
	std::vector<Vector3> spawn_ct_aux;
	std::vector<Vector3> spawn_terror_aux;
	int size_a = spawn_ct.size();
	for(int i=0; i< size_a;i++){
		int v1 = rand() % size_ct; 
		size_ct--;
		
		spawn_ct_aux.push_back(spawn_ct[v1]);
		spawn_ct.erase(spawn_ct.begin() + v1);
	}
	for(int i=0; i<spawn_terror.size();i++){
		int v1 = rand() % size_terror; 
		size_terror--;
		spawn_terror_aux.push_back(spawn_terror[v1]);
	}
	
	spawn_ct=spawn_ct_aux;
	spawn_terror=spawn_terror_aux;
	
}

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;
	tiempo_ronda=120.0;
	tiempo_total=0.0;
	//OpenGL flags
	glEnable(GL_CULL_FACE); //render both sides of every triangle
	glEnable(GL_DEPTH_TEST); //check the occlusions using the Z buffer

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f, 100.f, 100.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f, window_width / (float)window_height, 0.1f, 10000.f); //set the projection, we want to be perspective

	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	texture = new Texture();
	texture->load("data/wall.png");



	// example of loading Mesh from Mesh Manager
	mesh = Mesh::Get("data/export.obj");

	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	machete_texture = new Texture();
	machete_texture = Texture::Get("data/m4/skin.png");
	
	machete_mesh = Mesh::Get("data/m4/M4A1.obj");


	
	plane = Floor(128);
	wall = Wall(128);
	wall2 = Wall2(true);
	captain = Captain("data/people/captain.tga", "data/people/captain.ASE");
	//captain = Captain("data/people/1.png", "data/people/soap.obj");
	//captain = Captain("data/people/captain.tga", "data/people/captain.ASE");
	t_players=T_Players("data/people/captain.tga", "data/people/captain.ASE");
	//ct_players=CT_Players("data/people/1.png", "data/people/soap.obj");
	ct_players=CT_Players("data/people/captain.tga", "data/people/captain.ASE");
	
	captain.arma = Arma("data/m4/skin.png", "data/m4/M4A1.obj");
	t_players.arma = Arma("data/m4/skin.png", "data/m4/M4A1.obj");
	ct_players.arma = Arma("data/m4/skin.png", "data/m4/M4A1.obj");
	bomba_planted = false;

	a_planted = false;
	b_planted = false;
	bomba_cogida_t=false;
	for(int i=0;i<5;i++){
		angulo_t.push_back(90 * DEG2RAD);
		angulo_ct.push_back(90.0 * DEG2RAD);
	
		
	}
	
	//initialize spawns
	
	spawn_ct.push_back(Vector3(-9,0,-122));
	//spawn_ct.push_back(Vector3(28, 0, 103));

	
	spawn_ct.push_back(Vector3(-20,0,-113));
	spawn_ct.push_back(Vector3(2,0,-107));
	spawn_ct.push_back(Vector3(-4,0,-97));
	spawn_ct.push_back(Vector3(-34,0,-115));
	
	spawn_terror.push_back(Vector3(18, 0, 103));
	
	spawn_terror.push_back(Vector3(28,0,103));
	spawn_terror.push_back(Vector3(38,0,113));
	spawn_terror.push_back(Vector3(56,0,88));
	spawn_terror.push_back(Vector3(76,0,100));
	
	//initializa first positions
	
	//primera_ct.push_back(Vector3(-56,0,-112));
	primera_ct.push_back(Vector3(-52,0,-7));
	//primera_ct.push_back(Vector3(-50,0,-88));
	primera_ct.push_back(Vector3(-63, 0, -35));
	//primera_ct.push_back(Vector3(40,0,-91));
	primera_ct.push_back(Vector3(1,0,-90));
	primera_ct.push_back(Vector3(78,0,-72));
	primera_ct.push_back(Vector3(120,0,-89));
	
	primera_terror.push_back(Vector3(-90, 0, 62));
	primera_terror.push_back(Vector3(-43,0,62));
	primera_terror.push_back(Vector3(44,0,62));
	primera_terror.push_back(Vector3(71,0,9));
	primera_terror.push_back(Vector3(98,0,22));
	
	segunda_terror.push_back(Vector3(-112, 0, 47));
	segunda_terror.push_back(Vector3(-56, 0, 6));
	segunda_terror.push_back(Vector3(44, 0, 62));
	segunda_terror.push_back(Vector3(107, 0, -25));
	segunda_terror.push_back(Vector3(116, 0, -8));
	
	bomba_b_posicion.push_back(Vector3(77, 0, -87));
	bomba_b_posicion.push_back(Vector3(79, 0, -108));
	bomba_b_posicion.push_back(Vector3(62, 0, -102));
	bomba_b_posicion.push_back(Vector3(60,0,-89));
	bomba_b_posicion.push_back(Vector3(70,0,-97));
	
	bomba_a_posicion.push_back(Vector3(-94, 0, 1));
	bomba_a_posicion.push_back(Vector3(-108, 0, 1));
	bomba_a_posicion.push_back(Vector3(-103, 0, -22));
	bomba_a_posicion.push_back(Vector3(-88,0,-24));
	bomba_a_posicion.push_back(Vector3(-107,0,-37));
	
	
	ataque_bomba_a = false;
	ataque_bomba_b = false;
	
	
	zona_plante_a.push_back(Vector3(-114, 0, 6));
	zona_plante_a.push_back(Vector3(-114, 0, -46));
	zona_plante_a.push_back(Vector3(-81, 0, 6));
	zona_plante_a.push_back(Vector3(-81, 0, -46));
	
	zona_plante_b.push_back(Vector3(54, 0, -81));
	zona_plante_b.push_back(Vector3(54, 0, -117));
	zona_plante_b.push_back(Vector3(84, 0, -81));
	zona_plante_b.push_back(Vector3(84, 0, -117));
	
	
	
	
	int enemigos_zonas_terror[3];
	int enemigos_zonas_ct[3];
	
	for(int i=0;i<3;i++){
		enemigos_zonas_terror[i]=0;
		enemigos_zonas_ct[i]=0;
	}
	
	
	//initialize teams
	
	
	antiterrorists.push_back(captain.model);
	antiterrorists_alive.push_back(true);
	destino_ct.push_back(Vector3(0,0,0));
	pos_aux_ct.push_back(Vector3(0,0,0));
	vida_ct.push_back(100);
	for(int i=0;i<4;i++){
		Captain captain2 = Captain("data/people/captain.tga", "data/people/captain.ASE");
		captain2.model.translate(spawn_ct[i+1][0], spawn_ct[i + 1][1], spawn_ct[i + 1][2]);
		
		antiterrorists.push_back(captain2.model);
		antiterrorists_alive.push_back(true);
		// hemos de saber que zona les toca
		destino_ct.push_back(Vector3(0,0,0)); 
		pos_aux_ct.push_back(Vector3(0,0,0));
		
		if(i==0){
			zona_correspondiente_ct.push_back(0);
		
		}
		else if(i==1){
			zona_correspondiente_ct.push_back(1);
		
		}
		else{
			zona_correspondiente_ct.push_back(2);
		
		}
		vida_ct.push_back(100);
	
	}
	for(int i=0;i<5;i++){
		Captain captain3 = Captain("data/people/captain.tga", "data/people/captain.ASE");
		//captain3.model.rotate(180, Vector3(0, 1, 0));
		
		terrorists.push_back(captain3.model);
		
		terrorists_alive.push_back(true);
		destino_terror.push_back(Vector3(0,0,0)); 
		pos_aux_terror.push_back(Vector3(0,0,0));
		if(i<2){
			zona_correspondiente_t.push_back(0);
		
		}
		else if(i==2){
			zona_correspondiente_t.push_back(1);
		
		}
		else{
			zona_correspondiente_t.push_back(2);
		
		}
		primera_terror_hecha.push_back(false);
		segunda_terror_hecha.push_back(false);
		avisados_terror.push_back(false);
		avisados_ct.push_back(false);
		vida_t.push_back(100);
		
		antiterrorists_shoot.push_back(false);
		terrorists_shoot.push_back(false);

		antiterrorists_shoot_disp.push_back(true);
		terrorists_shoot_disp.push_back(true);
		
		/*antiterrorists_shoot_disp.push_back(true);
		terrorists_shoot_disp.push_back(true);*/

		antiterrorists_shoot_time.push_back(0.0);
		terrorists_shoot_time.push_back(0.0);
		
		antiterrorists_move_time.push_back(0.0);
		terrorists_move_time.push_back(0.0);

		
		

	}
	
	
	//colocacion
	for(int i=0;i<terrorists.size();i++){
		if(i==0){
			captain.position=spawn_ct[i];
			
			captain.model.setTranslation(spawn_ct[i][0], spawn_ct[i][1], spawn_ct[i][2]);
			captain.model.scale(5.0f, 5.0f, 5.0f);
		}
		
		antiterrorists[i].setTranslation(spawn_ct[i][0], spawn_ct[i][1], spawn_ct[i][2]);
		terrorists[i].setTranslation(spawn_terror[i][0], spawn_terror[i][1], spawn_terror[i][2]);  
		terrorists[i].scale(5.0f, 5.0f, 5.0f);
		//antiterrorists[i].setIdentity();
		antiterrorists[i].scale(5.0f, 5.0f, 5.0f);
		
		//terrorists[i].setIdentity();
		

		pos_aux_ct[i]=spawn_ct[i];
		pos_aux_terror[i]=spawn_terror[i];
		
		//indicaremos que zonas han de ir cada uno
		
	}

	

	for(int i=0; i<3 ; i++){
		zona_segura_ct.push_back(true);
		zona_segura_t.push_back(false);
	
	}




	//PAREDES	
	
	for (int i = -85; i > -128; i--) {
		Matrix44 m;
		m.setIdentity();
		m.translate(-71, 0.0, i);
		//m.scale(6.0f, 6.0f, 6.0f);
		m.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
		wall2_model.push_back(m);
		wall2_angle.push_back(90);
	}
	for (int i = 42; i < 76; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(-62, 0.0, i);
		//m.scale(6.0f, 6.0f, 6.0f);
		m.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
		wall2_model.push_back(m);
		wall2_angle.push_back(90);
	}
	for (int i = -7; i < 77; i++) {
	//for (int i = -70; i < -2; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(-23, 0.0, i);
		//m.scale(6.0f, 6.0f, 6.0f);
		m.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
		wall2_model.push_back(m);
		wall2_angle.push_back(90);
	}
	for (int i = -54; i < -27; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(-23, 0.0, i);
		//m.scale(6.0f, 6.0f, 6.0f);
		m.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
		wall2_model.push_back(m);
		wall2_angle.push_back(90);
	}
	for (int i = -71; i < -55; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(-44, 0.0, i);
		//m.scale(6.0f, 6.0f, 6.0f);
		m.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
		wall2_model.push_back(m);
		wall2_angle.push_back(90);
	}
	for (int i = -128; i < -91; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(31, 0.0, i);
		//m.scale(6.0f, 6.0f, 6.0f);
		m.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
		wall2_model.push_back(m);
		wall2_angle.push_back(90);
	}
	for (int i = -117; i < -103; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(50, 0.0, i);
		//m.scale(6.0f, 6.0f, 6.0f);
		m.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
		wall2_model.push_back(m);
		wall2_angle.push_back(90);
	}
	for (int i = -89; i < -72; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(50, 0.0, i);
		//m.scale(6.0f, 6.0f, 6.0f);
		m.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
		wall2_model.push_back(m);
		wall2_angle.push_back(90);
	}
	for (int i = -72; i < -33; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(66, 0.0, i);
		//m.scale(6.0f, 6.0f, 6.0f);
		m.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
		wall2_model.push_back(m);
		wall2_angle.push_back(90);
	}
	for (int i = -98; i < -11; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(91, 0.0, i);
		//m.scale(6.0f, 6.0f, 6.0f);
		m.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
		wall2_model.push_back(m);
		wall2_angle.push_back(90);
	}
	for (int i = -11; i < 45; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(50, 0.0, i);
		//m.scale(6.0f, 6.0f, 6.0f);
		m.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
		wall2_model.push_back(m);
		wall2_angle.push_back(90);
	}
	for (int i = 9; i < 47; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(83, 0.0, i);
		//m.scale(6.0f, 6.0f, 6.0f);
		m.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
		wall2_model.push_back(m);
		wall2_angle.push_back(90);
	}

	//horizontales
	for (int i = -128; i < -71; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(i, 0.0, -85);
		
		wall2_model.push_back(m);
		wall2_angle.push_back(0);
	}
	for (int i = -88; i < -62; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(i, 0.0, 42);

		wall2_model.push_back(m);
		wall2_angle.push_back(0);
	}
	for (float i = -23; i < 103; i=i+0.1) {
		Matrix44 m;
		m.setIdentity();
		m.translate(i, 0.0, 77);

		wall2_model.push_back(m);
		wall2_angle.push_back(0);
	}
	for (int i = -41; i < -23; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(i, 0.0, 2);

		wall2_model.push_back(m);
		wall2_angle.push_back(0);
	}
	for (int i = -44; i < -7; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(i, 0.0, -71);

		wall2_model.push_back(m);
		wall2_angle.push_back(0);
	}
	for (int i = 11; i < 66; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(i, 0.0, -72);

		wall2_model.push_back(m);
		wall2_angle.push_back(0);
	}
	for (int i = 51; i < 91; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(i, 0.0, -11);

		wall2_model.push_back(m);
		wall2_angle.push_back(0);
	}
	for (int i = 103; i < 128; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(i, 0.0, 13);
		

		wall2_model.push_back(m);
		wall2_angle.push_back(0);
	}
	for (int i = 49; i < 66; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(i, 0.0, -33);

		wall2_model.push_back(m);
		wall2_angle.push_back(0);
	}
	for (int i = 93; i < 106; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(i, 0.0, -48);

		wall2_model.push_back(m);
		wall2_angle.push_back(0);
	}
	  for (int i = 117; i < 128; i++) {
		Matrix44 m;
		m.setIdentity();
		m.translate(i, 0.0, -48);

		wall2_model.push_back(m);
		wall2_angle.push_back(0);
	}


	mapa_general= new int* [ROW];
	for (int i = 0; i < ROW; i++) {
		mapa_general[i] = new int[COLUMN];
	}
	aTob_safe_terror = new int* [ROW];
	for (int i = 0; i < ROW; i++) {
		aTob_safe_terror[i] = new int[COLUMN];
	}
	bToa_safe_terror = new int* [ROW];
	for (int i = 0; i < ROW; i++) {
		bToa_safe_terror[i] = new int[COLUMN];
	}
	
	//INIT MATRICES A*


	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			mapa_general[i][j] = 0;
			aTob_safe_terror[i][j] = 0;
			bToa_safe_terror[i][j] = 0;

		}



	}
	/*
	std::fill(*mapa_general,*mapa_general+ ROW*COLUMN,1);
	
	std::fill(*aTob_safe_terror,*aTob_safe_terror+ ROW*COLUMN,1);
	
	std::fill(*bToa_safe_terror,*bToa_safe_terror+ ROW*COLUMN,1);
	*/
	
	
	
	for(int i=0;i<wall2_model.size();i++){
		int x=wall2_model[i].getTranslation().x + 128;
		int z= wall2_model[i].getTranslation().z + 128;
		/*std::cout << "posicion" << std::endl;
		std::cout << x << std::endl;
		
		std::cout << z << std::endl;
		std::cout << "ROW-COLUMS" << std::endl;
		std::cout << ROW << std::endl;

		std::cout << COLUMN << std::endl;*/

		mapa_general[x][z]=1;
		aTob_safe_terror[x][z] = 1;
		if (x < (ROW - 10) && x> 9) {
			mapa_general[x - 9][z] = 1;
			mapa_general[x + 9][z] = 1;
			aTob_safe_terror[x - 9][z] = 1;
			aTob_safe_terror[x + 9][z] = 1;
			

		}
		if (z < (ROW - 10) && z> 9) {
			mapa_general[x][z - 9] = 1;
			mapa_general[x][z + 9] = 1;
			aTob_safe_terror[x][z - 9] = 1;
			aTob_safe_terror[x][z + 9] = 1;


		}
		/*if (z < (ROW - 7) && z> 5 && x < (ROW - 7) && x> 5) {
			mapa_general[x-6][z - 6] = 1;
			mapa_general[x+6][z - 6] = 1;
			mapa_general[x+6][z + 6] = 1;
			mapa_general[x-6][z + 6] = 1;
			


		}*/
		
		
		
		bToa_safe_terror[x][z]=1;
		
	}
	
	for (int i = -70; i < -2; i++)
	 {
		aTob_safe_terror[-23+128][i+128] = 1;
		
		aTob_safe_terror[-23+128 - 9][i + 128] = 1;
		aTob_safe_terror[-23 + 128 + 9][i + 128] = 1;


		
		if (i + 128 < (ROW - 10) && i + 128 > 9) {
			aTob_safe_terror[-23 + 128][i + 128 - 9] = 1;
			aTob_safe_terror[-23 + 128][i + 128 + 9] = 1;


		}

	}


	
	list_of_matrix.push_back(mapa_general);
	list_of_matrix.push_back(aTob_safe_terror);
	list_of_matrix.push_back(bToa_safe_terror);
	
	
	std::vector<Vector3> zona_aux_A;
	zona_aux_A.push_back(Vector3(-128,0,-128));
	zona_aux_A.push_back(Vector3(-128,0,128));
	zona_aux_A.push_back(Vector3(-23,0,-128));
	zona_aux_A.push_back(Vector3(-23,0,128));
	
	std::vector<Vector3> zona_aux_MID;
	zona_aux_MID.push_back(Vector3(-24,0,-128));
	zona_aux_MID.push_back(Vector3(-24,0,128));
	zona_aux_MID.push_back(Vector3(50,0,-128));
	zona_aux_MID.push_back(Vector3(50,0,128));
	
	std::vector<Vector3> zona_aux_B;
	zona_aux_B.push_back(Vector3(51,0,-128));
	zona_aux_B.push_back(Vector3(51,0,128));
	zona_aux_B.push_back(Vector3(128,0,-128));
	zona_aux_B.push_back(Vector3(128,0,128));
	
	zonas.push_back(zona_aux_A);
	zonas.push_back(zona_aux_MID);
	zonas.push_back(zona_aux_B);
	
	
	
	for(int i=0; i<primera_terror.size(); i++){
		//std::cout << i << std::endl;
		//std::cout << "aa" << std::endl;
		Vector3 pos_aux = terrorists[i].getTranslation();
		pos_aux.x = pos_aux.x + 128;
		pos_aux.z = pos_aux.z + 128;

		Vector3 dest_aux = primera_terror[i];
		dest_aux.x = dest_aux.x + 128;
		dest_aux.z = dest_aux.z + 128;
		//std::cout << "vamos alla" << std::endl;
		//std::cout << i << std::endl;
		std::vector<Vector3> recorregut = aStarSearch(list_of_matrix[0], pos_aux, dest_aux);
		/*if (i == 0) {
			for (int i = 0; i < recorregut.size(); i++) {
				std::cout << "recorrido" << std::endl;
				std::cout << recorregut[i].x << std::endl;
				std::cout << recorregut[i].z << std::endl;


			}

		}*/
		terror_auxiliar.push_back(1);
		first_auxiliar_points.push_back(recorregut);
	
	}


	
	for (int i = 0; i < primera_ct.size(); i++) {
		/*std::cout << i << std::endl;
		std::cout << "aa" << std::endl;*/
		Vector3 pos_aux = antiterrorists[i].getTranslation();
		pos_aux.x = pos_aux.x + 128;
		pos_aux.z = pos_aux.z + 128;

		Vector3 dest_aux = primera_ct[i];
		dest_aux.x = dest_aux.x + 128;
		dest_aux.z = dest_aux.z + 128;
		/*std::cout << "vamos alla" << std::endl;
		std::cout << i << std::endl;*/
		std::vector<Vector3> recorregut = aStarSearch(list_of_matrix[0], pos_aux, dest_aux);
		/*if (i == 0) {
			for (int i = 0; i < recorregut.size(); i++) {
				std::cout << "recorrido" << std::endl;
				std::cout << recorregut[i].x << std::endl;
				std::cout << recorregut[i].z << std::endl;


			}

		}*/
		ct_auxiliar.push_back(1);
		first_auxiliar_points_ct.push_back(recorregut);

	}
	



	/*for (int i = 0; i < 20; i++) {
		std::cout << "AAAAAAAAAAAAAAAAAAAA" << std::endl;
		std::cout << primera_terror[0].x << std::endl;
		std::cout << primera_terror[0].z << std::endl;
		std::cout << "BBBBBBBBBBBBBBBB" << std::endl;
		std::cout << first_auxiliar_points[0][i].x << std::endl;
		std::cout << first_auxiliar_points[0][i].z << std::endl;


	}*/
	
	
	mouse_locked = true;
	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse*/
}

//what to do when the image has to be draw
void Game::render(void)
{


	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//create model matrix for cube
	/*Matrix44 m;
	m.rotate(angle * DEG2RAD, Vector3(0, 1, 0));
	
	if (shader)
	{*/
		//enable shader
	/*for (int i = 0; i < entities.size(); i++) {
		shader->enable();

		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_texture", texture);
		shader->setUniform("u_model", entities[i]);
		shader->setUniform("u_time", time);

		//do the draw call
		mesh->render(GL_TRIANGLES);

		//disable shader
		shader->disable();




		}*/
		
	//}
	captain.render(time);
	if (empezamos && antiterrorists_alive[0]) {
		captain.arma.render(time);
	}
	
	ct_players.render(time);
	plane.render_floor(time);
	wall.render_walls(time);
	
	wall2.render_walls2(time);

	
	t_players.render(time);
	//glDisable(GL_DEPTH_TEST);
	


	//
	/**/
	//render the FPS, Draw Calls, etc
	
	//drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);

	
	Mesh quad;
	quad.createQuad(0, 0.8, 0.4, 0.4 * (Game::instance->window_width / (float)Game::instance->window_width), false);
	Shader* shader = Shader::Get("data/shaders/QUAD.vs", "data/shaders/flat.fs");
	shader->enable();
	
	
	if (tiempo_ronda > 0) {
		if (bomba_planted) {
			shader->setUniform("u_color", Vector4(1, 0.5, 0, 1));
			quad.render(GL_TRIANGLES);
			if (a_planted) {
				drawText(Game::instance->window_width / 2 - 40, 90, "Bomba A", Vector3(1, 1, 1), 2);
			}
			else if (b_planted) {
				drawText(Game::instance->window_width / 2 - 40, 90, "Bomba B", Vector3(1, 1, 1), 2);
			}
		}
		else {
			shader->setUniform("u_color", Vector4(0, 1, 0, 1));
			quad.render(GL_TRIANGLES);
		}
		
		
	}
	else {
		shader->setUniform("u_color", Vector4(1, 0, 0, 1));
		quad.render(GL_TRIANGLES);

	}
	
	drawText(Game::instance->window_width/2 -30, 40, std::to_string((int)tiempo_ronda), Vector3(1, 1, 1), 4);
	
	Mesh quad1;
	quad1.createQuad(-0.4, 0.8, 0.4, 0.4 * (Game::instance->window_width / (float)Game::instance->window_width), false);
	Shader* shader1 = Shader::Get("data/shaders/QUAD.vs", "data/shaders/flat.fs");
	shader1->enable();

	shader1->setUniform("u_color", Vector4(0, 0, 1, 1));
	quad1.render(GL_TRIANGLES);
	
	drawText(Game::instance->window_width / 2 -230, 20, "Counter-Terrorists", Vector3(1, 1, 1), 1.5);
	drawText(Game::instance->window_width / 2 -170, 60, std::to_string(num_rondas_ct), Vector3(1, 1, 1), 4);


	
	Mesh quad2;
	quad2.createQuad(0.4, 0.8, 0.4, 0.4 * (Game::instance->window_width / (float)Game::instance->window_width), false);
	Shader* shader2 = Shader::Get("data/shaders/QUAD.vs", "data/shaders/flat.fs");
	shader2->enable();

	shader2->setUniform("u_color", Vector4(0, 0, 1, 1));
	quad2.render(GL_TRIANGLES);

	drawText(Game::instance->window_width / 2 +125, 20, "Terrorists", Vector3(1, 1, 1), 1.5);
	drawText(Game::instance->window_width / 2 + 150, 60, std::to_string(num_rondas_terror), Vector3(1, 1, 1), 4);

	//Texture* text = Texture::Get("data/assets/aa.png");

	//shader->setUniform("u_texture", text);
	shader->setUniform("u_texture_tiling", 1.0f);


	if (empezamos ) {
		if (antiterrorists_alive[0]) {
			Mesh quad3;
			quad3.createQuad(0, 0, 0.01, 0.05 * (Game::instance->window_width / (float)Game::instance->window_width), false);
			Shader* shader3 = Shader::Get("data/shaders/QUAD.vs", "data/shaders/flat.fs");
			shader3->enable();

			shader3->setUniform("u_color", Vector4(0, 0, 1, 1));
			quad3.render(GL_TRIANGLES);



			Mesh quad4;
			quad4.createQuad(0, -0.05, 0.01, 0.05 * (Game::instance->window_width / (float)Game::instance->window_width), false);
			Shader* shader4 = Shader::Get("data/shaders/QUAD.vs", "data/shaders/flat.fs");
			shader4->enable();

			shader4->setUniform("u_color", Vector4(0, 0, 1, 1));
			quad4.render(GL_TRIANGLES);

			Mesh quad5;
			quad5.createQuad(0.02, -0.025, 0.05 * (Game::instance->window_width / (float)Game::instance->window_width), 0.01, false);
			Shader* shader5 = Shader::Get("data/shaders/QUAD.vs", "data/shaders/flat.fs");
			shader5->enable();

			shader5->setUniform("u_color", Vector4(0, 0, 1, 1));
			quad5.render(GL_TRIANGLES);


			Mesh quad6;
			quad6.createQuad(-0.02, -0.025, 0.05 * (Game::instance->window_width / (float)Game::instance->window_width), 0.01, false);
			Shader* shader6 = Shader::Get("data/shaders/QUAD.vs", "data/shaders/flat.fs");
			shader6->enable();

			shader6->setUniform("u_color", Vector4(0, 0, 1, 1));
			quad6.render(GL_TRIANGLES);
		}
		
		Mesh quad8;


		quad8.createQuad(-0.65, -0.75, 0.25, 0.25, false);
		Shader* shader8 = Shader::Get("data/shaders/QUAD.vs", "data/shaders/flat.fs");
		shader8->enable();
		
		if (vida_ct[0] > 65) {

			//shader8->setUniform("u_color", Vector4(0, 1, 0, 1));
			
		}
		else if (vida_ct[0] > 30) {
			shader8->setUniform("u_color", Vector4(1, 0.5, 0, 1));
		}
		else {
			shader8->setUniform("u_color", Vector4(1, 0, 0, 1));
		}
		//shader8->setUniform("u_color", Vector4(0, 0, 1, 1));

		shader8->setUniform("u_texture_tiling", 1.0f);
		quad8.render(GL_TRIANGLES);
		if (vida_ct[0] > 0) {
			drawText(100, 510, std::to_string(vida_ct[0]), Vector3(1, 1, 1), 5);
		}
		else {
			drawText(100, 510, "0", Vector3(1, 1, 1), 5);
		}
		
		//drawText(Game::instance->window_width / 2 + 125, 20, std::to_string(vida_ct[0]), Vector3(1, 1, 1), 1.5);
	}
	

	if (Game::instance->antiterrorists_shoot[0]) {
		
		Mesh quad7;


		quad7.createQuad(0.65, -0.75, 0.25, 0.25, false);
		Shader* shader7 = Shader::Get("data/shaders/QUAD.vs", "data/shaders/texture.fs");
		shader7->enable();


		shader7->setUniform("u_color", Vector4(1, 1, 1, 1));

		Texture* text7 = Texture::Get("data/disparo.png");

		shader7->setUniform("u_texture", text7);
		shader7->setUniform("u_texture_tiling", 1.0f);
		quad7.render(GL_TRIANGLES);


		
		
		
		



	}
	
	
	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{	
	tiempo_total+=seconds_elapsed;
	float speed = seconds_elapsed * 30; //the speed is defined by the seconds_elapsed so it goes constant

	//example
	angle += (float)seconds_elapsed * 10.0f;
	//captain.update(speed);
	if (Input::isKeyPressed(SDL_SCANCODE_T)) {
		empezamos = true;

	}
	
	//std::cout << "tiempo" << std::endl;
	//std::cout << tiempo_ronda << std::endl;
	if (empezamos) {
		if (tiempo_ronda > 0) {
			captain.update(speed);
			t_players.update(speed);
			ct_players.update(speed);
			tiempo_ronda = tiempo_ronda - seconds_elapsed;
		}
		else if (tiempo_ronda > -5 && tiempo_ronda < 0) {
			//std::cout << "we are here" << std::endl;
			tiempo_ronda = tiempo_ronda - seconds_elapsed;

		}
		else if (tiempo_ronda > -15 && tiempo_ronda < -5) {
			tiempo_ronda = 120.0 ;
			ataque_bomba_a = false;
			ataque_bomba_b = false;
			bomba_planted = false;
			a_planted = false;
			b_planted = false;
			bomba_cogida_t = false;
			for (int i = 0; i < 3; i++) {
				enemigos_zonas_terror[i] = 0;
				enemigos_zonas_ct[i] = 0;
			}

			change_spawn();
			/*for (int i = 0; i < terrorists.size(); i++) {
				std::cout << "spawn" << std::endl;
				std::cout << spawn_ct[i].x << std::endl;
				std::cout << spawn_ct[i].y << std::endl;
				std::cout << spawn_ct[i].z << std::endl;


			}*/
			for (int i = 0; i < terrorists.size(); i++) {
				if (i == 0) {
					captain.position = spawn_ct[i];
					captain.model.setTranslation(spawn_ct[i][0], spawn_ct[i][1], spawn_ct[i][2]);
					captain.model.scale(5.0f, 5.0f, 5.0f);
				}
				antiterrorists[i].setTranslation(spawn_ct[i][0], spawn_ct[i][1], spawn_ct[i][2]);
				terrorists[i].setTranslation(spawn_terror[i][0], spawn_terror[i][1], spawn_terror[i][2]);
				terrorists[i].scale(5.0f, 5.0f, 5.0f);
				//antiterrorists[i].setIdentity();
				if (i != 0) {
					antiterrorists[i].scale(5.0f, 5.0f, 5.0f);
				}
				


				pos_aux_ct[i] = spawn_ct[i];
				pos_aux_terror[i] = spawn_terror[i];
				angulo_t[i]=90 * DEG2RAD;
				angulo_ct[i]=90 * DEG2RAD;
				ang = 180 * DEG2RAD;

				if (i < 2) {
					zona_correspondiente_t.push_back(0);
					zona_correspondiente_ct.push_back(0);

				}
				else if (i == 2) {
					zona_correspondiente_t.push_back(1);
					zona_correspondiente_ct.push_back(1);

				}
				else {
					zona_correspondiente_t.push_back(2);
					zona_correspondiente_ct.push_back(2);

				}
				
				primera_terror_hecha[i]=false;
				segunda_terror_hecha[i]=false;
				
				avisados_terror[i]=false;
				avisados_ct[i]=false;
				
				vida_t[i]= 100;
				vida_ct[i]= 100;
				
				antiterrorists_shoot[i] = false;
				terrorists_shoot[i] = false;

				antiterrorists_shoot_disp[i] = true;
				terrorists_shoot_disp[i] = true;

				antiterrorists_shoot_time[i] = 0.0;
				terrorists_shoot_time[i] = 0.0;
				
				antiterrorists_move_time[i] = 0.0;
				terrorists_move_time[i] = 0.0;

				terrorists_alive[i] = true;
				antiterrorists_alive[i] = true;

				terror_auxiliar[i]=1;
				ct_auxiliar[i]=1;


			}
			for (int i = 0; i < primera_terror.size(); i++) {
				
				Vector3 pos_aux = terrorists[i].getTranslation();
				pos_aux.x = pos_aux.x + 128;
				pos_aux.z = pos_aux.z + 128;
				Vector3 dest_aux;
				if (rondas_seguidas_perdidas_t == 3) {
					
					dest_aux = bomba_a_posicion[i];
					dest_aux.x = dest_aux.x + 128;
					dest_aux.z = dest_aux.z + 128;
				}
				else {
					dest_aux = primera_terror[i];
					dest_aux.x = dest_aux.x + 128;
					dest_aux.z = dest_aux.z + 128;
				}
				
				
				std::vector<Vector3> recorregut = aStarSearch(list_of_matrix[0], pos_aux, dest_aux);
				
				terror_auxiliar[i]=1;
				first_auxiliar_points[i]=recorregut;

			}



			for (int i = 0; i < primera_ct.size(); i++) {
				
				Vector3 pos_aux = antiterrorists[i].getTranslation();
				pos_aux.x = pos_aux.x + 128;
				pos_aux.z = pos_aux.z + 128;

				Vector3 dest_aux = primera_ct[i];
				dest_aux.x = dest_aux.x + 128;
				dest_aux.z = dest_aux.z + 128;

				std::vector<Vector3> recorregut = aStarSearch(list_of_matrix[0], pos_aux, dest_aux);
				
				ct_auxiliar[i]=1;
				first_auxiliar_points_ct[i]=recorregut;
				std::cout << "ct" << std::endl;
				std::cout << ct_auxiliar[i] << std::endl;
				std::cout << first_auxiliar_points_ct[i].size() << std::endl;


			}
			empezamos = false;

		}

	}
	
	
	int num_ct_vivos = 0;
	int num_t_vivos = 0;
	for (int i = 0; i < 5; i++) {
		if (terrorists_alive[i] == true) {
			num_t_vivos++;
		}
		if (antiterrorists_alive[i] == true) {
			num_ct_vivos++;
		}
	}

	if (num_ct_vivos == 0) {
		if (empezamos) {
			std::cout << "gana 111" << std::endl;
			//empezamos = false;
			tiempo_ronda = -6;
			num_rondas_terror++;
			rondas_seguidas_perdidas_t = 0;

		}
		
	}
	else if (num_t_vivos == 0 && bomba_planted == false) {
		if (empezamos) {
			std::cout << "gana 222" << std::endl;
			//empezamos = false;
			tiempo_ronda = -6;
			num_rondas_ct++;
			rondas_seguidas_perdidas_t++;

		}

	}
	else if (tiempo_ronda <= 0 && bomba_planted==false) {
		if (empezamos) {
			std::cout << "gana 333" << std::endl;
			//empezamos = false;
			tiempo_ronda = -6;
			num_rondas_ct++;
			rondas_seguidas_perdidas_t++;

		}

	}
	else if (tiempo_ronda <= 0 && bomba_planted) {
		if (empezamos) {
			std::cout << "gana 111" << std::endl;
			//empezamos = false;
			tiempo_ronda = -6;
			num_rondas_terror++;
			rondas_seguidas_perdidas_t = 0;

		}

	}

	
	
	/*
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) {
		
		
		
		FILE* f;
		f = fopen("paredes.txt", "w");
		
		for (int i = 0; i < wall2_model.size(); i++) {
			Vector3 wall_pos = (wall2_model[i] * Vector4(0.0, 0.0, 0.0, 1.0)).xyz;
			fprintf(f, "%f %f %f\n", wall_pos.x, wall_pos.z, wall2_angle[i]);


		}
		
		fclose(f);



	}
	
	if((a_planted || b_planted) && bomb_aux ){
		bomb_aux=false;
		//30 segundos para desplantar
		tiempo_ronda=30.0+seconds_elapsed;
	
	}*/
	//to navigate with the mouse fixed in the middle
	
	if (mouse_locked)
		Input::centerMouse();
		
}

//Keyboard event handler (sync input)
void Game::onKeyDown(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
	case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
	case SDLK_F1: Shader::ReloadAll(); break;
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	mouse_speed *= event.y > 0 ? 1.1 : 0.9;
}

void Game::onResize(int width, int height)
{
	std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport(0, 0, width, height);
	camera->aspect = width / (float)height;
	window_width = width;
	window_height = height;
}

