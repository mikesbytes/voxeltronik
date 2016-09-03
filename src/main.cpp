#include "game.h"
#include "scenes/testscene.h"
#include "config.h"
#include "mathplus.h"

#include <iostream>


int main (int argc, char *argv[])
{
    auto conf = new Config;
    conf->loadConfigFromFile("res/config.conf");
    conf->addArgumentRule("-w", "graphics.res.x");
    conf->addArgumentRule("-h", "graphics.res.y");
    conf->loadConfigFromArguments(argc, argv);    

    vtk::Game game;
    game.setConfig(conf);
    game.init();
    game.setScene(new vtk::TestScene);
    game.start();

    return 0;
}
