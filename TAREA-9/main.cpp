//Geometría Computacional: Virus Invation (Tower Defence)
// Punto Poligono: Determina si el enemigo se encuentra dentro del area de la torre
// Cerco Convexo: Genera el área de disparo de la torre
// Intersección de Segmentos: La colisión entre los proyectiles de la torre y los enemigos (Hit)
// Busqueda por Rango: Genera la zon critica, donde deben llegar los enemigos
// Vecino mas Próximo: La torre selecciona al enemigo mas cercano para disparar
// Voronoi: Muestra el area de la torre previamente generada por Cerco Convexo
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <set>
#include <fstream>
#include "SegmentIntersection.h"
#include "PointInPolygon.h"
#include "AuxFunctions.h"
#include "CalculateIntercept.h"
bool LoadFont(sf::Font& font) {
    if (font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) return true;
        return false;
}
// -------------------- Modos de Construcción --------------------
enum class BuildMode { Towers, Enemies };

// -------------------- MAIN --------------------
int main() {
    
    sf::RenderWindow window(sf::VideoMode(1000, 700), "Tower Defense - Zona Critica");
    window.setFramerateLimit(60);

    // Fondo del juego
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("C:\\assets\\fondo.jpg")) {
        std::cout << "No se pudo cargar la imagen de fondo\n";
    }
    sf::Sprite backgroundSprite(backgroundTexture);

    // Ajustar fondo al tamaño completo de la ventana
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();
    backgroundSprite.setScale(
        float(windowSize.x) / textureSize.x,
        float(windowSize.y) / textureSize.y
    );

    // Cargar textura de torre
    sf::Texture towerTexture;
    if (!towerTexture.loadFromFile("C:\\assets\\tower.png")) {
        std::cout << "No se pudo cargar la imagen de la torre\n";
    }

    // Cargar textura de enemigos
    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile("C:\\assets\\virus.png")) {
        std::cout << "No se pudo cargar la imagen del enemigo\n";
    }

    // Cargar textura de la zona crítica
    sf::Texture cpuTexture;
    if (!cpuTexture.loadFromFile("C:\\assets\\cpu.png")) {
        std::cout << "No se pudo cargar la imagen de la zona crítica\n";
    }

    // Cargar textura de corazones
    sf::Texture heartTexture;
    if (!heartTexture.loadFromFile("C:\\assets\\corazon.png")) {
        std::cout << "No se pudo cargar la imagen del corazon\n";
    }

    BuildMode mode = BuildMode::Towers;
    std::vector<Tower> towers;
    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;

    std::vector<sf::Vector2f> waypoints = {
        {50, 100}, {300, 100}, {300, 400}, {700, 400}, {700, 600}, {950, 600}
    };

    int lives = 5;
    sf::Font font;
    bool GetFont = LoadFont(font);
    if (!GetFont) {
        std::cerr << "No se encontro la fuente";
        
    }
    sf::Text livesText;
    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::Red);

    sf::Vector2f criticalZoneCenter = waypoints.back();
    float criticalRadius = 40.f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::T) mode = BuildMode::Towers;
                if (event.key.code == sf::Keyboard::E) mode = BuildMode::Enemies;
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (mode == BuildMode::Towers) {
                    if (!isOnPath(mousePos, waypoints)) {
                        towers.push_back({ mousePos });
                    }
                }
                else if (mode == BuildMode::Enemies) {
                    if (isOnPath(mousePos, waypoints)) {
                        Enemy e; e.pos = mousePos;
                        enemies.push_back(e);
                    }
                }
            }
        }

        // Mover enemigos
        for (auto& e : enemies) e.move(waypoints);

        // Resetear estados
        for (auto& e : enemies) { e.detected = false; e.isTarget = false; }

        // Torres disparan con cooldown + predicción
        std::vector<Enemy*> targets; // NUEVO: lista de enemigos objetivo

        for (auto& t : towers) {
            auto hull = t.getConvexArea();
            Enemy* nearest = nullptr;
            float minDist = 1e9;

            for (auto& e : enemies) {
                if (pointInPolygon(e.pos, hull)) {
                    e.detected = true;
                    float d = distance(t.pos, e.pos);
                    if (d < minDist) {
                        minDist = d;
                        nearest = &e;
                    }
                }
            }
            if (nearest && t.canShoot()) {
                targets.push_back(nearest); // NUEVO: guardar puntero
                sf::Vector2f intercepto;
                if (calcularIntercepto(t.pos, *nearest, waypoints, t.projectileSpeed, intercepto)) {
                    projectiles.emplace_back(t.pos, intercepto);
                    t.resetCooldown();
                }
            }
        }

        // NUEVO: marcar solo los enemigos objetivo como isTarget
        for (auto* e : targets) {
            e->isTarget = true;
        }


        // Mover proyectiles
        for (auto& p : projectiles) p.move();

        // Colisiones proyectil-enemigo
        for (auto& p : projectiles) {
            if (!p.alive) continue;
            for (auto& e : enemies) {
                if (distance(p.pos, e.pos) <= e.radius) {
                    p.alive = false;
                    e.pos = { -100, -100 }; // eliminado
                }
            }
        }
        projectiles.erase(
            std::remove_if(projectiles.begin(), projectiles.end(),
                [](const Projectile& p) { return !p.alive; }),
            projectiles.end());
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const Enemy& e) { return e.pos.x < 0; }),
            enemies.end());

        // Zona crítica
        for (size_t i = 0; i < enemies.size();) {
            float d = distance(enemies[i].pos, criticalZoneCenter);
            if (d < criticalRadius) {
                lives--;
                enemies.erase(enemies.begin() + i);
                continue;
            }
            i++;
        }
        // Si las vidas llegan a 0, cerrar el juego
        if (lives <= 0) {
            std::cout << "¡Juego terminado! Se acabaron las vidas.\n";
            window.close();
        }


        window.clear();
        window.draw(backgroundSprite);

        // Camino
        sf::VertexArray path(sf::LinesStrip, waypoints.size());
        for (size_t i = 0; i < waypoints.size(); i++) {
            path[i].position = waypoints[i];
            path[i].color = sf::Color::White;
        }
        window.draw(path);

        // Zona crítica con imagen
        sf::Sprite cpuSprite(cpuTexture);
        sf::Vector2u texSize = cpuTexture.getSize();
        float scale = 1.0f * (criticalRadius * 2) / std::max(texSize.x, texSize.y);
        cpuSprite.setScale(scale, scale);
        cpuSprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
        cpuSprite.setPosition(criticalZoneCenter);
        window.draw(cpuSprite);

        // ÁREAS DE VORONOI (violeta circulares)
        if (!towers.empty()) {
            for (auto& t : towers) {
                // Dibujar el área circular violeta centrada en la torre
                sf::CircleShape area(t.range);
                area.setOrigin(t.range, t.range);
                area.setPosition(t.pos);
                area.setFillColor(sf::Color(180, 0, 255, 80)); // violeta translúcido
                window.draw(area);
            }
        }

        // Torres
        for (auto& t : towers) {
            // Imagen de la torre
            sf::Sprite towerSprite(towerTexture);
            sf::Vector2u texSize = towerTexture.getSize();
            float scale = 3.0f * (t.baseRadius * 2) / std::max(texSize.x, texSize.y);
            towerSprite.setScale(scale, scale);
            towerSprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
            towerSprite.setPosition(t.pos);
            window.draw(towerSprite);
        }

        // Enemigos con imagen
        for (const auto& e : enemies) {
            sf::Sprite enemySprite(enemyTexture);
            sf::Vector2u texSize = enemyTexture.getSize();
            float scale = 2.0f * (e.radius * 2) / std::max(texSize.x, texSize.y);
            enemySprite.setScale(scale, scale);
            enemySprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
            enemySprite.setPosition(e.pos);
            window.draw(enemySprite);
        }

        // Proyectiles
        for (auto& p : projectiles) {
            sf::CircleShape bullet(3.f);
            bullet.setOrigin(3.f, 3.f);
            bullet.setPosition(p.pos);
            bullet.setFillColor(sf::Color::Green);
            window.draw(bullet);
        }

        // Vidas
        livesText.setString("VIDAS: ");
        livesText.setPosition(20, 15);
        window.draw(livesText);

        // Dibujar corazones de vida
        float heartX = livesText.getPosition().x + livesText.getGlobalBounds().width + 10.f;
        float heartY = livesText.getPosition().y - 4.f;
        float heartSpacing = 35.f;
        sf::Vector2u heartSize = heartTexture.getSize();
        float heartScale = 0.05f;

        for (int i = 0; i < lives; i++) {
            sf::Sprite heartSprite(heartTexture);
            heartSprite.setScale(heartScale, heartScale);
            heartSprite.setPosition(heartX + i * heartSpacing, heartY);
            window.draw(heartSprite);
        }

        window.display();
    }
    return 0;
}