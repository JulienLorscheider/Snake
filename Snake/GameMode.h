//
//  GameMode.cpp
//  Snake
//
//  Created by Julien on 20/01/2024.
//

#include <stdio.h>

class GameMode {
public:
    virtual ~GameMode() = default;
    virtual void initialiserJeu() = 0;
    virtual void bouclePrincipale() = 0;
};
