#pragma once
#ifndef CALCULATEINTERCEPT_H
#define CALCULATEINTERCEPT_H
#include <SFML/Graphics.hpp>
#include "Structs.h"
#include <vector>

// Cálculo de intercepto entre proyectil y enemigo en movimiento
bool calcularIntercepto(const sf::Vector2f& torre, const Enemy& enemigo,
    const std::vector<sf::Vector2f>& waypoints, float speedProj, sf::Vector2f& intercepto) {

    sf::Vector2f E = enemigo.pos;
    sf::Vector2f V = enemigo.velocity(waypoints);
    sf::Vector2f R = E - torre;

    float a = V.x * V.x + V.y * V.y - speedProj * speedProj;
    float b = 2 * (R.x * V.x + R.y * V.y);
    float c = R.x * R.x + R.y * R.y;

    float disc = b * b - 4 * a * c;
    if (disc < 0) return false;

    float t1 = (-b + std::sqrt(disc)) / (2 * a);
    float t2 = (-b - std::sqrt(disc)) / (2 * a);
    float t = (t1 > 0) ? t1 : ((t2 > 0) ? t2 : -1);

    if (t <= 0) return false;
    intercepto = E + V * t;
    return true;
}
#endif