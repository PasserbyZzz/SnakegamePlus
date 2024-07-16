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
int renderMenu();
std::string enterName();
void enterNamePair(std::string &t1, std::string &t2);

//��������
sf::Music* gameMusic(std::string music_name);
sf::Sound* gameSound(std::string sound_name);

#endif
