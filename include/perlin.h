// include/perlin.h
#pragma once
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>

// Simple 2D Perlin Noise (header-only)
class PerlinNoise {
private:
    std::vector<int> p;  // Permutation table

public:
    PerlinNoise(unsigned int seed = 0) {
        p.resize(256);
        std::iota(p.begin(), p.end(), 0);
        std::default_random_engine engine(seed);
        std::shuffle(p.begin(), p.end(), engine);
        p.insert(p.end(), p.begin(), p.end());  // Duplicate for wrap
    }

    double noise(double x, double y) {
        int X = (int)floor(x) & 255;
        int Y = (int)floor(y) & 255;

        x -= floor(x);
        y -= floor(y);

        double u = fade(x);
        double v = fade(y);

        int aa = p[p[X] + Y];
        int ab = p[p[X] + Y + 1];
        int ba = p[p[X + 1] + Y];
        int bb = p[p[X + 1] + Y + 1];

        double res = lerp(v, lerp(u, grad(aa, x, y), grad(ba, x - 1, y)),
                             lerp(u, grad(ab, x, y - 1), grad(bb, x - 1, y - 1)));
        return (res + 1.0) / 2.0;  // Normalize to [0,1]
    }

private:
    double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
    double lerp(double t, double a, double b) { return a + t * (b - a); }
    double grad(int hash, double x, double y) {
        switch (hash & 3) {
            case 0: return x + y;
            case 1: return -x + y;
            case 2: return x - y;
            case 3: return -x - y;
            default: return 0;
        }
    }
};