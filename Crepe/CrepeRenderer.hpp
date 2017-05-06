//
//  CrepeRenderer.hpp
//  Crepe
//
//  Created by Samuel Prevost on 29/12/2016.
//  Copyright © 2016 Samuel Prevost. All rights reserved.
//

#ifndef CrepeRenderer_hpp
#define CrepeRenderer_hpp
#define _USE_MATH_DEFINES

#define RAW_COLOR Color(255,255,255)
#define COOKED_COLOR Color(165,42,42)

#define OUTLINE_COLOR Color(0,0,0)
#define OUTLINE_WEIGHT 1
#include <iostream>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <math.h>
//#include <thread>
//#include <functional>

using std::string;
using std::vector;
using std::map;
//using std::thread;

using namespace sf;

struct Point{
    double x;
    double y;
};

// from Crepe.hpp, forward declarations
class Crepe;
enum class CrepePiece : unsigned int;

enum class RenderMode : unsigned int { POLYGONS, LINES };

class CrepeRenderer{
private:
    RenderWindow* window;
    vector<Point> RegularPolygon(Point center, int radius, int numberOfVertex);
    // int is the angle, second is the end of the line (center <--> Point)
    map<int,Point> LinesCircle(Point center, int radius, int numberOfLines, int maxAngle);
    vector<ConvexShape> GeneratePolygonPieces(map<int, CrepePiece> pieces, Point center, int radius);
    vector<ConvexShape> GenerateLinePieces(map<int, CrepePiece> pieces, Point center, int numberOfLines, int radius, int maxAngle = 360);
    
    void render(vector<ConvexShape> elements);
    
public:
    CrepeRenderer(int width, int height, string title, Point center = {-1, -1});
    void render(Crepe* crepe, RenderMode renderMode = RenderMode::POLYGONS, int radius = -1, int numberOfLines = 200);
    
    Point center;
    int width = 800, height = 800;
    string title;
};

#endif /* CrepeRenderer_hpp */
