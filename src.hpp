#ifndef SRC_HPP
#define SRC_HPP

#include <vector>
#include <cmath>
#include <algorithm>

typedef std::vector<std::vector<double>> IMAGE_T;

// Helper function to count white pixels in a region
double count_pixels(const IMAGE_T &img, int r1, int c1, int r2, int c2) {
    double count = 0;
    for (int i = r1; i < r2 && i < img.size(); i++) {
        for (int j = c1; j < c2 && j < img[i].size(); j++) {
            count += img[i][j];
        }
    }
    return count;
}

// Find bounding box of the digit
void find_bounds(const IMAGE_T &img, int &top, int &bottom, int &left, int &right) {
    int h = img.size();
    int w = img[0].size();
    double threshold = 0.3;
    
    top = h; bottom = 0; left = w; right = 0;
    
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (img[i][j] > threshold) {
                top = std::min(top, i);
                bottom = std::max(bottom, i);
                left = std::min(left, j);
                right = std::max(right, j);
            }
        }
    }
    
    if (top > bottom) {
        top = 0; bottom = h - 1; left = 0; right = w - 1;
    }
}

// Count holes/loops in the image
int count_holes(const IMAGE_T &img) {
    int h = img.size();
    int w = img[0].size();
    double threshold = 0.3;
    
    // Create binary image
    std::vector<std::vector<int>> binary(h, std::vector<int>(w, 0));
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            binary[i][j] = (img[i][j] > threshold) ? 1 : 0;
        }
    }
    
    // Count transitions from 0 to 1 in middle rows
    int holes = 0;
    for (int i = h/4; i < 3*h/4; i++) {
        int transitions = 0;
        for (int j = 1; j < w; j++) {
            if (binary[i][j] == 1 && binary[i][j-1] == 0) {
                transitions++;
            }
        }
        if (transitions >= 2) holes++;
    }
    
    return holes;
}

// Calculate center of mass
void center_of_mass(const IMAGE_T &img, double &cy, double &cx) {
    double sum = 0, sum_y = 0, sum_x = 0;
    int h = img.size();
    int w = img[0].size();
    
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            sum += img[i][j];
            sum_y += img[i][j] * i;
            sum_x += img[i][j] * j;
        }
    }
    
    if (sum > 0) {
        cy = sum_y / sum;
        cx = sum_x / sum;
    } else {
        cy = h / 2.0;
        cx = w / 2.0;
    }
}

// Check for horizontal symmetry
double horizontal_symmetry(const IMAGE_T &img) {
    int h = img.size();
    int w = img[0].size();
    double diff = 0;
    int count = 0;
    
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w/2; j++) {
            diff += std::abs(img[i][j] - img[i][w-1-j]);
            count++;
        }
    }
    
    return count > 0 ? diff / count : 0;
}

// Check for vertical symmetry
double vertical_symmetry(const IMAGE_T &img) {
    int h = img.size();
    int w = img[0].size();
    double diff = 0;
    int count = 0;
    
    for (int i = 0; i < h/2; i++) {
        for (int j = 0; j < w; j++) {
            diff += std::abs(img[i][j] - img[h-1-i][j]);
            count++;
        }
    }
    
    return count > 0 ? diff / count : 0;
}

int judge(IMAGE_T &img) {
    int h = img.size();
    int w = img[0].size();
    
    if (h == 0 || w == 0) return 0;
    
    // Find bounding box
    int top, bottom, left, right;
    find_bounds(img, top, bottom, left, right);
    
    int height = bottom - top + 1;
    int width = right - left + 1;
    double aspect_ratio = (width > 0) ? (double)height / width : 1.0;
    
    // Calculate features
    double total_pixels = count_pixels(img, 0, 0, h, w);
    
    // Divide image into regions
    double top_half = count_pixels(img, 0, 0, h/2, w);
    double bottom_half = count_pixels(img, h/2, 0, h, w);
    double left_half = count_pixels(img, 0, 0, h, w/2);
    double right_half = count_pixels(img, 0, w/2, h, w);
    
    double top_third = count_pixels(img, 0, 0, h/3, w);
    double middle_third = count_pixels(img, h/3, 0, 2*h/3, w);
    double bottom_third = count_pixels(img, 2*h/3, 0, h, w);
    
    double center = count_pixels(img, h/4, w/4, 3*h/4, 3*w/4);
    
    // Center of mass
    double cy, cx;
    center_of_mass(img, cy, cx);
    
    // Symmetry
    double h_sym = horizontal_symmetry(img);
    double v_sym = vertical_symmetry(img);
    
    // Holes
    int holes = count_holes(img);
    
    // Ratios
    double top_bottom_ratio = (bottom_half > 0) ? top_half / bottom_half : 0;
    double left_right_ratio = (right_half > 0) ? left_half / right_half : 0;
    
    // Decision tree based on features
    
    // 0: Has a hole, symmetric
    if (holes >= 3 && h_sym < 0.15) {
        return 0;
    }
    
    // 1: Tall and thin, most pixels on right or center
    if (aspect_ratio > 1.5 && width < height * 0.6) {
        return 1;
    }
    
    // 8: Two holes, symmetric both ways
    if (holes >= 4 && h_sym < 0.15 && v_sym < 0.15) {
        return 8;
    }
    
    // 6: Hole in bottom half, top-heavy
    if (holes >= 2 && bottom_half > top_half * 1.2 && cy > h * 0.5) {
        return 6;
    }
    
    // 9: Hole in top half, bottom-heavy in reverse
    if (holes >= 2 && top_half > bottom_half * 1.1 && cy < h * 0.5) {
        return 9;
    }
    
    // 4: Open on right side, high center of mass on left
    if (left_half > right_half * 1.3 && middle_third > bottom_third) {
        return 4;
    }
    
    // 7: Top-heavy, thin bottom
    if (top_third > middle_third * 1.3 && top_third > bottom_third * 1.5) {
        return 7;
    }
    
    // 2: Bottom-heavy, more pixels at bottom
    if (bottom_half > top_half * 1.2 && bottom_third > top_third) {
        return 2;
    }
    
    // 3: Two bumps on right, center-right heavy
    if (right_half > left_half * 1.2 && middle_third > top_third * 0.8) {
        return 3;
    }
    
    // 5: Top-heavy with flat top
    if (top_half > bottom_half && left_half > right_half * 0.9) {
        return 5;
    }
    
    // Default fallback based on simple heuristics
    if (holes >= 3) return 8;
    if (holes >= 2) return 0;
    if (aspect_ratio > 1.8) return 1;
    if (top_third > bottom_third * 1.5) return 7;
    if (bottom_third > top_third * 1.3) return 2;
    
    return 0;
}

#endif // SRC_HPP
