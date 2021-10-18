// TextAdventure.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <conio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void clearScreen();
char getKey();
class Level;
struct Game;

class Option
{
public:
    std::string display;
    std::string opcode;
};

class Scene
{
protected:
    std::vector<Option> options;

    void display_options()
    {
        for (int i = 0; i < this->options.size(); i++)
        {
            std::cout << i+1 << ": " << this->options.at(i).display << std::endl;
        }
    }

public:
    Scene() {}
    Scene(std::vector<Option> pOptions)
    {
        this->options = pOptions;
    }

    virtual int load()
    {
        clearScreen();

        while (true)
        {
            // std::cout << "TESTING" << std::endl;
            this->display_options();
            char _inp = getKey();
            std::cout << _inp << std::endl;
            int _inpi = _inp - '0';
            --_inpi;

            if (_inpi < 0 || _inpi > 10)
            {
                std::cout << "Please Enter A Valid Option..." << std::endl;
                system("pause");
            }
            else
            {
                // Get Opcode
                if (_inpi < this->options.size())
                {
                    std::string opcode = this->options.at(_inpi).opcode;
                    if (opcode == "exit")    return 0;
                    if (opcode == "return")  return 1;
                    if (opcode == "play")    return 2;
                    if (opcode == "options") return 3;
                    break;
                }
                else
                {
                    std::cout << "Please Enter A Valid Option..." << std::endl;
                    system("pause");
                }
            }
            clearScreen();
        }
    }
};

// Create Level Maps
std::string lmaptemplate_roof = "############\n";
std::string lmaptemplate_wall = "#          #\n";

std::string levelmap_l0 =
lmaptemplate_roof
+ lmaptemplate_wall
+ lmaptemplate_wall
+ lmaptemplate_wall
+ lmaptemplate_wall
+ lmaptemplate_wall
+ lmaptemplate_wall
+ lmaptemplate_wall
+ lmaptemplate_wall
+ lmaptemplate_wall
+ lmaptemplate_wall
+ lmaptemplate_roof;

/*
* Main Game Loop
* Display HUD
* Display Options
* Take Input
* Process Input
*/

struct Game {
    bool is_looping = true;
    std::vector<int> player_pos = { 1, 1 };
    char player_icon = '@';
} game;

class Level
{
private:
    struct invalidmapException : public std::exception {
        const char* what() const throw ()
        {
            return "Invalid Map Initialised";
        }
    };
public:
    bool valid = false;
    std::string smap;
    std::vector<std::vector<char>> vmap;
    Level() { return; }
    Level(std::string pMap)
    {
        smap = pMap;

        // Parse Map
        vmap = std::vector<std::vector<char>>();
        std::vector<char> cvec = std::vector<char>();
        for (char c : pMap)
        {
            if (c != '\n') cvec.push_back(c);
            else
            {
                vmap.push_back(cvec);
                cvec = std::vector<char>();
            }
        }

        // Check Sizes
        try
        {
            if (vmap.size() != this->sizey) { throw invalidmapException(); }
            for (std::vector<char> cv1 : vmap) {
                if (cv1.size() != this->sizex) { throw invalidmapException(); }
            }
        }
        catch (invalidmapException ime)
        {
            return;
        }

        this->valid = true;
        return;
    }

    int sizex = 12, sizey = 12;

    char iden_wall = '#';

    void display()
    {
        char prev;
        prev = this->vmap.at(game.player_pos[0]).at(game.player_pos[1]);
        this->vmap.at(game.player_pos[0]).at(game.player_pos[1]) = game.player_icon;
        for (std::vector<char> cv1 : vmap) {
            for (char cv2 : cv1) {
                std::cout << cv2;
            }
            std::cout << std::endl;
        }
        this->vmap.at(game.player_pos[0]).at(game.player_pos[1]) = prev;
    }
};

// // // // // // // // // // // // // // // // //
Level level_current;
void Gameloop()
{
    // Initialise Level 1
    level_current = Level(levelmap_l0);
    game.is_looping = true;
    while (game.is_looping)
    {
        clearScreen();

        // Display Map
        level_current.display();

        // Take Input
        char _inp = getKey();
        std::cout << _inp << std::endl;
        if (_inp == 'c')
        {
            game.is_looping = false;
        }

        // get input and eval



        if (game.player_pos[0] < level_current.sizex - 1) {
            if(level_current.vmap.at(game.player_pos[0]).at(game.player_pos[1]+1) != level_current.iden_wall) game.player_pos[0] += 1;
        }

        system("pause");
    }
}

std::vector<std::vector<Option>> createScenes();
std::vector<Scene> sceneHistory = std::vector<Scene>();
Scene menu_main, menu_options;
Scene currentScene;

bool app_is_running = true;

void changeScene(Scene, Scene);

int main()
{
    std::vector<std::vector<Option>> scene_options = createScenes();

    menu_main    = Scene(scene_options.at(0));
    menu_options = Scene(scene_options.at(1));

    currentScene = menu_main;

    while (app_is_running)
    {
        int ropcode = currentScene.load();
        switch (ropcode)
        {
        case 0:
            app_is_running = false;
            break;
        case 1:
            if (sceneHistory.size() >= 1)
            {
                currentScene = sceneHistory.at(sceneHistory.size() - 1);
                sceneHistory.pop_back();
            }
            else app_is_running = false;
            break;
        // Start Game
        case 2:
            // Load Game
            Gameloop();
            // std::cout << "SWITCHING TO GAMESCENE" << std::endl;
            // system("pause");
            break;
        // Options
        case 3:
            changeScene(currentScene, menu_options);
            break;
        }
    }

    clearScreen();
    std::cout << "Thanks For Playing..." << std::endl << std::endl;
    std::cout << "\t,d88b.d88b," << std::endl;
    std::cout << "\t88888888888" << std::endl;
    std::cout << "\t`Y8888888Y'" << std::endl;
    std::cout << "\t  `Y888Y'  " << std::endl;
    std::cout << "\t    `Y'    " << std::endl << std::endl;
    system("pause");
    return 0;
}

void clearScreen()
{
    std::cout << std::flush;
    system("CLS");
    // std::cout << std::endl;
    return;
}

std::vector<std::vector<Option>> createScenes()
{
    std::vector<std::vector<Option>> R = std::vector<std::vector<Option>>();

    // Main Menu
    std::vector<Option> ops_mainmenu = std::vector<Option>();

    Option ops_mm_1 = Option();
    ops_mm_1.display = "Play";
    ops_mm_1.opcode = "play";
    ops_mainmenu.push_back(ops_mm_1);

    Option ops_mm_2 = Option();
    ops_mm_2.display = "Options";
    ops_mm_2.opcode = "options";
    ops_mainmenu.push_back(ops_mm_2);

    Option ops_mm_3 = Option();
    ops_mm_3.display = "Exit";
    ops_mm_3.opcode = "exit";
    ops_mainmenu.push_back(ops_mm_3);

    R.push_back(ops_mainmenu);

    // Options Menu
    std::vector<Option> ops_options = std::vector<Option>();

    Option ops_op_1 = Option();
    ops_op_1.display = "Return";
    ops_op_1.opcode = "return";
    ops_options.push_back(ops_op_1);

    Option ops_op_3 = Option();
    ops_op_3.display = "Exit";
    ops_op_3.opcode = "exit";
    ops_options.push_back(ops_op_3);

    R.push_back(ops_options);

    return R;
}

void changeScene(Scene pCurrentScene, Scene pNewScene)
{
    sceneHistory.push_back(pCurrentScene);
    currentScene = pNewScene;
}

char getKey()
{
    std::cout << "Enter a key: ";
    return _getch();
}