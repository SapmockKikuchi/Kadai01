//
//  TapSprite.cpp
//  Kadai01
//
//  Created by 菊池　裕太 on 2014/04/16.
//
//

#include <math.h>
#include "MainScene.h"
#include "TapSprite.h"

NS_CC_BEGIN

TapSprite* TapSprite::create(const char *pszFileName)
{
    TapSprite *pobSprite = new TapSprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

void TapSprite::onEnter()
{
    CCSprite::onEnter();
    CCTouchDispatcher* dispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    dispatcher->addTargetedDelegate(this, 0, true);
}

void TapSprite::onExit() {
    CCSprite::onExit();
    CCTouchDispatcher* dispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    dispatcher->removeDelegate(this);
}

bool TapSprite::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(status->getAnimationFlg()) return false;
    
    // タッチ開始処理
    
    // タッチ位置を取得
    CCPoint point = pTouch->getLocation();
    // スプライトの大きさ
    CCSize size = this->getTexture()->getContentSize();
    // スプライトの範囲
    CCRect rect = CCRectMake(this->getPositionX()-size.width/2, this->getPositionY()-size.height/2, size.width, size.height);
    
    // タッチ位置がスプライト内ならイベント処理するのでtrueを返す
    if(rect.containsPoint(point)){
        status->setWaitFlg(false);
        status->setChangeFlg(false);
        CCNode* parent;
        CCSprite* sprite;
        sprite = status->getSprite(thisx,thisy);
        parent = sprite->getParent();
        parent->removeChild(sprite);
        parent->addChild(sprite);
        //this->setPosition(point);
        this->setPosition(ccp(point.x,point.y + 25));
        return true;
    }else{
        return false;
    }
}

void TapSprite::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if(status->getWaitFlg()) return;
    // タッチ移動処理
    CCPoint point = pTouch->getLocation();
    this->setPosition(ccp(point.x,point.y + 25));
    //float nowx = point.x;
    //float nowy = point.y;
    CCSize size = this->getTexture()->getContentSize();
    //CCPoint beforePoint = ccp((size.width * thisx) + (size.width/2) + 5, (size.height * thisy) + size.height/2);
    
    int dest = SOLDIER_HORIZONTAL - 1;
    
    // まずマスを4分割する
    if(pTouch->getLocation().x > (size.width * thisx) + (size.width/2) + 5){
        // 現在のタッチポイントから移動先が3つにしぼれるので、そこに対しての移動判定が発生しているかどうかを判定
        if(pTouch->getLocation().y > (size.height * thisy) + size.height/2){
            //CCLOG("++ %d,%d",thisx,thisy);
            while(dest > 0){
                if(thisx + dest < SOLDIER_HORIZONTAL && thisy + dest < SOLDIER_VERTICAL){
                    if(moveSoldier(point, dest, dest)){
                        setChangeFlg();
                        return;
                    }
                }
                if(thisx + dest < SOLDIER_HORIZONTAL){
                    if(moveSoldier(point, dest, 0)){
                        setChangeFlg();
                        return;
                    }
                }
                if(thisy + dest < SOLDIER_VERTICAL){
                    if(moveSoldier(point, 0, dest)){
                        setChangeFlg();
                        return;
                    }
                }
                dest--;
            }
        }else{
            //CCLOG("+- %d,%d",thisx,thisy);
            while(dest > 0){
                if(thisx + dest < SOLDIER_HORIZONTAL && thisy - dest >= 0){
                    if(moveSoldier(point, dest, -dest)){
                        setChangeFlg();
                        return;
                    }
                }
                if(thisx + dest < SOLDIER_HORIZONTAL){
                    if(moveSoldier(point, dest, 0)){
                        setChangeFlg();
                        return;
                    }
                }
                if(thisy - dest >= 0){
                    if(moveSoldier(point, 0, -dest)){
                        setChangeFlg();
                        return;
                    }
                }
                dest--;
            }
        }
    }else{
        if(pTouch->getLocation().y > (size.height * thisy) + size.height/2){
            //CCLOG("-+ %d,%d",thisx,thisy);
            while(dest > 0){
                if(thisx - dest >= 0 && thisy + dest < SOLDIER_VERTICAL){
                    if(moveSoldier(point, -dest, dest)){
                        setChangeFlg();
                        return;
                    }
                }
                if(thisx - dest >= 0){
                    if(moveSoldier(point, -dest, 0)){
                        setChangeFlg();
                        return;
                    }
                }
                if(thisy + dest < SOLDIER_VERTICAL){
                    if(moveSoldier(point, 0, dest)){
                        setChangeFlg();
                        return;
                    }
                }
                dest--;
            }
        }else{
            //CCLOG("-- %d,%d",thisx,thisy);
            while(dest > 0){
                if(thisx - dest >= 0 && thisy - dest >=0){
                    if(moveSoldier(point, -dest, -dest)){
                        setChangeFlg();
                        return;
                    }
                }
                if(thisx - dest >= 0){
                    if(moveSoldier(point, -dest, 0)){
                        setChangeFlg();
                        return;
                    }
                }
                if(thisy - dest >=0){
                    if(moveSoldier(point, 0, -dest)){
                        setChangeFlg();
                        return;
                    }
                }
                dest--;
            }
        }
    }
}

void TapSprite::setChangeFlg(){
    if(!status->getChangeFlg()){
        status->setChangeFlg(true);
        redlineSprite = cocos2d::TapSprite::create("redline.png");
        
        CCScaleTo* scaleTo = CCScaleTo::create(3.0f, 0.0f,1.0f);
        CCFiniteTimeAction* func = CCCallFunc::create( this,callfunc_selector(TapSprite::waitOver));
        CCSequence* sequense = CCSequence::createWithTwoActions(scaleTo,func);
        animation++;
        redlineSprite->setPosition(ccp(0,500));
        parent = this->getParent();
        parent->addChild(redlineSprite);
        wait = sequense;
        redlineSprite->runAction(sequense);
    }
}

// 入れ替え判定メソッド
bool TapSprite::moveSoldier(CCPoint point,int xDest,int yDest){
    
    int destx = thisx + xDest;
    int desty = thisy + yDest;
    //tempx = xDest;
    //tempy = yDest;
    
    // マス外制御
    if(destx >= SOLDIER_HORIZONTAL){
        destx = SOLDIER_HORIZONTAL - 1;
        xDest = 0;
    }
    if(destx < 0){
        destx = 0;
        xDest = 0;
    }
    
    if(desty >= SOLDIER_VERTICAL){
        desty = SOLDIER_VERTICAL - 1;
        yDest = 0;
    }
    if(desty < 0){
        desty = 0;
        yDest = 0;
    }
    
    if(xDest == 0 && yDest == 0)return false;
    
    // スプライトの大きさ
    CCSize size = this->getTexture()->getContentSize();
    
    float nowx = point.x;
    float nowy = point.y;
    
    // マス外制御
    if(nowx >= size.width * SOLDIER_HORIZONTAL){
        nowx = size.width * SOLDIER_HORIZONTAL;
    }
    if(nowx < 0){
        nowx = 0;
    }
    
    if(nowy >= size.height * SOLDIER_VERTICAL){
        nowy = size.height * SOLDIER_VERTICAL;
    }
    if(nowy < 0){
        nowy = 0;
    }
    
    float width;
    float height;
    CCPoint destPoint;
    float length;

    // タッチポイントと移動対象マスとの距離を測定する
    width = nowx - ((size.width * (destx)) + (size.width / 2) + 5);
    height = nowy - ((size.height * (desty)) + (size.height / 2));
    
    // 距離は2点間の三平方の定理で取る
    length = sqrtf((width * width) + (height * height));
    //CCLOG("%d",(int)length);
    
    // 2点間の距離が、隣のスプライトの四辺を接線とする円の中に入った場合、入れ替えを行う
    if(length <= (size.width / 2) + 1){
        changeSoldier(xDest,yDest);
        return true;
    }else{
        return false;
    }
}

void TapSprite::changeSoldier(int x, int y){
    
    int i;
    if(x){
        i = abs(x);
    }else{
        i = abs(y);
    }
    
    int absx = 0;
    if(x > 0){
        absx = 1;
    }else if (x < 0){
        absx = -1;
    }
    int absy = 0;
    if(y > 0){
        absy = 1;
    }else if (y < 0){
        absy = -1;
    }
    
    int destx = thisx;
    int desty = thisy;
    
    for(int j = 0;j < i;j++){
        if(x > 0){
            destx++;
        }else if(x < 0){
            destx--;
        }
        if(y > 0){
            desty++;
        }else if(y < 0){
            desty--;
        }
        
        CCSize size = this->getTexture()->getContentSize();
        CCSprite* dest = status->getSprite(destx,desty);
        
        CCNode* parent;
        CCSprite* sprite;
        
        parent = dest->getParent();
        parent->removeChild(dest);
        parent->addChild(dest);
        
        sprite = status->getSprite(thisx,thisy);
        parent = sprite->getParent();
        parent->removeChild(sprite);
        parent->addChild(sprite);
        
        CCPointArray* pAry = CCPointArray::create(3);
        pAry->addControlPoint(ccp(0,0));
        dest->stopAllActions();
        dest->setPosition(ccp((size.width * (destx)) + size.width/2 + 5, (size.height * (desty)) + size.height/2));
        if(x && y){
            pAry->addControlPoint(ccp(size.width * (-absx) * 0.75f, size.height * (-absy) * 0.25f ));
        }
        else if(x){
            pAry->addControlPoint(ccp(size.width * 0.5f * (-absx), -size.height * 0.5f ));
        }
        else if(y){
            pAry->addControlPoint(ccp(size.width * 0.5f,size.height * 0.5f *  (-absy)));
        }
        pAry->addControlPoint(ccp(size.width * (-absx),size.height *  (-absy)));
        dest->runAction(CCCardinalSplineBy::create(0.125f, pAry,1.0f));
        
        ((TapSprite*)dest)->setPlace(thisx,thisy);
        
        int tempStatus = status->getSoldier(destx, desty);
        status->setSoldier(status->getSoldier(thisx, thisy),destx, desty);
        status->setSoldier(tempStatus,thisx,thisy);
        
        status->setSprite(status->getSprite(thisx, thisy),destx, desty);
        status->setSprite(dest,thisx,thisy);
        
        thisx = destx;
        thisy = desty;
        
    }
}

void TapSprite::waitOver(){
    status->setWaitFlg(true);
    finishMove();
}

void TapSprite::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(status->getWaitFlg()) return;
    if(redlineSprite){
        redlineSprite->stopAction(wait);
    }
    finishMove();
}

void TapSprite::finishMove(){
    parent = this->getParent();
    if(redlineSprite){
        parent->removeChild(redlineSprite);
    }
    CCSize size = this->getTexture()->getContentSize();
    CCPoint nowPoint;
    
    nowPoint = ccp((size.width * thisx) + (size.width/2) + 5, (size.height * thisy) + size.height/2);
    this->setPosition(nowPoint);
    
    if(status->getChangeFlg()){
        parent = this->getParent();
        // タッチ終了処理
        
        loopx = 0;
        loopy = 0;
        combo = 1;
        animation = 0;
        labelVec.clear();
        
        //CC_SAFE_RELEASE(spawn);
        //CC_SAFE_RELEASE(sequense);
        
        if(!eraseSoldier()){
            status->setAnimationFlg(false);
        }
    }
}

void TapSprite::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    // タッチキャンセル処理
    CCLOG("TapSprite: touch cancelled");
}

void TapSprite::setPlace(int x,int y){
    thisx = x;
    thisy = y;
}

void TapSprite::setStatus(SoldierStatus* soldierStatus){
    status = soldierStatus;
}

void TapSprite::endFill(){
    fillSoldier = false;
    restartErase();
}

void TapSprite::restartErase(){
    if(!upSoldier && !fillSoldier){
        //loopx = 0;
        //loopy = 0;
        if(!eraseSoldier()){
            status->setAnimationFlg(false);
            CCLOG("End");
            CCLOG("-------------0");
            for(int yy = SOLDIER_VERTICAL - 1;yy >= 0;yy--){
                CCLOG("%d,%d,%d,%d,%d,%d,%d",status->getEraseFlg(0,yy),status->getEraseFlg(1,yy),status->getEraseFlg(2,yy),status->getEraseFlg(3,yy),status->getEraseFlg(4,yy),status->getEraseFlg(5,yy),status->getEraseFlg(6,yy));
            }
            
            CCLOG("-------------1");
            for(int yy = SOLDIER_VERTICAL - 1;yy >= 0;yy--){
                CCLOG("%d,%d,%d,%d,%d,%d,%d",status->getSoldier(0,yy),status->getSoldier(1,yy),status->getSoldier(2,yy),status->getSoldier(3,yy),status->getSoldier(4,yy),status->getSoldier(5,yy),status->getSoldier(6,yy));
            }
        }
    }
}

bool TapSprite::eraseSoldier()
{
    status->setAnimationFlg(true);
    for(;loopy < SOLDIER_VERTICAL; loopy++){
        for(;loopx < SOLDIER_HORIZONTAL; loopx++){
            bool eraseFlg = false;
            if(eraseHorizontalAnim(loopx,loopy)){
                eraseFlg = true;
            };
            if(eraseVerticalAnim(loopx,loopy)){
                eraseFlg = true;
            };

            if(eraseFlg){
                CCLOG("COMBO:%d",combo);
                
                CCLOG("-------------0");
                for(int yy = SOLDIER_VERTICAL - 1;yy >= 0;yy--){
                    CCLOG("%d,%d,%d,%d,%d,%d,%d",status->getEraseFlg(0,yy),status->getEraseFlg(1,yy),status->getEraseFlg(2,yy),status->getEraseFlg(3,yy),status->getEraseFlg(4,yy),status->getEraseFlg(5,yy),status->getEraseFlg(6,yy));
                }
                
                CCLOG("-------------1");
                for(int yy = SOLDIER_VERTICAL - 1;yy >= 0;yy--){
                    CCLOG("%d,%d,%d,%d,%d,%d,%d",status->getSoldier(0,yy),status->getSoldier(1,yy),status->getSoldier(2,yy),status->getSoldier(3,yy),status->getSoldier(4,yy),status->getSoldier(5,yy),status->getSoldier(6,yy));
                }
                combo += 1;
                return true;
            }
        }
        loopx = 0;
    }
    return false;
}

bool TapSprite::eraseVerticalAnim(int x,int y){
    bool ret = false;
    
    // 縦方向に同じものが3つ以上続いているものがある場合、それを消去
    int loopx = x;
    int loopy = y;
    
    int element = status->getSoldier(loopx, loopy);
    int desty1 = loopy - 1;
    int desty2;
    
    // 現在のマス-1が存在しなければ判定を行わない
    if(desty1 >= 0){
        if(element == status->getSoldier(loopx, desty1)){
            desty2 = loopy - 2;
            // 現在のマス-2も存在する場合、statusを消滅用に書き換える
            if(desty2 >= 0){
                if(element == status->getSoldier(loopx, desty2)){
                    
                    // 現在のマスが書きかわっていない場合は書き換えを行う
                    if(status->getEraseFlg(loopx, loopy) == 0){
                        eraseSoldierAnim(loopx, loopy);
                        ret = true;
                    }
                    
                    // 現在のマス-1
                    if(status->getEraseFlg(loopx, desty1) == 0){
                        eraseSoldierAnim(loopx, desty1);
                        eraseHorizontalAnim(loopx, desty1);
                        ret = true;
                    }
                    
                    // 現在のマス-2
                    if(status->getEraseFlg(loopx, desty2) == 0){
                        eraseSoldierAnim(loopx, desty2);
                        eraseHorizontalAnim(loopx, desty2);
                        ret = true;
                    }
                    
                    // さらにその先に消すべき兵士があるかどうかを判定する
                    for(desty2 = desty2 -1;desty2 >= 0;desty2--){
                        if(element == status->getSoldier(loopx, desty2)){
                            if(status->getEraseFlg(loopx, desty2) == 0){
                                eraseSoldierAnim(loopx, desty2);
                                eraseHorizontalAnim(loopx, desty2);
                                ret = true;
                            }
                        }else{
                            break;
                        }
                    }
                    
                    //CCLOG("%d:%d;V1",loopy,desty2);
                    for(int loopyy = loopy;loopyy > desty2;loopyy--){
                        //CCLOG("for in V1");
                        if(loopx > 0){
                            if((element == status->getSoldier(loopx -1, loopyy)) && (status->getEraseFlg(loopx -1, loopyy) == 0)){
                                if(eraseVerticalAnim(loopx -1, loopyy)){
                                    ret = true;
                                }
                            }
                        }
                        if(loopx < SOLDIER_HORIZONTAL - 1){
                            if((element == status->getSoldier(loopx +1, loopyy)) && (status->getEraseFlg(loopx +1, loopyy) == 0)){
                                if(eraseVerticalAnim(loopx +1, loopyy)){
                                    ret = true;
                                }
                            }
                        }
                    }
                    
                }
            }
            
            
            // 現在のマス-1,現在のマス,現在のマス+1の並びが成立している場合を検出する
            desty2 = loopy + 1;
            if(desty2 < SOLDIER_VERTICAL){
                if(element == status->getSoldier(loopx, desty2)){
                    
                    if(status->getEraseFlg(loopx, loopy) == 0){
                        eraseSoldierAnim(loopx, loopy);
                        ret = true;
                    }
                    
                    if(status->getEraseFlg(loopx, desty1) == 0){
                        eraseSoldierAnim(loopx, desty1);
                        eraseHorizontalAnim(loopx, desty1);
                        ret = true;
                    }
                    
                    if(status->getEraseFlg(loopx, desty2) == 0){
                        eraseSoldierAnim(loopx, desty2);
                        eraseHorizontalAnim(loopx, desty2);
                        ret = true;
                    }
                    
                    if(loopx > 0){
                        if((element == status->getSoldier(loopx -1, loopy)) && (status->getEraseFlg(loopx -1, loopy) == 0)){
                            if(eraseVerticalAnim(loopx -1, loopy)){
                                ret = true;
                            }
                        }
                        if((element == status->getSoldier(loopx -1, desty1)) && (status->getEraseFlg(loopx -1, desty1) == 0)){
                            if(eraseVerticalAnim(loopx -1, desty1)){
                                ret = true;
                            }
                        }
                        if((element == status->getSoldier(loopx -1, desty2)) && (status->getEraseFlg(loopx -1, desty2) == 0)){
                            if(eraseVerticalAnim(loopx -1, desty2)){
                                ret = true;
                            }
                        }
                    }
                    if(loopx < SOLDIER_HORIZONTAL - 1){
                        if((element == status->getSoldier(loopx +1, loopy)) && (status->getEraseFlg(loopx +1, loopy) == 0)){
                            if(eraseVerticalAnim(loopx +1, loopy)){
                                ret = true;
                            }
                        }
                        if((element == status->getSoldier(loopx +1, desty1)) && (status->getEraseFlg(loopx +1, desty1) == 0)){
                            if(eraseVerticalAnim(loopx +1, desty1)){
                                ret = true;
                            }
                        }
                        if((element == status->getSoldier(loopx +1, desty2)) && (status->getEraseFlg(loopx +1, desty2) == 0)){
                            if(eraseVerticalAnim(loopx +1, desty2)){
                                ret = true;
                            }
                        }
                    }
                    
                }
            }
            
        }
    }
    
    // 上記の流れをプラス方向に実行する
    desty1 = loopy + 1;
    if(desty1 < SOLDIER_VERTICAL){
        if(element == status->getSoldier(loopx, desty1)){
            desty2 = loopy + 2;
            if(desty2 < SOLDIER_VERTICAL){
                if(element == status->getSoldier(loopx, desty2)){
                    
                    if(status->getEraseFlg(loopx, loopy) == 0){
                        eraseSoldierAnim(loopx, loopy);
                        ret = true;
                    }
                    
                    if(status->getEraseFlg(loopx, desty1) == 0){
                        eraseSoldierAnim(loopx, desty1);
                        eraseHorizontalAnim(loopx, desty1);
                        ret = true;
                    }
                    
                    if(status->getEraseFlg(loopx, desty2) == 0){
                        eraseSoldierAnim(loopx, desty2);
                        eraseHorizontalAnim(loopx, desty2);
                        ret = true;
                    }
                    
                    
                    for(desty2 = desty2 +1;desty2 < SOLDIER_VERTICAL;desty2++){
                        if(element == status->getSoldier(loopx, desty2)){
                            if(status->getEraseFlg(loopx, desty2) == 0){
                                eraseSoldierAnim(loopx, desty2);
                                eraseHorizontalAnim(loopx, desty2);
                                ret = true;
                            }
                        }else{
                            break;
                        }
                    }
                    
                    //CCLOG("%d:%d;V2",loopy,desty2);
                    
                    for(int loopyy = loopy;loopyy < desty2;loopyy++){
                        //CCLOG("for in V2");
                        
                        if(loopx > 0){
                            if((element == status->getSoldier(loopx -1, loopyy)) && (status->getEraseFlg(loopx -1, loopyy) == 0)){
                                if(eraseVerticalAnim(loopx -1, loopyy)){
                                    ret = true;
                                }
                            }
                        }
                        if(loopx < SOLDIER_HORIZONTAL - 1){
                            if((element == status->getSoldier(loopx +1, loopyy)) && (status->getEraseFlg(loopx +1, loopyy) == 0)){
                                if(eraseVerticalAnim(loopx +1, loopyy)){
                                    ret = true;
                                }
                            }
                        }
                    }
                    
                }
            }
        }
    }
    
    return ret;
}

bool TapSprite::eraseHorizontalAnim(int x,int y){
    bool ret = false;
    
    int loopx = x;
    int loopy = y;
    
    int element = status->getSoldier(loopx, loopy);
    int destx1 = loopx - 1;
    int destx2;
    
    if(destx1 >= 0){
        if(element == status->getSoldier(destx1, loopy)){
            destx2 = loopx - 2;
            if(destx2 >= 0){
                if(element == status->getSoldier(destx2, loopy)){
                    
                    if(status->getEraseFlg(loopx, loopy) == 0){
                        eraseSoldierAnim(loopx, loopy);
                        ret = true;
                    }
                    
                    if(status->getEraseFlg(destx1, loopy) == 0){
                        eraseSoldierAnim(destx1, loopy);
                        eraseVerticalAnim(destx1, loopy);
                        ret = true;
                    }
                    
                    if(status->getEraseFlg(destx2, loopy) == 0){
                        eraseSoldierAnim(destx2, loopy);
                        eraseVerticalAnim(destx2, loopy);
                        ret = true;
                    }
                    
                    for(destx2 = destx2 - 1;destx2 >= 0;destx2--){
                        if(element == status->getSoldier(destx2, loopy)){
                            if(status->getEraseFlg(destx2, loopy) == 0){
                                eraseSoldierAnim(destx2, loopy);
                                eraseVerticalAnim(destx2, loopy);
                                ret = true;
                            }
                        }else{
                            break;
                        }
                    }
                    
                    //CCLOG("%d:%d;H1",loopx,destx2);
                    
                    for(int loopxx = loopx;loopxx > destx2;loopxx--){
                        //CCLOG("for in H1");
                        
                        if(loopy > 0){
                            if((element == status->getSoldier(loopxx, loopy - 1)) && (status->getEraseFlg(loopxx , loopy -1) == 0)){
                                if(eraseHorizontalAnim(loopxx, loopy -1)){
                                    ret = true;
                                }
                            }
                        }
                        if(loopy < SOLDIER_VERTICAL - 1){
                            if((element == status->getSoldier(loopxx, loopy + 1)) && (status->getEraseFlg(loopxx , loopy +1) == 0)){
                                if(eraseHorizontalAnim(loopxx, loopy +1)){
                                    ret = true;
                                }
                            }
                        }
                    }
                    
                }
            }
            destx2 = loopx + 1;
            if(destx2 < SOLDIER_HORIZONTAL){
                if(element == status->getSoldier(destx2, loopy)){
                    if(status->getEraseFlg(loopx, loopy) == 0){
                        eraseSoldierAnim(loopx, loopy);
                        ret = true;
                    }
                    
                    if(status->getEraseFlg(destx1, loopy) == 0){
                        eraseSoldierAnim(destx1, loopy);
                        eraseVerticalAnim(destx1, loopy);
                        ret = true;
                    }
                    
                    if(status->getEraseFlg(destx2, loopy) == 0){
                        eraseSoldierAnim(destx2, loopy);
                        eraseVerticalAnim(destx2, loopy);
                        ret = true;
                    }
                    
                    if(loopy > 0){
                        if((element == status->getSoldier(loopx, loopy - 1)) && (status->getEraseFlg(loopx , loopy -1) == 0)){
                            if(eraseHorizontalAnim(loopx, loopy - 1)){
                                ret = true;
                            }
                        }
                        if((element == status->getSoldier(destx1, loopy - 1)) && (status->getEraseFlg(destx1 , loopy -1) == 0)){
                            if(eraseHorizontalAnim(destx1, loopy -1)){
                                ret = true;
                            }
                        }
                        if((element == status->getSoldier(destx2, loopy - 1)) && (status->getEraseFlg(destx2 , loopy -1) == 0)){
                            if(eraseHorizontalAnim(destx2, loopy -1)){
                                ret = true;
                            }
                        }
                    }
                    if(loopy < SOLDIER_VERTICAL - 1){
                        if((element == status->getSoldier(loopx, loopy + 1)) && (status->getEraseFlg(loopx , loopy +1) == 0)){
                            if(eraseHorizontalAnim(loopx, loopy + 1)){
                                ret = true;
                            }
                        }
                        if((element == status->getSoldier(destx1, loopy - 1)) && (status->getEraseFlg(destx1 , loopy -1) == 0)){
                            if(eraseHorizontalAnim(destx1, loopy -1)){
                                ret = true;
                            }
                        }
                        if((element == status->getSoldier(destx2, loopy - 1)) && (status->getEraseFlg(destx2 , loopy -1) == 0)){
                            if(eraseHorizontalAnim(destx2, loopy -1)){
                                ret = true;
                            }
                        }
                    }
                }
            }
        }
    }
    
    destx1 = loopx + 1;
    if(destx1 < SOLDIER_HORIZONTAL){
        if(element == status->getSoldier(destx1, loopy)){
            destx2 = loopx + 2;
            if(destx2 >= 0){
                if(element == status->getSoldier(destx2, loopy)){
                    
                    if(status->getEraseFlg(loopx, loopy) == 0){
                        eraseSoldierAnim(loopx, loopy);
                        ret = true;
                    }
                    
                    if(status->getEraseFlg(destx1, loopy) == 0){
                        eraseSoldierAnim(destx1, loopy);
                        eraseVerticalAnim(destx1, loopy);
                        ret = true;
                    }
                    
                    if(status->getEraseFlg(destx2, loopy) == 0){
                        eraseSoldierAnim(destx2, loopy);
                        eraseVerticalAnim(destx2, loopy);
                        ret = true;
                    }
                    
                    for(destx2 = destx2 + 1;destx2 < SOLDIER_HORIZONTAL;destx2++){
                        if(element == status->getSoldier(destx2, loopy)){
                            if(status->getEraseFlg(destx2, loopy) == 0){
                                eraseSoldierAnim(destx2, loopy);
                                eraseVerticalAnim(destx2, loopy);
                                ret = true;
                            }
                        }else{
                            break;
                        }
                    }
                    
                    //CCLOG("%d:%d;H2",loopx,destx2);
                    for(int loopxx = loopx;loopxx < destx2;loopxx++){
                        //CCLOG("for in H2");
                        
                        if(loopy > 0){
                            if((element == status->getSoldier(loopxx, loopy - 1)) && (status->getEraseFlg(loopxx , loopy -1) == 0)){
                                if(eraseHorizontalAnim(loopxx, loopy -1)){
                                    ret = true;
                                }
                            }
                        }
                        if(loopy < SOLDIER_VERTICAL - 1){
                            if((element == status->getSoldier(loopxx, loopy + 1)) && (status->getEraseFlg(loopxx , loopy +1) == 0)){
                                if(eraseHorizontalAnim(loopxx, loopy +1)){
                                    ret = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return ret;
}

void TapSprite::eraseSoldierMap(int x,int y){
    
    status->setEraseFlg(combo, x, y);
    
    // 削除対象ノードにペアレントが存在する場合は切り離して捨てる
    CCNode* parent;
    CCSprite* sprite;
    sprite = status->getSprite(x,y);
    
    if(!sprite) return;
    parent = sprite->getParent();
    
    if(!parent) return;
    parent->removeChild(sprite);
    
    status->setSprite(NULL, x, y);
}

void TapSprite::eraseSoldierAnim(int x,int y){
    
    status->setEraseFlg(combo, x, y);
    
    CCSprite* sprite;
    sprite = status->getSprite(x,y);
    
    CCDelayTime* delay = CCDelayTime::create(0.1875f);
    CCFiniteTimeAction* func = CCCallFuncO::create( this,callfuncO_selector(TapSprite::eraseSoldierDelay),(CCObject*)sprite) ;
    CCSequence* sequense = CCSequence::createWithTwoActions(delay,func);
    animation++;
    ((CCSprite*)sprite)->runAction(sequense);

    if(animation == 1){
        CCScaleTo* scaleTo = CCScaleTo::create(0.1825f, 1.0f);
        CCString* string = CCString::createWithFormat("%d combo",combo);
        CCLabelTTF* label = CCLabelTTF::create(string->getCString(), "arial", 18.0f);
        label->setScale(0.125f);
        label->setPosition(sprite->getPosition());
        CCNode* node = ((CCSprite*)sprite)->getParent();
        if(node){
            node->addChild(label);
            label->runAction(scaleTo);
            labelVec.push_back(label);
        }
    }
    
    status->setSprite(NULL, x, y);

}

void TapSprite::eraseSoldierDelay(CCObject* sprite){
    CCScaleTo* scaleTo = CCScaleTo::create(0.1825f, 0.0f);
    CCFiniteTimeAction* func = CCCallFuncO::create( this,callfuncO_selector(TapSprite::eraseSoldierInstance),(CCObject*)sprite) ;
    CCSequence* sequense = CCSequence::createWithTwoActions(scaleTo,func);
    ((CCSprite*)sprite)->runAction(sequense);
    //((CCSprite*)sprite)->runAction(func);
}

void TapSprite::eraseSoldierInstance(CCObject* sprite){
    CCNode* mParent;
    if(!sprite) return;
    mParent = ((CCSprite*)sprite)->getParent();
    
    if(!mParent) return;
    mParent->removeChild(((CCSprite*)sprite));
    
    animation--;
    
    if(animation == 0){
        if(!eraseSoldier()){
            loopx = 0;
            loopy = 0;
            fillSoldierAnim();
        }
    }
}

void TapSprite::fillSoldierAnim(){
    upSoldier = false;
    CCSize size = this->getTexture()->getContentSize();
    for(int x = 0; x < SOLDIER_HORIZONTAL; x++){
        // 一番下の列は入れ替え判定をとらない
        for(int y = SOLDIER_VERTICAL - 1;y > 0; y--){
            // 空欄のマスがあった場合、下から詰め込める兵士があるかどうかを判定
            if(status->getEraseFlg(x, y) != 0){
                // 入れ替え対象は現在の列の一つ下から一番下まで
                for(int desty = y - 1;desty >= 0; desty--){
                    // 詰め込める兵士がある場合は入れ替えを実行
                    int tempErase = status->getEraseFlg(x, desty);
                    if(tempErase == 0){
                        // 単純に入れ替え元と入れ替え先のステータスを入れ替える
                        CCSprite* tempSprite = status->getSprite(x, desty);
                        int tempSoldier = status->getSoldier(x, desty);
                        
                        status->setSoldier(SOLDIER_ELEMENT,x,desty);
                        status->setSprite(NULL,x,desty);
                        status->setEraseFlg(status->getEraseFlg(x, y),x,desty);
                        
                        status->setSoldier(tempSoldier,x,y);
                        status->setSprite(tempSprite,x,y);
                        status->setEraseFlg(tempErase,x,y);
                        
                        upSoldier = true;
                        // 入れ替えたスプライトの画面上の移動
                        CCActionInterval* action;
                        action = CCMoveTo::create(0.25f, ccp((size.width * x) + size.width/2 + 5, (size.height * y) + size.height/2));
                        animation++;
                        // 再配置メソッドをfalseでコールすることで、現在兵士の存在しないマスを対象に埋め合わせる
                        CCFiniteTimeAction* func = CCCallFunc::create( this,callfunc_selector(TapSprite::fillSoldierEnd));
                        
                        CCSequence* sequense = CCSequence::createWithTwoActions(action,func);
                        tempSprite->runAction(sequense);
                        //((cocos2d::CCSprite*)tempSprite)->setPosition(ccp((size.width * x) + size.width/2 + 5, (size.height * y) + size.height/2));
                        //tempSprite->runAction(func);

                        ((TapSprite*)tempSprite)->setPlace(x, y);
                        break;
                    }
                }
            }
        }
    }
    fillSoldier = true;
    ((MainScene*)parent)->resetSoldierPlace(false,this);
    
}

void TapSprite::fillSoldierEnd(){
    animation--;
    if(animation == 0) {
        //loopx = 0;
        //loopy = 0;
        
        labelClear();
        
        upSoldier = false;
        restartErase();
    }
}

void TapSprite::labelClear(){
    std::vector<CCLabelTTF*>::iterator it = labelVec.begin();  // イテレータのインスタンス化
    while( it != labelVec.end() )  // 末尾要素まで
    {
        // 削除対象ノードにペアレントが存在する場合は切り離して捨てる
        CCNode* parent;
        CCLabelTTF* label = *it;
        
        if(!label) return;
        parent = label->getParent();
        
        if(!parent) return;
        parent->removeChild(label);
        
        it++;
    }
    labelVec.clear();
}

NS_CC_END