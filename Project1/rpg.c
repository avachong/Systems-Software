//Ava Chong
//449 project 1
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//define functions 
int four();
int six(); 
int eight();
int twelve();
int attack();
void roll(int, int, int, int);
//define constants
const char * armor[] = {"cloth", "studded leather", "ring mail", "chain mail", "plate"};
const int armorClass[] = {10, 12, 14, 16, 18};
const char * weapon[]  = {"dagger", "short sword", "long sword", "great sword", "great axe"};
char c = 'y';
//player health
int p1h = 20;
int p2h = 20;
//num rounds
int rounds = 0;

int main () {
    srand((unsigned int)time(NULL));

    //armor choices
    int p1a = 0;
    int p2a = 0;

    //weapon choices
    int p1w = 0;
    int p2w = 0;

    //menu
    printf("List of available armors: \n0: cloth (AC=10)\n1: studded leather (AC=12)\n2: ring mail (AC=14)\n3: chain mail (AC=16)\n4: plate (AC=18)\n\nChoose Player 1 Armor (0~4): ");
    scanf("%d", &p1a);
    printf("Choose Player 2 Armor (0~4): ");
    scanf("%d", &p2a);

    printf("\nList of available weapons:\n0: dagger (damage=1d4)\n1: short sword (damage=1d6)\n2: long sword (damage=1d8)\n3: great sword (damage=2d6)\n4: great axe (damage=1d12)\n\nChoose Player 1 Weapon(0~4): ");
    scanf("%d", &p1w);
    printf("Choose Player 2 Weapon(0~4): ");
    scanf("%d", &p2w); 
    
    printf("\nPlayer setting complete: \n");
    printf("Player 1: hp = %d, armor = %s, weapon = %s\n", p1h, armor[p1a], weapon[p1w]);
    fflush(stdout);
    printf("Player 2: hp = %d, armor = %s, weapon = %s\n", p2h, armor[p2a], weapon[p2w]);
    fflush(stdout);

    while (c == 'y' || c == 'Y'){
        printf("Fight? y/n: ");
        scanf(" %c", &c);
        p1h = 20;               //reset health + rounds
        p2h = 20;
        rounds = 0;

        while (p1h > 0 && p2h > 0){
        roll(p1a, p2a, p1w, p2w);
        }
        if (p1h <= 0){
            printf("Player 2 had a fatal hit and wins!\n\n");
        } else if (p2h <=0){
            printf("Player 1 had a fatal hit and wins!\n\n");
        }
    }
}

void roll(int p1a, int p2a, int p1w, int p2w){
    printf("\n----------------- Round %d -----------------\n", rounds);
    int damage;
    int attackVal = attack();
    if (attackVal > armorClass[p2a] && p1h > 0 && p2h > 0){       //player 1 hits player 2 - get damage
        if(p1w == 0){                       //dagger
            damage = four();
        } else if (p1w == 1){               //short sword
            damage = six();
        } else if (p1w == 2){               //long sword
            damage = eight();
        } else if (p1w == 3){               //great sword
            damage = six();
            damage = damage + six();
        } else if (p1w == 4){               //great axe
            damage = twelve();
        }
        p2h = p2h - damage;
        printf("Player 1 hits Player 2 for %d damage (attack roll = %d)\n", damage, attackVal);
    } else {
        printf("Player 1 misses Player 2 (attack roll = %d)\n", attackVal);
    }
    attackVal = attack();
    if (attackVal > armorClass[p1a] && p1h > 0 && p2h > 0){       //player 2 hits player 1
        if(p2w == 0){                       //dagger
            damage = four();
        } else if (p2w == 1){               //short sword
            damage = six();
        } else if (p2w == 2){               //long sword
            damage = eight();
        } else if (p2w == 3){               //great sword
            damage = six();
            damage = damage + six();
        } else if (p2w == 4){               //great axe
            damage = twelve();
        }
        p1h = p1h - damage;
        printf("Player 2 hits Player 1 for %d damage (attack roll = %d)\n", damage, attackVal);
    } else {
        printf("Player 2 misses Player 1 (attack roll = %d)\n", attackVal);
    }
    printf("Player 1: hp = %d, armor = %s, weapon = %s \n", p1h, armor[p1a], weapon[p1w]);
    printf("Player 2: hp = %d, armor = %s, weapon = %s \n", p2h, armor[p2a], weapon[p2w]);
    rounds ++;
}
//random number generator per dice
int four(){
    int value = rand() % (4 - 1 + 1) + 1;
    return value;
}
int six(){
    int value = rand() % (6- 1 + 1) + 1;
    return value;
}
int eight(){
    int value = rand() % (8 - 1 + 1) + 1;
    return value;
}
int twelve(){
    int value = rand() % (12 - 1+ 1) + 1;
    return value;
}
int attack(){
    int value = rand() % (20 - 1+ 1) + 1;
    return value;
}
