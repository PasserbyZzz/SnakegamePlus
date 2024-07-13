#ifndef COLOR_H
#define COLOR_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <string>

//绘制颜色
void initColors();

//绘制开始界面
sf::Vector2u getScreenResolution();
void displayImageWithText();
std::string enterName();

//播放音乐
sf::Music* gameMusic(std::string music_name);
sf::Sound* gameSound(std::string sound_name);

#endif
