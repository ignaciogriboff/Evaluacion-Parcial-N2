#pragma once
#ifndef POINTINPOLYGON_H
#define POINTINPOLYGON_H
#include <SFML/Graphics.hpp>
#include <vector>

// -------------------- Punto en Polígono --------------------
bool pointInPolygon(const sf::Vector2f& p, const std::vector<sf::Vector2f>& poly) {
    bool inside = false;
    int n = poly.size();
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if (((poly[i].y > p.y) != (poly[j].y > p.y)) &&
            (p.x < (poly[j].x - poly[i].x) * (p.y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x)) {
            inside = !inside;
        }
    }
    return inside;
}

#endif
