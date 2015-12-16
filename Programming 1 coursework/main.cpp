#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


#include <windows.h>
#include "windowOGL.h"
#include "GameConstants.h"
#include "cWNDManager.h"
#include "cInputMgr.h"
#include "cSprite.h"
#include "cBkGround.h"
#include "cFontMgr.h"
#include "mazeMaker.h"
#include "cFileHandler.h"
#include "cButton.h"
#include "main.h"
#include "cSound.h"



int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{

    //Set our window settings
    const int windowWidth = 1024;
    const int windowHeight = 768;
    const int windowBPP = 16;



    //This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();
	
	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();


    //The example OpenGL code
    windowOGL theOGLWnd;
	
    //Attach our the OpenGL window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);

	// Cube

    //Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
    {
        //If it fails

        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
    {
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER | theInputMgr->MOUSE_BUFFER);

	//Declaring the maps.

	int map[8][8] = { { 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 } };

	int winmap[8][8] = { { 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 } };

	// load game fontss
	// Load Fonts
	LPCSTR gameFonts[2] = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };

	theFontMgr->addFont("SevenSeg", gameFonts[0], 24);
	theFontMgr->addFont("Space", gameFonts[1], 24);

	// Create vector array of textures for the pieces
	vector<cTexture*> textureList;
	LPCSTR texturesToUse[] = { "Images/blank.png", "Images/Track/Pieces/HorizontalPiece.png", "Images/Track/Pieces/VerticalPiece.png", "Images/Track/Pieces/TurnRightPiece.png", "Images/Track/Pieces/TurnLeftPiece.png", "Images/Track/Pieces/TurnLeft180Piece.png", "Images/Track/Pieces/TurnRight180Piece.png", "Images/Track/Pieces/GoalPiece.png", "Images/Track/Pieces/NopePiece.png" };
	for (int tCount = 0; tCount < 9; tCount++)
	{
		textureList.push_back(new cTexture());
		textureList[tCount]->createTexture(texturesToUse[tCount]);
	}

	// Create vector array of textures for backgrounds
	vector<cTexture*> textureBkgList;
	LPCSTR bkgTexturesToUse[] = { "Images/GameBkg.png", "Images/MainMenuBkg.png", "Images/EndScreen.png", "Images/WinScreen.png"};
	for (int tCount = 0; tCount < 4; tCount++)
	{
		textureBkgList.push_back(new cTexture());
		textureBkgList[tCount]->createTexture(bkgTexturesToUse[tCount]);
	}

	//create vector array of sounds
	/*vector<cSound*> soundList;
	LPCSTR soundsToUse[] = { "Music Files/MenuMusic", "Music Files/MainGame", "Music Files/EndMusic" };
	for (int tCount = 0; tCount < 9; tCount++)
	{
		soundList.push_back(new cSound());
		soundList[tCount]->loadWAVFile(soundsToUse[tCount]);
	}
*/
	cTexture transSprite;
	transSprite.createTexture("Images/blank.png");

	cBkGround spriteBkgd;
	spriteBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteBkgd.setTexture(textureBkgList[0]->getTexture());
	spriteBkgd.setTextureDimensions(textureBkgList[0]->getTWidth(), textureBkgList[0]->getTHeight());

	cBkGround spriteStartBkgd;
	spriteStartBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteStartBkgd.setTexture(textureBkgList[1]->getTexture());
	spriteStartBkgd.setTextureDimensions(textureBkgList[1]->getTWidth(), textureBkgList[1]->getTHeight());

	cBkGround spriteEndBkgd;
	spriteEndBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteEndBkgd.setTexture(textureBkgList[2]->getTexture());
	spriteEndBkgd.setTextureDimensions(textureBkgList[2]->getTWidth(), textureBkgList[2]->getTHeight());

	cBkGround spriteWinBkgd;
	spriteWinBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteWinBkgd.setTexture(textureBkgList[3]->getTexture());
	spriteWinBkgd.setTextureDimensions(textureBkgList[3]->getTWidth(), textureBkgList[3]->getTHeight());

	cTileMap tileMap(textureList, map);
	tileMap.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	tileMap.setSpritePos(glm::vec2(100.0f, 100.0f));

	cTileMap winMap(textureList, winmap);

	cTilePicker roadMap(textureList, map);
	roadMap.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	roadMap.setSpritePos(glm::vec2(740.0f, 100.0f));

	cTile tileToDrag;
	tileToDrag.attachInputMgr(theInputMgr);
	tileToDrag.setSpritePos(glm::vec2(0.0f, 00.0f));
	tileToDrag.setTexture(transSprite.getTexture());
	tileToDrag.setTextureDimensions(transSprite.getTWidth(), transSprite.getTHeight());

	// Create vector array of button textures
	vector<cTexture*> btnTextureList;
	LPCSTR btnTexturesToUse[] = { "Images/Buttons/exitBtn.png", "Images/Buttons/instructionsBtn.png", "Images/Buttons/loadBtn.png", "Images/Buttons/playBtn.png", "Images/Buttons/saveBtn.png", "Images/Buttons/settingsBtn.png"};
	for (int tCount = 0; tCount < 6; tCount++)
	{
		btnTextureList.push_back(new cTexture());
		btnTextureList[tCount]->createTexture(btnTexturesToUse[tCount]);
	}

	cButton saveButton;
	saveButton.attachInputMgr(theInputMgr);
	saveButton.setTexture(btnTextureList[4]->getTexture());
	saveButton.setTextureDimensions(btnTextureList[4]->getTWidth(), btnTextureList[4]->getTHeight());

	cButton exitButton;
	exitButton.attachInputMgr(theInputMgr);
	exitButton.setTexture(btnTextureList[0]->getTexture());
	exitButton.setTextureDimensions(btnTextureList[0]->getTWidth(), btnTextureList[0]->getTHeight());

	cButton playButton;
	playButton.attachInputMgr(theInputMgr);
	playButton.setTexture(btnTextureList[3]->getTexture());
	playButton.setTextureDimensions(btnTextureList[3]->getTWidth(), btnTextureList[3]->getTHeight());

	cButton loadButton;
	loadButton.attachInputMgr(theInputMgr);
	loadButton.setTexture(btnTextureList[2]->getTexture());
	loadButton.setTextureDimensions(btnTextureList[2]->getTWidth(), btnTextureList[2]->getTHeight());

	// include an exit button

	cFileHandler theFile("Data/blankmap.dat");
	string mapData;

	string outputMsg;
	string strMsg[] = { "Use a variety of Pipes to complete the level!","Drag and Drop the pipes.", "Pipe Game", "Thanks for playing!","See you again soon!", "Well Done", "Thank You for Playing" };

	if (!theFile.openFile(ios::in)) //open file for input output
	{
		MessageBox(NULL, "Could not open specified file.", "An error occurred", MB_ICONERROR | MB_OK);
	}
	else
	{
		mapData = theFile.readDataFromFile();
		theFile.closeFile();
		// Fill map with data from file
		int strPos = 0;
		for (int row = 0; row < 8; row++)
		{
			for (int column = 0; column < 8; column++)
			{
				if (mapData[strPos] != ',')
				{
					map[row][column] = mapData[strPos] - 48;
					strPos+= 2;
				}
			}
		}
	}
	cFileHandler winFile("Data/winmap.dat");
	if (!winFile.openFile(ios::in)) //open file for input output
	{
		MessageBox(NULL, "Could not open specified file.", "An error occurred", MB_ICONERROR | MB_OK);
	}
	else
	{
		mapData = winFile.readDataFromFile();
		winFile.closeFile();
		// Fill map with data from file
		int strPos = 0;
		for (int row = 0; row < 8; row++)
		{
			for (int column = 0; column < 8; column++)
			{
				if (mapData[strPos] != ',')
				{
					winmap[row][column] = mapData[strPos] - 48;
					strPos += 2;
				}
			}
		}
	}

	winMap.initialiseMap(winmap);
	tileMap.initialiseMap(map);

	gameState theGameState = MENU;
	btnTypes theBtnType = EXIT;

	//This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
    {
		pgmWNDMgr->processWNDEvents(); //Process any window events

        //We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		switch (theGameState)
		{
		case MENU:
		{
			spriteStartBkgd.render();

			
			playButton.setSpritePos(glm::vec2(400.0f, 300.0f));
			exitButton.setSpritePos(glm::vec2(400.0f, 375.0f));

			playButton.render();
			exitButton.render();

			theGameState = playButton.update(theGameState, PLAYING);
			exitButton.update();

			outputMsg = strMsg[2];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(10, 15, 0.0f));
			outputMsg = strMsg[0];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 100, 0.0f));
			outputMsg = strMsg[1];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 150, 0.0f));
			if (exitButton.getClicked())
			{
				SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			}
		}
			break;
		case PLAYING:
		{
			
		
			cFileHandler theFile("Data/blankmap.dat");
			string mapData;
						
			spriteBkgd.render();

			roadMap.update();
			roadMap.render();
			tileMap.update();
			tileMap.render();
			tileMap.renderGridLines();
			tileToDrag.update();
			tileToDrag.render();

			// render button and reset clicked to false
			saveButton.setSpritePos(glm::vec2(740.0f, 500.0f));
			saveButton.render();
			saveButton.update("Data/usermap.dat", tileMap.getMap());

			//compares the game map string to a win condition map string
			//checks each frame if the maps are the same
			bool playerWins = true;
			if (tileMap.getMap() != winMap.getMap())
				playerWins = false;

			//declaring score
			int score = 0;
			string playerScore;
			
			//if the maps are the same this triggers the win screen.
			if (playerWins == true){
				score = score++;
				std::remove("Data / usermap.dat");
				tileMap.getMap(), "Data/blankMap.dat";
				theGameState = WIN;
			}
			exitButton.setSpritePos(glm::vec2(740.0f, 575.0f));
			exitButton.render();
			theGameState = exitButton.update(theGameState, END);

			outputMsg = strMsg[2];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(10, 15, 0.0f));
		}
			break;
			// this is the game state the player reaches if they complete the maze.
		case WIN:
		{
			spriteWinBkgd.render();

			loadButton.setClicked(false);
			exitButton.setClicked(false);

			loadButton.setSpritePos(glm::vec2(400.0f, 300.0f));
			exitButton.setSpritePos(glm::vec2(400.0f, 375.0f));
			loadButton.render();
			exitButton.render();

			theGameState = loadButton.update(theGameState, PLAYING);
			exitButton.update();

			outputMsg = strMsg[2];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(10, 15, 0.0f));
			outputMsg = strMsg[5];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 100, 0.0f));
			outputMsg = strMsg[6];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 150, 0.0f));
			if (exitButton.getClicked())
			{
				SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			}
			//delete a file
			std::remove("Data / usermap.dat");
			break;
		}
		//when the player uses the exit button during the main game they reach this game state. equivilent to the game over screen.
		case END:
		{
			spriteEndBkgd.render();

			loadButton.setClicked(false);
			exitButton.setClicked(false);

			loadButton.setSpritePos(glm::vec2(400.0f, 300.0f));
			exitButton.setSpritePos(glm::vec2(400.0f, 375.0f));
			loadButton.render();
			exitButton.render();

			theGameState = loadButton.update(theGameState, PLAYING);
			exitButton.update();

			outputMsg = strMsg[2];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(10, 15, 0.0f));
			outputMsg = strMsg[3];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 100, 0.0f));
			outputMsg = strMsg[4];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 150, 0.0f));
			if (exitButton.getClicked())
			{
				SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			}
		}
			break;
		}

		pgmWNDMgr->swapBuffers();
		//theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER | theInputMgr->MOUSE_BUFFER);
    }

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}
