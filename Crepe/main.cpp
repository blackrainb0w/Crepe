//
//  main.cpp
//  Crepe
//
//  Created by Samuel Prevost on 29/12/2016.
//  Copyright © 2016 Samuel Prevost. All rights reserved.
//

#include <iostream>
#include "Crepe.hpp"
#include "CrepeRenderer.hpp"

using std::cout;
using std::endl;
using std::string;

int main(int argc, const char * argv[]) {
    
    // Angle of 2° => 360/2 = 180 pieces
    Crepe* crepe = new Crepe(20, 360);
    cout << crepe->black() << " black pieces" << endl;
    cout << crepe->white() << " white pieces" << endl;
    
    // Flip 1/2 piece
    for(int i = 0; i < crepe->getPieces().size(); i++){
        // if i even flip the angle*i piece
        if (i%2)
            crepe->flip(crepe->getAngle()*i);
    }
    
    cout << crepe->black() << " black pieces" << endl;
    cout << crepe->white() << " white pieces" << endl;
    
    CrepeRenderer* crepe_r = new CrepeRenderer(1600, 1600, "Crepe Renderer 1");
    crepe_r->render(crepe, RenderMode::LINES, 800, 300);
    crepe_r->render(crepe, RenderMode::POLYGONS);
    for(int j = 0; j < 3; j++){
        // Flip 1/2 piece
        for(int i = 0; i < crepe->getPieces().size(); i++){
            // if i even flip the angle*i piece
            if (i%3)
                crepe->flip(crepe->getAngle()*i);
        }
        crepe_r->title = "Crepe Renderer 2";
        crepe_r->render(crepe, RenderMode::LINES, 800, 300);
        crepe_r->render(crepe, RenderMode::POLYGONS);
    }
    return EXIT_SUCCESS;
}
