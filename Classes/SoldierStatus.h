//
//  SoldierStatus.h
//  Kadai01
//
//  Created by 菊池　裕太 on 2014/04/15.
//
//

#include "cocos2d.h"
#include "cocos-ext.h"

#ifndef __Kadai01__SoldierStatus__
#define __Kadai01__SoldierStatus__

#define SOLDIER_HORIZONTAL 7
#define SOLDIER_VERTICAL 5
#define SOLDIER_ELEMENT 6

struct SoldierStatus{
private:
    bool animationFlg;
    bool changeFlg;
    bool waitFlg;
    int soldierMap[SOLDIER_HORIZONTAL][SOLDIER_VERTICAL];
    cocos2d::CCSprite* spriteMap[SOLDIER_HORIZONTAL][SOLDIER_VERTICAL];
    int eraseFlg[SOLDIER_HORIZONTAL][SOLDIER_VERTICAL];
public:
    void setSoldier(int soldier,int x,int y){
        soldierMap[x][y] = soldier;
    }
    int getSoldier(int x,int y){
        return soldierMap[x][y];
    }
    void setChangeFlg(bool flg){
        changeFlg = flg;
    }
    bool getChangeFlg(){
        return changeFlg;
    }
    void setAnimationFlg(bool flg){
        animationFlg = flg;
    }
    bool getAnimationFlg(){
        return animationFlg;
    }
    void setSprite(cocos2d::CCSprite* sprite,int x,int y){
        spriteMap[x][y] = sprite;
    }
    cocos2d::CCSprite* getSprite(int x,int y){
        return spriteMap[x][y];
    }
    void setEraseFlg(int combo,int x,int y){
        eraseFlg[x][y] = combo;
    }
    int getEraseFlg(int x,int y){
        return eraseFlg[x][y];
    }
    void setWaitFlg(bool flg){
        waitFlg = flg;
    }
    bool getWaitFlg(){
        return waitFlg;
    }
};

#endif