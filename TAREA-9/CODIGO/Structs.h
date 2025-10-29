#pragma once
#ifndef STRUCTS_H
#define STRUCTS_H
#include <SFML/Graphics.hpp>
#include <vector>
#include "MPI.h"

// -------------------- Estructuras --------------------
struct Enemy {
    sf::Vector2f pos;
    float speed = 1.5f;
    int targetIndex = 0;
    float radius = 8.f;
    bool detected = false;
    bool isTarget = false;

    void move(const std::vector<sf::Vector2f>& waypoints) {
        if (targetIndex >= waypoints.size()) return;
        sf::Vector2f target = waypoints[targetIndex];
        sf::Vector2f dir = target - pos;
        float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (length < 1.f) {
            targetIndex++;
            return;
        }
        dir /= length;
        pos += dir * speed;
    }

    sf::Vector2f velocity(const std::vector<sf::Vector2f>& waypoints) const {
        if (targetIndex >= waypoints.size()) return { 0,0 };
        sf::Vector2f target = waypoints[targetIndex];
        sf::Vector2f dir = target - pos;
        float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (length != 0) dir /= length;
        return dir * speed;
    }
};

struct Projectile {
    sf::Vector2f pos;
    sf::Vector2f dir;
    float speed = 5.f;
    bool alive = true;

    Projectile(sf::Vector2f start, sf::Vector2f target) {
        pos = start;
        sf::Vector2f d = target - start;
        float len = std::sqrt(d.x * d.x + d.y * d.y);
        if (len != 0) dir = d / len;
        else dir = { 0, 0 };
    }

    void move() { pos += dir * speed; }
};

struct Tower {
    sf::Vector2f pos;
    float range = 120.f;
    float baseRadius = 12.f;
    float projectileSpeed = 5.f;
    int cooldownMs = 1000; // 1 segundo
    sf::Clock clock;       // para cooldown

    std::vector<sf::Vector2f> getConvexArea(int samples = 20) const {
        std::vector<sf::Vector2f> P;
        for (int i = 0; i < samples; i++) {
            float angle = i * 2 * M_PI / samples;
            P.push_back({ pos.x + range * std::cos(angle),
                          pos.y + range * std::sin(angle) });
        }

        // --- Construir convex hull iterativamente ---
        int n = P.size(), k = 0;
        if (n <= 3) return P;
        std::vector<sf::Vector2f> H(2 * n);
        std::sort(P.begin(), P.end(), [](sf::Vector2f a, sf::Vector2f b) {
            return a.x < b.x || (a.x == b.x && a.y < b.y);
            });
        for (int i = 0; i < n; i++) {
            while (k >= 2 && ((H[k - 1].x - H[k - 2].x) * (P[i].y - H[k - 2].y)
                - (H[k - 1].y - H[k - 2].y) * (P[i].x - H[k - 2].x)) <= 0)
                k--;
            H[k++] = P[i];
        }
        for (int i = n - 2, t = k + 1; i >= 0; i--) {
            while (k >= t && ((H[k - 1].x - H[k - 2].x) * (P[i].y - H[k - 2].y)
                - (H[k - 1].y - H[k - 2].y) * (P[i].x - H[k - 2].x)) <= 0)
                k--;
            H[k++] = P[i];
        }
        H.resize(k - 1);
        return H;
    }


    bool canShoot() {
        return clock.getElapsedTime().asMilliseconds() >= cooldownMs;
    }

    void resetCooldown() { clock.restart(); }
};

bool circumCircle(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const sf::Vector2f& p) {
    float ax = a.x - p.x, ay = a.y - p.y;
    float bx = b.x - p.x, by = b.y - p.y;
    float cx = c.x - p.x, cy = c.y - p.y;

    float det = (ax * ax + ay * ay) * (bx * cy - cx * by)
        - (bx * bx + by * by) * (ax * cy - cx * ay)
        + (cx * cx + cy * cy) * (ax * by - bx * ay);
    return det > 0;
}

bool sameEdge(const std::pair<sf::Vector2f, sf::Vector2f>& e1, const std::pair<sf::Vector2f, sf::Vector2f>& e2) {
    return ((e1.first == e2.first && e1.second == e2.second) ||
        (e1.first == e2.second && e1.second == e2.first));
}


#endif