
#include <iostream>
#include "GameObject.h"

GameObject::GameObject()
{
  sprite = new sf::Sprite();
}
GameObject::~GameObject()
{
  if(sprite != nullptr)
  {
    delete sprite;
    sprite = nullptr;
  }
}

bool GameObject::initSprite(sf::Texture &texture, const std::string& filename)
{
  if(!texture.loadFromFile(filename))
  {
    std::cout << "Error loading file: " << filename << "\n";
    return false;
  }
  sprite->setTexture(texture);
  return true;
}
sf::Sprite* GameObject::getSprite()
{
  return sprite;
}
