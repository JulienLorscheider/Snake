//
//  main.cpp
//  Snake
//
//  Created by Julien on 20/01/2024.
//

int main() {
    GameModeHandler handler;

    handler.ajouterMode(std::make_unique<CLIMode>());
    handler.ajouterMode(std::make_unique<SFMLMode>());

    handler.handleRequest();

    return 0;
}
