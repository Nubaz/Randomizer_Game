#include "Player.hpp"
#include "Sub_Hero.hpp"
#include "Enemy.hpp"
#include "Sub_Boss.hpp"
#include "Game.hpp"
#include <stack>

int start(Hero &hero, std::stack< std::unique_ptr<Enemy> > &enemy_stack, Boss &boss, std::ostream &fout){
    int opt, result, init_stamina, boss_flag = 0;
    init_stamina = hero.getStamina();

    while(1){
        std::cout <<"------------------------" <<std::endl
        <<"Choose an option:" <<std::endl
        <<"1) Check STATS" <<std::endl
        <<"2) FIGHT enemies" <<std::endl
        <<"3) FIGHT boss" <<std::endl
        <<"4) QUIT game" <<std::endl
        <<"> ";

        std::cin >>opt;

        std::cout <<"------------------------" <<std::endl;

        switch(opt) {
            case(1):
                std::cout <<hero;
                break;
            case(2):
                //fight enemies from the stack until either the stack is empty or you die
                result = game<Enemy>(hero, *enemy_stack.top(), fout);
                if(result == 0){
                    std::cout <<std::endl <<"You were killed. Game over!" <<std::endl;
                    return 1;
                }
                else{
                    std::cout <<"------------------------";
                    std::cout <<std::endl <<"You killed " <<enemy_stack.top()->getName() <<std::endl;

                    hero.setStamina(init_stamina);
                    enemy_stack.pop();

                    //if you killed all the enemies, return to main
                    if(enemy_stack.empty() == 1){
                        std::cout <<std::endl <<"You killed all the enemies! You win!" <<std::endl;
                        return 1;
                    }
                }
                break;
            case(3):
                if(boss_flag == 0) {
                    result = game<Boss>(hero, boss, fout);
                    if(result == 0){
                        std::cout <<std::endl <<"You were killed. Game over!" <<std::endl;
                        return 1;
                    }
                    else{
                        boss_flag = 1;

                        std::cout <<"------------------------";
                        std::cout <<std::endl <<"You killed " <<boss.getName() <<std::endl;
                        std::cout <<"As a reward for killing the boss, you are given: " <<std::endl;

                        if(boss.getName().find("Small") != std::string::npos) {
                            hero.give_potion("small_hp");
                            std::cout <<"One small potion!" <<std::endl;
                        }
                        if(boss.getName().find("Average") != std::string::npos) {
                            hero.give_potion("big_hp");
                            std::cout <<"One big potion!" <<std::endl;
                        }
                        if(boss.getName().find("Huge") != std::string::npos) {
                            hero.give_potion("small_hp");
                            hero.give_potion("big_hp");
                            std::cout <<"One small potion and one big potion!" <<std::endl;
                        }

                        hero.setStamina(init_stamina);
                    }
                    break;
                }
                else {
                    std::cout <<"You already defeated the boss!" <<std::endl;
                    break;
                }
            case(4):
                std::cout <<"You quit the game. :(" <<std::endl;

                //return to main after result
                return 1;
        }
    }
}

int main(){
    //time seed for random number generator
    srand(time(NULL));

    //enemies declaration
    std::stack< std::unique_ptr<Enemy> > enemy_stack;
    enemy_stack.push(std::make_unique<Enemy>("Brutus"));
    enemy_stack.push(std::make_unique<Enemy>("Leroy"));

    //hero declaration
    std::shared_ptr<Hero> hero;

    //check hero for errors after creation
    try {
        hero = Hero::create();
    }catch(TypeException &e){
        std::cout <<"Exception caught: " <<e.what() <<std::endl;
        return 0;
    }catch(StatsException &e){
        std::cout <<"Exception caught: " <<e.what() <<std::endl;
        return 0;
    }
    
    //boss declaration
    std::shared_ptr<Boss> boss;

    //check boss for errors after creation
    try{
        boss = Boss::create();
    }catch(TypeException &e){
        std::cout <<"Exception caught: " <<e.what() <<std::endl;
        return 0;
    }catch(std::exception &e){
        std::cout <<"Exception caught: " <<e.what() <<std::endl;
        return 0;
    }

    hero->show_type();

    //open file for logging
    std::ofstream fout;
    fout.open("game_logs.out");

    //main menu
    start(*hero, enemy_stack, *boss, fout);
    
    //close logging
    fout.close();

    return 0;
}