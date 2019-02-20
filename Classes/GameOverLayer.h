//
//  GameOverLayer.hpp
//  SimpleGame-mobile
//
//  Created by 杜甲 on 2019/2/20.
//

#ifndef GameOverLayer_hpp
#define GameOverLayer_hpp

#include "cocos2d.h"

class GameOverLayer : public cocos2d::LayerColor
{
    
    GameOverLayer(void);
    ~GameOverLayer(void);
public:
    bool initWithWon(bool won);
//
    static cocos2d::Scene* sceneWithWon(bool won);
    static GameOverLayer* createWithWon(bool won);
    void gameOverDone();

};

#endif /* GameOverLayer_hpp */
