#include "color.h"
#include "curses.h"

#include <windows.h>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>

//����������ɫ
void initColors() {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  // ��ɫ
    init_pair(2, COLOR_GREEN, COLOR_BLACK);  // ��ɫ
    init_pair(3, COLOR_BLUE, COLOR_BLACK);   //��ɫ
    init_pair(4, COLOR_RED, COLOR_BLACK);    //��ɫ
    init_pair(5,COLOR_YELLOW, COLOR_BLACK); //��ɫ
    init_pair(6, COLOR_WHITE, COLOR_WHITE); //�׵װ�ɫ
    init_pair(7, COLOR_BLUE, COLOR_BLUE); //������ɫ
    init_pair(8, COLOR_MAGENTA, COLOR_BLACK); //��ֵ1ʳ��
    init_pair(9, COLOR_RED, COLOR_BLACK); //��ֵ2ʳ��
    init_pair(10, COLOR_YELLOW, COLOR_BLACK); //��ֵ3ʳ��
    init_pair(11, COLOR_BLUE, COLOR_BLACK); //��ֵ5ʳ��
}

// ��ȡ��Ļ�ֱ���
sf::Vector2u getScreenResolution() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Get Screen Resolution", sf::Style::None);
    sf::Vector2u screenSize = window.getSize();
    window.close();
    return screenSize;
}

//��ӡ����ͼƬ������
void displayImageWithText() {
    // ��ȡ��Ļ�ֱ���
    sf::Vector2u screenSize = getScreenResolution();

    // ����һ��ȫ������
    sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), "SnakeGame", sf::Style::Fullscreen);

    // ���ر���ͼ��
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.jpg")) {
        std::cerr << "Failed to load background image" << std::endl;
        return;
    }

    // �������鲢��������
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    // ��ȡ����ͼ���С
    sf::Vector2u textureSize = backgroundTexture.getSize();

    // �������ű���
    float scaleX = static_cast<float>(screenSize.x) / textureSize.x;
    float scaleY = static_cast<float>(screenSize.y) / textureSize.y;

    // ���þ�������ű�����ʹ��������������
    backgroundSprite.setScale(scaleX, scaleY);

    // ��������
    sf::Font font;
    if (!font.loadFromFile("PixelOperator8-Bold.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return;
    }

    // �����ı�����
    sf::Text text;
    text.setFont(font);
    text.setString("Press Any Key to START");
    text.setCharacterSize(80); // �ַ���С
    text.setFillColor(sf::Color::White); // ������ɫ
    text.setPosition(500, 1460); // �ı�λ��

    // ������˸��ʱ��
    sf::Clock clock;
    bool showText = true;
    float blinkInterval = 0.5f; // ��˸������룩

    // ��ѭ��
    while (window.isOpen()) {
        // �����¼�
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // ��������˳�
            if (event.type == sf::Event::KeyPressed)
                window.close();
        }

        // ������˸״̬
        if (clock.getElapsedTime().asSeconds() >= blinkInterval) {
            showText = !showText;
            clock.restart();
        }

        // ��մ���
        window.clear();

        // ���Ʊ���ͼ��
        window.draw(backgroundSprite);

        // �����ı�
        if (showText) {
            window.draw(text);
        }

        // ��ʾ����
        window.display();
    }
}

int renderMenu()
{
    sf::Sound* switchsound = gameSound("light_switch.wav");

    // ��ʼ��curses��
    initscr();
    noecho();  // ��ֹ����
    cbreak();  // ��ֹ�л���
    keypad(stdscr, TRUE); // ���ù��ܼ�
    curs_set(0); // ���ù��

    std::vector<std::string> menuItems = {"Single", "Pair", "Fun", "Quit"};

    attron(A_STANDOUT);
    mvprintw(LINES / 2 - 2, (COLS - strlen(menuItems[0].c_str())) / 2, menuItems[0].c_str());
    attroff(A_STANDOUT);

    mvprintw(LINES / 2 - 1, (COLS - strlen(menuItems[1].c_str())) / 2, menuItems[1].c_str());

    mvprintw(LINES / 2 - 0, (COLS - strlen(menuItems[2].c_str())) / 2, menuItems[2].c_str());

    mvprintw(LINES / 2 + 1, (COLS - strlen(menuItems[3].c_str())) / 2, menuItems[3].c_str());

    refresh();

    int index = 0;
    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                switchsound->play();
                mvprintw(LINES / 2 + index - 2, (COLS - strlen(menuItems[index].c_str())) / 2, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                attron(A_STANDOUT);
                mvprintw(LINES / 2 + index - 2, (COLS - strlen(menuItems[index].c_str())) / 2, menuItems[index].c_str());
                attroff(A_STANDOUT);
                int index_1 = (index - 1 < 0) ? menuItems.size() - 1 : index - 1;
                int index_2 = (index_1 - 1 < 0) ? menuItems.size() - 1 : index_1 - 1;
                mvprintw(LINES / 2 + index_1 - 2, (COLS - strlen(menuItems[index_1].c_str())) / 2, menuItems[index_1].c_str());
                mvprintw(LINES / 2 + index_2 - 2, (COLS - strlen(menuItems[index_2].c_str())) / 2, menuItems[index_2].c_str());
                break;
            }

            case 'S':
            case 's':
            case KEY_DOWN:
            {
                switchsound->play();
                mvprintw(LINES / 2 + index - 2, (COLS - strlen(menuItems[index].c_str())) / 2, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                attron(A_STANDOUT);
                mvprintw(LINES / 2 + (index - 2), (COLS - strlen(menuItems[index].c_str())) / 2, menuItems[index].c_str());
                attroff(A_STANDOUT);
                int index_1 = (index + 1 > menuItems.size() - 1) ? 0 : index + 1;
                int index_2 = (index_1 + 1 > menuItems.size() - 1) ? 0 : index_1 + 1;
                mvprintw(LINES / 2 + index_1 - 2, (COLS - strlen(menuItems[index_1].c_str())) / 2, menuItems[index_1].c_str());
                mvprintw(LINES / 2 + index_2 - 2, (COLS - strlen(menuItems[index_2].c_str())) / 2, menuItems[index_2].c_str());
                break;
            }
        }
        refresh();

        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    endwin();
    delete switchsound;

    return index;
}

//�û���������
std::string enterName()
{
    initscr(); //��ʼ��cursesģʽ
    raw();
    curs_set(0);

    char* welcome = "Welcome to The Snake Game!";
    mvprintw(LINES / 2 - 1, (COLS - strlen(welcome)) / 2, welcome);
    char* player = "Please enter your name: ";
    mvprintw(LINES / 2, (COLS - strlen(player)) / 2, player);

    refresh();
    char name[7];
    mvgetstr(LINES / 2, (COLS - strlen(player)) / 2 + strlen(player), name); //�����������
    std::string s(name);

    endwin(); //����cursesģʽ

    return s;
}

//���ų�����
sf::Music* gameMusic(std::string music_name)
{
    // ���ر�������
    sf::Music* backgroundMusic = new sf::Music;

    if (!backgroundMusic->openFromFile(music_name)) {
        std::cerr << "Failed to load background music" << std::endl;
        delete backgroundMusic; // �ͷ��ڴ�
        return nullptr;
    }

    // ����ѭ�����Ų����ű�������
    backgroundMusic->setLoop(true);

    return backgroundMusic;
}

//���Ŷ�����
sf::Sound* gameSound(std::string sound_name)
{
    sf::SoundBuffer* buffer = new sf::SoundBuffer;

    if (!buffer->loadFromFile(sound_name)) {
        std::cerr << "Failed to load background music" << std::endl;
        return nullptr;
    }

    sf::Sound* sound = new sf::Sound;
    sound->setBuffer(*buffer);

    return sound;
}
