#include "game.h"
#include "scenes/testscene.h"
#include "config.h"
#include "mathplus.h"
#include "spdlog/spdlog.h"
#include "loggersetup.h"

#include <iostream>
#include <vector>
#include <memory>


int main (int argc, char *argv[])
{
	//testing	
	unsigned uTopModel [] = { 0,0,0,0,0,0 };
	uTopModel[0] = (uTopModel[0] << 4) | unsigned(8);
	uTopModel[0] = (uTopModel[0] << 4) | unsigned(6);
	std::cout << "Extracted: " << unsigned((uTopModel[0] >> 4)&((1<<4)-1)) <<
		", " << unsigned((uTopModel[0] >> 0)&((1<<4)-1));

	vtk::LoggerSetup lSetup;
	lSetup.setup();
	spdlog::get("general")->info("Starting Voxeltronik Engine...");
	spdlog::get("general");

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
