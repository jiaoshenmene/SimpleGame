//
//  GameOverLayer.cpp
//  SimpleGame-mobile
//
//  Created by 杜甲 on 2019/2/20.
//

#include "GameOverLayer.h"
#include "HelloWorldScene.h"

USING_NS_CC;

GameOverLayer::GameOverLayer(void)
{
}

GameOverLayer::~GameOverLayer(void)
{
}

GameOverLayer* GameOverLayer::createWithWon(bool won)
{
    GameOverLayer *pRet = new GameOverLayer();

    if (pRet && pRet->initWithWon(won)) {
       pRet->autorelease();
       return pRet;
    } else {
       CC_SAFE_DELETE(pRet);
       return NULL;
    }
}

bool GameOverLayer::initWithWon(bool won)
{
       bool bRet = false;
       do {
                   CC_BREAK_IF(! CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)));
            
                   char *message;
                   if (won)
                       {
                               message = "You Won!";
                           }
                   else
                       {
                               message = "You Lose :[";
                           }

                   CCSize winSize = CCDirector::sharedDirector()->getWinSize();
                   CCLabelTTF *label = CCLabelTTF::create(message, "Arial", 32);
                   label->setColor(ccc3(0, 0, 0));
                   label->setPosition(ccp(winSize.width / 2, winSize.height / 2));
                   this->addChild(label);
            
                   this->runAction(CCSequence::create(CCDelayTime::create(3),
                                                                  CCCallFunc::create(this, callfunc_selector(GameOverLayer::gameOverDone)),
                                                                  NULL));

                   bRet = true;
               } while (0);
    
       return bRet;
}


cocos2d::Scene* GameOverLayer::sceneWithWon(bool won)
{
       Scene * scene = NULL;
       do
           {
                   scene = Scene::create();
                   CC_BREAK_IF(! scene);
            
                   GameOverLayer *layer = GameOverLayer::createWithWon(won);
                   CC_BREAK_IF(! layer);
            
                   scene->addChild(layer);
               } while (0);
    
       return scene;
}

void GameOverLayer::gameOverDone()
{
      Director::getInstance()->replaceScene(HelloWorld::createScene());
}
