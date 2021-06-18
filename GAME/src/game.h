/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H
#define ROW 256
#define COLUMN 256

#include "includes.h"
#include "camera.h"
#include "utils.h"

class Game
{
public:
	static Game* instance;

	//window
	SDL_Window* window;
	int window_width;
	int window_height;

	//some globals
	long frame;
	float time;
	float elapsed_time;
	int fps;
	

	float tiempo_ronda;
	double tiempo_total;
	int rondas_seguidas_perdidas_t=0;
	bool must_exit;
	std::vector<Matrix44> wall_model;
	std::vector<Matrix44> wall2_model;
	std::vector<int> wall2_angle;
	std::vector<Matrix44> entities;
	
	std::vector<Matrix44> terrorists;
	std::vector<bool> terrorists_alive;
	std::vector<Matrix44> antiterrorists;
	std::vector<bool> antiterrorists_alive;
	bool empezamos = false;
	
	std::vector<bool> antiterrorists_shoot;
	std::vector<bool> terrorists_shoot;
	
	std::vector<bool> antiterrorists_shoot_disp;
	std::vector<bool> terrorists_shoot_disp;

	std::vector<float> antiterrorists_shoot_time;
	std::vector<float> terrorists_shoot_time;

	std::vector<float> antiterrorists_move_time;
	std::vector<float> terrorists_move_time;

	std::vector<int> vida_t;
	std::vector<int> vida_ct;



	int num_terror_vivos = 5;
	int num_ct_vivos = 5;

	int num_rondas_ct = 0;
	int num_rondas_terror = 0;
	//seran 4 puntos que formaran un rectangulo
	std::vector<Vector3> zona_A;
	std::vector<Vector3> zona_MID;
	std::vector<Vector3> zona_B;
	
	//pensado en moverlo a sus propias clases
	int enemigos_zonas_terror[3];
	int enemigos_zonas_ct[3];
	std::vector<int> zona_correspondiente_t;
	std::vector<int> zona_correspondiente_ct;
	std::vector<bool> zona_segura_ct;
	std::vector<bool> zona_segura_t;
	float ang = 180 * DEG2RAD;
	
	std::vector<Vector3> spawn_ct;
	std::vector<Vector3> spawn_terror;
	std::vector<Vector3> destino_ct;
	std::vector<Vector3> pos_aux_ct;
	std::vector<Vector3> destino_terror;
	std::vector<Vector3> pos_aux_terror;
	std::vector<Vector3> primera_terror;
	std::vector<bool> primera_terror_hecha;
	std::vector<bool> segunda_terror_hecha;
	std::vector<Vector3> primera_ct;
	
	std::vector<Vector3> segunda_terror;
	std::vector<Vector3> bomba_a_posicion;
	std::vector<Vector3> bomba_b_posicion;
	std::vector<bool> avisados_terror;
	std::vector<bool> avisados_ct;
	
	std::vector<float> angulo_t;
	std::vector<float> angulo_ct;
	std::vector<bool> bomba_t;
	std::vector<bool> bomba_ct;
	bool bomba_cogida_t;
	bool ataque_bomba_a;
	bool ataque_bomba_b;
	bool bomba_planted;
	Vector3 bomb_position;

	bool cheats = true;
	//Image floor_image;
	std::vector<int**> list_of_matrix;
	
	int **mapa_general;
	
	// a-->b sin pasar por medio (safe)
	int **aTob_safe_terror;
	
	
	//b-->a sin pasar por medio (safe)
	int **bToa_safe_terror;
	
	std::vector<int> terror_auxiliar;
	std::vector<std::vector<Vector3>> first_auxiliar_points;
	
	std::vector<int> ct_auxiliar;
	std::vector<std::vector<Vector3>> first_auxiliar_points_ct;
	

	
	std::vector<std::vector<Vector3>> zonas;
	
	std::vector<Vector3> auxiliarpoints_a;
	std::vector<Vector3> auxiliarpoints_b;
	std::vector<Vector3> auxiliarpoints_mid;
	std::vector<Vector3> a_to_b_safe;
	std::vector<Vector3> b_to_a_safe;
	std::vector<Vector3> mid_to_a_safe;
	std::vector<Vector3> mid_to_b_safe;
	
	std::vector<std::vector<Vector3>> strategias;
	
	//4puntos que determinaran el area de plante
	std::vector<Vector3> zona_plante_a;
	
	std::vector<Vector3>zona_plante_b;
	
	bool a_planted;
	bool b_planted;
	
	//some vars
	Camera* camera; //our global camera
	bool mouse_locked; //tells if the mouse is locked (not seen)
	
	void change_spawn();
	Game(int window_width, int window_height, SDL_Window* window);

	//main functions
	void render(void);
	void update(double dt);

	//events
	void onKeyDown(SDL_KeyboardEvent event);
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown(SDL_MouseButtonEvent event);
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onMouseWheel(SDL_MouseWheelEvent event);
	void onGamepadButtonDown(SDL_JoyButtonEvent event);
	void onGamepadButtonUp(SDL_JoyButtonEvent event);
	void onResize(int width, int height);

};


#endif 
