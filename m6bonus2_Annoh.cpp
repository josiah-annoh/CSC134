#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>

/**
 * PPM_Image_Generator: Shooting Fries
 * * This program generates a .ppm image (Portable Pixmap) in the P3 (ASCII) format.
 * It manually "paints" a box of fries that appears to be "shooting" or flying 
 * across the screen by calculating geometric overlaps for each pixel.
 */

struct Color {
    int r, g, b;
};

// Constant Colors
const Color SKY_BLUE = {135, 206, 235};
const Color FRY_YELLOW = {255, 223, 0};
const Color FRY_BROWN = {218, 165, 32};
const Color BOX_RED = {200, 0, 0};
const Color BOX_DARK_RED = {150, 0, 0};
const Color WHITE = {255, 255, 255};

// Image Dimensions
const int WIDTH = 800;
const int HEIGHT = 600;

bool isInsideFry(int x, int y, int fx, int fy, int fw, int fh, float angle) {
    // Translate point to origin relative to fry center
    float cx = fx + fw / 2.0f;
    float cy = fy + fh / 2.0f;
    float tx = x - cx;
    float ty = y - cy;

    // Rotate point back by -angle to check against axis-aligned rectangle
    float rx = tx * cos(-angle) - ty * sin(-angle);
    float ry = tx * sin(-angle) + ty * cos(-angle);

    return (rx >= -fw / 2.0f && rx <= fw / 2.0f && ry >= -fh / 2.0f && ry <= fh / 2.0f);
}

int main() {
    std::string filename = "shooting_fries.ppm";
    std::ofstream outFile(filename);

    if (!outFile) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return 1;
    }

    // 1. Write PPM Header
    // P3: ASCII Color, Width, Height, Max Color Value (255)
    outFile << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";

    // 2. Generate Image Data
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            Color pixel = SKY_BLUE; // Default Background

            // --- Draw Motion Trails (Simple Lines) ---
            if (y > 100 && y < 500 && x < 400) {
                if ((y + x/2) % 40 == 0 && x < (y * 0.8)) {
                   pixel = WHITE;
                }
            }

            // --- Draw the Fry Box (Trapezoid) ---
            // Centered roughly at 500, 400
            int bx = 500, by = 400, bw = 120, bh = 140;
            if (y >= by && y <= by + bh) {
                float slant = (y - by) * 0.2f;
                if (x >= bx - slant && x <= bx + bw + slant) {
                    pixel = (x > bx + bw / 2) ? BOX_RED : BOX_DARK_RED;
                }
            }

            // --- Draw "Shooting" Fries ---
            // We define a few fries flying out at angles
            struct Fry { float x, y, w, h, angle; };
            std::vector<Fry> fries = {
                {450, 250, 15, 80, 0.5f},
                {480, 200, 15, 90, 0.2f},
                {520, 180, 15, 75, -0.1f},
                {570, 220, 15, 85, -0.4f},
                {420, 320, 15, 70, 0.8f},
                // Some fries still in the box
                {510, 350, 12, 100, 0.05f},
                {535, 360, 12, 95, -0.05f},
                {550, 355, 12, 110, 0.1f}
            };

            for (const auto& f : fries) {
                if (isInsideFry(x, y, f.x, f.y, f.w, f.h, f.angle)) {
                    // Check for a little "shadow" or edge on the fry
                    if (x % 5 == 0 || y % 5 == 0) pixel = FRY_BROWN;
                    else pixel = FRY_YELLOW;
                }
            }

            // Write the RGB triplet to the file
            outFile << pixel.r << " " << pixel.g << " " << pixel.b << "  ";
        }
        outFile << "\n"; // End of row
    }

    outFile.close();
    std::cout << "Successfully generated " << filename << std::endl;
    std::cout << "You can view this file using a PPM viewer like Jumpshare or a browser extension." << std::endl;

    return 0;
}