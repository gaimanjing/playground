//
//  e1.hpp
//  playground
//
//  Created by houguanhua on 2022/11/8.
//

#ifndef e1_hpp
#define e1_hpp

class GamePlayer {
public:
    void hello();
private:
    static const int NUM_TURNS = 5;
    
    int scores[NUM_TURNS];
};

#endif /* e1_hpp */
