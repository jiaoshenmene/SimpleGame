//
//  LoginScene.hpp
//  SimpleGame-mobile
//
//  Created by 杜甲 on 2019/2/21.
//

#ifndef LoginScene_hpp
#define LoginScene_hpp

#include "cocos2d.h"

class LoginScene : public cocos2d::LayerColor
{
    
    LoginScene(void);
    ~LoginScene(void);
public:
    bool initWithWon(bool won);
    //
    static cocos2d::Scene* gameStart();
    
};

#endif /* LoginScene_hpp */
