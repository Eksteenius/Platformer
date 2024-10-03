
#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include "SFML/Graphics.hpp"
#include "vector"
#include "Vector2.h"
#include "GameObject.h"
#include "Dynamic.h"
#include "Map.h"

class Game
{
 public:
  Game(sf::RenderWindow& window);
  ~Game();
  bool init();
  void update(float dt);
  void render();
  void mouseClicked(sf::Event event);
  void keyPressed(sf::Event event);
  void keyReleased(sf::Event event);
  void inputUpdates(float dt);
  void toggleHandler();

  void renderScaledUp(Dynamic &_object, float _scale);

//  void handleEnemies(Dynamic &_object, float dt);

  bool boxCollisionCheck(sf::Sprite &sprite_a, sf::Sprite &sprite_b);
  void blockCollisions(Dynamic &_object);
  bool offscreenCheck(Dynamic &_object);
  void screenBoundHandler(Dynamic &_object);
  void offsetGameObjects(float _x, float _y);
  void offsetDynamics(float _x, float _y);

  void changeHealth(int amount);

  void resetInit();
  void resetMap();
  void resetInputs();
  void reset();

 private:
  int game_state = 0;
  bool game_over = false;
  bool victory = false;
  bool awaiting_input = false;
  float previous_time = 0;
  float game_time = 0;

  //std::vector<Item> items;

  float gravitational_constant = 490.f;
  //float gravitational_constant = 0.f;


  bool last_direction_right = true;

  int health = 6;
  int score = 0;
  bool invincible = false;

  const float player_bouncy = 0.25f;
  //const float player_bouncy = 0;
  //const float player_bouncy = 0.5f;
  //const float player_bouncy = 1;
  float player_max_move_speed = 200.f;


  float character_visual_scale = 1.6f;

  int enemy_count = 20;
  //int enemy_count = 0;
  float enemy_max_move_speed = 60.f;

  int coin_count = 40;
  //int coin_count = 0;
  int coins_on_screen = 40;
  int coins_collected = 0;
  const int coin_score_value = 50;

  int frame_count_dot = 0;
  int frame_count_invincibility = 0;
  int frame_count_enemy = 0;

  /// Dont need to be reset
  sf::RenderWindow& window;

  Map map;

  Dynamic player;
  sf::Texture player_texture;

  //Dynamic enemy;
  std::vector<Dynamic*> enemies;
  std::vector<int> active_enemies;
  std::vector<int> inactive_enemies;
  sf::Texture enemy_texture;

  //Dynamic coin;
  std::vector<Dynamic*> coins;
  std::vector<int> active_coins;
  std::vector<int> inactive_coins;
  sf::Texture coin_texture;


  GameObject tilemap;
  sf::Texture tilemap_texture;
  GameObject backgrounds;
  sf::Texture backgrounds_texture;
  GameObject characters;
  sf::Texture characters_texture;

  sf::Font font;
  sf::Text menu_text;
  sf::Text control_text;
  sf::Text instruction_text;
  sf::Text score_text;


  /// Inputs
  Vector2 cursor_position = Vector2(0,0);

  bool w_pressed = false;
  bool a_pressed = false;
  bool s_pressed = false;
  bool d_pressed = false;

  bool g_pressed = false;

  bool left_pressed = false;
  bool right_pressed = false;
  bool up_pressed = false;
  bool down_pressed = false;

  bool space_pressed = false;
  bool enter_pressed = false;


  bool g_toggled = false;

  bool space_toggled = false;
  bool enter_toggled = false;

  bool up_toggled = false;
  bool down_toggled = false;
  bool left_toggled = false;
  bool right_toggled = false;
};

#endif // PLATFORMER_GAME_H
