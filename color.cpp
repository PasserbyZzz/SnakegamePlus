#include "color.h"
#include "curses.h"

#include <windows.h>
#include <string>
#include <iostream>

//设置字体颜色
void initColors() {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  // 白色
    init_pair(2, COLOR_GREEN, COLOR_BLACK);  // 绿色
    init_pair(3, COLOR_BLUE, COLOR_BLACK);   //蓝色
    init_pair(4, COLOR_RED, COLOR_BLACK);    //红色
    init_pair(5,COLOR_YELLOW, COLOR_BLACK); //黄色
}

// 获取屏幕分辨率
sf::Vector2u getScreenResolution() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Get Screen Resolution", sf::Style::None);
    sf::Vector2u screenSize = window.getSize();
    window.close();
    return screenSize;
}

//打印背景图片和文字
void displayImageWithText() {
    // 获取屏幕分辨率
    sf::Vector2u screenSize = getScreenResolution();

    // 创建一个全屏窗口
    sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), "SnakeGame", sf::Style::Fullscreen);

    // 加载背景图像
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.jpg")) {
        std::cerr << "Failed to load background image" << std::endl;
        return;
    }

    // 创建精灵并设置纹理
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    // 获取背景图像大小
    sf::Vector2u textureSize = backgroundTexture.getSize();

    // 计算缩放比例
    float scaleX = static_cast<float>(screenSize.x) / textureSize.x;
    float scaleY = static_cast<float>(screenSize.y) / textureSize.y;

    // 设置精灵的缩放比例，使其铺满整个窗口
    backgroundSprite.setScale(scaleX, scaleY);

    // 加载字体
    sf::Font font;
    if (!font.loadFromFile("PixelOperator8-Bold.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return;
    }

    // 创建文本对象
    sf::Text text;
    text.setFont(font);
    text.setString("Press Any Key to START");
    text.setCharacterSize(80); // 字符大小
    text.setFillColor(sf::Color::White); // 字体颜色
    text.setPosition(500, 1460); // 文本位置

    // 设置闪烁计时器
    sf::Clock clock;
    bool showText = true;
    float blinkInterval = 0.5f; // 闪烁间隔（秒）

    // 主循环
    while (window.isOpen()) {
        // 处理事件
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // 按任意键退出
            if (event.type == sf::Event::KeyPressed)
                window.close();
        }

        // 更新闪烁状态
        if (clock.getElapsedTime().asSeconds() >= blinkInterval) {
            showText = !showText;
            clock.restart();
        }

        // 清空窗口
        window.clear();

        // 绘制背景图像
        window.draw(backgroundSprite);

        // 绘制文本
        if (showText) {
            window.draw(text);
        }

        // 显示内容
        window.display();
    }
}

//用户输入姓名
std::string enterName()
{
    initscr(); //初始化curses模式
    raw();
    curs_set(0);

    char* welcome = "Welcome to The Snake Game!";
    mvprintw(LINES / 2 - 1, (COLS - strlen(welcome)) / 2, welcome);
    char* player = "Please enter your name: ";
    mvprintw(LINES / 2, (COLS - strlen(player)) / 2, player);

    refresh();
    char name[7];
    mvgetstr(LINES / 2, (COLS - strlen(player)) / 2 + strlen(player), name); //玩家输入姓名
    std::string s(name);

    endwin(); //结束curses模式

    return s;
}

//播放长音乐
sf::Music* gameMusic(std::string music_name)
{
    // 加载背景音乐
    sf::Music* backgroundMusic = new sf::Music;

    if (!backgroundMusic->openFromFile(music_name)) {
        std::cerr << "Failed to load background music" << std::endl;
        delete backgroundMusic; // 释放内存
        return nullptr;
    }

    // 设置循环播放并播放背景音乐
    backgroundMusic->setLoop(true);

    return backgroundMusic;
}

//播放短音乐
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
