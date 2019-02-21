//
//  LoginScene.cpp
//  SimpleGame-mobile
//
//  Created by 杜甲 on 2019/2/21.
//

#include "LoginScene.h"
#include "HelloWorldScene.h"

USING_NS_CC;

LoginSceneLayer::LoginSceneLayer(void)
{
}

LoginSceneLayer::~LoginSceneLayer(void)
{
}

LoginSceneLayer* LoginSceneLayer::createWithWon(bool won)
{
    LoginSceneLayer *pRet = new LoginSceneLayer();
    
    if (pRet && pRet->initWithWon(won)) {
        pRet->autorelease();
        return pRet;
    } else {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool LoginSceneLayer::initWithWon(bool won)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(! CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)));
        
        char *message;
        if (won)
        {
            message = "Game Start";
        }
        else
        {
            message = "You Lose :[";
        }
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
//        CCLabelTTF *label = CCLabelTTF::create(message, "Arial", 32);
//        label->setColor(ccc3(0, 0, 0));
//        label->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
//        this->addChild(label);
        
//        this->runAction(CCSequence::create(CCDelayTime::create(3),
//                                           CCCallFunc::create(this, callfunc_selector(LoginSceneLayer::gameOverDone)),
//                                           NULL));

        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        // add a "close" icon to exit the progress. it's an autorelease object
        
        auto closeItem = MenuItemFont::create("Game Start", CC_CALLBACK_0(LoginSceneLayer::gameStart, this));
        closeItem->setFontName("Arial");
        closeItem->setFontSizeObj(32);
        closeItem->setColor(Color3B(0, 0, 0));
        
        closeItem->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
        
        
        // create menu, it's an autorelease object
        auto menu = Menu::create(closeItem, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1);
        
        
        bRet = true;
    } while (0);
    
    return bRet;
}


void LoginSceneLayer::gameStart()
{
    Director::getInstance()->replaceScene(HelloWorld::createScene());
}


cocos2d::Scene* LoginSceneLayer::createScene()
{
    Scene * scene = NULL;
    do
    {
        scene = Scene::create();
        CC_BREAK_IF(! scene);
        
        LoginSceneLayer *layer = LoginSceneLayer::createWithWon(true);
        CC_BREAK_IF(! layer);
        
        scene->addChild(layer);
    } while (0);
    
    return scene;
    
    
}
