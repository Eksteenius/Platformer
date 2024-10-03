#include "Game.h"
#include <iostream>
#include <cmath>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(NULL));
}

Game::~Game()
{

}

bool Game::init()
{

  // Init Textures and Sprites
  backgrounds.initSprite(backgrounds_texture, "Data/Images/kenney_pixel-platformer/Tilemap/tilemap-backgrounds.png");
  tilemap.initSprite(tilemap_texture, "Data/Images/kenney_pixel-platformer/Tilemap/tilemap.png");
  map.initSprites(std::vector<sf::Sprite*>{backgrounds.getSprite(),tilemap.getSprite()});

  characters.initSprite(characters_texture, "Data/Images/kenney_pixel-platformer/Tilemap/tilemap-characters.png");
  //player.getSprite()->setTexture(*backgrounds.getSprite()->getTexture());
  player.getSprite()->setTexture(*characters.getSprite()->getTexture());
  //player_texture = characters_texture;

  player.getSprite()->setScale(1,1);
  player.getSprite()->setPosition(0, 0);
  map.setOffset(-(float)window.getView().getSize().x/5, -(float)window.getView().getSize().y/5);
  int other_tile_size = 24;
  //player.getSprite()->setTextureRect(sf::Rect(tile_size*4,tile_size*1,tile_size,tile_size));
  player.getSprite()->setTextureRect(sf::Rect((other_tile_size+1)*0,(other_tile_size+1)*0,other_tile_size,other_tile_size));
  player.getSprite()->setColor(sf::Color(255,255,255,255*1.f));
  player.setBounciness(player_bouncy);
  player.setMaxSpeed(1000.f);

  enemy_texture = characters_texture;
  enemy_texture.setSmooth(false);
  if(enemy_count > 0)
  {
    for(int i = 0; i < enemy_count; i++)
    {
      enemies.push_back(new Dynamic());
      enemies[i]->getSprite()->setTexture(enemy_texture);
      enemies[i]->getSprite()->setTextureRect(sf::Rect((other_tile_size+1)*6,(other_tile_size+1)*1,other_tile_size,other_tile_size));

      enemies[i]->getSprite()->setScale(1,1);
      enemies[i]->setVelocity(0,0);
      enemies[i]->setBounciness(0.25f);
      enemies[i]->setMaxSpeed(1000.f);
    }
  }


  /// Temporary
  if(!enemies.empty())
  {
    enemies[0]->getSprite()->setPosition((float)window.getView().getSize().x / 2, 0);
    active_enemies.push_back(0);
    enemies[1]->getSprite()->setPosition((float)window.getView().getSize().x / 3, 0);
    active_enemies.push_back(1);
  }

  coin_texture = tilemap_texture;
  coin_texture.setSmooth(false);
  int tile_size = 18;
  if(coin_count > 0)
  {
    for (int i = 0; i < coin_count; i++)
    {
      coins.push_back(new Dynamic());
      coins[i]->getSprite()->setTexture(coin_texture);
      coins[i]->getSprite()->setTextureRect(
        sf::Rect((tile_size + 1) * 11,(tile_size + 1) * 7, tile_size, tile_size));

      coins[i]->getSprite()->setScale(2, 2);
      if ((float)i < (float)coin_count / 2.f)
      {
        coins[i]->getSprite()->setPosition(
          (float)tile_size * coins[i]->getSprite()->getScale().x *
            (float)(i + 5),
          (float)tile_size * coins[i]->getSprite()->getScale().y * (float)0);
      }
      else
      {
        coins[i]->getSprite()->setPosition(
          (float)tile_size * coins[i]->getSprite()->getScale().x *
            ((float)i - ((float)coin_count / 2.f) + 5),
          (float)tile_size * coins[i]->getSprite()->getScale().y * (float)8);
      }
      coins[i]->setVelocity(0, 0);
      coins[i]->setBounciness(0.5f);
      coins[i]->setMaxSpeed(1000.f);

      if (i % 2 == 0)
      {
        coins[i]->setVelocityX(20);
      }
      else
      {
        coins[i]->setVelocityX(-20);
      }

      active_coins.push_back(i);
    }
  }


  // Init Fonts
  if(!font.loadFromFile("Data/Silkscreen/slkscr.ttf"))
  {
    std::cout << "font did not load \n";
    return false;
  }
  menu_text.setString("PLATFORMER GAME_");
  menu_text.setFont(font);
  menu_text.setCharacterSize(75);
  menu_text.setFillColor(sf::Color(255,255,255,255));
  menu_text.setPosition(40,(float)window.getView().getSize().y/2 - (75 + (75*0.5f) ));

  control_text.setString("WASD: TO MOVE | Space to Jump |\nHold W while Jumping to Go Higher |\nDEV TOOLS:\nG to Invert Gravity |");
  control_text.setFont(font);
  control_text.setCharacterSize(25);
  control_text.setFillColor(sf::Color(255,255,255,255));
  control_text.setPosition(40,(float)window.getView().getSize().y/1.75f - (25 + (25*0.5f) ));

  instruction_text.setString("PRESS ENTER TO START . . .");
  instruction_text.setFont(font);
  instruction_text.setCharacterSize(40);
  instruction_text.setFillColor(sf::Color(255,255,255,255));
  instruction_text.setPosition(40,(float)window.getView().getSize().y*0.8f - (40 + (40*0.5f) ));

  score_text.setString("0");
  score_text.setFont(font);
  score_text.setCharacterSize(40);
  score_text.setFillColor(sf::Color(255,255,255,255));
  score_text.setPosition((float)window.getView().getSize().x - 40,0);

  score_text.setFont(font);
  score_text.setCharacterSize(40);
  score_text.setFillColor(sf::Color(255,255,255,255));

  //Init Logic
  map.setOffset(0,0);
  map.setMap(std::vector<std::vector<std::vector<std::vector<int>>>>
             {
               {
                 {{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0}},
                 {{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0}},
                 {{0,1},{1,1},{2,1},{3,1},{0,1},{1,1},{2,1},{3,1},{0,1},{1,1},{2,1},{3,1},{0,1},{1,1},{2,1},{3,1},{0,1},{1,1},{2,1},{3,1},{0,1},{1,1},{2,1},{3,1},{0,1},{1,1},{2,1},{3,1},{0,1},{1,1},{2,1},{3,1}},
                 {{0,2},{1,2},{2,2},{3,2},{0,2},{1,2},{2,2},{3,2},{0,2},{1,2},{2,2},{3,2},{0,2},{1,2},{2,2},{3,2},{0,2},{1,2},{2,2},{3,2},{0,2},{1,2},{2,2},{3,2},{0,2},{1,2},{2,2},{3,2},{0,2},{1,2},{2,2},{3,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
               },
               {
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{ 0, 0}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 7, 2},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 7, 2},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 7, 2},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 7, 2},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 7, 2},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 7, 2},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 7, 2},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{ 0, 0}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
               },
               /// UI
               {
                 {{4,2},{4,2},{4,2}},
               },
             });

  return true;
}

void Game::update(float dt)
{
  inputUpdates(dt);
  toggleHandler();

  previous_time = game_time;
  game_time += 1*dt;

  switch (game_state)
  {
    case -10:
      reset();
      break;
    case 0:
      if(fmodf(game_time,1) < fmodf(previous_time,1))
      {
        if(frame_count_dot == 0)
        {
          instruction_text.setString("PRESS ENTER TO START . . .");
        }
        else if(frame_count_dot == 1)
        {
          instruction_text.setString("PRESS ENTER TO START .    ");
        }
        else if(frame_count_dot == 2)
        {
          instruction_text.setString("PRESS ENTER TO START . .  ");
        }
        else if(frame_count_dot == 3)
        {
          instruction_text.setString("PRESS ENTER TO START . . .");
        }
        else if(frame_count_dot == 4)
        {
          instruction_text.setString("PRESS ENTER TO START      ");
        }
        else if(frame_count_dot == 5)
        {
          instruction_text.setString("PRESS ENTER TO START . . .");
        }

        frame_count_dot ++;
        frame_count_dot = frame_count_dot % 6;
      }
      break;
    case 1:
      if(!game_over)
      {
        // player.getSprite()->setPosition(cursor_position.x, cursor_position.y);

        player.setVelocityY(player.getVelocity()->y + gravitational_constant * dt);
        player.getSprite()->move(player.getVelocity()->x * dt, player.getVelocity()->y * dt);
        screenBoundHandler(player);
        blockCollisions(player);
        if(invincible)
        {
          if(frame_count_invincibility == 2)
          {
            player.getSprite()->setColor(sf::Color(255,255,255,(int)(255*0.75f)));
          }
          else
          {
            player.getSprite()->setColor(sf::Color(255,255,255,(int)(255*0.5f)));
          }

          if(fmodf(game_time,1) < fmodf(previous_time,1))
          {
            frame_count_invincibility++;
            frame_count_invincibility = frame_count_invincibility % 3;
            if(frame_count_invincibility == 0)
            {
              invincible = false;
              player.getSprite()->setColor(sf::Color(255,255,255,255*1.f));
            }
          }
        }

        if(fmodf(game_time,1) < fmodf(previous_time,1))
        {
          frame_count_enemy++;
          frame_count_enemy = frame_count_enemy % 10;
        }
        if(!active_enemies.empty())
        {
          for (int i = 0; i < active_enemies.size(); i++)
          {
            enemies[active_enemies[i]]->setVelocityY(
              enemies[active_enemies[i]]->getVelocity()->y +
              gravitational_constant * dt);
            if (frame_count_enemy < 10 / 2)
            {
              if(enemies[active_enemies[i]]->getVelocity()->x < enemy_max_move_speed)
              {
                enemies[active_enemies[i]]->setVelocityX(
                  enemies[active_enemies[i]]->getVelocity()->x + 100 * dt);
              }
            }
            else
            {
              if(enemies[active_enemies[i]]->getVelocity()->x > -enemy_max_move_speed)
              {
                enemies[active_enemies[i]]->setVelocityX(
                  enemies[active_enemies[i]]->getVelocity()->x - 100 * dt);
              }
            }
            enemies[active_enemies[i]]->getSprite()->move(
              enemies[active_enemies[i]]->getVelocity()->x * dt,
              enemies[active_enemies[i]]->getVelocity()->y * dt);
            blockCollisions(*enemies[active_enemies[i]]);

            if (!invincible)
            {
              if (boxCollisionCheck(
                    *enemies[active_enemies[i]]->getSprite(),
                    *player.getSprite()))
              {
                changeHealth(-2);
                invincible = true;
              }
            }
            if(offscreenCheck(*enemies[active_enemies[i]]))
            {
              inactive_enemies.push_back(active_enemies[i]);
              active_enemies.erase(active_enemies.begin() + i);
              i--;
            }
          }
        }

        coins_on_screen = (int)active_coins.size();
        if(!active_coins.empty())
        {
          for (int i = 0; i < active_coins.size(); i++)
          {
            coins[active_coins[i]]->setVelocityY(
              coins[active_coins[i]]->getVelocity()->y +
              gravitational_constant * dt);
            coins[active_coins[i]]->getSprite()->move(
              coins[active_coins[i]]->getVelocity()->x * dt,
              coins[active_coins[i]]->getVelocity()->y * dt);
            blockCollisions(*coins[active_coins[i]]);
            if (offscreenCheck(*coins[active_coins[i]]))
            {
              coins_on_screen --;
            }
            else
            {
              if (boxCollisionCheck(
                    *coins[active_coins[i]]->getSprite(), *player.getSprite()))
              {
                coins_on_screen++;
                coins_collected += 1;
                score += coin_score_value;
                inactive_coins.push_back(active_coins[i]);
                active_coins.erase(active_coins.begin() + i);
                i--;
              }
            }
          }
        }
        if(coins_on_screen == 0)
        {
//          victory = true;
//          game_over = true;
        }
      }
      break;
    case 2:
      break;
    case 3:
      break;
    default:
      break;
  }
}

void Game::render()
{
  int tile_size = 32;
  switch (game_state)
  {
    case 0:
      window.draw(menu_text);
      window.draw(control_text);
      window.draw(instruction_text);
      break;
    case 1:
      map.render(window);

      //window.draw(*coins[0]->getSprite());
      if(!active_coins.empty())
      {
        for (int i = 0; i < active_coins.size(); i++)
        {
          window.draw(*coins[active_coins[i]]->getSprite());
        }
      }

      //window.draw(*player.getSprite());
      renderScaledUp(player,character_visual_scale);

      if(!active_enemies.empty())
      {
        for (int i = 0; i < active_enemies.size(); i++)
        {
          renderScaledUp(*enemies[active_enemies[i]], character_visual_scale);
        }
      }

      if(game_over)
      {
        if(victory)
        {
          menu_text.setString("YOU WIN");
        }
        else
        {
          menu_text.setString("GAME OVER");
        }
        window.draw(menu_text);
      }
      if(score != 0)
      {
        score_text.setPosition((float)window.getView().getSize().x + 20 - (floorf(log10f((float)score) + 1) * 40),0);
      }
      score_text.setFillColor(sf::Color::Black);
      score_text.setString(std::to_string(score));
      window.draw(score_text);

      map.renderUI(window);
      break;
    case 2:
      break;
    case 3:
      break;
    default:
      break;
  }
}
void Game::renderScaledUp(Dynamic& _object, float _scale)
{
  /// Renders the sprite larger without needing an additional hit box
  float scale_value;
  scale_value = _scale;
  float scale_x;
  scale_x = scale_value;
  float scale_y;
  scale_y = scale_value;
  _object.getSprite()->move(-_object.getSprite()->getGlobalBounds().width/4,-_object.getSprite()->getGlobalBounds().height/2);
  if(gravitational_constant < 0)
  {
    _object.getSprite()->move(0, _object.getSprite()->getGlobalBounds().height/2);
    scale_y = -scale_value;
    _object.getSprite()->setScale(scale_x,-scale_y);
    _object.getSprite()->move(0, _object.getSprite()->getGlobalBounds().height);
  }
  else
  {
    scale_y = scale_value;
    _object.getSprite()->setScale(scale_x,scale_y);
  }
  if(_object.getVelocity()->x < -5)
  {
    scale_x = scale_value;
    _object.getSprite()->setScale(scale_x,scale_y);
    window.draw(*_object.getSprite());

    if(&_object == &player)
    {
      last_direction_right = true;
    }
  }
  else if(_object.getVelocity()->x > 5)
  {
    scale_x = -scale_value;
    _object.getSprite()->setScale(scale_x,scale_y);
    _object.getSprite()->move(_object.getSprite()->getGlobalBounds().width,0);
    window.draw(*_object.getSprite());
    _object.getSprite()->move(-_object.getSprite()->getGlobalBounds().width,0);
    if(&_object == &player)
    {
      last_direction_right = false;
    }
  }
  else
  {
    if(last_direction_right)
    {
      _object.getSprite()->setScale(scale_value,scale_y);
      window.draw(*_object.getSprite());
    }
    else
    {
      _object.getSprite()->setScale(-scale_value,scale_y);
      _object.getSprite()->move(_object.getSprite()->getGlobalBounds().width,0);
      window.draw(*_object.getSprite());
      _object.getSprite()->move(-_object.getSprite()->getGlobalBounds().width,0);
    }
  }
  if(gravitational_constant < 0)
  {
    _object.getSprite()->move(0,-_object.getSprite()->getGlobalBounds().height);
  }
  _object.getSprite()->setScale(1,1);
  _object.getSprite()->move(_object.getSprite()->getGlobalBounds().width/4,_object.getSprite()->getGlobalBounds().height/2);
  if(gravitational_constant < 0)
  {
    _object.getSprite()->move(0, -_object.getSprite()->getGlobalBounds().height / 2);
  }
}

void Game::inputUpdates(float dt)
{
  switch (game_state)
  {
    case 0:
      if(enter_pressed)
      {
        if (!enter_toggled)
        {
          game_state += 1;
          resetInputs();
        }
      }
      break;
    case 1:
      if(!game_over)
      {
        if (w_pressed)
        {
          if(player.getVelocity()->y > -player_max_move_speed)
          {
            // player.setVelocityY(-180);
            player.setVelocityY(player.getVelocity()->y - 200 * dt);
          }
        }
        if (s_pressed)
        {
          if(player.getVelocity()->y < player_max_move_speed)
          {
            // player.setVelocityY(180);
            player.setVelocityY(player.getVelocity()->y + 200 * dt);
          }
        }
        if (d_pressed)
        {
          if(player.getVelocity()->x < player_max_move_speed)
          {
            // player.setVelocityX(180);
            player.setVelocityX(player.getVelocity()->x + 200 * dt);
          }
        }
        if (a_pressed)
        {
          if(player.getVelocity()->x > -player_max_move_speed)
          {
            // player.setVelocityX(-180);
            player.setVelocityX(player.getVelocity()->x - 200 * dt);
          }
        }
        if (g_pressed)
        {
          if (!g_toggled)
          {
            changeHealth(-1);
            gravitational_constant *= -1;
          }
        }
        if (space_pressed)
        {
          if (player.grounded)
          {
            if (gravitational_constant > 0)
            {
              // player.setVelocityY(-80);
              player.setVelocityY(player.getVelocity()->y - 300);
              player.grounded = false;
            }
            else if (gravitational_constant < 0)
            {
              player.setVelocityY(player.getVelocity()->y + 300);
              player.grounded = false;
            }
          }
        }
      }
      else
      {
        if (enter_pressed)
        {
          if (!enter_toggled)
          {
            game_state = -10;
            resetInputs();
          }
        }
      }
      break;
    case 2:
      break;
    case 3:
      break;
    default:
      break;
  }
  toggleHandler();
}

void Game::mouseClicked(sf::Event event)
{
  //get the click position
  //sf::Vector2i click = sf::Mouse::getPosition(window);

//  cursor_position = Vector2((float)sf::Mouse::getPosition(window).x,(float)sf::Mouse::getPosition(window).y);
//  enemies[active_enemies[0]]->getSprite()->setPosition(cursor_position.x,cursor_position.y);

  sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
  if (active_enemies.size() > 0)
  {
    enemies[active_enemies[0]]->getSprite()->setPosition(
      window.mapPixelToCoords(pixelPos).x, window.mapPixelToCoords(pixelPos).y);
  }
 }

void Game::keyPressed(sf::Event event)
{
  if (event.key.code == sf::Keyboard::W)
  {
    w_pressed = true;
  }
  else if (event.key.code == sf::Keyboard::A)
  {
    a_pressed = true;
  }
  else if (event.key.code == sf::Keyboard::S)
  {
    s_pressed = true;
  }
  else if (event.key.code == sf::Keyboard::D)
  {
    d_pressed = true;
  }

  else if (event.key.code == sf::Keyboard::G)
  {
    g_pressed = true;
  }

  else if (event.key.code == sf::Keyboard::Left)
  {
    left_pressed = true;
  }
  else if (event.key.code == sf::Keyboard::Right)
  {
    right_pressed = true;
  }
  else if (event.key.code == sf::Keyboard::Up)
  {
    up_pressed = true;
  }
  else if (event.key.code == sf::Keyboard::Down)
  {
    down_pressed = true;
  }
  else if(event.key.code == sf::Keyboard::Space)
  {
    space_pressed = true;
  }
  else if(event.key.code == sf::Keyboard::Enter)
  {
    enter_pressed = true;
  }
}

void Game::keyReleased(sf::Event event)
{
  if (event.key.code == sf::Keyboard::W)
  {
    w_pressed = false;
  }
  else if (event.key.code == sf::Keyboard::A)
  {
    a_pressed = false;
  }
  else if (event.key.code == sf::Keyboard::S)
  {
    s_pressed = false;
  }
  else if (event.key.code == sf::Keyboard::D)
  {
    d_pressed = false;
  }

  else if (event.key.code == sf::Keyboard::G)
  {
    g_pressed = false;
  }

  else if (event.key.code == sf::Keyboard::Left)
  {
    left_pressed = false;
  }
  else if (event.key.code == sf::Keyboard::Right)
  {
    right_pressed = false;
  }
  else if (event.key.code == sf::Keyboard::Up)
  {
    up_pressed = false;
  }
  else if (event.key.code == sf::Keyboard::Down)
  {
    down_pressed = false;
  }
  else if(event.key.code == sf::Keyboard::Space)
  {
    space_pressed = false;
  }
  else if(event.key.code == sf::Keyboard::Enter)
  {
    enter_pressed = false;
  }
}

void Game::toggleHandler()
{
  ///g
  if (g_pressed)
  {
    g_toggled = true;
  }
  else
  {
    if (g_toggled)
    {
      g_toggled = false;
    }
  }
  ///enter
  if (enter_pressed)
  {
    enter_toggled = true;
  }
  else
  {
    if (enter_toggled)
    {
      enter_toggled = false;
    }
  }
  ///space
  if (space_pressed)
  {
    space_toggled = true;
  }
  else
  {
    if (space_toggled)
    {
      space_toggled = false;
    }
  }
  ///up
  if (up_pressed)
  {
    up_toggled = true;
  }
  else
  {
    if (up_toggled)
    {
      up_toggled = false;
    }
  }
  ///down
  if (down_pressed)
  {
    down_toggled = true;
  }
  else
  {
    if (down_toggled)
    {
      down_toggled = false;
    }
  }
  ///left
  if (left_pressed)
  {
    left_toggled = true;
  }
  else
  {
    if (left_toggled)
    {
      left_toggled = false;
    }
  }
  ///right
  if (right_pressed)
  {
    right_toggled = true;
  }
  else
  {
    if (right_toggled)
    {
      right_toggled = false;
    }
  }
}

//void Game::handleEnemies(Dynamic& _object, float dt)
//{
//
//}

bool Game::boxCollisionCheck(sf::Sprite &sprite_a, sf::Sprite &sprite_b)
{
  if(sprite_a.getPosition().x < (sprite_b.getPosition().x + sprite_b.getGlobalBounds().width)
      && sprite_a.getPosition().x + sprite_a.getGlobalBounds().width > sprite_b.getPosition().x
      && sprite_a.getPosition().y < sprite_b.getPosition().y + sprite_b.getGlobalBounds().height
      && sprite_a.getPosition().y + sprite_a.getGlobalBounds().height > sprite_b.getPosition().y)
  {
    return true;
  }
  return false;
}
/*
 * The blockCollisions method works by taking the 4 corner points of the sprite
 * of the Dynamic& _object's sprite. Using these points it checks where these
 * points overlap on the grid of blocks (the map with the tile map).
 * by taking all the points and working out the coordinate of the square it
 * would overlap it then checks whether that coordinate is a solid block or not.
 * After all 4 coordinates are checked to see whether they "hit" a block or not
 * the collisions are then handled depending on how many of the points hit.
 * If two points hit the object must have hit from the common side:
 * for example if the bottom_left and bottom_right both hit, the hit must have
 * been from the bottom. What is then done is the _objects sprite is moved above
 * where the collision would have taken place and the velocity of the hit axis
 * is set to be the negative initial velocity * the bounce coefficient.
 * For times when only 1 corner point hits, or to diagonally opposite corners hit
 * to work out where the collision would have hit, the gradient of the movement of
 * the sprite is calculated using the velocity, and with this, the y intercept
 * is then calculated. if the y intercept is above, it hit the above edge, and
 * if below it hit the below edge. What these edges refer to is dependent on
 * which corner is hit and so each has a unique if statement.
 * The method also calculates whether the object is grounded or not for jumping.
 */
void Game::blockCollisions(Dynamic& _object)
{
  /// swapped x and y for (top and bottoms: left and right)
  /// swapped x and y for (map position and tile placements)
  /// swapped x and y for the mid checks
  if (
    _object.getSprite()->getPosition().x < (map.getOffset().x + (map.getSize(1).y * ((float)map.getTileSize() * map.getScale(1).x))) &&
    _object.getSprite()->getPosition().x + _object.getSprite()->getGlobalBounds().width > map.getOffset().x &&
    _object.getSprite()->getPosition().y < map.getOffset().y + (map.getSize(1).x * ((float)map.getTileSize() * map.getScale(1).x)) &&
    _object.getSprite()->getPosition().y + _object.getSprite()->getGlobalBounds().height > map.getOffset().y)
  {
    bool top_left_hit = false;
    int top_left_x    = 0;
    int top_left_y    = 0;

    bool top_right_hit = false;
    int top_right_x    = 0;
    int top_right_y    = 0;

    bool bottom_left_hit = false;
    int bottom_left_x    = 0;
    int bottom_left_y    = 0;

    bool bottom_right_hit = false;
    int bottom_right_x    = 0;
    int bottom_right_y    = 0;

    int horizontal_increment = 0;
    int vertical_increment = 0;

    top_left_y = std::floor(((_object.getSprite()->getPosition().x) - map.getOffset().x) / ((float)map.getTileSize() * map.getScale(1).x));
    top_left_x = std::floor(((_object.getSprite()->getPosition().y) - map.getOffset().y) / ((float)map.getTileSize() * map.getScale(1).x));
    while(!top_left_hit
           && (((float)vertical_increment * (float)map.getTileSize() * map.getScale(1).x) < _object.getSprite()->getGlobalBounds().width/2))
    {
      if(top_left_x + horizontal_increment <= (int)map.getSize(1).x - 1 && top_left_x + horizontal_increment >= 0)
      {
        if(top_left_y + vertical_increment <= (int)map.getSize(1).y - 1 && top_left_y + vertical_increment >= 0)
        {
          if(map.getTile(1, top_left_x + horizontal_increment, top_left_y + vertical_increment)[0] != -1)
          {
            top_left_hit = true;
          }
        }
      }
      horizontal_increment ++;
      if(((float)horizontal_increment * (float)map.getTileSize() * map.getScale(1).x) > _object.getSprite()->getGlobalBounds().height/2)
      {
        vertical_increment ++;
        horizontal_increment = 0;
      }
    }
    horizontal_increment = 0;
    vertical_increment = 0;

    top_right_y = std::floor(((_object.getSprite()->getPosition().x + _object.getSprite()->getGlobalBounds().width)
                              - map.getOffset().x) / ((float)map.getTileSize() * map.getScale(1).x));
    top_right_x = std::floor(((_object.getSprite()->getPosition().y) - map.getOffset().y) / ((float)map.getTileSize() * map.getScale(1).x));
    while(!top_right_hit
           && (((float)vertical_increment * (float)map.getTileSize() * map.getScale(1).x) < _object.getSprite()->getGlobalBounds().width/2))
    {
      if(top_right_x + horizontal_increment <= (int)map.getSize(1).x - 1 && top_right_x + horizontal_increment >= 0)
      {
        if(top_right_y - vertical_increment <= (int)map.getSize(1).y - 1 && top_right_y - vertical_increment >= 0)
        {
          if(map.getTile(1, top_right_x + horizontal_increment, top_right_y - vertical_increment)[0] != -1)
          {
            top_right_hit = true;
          }
        }
      }
      horizontal_increment ++;
      if(((float)horizontal_increment * (float)map.getTileSize() * map.getScale(1).x) > _object.getSprite()->getGlobalBounds().height/2)
      {
        vertical_increment ++;
        horizontal_increment = 0;
      }
    }
    horizontal_increment = 0;
    vertical_increment = 0;

    bottom_left_y = std::floor(((_object.getSprite()->getPosition().x) - map.getOffset().x) / ((float)map.getTileSize() * map.getScale(1).x));
    bottom_left_x = std::floor(((_object.getSprite()->getPosition().y + _object.getSprite()->getGlobalBounds().height)
                                - map.getOffset().y) / ((float)map.getTileSize() * map.getScale(1).x));
    while(!bottom_left_hit
           && (((float)vertical_increment * (float)map.getTileSize() * map.getScale(1).x) < _object.getSprite()->getGlobalBounds().width/2))
    {
      if(bottom_left_x - horizontal_increment <= (int)map.getSize(1).x - 1 && bottom_left_x - horizontal_increment >= 0)
      {
        if(bottom_left_y + vertical_increment <= (int)map.getSize(1).y - 1 && bottom_left_y + vertical_increment >= 0)
        {
          if(map.getTile(1, bottom_left_x - horizontal_increment, bottom_left_y + vertical_increment)[0] != -1)
          {
            bottom_left_hit = true;
          }
        }
      }
      horizontal_increment ++;
      if(((float)horizontal_increment * (float)map.getTileSize() * map.getScale(1).x) > _object.getSprite()->getGlobalBounds().height/2)
      {
        vertical_increment ++;
        horizontal_increment = 0;
      }
    }
    horizontal_increment = 0;
    vertical_increment = 0;

    bottom_right_y = std::floor(((_object.getSprite()->getPosition().x + _object.getSprite()->getGlobalBounds().width)
                                 - map.getOffset().x) / ((float)map.getTileSize() * map.getScale(1).x));
    bottom_right_x = std::floor(((_object.getSprite()->getPosition().y + _object.getSprite()->getGlobalBounds().height)
                                 - map.getOffset().y) / ((float)map.getTileSize() * map.getScale(1).x));
    while(!bottom_right_hit
           && (((float)vertical_increment * (float)map.getTileSize() * map.getScale(1).x) < _object.getSprite()->getGlobalBounds().width/2))
    {
      if(bottom_right_x - horizontal_increment <= (int)map.getSize(1).x - 1 && bottom_right_x - horizontal_increment >= 0)
      {
        if(bottom_right_y - vertical_increment <= (int)map.getSize(1).y - 1 && bottom_right_y - vertical_increment >= 0)
        {
          if(map.getTile(1, bottom_right_x - horizontal_increment, bottom_right_y - vertical_increment)[0] != -1)
          {
            bottom_right_hit = true;
          }
        }
      }
      horizontal_increment ++;
      if(((float)horizontal_increment * (float)map.getTileSize() * map.getScale(1).x) > _object.getSprite()->getGlobalBounds().height/2)
      {
        vertical_increment ++;
        horizontal_increment = 0;
      }
    }

    ///Mid Check

    int right_mid_coord_y = std::floor(((_object.getSprite()->getPosition().x + _object.getSprite()->getGlobalBounds().width) - map.getOffset().x)
                                       / ((float)map.getTileSize() * map.getScale(1).x));
    int right_mid_coord_x = std::floor(((_object.getSprite()->getPosition().y + _object.getSprite()->getGlobalBounds().height / 2) - map.getOffset().y)
                                       / ((float)map.getTileSize() * map.getScale(1).x));
    if(right_mid_coord_x <= (int)map.getSize(1).x - 1 && right_mid_coord_x >= 0)
    {
      if(right_mid_coord_y <= (int)map.getSize(1).y - 1 && right_mid_coord_y >= 0)
      {
        if(map.getTile(1, right_mid_coord_x, right_mid_coord_y)[0] != -1)
        {
          top_right_hit = true;
          bottom_right_hit = true;
        }
      }
    }


    int left_mid_coord_y = std::floor(((_object.getSprite()->getPosition().x) - map.getOffset().x)
                                      / ((float)map.getTileSize() * map.getScale(1).x));
    int left_mid_coord_x = std::floor(((_object.getSprite()->getPosition().y + _object.getSprite()->getGlobalBounds().height / 2) - map.getOffset().y)
                                      / ((float)map.getTileSize() * map.getScale(1).x));
    if(left_mid_coord_x <= (int)map.getSize(1).x - 1 && left_mid_coord_x >= 0)
    {
      if(left_mid_coord_y <= (int)map.getSize(1).y - 1 && left_mid_coord_y >= 0)
      {
        if(map.getTile(1, left_mid_coord_x, left_mid_coord_y)[0] != -1)
        {
          bottom_left_hit = true;
          top_left_hit = true;
        }
      }
    }

    int bottom_mid_coord_y = std::floor(((_object.getSprite()->getPosition().x + _object.getSprite()->getGlobalBounds().width / 2) -map.getOffset().x)
                                        / ((float)map.getTileSize() * map.getScale(1).x));
    int bottom_mid_coord_x = std::floor(((_object.getSprite()->getPosition().y + _object.getSprite()->getGlobalBounds().height) - map.getOffset().y)
                                        / ((float)map.getTileSize() * map.getScale(1).x));
    if(bottom_mid_coord_x <= (int)map.getSize(1).x - 1 && bottom_mid_coord_x >= 0)
    {
      if(bottom_mid_coord_y <= (int)map.getSize(1).y - 1 && bottom_mid_coord_y >= 0)
      {
        if(map.getTile(1, bottom_mid_coord_x, bottom_mid_coord_y)[0] != -1)
        {
          bottom_left_hit = true;
          bottom_right_hit = true;
        }
      }
    }

    int top_mid_coord_y = std::floor(((_object.getSprite()->getPosition().x + _object.getSprite()->getGlobalBounds().width / 2) - map.getOffset().x)
                                     / ((float)map.getTileSize() * map.getScale(1).x));
    int top_mid_coord_x = std::floor(((_object.getSprite()->getPosition().y) - map.getOffset().y) / ((float)map.getTileSize() * map.getScale(1).x));
    if(top_mid_coord_x <= (int)map.getSize(1).x - 1 && top_mid_coord_x >= 0)
    {
      if(top_mid_coord_y <= (int)map.getSize(1).y - 1 && top_mid_coord_y >= 0)
      {
        if(map.getTile(1, top_mid_coord_x, top_mid_coord_y)[0] != -1)
        {
          top_left_hit = true;
          top_right_hit = true;
        }
      }
    }


    _object.grounded = false;
    bool flip_x = false;
    bool flip_y = false;
    /// Do collisions
    if (top_right_hit && bottom_right_hit)
    {
      int hoz_mid_coord_x = std::floor(
        ((_object.getSprite()->getPosition().x +
          _object.getSprite()->getGlobalBounds().width) -
         map.getOffset().x) /
        ((float)map.getTileSize() * map.getScale(1).x));
      int hoz_mid_coord_y = std::floor(
        ((_object.getSprite()->getPosition().y +
          _object.getSprite()->getGlobalBounds().height / 2) -
         map.getOffset().y) /
        ((float)map.getTileSize() * map.getScale(1).x));
      _object.getSprite()->setPosition(
        -0.1f - _object.getSprite()->getGlobalBounds().width +
          map.getOffset().x +
          (float)hoz_mid_coord_x * (float)map.getTileSize() * map.getScale(1).x,
        _object.getSprite()->getPosition().y);
      flip_x = true;
    }
    if (top_left_hit && bottom_left_hit)
    {
      int hoz_mid_coord_x = std::floor(
        ((_object.getSprite()->getPosition().x) - map.getOffset().x) /
        ((float)map.getTileSize() * map.getScale(1).x));
      int hoz_mid_coord_y = std::floor(
        ((_object.getSprite()->getPosition().y +
          _object.getSprite()->getGlobalBounds().height / 2) -
         map.getOffset().y) /
        ((float)map.getTileSize() * map.getScale(1).x));
      _object.getSprite()->setPosition(
        0.1f + map.getOffset().x +
          (float)(hoz_mid_coord_x + 1) * (float)map.getTileSize() *
            map.getScale(1).x,
        _object.getSprite()->getPosition().y);
      flip_x = true;
    }
    if (bottom_left_hit && bottom_right_hit)
    {
      int vert_mid_coord_x = std::floor(
        ((_object.getSprite()->getPosition().x +
          _object.getSprite()->getGlobalBounds().width / 2) -
         map.getOffset().x) /
        ((float)map.getTileSize() * map.getScale(1).x));
      int vert_mid_coord_y = std::floor(
        ((_object.getSprite()->getPosition().y +
          _object.getSprite()->getGlobalBounds().height) -
         map.getOffset().y) /
        ((float)map.getTileSize() * map.getScale(1).x));
      _object.getSprite()->setPosition(
        _object.getSprite()->getPosition().x,
        -0.1f - _object.getSprite()->getGlobalBounds().height +
          map.getOffset().y +
          (float)(vert_mid_coord_y) * (float)map.getTileSize() *
            map.getScale(1).x);
      flip_y = true;
      if(gravitational_constant > 0)
      {
        _object.grounded = true;
      }
    }
    if (top_left_hit && top_right_hit)
    {
      int vert_mid_coord_x = std::floor(
        ((_object.getSprite()->getPosition().x +
          _object.getSprite()->getGlobalBounds().width / 2) -
         map.getOffset().x) /
        ((float)map.getTileSize() * map.getScale(1).x));
      int vert_mid_coord_y = std::floor(
        ((_object.getSprite()->getPosition().y) - map.getOffset().y) /
        ((float)map.getTileSize() * map.getScale(1).x));
      _object.getSprite()->setPosition(
        _object.getSprite()->getPosition().x,
        0.1f + map.getOffset().y +
          (float)(vert_mid_coord_y + 1) * (float)map.getTileSize() *
            map.getScale(1).x);
      flip_y = true;
      if(gravitational_constant < 0)
      {
        _object.grounded = true;
      }
    }
    // else
    {
      float m = (_object.getVelocity()->y / _object.getVelocity()->x);

      if (top_left_hit && !bottom_left_hit && !top_right_hit)
      {
        float c = _object.getSprite()->getPosition().y -
                  (m * _object.getSprite()->getPosition().x);
        float y_overlap = m * (map.getOffset().x +
                               ((float)top_left_y * (float)map.getTileSize() *
                                map.getScale(1).x) +
                               (float)map.getTileSize() * map.getScale(1).x) +
                          c;
        if (_object.getVelocity()->y < 0)
        {
          if (
            y_overlap > map.getOffset().y +
                          ((float)top_left_x * (float)map.getTileSize() *
                           map.getScale(1).x) +
                          (float)map.getTileSize() * map.getScale(1).x)
          {
            _object.getSprite()->setPosition(
              _object.getSprite()->getPosition().x,
              0.1f + map.getOffset().y +
                (float)(top_left_x + 1) * (float)map.getTileSize() *
                  map.getScale(1).x);
            flip_y = true;
            if(gravitational_constant < 0)
            {
              _object.grounded = true;
            }
          }
          else if (_object.getVelocity()->x < 0)
          {
            _object.getSprite()->setPosition(
              0.1f + map.getOffset().x +
                (float)(top_left_y + 1) * (float)map.getTileSize() *
                  map.getScale(1).x,
              _object.getSprite()->getPosition().y);
            flip_x = true;
          }
          else
          {
            _object.getSprite()->setPosition(
              _object.getSprite()->getPosition().x,
              0.1f + map.getOffset().y +
                (float)(top_left_x + 1) * (float)map.getTileSize() *
                  map.getScale(1).x);
            flip_y = true;
            if(gravitational_constant < 0)
            {
              _object.grounded = true;
            }
          }
        }
        else if (_object.getVelocity()->x < 0)
        {
          _object.getSprite()->setPosition(
            0.1f + map.getOffset().x +
              (float)(top_left_y + 1) * (float)map.getTileSize() *
                map.getScale(1).x,
            _object.getSprite()->getPosition().y);
          flip_x = true;
        }
      }
      if (top_right_hit && !top_left_hit && !bottom_right_hit)
      {
        float c = _object.getSprite()->getPosition().y -
                  (m * (_object.getSprite()->getPosition().x +
                        _object.getSprite()->getGlobalBounds().width));
        float y_overlap = m * (map.getOffset().x +
                               ((float)top_right_y * (float)map.getTileSize() *
                                map.getScale(1).x)) +
                          c;
        if (_object.getVelocity()->y < 0)
        {
          if (
            y_overlap > map.getOffset().y +
                          ((float)top_right_x * (float)map.getTileSize() *
                           map.getScale(1).x) +
                          (float)map.getTileSize() * map.getScale(1).x)
          {
            _object.getSprite()->setPosition(
              _object.getSprite()->getPosition().x,
              0.1f + map.getOffset().y +
                (float)(top_right_x + 1) * (float)map.getTileSize() *
                  map.getScale(1).x);
            flip_y = true;
            if(gravitational_constant < 0)
            {
              _object.grounded = true;
            }
          }
          else if (_object.getVelocity()->x > 0)
          {
            _object.getSprite()->setPosition(
              -0.1f - _object.getSprite()->getGlobalBounds().width +
                map.getOffset().x +
                (float)(top_right_y) * (float)map.getTileSize() *
                  map.getScale(1).x,
              _object.getSprite()->getPosition().y);
            flip_x = true;
          }
          else
          {
            _object.getSprite()->setPosition(
              _object.getSprite()->getPosition().x,
              0.1f + map.getOffset().y +
                (float)(top_right_x + 1) * (float)map.getTileSize() *
                  map.getScale(1).x);
            flip_y = true;
            if(gravitational_constant < 0)
            {
              _object.grounded = true;
            }
          }
        }
        else if (_object.getVelocity()->x > 0)
        {
          _object.getSprite()->setPosition(
            -0.1f - _object.getSprite()->getGlobalBounds().width +
              map.getOffset().x +
              (float)(top_right_y) * (float)map.getTileSize() *
                map.getScale(1).x,
            _object.getSprite()->getPosition().y);
          flip_x = true;
        }
      }
      if (bottom_left_hit && !bottom_right_hit && !top_left_hit)
      {
        float c = _object.getSprite()->getPosition().y +
                  _object.getSprite()->getGlobalBounds().height -
                  (m * (_object.getSprite()->getPosition().x));
        float y_overlap = m * ((map.getOffset().x + ((float)bottom_left_y *
                                                     (float)map.getTileSize() *
                                                     map.getScale(1).x)) +
                               (float)map.getTileSize() * map.getScale(1).x) +
                          c;
        if (_object.getVelocity()->y > 0)
        {
          if (y_overlap < map.getOffset().y +
                            ((float)bottom_left_x * (float)map.getTileSize()
                             * map.getScale(1).x))
          {
            _object.getSprite()->setPosition(
              _object.getSprite()->getPosition().x,
              -0.1f - _object.getSprite()->getGlobalBounds().height +
                map.getOffset().y +
                (float)(bottom_left_x) * (float)map.getTileSize() *
                  map.getScale(1).x);
            flip_y = true;
            if(gravitational_constant > 0)
            {
              _object.grounded = true;
            }
          }
          else if (_object.getVelocity()->x < 0)
          {
            _object.getSprite()->setPosition(
              0.1f + map.getOffset().x +
                (float)(bottom_left_y + 1) * (float)map.getTileSize() *
                  map.getScale(1).x,
              _object.getSprite()->getPosition().y);
            flip_x = true;
          }
          else
          {
            _object.getSprite()->setPosition(
              _object.getSprite()->getPosition().x,
              -0.1f - _object.getSprite()->getGlobalBounds().height +
                map.getOffset().y +
                (float)(bottom_left_x) * (float)map.getTileSize() *
                  map.getScale(1).x);
            flip_y = true;
            if(gravitational_constant > 0)
            {
              _object.grounded = true;
            }
          }
        }
        else if (_object.getVelocity()->x < 0)
        {
          _object.getSprite()->setPosition(
            0.1f + map.getOffset().x +
              (float)(bottom_left_y + 1) * (float)map.getTileSize() *
                map.getScale(1).x,
            _object.getSprite()->getPosition().y);
          flip_x = true;
        }
      }
      if (bottom_right_hit && !bottom_left_hit && !top_right_hit)
      {
        float c = _object.getSprite()->getPosition().y +
                  _object.getSprite()->getGlobalBounds().height -
                  (m * (_object.getSprite()->getPosition().x +
                        _object.getSprite()->getGlobalBounds().width));
        float y_overlap = m * (map.getOffset().x +
                               ((float)bottom_right_y *
                                (float)map.getTileSize() * map.getScale(1).x)) +
                          c;
        if (_object.getVelocity()->y > 0)
        {
          if (
            y_overlap <
            map.getOffset().y + ((float)bottom_right_x *
                                 (float)map.getTileSize() * map.getScale(1).x))
          {
            _object.getSprite()->setPosition(
              _object.getSprite()->getPosition().x,
              -0.1f - _object.getSprite()->getGlobalBounds().height +
                map.getOffset().y +
                (float)(bottom_right_x) * (float)map.getTileSize() *
                  map.getScale(1).x);
            flip_y = true;
            if(gravitational_constant > 0)
            {
              _object.grounded = true;
            }
          }
          else if (_object.getVelocity()->x > 0)
          {
            _object.getSprite()->setPosition(
              -0.1f - _object.getSprite()->getGlobalBounds().width +
                map.getOffset().x +
                (float)(bottom_right_y) * (float)map.getTileSize() *
                  map.getScale(1).x,
              _object.getSprite()->getPosition().y);
            flip_x = true;
          }
          else
          {
            _object.getSprite()->setPosition(
              _object.getSprite()->getPosition().x,
              -0.1f - _object.getSprite()->getGlobalBounds().height +
                map.getOffset().y +
                (float)(bottom_right_x) * (float)map.getTileSize() *
                  map.getScale(1).x);
            flip_y = true;
            if(gravitational_constant > 0)
            {
              _object.grounded = true;
            }
          }
        }
        else if (_object.getVelocity()->x > 0)
        {
          _object.getSprite()->setPosition(
            -0.1f - _object.getSprite()->getGlobalBounds().width +
              map.getOffset().x +
              (float)(bottom_right_y) * (float)map.getTileSize() *
                map.getScale(1).x,
            _object.getSprite()->getPosition().y);
          flip_x = true;
        }
      }
    }
    if(flip_x)
    {
      _object.setVelocityX(-_object.getVelocity()->x * (sqrtf(_object.getBounciness()) ));
    }
    if(flip_y)
    {
      _object.setVelocityY(-_object.getVelocity()->y * (sqrtf(_object.getBounciness()) ));
    }

  }
  else
  {
    _object.grounded = false;
  }
}
bool Game::offscreenCheck(Dynamic& _object)
{
  if(_object.getSprite()->getPosition().x + _object.getSprite()->getGlobalBounds().width < 0)
  {
    return true;
  }
  else if(_object.getSprite()->getPosition().x -_object.getSprite()->getGlobalBounds().width > (float)window.getView().getSize().x)
  {
    return true;
  }

  if(_object.getSprite()->getPosition().y + _object.getSprite()->getGlobalBounds().height < 0)
  {
    return true;
  }
  else if(_object.getSprite()->getPosition().y - _object.getSprite()->getGlobalBounds().height > (float)window.getView().getSize().y)
  {
    return true;
  }
  return false;
}

void Game::screenBoundHandler(Dynamic &_object)
{
  if(_object.getSprite()->getPosition().x < 0 + (float)window.getView().getSize().x/4.0f)
  {
    map.changeOffset(0+(float)window.getView().getSize().x/4.0f -_object.getSprite()->getPosition().x, 0);
    offsetDynamics(0+(float)window.getView().getSize().x/4.0f -_object.getSprite()->getPosition().x, 0);
    _object.getSprite()->setPosition(0 + (float)window.getView().getSize().x/4.0f, _object.getSprite()->getPosition().y);
    //_object.setVelocityX(0);
  }
  else if(_object.getSprite()->getPosition().x +_object.getSprite()->getGlobalBounds().width > (float)window.getView().getSize().x - (float)window.getView().getSize().x/4.0f)
  {
    map.changeOffset((float)window.getView().getSize().x-(float)window.getView().getSize().x/4.0f -_object.getSprite()->getGlobalBounds().width-_object.getSprite()->getPosition().x, 0);
    offsetDynamics((float)window.getView().getSize().x-(float)window.getView().getSize().x/4.0f -_object.getSprite()->getGlobalBounds().width-_object.getSprite()->getPosition().x, 0);
    _object.getSprite()->setPosition((float)window.getView().getSize().x - (float)window.getView().getSize().x/4.0f -_object.getSprite()->getGlobalBounds().width, _object.getSprite()->getPosition().y);//_object.setVelocityX(0);
  }

  if(_object.getSprite()->getPosition().y < 0 + (float)window.getView().getSize().y/4.0f)
  {
    map.changeOffset(0,0+(float)window.getView().getSize().y/4.0f -_object.getSprite()->getPosition().y);
    offsetDynamics(0,0+(float)window.getView().getSize().y/4.0f -_object.getSprite()->getPosition().y);
    _object.getSprite()->setPosition(_object.getSprite()->getPosition().x,0 + (float)window.getView().getSize().y/4.0f);
  }
  else if(_object.getSprite()->getPosition().y + _object.getSprite()->getGlobalBounds().height > (float)window.getView().getSize().y - (float)window.getView().getSize().y/4.0f)
  {
    map.changeOffset(0, (float)window.getView().getSize().y-(float)window.getView().getSize().y/4.0f -_object.getSprite()->getGlobalBounds().height-_object.getSprite()->getPosition().y);
    offsetDynamics(0, (float)window.getView().getSize().y-(float)window.getView().getSize().y/4.0f -_object.getSprite()->getGlobalBounds().height-_object.getSprite()->getPosition().y);
    _object.getSprite()->setPosition(_object.getSprite()->getPosition().x,(float)window.getView().getSize().y - (float)window.getView().getSize().y/4.0f - _object.getSprite()->getGlobalBounds().height);
  }
}
void Game::offsetGameObjects(float _x, float _y)
{

}
void Game::offsetDynamics(float _x, float _y)
{
  if(!enemies.empty())
  {
    for (int i = 0; i < enemies.size(); i++)
    {
      enemies[i]->getSprite()->setPosition(
        enemies[i]->getSprite()->getPosition().x + _x,
        enemies[i]->getSprite()->getPosition().y + _y);
    }
  }
  if(!coins.empty())
  {
    for (int i = 0; i < coins.size(); i++)
    {
      coins[i]->getSprite()->setPosition(
        coins[i]->getSprite()->getPosition().x + _x,
        coins[i]->getSprite()->getPosition().y + _y);
    }
  }
}

void Game::changeHealth(int amount)
{
  health += amount;
  if(health >= 6)
  {
    map.setTile(map.getLayerCount(),0,0,4,2);
    map.setTile(map.getLayerCount(),0,1,4,2);
    map.setTile(map.getLayerCount(),0,2,4,2);
  }
  else if(health == 5)
  {
    map.setTile(map.getLayerCount(),0,0,4,2);
    map.setTile(map.getLayerCount(),0,1,4,2);
    map.setTile(map.getLayerCount(),0,2,5,2);
  }
  else if(health == 4)
  {
    map.setTile(map.getLayerCount(),0,0,4,2);
    map.setTile(map.getLayerCount(),0,1,4,2);
    map.setTile(map.getLayerCount(),0,2,6,2);
  }
  else if(health == 3)
  {
    map.setTile(map.getLayerCount(),0,0,4,2);
    map.setTile(map.getLayerCount(),0,1,5,2);
    map.setTile(map.getLayerCount(),0,2,6,2);
  }
  else if(health == 2)
  {
    map.setTile(map.getLayerCount(),0,0,4,2);
    map.setTile(map.getLayerCount(),0,1,6,2);
    map.setTile(map.getLayerCount(),0,2,6,2);
  }
  else if(health == 1)
  {
    map.setTile(map.getLayerCount(),0,0,5,2);
    map.setTile(map.getLayerCount(),0,1,6,2);
    map.setTile(map.getLayerCount(),0,2,6,2);
  }
  else if(health <= 0)
  {
    map.setTile(map.getLayerCount(),0,0,6,2);
    map.setTile(map.getLayerCount(),0,1,6,2);
    map.setTile(map.getLayerCount(),0,2,6,2);
    /// GAME OVER
    //reset();
    game_over = true;
  }
}



void Game::reset()
{
  game_state = 0;
  game_over = false;
  victory = false;
  awaiting_input = false;
  previous_time = 0;
  game_time = 0;

  score = 0;
  health = 6;
  invincible = false;
  player.grounded = false;

  last_direction_right = true;
  gravitational_constant = fabsf(gravitational_constant);

  coin_count = 40;
  coins_on_screen = 40;
  coins_collected = 0;

  frame_count_dot = 0;
  frame_count_invincibility = 0;
  frame_count_enemy = 0;

  resetMap();
  resetInit();
  resetInputs();
}
void Game::resetMap()
{
  map.setOffset(0,0);
  map.setMap(std::vector<std::vector<std::vector<std::vector<int>>>>
             {
               {
                 {{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0}},
                 {{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0},{0,0},{1,0},{2,0},{3,0}},
                 {{0,1},{1,1},{2,1},{3,1},{0,1},{1,1},{2,1},{3,1},{0,1},{1,1},{2,1},{3,1},{0,1},{1,1},{2,1},{3,1},{0,1},{1,1},{2,1},{3,1},{0,1},{1,1},{2,1},{3,1},{0,1},{1,1},{2,1},{3,1},{0,1},{1,1},{2,1},{3,1}},
                 {{0,2},{1,2},{2,2},{3,2},{0,2},{1,2},{2,2},{3,2},{0,2},{1,2},{2,2},{3,2},{0,2},{1,2},{2,2},{3,2},{0,2},{1,2},{2,2},{3,2},{0,2},{1,2},{2,2},{3,2},{0,2},{1,2},{2,2},{3,2},{0,2},{1,2},{2,2},{3,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
                 {{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2},{0,2}},
               },
               {
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{ 0, 0}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 7, 2},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 7, 2},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 7, 2},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 7, 2},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 7, 2},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 7, 2},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 7, 2},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{ 0, 0}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{ 0, 0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
                 {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
               },
               /// UI
               {
                 {{4,2},{4,2},{4,2}},
               },
             });
}
void Game::resetInit()
{
  player.getSprite()->setScale(1,1);
  player.getSprite()->setPosition(0,0);
  player.getSprite()->setColor(sf::Color(255,255,255,255*1.f));
  player.setVelocity(0,0);
  player.setBounciness(player_bouncy);

  menu_text.setString("PLATFORMER GAME_");
  menu_text.setFont(font);
  menu_text.setCharacterSize(75);
  menu_text.setFillColor(sf::Color(255,255,255,255));
  menu_text.setPosition(40,(float)window.getView().getSize().y/2 - (75 + (75*0.5f) ));

  control_text.setString("WASD: TO MOVE | Space to Jump |\nHold W while Jumping to Go Higher |\nDEV TOOLS:\nG to Invert Gravity |");
  control_text.setFont(font);
  control_text.setCharacterSize(25);
  control_text.setFillColor(sf::Color(255,255,255,255));
  control_text.setPosition(40,(float)window.getView().getSize().y/1.75f - (25 + (25*0.5f) ));

  instruction_text.setString("PRESS ENTER TO START . . .");
  instruction_text.setFont(font);
  instruction_text.setCharacterSize(40);
  instruction_text.setFillColor(sf::Color(255,255,255,255));
  instruction_text.setPosition(40,(float)window.getView().getSize().y/1.25f - (40 + (40*0.5f) ));

  score_text.setFont(font);
  score_text.setCharacterSize(40);
  score_text.setFillColor(sf::Color(255,255,255,255));

  active_enemies.clear();
  inactive_enemies.clear();
  int other_tile_size = 24;
  if(enemy_count > 0)
  {
    for (int i = 0; i < enemy_count; i++)
    {
      enemies[i]->getSprite()->setScale(1, 1);

      enemies[i]->setVelocity(0, 0);
      enemies[i]->setBounciness(0.25f);
      enemies[i]->setMaxSpeed(1000.f);
    }
  }
  /// Temporary
  enemies[0]->getSprite()->setPosition((float)window.getView().getSize().x/2,0);
  active_enemies.push_back(0);
  enemies[1]->getSprite()->setPosition((float)window.getView().getSize().x/3,0);
  active_enemies.push_back(1);

  active_coins.clear();
  inactive_coins.clear();
  int tile_size = 18;
  if(coin_count > 0)
  {
    for (int i = 0; i < coin_count; i++)
    {
      coins[i]->getSprite()->setScale(2, 2);
      if ((float)i < (float)coin_count / 2.f)
      {
        coins[i]->getSprite()->setPosition(
          (float)tile_size * coins[i]->getSprite()->getScale().x *
            (float)(i + 5),
          (float)tile_size * coins[i]->getSprite()->getScale().y * (float)0);
      }
      else
      {
        coins[i]->getSprite()->setPosition(
          (float)tile_size * coins[i]->getSprite()->getScale().x *
            ((float)i - ((float)coin_count / 2.f) + 5),
          (float)tile_size * coins[i]->getSprite()->getScale().y * (float)8);
      }
      coins[i]->setVelocity(0, 0);
      coins[i]->setBounciness(0.5f);
      coins[i]->setMaxSpeed(1000.f);

      if (i % 2 == 0)
      {
        coins[i]->setVelocityX(20);
      }
      else
      {
        coins[i]->setVelocityX(-20);
      }

      active_coins.push_back(i);
    }
  }
}
void Game::resetInputs()
{
  w_pressed = false;
  a_pressed = false;
  s_pressed = false;
  d_pressed = false;

  g_pressed = false;

  left_pressed = false;
  right_pressed = false;
  up_pressed = false;
  down_pressed = false;

  space_pressed = false;
  enter_pressed = false;


  g_toggled = false;

  space_toggled = false;
  enter_toggled = false;

  up_toggled = false;
  down_toggled = false;
  left_toggled = false;
  right_toggled = false;
}

//if (top_right_hit && bottom_right_hit)
//{
//  _object.getSprite()->setPosition(-0.1f - _object.getSprite()->getGlobalBounds().width + map.getOffset().x + (float)right_mid_coord_x * (float)map.getTileSize() * map.getScale(1).x,
//                                   _object.getSprite()->getPosition().y);
//  flip_x = true;
//}
//if (top_left_hit && bottom_left_hit)
//{
//  _object.getSprite()->setPosition(0.1f + map.getOffset().x + (float)(left_mid_coord_x + 1) * (float)map.getTileSize() * map.getScale(1).x,
//                                   _object.getSprite()->getPosition().y);
//  flip_x = true;
//}
//if (bottom_left_hit && bottom_right_hit)
//{
//  _object.getSprite()->setPosition(_object.getSprite()->getPosition().x,
//                                   -0.1f - _object.getSprite()->getGlobalBounds().height + map.getOffset().y + (float)(bottom_mid_coord_x) * (float)map.getTileSize() * map.getScale(1).x);
//  flip_y = true;
//  if(gravitational_constant > 0)
//  {
//    _object.grounded = true;
//  }
//}
//if (top_left_hit && top_right_hit)
//{
//  _object.getSprite()->setPosition(_object.getSprite()->getPosition().x,
//                                   0.1f + map.getOffset().y + (float)(top_mid_coord_y + 1) * (float)map.getTileSize() * map.getScale(1).x);
//  flip_y = true;
//  if(gravitational_constant < 0)
//  {
//    _object.grounded = true;
//  }
//}