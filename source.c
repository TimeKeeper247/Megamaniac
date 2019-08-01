#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "cab202_graphics.h"
#include "cab202_timers.h"
#include "cab202_sprites.h"

void mainloop(void);
void thanks(void);

int main(void){
    setup_screen();
    mainloop();
    thanks();
    timer_pause(5000);
    cleanup_screen();
    return 0;
}

typedef char *STRING;

STRING alienbit[] = {
    "   "
    " @ "
    "   ",
    "   "
    "   "
    "   "
};
STRING aliendeathbit[] = {
    "   "
    " * "
    "   ",
    " , "
    "-o-"
    " ' ",
    "\\\t/"
    "(\t)"
    "/\t\\",
    "\\|/"
    "-\t-"
    "/|\\",
    "`. "
    "   "
    ", `",
    "\t` "
    "   "
    "\t `",
    "   "
    "   "
    "   "
};
STRING playerbit[] = {"^"};
STRING bulletbit[] = {"|"};
STRING veerbulletbit[] = {"\\", "/"};
STRING level_name[] = {
    "Basic",
    "Harmonic Motion",
    "Falling Motion",
    "Drunken Motion",
    "Agressive Motion"
};
STRING deathbit[] = {
    "           YOU HAVE DIED.              "
    "THE ALIENS HAVE TAKEN OVER THE EARTH..."
    "         GREAT JOB CAPTAIN.            "
};

void loop(sprite_id sprite);
void move(char key, sprite_id sprite);
bool shoot(sprite_id sprite, sprite_id player_sprite);
void draw_formatted(int x, int y, const char *, ... );

void mainloop(void){
    timer_id alien_timer = create_timer(1000);
    timer_id convergealien_timer = create_timer(800);
    timer_id falling_timer = create_timer(6000);
    timer_id bullet_timer = create_timer(80);
    timer_id alien_shoot = create_timer(3000);
    timer_id alienbullet_timer = create_timer(250);
    timer_id anim_timer = create_timer(100);
    timer_id agress_timer = create_timer(((double)2001/RAND_MAX)*rand()+1000);
    timer_id agressmove_timer = create_timer(250);
    timer_id converge_timer = create_timer(((double)5001/RAND_MAX)*rand()+10000);

    sprite_id alien[10];
    int sprite_x[] = {12,21,30,8,17,26,34,12,21,30};
    int sprite_y[] = {2,2,2,5,5,5,5,8,8,8};
    for (int i=0;i<10;i=i+1){
        alien[i] = create_sprite(sprite_x[i],sprite_y[i],3,3,alienbit[0]);
        alien[i]->dx = 1;
        draw_sprite(alien[i]);
    }

    sprite_id alien5;
    alien5 = create_sprite(-2, -2, 3,3, alienbit[0]);

    sprite_id alien_bullet[4];
    for (int i=0;i<4;i=i+1){
        alien_bullet[i] = create_sprite(0,0,1,1,bulletbit[0]);
    }

    sprite_id player;
    player = create_sprite(screen_width()/2, screen_height()-4,1,1,playerbit[0]);
    draw_sprite(player);

    sprite_id bullet;
    bullet = create_sprite(0,0,1,1,bulletbit[0]);

    sprite_id death_label;
    death_label = create_sprite(0,0,39,3,deathbit[0]);

    show_screen();

    bool shoot_check = false;
    bool alien_shoot_check = false;
    int alien_list_check = 0;
    char key = get_char();
    int score = 0;
    int lives = 3;
    int level = 1;
    int name_cntr = 0;
    int alien_list[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int aliendrunk_list[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int aliendrunk_list2[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int alienbullet_list[] = {0, 0, 0, 0};
    int alien_rand = 0;
    int alien_cntr = 7;
    int alienconverge_cntr = 0;
    int alien_ref = 0.0;
    int alienmove_cntr = 0;
    double alien5move_cntr = 0;
    double tmpsine = 0.0;
    int drunkrand = 0;
    bool alienboolpara = false;
    bool randaliencheck = false;
    bool paralocationcheck = false;
    bool homecheck = false;
    bool drunk_converge = false;
    double tmppara = 0.0;
    int randalienpara = 0;
    while(key != 'q'){
        key = get_char();
        draw_line(0, screen_height()-3, screen_width(), screen_height()-3, '-');
        draw_formatted(1, screen_height()-2, "Duval Longa (n9452257)");
        draw_formatted(screen_width()-(18 + sizeof(score)), screen_height()-2, "Lives: %d | Score: %d", lives, score);
        if ((level == 3 && timer_expired(falling_timer)) || (level == 4 && timer_expired(falling_timer))){
            for (int i=0;i<10;i=i+1){
                alien[i]-> dy = 1;
                alien[i]-> y += alien[i]-> dy;
                if (alien[i]-> y == screen_height()-4){
                    alien[i]-> y = -1;
                }
            }
        }
        if (level != 4 && timer_expired(alien_timer)){
            clear_screen();
            alienmove_cntr += 1;
            for (int i=0;i<10;i=i+1){
                if (level == 2){
                    tmpsine = (((M_PI*2.0)*0.9)*alienmove_cntr)+1.0;
                    alien[i]-> dy = round(sin(tmpsine))*1;
                    alien[i]-> y += alien[i]-> dy;
                }
                alien[i]-> x += alien[i]-> dx;
                if (alien[i]-> x == screen_width()-1){
                    alien[i]-> x = -1;
                }
                draw_sprite(alien[i]);
            }
        }
        if (level == 4 && timer_expired(alien_timer) && !drunk_converge){
            clear_screen();
            for (int i=0;i<10;i=i+1){
                drunkrand = rand() % 10;
                if (drunkrand > 4){
                    alien[i]-> dx = 1;
                } else {
                    alien[i]-> dx = -1;
                }
                for (int j=0;j<10;j=j+1){
                    if (alien[i]-> y == alien[j]-> y && i != j){
                        if (alien[i]-> x - alien[j]-> x < 15 && alien[i]-> x - alien[j]-> x > -15){
                            if (alien[i]-> x - alien[j]-> x > 1){
                                alien[i]-> dx = 1;
                                aliendrunk_list[i] = 11;
                            } else if (alien[i]-> x - alien[j]-> x < -1){
                                alien[i]-> dx = -1;
                                aliendrunk_list2[i] = 11;
                            } else {
                                alien[i]-> x += 2;
                            }
                            if (aliendrunk_list[i] == aliendrunk_list2[i]){
                                alien[i]-> dx = 0;
                            }
                        }
                    }
                }
                alien[i]-> x += alien[i]-> dx;
                if (alien[i]-> x == screen_width()-1){
                    alien[i]-> x = -1;
                } else if (alien[i]-> x < -1){
                    alien[i]-> x = screen_width()-2;
                }
                draw_sprite(alien[i]);
            }
            for (int i=0;i<10;i=i+1){
                aliendrunk_list[i] = i;
                aliendrunk_list2[i] = i;
            }
        }
        if (level == 4 && timer_expired(converge_timer)){
            drunk_converge = true;
        }
        if (level == 4 && drunk_converge && timer_expired(convergealien_timer)){
            clear_screen();
            for (int i=0;i<10;i=i+1){
                alien[i]-> dx = 0;
            }
            for (int i=0;i<10;i=i+1){
                if(alien[i]-> x < sprite_x[i]){
                    alien[i]-> x += 1;
                } else if (alien[i]-> x > sprite_x[i] && alien[i]-> x < screen_width()-screen_width()/3){
                    alien[i]-> x -= 1;
                } else if (alien[i]-> x > sprite_x[i] && alien[i]-> x > screen_width()-screen_width()/3){
                    alien[i]-> x += 1;
                } else {
                    alienconverge_cntr += 1;
                }
                if (alien[i]-> x > screen_width()-1){
                    alien[i]-> x = -1;
                } else if (alien[i]-> x < -1){
                    alien[i]-> x = screen_width()-2;
                }
                draw_sprite(alien[i]);
            }
            if (alienconverge_cntr == 10){
                drunk_converge = false;
                reset_timer(converge_timer);
                alienconverge_cntr = 0;
            } else {
                alienconverge_cntr = 0;
            }
        }
        if (level == 5){
            if (timer_expired(agress_timer) && !alienboolpara){
                alienboolpara = true;
            }
            while (alien_list_check != 9 && alienboolpara && !randaliencheck) {
                randalienpara = rand()%10;
                if (randalienpara == alien_list[randalienpara]){
                    alien5-> x = alien[randalienpara]-> x;
                    alien5-> y = alien[randalienpara]-> y;
                    alien[randalienpara]-> bitmap = alienbit[1];
                    randaliencheck = true;
                }
            }
            if (alienboolpara && randaliencheck && timer_expired(agressmove_timer)){
                clear_screen();
                if (alien5-> y != 10 && !paralocationcheck){
                    alien5-> y += 1;
                    draw_sprite(alien5);
                }
                if (alien5-> y == 10 && alien5-> x < alien[9]-> x + 2 && !paralocationcheck){
                    alien5-> x += 1;
                    draw_sprite(alien5);
                }
                if (alien5-> y == 10 && alien5-> x >= alien[9]-> x + 2 && !paralocationcheck){
                    paralocationcheck = true;
                    alien5move_cntr = screen_height()-11;
                }
                if (paralocationcheck){
                    alien5move_cntr -= 1.0;
                    tmppara = (alien5move_cntr-1.0)/((rand()%2)+1);
                    tmppara = (sqrt(tmppara))-1.5;
                    alien5-> y += 1;
                    alien5-> dx = round(tmppara);
                    alien5-> x += alien5-> dx;
                    if (alien5-> y == 15){
                        homecheck = true;
                        paralocationcheck = false;
                    }
                    draw_sprite(alien5);
                }
                if (homecheck){
                    if (round(alien5-> x+1) < player-> x){
                        alien5-> x += 1;
                    } else if (round(alien5-> x)+1 > player-> x){
                        alien5-> x -= 1;
                    }
                    if (round(alien5-> y)+1 < player-> y){
                        alien5-> y += 1;
                    }
                    if (round(alien5-> y)+1 == screen_height()-3){
                        alien[randalienpara]-> bitmap = alienbit[0];
                        alien5-> x = -2;
                        alien5-> y = -2;
                        alienboolpara = false;
                        randaliencheck = false;
                        paralocationcheck = false;
                        homecheck = false;
                        alienmove_cntr = 0;
                        reset_timer(agress_timer);
                    }
                }
                for (int i=0;i<10;i=i+1){
                    draw_sprite(alien[i]);
                }
                if (round(alien5-> x)+1 == player-> x && round(alien5-> y)+1 == player-> y){
                    if (randalienpara == alien_list[randalienpara]){
                        lives -= 1;
                        player-> x = screen_width()/2;
                        if (lives != 0){
                            alien_cntr = 0;
                        } else if (lives == 0) {
                            alien_cntr = 7;
                        }
                        alien_ref = randalienpara;
                        alien_list[randalienpara] = 11;
                        score += 30;
                    }
                }
                draw_sprite(player);
            }
            if (key == 'z'){
                bullet-> dx -= 0.3;
                draw_sprite(bullet);
            } else if (key == 'c'){
                bullet-> dx += 0.3;
                draw_sprite(bullet);
            }
            draw_sprite(bullet);
            if(bullet-> y < 0 || bullet-> x < 0){
                bullet-> dy = 0;
                bullet-> dx = 0;
                bullet-> bitmap = bulletbit[0];
                shoot_check = false;
            }
        }
        if (key == 's'){
            if (!shoot_check ){
                bullet-> x = player-> x;
                bullet-> y = player-> y;
                bullet-> dy = -1;
                draw_sprite(bullet);
                shoot_check = true;
            }
        } else if (key == 'a' ){
            player-> x -= 1;
            draw_char(player-> x + 1, player-> y, ' ');
            draw_char(player-> x - 1, player-> y, ' ');
        } else if (key == 'd'){
            player-> x += 1;
            draw_char(player-> x + 1, player-> y, ' ');
            draw_char(player-> x - 1, player-> y, ' ');
        }
        else if (key == 'l'){
            level = (level + 1) % 6;
            if (level == 0){
                level = 1;
            }
            name_cntr = (name_cntr + 1) % 5;
            for (int i=0;i<10;i=i+1){
                alien_list[i] = i;
                alien[i]-> bitmap = alienbit[0];
                alien[i]-> x = sprite_x[i];
                alien[i]-> y = sprite_y[i];
                alien[i]-> dx = 1;
            }
        }
        draw_sprite(player);
        if (player-> x == screen_width()){
            player-> x = screen_width()-1;
        } else if (player-> x < 0){
            player-> x = 0;
        } else if (player-> y != screen_height()-4){
            player-> y  = screen_height()-4;
        }
        draw_formatted(screen_width()/2 - ((8 + sizeof(level) + strlen(level_name[name_cntr])))/2, screen_height()-1,"level %d: %s", level, level_name[name_cntr]);
        if (shoot_check && timer_expired(bullet_timer)){
            bullet-> y += bullet-> dy;
            bullet-> x += bullet-> dx;
            draw_char(bullet-> x, bullet-> y + 1, ' ');
            if ((bullet-> dx < 0 && bullet-> dx > -1) || (bullet-> dx > 0 && bullet-> dx < 1)){
                draw_char(bullet-> x, bullet-> y + 1, ' ');
                draw_char(bullet-> x + 1, bullet-> y + 1, ' ');
                draw_char(bullet-> x - 1, bullet-> y + 1, ' ');
            }
            if (bullet-> dx <= -1){
                draw_char(bullet-> x, bullet-> y + 1, ' ');
                draw_char(bullet-> x + 1, bullet-> y, ' ');
                draw_char(bullet-> x + 1, bullet-> y + 1, ' ');
            } else if (bullet-> dx >= 1){
                draw_char(bullet-> x, bullet-> y + 1, ' ');
                draw_char(bullet-> x - 1, bullet-> y, ' ');
                draw_char(bullet-> x - 1, bullet-> y - 1, ' ');
            }
            if (bullet-> dx < 0){
                bullet-> bitmap = veerbulletbit[0];
            } else if (bullet-> dx > 0){
                bullet-> bitmap = veerbulletbit[1];
            } else {
                bullet-> bitmap = bulletbit[0];
            }
            draw_sprite(bullet);
            for (int i=0;i<10;i=i+1){
                if (round(bullet-> x) == alien[i]-> x+1 && round(bullet-> y) == alien[i]-> y+1){
                    alien[i]-> bitmap = alienbit[1];
                    if (i == alien_list[i]){
                        alien_cntr = 0;
                        alien_ref = i;
                        alien_list[i] = 11;
                        score += 30;
                    }
                }
            }
            alien_list_check = 0;
            for (int i=0;i<9;i=i+1){
                if (alien_list[i] == alien_list[i+1]){
                    alien_list_check += 1;
                }
            }
            if (alien_list_check == 9){
                shoot_check = false;
                alien_shoot_check = false;
                alien_list_check = 0;
                key = get_char();
                score += 500;
                level = (level + 1) % 6;
                if (level == 0){
                level = 1;
                }
                name_cntr = (name_cntr + 1) % 5;
                for (int i=0;i<10;i=i+1){
                    alien_list[i] = i;
                    alien[i]-> bitmap = alienbit[0];
                    alien[i]-> x = sprite_x[i];
                    alien[i]-> y = sprite_y[i];
                    alien[i]-> dx = 1;
                }
                for (int i=0;i<4;i=i+1){
                    alienbullet_list[i] = 0;
                }
                player-> x = screen_width()/2;
                alien_rand = 0;
                alien_cntr = 7;
                clear_screen();
            }
            if(bullet-> y < 0 || bullet-> x < 0){
                bullet-> dy = 0;
                bullet-> dx = 0;
                shoot_check = false;
            }
        }
        if (level == 5){
            if (bullet-> x == round(alien5-> x)+1 && bullet-> y == round(alien5-> y)+1){
                alien[randalienpara]-> bitmap = alienbit[1];
                alien5-> x = -2;
                alien5-> y = -2;
                alienboolpara = false;
                randaliencheck = false;
                paralocationcheck = false;
                homecheck = false;
                alienmove_cntr = 0;
                reset_timer(agress_timer);
                if (randalienpara == alien_list[randalienpara]){
                    alien_cntr = 0;
                    alien_ref = randalienpara;
                    alien_list[randalienpara] = 11;
                    score += 30;
                }
            }
        }
        if (timer_expired(alien_shoot) && alien_list_check != 9){
            for (int i=0;i<4;i=i+1){
                if (alienbullet_list[i] == 0){
                    do {
                        alien_rand = rand()%10;
                    } while (alien_rand != alien_list[alien_rand]);
                    alien_bullet[i]-> x = alien[alien_rand]-> x+2;
                    alien_bullet[i]-> y = alien[alien_rand]-> y+2;
                    alien_bullet[i]-> dy = 1;
                    alien_shoot_check = true;
                    alienbullet_list[i] = 1;
                    i = 4;
                }
            }
        }
        if (alien_shoot_check && timer_expired(alienbullet_timer)){
            for (int i=0;i<4;i=i+1){
                if (alienbullet_list[i] == 1){
                    alien_bullet[i]-> y += alien_bullet[i]-> dy;
                    draw_char(alien_bullet[i]-> x, alien_bullet[i]-> y - 1, ' ');
                    draw_sprite(alien_bullet[i]);
                }
            }
            for (int i=0;i<4;i=i+1){
                if (alien_bullet[i]-> x == player-> x && alien_bullet[i]-> y == player-> y){
                    lives -= 1;
                    player-> x = screen_width()/2;
                }
                if (alien_bullet[i]-> y >= screen_height()-3){
                    alienbullet_list[i] = 0;
                    alien_bullet[i]-> x = -1;
                    alien_bullet[i]-> y = -1;
                    alien_bullet[i]-> dy = 0;
                }
            }
        }
        if (level == 3 || level == 4 || level == 5){
            for (int i=0;i<10;i=i+1){
                if (round(alien[i]-> x)+1 == player-> x && round(alien[i]-> y)+1 == player-> y){
                    alien[i]-> bitmap = alienbit[1];
                    if (i == alien_list[i]){
                        lives -= 1;
                        player-> x = screen_width()/2;
                        if (lives != 0){
                            alien_cntr = 0;
                        } else if (lives == 0) {
                            alien_cntr = 7;
                        }
                        alien_ref = i;
                        alien_list[i] = 11;
                        score += 30;
                    }
                }
            }
        }
        if (alien_cntr != 7 && timer_expired(anim_timer)){
            alien[alien_ref]->bitmap=aliendeathbit[alien_cntr];
            draw_sprite(alien[alien_ref]);
            alien_cntr += 1;
        }
        if (alien_cntr == 7){
            for (int i=0;i<10;i=i+1){
                if (alien_list[i] != i){
                    alien[i]-> bitmap = alienbit[1];
                }
            }
        }
        if (lives == 0){
            death_label-> x = (screen_width()/2 - 18);
            death_label-> y = 7;
            draw_sprite(death_label);
            draw_formatted(screen_width()/2 - 9, 10, "Your score was: %d", score);
            draw_formatted(screen_width()/2 - 19, 11, "Give up? 'q' or want to try again? 'r'");
            while (key != 'q' && key != 'r'){
                key = wait_char();
            }
        }
        if (key == 'r'){
            shoot_check = false;
            alien_shoot_check = false;
            alien_list_check = 0;
            key = get_char();
            score = 0;
            lives = 3;
            level = 1;
            name_cntr = 0;
            for (int i=0;i<10;i=i+1){
                alien_list[i] = i;
                alien[i]-> bitmap = alienbit[0];
                alien[i]-> x = sprite_x[i];
                alien[i]-> y = sprite_y[i];
                alien[i]-> dx = 1;
                alien[i]-> dy = 0;
            }
            for (int i=0;i<4;i=i+1){
                alienbullet_list[i] = 0;
            }
            alien_rand = 0;
            player-> x = screen_width()/2;
            clear_screen();
        }
        show_screen();
        timer_pause(20);
    }// end while
}

void thanks(void){
    draw_formatted(screen_width()/2-9, 12, "Thanks for playing!");
    show_screen();
}
