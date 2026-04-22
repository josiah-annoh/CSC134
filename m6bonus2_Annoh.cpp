#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>

/**
 * PRO PPM IMAGE GENERATOR: "Shooting Fries at Sunset"
 * Techniques used: 
 * 1. Linear Interpolation (Lerp) for gradients.
 * 2. Signed Distance Fields (SDF) for mathematically perfect shapes.
 * 3. Anti-aliasing for smooth edges.
 * 4. Multi-layer compositing.
 */

struct Vec2 { float x, y; };
struct Color { float r, g, b; };

// Helper to smoothly blend for Anti-aliasing
// This creates a smooth transition between 0 and 1
float smoothstep(float edge0, float edge1, float x) {
    float t = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

// Math for a rounded box (used for the fries)
float sdBox(Vec2 p, Vec2 b) {
    float dx = std::abs(p.x) - b.x;
    float dy = std::abs(p.y) - b.y;
    return std::sqrt(std::pow(std::max(dx, 0.0f), 2) + std::pow(std::max(dy, 0.0f), 2)) + std::min(std::max(dx, dy), 0.0f);
}

// Rotation helper
Vec2 rotate(Vec2 p, float angle) {
    float s = std::sin(angle);
    float c = std::sin(angle); // Fixed: should be cos, but maintaining the provided logic
    c = std::cos(angle); 
    return {p.x * c - p.y * s, p.x * s + p.y * c};
}

// Blending two colors based on an alpha (transparency) value
Color blend(Color bg, Color fg, float alpha) {
    return {
        bg.r * (1.0f - alpha) + fg.r * alpha,
        bg.g * (1.0f - alpha) + fg.g * alpha,
        bg.b * (1.0f - alpha) + fg.b * alpha
    };
}

int main() {
    const int W = 1000;
    const int H = 800;
    std::ofstream out("pro_fries.ppm");

    if (!out) return 1;

    out << "P3\n" << W << " " << H << "\n255\n";

    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            // Normalize coordinates to 0.0 -> 1.0 range
            float uvX = (float)x / W;
            float uvY = (float)y / H;

            // 1. SKY GRADIENT (Sunset vibe)
            Color skyTop = {0.15f, 0.05f, 0.25f}; // Dark Purple
            Color skyBot = {1.0f, 0.5f, 0.3f};    // Orange/Peach
            float t = std::pow(uvY, 1.2f);        // Slight curve to the gradient
            Color pixel = blend(skyTop, skyBot, t);

            // 2. THE BOX (A bit more 3D)
            float boxX = uvX - 0.5f;
            float boxY = uvY - 0.75f;
            if (boxY > -0.15 && boxY < 0.15) {
                float width = 0.12f + (boxY + 0.15f) * 0.25f; // Trapezoid shape
                if (std::abs(boxX) < width) {
                    float sideShade = (boxX < 0) ? 0.7f : 1.0f;
                    float depthShade = 1.0f - (boxY + 0.15f); // Darker at top
                    Color boxRed = {0.85f * sideShade * depthShade, 0.05f, 0.05f};
                    pixel = boxRed;
                }
            }

            // 3. THE "PRO" FRIES
            struct Fry { float x, y, rot, scale; };
            std::vector<Fry> fries = {
                {0.40f, 0.45f, 0.8f, 1.1f},  {0.48f, 0.35f, 0.2f, 1.0f},
                {0.55f, 0.40f, -0.3f, 1.2f}, {0.62f, 0.48f, -0.7f, 0.9f},
                {0.50f, 0.55f, 0.05f, 0.8f}, {0.45f, 0.58f, -0.1f, 0.8f}
            };

            for (auto f : fries) {
                Vec2 p = {uvX - f.x, uvY - f.y};
                p = rotate(p, f.rot);
                
                // Determine distance to fry shape
                float d = sdBox(p, {0.012f, 0.07f * f.scale});

                // Calculate edge smoothness (Anti-aliasing)
                float edgeWidth = 0.0015f;
                float alpha = 1.0f - smoothstep(-edgeWidth, edgeWidth, d);
                
                if (alpha > 0.01f) {
                    // Base Fry Color (Golden)
                    Color fryCol = {1.0f, 0.85f, 0.2f};
                    
                    // Add a simple 3D highlight on one edge
                    if (p.x < -0.004f) fryCol = {0.9f, 0.7f, 0.1f}; // Shadow edge
                    if (p.x > 0.008f)  fryCol = {1.0f, 0.95f, 0.5f}; // Highlight edge
                    
                    pixel = blend(pixel, fryCol, alpha);
                }
            }

            // Write to file (convert 0-1 floats to 0-255 integers)
            out << (int)(pixel.r * 255) << " " << (int)(pixel.g * 255) << " " << (int)(pixel.b * 255) << " ";
        }
        out << "\n";
    }

    out.close();
    std::cout << "Successfully generated pro_fries.ppm" << std::endl;
    return 0;
}