//
//  Crepe.cpp
//  Crepe
//
//  Created by Samuel Prevost on 29/12/2016.
//  Copyright © 2016 Samuel Prevost. All rights reserved.
//

#include "Crepe.hpp"

Crepe::Crepe(int angle, int maxAngle){
    // Avoid dividing by zzzzzzzzero
    if(angle == 0 || maxAngle == 0)
        exit(EXIT_FAILURE);
    this->pieceAngle    = angle;
    this->maxAngle      = maxAngle;
    
    // Initialize empty crepe (all white)
    this->init();
}

int Crepe::black(){
    int blackCount = 0;
    // Crawl through every pieces and count the black ones
    for(pieceItr itr = this->pieces.begin(); itr != this->pieces.end(); itr++){
        if(itr->second == CrepePiece::BLACK)
            blackCount++;
    }
    return blackCount;
}

int Crepe::white(){
    int whiteCount = 0;
    // Crawl through every pieces and count the white ones
    for(pieceItr itr = this->pieces.begin(); itr != this->pieces.end(); itr++){
        if(itr->second == CrepePiece::WHITE)
            whiteCount++;
    }
    return whiteCount;
}

void Crepe::init(){
    // Init all pieces to white
    for (int currentAngle = 0; currentAngle < this->maxAngle; currentAngle += this->pieceAngle) {
        this->pieces[currentAngle] = CrepePiece::WHITE;
    }
}

void Crepe::flip(int angleOfPiece){
    // If we found our key
    if(this->pieces.find(angleOfPiece) != this->pieces.end()){
        CrepePiece* ourPiece = &this->pieces[angleOfPiece];
        if(*ourPiece == CrepePiece::BLACK)
            *ourPiece = CrepePiece::WHITE;
        else
            *ourPiece = CrepePiece::BLACK;
    }
}

void Crepe::flipAtIndex(int indexAngle){
    // Very inefficient, try avoid using this method
    int i = 0;
    for(pieceItr itr = this->pieces.begin(); itr != pieces.end(); itr++){
        i++;
        if(i == indexAngle)
            this->flip(itr->first);
    }
}

map<int, CrepePiece> Crepe::getPieces(){
    return this->pieces;
}

int Crepe::getAngle(){
    return this->pieceAngle;
}

int Crepe::getMaxAngle(){
    return this->maxAngle;
}
