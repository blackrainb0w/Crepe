//
//  CrepeRenderer.cpp
//  Crepe
//
//  Created by Samuel Prevost on 29/12/2016.
//  Copyright © 2016 Samuel Prevost. All rights reserved.
//

#include "CrepeRenderer.hpp"
#include "Crepe.hpp"

CrepeRenderer::CrepeRenderer(int width, int height, string title, Point center){
    this->width = width;
    this->height = height;
    if(center.x < 0 || center.y < 0)
        // Default center
        this->center = {(double)width/2, (double)height/2};
    this->title = title;
    ContextSettings settings;
    settings.antialiasingLevel = 8;
    this->window = new RenderWindow(VideoMode(this->width, this->height), this->title, Style::Default, settings);
}

void CrepeRenderer::render(Crepe* crepe, RenderMode renderMode, int radius, int numberOfLines){
    if(this->window->isOpen()){
        if(radius < 0)
            radius = height/2;
        
        string whites  = std::to_string(crepe->white());
        string blacks  = std::to_string(crepe->black());
        string numberOfPieces = std::to_string(crepe->getPieces().size());
        string angle           = std::to_string(crepe->getAngle());
        string windowTitle = this->title + " @ " + numberOfPieces + " pieces (" + whites + " whites, " + blacks + " blacks) " + angle + " deg ";
        // Will loop forever until window is closed
        if(renderMode == RenderMode::POLYGONS){
            this->window->setTitle(windowTitle + "[POLYGONS]");
            this->render(this->GeneratePolygonPieces(crepe->getPieces(), this->center, radius));
        }else if (renderMode == RenderMode::LINES){
            this->window->setTitle(windowTitle + "[LINES]");
            this->render(this->GenerateLinePieces(crepe->getPieces(), this->center, numberOfLines, radius, 360));
        }
    }else{
        // Re-create the window and retry
        this->window->create(VideoMode(this->width, this->height), this->title);
        this->render(crepe, renderMode, radius, numberOfLines);
    }
}

void CrepeRenderer::render(vector<ConvexShape> elements){
    while (this->window->isOpen())
    {
        // Process events
        Event event;
        while (this->window->pollEvent(event))
        {
            // Close window: exit
            if (event.type == Event::Closed) {
                this->window->close();
            }
            
            // Escape pressed: exit
            if (event.type == Event::KeyPressed && (event.key.code == Keyboard::Escape || event.key.code == Keyboard::Right )) {
                this->window->close();
            }
        }
        
        // Clear screen
        this->window->clear();
        
        // Draw every elements (ConcaveShape-s)
        for(int i = 0; i < elements.size(); i++){
            this->window->draw(elements[i]);
        }
        
        // Update the window
        this->window->display();
    }
}

vector<ConvexShape> CrepeRenderer::GeneratePolygonPieces(map<int, CrepePiece> pieces, Point center, int radius){
    // Contient les coordonés de tout les points formant un polygone régulier de pieces.size() sommets (a.k.a vertex)
    vector<Point> pointsCoords = this->RegularPolygon(center, radius, (int)pieces.size());
    // Contiendra les pieces.size() pièces de crepe
    vector<ConvexShape> piecesShapes;
    // 2 points + center = 3 points (triangle)
    for (int i = 0; i < pieces.size(); i++) {
        ConvexShape convex;
        // number of points (3 per piece (a.k.a triangle))
        convex.setPointCount(3);
        
        /* Add points to polygon **/
        // First point
        convex.setPoint(0, Vector2f(pointsCoords[i].x, pointsCoords[i].y));
        // Second point (the next point in pointsCoords)
        if(i != pieces.size()-1)
            convex.setPoint(1, Vector2f(pointsCoords[i+1].x, pointsCoords[i+1].y));
        else
            // If there is no next point, so the next is the first
            convex.setPoint(1, Vector2f(pointsCoords[0].x, pointsCoords[0].y));
        
        // Third point (the center)
        convex.setPoint(2, Vector2f(center.x, center.y));
        
        // 360/pieces.size()*i --> the angle we are rendering in this iteration
        if(pieces[360/pieces.size()*i] == CrepePiece::WHITE)
            // White
            convex.setFillColor(RAW_COLOR);
        else
            // Brown
            convex.setFillColor(COOKED_COLOR);
        
        // Outline color
        convex.setOutlineThickness(OUTLINE_WEIGHT);
        convex.setOutlineColor(OUTLINE_COLOR);
        
        piecesShapes.push_back(convex);
    }
    return piecesShapes;
}

vector<Point> CrepeRenderer::RegularPolygon(Point center, int radius, int numberOfVertex){
    vector<Point> points;
    for (int i = 0; i < numberOfVertex; i++) {
        double x_coord = center.x + radius * cos(2 * M_PI * i / numberOfVertex);
        double y_coord = center.y + radius * sin(2 * M_PI * i / numberOfVertex);
        
        points.push_back((Point){x_coord, y_coord});
    }
    return points;
}


vector<ConvexShape> CrepeRenderer::GenerateLinePieces(map<int, CrepePiece> pieces, Point center, int numberOfLines, int radius, int maxAngle){
    
    // Get vertices of every lines to draw a circle
    map<int, Point> points = this->LinesCircle(center, radius, numberOfLines, maxAngle);
    
    // Will contain all of our lines
    vector<ConvexShape> lines;
    
    // Loop through every points
    for (map<int, Point>::iterator itr = points.begin(); itr != points.end(); itr++) {
        ConvexShape thisLine;
        //it's a rect after all
        thisLine.setPointCount(4);
    
        thisLine.setPoint(0, Vector2f(
                                      center.x,
                                      center.y
                                      ));
        thisLine.setPoint(1, Vector2f(
                                      itr->second.x,
                                      itr->second.y
                                      ));
    
        { // new scope
            map<int, Point>::iterator nextItr = itr;
            nextItr++;
            thisLine.setPoint(2, Vector2f(
                                          nextItr->second.x,
                                          nextItr->second.y
                                          ));
            thisLine.setPoint(3, Vector2f(
                                          center.x + (nextItr->second.x - itr->second.x),
                                          center.y + (nextItr->second.y - itr->second.y)
                                          ));
        }
            
        for (map<int, CrepePiece>::iterator itrPieces = pieces.begin(); itrPieces != pieces.end(); itrPieces++) {
            // Draw lines between pieces, but they arn't quite sharp
//            if(labs((long)(itr->first - itrPieces->first)) < 1 ){
//                thisLine.setFillColor(OUTLINE_COLOR);
////                thisLine.setOutlineColor(OUTLINE_COLOR);
////                thisLine.setOutlineThickness(0.5);
//                break;
//            }
            if(itr->first < itrPieces->first){
                if(itrPieces->second == CrepePiece::WHITE){
                    thisLine.setFillColor(RAW_COLOR);
                    break;
                }else{
                    thisLine.setFillColor(COOKED_COLOR);
                    break;
                }
            }
        }
        
        lines.push_back(thisLine);
    }
    
    // Draw lines between each pieces
    
    points = this->LinesCircle(center, radius, (int)pieces.size(), maxAngle);
    for (map<int, Point>::iterator itr = points.begin(); itr != points.end(); itr++) {
        ConvexShape thisLine;
        //it's a rect after all
        thisLine.setPointCount(4);
        
        thisLine.setPoint(0, Vector2f(
                                      center.x,
                                      center.y
                                      ));
        thisLine.setPoint(1, Vector2f(
                                      itr->second.x,
                                      itr->second.y
                                      ));
        

        thisLine.setPoint(2, Vector2f(
                                      itr->second.x,
                                      itr->second.y
                                      ));
        thisLine.setPoint(3, Vector2f(
                                      center.x,
                                      center.y
                                      ));
        
        thisLine.setOutlineThickness(OUTLINE_WEIGHT);
        thisLine.setOutlineColor(OUTLINE_COLOR);
        thisLine.setFillColor(OUTLINE_COLOR);
        lines.push_back(thisLine);
    }
    
    return lines;
}

map<int,Point> CrepeRenderer::LinesCircle(Point center, int radius, int numberOfPieces, int maxAngle){
    map<int,Point> points;
    // currAngle = each angle from 0 to maxAngle with step of equal angle (floored to nearest int)
    for (int currAngle = 0; currAngle < maxAngle; currAngle += maxAngle/numberOfPieces) {
        double x_coord = center.x + radius * cos(currAngle*M_PI/180);
        double y_coord = center.y + radius * sin(currAngle*M_PI/180);
        points[currAngle] = (Point){x_coord, y_coord};
    }
    return points;
}
