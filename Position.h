/* 
 * File:   polohaClass.h
 * Author: root
 *
 * Created on Sobota, 2015, január 24, 10:45
 * 
 * for storing position of agent
 */

#ifndef POLOHACLASS_H
#define	POLOHACLASS_H

#include <rapidjson/document.h>
#include <cstdio>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <iostream>

#include "CoordinationPosition.h"

class Position {
public:
    Position(int id, int id_spustenia, int robot, float x, float y, float fi);
    virtual ~Position();
    
    const char * toJson();
    static Position * fromJson(const char *json);
    const char * toString();
    
    float getDistance(Position *p2);
    float getDistance(CoordinationPosition *koorSur);
    
    float GetFi();
    void SetFi(float fi);
    int GetId();
    void SetId(int id);
    int GetId_mapping();
    void SetId_mapping(int id_spustenia);
    int GetRobot();
    void SetRobot(int robot);
    float GetX();
    void SetX(float x);
    float GetY();
    void SetY(float y);
private:
    int id;
    int id_spustenia;
    int robot;
    float x;
    float y;
    float fi;
};

#endif	/* POLOHACLASS_H */

