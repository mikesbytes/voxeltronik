#include "game.h"
#include "scenes/testscene.h"
#include "config.h"
#include "mathplus.h"
#include "spdlog/spdlog.h"
#include "loggersetup.h"
#include "threadpool.h"

#include <iostream>
#include <vector>
#include <memory>


int main (int argc, char *argv[])
{
	vtk::LoggerSetup lSetup;
	lSetup.setup();
	spdlog::get("general")->info("Starting Voxeltronik Engine...");
	spdlog::get("general");

	vtk::ThreadPool::getInstance().addThreads(2);

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
