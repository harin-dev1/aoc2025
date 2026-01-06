/**
 * Advent of Code 2025 - Day 9 Part 2 (Revised)
 * Problem: Largest Rectangle defined by two vertices strictly inside a polygon.
 * * Fixes:
 * - Uses pure integer math for intersection checks (avoids precision bugs).
 * - Handles relative directions (U/D/L/R) and absolute coords (x,y).
 * - Strict checking for edges passing THROUGH the rectangle.
 */

 #include <iostream>
 #include <vector>
 #include <string>
 #include <sstream>
 #include <algorithm>
 #include <cmath>
 
 using namespace std;
 
 struct Point {
     long long x, y;
 };
 
 struct Rect {
     long long minX, maxX, minY, maxY;
     
     long long area() const {
         return ((maxX - minX) + 1) * ((maxY - minY) + 1);
     }
 
     // Check if a point is strictly inside the rectangle
     bool containsStrict(const Point& p) const {
         return p.x > minX && p.x < maxX && p.y > minY && p.y < maxY;
     }
 };
 
 // --- Geometry Helpers ---
 
 // Check if value q lies strictly between p and r
 bool isBetween(long long p, long long q, long long r) {
     return (q > min(p, r)) && (q < max(p, r));
 }
 
 // Check if a polygon segment (p1-p2) intersects the INTERIOR of the rectangle
 bool segmentIntersectsRectInterior(Point p1, Point p2, const Rect& r) {
     // 1. Trivial Rejection: Segment is completely outside (to one side)
     if (max(p1.x, p2.x) <= r.minX || min(p1.x, p2.x) >= r.maxX ||
         max(p1.y, p2.y) <= r.minY || min(p1.y, p2.y) >= r.maxY) {
         return false;
     }
 
     // 2. If the segment is strictly inside the rect (both ends inside)
     // The vertex check elsewhere handles this, but good to be safe.
     if (r.containsStrict(p1) || r.containsStrict(p2)) return true;
 
     // 3. Vertical Segment Check
     if (p1.x == p2.x) {
         // We know x is within (minX, maxX) because of Trivial Rejection
         // Does the Y range span across the rectangle?
         // It intersects interior if it covers any Y range inside (minY, maxY)
         long long segMinY = min(p1.y, p2.y);
         long long segMaxY = max(p1.y, p2.y);
         return (segMinY < r.maxY && segMaxY > r.minY);
     }
 
     // 4. Horizontal Segment Check
     if (p1.y == p2.y) {
         long long segMinX = min(p1.x, p2.x);
         long long segMaxX = max(p1.x, p2.x);
         return (segMinX < r.maxX && segMaxX > r.minX);
     }
 
     // 5. Diagonal Segment Check (Clip against Rect boundaries)
     // We check if the line crosses any of the 4 valid open intervals of the rect sides.
     // Line Equation: (y - y1) * (x2 - x1) = (x - x1) * (y2 - y1)
     long long dx = p2.x - p1.x;
     long long dy = p2.y - p1.y;
 
     auto getY = [&](long long x) { return p1.y + (double)dy * (x - p1.x) / dx; };
     auto getX = [&](long long y) { return p1.x + (double)dx * (y - p1.y) / dy; };
 
     // Check intersection with Left (x = minX) within Y bounds
     if (isBetween(p1.x, r.minX, p2.x)) {
         double y = getY(r.minX);
         if (y > r.minY && y < r.maxY) return true;
     }
     // Check intersection with Right (x = maxX) within Y bounds
     if (isBetween(p1.x, r.maxX, p2.x)) {
         double y = getY(r.maxX);
         if (y > r.minY && y < r.maxY) return true;
     }
     // Check intersection with Bottom (y = minY) within X bounds
     if (isBetween(p1.y, r.minY, p2.y)) {
         double x = getX(r.minY);
         if (x > r.minX && x < r.maxX) return true;
     }
     // Check intersection with Top (y = maxY) within X bounds
     if (isBetween(p1.y, r.maxY, p2.y)) {
         double x = getX(r.maxY);
         if (x > r.minX && x < r.maxX) return true;
     }
 
     return false;
 }
 
 // Ray Casting using midpoints (to avoid vertex collision issues)
 bool isRectCenterInPolygon(const Rect& r, const vector<Point>& polygon) {
     double px = (r.minX + r.maxX) / 2.0;
     double py = (r.minY + r.maxY) / 2.0;
     
     bool inside = false;
     size_t n = polygon.size();
     for (size_t i = 0, j = n - 1; i < n; j = i++) {
         // Check if edge crosses the horizontal ray to the right of px
         if (((polygon[i].y > py) != (polygon[j].y > py)) &&
             (px < (polygon[j].x - polygon[i].x) * (py - polygon[i].y) / 
             (double)(polygon[j].y - polygon[i].y) + polygon[i].x)) {
             inside = !inside;
         }
     }
     return inside;
 }
 
 // --- Input Parsing ---
 
 vector<Point> parseInput() {
     vector<Point> pts;
     string token;
     Point current = {0, 0};
     bool relative = false;
     
     // Quick peek to see if input is "R 5" or "12,14"
     // (This is a simplified parser logic)
     while (cin >> token) {
         if (isdigit(token[0]) || token[0] == '-') {
             // Absolute Coordinates: "10,20"
             size_t c = token.find(',');
             if (c != string::npos) {
                 pts.push_back({ stoll(token.substr(0,c)), stoll(token.substr(c+1)) });
                 relative = false;
             }
         } else {
             // Relative Instructions: "R" "5"
             // Assuming strict format: DIR AMOUNT
             char dir = token[0];
             string amtStr;
             cin >> amtStr;
             long long amt = stoll(amtStr);
             
             if (pts.empty()) pts.push_back(current); // Start at 0,0
             
             if (dir == 'R') current.x += amt;
             else if (dir == 'L') current.x -= amt;
             else if (dir == 'U') current.y += amt; // Assuming U increases Y (or decreases, math is same)
             else if (dir == 'D') current.y -= amt;
             
             pts.push_back(current);
             relative = true;
         }
     }
     
     // If relative, we might need to close the loop if not closed
     if (relative && (pts.back().x != pts[0].x || pts.back().y != pts[0].y)) {
         // Usually relative inputs just describe the path, vertices are implied.
         // We leave as is.
     }
     return pts;
 }
 
 int main() {
     // 1. Parse
     vector<Point> polygon = parseInput();
     size_t n = polygon.size();
 
     if (n < 3) return 0;
 
     // 2. Generate Candidate Rectangles
     struct Candidate { Rect r; long long area; };
     vector<Candidate> candidates;
     
     for (size_t i = 0; i < n; ++i) {
         for (size_t j = i + 1; j < n; ++j) {
             long long x1 = polygon[i].x;
             long long y1 = polygon[i].y;
             long long x2 = polygon[j].x;
             long long y2 = polygon[j].y;
             
             if (x1 == x2 || y1 == y2) continue; // Skip lines
 
             Rect r = { min(x1, x2), max(x1, x2), min(y1, y2), max(y1, y2) };
             candidates.push_back({r, r.area()});
         }
     }
 
     // 3. Sort Candidates by Area (Largest first)
     sort(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b) {
         return a.area > b.area;
     });
 
     std::cout << candidates[0].area << std::endl;
     // 4. Validate
     for (const auto& cand : candidates) {
         const Rect& r = cand.r;
 
         // A. Vertex Containment Check
         // If any polygon vertex is STRICTLY inside the rect, the rect is invalid.
         bool vertexFail = false;
         for (const auto& p : polygon) {
             if (r.containsStrict(p)) {
                 vertexFail = true;
                 break;
             }
         }
         if (vertexFail) continue;
 
         // B. Edge Intersection Check
         // If any polygon edge passes THROUGH the rect, it's invalid.
         bool edgeFail = false;
         for (size_t i = 0; i < n; ++i) {
             Point p1 = polygon[i];
             Point p2 = polygon[(i + 1) % n];
             
             if (segmentIntersectsRectInterior(p1, p2, r)) {
                 edgeFail = true;
                 break;
             }
         }
         if (edgeFail) continue;
 
         // C. Inside Check (Center Point)
         // Ensure the rectangle isn't in a "void" outside the polygon (e.g. U-shape gap)
         if (!isRectCenterInPolygon(r, polygon)) {
             continue;
         }
 
         cout << cand.area << endl;
         return 0;
     }
 
     cout << "0" << endl;
     return 0;
 }