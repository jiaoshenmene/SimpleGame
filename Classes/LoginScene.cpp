//
//  LoginScene.cpp
//  SimpleGame-mobile
//
//  Created by 杜甲 on 2019/2/21.
//

#include "LoginScene.h"

LoginScene::GameOverLayer(void)
{
}

LoginScene::~GameOverLayer(void)
{
}

LoginScene* LoginScene::createWithWon(bool won)
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

bool LoginScene::initWithWon(bool won)
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


