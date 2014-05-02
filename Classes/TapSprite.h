//
//  TapSprite.h
//  Kadai01
//
//  Created by 菊池　裕太 on 2014/04/16.
//
//

#ifndef _Recipe37_TapSprite_H_
#define _Recipe37_TapSprite_H_

#include <iostream>
#include <vector>
#include <stdlib.h>

#include "cocos2d.h"
#include "SoldierStatus.h"
//#include "SingletonStatus.h"

NS_CC_BEGIN

class TapSprite : public CCSprite, public CCTargetedTouchDelegate
{
private:
    SoldierStatus* status;
    CCNode* parent;
    int thisx;
    int thisy;
    int loopx;
    int loopy;
    int combo;
    
    int animation;
    bool upSoldier;
    bool fillSoldier;
    
    CCSprite* redlineSprite;
    CCSequence* wait;
    
    void setChangeFlg();
    
    void changeSoldier(int x,int y);
    bool moveSoldier(CCPoint point,int xDest,int yDest);
    
    // 兵士削除関連
    bool eraseSoldier();
    void eraseSoldierMap(int x,int y);
    void finishMove();
    void waitOver();
    
    std::vector<CCLabelTTF*> labelVec;
    
    //bool eraseHorizontalAnim();
    //bool eraseVerticalAnim();
    bool eraseHorizontalAnim(int x,int y);
    bool eraseVerticalAnim(int x,int y);
    void eraseSoldierAnim(int x,int y);
    
    // 兵士補充
    void fillSoldierAnim();
    void fillSoldierEnd();
    void restartErase();
    
    void labelClear();
public:
    
    static TapSprite* create(const char *pszFileName);
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    void setPlace(int x,int y);
    void setStatus(SoldierStatus* soldierStatus);
    
    void eraseSoldierDelay(CCObject* sprite);
    void eraseSoldierInstance(CCObject* sprite);    void endFill();

};

NS_CC_END

#endif // _Recipe37_TapSprite_H_
