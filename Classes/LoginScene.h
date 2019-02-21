//
//  LoginScene.hpp
//  SimpleGame-mobile
//
//  Created by 杜甲 on 2019/2/21.
//

#ifndef LoginScene_hpp
#define LoginScene_hpp

#include "cocos2d.h"

class LoginSceneLayer : public cocos2d::LayerColor
{
    
    LoginSceneLayer(void);
    ~LoginSceneLayer(void);
public:
    bool initWithWon(bool won);
    //
    void gameStart();
    static LoginSceneLayer* createWithWon(bool won);
    static cocos2d::Scene* createScene();
};

#endif /* LoginScene_hpp */
