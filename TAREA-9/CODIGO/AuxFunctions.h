#pragma once
#ifndef AUXFUNCTIONS_H
#define AUXFUNCTIONS_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

// -------------------- Funciones Aux --------------------
bool isOnPath(sf::Vector2f p, const std::vector<sf::Vector2f>& waypoints, float tolerance = 20.f) {
    for (size_t i = 1; i < waypoints.size(); i++) {
        sf::Vector2f A = waypoints[i - 1];
        sf::Vector2f B = waypoints[i];
        sf::Vector2f AB = B - A;
        sf::Vector2f AP = p - A;

        float ab2 = AB.x * AB.x + AB.y * AB.y;
        float t = std::max(0.f, std::min(1.f, (AP.x * AB.x + AP.y * AB.y) / ab2));
        sf::Vector2f closest = A + t * AB;

        float dx = p.x - closest.x;
        float dy = p.y - closest.y;
        if (dx * dx + dy * dy <= tolerance * tolerance) return true;
    }
    return false;
}

float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}


#endif 