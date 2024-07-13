#ifndef COLOR_H
#define COLOR_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <string>

//������ɫ
void initColors();

//���ƿ�ʼ����
sf::Vector2u getScreenResolution();
void displayImageWithText();
std::string enterName();

//��������
sf::Music* gameMusic(std::string music_name);
sf::Sound* gameSound(std::string sound_name);

#endif
