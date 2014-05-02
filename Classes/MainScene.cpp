//
//  MainScene.cpp
//  Kadai01
//
//  Created by 菊池　裕太 on 2014/04/16.
//
//

#include <time.h>
#include "MainScene.h"

typedef void (cocos2d::CCObject::*SEL_MenuHandler)(cocos2d::CCObject*);
typedef void (cocos2d::CCObject::*SEL_CallFunc)();
typedef void (cocos2d::CCObject::*SEL_CallFuncO)(cocos2d::CCObject*);

MainScene::MainScene(){
    status = new SoldierStatus();
}

// リセットボタンタップコールバック
void MainScene::tappedButton(cocos2d::CCObject *pSender)
{
    resetSoldierPlace(true,0);
    CCLOG("Reset");
    CCLOG("-------------0");
    for(int yy = SOLDIER_VERTICAL - 1;yy >= 0;yy--){
        CCLOG("%d,%d,%d,%d,%d,%d,%d",status->getEraseFlg(0,yy),status->getEraseFlg(1,yy),status->getEraseFlg(2,yy),status->getEraseFlg(3,yy),status->getEraseFlg(4,yy),status->getEraseFlg(5,yy),status->getEraseFlg(6,yy));
    }
    
    CCLOG("-------------1");
    for(int yy = SOLDIER_VERTICAL - 1;yy >= 0;yy--){
        CCLOG("%d,%d,%d,%d,%d,%d,%d",status->getSoldier(0,yy),status->getSoldier(1,yy),status->getSoldier(2,yy),status->getSoldier(3,yy),status->getSoldier(4,yy),status->getSoldier(5,yy),status->getSoldier(6,yy));
    }
}

// ボタンとコールバックの関連づけ
cocos2d::SEL_MenuHandler MainScene::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    /*if(pTarget == this && strcmp(pSelectorName, "tappedButton") == 0) {
        return menu_selector(MainScene::tappedButton);
    }*/
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "tappedButton", MainScene::tappedButton);
    return NULL;
}

cocos2d::extension::SEL_CCControlHandler MainScene::onResolveCCBCCControlSelector(cocos2d::CCObject* pTarget, const char* pSelectorName){
    return NULL;
}

// 兵士初期値決定
void MainScene::resetSoldierPlace(bool allReset,cocos2d::TapSprite* child){
    cocos2d::TapSprite* pSprite = NULL;
    animation = 0;
    
    srand((unsigned)time(NULL));
    
    int xanime[SOLDIER_HORIZONTAL] = {1,1,1,1,1,1,1};
    for(int y = 0;y < SOLDIER_VERTICAL; y++){
        for(int x = 0;x < SOLDIER_HORIZONTAL; x++){
            if(!allReset && status->getEraseFlg(x,y) == 0){
                continue;
            }
            
            // 配置する兵士をランダムで決定
            int element = rand() % SOLDIER_ELEMENT;
            
            if(allReset){
                int ignore[2] = {SOLDIER_ELEMENT,SOLDIER_ELEMENT};
                
                // 左方向に同じものが２つ続いているものがある場合、それを配置候補から除外
                if(x > 1){
                    if(status->getSoldier(x - 1,y) == status->getSoldier(x - 2,y)){
                        ignore[1] = status->getSoldier(x - 1,y);
                    }
                }
                // 下方向も同様に処理
                if(y > 1){
                    if(status->getSoldier(x,y - 1) == status->getSoldier(x,y- 2)){
                        ignore[0] = status->getSoldier(x,y - 1);
                    }
                }
                
                // リセットの場合、配置対象外の兵士に決定されたらやり直し
                while(element == ignore[0] || element == ignore[1]){
                    element = rand() % SOLDIER_ELEMENT;
                }
            }
            
            // 兵士の種類に対応したスプライトを用意する
            switch (element) {
                case 0:
                    pSprite = cocos2d::TapSprite::create("block_blue.png");
                    break;
                case 1:
                    pSprite = cocos2d::TapSprite::create("block_green.png");
                    break;
                    
                case 2:
                    pSprite = cocos2d::TapSprite::create("block_pink.png");
                    break;
                    
                case 3:
                    pSprite = cocos2d::TapSprite::create("block_purple.png");
                    break;
                    
                case 4:
                    pSprite = cocos2d::TapSprite::create("block_red.png");
                    break;
                    
                case 5:
                    pSprite = cocos2d::TapSprite::create("block_white.png");
                    break;
                    
                default:
                    return;
                    break;
            }
            
            // 念のためマスに既に兵士がいる場合は削除
            this->removeChild(status->getSprite(x,y));
            
            // 各種初期設定
            
            // 毎回書き換わる・・・保留
            soldierSize = ((cocos2d::CCSprite*)pSprite)->getContentSize();
            
            if(allReset){
                // 配置位置を計算（兵士の幅 * 横位置 + 画像の半分 + 余白5 , 兵士の高さ * 縦位置 + 画像の半分）
                ((cocos2d::CCSprite*)pSprite)->setPosition(ccp((soldierSize.width* x) + soldierSize.width/2 + 5, (soldierSize.height * y) + soldierSize.height/2));
            }else{
                // 配置位置を計算（兵士の幅 * 横位置 + 画像の半分 + 余白5 , 兵士の高さ * 縦位置 + 画像の半分）
                ((cocos2d::CCSprite*)pSprite)->setPosition(ccp((soldierSize.width* x) + soldierSize.width/2 + 5, (-soldierSize.height * xanime[x]) - (soldierSize.height/2)));
                cocos2d::CCActionInterval* action = cocos2d::CCMoveTo::create(0.25f, ccp((soldierSize.width * x) + (soldierSize.width/2) + 5, (soldierSize.height * y) + soldierSize.height/2));
                animation++;
                cocos2d::CCFiniteTimeAction* func = cocos2d::CCCallFuncO::create( this,callfuncO_selector(MainScene::endAnimation),(CCObject*)child);
                cocos2d::CCSequence* sequense = cocos2d::CCSequence::createWithTwoActions(action,func);
                cocos2d::CCPoint point = pSprite->getPosition();
                pSprite->runAction(sequense);
                //((cocos2d::CCSprite*)pSprite)->setPosition(ccp((soldierSize.width* x) + soldierSize.width/2 + 5, (soldierSize.height * y) + soldierSize.height/2));
                //pSprite->runAction(func);
                xanime[x] = xanime[x] + 1;
            }
            
            //pSprite->setParent(this);
            
            // ステータス配列に兵士データを配置
            status->setSoldier(element,x,y);
            status->setSprite(pSprite,x,y);
            status->setEraseFlg(0, x, y);
            
            // ステータス配列とのリンク用データを設定
            pSprite->setPlace(x,y);
            pSprite->setStatus(status);
            
            // add the sprite as a child to this layer
            this->addChild((cocos2d::CCSprite*)pSprite, 0);
        }
    }
}

void MainScene::endAnimation(CCObject* child){
    animation--;
    if(animation == 0){
        ((cocos2d::TapSprite*)child)->endFill();
    }
}

SoldierStatus* MainScene::getStatus(){
    return status;
}