#include <stdlib.h>

#include "HelloWorldScene.h"
#include "MainScene.h"
#include "TapSprite.h"

USING_NS_CC;
CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCNode* node = NULL;
    
    // MainScene.ccbiを取り込む
    CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    // MainSceneクラスと紐づける
    ccNodeLoaderLibrary->registerCCNodeLoader("MainScene", MainSceneLoader::loader());
    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();
    node = ccbReader->readNodeGraphFromFile("MainScene.ccbi");
    this->addChild(node);
    
    // 紐付けたMainSceneにアップキャストして、再配置関数をコールし初期配置を行う
    ((MainScene*)node)->resetSoldierPlace(true,0);

    //startSoldierPlace(node);
    
    //シングルタッチを受け付ける
    this->setTouchMode(kCCTouchesOneByOne);
    
    //タッチを有効にする
    this->setTouchEnabled(true);
    
    return true;
}

bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return false;
}

void HelloWorld::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{}

void HelloWorld::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
