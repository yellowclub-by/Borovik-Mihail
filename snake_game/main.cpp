
#include <SFML/Graphics.hpp>
#include <ctime>
#include <optional>
#include <iostream>

using namespace sf;

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    const int width = 40, height = 30; // Чуть уменьшил для удобства
    const float cell = 20.f;

    RenderWindow window(VideoMode({ (unsigned int)(width * cell), (unsigned int)(height * cell) }), "Snake Game");
    window.setFramerateLimit(60);

    int sneak_x[100], sneak_y[100]; // Массивы для координат тела
    int length = 3;

    // Начальная позиция
    for (int i = 0; i < length; i++) {
        sneak_x[i] = width / 2;
        sneak_y[i] = height / 2 + i;
    }

    int dx = 0, dy = -1; // Начальное движение вверх
    int apple_x = rand() % (width - 2) + 1;
    int apple_y = rand() % (height - 2) + 1;

    bool isGameRunning = true;
    float timer = 0.f, delay = 0.1f; 
    Clock clock;

    CircleShape apple(cell - 10.f);
    RectangleShape block(Vector2f(cell - 1.f, cell - 1.f));

    while (window.isOpen())
    {
        float time = clock.restart().asSeconds();
        timer += time;

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }

        // Управление
        if (Keyboard::isKeyPressed(Keyboard::Key::W) && dy != 1) { dx = 0; dy = -1; }
        if (Keyboard::isKeyPressed(Keyboard::Key::S) && dy != -1) { dx = 0; dy = 1; }
        if (Keyboard::isKeyPressed(Keyboard::Key::A) && dx != 1) { dx = -1; dy = 0; }
        if (Keyboard::isKeyPressed(Keyboard::Key::D) && dx != -1) { dx = 1; dy = 0; }

        // Логика движения по таймеру
        if (timer > delay && isGameRunning)
        {
            timer = 0;

            // Двигаем хвост
            for (int i = length - 1; i > 0; i--) {
                sneak_x[i] = sneak_x[i - 1];
                sneak_y[i] = sneak_y[i - 1];
            }

            // Двигаем голову
            sneak_x[0] += dx;
            sneak_y[0] += dy;

            // Проверка на съедение яблока
            if (sneak_x[0] == apple_x && sneak_y[0] == apple_y) {
                length++;
                apple_x = rand() % (width - 2) + 1;
                apple_y = rand() % (height - 2) + 1;
            }

            // Столкновение со стенами
            if (sneak_x[0] <= 0 || sneak_x[0] >= width - 1 || sneak_y[0] <= 0 || sneak_y[0] >= height - 1)
                isGameRunning = false;

            // Столкновение с собой
            for (int i = 1; i < length; i++) {
                if (sneak_x[0] == sneak_x[i] && sneak_y[0] == sneak_y[i])
                    isGameRunning = false;
            }
        }

        // --- ОТРИСОВКА ---
        window.clear();

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                // Стены
                if (i == 0 || i == width - 1 || j == 0 || j == height - 1) {
                    block.setFillColor(Color(100, 100, 100));
                    block.setPosition({ i * cell, j * cell });
                    window.draw(block);
                }
            }
        }

        // Яблоко
        apple.setFillColor(Color::Red);
        apple.setPosition({ apple_x * cell, apple_y * cell });
        window.draw(apple);

        // Змейка
        for (int i = 0; i < length; i++) {
            block.setFillColor(i == 0 ? Color::Yellow : Color::Green);
            block.setPosition({ sneak_x[i] * cell, sneak_y[i] * cell });
            window.draw(block);
        }

        if (!isGameRunning) {
            // Можно добавить текст Game Over здесь
        }

        window.display();
    }

    return 0;
}    

