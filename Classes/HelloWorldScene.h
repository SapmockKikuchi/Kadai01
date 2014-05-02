#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
//#include "SingletonStatus.h"

using namespace cocos2d::extension;

class HelloWorld : public cocos2d::CCLayer
{
    
public:
    bool tapFlg = false;
    cocos2d::CCPoint touchPoint;
    cocos2d::CCSize soldierSize;
    //SoldierStatus* status;
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
    //ccTouchBegan()関数->タッチ開始
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    //ccTouchMoved()関数->タッチ中
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    //ccTouchEnded()関数->タッチ終了
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    //void startSoldierPlace(CCNode* node);
};

#endif // __HELLOWORLD_SCENE_H__
