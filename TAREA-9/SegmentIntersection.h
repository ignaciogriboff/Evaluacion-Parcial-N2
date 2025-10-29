#pragma once
#ifndef SEGMENTINSERSECTION_H
#define SEGMENTINTERSECTION_H
#include <SFML/Graphics.hpp>
#include<vector>

// -------------------- Intersección de Segmentos --------------------
int orientation(const sf::Vector2f& p, const sf::Vector2f& q, const sf::Vector2f& r) {
    float val = (q.y - p.y) * (r.x - q.x) -
        (q.x - p.x) * (r.y - q.y);
    if (std::fabs(val) < 1e-6) return 0;  // colineales
    return (val > 0) ? 1 : 2; // horario o antihorario
}

bool onSegment(const sf::Vector2f& p, const sf::Vector2f& q, const sf::Vector2f& r) {
    return (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y));
}

bool doIntersect(const sf::Vector2f& p1, const sf::Vector2f& q1,
    const sf::Vector2f& p2, const sf::Vector2f& q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4) return true;
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;
    return false;
}

#endif
