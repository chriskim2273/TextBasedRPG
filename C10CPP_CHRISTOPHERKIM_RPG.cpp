/*
Christopher Kim
P10CPP
5/28/19
*/

/*
Features:
- Help menu (H key).
- Saving and Loading (Saving is not automated, just in case players mess up. Press K to save)
- Random Map Generation
- WASD controls
- Wandering Enemies
- Currency
	- Weapon Scrapping (for 5 gold, planning for different values for each weapon)
	- Purchasing Health Potions
- Treasure
- Randomly generated weapons
	- A Sort of Inventory Style System (currently only 1 weapon).
	- Players can replace their current weapons
- Randomly generated healing pads
- Rewards for killing enemies
- Different ways to view the map, make hasmapitem false, increasedvision true, to make the camera only follow player. Set the the whole map as viewable for Mr.Chan's ease of use.
- Different colors for different things on the map.
- Experience and Levels
- Different enemy names and voice lines.
- and more maybe but I forget and am lazy.
*/

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h> 
#include <windows.h>
#include <ctime>
#include <time.h>
#include <string>
#include <fstream>
#include <conio.h>
#include <vector>
#include <ctype.h>
using namespace std;

char map[100][100] = {};
HANDLE color=GetStdHandle(STD_OUTPUT_HANDLE); //just once
int gr,gc,random,randomtwo,mapgencount = 0,coins = 20, remainingenemiestokill = 0, dungeonfloor = 1;
int exp = 0, expmax = 100, level = 1, rewardexp, tempr, count = 5, randomr, choice;
bool ftreasure = false, fweapon = false, fhealpad = false, fenemy = false, fobjective = false, fcoins  = false, hasmapitem = true, increasedvision = false;//
int turn; //determines who is going, the player or the enemy.
//Voice Lines for the enemies.
string voicelines[8] = {"You will never defeat me!","I am the greatest!","I am the most powerful of them all!","You seem weak.","Add to my legacy fool!","Your legacy shall drift away, blown into eternity, like the sands of the desert.","Come mortal, witness your demise.","The cycle of life and death continues, we will live, they will die."};
string enemynames[14] = {"Mogo","Ghaytha","Kitlia","Astisth","Fata","Ova","Biba","Marmasa","Tjolenn","Jinnvaror","Saak","Ahdruk","Sahlier","Bimiak"};

// Prototypes
void foundweapon();
void pdab();
void foundenemy();
void saveg();
void foundhealingpad();
void makemap();
void placetreasures();
void spawnweapons();
void spawnenemies();
void tutorialweapon();
void placehealingpads();
string removeSpaces(string str);
void findcoord();
bool legalmove(int r, int c);
void foundtreasure();
void enemyai();
void printmap();
void foundtreasure();


class Weapons{ //Class for weapons
private:
	string name;
	int damage;
	// I didn't have time to add attack speed and durability.
	int attackspeed;
	int durability;

public:
	int getDamage() const {
       return damage;
   }
	void setDamage(int damage) {
       Weapons::damage = damage;
   }
	int getattackspeed() const {
       return attackspeed;
   }
	void setattackspeed(int attackspeed) {
       Weapons::attackspeed = attackspeed;
   }
   	int getdurability() const {
       return attackspeed;
   }
	void setdurability(int durability) {
       Weapons::durability = durability;
   }
    const string &getName() const {
       return name;
   }
   void setName(const string &name) {
       Weapons::name = name;
   }
    Weapons(string na, int da, int as, int du){
    	name = na;
		damage = da;
		attackspeed = as;
		durability = du;
   }
};

class Character{ // Class for character
private:
	int health;
	int strength;
	int defense;
	string name;
	
public:
   int getHealth() const {
       return health;
   }
   void setHealth(int health) {
       Character::health = health;
   }

   int getStrength() const {
       return strength;
   }
   void setStrength(int strength) {
       Character::strength = strength;
   }

   int getDefense() const {
       return defense;
   }
   void setDefense(int defense) {
       Character::defense = defense;
   }

   const string &getName() const {
       return name;
   }
   void setName(const string &name) {
       Character::name = name;
   }

   Character();
   Character(string n, int h, int s, int d){
       name = n;
       health = h;
       strength = s;
       defense = d;
   }
   
   void viewStat();
   void takeDamage(int n);
   int attack();
};

/*
PROTOTYPE MAP GENERATION:
void makemap(){ // Makes the board
    for(int r=0;r<50;r++){
        for(int c=0;c<50;c++){
        	random = rand()%100;
        	if(r == 0 && c == 0){
        		map[r][c] = 'X';
			}
			else{
				map[r][c] = ' ';
        	}
        	if(c!=0 && c%5 == 0){
        		if(random < 26){
        			map[r][c] = ' ';
				}
				else{
        			map[r][c] = '|';
        		}
			}
        	if(r!=0 && r%5 == 0){
        		if(random < 26){
        			map[r][c] = ' ';
				}
				else{
        			map[r][c] = '-';
        		}
			}
		}
    }
}
*/

//		else if(GetKeyState('W') & 0x8000/*Check if high-order bit is set (1 << 15)*/){

Character Player("Player",100,5,5); //Character Set-Up
Character Enemy("Empty",0,0,0);
Weapons PlayerWeapon("Hands",5,1,999999); //Default weapon of Player
Weapons Tempw("Null",0,0,0); //Empty random weapon
void viewBattleStats(Character a, Character b); //Prototype
void helpgui();
// Creating the Weapons
Weapons woodensword("Wooden_Sword",10,40,20);
Weapons stonesword("Stone_Sword",15,40,30);
Weapons woodenmallet("Wooden_Mallet",20,5,20);
Weapons ironsword("Iron_Sword",20,40,40);
Weapons soulreaper("Soul_Reaper",30,90,100);
//

int main(){
	// Variables
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE); // set full screen
	int health, strength, defense; string name; //stat variables
	int loadorsave = 0, randomr, temp, count=1;
	Character CurrentEnemy("Empty",rand()%101,rand()%30,rand()%5); //Basic empty enemy (before randomization)
	srand((unsigned)time(0));
	//
	//
	// Setup
	makemap();
	placetreasures();
	//pdab();
	//printmap();
	spawnweapons();
	spawnenemies();
	tutorialweapon();
	placehealingpads();
	//
	//
	// Welcome Screen
	string savename, line, pwn;
	int linenum,savecount = 0, savehealth, savecoins, savedefense, savestrength, pwd;
	// Asks if the user wants to save or load. Remember: K is save. Save is manual.
	ifstream opensaveFile("HilrymSave.txt");
	while(loadorsave != 1 || loadorsave != 2){
		system("CLS");
		cout << "#################################" << endl;
		cout << "# Welcome to the Land of Hilrym #" << endl;
		cout << "#               -               #" << endl;
		cout << "# Would You Like To Load A Save #" << endl;
		cout << "#               OR              #" << endl;
		cout << "#       Start A New Game?       #" << endl;
		cout << "#################################" << endl << endl << endl;
		cout << "<< 1. Load A Save" << endl;
		cout << "<< 2. Start A New Game" << endl;
		cin >> loadorsave;
		if(loadorsave == 1){
			for(int i =0; i<3; i++){
				system("CLS");
				cout << ">> Loading Save File.";
				Sleep(500);
				system("CLS");
				cout << ">> Loading Save File..";
				Sleep(500);
				system("CLS");
				cout << ">> Loading Save File...";
			}
			system("CLS");
				//opensaveFile << name << health << strength << defense;
   				//Character Player(name,health,strength,defense);
   			if(!opensaveFile.is_open()){
   				cout << "There is no save file!" << endl;
				do {
				   cout << '\n' << "Press the Enter key to continue.";
				} while (cin.get() != '\n');
   				loadorsave = 2;
   				continue;
			}
			/*
			while(opensaveFile.is_open()){
				while (getline(opensaveFile,line) && savecount == 0){
				  Player.setName(line);
				  savecount++;
				  //opensaveFile.close();
				}
				while (f >>  && savecount == 1){
				  Player.setHealth(linenum);
				  savecount++;
				  //opensaveFile.close();
				}
				while (getline(opensaveFile,linenum) && savecount == 2){
				  Player.setStrength(linenum);
				  savecount++;
				  //opensaveFile.close();
				}
				while (getline(opensaveFile,linenum) && savecount == 3){
				  Player.setDefense(linenum;)
				  savecount++;
				  //opensaveFile.close();
				}
				while (getline(opensaveFile,linenum) && savecount == 4){
				  coins = linenum;
				  savecount++;
				  //opensaveFile.close();
				}
				while (getline(opensaveFile,line) && savecount == 5){
					PlayerWeapon.setName(line);
					savecount++;
				  //opensaveFile.close();
				}
				while (getline(opensaveFile,linenum) && savecount == 6){
					PlayerWeapon.setDamage(line);
					savecount++;
				}
				opensaveFile.close();
			}
			*/
			while(opensaveFile >> savename >> savehealth >> savestrength >> savedefense >> savecoins >> pwn >> pwd >> temp){
				Player.setName(savename);
				Player.setHealth(savehealth);
				Player.setStrength(savestrength);
				Player.setDefense(savedefense);
				coins = savecoins;
				PlayerWeapon.setName(pwn);
				PlayerWeapon.setDamage(pwd);
				dungeonfloor = temp;
			}
			opensaveFile.close();
			cout << ">> Press any key to continue." << endl;
		}
		else if(loadorsave != 1 && loadorsave != 2){
			cin.clear();
			string ignore;
			getline(cin, ignore);
			continue;
		}
		// If user wants a new save.
		if(loadorsave == 2){
			system("CLS");
			cout << "#################################" << endl;
			cout << "# Welcome to the Land of Hilrym #" << endl;
			cout << "#               -               #" << endl;
			cout << "# Info:                         #" << endl;
			cout << "# By Christopher Kim -          #" << endl;
			cout << "# AKA POYATO/KHRIX/POTATOCHEZ   #" << endl;
			cout << "#################################" << endl << endl << endl;
			do {
			   cout << '\n' << "Press the Enter key to continue.";
			} while (cin.get() != '\n');
			do {
			   cout << '\n' << "Press the Enter key to continue.";
			} while (cin.get() != '\n');
			do{
				system("CLS");
				//
				//
				// Ask user for a name for their character.
				cout << "###################################" << endl;
				cout << "# Enter A Name For Your Character #" << endl;
				cout << "#                                 #" << endl;
				cout << "# -Default Stats:-                #" << endl;
				cout << "# >> Health: 100                  #" << endl;
				cout << "# >> Strength: 5                  #" << endl;
				cout << "# >> Defense: 5                   #" << endl;
				cout << "###################################" << endl;
				cout << "Enter: ";
				name = "";
				cin.clear();
				getline(cin, name);
			}while(name == "");
			name = removeSpaces(name);
			Player.setName(name);
			
			//
			//
			
			system("CLS");
			
			//Task.
			cout << " >> Welcome to Hilrym " << name << "! You shall begin your adventure with only your hands and 20 gold pieces." << endl;
			cout << " >> Enemy villagers have taken control of our dungeon. Return with their skulls and you shall be rewarded! " << endl;
			do {
				cout << '\n' << "Press the Enter key to enter the dungeon.";
			} while (cin.get() != '\n');
			system("CLS");
			cout << " >> You see a small handle in the ground, and go to inspect it." << endl;
			do {
				cout << '\n' << "Press the Enter key and then A,S, or D to continue.";
			}while (cin.get() != '\n');
	}
	else if(loadorsave != 1 && loadorsave != 2){
		cin.clear();
		string ignore;
		getline(cin, ignore);
		continue;
	}
	//
	//
	
	// Player movement.
	// Anything below the getkeystate if statements, then check for what the player walked into. 
	while(true){
		findcoord();
		getch();
		if(GetKeyState('S') & 0x8000 && legalmove(gr+1,gc) == true){
			if(map[gr+1][gc] == '*')
				ftreasure = true;
			if(map[gr+1][gc] == '^')
				fweapon = true;
			if(map[gr+1][gc] == '@')
				fenemy = true;
			if(map[gr+1][gc] == '+')
				fhealpad = true;
			system("CLS");
			map[gr+1][gc] = 'X';
			map[gr][gc] = ' ';
			foundtreasure();
			foundweapon();
			foundenemy();
			foundhealingpad();
			enemyai();
			printmap();
		}
		else if(GetKeyState('W') & 0x8000 && legalmove(gr-1,gc) == true){
			if(map[gr-1][gc] == '*')
				ftreasure = true;
			if(map[gr-1][gc] == '^')
				fweapon = true;
			if(map[gr-1][gc] == '@')
				fenemy = true;
			if(map[gr-1][gc] == '+')
				fhealpad = true;
			system("CLS");
			map[gr-1][gc] = 'X';
			map[gr][gc] = ' ';
			foundtreasure();
			foundweapon();
			foundenemy();
			foundhealingpad();
			enemyai();
			printmap();
		}
		else if(GetKeyState('A') & 0x8000 && legalmove(gr,gc-1) == true && gc>0){
			if(map[gr][gc-1] == '*')
				ftreasure = true;
			if(map[gr][gc-1] == '^')
				fweapon = true;
			if(map[gr][gc-1] == '@')
				fenemy = true;
			if(map[gr][gc-1] == '+')
				fhealpad = true;
			system("CLS");
			map[gr][gc-1] = 'X';
			map[gr][gc] = ' ';
			foundtreasure();
			foundweapon();
			foundenemy();
			foundhealingpad();
			enemyai();
			printmap();
		}
		else if(GetKeyState('D') & 0x8000 && legalmove(gr,gc+1) == true && gc<49){
			if(map[gr][gc+1] == '*')
				ftreasure = true;
			if(map[gr][gc+1] == '^')
				fweapon = true;
			if(map[gr][gc+1] == '@')
				fenemy = true;
			if(map[gr][gc+1] == '+')
				fhealpad = true;
			system("CLS");
			map[gr][gc+1] = 'X';
			map[gr][gc] = ' ';
			foundtreasure();
			foundweapon();
			foundenemy();
			foundhealingpad();
			enemyai();
			printmap();
		}
		else if(GetKeyState('H') & 0x8000){
			system("CLS");
			helpgui();
		}
		else if(GetKeyState('K') & 0x8000){
			saveg();
		}
	}
}
}


void Character::viewStat(){ // Prints the stats of the Character
   cout << endl;
   cout << "-" << name << "-" << endl;
   cout << "+Health: " << health << endl;
   cout << "+Strength: " << strength << endl;
   cout << "+Defense: " << defense << endl;
}

int Character::attack(){ //Returns the amount of damage the character will deal
   cout << endl;
   int s = rand() % (strength+1);
	if(turn == 1)
		s += PlayerWeapon.getDamage();
   if(rand()%100 > 90) {
       s = s * 2;
       cout << "<< " << "CRITICAL HIT!" << endl;
   }
   else if(rand() % 100 == 0 ){
       cout << "<< " << name << " missed!" << endl;
       return 0;
   }
   else {
    	cout << "<< " << name << " dealt " << s << " damage" << endl;
   }
   return s;

}

void Character::takeDamage(int n) { // Subtracts HP from the character based on the damage of the opponent
   int damage;
   if(n-defense <=0)
       damage = 0;
   else
       damage = n-defense;
   cout << "<< " << name << " took " << damage << " damage" << endl;
   health -= damage;
}

void viewBattleStats(Character a, Character b){ //returns both party's healths
   cout << endl;
   cout << a.getName() <<"\t\t\t|\t"<<b.getName()<<endl;
   cout << "Health: "<<a.getHealth() <<"\t\t|\t"<<"Health: "<<b.getHealth()<<endl;
}

void foundweapon(){ // If the player found a weapon.
	int randomr, choice;
	randomr = (rand()%5)+1;
	switch(randomr){
		case 1:
			Tempw = woodensword;
			break;
		case 2:
			Tempw = stonesword;
			break;
		case 3:
			Tempw = woodenmallet;
			break;
		case 4:
			Tempw = ironsword;
			break;
		case 5:
			Tempw = soulreaper;
			break;
	}
	if(fweapon == true){
		system("CLS");
		cout << "###################################" << endl;
		cout << "#                                 #" << endl;
		cout << "#          You have found         #" << endl;
		cout << "#             A Weapon!           #" << endl;
		cout << "#                                 #" << endl;
		cout << "#                                 #" << endl;
		cout << "#                                 #" << endl;
		cout << "###################################" << endl;
		cout << ">> Your current weapon is ... " << PlayerWeapon.getName() << endl;
		cout << ">> The weapon you have found is a ... " << Tempw.getName() << endl;
		cout << "<< Damage: " << Tempw.getDamage() << endl;
		if(Tempw.getName() == "Soul_Reaper"){
			cout << ">>      ./~                     " << endl;
			cout << ">> (=#####{>==================- " << endl;
			cout << ">>      `\\_                    " << endl;
			cout << ">> This Weapon is the strongest in the land! Only previously known to exist in myths. You are very lucky adventurer!" << endl;
		}
		cout << "<< Would you like to replace your current weapon?" << endl; // Ask user if they wish to replace their current weapon.
		cout << "1. Yes" << endl;
		cout << "2. No" << endl << endl;
		cin >> choice;
		if(choice == 1){
			PlayerWeapon = Tempw; //replace weapon
			cout << ">> Successfully replaced your weapon!" << endl;
		}
		else if(choice == 2){
			cout << "<< Scrapped for 5 gold coins." << endl;
			coins+=5;
			Sleep(2000);
		}
		else{
			cin.clear();
			cout << "<< Scrapped for 5 gold coins." << endl;
			Sleep(2000);
			coins+=5;
		}
		//printmap();
		fweapon = false;
		Sleep(1000);
		for(int i = 3; i>0; i--){
			system("CLS");
			cout << "###################################" << endl;
			cout << "#                                 #" << endl;
			cout << "#          You have found         #" << endl;
			cout << "#             A Weapon!           #" << endl;
			cout << "#                                 #" << endl;
			cout << "#         CONTINUING IN "<<i<<"         #" << endl;
			cout << "#                                 #" << endl;
			cout << "###################################" << endl;
			Sleep(1000);
			if(i==1){
				system("CLS");
				pdab();
			}
		}
	}
	else{
		return;
	}
}

void pdab(){ //stands for printdefaultactionbox
	cout << "###################################" << endl;
	cout << ">>          --USER STATS--" << endl;
	cout << ">>           HEALTH: " << Player.getHealth() << endl;
	cout << ">>         STRENGTH: " << Player.getStrength() << endl;
	cout << ">>          DEFENSE: " << Player.getDefense() << endl;
	cout << ">>            COINS: " << coins << endl;
	cout << ">>            LEVEL: " << level << " (" << exp << "/" << expmax << ")" << endl;
	cout << ">>   CURRENT WEAPON: " << PlayerWeapon.getName() << endl;
	cout << ">>       ROOM [X,Y]: " << "[" << (10-(gc/5)) << "," << (10-(gr/5)) << "]" << endl;
	cout << "###################################" << endl;
}

void foundenemy(){ // If the player encounters an enemy.
	if(fenemy == true){
		Enemy.setName(enemynames[(rand()%13)+1]);
		Enemy.setHealth((rand()%50)+100);
		Enemy.setStrength((rand()%10)+20);
		Enemy.setDefense(rand()%10);
		cout << "###################################" << endl;
		cout << "#                                 #" << endl;
		cout << "#       You have encountered      #" << endl;
		cout << "#             an Enemy!           #" << endl;
		cout << "#                                 #" << endl;
		cout << "#                                 #" << endl;
		cout << "#                                 #" << endl;
		cout << "###################################" << endl;
		cout << ">>" << Enemy.getName() << ": " << voicelines[(rand()%7)+1] << endl;
		tempr = Enemy.getHealth();
		Enemy.viewStat();
		cout << endl;
		Player.viewStat();
		cout << "+Current Weapon: " << PlayerWeapon.getName() << endl;
		cin.clear();
		while(Player.getHealth() > 0 and Enemy.getHealth() > 0) {
			cout << endl;
			// Display stats.
			cout << "<< Enemies left: " << count << endl;
		   	cout << "<< Coins: " << coins << endl;
		   	cout << "<< Level: " << level << " (" << exp << "/" << expmax << ")" << endl << endl;
		    cout << ">> 1. Purchase an Instant Health Potion - 5 Coins (DOES NOT STACK)" << endl;
		    cout << ">> 2. Attack" << endl;
		    cin.clear();
		    cin >> choice;
			if(choice == 1 && coins >= 5){ // If the user decides to buy an Instant Health Potion, they heal to 100.
				system("CLS");
				cout << ">> " << Player.getName() << " has healed!" << endl;
				coins -= 5;
				Player.setHealth(100);
				viewBattleStats(Player, Enemy);
				continue;
			}
			else if(choice == 2){ // Attack the enemy.
				turn = 1;
				Enemy.takeDamage(Player.attack()); // Damage the enemy
				viewBattleStats(Player, Enemy);
				
			   do {
			       cout << '\n' << "Press the Enter key to continue.";
			   } while (cin.get() != '\n');
			   
				system("CLS");
				turn = 2;
				randomr = (rand()%100)+1;
				if(randomr > 95){ // ~5% chance of the enemy healing
					cout << ">> " << Enemy.getName() << " has healed!" << endl << endl;
					Enemy.setHealth(tempr);
					Player.viewStat();
				}
				else{
				   Player.takeDamage(Enemy.attack()); // Get damaged by the enemy.
				   viewBattleStats(Player, Enemy);
					if(Enemy.getHealth() <= 0){
						system("CLS");
						cout << "-!- " << Enemy.getName() << " has died!" << endl; // State that the enemy has died
						randomr = (rand()%100)+1;
						if(randomr > 95){ // ~5% chance of enemy dropping a weapon
							switch(randomr){
								case 96:
									Tempw = woodensword;
									break;
								case 97:
									Tempw = stonesword;
									break;
								case 98:
									Tempw = woodenmallet;
									break;
								case 99:
									Tempw = ironsword;
									break;
								case 100:
									Tempw = soulreaper;
									break;
							}
							cout << "<< " << Enemy.getName() << " has dropped a " << Tempw.getName() << endl;
							cout << "<< Damage: " << Tempw.getDamage() << endl;
							if(Tempw.getName() == "Soul_Reaper"){
								cout << " >> This Weapon is the strongest in the land! Only previously known to exist in myths. You are very lucky adventurer!" << endl;
							}
							cout << "<< Would you like to replace your current weapon?" << endl; // Ask user if they wish to replace their current weapon.
							cout << "1. Yes" << endl;
							cout << "2. No" << endl << endl;
							cin >> choice;
							if(choice == 1)
								PlayerWeapon = Tempw; //replace weapon
							else if(choice == 2){
								cout << "<< Scrapped for 5 gold coins." << endl;
								Sleep(2000);
								coins+=5;
								continue;
							}
							else{
								cin.clear();
								cout << "<< Scrapped for 5 gold coins." << endl;
								Sleep(2000);
								coins+=5;
							}
						}
						//Reward user with EXP and Coins, if EXP exceed limit, then level them up.
						rewardexp = (rand()%50)+40;
						randomr = (rand()%10)+1;
						cout << "<< You received " << rewardexp << " EXP and " << randomr << " coins!" << endl;
						exp += rewardexp;
						coins += randomr;
						if(exp > expmax){
							exp = 0;
							expmax += 50;
							level++;
							Player.setStrength(Player.getStrength()+5);
							Player.setDefense(Player.getDefense()+5);
							cout << "-!- YOU LEVELED UP TO " << level << "!" << endl;
						}
						count--; //
						Sleep(2000);
					}
				}
			}
			else{
				cin.clear();
				string ignore;
				getline(cin, ignore);
				continue;
			}
		}
		if(Player.getHealth() <= 0){ // If the player has reached 0 health or below, end the game and inform them that they have died.
			cout << endl << "-!- You Died! " << endl;
			cout << endl << ">> You have respawned, but be aware, you have lost at most 10 coins! " << endl;
			if(coins>10)
				coins -= 10;
			else
				coins = 0;
			findcoord();
			map[gr][gc] = '@';
			map[1][1] = 'X';
			Player.setHealth(100);
		}
		fenemy = false;
		return;
	}
	else{
		return;
	}
}

void helpgui(){ // A help gui if the player presses H
	cout << "###################################" << endl;
	cout << "#            HELP MENU            #" << endl;
	cout << "#           ^ - Weapons           #" << endl;
	cout << "#          * - Treasure           #" << endl;
	cout << "#           @ - Enemies           #" << endl;
	cout << "#          + - Healing            #" << endl;
	cout << "#           ~ - Traps             #" << endl;
	cout << "#        -PRESS K TO SAVE-        #" << endl;
	cout << "###################################" << endl;
}

void saveg(){ // Save feature.
	ofstream saveFile("HilrymSave.txt");
	saveFile.clear();
	saveFile << Player.getName() << " " << Player.getHealth() << " " << Player.getStrength() << " " << Player.getDefense() << " " << coins << " " << PlayerWeapon.getName() << " " << PlayerWeapon.getDamage() << " " << dungeonfloor;
	saveFile.close();
}

void foundhealingpad(){ // If the player finds a healing pad.
	if(fhealpad == true){
		system("CLS");
		cout << "###################################" << endl;
		cout << "#                                 #" << endl;
		cout << "#            You've Been          #" << endl;
		cout << "#              Healed             #" << endl;
		cout << "#                                 #" << endl;
		cout << "#                                 #" << endl;
		cout << "#                                 #" << endl;
		cout << "###################################" << endl;
		fhealpad = false;
		Player.setHealth(100);
		do {
		   cout << '\n' << "Press the Enter key to continue.";
		} while (cin.get() != '\n');
	}
}

string removeSpaces(string str){ // Removes spaces from strings.
	char c;
	int count;
	for(int i = 0; i<str.length(); i++){
		c = str[i];
		if(isspace(c))
			count++;
	}
	for(int i = 0; i<count; i++)
		str.erase(str.find(' '), 1);
    return str;
} 

void makemap(){ // Makes the board
    for(int r=0;r<50;r++){
        for(int c=0;c<50;c++){
        	random = rand()%4;
        	if(r == 1 && c == 1){
        		map[r][c] = 'X';
			}
			else{
				map[r][c] = ' ';
        	}
        	if(c!=0 && c%5 == 0){
        		if(mapgencount == random){
        			map[r][c] = ' ';
				}
				if(mapgencount>=4||mapgencount<0)
					mapgencount = 0;
				else{
        			map[r][c] = '|';
        		}
        		randomtwo = rand()%100;
        		if(randomtwo%5==0)
        			mapgencount--;
        		else
        			mapgencount++;
			}
        	if(r!=0 && r%5 == 0){
        		if(mapgencount == random){
        			map[r][c] = ' ';
				}
				if(mapgencount>=4||mapgencount<0)
					mapgencount = 0;
				else{
        			map[r][c] = '-';
        		}
        		if(randomtwo%5==0)
        			mapgencount--;
        		else
        			mapgencount++;
			}
			if(r == 0 || r == 49)
        		map[r][c] = '-';
			if(c == 0 || c == 49)
        		map[r][c] = '|';
			if(r == 45 && c == 47) // just in case the player gets really unlucky
        		map[r][c] = ' ';
		}
    }
}

void enemyai(){// Randomly generates a number from 0-3. Depending on the number, the enemy either goes up, left, right, or down.
	int randum;
	bool success = false;
    for(int r=0;r<50;r++){
        for(int c=0;c<50;c++){
        	success = false;
        	if(map[r][c]=='@'){
        		map[r][c] = ' ';
        		while(success != true){
        			randum = rand()%4;
        			if(randum == 0){
        				if(map[r+1][c]==' '){
        					map[r+1][c] = '@';
        					success = true;
						}
						else{
							continue;
						}
					}
        			if(randum == 1){
        				if(map[r-1][c]==' '){
        					map[r-1][c] = '@';
        					success = true;
						}
						else{
							continue;
						}
					}
        			if(randum == 2){
        				if(map[r][c+1]==' '){
        					map[r][c+1] = '@';
        					success = true;
						}
						else{
							continue;
						}
					}
        			if(randum == 3){
        				if(map[r][c-1]==' '){
        					map[r][c-1] = '@';
        					success = true;
						}
						else{
							continue;
						}
					}
				}
			}
       	}
	}
}

void findcoord(){ // Finds the current coordinates of the player.
    for(int r=0;r<50;r++){
        for(int c=0;c<50;c++){
			if(map[r][c]=='X'){
				gr = r;
				gc = c;
			}
    	}
	}
}

void printmap(){ // Prints the board.
	findcoord();
	int temp = gr/5, temptwo = gc/5;
	if(hasmapitem == true){
	    for(int r=0;r<50;r++){
	        for(int c=0;c<50;c++){
	        	if(map[r][c]=='@'){
	        		SetConsoleTextAttribute(color, 12);
	        		cout << map[r][c];
	        		SetConsoleTextAttribute(color, 7);
				}
				else if(map[r][c]=='^'){
	        		SetConsoleTextAttribute(color, 9);
	        		cout << map[r][c];
	        		SetConsoleTextAttribute(color, 7);
				}
				else if(map[r][c]=='*'){
	        		SetConsoleTextAttribute(color, 14);
	        		cout << map[r][c];
	        		SetConsoleTextAttribute(color, 7);
				}
				else if(map[r][c]=='+'){
	        		SetConsoleTextAttribute(color, 5);
	        		cout << map[r][c];
	        		SetConsoleTextAttribute(color, 7);
				}
				else
					cout << map[r][c];
			}
			cout << endl;
	    }
	}
	else if(increasedvision == true){
	    for(int r=(gr-3);r<(gr+3);r++){
	        for(int c=gc-3;c<gc+3;c++){
	        	if(map[r][c]=='@'){
	        		SetConsoleTextAttribute(color, 12);
	        		cout << map[r][c];
	        		SetConsoleTextAttribute(color, 7);
				}
				else if(map[r][c]=='^'){
	        		SetConsoleTextAttribute(color, 9);
	        		cout << map[r][c];
	        		SetConsoleTextAttribute(color, 7);
				}
				else if(map[r][c]=='*'){
	        		SetConsoleTextAttribute(color, 14);
	        		cout << map[r][c];
	        		SetConsoleTextAttribute(color, 7);
				}
				else if(map[r][c]=='+'){
	        		SetConsoleTextAttribute(color, 5);
	        		cout << map[r][c];
	        		SetConsoleTextAttribute(color, 7);
				}
				else
					cout << map[r][c];
			}
			cout << endl;
	    }
	}
	else{
	    for(int r=(temp*5);r<((temp+1)*5)+1;r++){
	        for(int c=(temptwo*5);c<((temptwo+1)*5)+1;c++){
				cout << map[r][c];
			}
			cout << endl;
	    }
	}
}

void placetreasures(){ // Randomly places one treasure on the map.
	int crand = rand()%50, rrand = rand()%50;
	if(map[crand][rrand]==' ')
		map[crand][rrand] = '*';
}

void placehealingpads(){ // Randomly placces healing pads.
	int crand, rrand;
	for(int i=0;i<10;i++){
		do{
			crand = rand()%50;
			rrand = rand()%50;
		}while(map[crand][rrand]!=' ');
		if(map[crand][rrand]==' ')
			map[crand][rrand] = '+';
	}
}

void spawnenemies(){ // Randomly spawns weapons.
	int crand, rrand;
	for(int i=0;i<30;i++){
		do{
			crand = rand()%50;
			rrand = rand()%50;
		}while(map[crand][rrand]!=' ');
		if(map[crand][rrand]==' ')
			map[crand][rrand] = '@';
	}
}

void tutorialweapon(){ // Spawns at least one weapon right next to or on top of the player.
	int crand, rrand;
	do{
		crand = rand()%5;
		rrand = rand()%5;
	}while(map[crand][rrand]!=' ');
	if(map[crand][rrand]==' ')
		map[crand][rrand] = '^';
}

void spawnweapons(){ // Randomly spawns weapons across the map.
	int crand, rrand;
	for(int i=0;i<5;i++){
		do{
			crand = rand()%50;
			rrand = rand()%50;
		}while(map[crand][rrand]!=' ');
		if(map[crand][rrand]==' ')
			map[crand][rrand] = '^';
	}
}

bool legalmove(int r, int c){ // Checks if the player can move to the spot. Now that I think about it I could have just denied movement if its a wall but *shrug*.
	if(map[r][c] == ' ')
		return true;
	if(map[r][c] == '*') // treasure (+10 coins)
		return true;
	if(map[r][c] == ';' || map[r][c] == '~') //traps
		return true;
	if(map[r][c] == '+') //healing
		return true;
	if(map[r][c] == '@' || map[r][c] == '#') //enemies
		return true;
	if(map[r][c] == '!' || map[r][c] == '$') //objectives or cash
		return true;
	if(map[r][c] == '?') // Letters or dialogue for story
		return true;
	if(map[r][c] == '^') // Weapons
		return true;
	if(map[r][c] == '|' || map[r][c] == '-') //walls
		return false;
}


void foundtreasure(){ // If the player finds the treasure. Treasure is kind of bad honestly.
	if(ftreasure == true){
		cout << "###################################" << endl;
		cout << "#                                 #" << endl;
		cout << "#         !!YAY TREASURE!!        #" << endl;
		cout << "#                                 #" << endl;
		cout << "#            +10 Coins            #" << endl;
		cout << "#                                 #" << endl;
		cout << "#                                 #" << endl;
		cout << "###################################" << endl;
		printmap();
		ftreasure = false;
		Sleep(2000);
		for(int i = 5; i>0; i--){
			system("CLS");
			cout << "###################################" << endl;
			cout << "#                                 #" << endl;
			cout << "#         !!YAY TREASURE!!        #" << endl;
			cout << "#                                 #" << endl;
			cout << "#         CONTINUING IN "<<i<<"         #" << endl;
			cout << "#                                 #" << endl;
			cout << "#                                 #" << endl;
			cout << "###################################" << endl;
			printmap();
			Sleep(1000);
			if(i==1){
				system("CLS");
				pdab();
			}
		}
	}
	else{
		pdab();
		return;
	}
}
