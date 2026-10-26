#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>

const float DEG_TO_RAD = 3.14159265358979323846f / 180.f;

int main() {
    // Створення вікна
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "Space Ship Simulation");
    window.setFramerateLimit(60);

    // Завантаження шрифту для інтерфейсу
    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {

    }

    sf::Text infoText(font, "", 18);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition({20.f, 20.f});

    // Графічне представлення корабля (трикутник)
    sf::ConvexShape ship;
    ship.setPointCount(3);
    ship.setPoint(0, {0.f, -15.f});   
    ship.setPoint(1, {-10.f, 15.f});  
    ship.setPoint(2, {10.f, 15.f});   
    ship.setFillColor(sf::Color::Cyan);
    ship.setOrigin({0.f, 0.f});
    ship.setPosition({600.f, 400.f});

    // Фізичні параметри корабля
    sf::Vector2f position(600.f, 400.f);
    sf::Vector2f velocity(0.f, 0.f);
    float angle = 0.f;          // Кут
    float accelerationPower = 200.f; // Потужність двигуна
    float rotationSpeed = 180.f; // Швидкість повороту
    bool isEngineOn = false;

    // Контейнер для траєкторії
    std::vector<sf::Vector2f> trajectory;
    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // Обробка подій та керування
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // Плавне/дискретне керування клавіатурою у реальному часі
        isEngineOn = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            angle -= rotationSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            angle += rotationSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            isEngineOn = true;
        }

        // Оновлення фізичної моделі
        sf::Vector2f acceleration(0.f, 0.f);
        if (isEngineOn) {
            float rad = angle * DEG_TO_RAD;
            // Вектор тяги залежить від кута орієнтації корабля
            acceleration.x = std::sin(rad) * accelerationPower;
            acceleration.y = -std::cos(rad) * accelerationPower;
        }

        // Інтегрування методом Ейлера
        velocity += acceleration * dt;
        position += velocity * dt;

        // Ефект торусу (щоб корабель не вилітав назавжди за межі екрана)
        if (position.x < 0.f) position.x += 1200.f;
        if (position.x > 1200.f) position.x -= 1200.f;
        if (position.y < 0.f) position.y += 800.f;
        if (position.y > 800.f) position.y -= 800.f;

        ship.setPosition(position);
        ship.setRotation(sf::degrees(angle));

        // Збереження точок траєкторії
        if (trajectory.empty() || std::hypot(position.x - trajectory.back().x, position.y - trajectory.back().y) > 5.f) {
            trajectory.push_back(position);
            if (trajectory.size() > 600) {
                trajectory.erase(trajectory.begin());
            }
        }

        // Формування лінії траєкторії
        sf::VertexArray trajectoryLine(sf::PrimitiveType::LineStrip);
        for (const auto& p : trajectory) {
            trajectoryLine.append(sf::Vertex{ p, sf::Color(100, 100, 100) });
        }

        // Оновлення тексту телеметрії
        float speed = std::hypot(velocity.x, velocity.y);
        infoText.setString(
            "Engine (W/Up): " + std::string(isEngineOn ? "ON" : "OFF") +
            "\nRotation (A/D): " + std::to_string(angle) + " deg" +
            "\nSpeed: " + std::to_string(speed) + " px/s" +
            "\nPosition: (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")"
        );

        // Відображення сцени
        window.clear(sf::Color::Black);

        window.draw(trajectoryLine);
        window.draw(ship);
        window.draw(infoText);

        window.display();
    }

    return 0;
}