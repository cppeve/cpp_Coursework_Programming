// TextAdventure.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <conio.h>
#include <iostream>
#include <string>
#include <vector>

void clearScreen();
char getKey();

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

class Scene_Game : public Scene
{
private:
    bool is_looping;

    struct Player {
        int hp   = 100;
        int stam = 50;
    } player;

    void display_hud()
    {
        std::string _out = "";
        _out += "Health : ";
        for (int i = 0; i < floor(this->player.hp/10); i++)
        {
            _out += '\u2588';
        }
        _out += "\n";

        _out += "Stamina: ";
        for (int i = 0; i < floor(this->player.stam / 10); i++)
        {
            _out += '\u2588';
        }
        _out += "\n";
    }

    /* Main Game Loop
    * Display HUD
    * Display Options
    * Take Input
    * Process Input
    */
    void loop()
    {
        while (this->is_looping)
        {
            clearScreen();
            this->display_hud();

            system("pause");
        }
    }
public:
    int load() override
    {
        // Setup game loop
        this->is_looping = true;
        // Start Game Loop
        this->loop();
        // Return Opcode Return
        return 1;
    }
};

std::vector<std::vector<Option>> createScenes();
std::vector<Scene> sceneHistory = std::vector<Scene>();
Scene menu_main, menu_options;
Scene currentScene;
Scene_Game GameScene;
bool app_is_running = true;

void changeScene(Scene, Scene);

int main()
{
    std::vector<std::vector<Option>> scene_options = createScenes();

    menu_main    = Scene(scene_options.at(0));
    menu_options = Scene(scene_options.at(1));
    GameScene    = Scene_Game();

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
            changeScene(currentScene, GameScene);
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
    std::cout << "Thanks For Playing..." << std::endl;
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