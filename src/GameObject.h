
#ifndef SPACEINVADERS_GAMEOBJECT_H
#define SPACEINVADERS_GAMEOBJECT_H

#include "SFML/Graphics.hpp"

class GameObject
{
 public:
  GameObject();
  ~GameObject();

  //bool exists = true;
  bool initSprite(sf::Texture &texture, const std::string& filename);
  sf::Sprite* getSprite();

 private:
  sf::Sprite* sprite = nullptr;
};


#endif // SPACEINVADERS_GAMEOBJECT_H
