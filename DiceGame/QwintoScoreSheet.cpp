//
//  QwintoScoreSheet.cpp
//  DiceGame
//
//  Created by Adel Araji on 2017-11-28.
//  Copyright © 2017 Adel Araji. All rights reserved.
//

#include <stdio.h>
#include "QwintoScoreSheet.h"

/**
    Validates if the score is posssible for a certain roll of dice
 */
bool QwintoScoreSheet::validate(RollOfDice rd, Colour cl, int positionFromLeft){
    
    //Condition 1: is it posible to add in the chosen row
    bool condition1 = false;
    if (cl == RED)
        condition1 = redRow.validate(rd, positionFromLeft);
    else if (cl == YELLOW)
        condition1 = yellowRow.validate(rd, positionFromLeft);
    else if (cl == BLUE)
        condition1 = blueRow.validate(rd, positionFromLeft);
    
    //Condition 2: is it possible compared to the surrounding rows
    bool condition2 = false;
    if (cl == RED){
        switch (positionFromLeft) {
            case 9:
                condition2 = true;
                break;
            case 8:
                if(redRow[positionFromLeft] != yellowRow[positionFromLeft+1])
                    condition2 = true;
                break;
            default:
                if((yellowRow[positionFromLeft+1] != redRow[positionFromLeft]) && (blueRow[positionFromLeft+2] != redRow[positionFromLeft]))
                    condition2 = true;
                break;
        }
            
        
    }else if (cl == YELLOW){
        switch (positionFromLeft) {
            case 0:
                if(yellowRow[positionFromLeft] != blueRow[positionFromLeft+1])
                    condition2 = true;
                break;
            case 9:
                if(yellowRow[positionFromLeft] != redRow[positionFromLeft-1])
                    condition2 = true;
                break;
            default:
                if((yellowRow[positionFromLeft] != redRow[positionFromLeft-1]) && (blueRow[positionFromLeft+1] != yellowRow[positionFromLeft]))
                    condition2 = true;
                break;
        }
        
    }else if (cl == BLUE){
        switch (positionFromLeft) {
            case 0:
                condition2 = true;
                break;
            case 1:
                if(blueRow[positionFromLeft] != yellowRow[positionFromLeft-1])
                    condition2 = true;
                break;
            default:
                if((blueRow[positionFromLeft] != redRow[positionFromLeft-2]) && (blueRow[positionFromLeft] != yellowRow[positionFromLeft-1]))
                    condition2 = true;
                break;
        }
        
    }
        
    return (condition1 && condition2);
}


/**
    Returns the total score for Qwinto
 */
int QwintoScoreSheet::calcTotal(){
    //Checking if any of the rows are full or not and get number of entries in each row
    bool isRedRowFull = false;
    bool isBlueRowFull = false;
    bool isYellowRowFull = false;
    
    int redCounter = 0, blueCounter = 0, yellowCounter = 0;
    for (int i = 0; i < 10; i++){
        if (redRow[i] != 0)
            redCounter++;
        if(yellowRow[i] != 0)
            yellowCounter++;
        if(blueRow[i] != 0)
            blueCounter++;
    }
    
    if(redCounter == 9)
        isRedRowFull = true;
    else if (blueCounter == 9)
        isBlueRowFull = true;
    else if (yellowCounter == 9)
        isYellowRowFull = true;
    
    //Checking for Bonus
    bool doBonusExist = false;
    
    //Vector of bonuses
    std::vector<int> arrayOfBonusIndex;
    
    for (int i = 0; i < 8; i++){
        if ((redRow[i] != 0) && (yellowRow[i] != 0) && (blueRow[i] != 0)){
            doBonusExist = true;
            arrayOfBonusIndex.push_back(findMax(redRow[i], yellowRow[i], blueRow[i]));
        }
    }
    
    
    //Final calculation of score
    int finalScore = 0;
    
    //Getting final value if any row is full
    if(isRedRowFull)
        finalScore += redRow[9];
    else if (isYellowRowFull)
        finalScore += yellowRow[9];
    else if (isBlueRowFull)
        finalScore += blueRow[9];
    
    //Getting number of entries
    for (int i = 0; i < 10; i++){
        if (!isRedRowFull)
            finalScore += redCounter;
        else if (!isBlueRowFull)
            finalScore += blueCounter;
        else if (!isYellowRowFull)
            finalScore += yellowCounter;
    }
    
    //Adding bonus
    for (std::vector<int>::const_iterator i = arrayOfBonusIndex.begin(); i != arrayOfBonusIndex.end(); ++i)
        finalScore += arrayOfBonusIndex.at(*i);
    
    //Substracting failed throws
    int failedAttempts = ScoreSheet::getNumberOfFailedAttempts();
    finalScore = finalScore - failedAttempts * 5;
    
    //Setting Final Score in ScoreSheet, still unsusre about this ????
    ScoreSheet::setFinalScore(finalScore);
    
    return finalScore;
}

/**
    Overloading output operator to print the Qwinto ScoreSheet
 */
std::ostream& operator<< (std::ostream& os, QwintoScoreSheet qw){
    
    os << "Player name: " << qw.playerName << "          Points: " << qw.calcTotal();
    os << "---------------------------------------";
    os << qw.redRow;
    os << "---------------------------------------";
    os << qw.yellowRow;
    os << "---------------------------------------";
    os << qw.blueRow;
    os << "---------------------------------------";
    os << "Failed throws: " << qw.failedAttempts;
    
    return os;
}


/**
    Function to get max of 3 integers
 */
int QwintoScoreSheet::findMax(int a, int b, int c){
    int max = 0;
    int maxOf2 = 0;
    
    if (a >b)
        maxOf2 = a;
    else
        maxOf2 = b;
    
    if (c > maxOf2)
        max = c;
    else
        max = maxOf2;
    
    return max;
    
}

