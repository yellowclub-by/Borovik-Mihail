#include <SFML/Graphics.hpp>
#include <ctime>

using namespace sf;

int main() {
    srand(time(0));

    const int width = 40, height = 30; // Уменьшил для удобства
    const float cell = 20.f;

    RenderWindow window(VideoMode({ (unsigned int)(width * cell), (unsigned int)(height * cell) }), "Snake Game");
    window.setFramerateLimit(60);

    int sneak_x[100], sneak_y[100];
    int length = 1;
    sneak_x[0] = width / 2;
    sneak_y[0] = height / 2;

    int dx = 1, dy = 0;
    int apple_x = rand() % (width - 2) + 1;
    int apple_y = rand() % (height - 2) + 1;

    bool gameover = false;
    float timer = 0.f, delay = 0.1f; // Скорость игры
    Clock clock;

    RectangleShape block(Vector2f(cell - 1.f, cell - 1.f));

    while (window.isOpen()) {
        float time = clock.restart().asSeconds();
        timer += time;

        while (const std::optional event = window.pollEvent()) {
            if (event->is<Event::Closed>()) window.close();
        }

        // Управление
        if (Keyboard::isKeyPressed(Keyboard::Key::W) && dy != 1) { dx = 0; dy = -1; }
        if (Keyboard::isKeyPressed(Keyboard::Key::S) && dy != -1) { dx = 0; dy = 1; }
        if (Keyboard::isKeyPressed(Keyboard::Key::A) && dx != 1) { dx = -1; dy = 0; }
        if (Keyboard::isKeyPressed(Keyboard::Key::D) && dx != -1) { dx = 1; dy = 0; }

        if (timer > delay && !gameover) {
            timer = 0;

            // Движение хвоста
            for (int i = length; i > 0; i--) {
                sneak_x[i] = sneak_x[i - 1];
                sneak_y[i] = sneak_y[i - 1];
            }

            // Движение головы
            sneak_x[0] += dx;
            sneak_y[0] += dy;

            // Проверка яблока
            if (sneak_x[0] == apple_x && sneak_y[0] == apple_y) {
                length++;
                apple_x = rand() % (width - 2) + 1;
                apple_y = rand() % (height - 2) + 1;
            }

            // Столкновение со стенами
            if (sneak_x[0] <= 0 || sneak_x[0] >= width - 1 || sneak_y[0] <= 0 || sneak_y[0] >= height - 1)
                gameover = true;

            // Самопересечение
            for (int i = 1; i < length; i++)
                if (sneak_x[0] == sneak_x[i] && sneak_y[0] == sneak_y[i]) gameover = true;
        }

        // РИСОВАНИЕ
        window.clear();

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                // Стены
                if (i == 0 || i == width - 1 || j == 0 || j == height - 1) {
                    block.setFillColor(Color::Red);
                    block.setPosition({ i * cell, j * cell });
                    window.draw(block);
                }
            }
        }

        // Яблоко
        block.setFillColor(Color::Magenta);
        block.setPosition({ apple_x * cell, apple_y * cell });
        window.draw(block);

        // Змейка
        for (int i = 0; i < length; i++) {
            block.setFillColor(i == 0 ? Color::Yellow : Color::Green);
            block.setPosition({ sneak_x[i] * cell, sneak_y[i] * cell });
            window.draw(block);
        }

        window.display();

        if (gameover) {
            window.close();
        }
    }

    return 0;
}