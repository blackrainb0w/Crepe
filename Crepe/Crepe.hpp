//
//  Crepe.hpp
//  Crepe
//
//  Created by Samuel Prevost on 29/12/2016.
//  Copyright © 2016 Samuel Prevost. All rights reserved.
//

#ifndef Crepe_hpp
#define Crepe_hpp

#include <iostream>
#include <map>

using std::map;

enum class CrepePiece: unsigned int { WHITE, BLACK } ;

class Crepe {
private:
    // Every pieces of crepe, first int is the angle, second is the state of the piece (black|white)
    map<int, CrepePiece> pieces;
    typedef map<int, CrepePiece>::iterator pieceItr;
    
    int pieceAngle;
    int maxAngle;
    
    void init();
    
public:
    /* Constructor/Destructor **/
    Crepe(int angle, int maxAngle);
    // Not implemented
    // ~Crepe();
    
    /* Accessors **/
    // Getters
    int black();
    int white();
    map<int, CrepePiece> getPieces();
    int getAngle();
    int getMaxAngle();
    CrepePiece getPiece(int angle);
    // Setters
    
    
    void flip(int angleOfPiece);
    // Try avoid using this one since it's not meant to be used this way
    void flipAtIndex(int indexAngle);
    
};

#endif /* Crepe_hpp */
