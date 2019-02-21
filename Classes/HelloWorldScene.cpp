/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameOverLayer.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    _monstersDestroyed = 0;
    this->monsters = Vector<Sprite *>();
    this->projectiles = Vector<Sprite *>();
    
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("player2.png");
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }
    LayerColor *layer = LayerColor::create();
    layer->initWithColor(Color4B(255, 255, 255, 255));
    this->addChild(layer);
    
    Size winSize = Director::getInstance()->getWinSize();
    player = Sprite::create("player2.png");
    player->setPosition(Vec2(40, winSize.height / 2));
    this->addChild(player);
    
    this->schedule(schedule_selector(HelloWorld::gameLogic), 1);
    
    auto listener = EventListenerTouchOneByOne::create();
    CC_SAFE_RETAIN(listener);
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMove, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnd, this);
    
    EventDispatcher *eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->schedule(schedule_selector(HelloWorld::update));
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("background-music-aac.wav");
    
    return true;
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *event) {
    beforeVec2 = this->convertTouchToNodeSpace(touch);
    return true;
}

bool HelloWorld::onTouchMove(Touch *touch, Event *event) {
    Vec2 locationVec2 = this->convertTouchToNodeSpace(touch);
    player->setPosition(locationVec2);
    
    return true;
}

bool HelloWorld::onTouchEnd(Touch *touch, Event *event) {
    
    Vec2 location = this->convertTouchToNodeSpace(touch);
    Size winSize = Director::getInstance()->getWinSize();
    Sprite *projectile = Sprite::create("projectile2.png");
    projectile->setPosition(player->getPosition());
    Vec2 offset = location - projectile->getPosition();
    
    if (offset.x <= 0) {
        return true;
    }
    
    this->addChild(projectile);
    int realX = winSize.width + projectile->getContentSize().width / 2;
    float ratio = (float)offset.y / (float)offset.x;
    int realY = realX * ratio + projectile->getPosition().y;
    Vec2 realDest = Vec2(realX, realY);
    int offRealX = realX - projectile->getPosition().x;
    int offRealY = realY - projectile->getPosition().y;
    float length = sqrtf(offRealX * offRealX + offRealY * offRealY);
    float velocity = 480 / 1;
    float realMoveDuration = length / velocity;
    
    
    float angleRadians = atanf((float)offRealY / (float)offRealX);
    float angleDegrees = CC_RADIANS_TO_DEGREES(angleRadians);
    float cocosAngle = -1 * angleDegrees;
    
    float rotateDegreesPerSecond = 180 / 0.5;
    float degreesDiff = player->getRotation() - cocosAngle;

    float rotateDuration = fabs(degreesDiff / rotateDegreesPerSecond);
    
    typedef std::function<void (Node *)> fpi1;
    fpi1 f1 = std::bind(&HelloWorld::finishShoot, this);
    RotateTo *rotateTo = RotateTo::create(rotateDuration, cocosAngle);
    Sequence *sequence1 = Sequence::create(rotateTo, CallFuncN::create(f1),NULL);
    player->runAction(sequence1);

    

    
    
    player->setRotation(cocosAngle);

    
    typedef std::function<void (Node *)> fpi;
    fpi f = std::bind(&HelloWorld::spriteMoveFinished, this,std::placeholders::_1);
    
    MoveTo *moveTo = MoveTo::create(realMoveDuration, realDest);
    
    Sequence *sequence = Sequence::create(moveTo, CallFuncN::create(f),NULL);
    
    projectile->runAction(sequence);

    projectile->setTag(2);
    this->projectiles.pushBack(projectile);
    
    
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("pew-pew-lei.wav");
    return true;
}

void HelloWorld::finishShoot() {
    
}

void HelloWorld::gameLogic(float dt) {
    this->addMonster();
}

void HelloWorld::addMonster() {
    Sprite *monster = Sprite::create("monster.png");
    
    Size winSize = Director::getInstance()->getWinSize();
    int minY = monster->getContentSize().height / 2;
    int maxY = winSize.height - monster->getContentSize().height / 2;
    int rangeY = maxY - minY;
    int actualY = (rand() % rangeY) + minY;
    
    monster->setPosition(Vec2(winSize.width + monster->getContentSize().width / 2, actualY));
    this->addChild(monster);
    
    int minDuration = 2.0;
    int maxDuration = 9.0;
    int rangeDuration = maxDuration - minDuration;
    int actualDuration = (rand() % rangeDuration) + minDuration;
   
    MoveTo *actionMove = MoveTo::create(actualDuration, Vec2(-monster->getContentSize().width / 2, actualY));

    typedef std::function<void (Node *)> fpi;
    fpi f = std::bind(&HelloWorld::spriteMoveFinished, this,std::placeholders::_1);
    
    
    CallFuncN *actionMoveDone = CallFuncN::create(f);
    monster->runAction(Sequence::create(actionMove, actionMoveDone, NULL));
    
    monster->setTag(1);
    this->monsters.pushBack(monster);
    
}

void HelloWorld::update(float delta) {
    Vector<Sprite *> projectilesToDelete = Vector<Sprite *>();
    
    for(Sprite *projectile : projectiles) {
        Vector<Sprite *> monstersToDelete = Vector<Sprite *>();
        
        for (Sprite *monster : monsters) {
            if (projectile->getBoundingBox().intersectsRect(monster->getBoundingBox())) {
                monstersToDelete.pushBack(monster);
            }
        }
        
        for (Sprite *monster : monstersToDelete) {
            monsters.eraseObject(monster);
            this->removeChild(monster,true);
            _monstersDestroyed ++;
            if (_monstersDestroyed > 30) {
                Scene *gameOverScene = GameOverLayer::sceneWithWon(true);
                Director::getInstance()->replaceScene(gameOverScene);
            }
        }
        
        if (!monstersToDelete.empty()) {
            projectilesToDelete.pushBack(projectile);
        }
        
        
        
        
        monstersToDelete.clear();
    }
    
    
    for (Sprite *projectile : projectilesToDelete) {
        projectiles.eraseObject(projectile);
        this->removeChild(projectile,true);
    }
    
    projectilesToDelete.clear();
    
}

void HelloWorld::spriteMoveFinished(Node *sender) {
    Sprite *sprite = (Sprite *)sender;
    
    if (sprite->getTag() == 1) {
        monsters.eraseObject(sprite);
        _monstersEscape ++;
        if (_monstersEscape > 5) {
            Scene *gameOverScene = GameOverLayer::sceneWithWon(false);
            Director::getInstance()->replaceScene(gameOverScene);
        }
        
    } else if (sprite->getTag() == 2) {
        projectiles.eraseObject(sprite);
    }
    
    this->removeChild(sprite);
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}



