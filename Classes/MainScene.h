//
//  MainScene.h
//  Kadai01
//
//  Created by 菊池　裕太 on 2014/04/16.
//
//

#ifndef __Kadai01__MainScene__
#define __Kadai01__MainScene__

#include "cocos2d.h"
#include "cocos-ext.h"
//#include "SingletonStatus.h"
#include "HelloWorldScene.h"
#include "TapSprite.h"
#include "SoldierStatus.h"

class MainScene : public cocos2d::CCLayer
, public cocos2d::extension::CCBSelectorResolver /** CocosBuilderからメニューとかコントロールボタンを取得するのに必要 */
{
private:
    SoldierStatus* status;
    cocos2d::CCSize soldierSize;
    
    int animation;
    
    HelloWorld* parent;
    void endAnimation(CCObject* child);
    MainScene();
public:
    // CCBSelector
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    
    // ※add Startボタンのタップイベント用
    void tappedButton(CCObject* pTarget);
    
    void resetSoldierPlace(bool partsReset,cocos2d::TapSprite* child);
    
    SoldierStatus* getStatus();
    
    // createメソッド
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(MainScene, create);
    
};

//ローダークラス
class MainSceneLoader : public cocos2d::extension::CCLayerLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MainSceneLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MainScene);
};

#endif /* defined(__Kadai01__MainScene__) */
