#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include "windows.h"
using namespace std;
//написать фунцию чисто под менюшки игрового процесса
//int ip_menue - адрес файла с описание меню игрового процесса, bool mendifferent - меняется ли менюшка
//face - триггер обработки данных меню игрового процесса


int hd, wd, hdm, wdm; // динамические высота и ширина карт игры и окна меню
int hi, wi; // размеры картинки
const int rad = 15; //сделать так,чтоб в настройках можно было немного изменять
int *nmm; //массив с номерами команд перемещения для мобов

//char *matrx; // массив с символами
string room;// адрес для нахождения файла с картой
string mroom; // адрес для нахождения файла с меню игрового процесса
char mas_imagine[90][90]; //массив для вывода картинки в консоль
char mas[1000][1000], masmenue[90][90]; // массив с текущей локацией mas[hd<90][wd<90], masmenue[][] - массив с текущим меню
const char f{ '0' }, mouse{ 'M' }; //fuck; // f - запретный символ, символ, отечающий за отрисовку " " (пробела)
bool gameover, ifmap = 0, ifmenue = 0, goy = 0, I = 0; // ifmap - проверка, хотим ли мы сменить игровую карту, goy - условие выхода из конструкции
bool mendifferent = 1, menueover = 0; 
int ipmenue = 1;
int x = 5, y = 5, xm, g = 1; // x y  - текущие координаты, g - номер карты; xm - координаты окошка меню
int *menue_description[2]; //массив с координатами вкладок меню
int sizemdm = 0;
bool Interfaceio[50];//был ли когда-нибудь ткрыт тот или инной интерфейс "interface is open?" Interfaceio[ipmenue]


enum jojo {
	menue, game, pause, ima
	};
char jojo;
enum ch_movement { STOP = 0, LEVT, RIGHT, DOWN, UP, ENTER };
char movement;


int counter = 0, num_mobs;

enum NAMES { Jon = 0, Ron, Bob, Sasha };
enum ch_class { soldier = 0, wiz, wizard, king, npc, guard };
// soldier - воин, wiz - добрый волшебник, wizard - злой волшебник, king - король, troll - тролль (враг), npc - просто житель, guard - страж
enum ch_race { HUMAN = 0, TROLL, BOSS };
enum ch_bronya { vest = 0, helmet, boots };
// vest - жилет, helmet - шлем, boots - ботинки
enum ch_weapon { sword = 0, bow, arrows, conjuration, armor };
// weapon - оружие, sword - меч, bow - лук, arrows - стрелы, conjuration - колдовство
enum ch_agres { enemy = 0, potential_enemy, coward, outlaw };
// 
enum ch_path { vv = 0, vn, vl, vp };
//направления движения моба
enum ch_mode { active = 0, passive };
//режим моба: преследование или убегание OR движение по заданной траектории
enum ch_interaction { stoped = 0, hit, take, put };
ch_interaction interactions = stoped;
enum ch_Winment { stopt = 0, leftt, rightt, upt, downt, entert };//окончание от слова two, хахаах
ch_Winment Winment;
enum ch_jojos { meny = 0, sunduk, inventar };
ch_jojos jojos;



const int SWORD = 1, BOW = 2, CONJURATION = 2, ARMSR = 1;
const int SWORDD = 10, BOWD = 2, CONJURATIOND = 2, ARMSD = 1;


struct ARMOR
{
	ch_weapon weapon;
	int leave, damag;
};

struct ARMS
{
	ch_bronya bronya;
	int leave;
};
//а где же ебаные названия структур?? разобраться где они
//их тут нет, затем, чтобы все это в памяти не висело, мы прочитываем эту структуру только при ображении к мобу, вроде.

struct CHES
{

};
CHES ches;



struct user
{
	string Name;

	bool gender;

	ch_race race{ HUMAN };
	ch_class _class{ soldier };

	//ch_weapon weapon[10];
	//ch_bronya bronya[3];

	//int num_weapon;
	ARMOR *armor;
	int rad_attack;
	int num_gen;

	//int num_bronya;
	ARMS *arms;
	int _ap = 50;
	char skin = 'G';
	int health = 250;
	int _mp = 25;

	double _expiriencecap{ 900 };
	double _expirience{ 0 };
	int _level{ 1 };
	int _maxhealth = 250;
};
user USER;

struct Character
{
	NAMES Name;

	int number;
	bool gender;
	bool leaving = true;
	ch_race race;
	ch_class _class;

	int num_weapon;
	//ch_weapon weapon[10];
	//armor = new ARMOR[num_weapon];
	ARMOR *armor;

	int num_bronya;
	//ch_bronya bronya[3];
	ARMS *arms;

	int health;
	ch_agres agres;
	int sizepath;
	ch_path *path;
	int xonset, yonset; // координатывхода на траекторию
	ch_mode mode{ passive };
	int rad_mob = 3;
	int rad_attack;
	int num_gen;
	ch_interaction interectionm; // потом написать чтобы некоторые мобы могли давать тебе что-то, кк
	bool jorgan = 1; // направление по траектории пассивного перемещения моба
	char skin;
	int x_mob, y_mob;
	double _expiriencegiven{900};
};
Character *sushestvo;
bool *dead;


struct m_s // менюшки в процессе игры
{
	int chw, chh;//линейные размеры массива для интерфейса, chw - ширина, chh - высота
	char **massivem;//массив с интерфейсом меню игрового процесса

	int size_game_menue; //размер массива с координатами указателей, на которые ложится мышь
	int *coordinates[2];//массив с координатами указателей

	int chtx, chty;//размеры таблицы, chtx - широта, chty - высота
	int **tablo;//таблица 
	int tab_x, tab_y; //координаты указателя в таблице

	char mouse; //внешний вид указателя (буква)
	int num_work;//номер указателя, на котором стоит мышь

};
m_s expansion;

//m_s fantasia;//потому что фантазия кончилась хахааххаха

enum LAYER { GLOBAL = 0, SETINGS, RAD, SOUND, DIFFICULT, CHARACTER, MAINMENUE };//слой меню
LAYER layer;


struct CH2_VOLUME {

};

struct CH_SOUND {
	m_s fantasia;
	CH2_VOLUME VOLUME;
	bool BACK;
};

struct CH2_RADIUSE {

};

struct CH_RAD {
	m_s fantasia;
	CH2_RADIUSE RADIUSE;
	bool BACK;
};

struct CH2_LVL {

};

struct CH_DIFFICULT {
	m_s fantasia;
	CH2_LVL LVL;
	bool BACK;
};

struct CH_SETTINGS {
	m_s fantasia;
	CH_RAD RAD;
	CH_SOUND SOUND;
	CH_DIFFICULT DIFFICULT;
	bool BACK;
};

struct CH2_NAME {

	//**USER.Name;
	bool BACK;
};

struct CH2_SKIN {
	//**USER.skin;
	bool BACK;
};

struct CH_CHARACTER {
	m_s fantasia;
	CH2_NAME NAME;
	CH2_SKIN SKIN;
	bool BACK;
};

struct CH_MAINMENUE {
	m_s fantasia;
	int YES, NO;
};


struct CH_GLOBAL {
	m_s fantasia;
	bool CONTINUE;
	CH_SETTINGS SETTINGS;
	CH_CHARACTER CHARACTER;
	CH_MAINMENUE MAINMENUE;
};
CH_GLOBAL global;

void Userchange() {
	if (
		USER._expirience >= USER._expiriencecap) {
			USER._level++;
			USER._expiriencecap= USER._level * USER._expiriencecap / 1.5;
			USER._ap = USER._ap + 25 * USER._level;
			USER._expirience = 0;
			USER._mp = USER._mp + 10 * USER._level;
			USER._maxhealth = USER._maxhealth + 50 * USER._level;
			USER.health = USER._maxhealth;
	}
}

void Menueroom(int ie, LAYER Layer) {
	//ip_menue добавить в глобальные переменные
	//ЭТА ФУНКЦИЯ ЗАНИМАЕТ 328 СТРОК!!!
	if (mendifferent) {
		switch (ie)//инициализация адреса карты ipmenue - pmenu = ie:)
		{
		case 1:
			mroom = "D:\\Users\\samsonp\\Documents\\GitHub\\arazu.mai.cpp\\arazu.vs\\arazu.vs\\menues\\GLOBAL.txt";// потом можно создать доп. переменную для адресов менюшек
			break;// чтобы глаза room не мазолил
		case 2:
			mroom = "D:\\Users\\samsonp\\Documents\\GitHub\\arazu.mai.cpp\\arazu.vs\\arazu.vs\\menues\\11\\SETINGS.txt";
			break;
		case 3:
			mroom = "D:\\Users\\samsonp\\Documents\\GitHub\\arazu.mai.cpp\\arazu.vs\\arazu.vs\\menues\\11\\CHARACTER.txt";
			break;
		case 4:
			mroom = "D:\\Users\\samsonp\\Documents\\GitHub\\arazu.mai.cpp\\arazu.vs\\arazu.vs\\menues\\11\\MAINMENUE.txt";
			break;
		case 5:
			mroom = "D:\\Users\\samsonp\\Documents\\GitHub\\arazu.mai.cpp\\arazu.vs\\arazu.vs\\menues\\11\\12\\DIFFICULT.txt";
			break;
		case 6:
			mroom = "D:\\Users\\samsonp\\Documents\\GitHub\\arazu.mai.cpp\\arazu.vs\\arazu.vs\\menues\\11\\12\\NAME.txt";
			break;
		case 7:
			mroom = "D:\\Users\\samsonp\\Documents\\GitHub\\arazu.mai.cpp\\arazu.vs\\arazu.vs\\menues\\11\\12\\RAD.txt";
			break;
		case 8:
			mroom = "D:\\Users\\samsonp\\Documents\\GitHub\\arazu.mai.cpp\\arazu.vs\\arazu.vs\\menues\\11\\12\\SKIN.txt";
			break;
		case 9:
			mroom = "D:\\Users\\samsonp\\Documents\\GitHub\\arazu.mai.cpp\\arazu.vs\\arazu.vs\\menues\\11\\12\\SOUND.txt";
			break;
		}
		Interfaceio[ie] = true;
		mendifferent = false;
	}

	ifstream file(mroom);

	switch (layer)
	{
	case GLOBAL:
		file >> global.fantasia.chh >> global.fantasia.chw;//читаю размеры интерфейса
		global.fantasia.massivem = new char *[global.fantasia.chw]; //создаю массив под полученные размеры
		for (int i = 0; i < global.fantasia.chw; ++i) {
			global.fantasia.massivem[i] = new char[global.fantasia.chw];
		}

		for (int i = 0; i < global.fantasia.chh; ++i) {
			for (int j = 0; j < global.fantasia.chw; ++j) {
				file >> global.fantasia.massivem[j][i];
			}
		}

		file >> global.fantasia.size_game_menue;//ввод/чтение размера массива координат указателя
		global.fantasia.coordinates[0] = new int[global.fantasia.size_game_menue];
		global.fantasia.coordinates[1] = new int[global.fantasia.size_game_menue];

		for (int i = 0; i < global.fantasia.size_game_menue; ++i) {
			file >> global.fantasia.coordinates[0][i] >> global.fantasia.coordinates[1][i];// соответственно x, y
		}

		file >> global.fantasia.chtx >> global.fantasia.chty;// ввод ширины и высоты табло tablo 
		global.fantasia.tablo = new int *[global.fantasia.chtx];
		for (int i = 0; i < global.fantasia.chtx; ++i) {
			global.fantasia.tablo[i] = new int[global.fantasia.chty];
		}
		//expansion.tablo[expansion.chtx][expansion.chty]

		for (int i = 0; i < global.fantasia.chty; ++i) {
			for (int j = 0; j < global.fantasia.chtx; ++j) {
				file >> global.fantasia.tablo[j][i];
			}
		}

		global.fantasia.num_work = 0; //номер указателя, на который ложится мышь в первый раз 

		global.fantasia.tab_x = 0; global.fantasia.tab_y = 0;

		file >> global.fantasia.mouse; //внешний вид мыши
		break;
	case SETINGS:
		file >> global.SETTINGS.fantasia.chh >> global.SETTINGS.fantasia.chw;//читаю размеры интерфейса
		global.SETTINGS.fantasia.massivem = new char *[global.SETTINGS.fantasia.chw]; //создаю массив под полученные размеры
		for (int i = 0; i < global.SETTINGS.fantasia.chw; ++i) {
			global.SETTINGS.fantasia.massivem[i] = new char[global.SETTINGS.fantasia.chw];
		}

		for (int i = 0; i < global.SETTINGS.fantasia.chh; ++i) {
			for (int j = 0; j < global.SETTINGS.fantasia.chw; ++j) {
				file >> global.SETTINGS.fantasia.massivem[j][i];
			}
		}

		file >> global.SETTINGS.fantasia.size_game_menue;//ввод/чтение размера массива координат указателя
		global.SETTINGS.fantasia.coordinates[0] = new int[global.SETTINGS.fantasia.size_game_menue];
		global.SETTINGS.fantasia.coordinates[1] = new int[global.SETTINGS.fantasia.size_game_menue];

		for (int i = 0; i < global.SETTINGS.fantasia.size_game_menue; ++i) {
			file >> global.SETTINGS.fantasia.coordinates[0][i] >> global.SETTINGS.fantasia.coordinates[1][i];// соответственно x, y
		}

		file >> global.SETTINGS.fantasia.chtx >> global.SETTINGS.fantasia.chty;// ввод ширины и высоты табло tablo 
		global.SETTINGS.fantasia.tablo = new int *[global.SETTINGS.fantasia.chtx];
		for (int i = 0; i < global.SETTINGS.fantasia.chtx; ++i) {
			global.SETTINGS.fantasia.tablo[i] = new int[global.SETTINGS.fantasia.chty];
		}
		//expansion.tablo[expansion.chtx][expansion.chty]

		for (int i = 0; i < global.SETTINGS.fantasia.chty; ++i) {
			for (int j = 0; j < global.SETTINGS.fantasia.chtx; ++j) {
				file >> global.SETTINGS.fantasia.tablo[j][i];
			}
		}

		global.SETTINGS.fantasia.num_work = 0; //номер указателя, на который ложится мышь в первый раз 

		global.SETTINGS.fantasia.tab_x = 0; global.SETTINGS.fantasia.tab_y = 0;

		file >> global.SETTINGS.fantasia.mouse; //внешний вид мыши
		break;
	case RAD:
		file >> global.SETTINGS.RAD.fantasia.chh >> global.SETTINGS.RAD.fantasia.chw;//читаю размеры интерфейса
		global.SETTINGS.RAD.fantasia.massivem = new char *[global.SETTINGS.RAD.fantasia.chw]; //создаю массив под полученные размеры
		for (int i = 0; i < global.SETTINGS.RAD.fantasia.chw; ++i) {
			global.SETTINGS.RAD.fantasia.massivem[i] = new char[global.SETTINGS.RAD.fantasia.chw];
		}

		for (int i = 0; i < global.SETTINGS.RAD.fantasia.chh; ++i) {
			for (int j = 0; j < global.SETTINGS.RAD.fantasia.chw; ++j) {
				file >> global.SETTINGS.RAD.fantasia.massivem[j][i];
			}
		}

		file >> global.SETTINGS.RAD.fantasia.size_game_menue;//ввод/чтение размера массива координат указателя
		global.SETTINGS.RAD.fantasia.coordinates[0] = new int[global.SETTINGS.RAD.fantasia.size_game_menue];
		global.SETTINGS.RAD.fantasia.coordinates[1] = new int[global.SETTINGS.RAD.fantasia.size_game_menue];

		for (int i = 0; i < global.SETTINGS.RAD.fantasia.size_game_menue; ++i) {
			file >> global.SETTINGS.RAD.fantasia.coordinates[0][i] >> global.SETTINGS.RAD.fantasia.coordinates[1][i];// соответственно x, y
		}

		file >> global.SETTINGS.RAD.fantasia.chtx >> global.SETTINGS.RAD.fantasia.chty;// ввод ширины и высоты табло tablo 
		global.SETTINGS.RAD.fantasia.tablo = new int *[global.SETTINGS.RAD.fantasia.chtx];
		for (int i = 0; i < global.SETTINGS.RAD.fantasia.chtx; ++i) {
			global.SETTINGS.RAD.fantasia.tablo[i] = new int[global.SETTINGS.RAD.fantasia.chty];
		}
		//expansion.tablo[expansion.chtx][expansion.chty]

		for (int i = 0; i < global.SETTINGS.RAD.fantasia.chty; ++i) {
			for (int j = 0; j < global.SETTINGS.RAD.fantasia.chtx; ++j) {
				file >> global.SETTINGS.RAD.fantasia.tablo[j][i];
			}
		}

		global.SETTINGS.RAD.fantasia.num_work = 0; //номер указателя, на который ложится мышь в первый раз 

		global.SETTINGS.RAD.fantasia.tab_x = 0; global.SETTINGS.RAD.fantasia.tab_y = 0;

		file >> global.SETTINGS.RAD.fantasia.mouse; //внешний вид мыши
		break;
	case SOUND:
		file >> global.SETTINGS.SOUND.fantasia.chh >> global.SETTINGS.SOUND.fantasia.chw;//читаю размеры интерфейса
		global.SETTINGS.SOUND.fantasia.massivem = new char *[global.SETTINGS.SOUND.fantasia.chw]; //создаю массив под полученные размеры
		for (int i = 0; i < global.SETTINGS.SOUND.fantasia.chw; ++i) {
			global.SETTINGS.SOUND.fantasia.massivem[i] = new char[global.SETTINGS.SOUND.fantasia.chw];
		}

		for (int i = 0; i < global.SETTINGS.SOUND.fantasia.chh; ++i) {
			for (int j = 0; j < global.SETTINGS.SOUND.fantasia.chw; ++j) {
				file >> global.SETTINGS.SOUND.fantasia.massivem[j][i];
			}
		}

		file >> global.SETTINGS.SOUND.fantasia.size_game_menue;//ввод/чтение размера массива координат указателя
		global.SETTINGS.SOUND.fantasia.coordinates[0] = new int[global.SETTINGS.SOUND.fantasia.size_game_menue];
		global.SETTINGS.SOUND.fantasia.coordinates[1] = new int[global.SETTINGS.SOUND.fantasia.size_game_menue];

		for (int i = 0; i < global.SETTINGS.SOUND.fantasia.size_game_menue; ++i) {
			file >> global.SETTINGS.SOUND.fantasia.coordinates[0][i] >> global.SETTINGS.SOUND.fantasia.coordinates[1][i];// соответственно x, y
		}

		file >> global.SETTINGS.SOUND.fantasia.chtx >> global.SETTINGS.SOUND.fantasia.chty;// ввод ширины и высоты табло tablo 
		global.SETTINGS.SOUND.fantasia.tablo = new int *[global.SETTINGS.SOUND.fantasia.chtx];
		for (int i = 0; i < global.SETTINGS.SOUND.fantasia.chtx; ++i) {
			global.SETTINGS.SOUND.fantasia.tablo[i] = new int[global.SETTINGS.SOUND.fantasia.chty];
		}
		//expansion.tablo[expansion.chtx][expansion.chty]
		
		for (int i = 0; i < global.SETTINGS.SOUND.fantasia.chty; ++i) {
			for (int j = 0; j < global.SETTINGS.SOUND.fantasia.chtx; ++j) {
				file >> global.SETTINGS.SOUND.fantasia.tablo[j][i];
			}
		}

		global.SETTINGS.SOUND.fantasia.num_work = 0; //номер указателя, на который ложится мышь в первый раз 

		global.SETTINGS.SOUND.fantasia.tab_x = 0; global.SETTINGS.SOUND.fantasia.tab_y = 0;

		file >> global.SETTINGS.SOUND.fantasia.mouse; //внешний вид мыши
		break;
	case DIFFICULT:
		file >> global.SETTINGS.DIFFICULT.fantasia.chh >> global.SETTINGS.DIFFICULT.fantasia.chw;//читаю размеры интерфейса
		global.SETTINGS.DIFFICULT.fantasia.massivem = new char *[global.SETTINGS.DIFFICULT.fantasia.chw]; //создаю массив под полученные размеры
		for (int i = 0; i < global.SETTINGS.DIFFICULT.fantasia.chw; ++i) {
			global.SETTINGS.DIFFICULT.fantasia.massivem[i] = new char[global.SETTINGS.DIFFICULT.fantasia.chw];
		}

		for (int i = 0; i < global.SETTINGS.DIFFICULT.fantasia.chh; ++i) {
			for (int j = 0; j < global.SETTINGS.DIFFICULT.fantasia.chw; ++j) {
				file >> global.SETTINGS.DIFFICULT.fantasia.massivem[j][i];
			}
		}

		file >> global.SETTINGS.DIFFICULT.fantasia.size_game_menue;//ввод/чтение размера массива координат указателя
		global.SETTINGS.DIFFICULT.fantasia.coordinates[0] = new int[global.SETTINGS.DIFFICULT.fantasia.size_game_menue];
		global.SETTINGS.DIFFICULT.fantasia.coordinates[1] = new int[global.SETTINGS.DIFFICULT.fantasia.size_game_menue];

		for (int i = 0; i < global.SETTINGS.DIFFICULT.fantasia.size_game_menue; ++i) {
			file >> global.SETTINGS.DIFFICULT.fantasia.coordinates[0][i] >> global.SETTINGS.DIFFICULT.fantasia.coordinates[1][i];// соответственно x, y
		}

		file >> global.SETTINGS.DIFFICULT.fantasia.chtx >> global.SETTINGS.DIFFICULT.fantasia.chty;// ввод ширины и высоты табло tablo 
		global.SETTINGS.DIFFICULT.fantasia.tablo = new int *[global.SETTINGS.DIFFICULT.fantasia.chtx];
		for (int i = 0; i < global.SETTINGS.DIFFICULT.fantasia.chtx; ++i) {
			global.SETTINGS.DIFFICULT.fantasia.tablo[i] = new int[global.SETTINGS.DIFFICULT.fantasia.chty];
		}
		//expansion.tablo[expansion.chtx][expansion.chty]

		for (int i = 0; i < global.SETTINGS.DIFFICULT.fantasia.chty; ++i) {
			for (int j = 0; j < global.SETTINGS.DIFFICULT.fantasia.chtx; ++j) {
				file >> global.SETTINGS.DIFFICULT.fantasia.tablo[j][i];
			}
		}

		global.SETTINGS.DIFFICULT.fantasia.num_work = 0; //номер указателя, на который ложится мышь в первый раз 

		global.SETTINGS.DIFFICULT.fantasia.tab_x = 0; global.SETTINGS.DIFFICULT.fantasia.tab_y = 0;

		file >> global.SETTINGS.DIFFICULT.fantasia.mouse; //внешний вид мыши
		break;
	case CHARACTER:
		file >> global.CHARACTER.fantasia.chh >> global.CHARACTER.fantasia.chw;//читаю размеры интерфейса
		global.CHARACTER.fantasia.massivem = new char *[global.CHARACTER.fantasia.chw]; //создаю массив под полученные размеры
		for (int i = 0; i < global.CHARACTER.fantasia.chw; ++i) {
			global.CHARACTER.fantasia.massivem[i] = new char[global.CHARACTER.fantasia.chw];
		}

		for (int i = 0; i < global.CHARACTER.fantasia.chh; ++i) {
			for (int j = 0; j < global.CHARACTER.fantasia.chw; ++j) {
				file >> global.CHARACTER.fantasia.massivem[j][i];
			}
		}

		file >> global.CHARACTER.fantasia.size_game_menue;//ввод/чтение размера массива координат указателя
		global.CHARACTER.fantasia.coordinates[0] = new int[global.CHARACTER.fantasia.size_game_menue];
		global.CHARACTER.fantasia.coordinates[1] = new int[global.CHARACTER.fantasia.size_game_menue];

		for (int i = 0; i < global.CHARACTER.fantasia.size_game_menue; ++i) {
			file >> global.CHARACTER.fantasia.coordinates[0][i] >> global.CHARACTER.fantasia.coordinates[1][i];// соответственно x, y
		}

		file >> global.CHARACTER.fantasia.chtx >> global.CHARACTER.fantasia.chty;// ввод ширины и высоты табло tablo 
		global.CHARACTER.fantasia.tablo = new int *[global.CHARACTER.fantasia.chtx];
		for (int i = 0; i < global.CHARACTER.fantasia.chtx; ++i) {
			global.CHARACTER.fantasia.tablo[i] = new int[global.CHARACTER.fantasia.chty];
		}
		//expansion.tablo[expansion.chtx][expansion.chty]

		for (int i = 0; i < global.CHARACTER.fantasia.chty; ++i) {
			for (int j = 0; j < global.CHARACTER.fantasia.chtx; ++j) {
				file >> global.CHARACTER.fantasia.tablo[j][i];
			}
		}

		global.CHARACTER.fantasia.num_work = 0; //номер указателя, на который ложится мышь в первый раз 

		global.CHARACTER.fantasia.tab_x = 0; global.CHARACTER.fantasia.tab_y = 0;

		file >> global.CHARACTER.fantasia.mouse; //внешний вид мыши
		break;
	case MAINMENUE:
		file >> global.MAINMENUE.fantasia.chh >> global.MAINMENUE.fantasia.chw;//читаю размеры интерфейса
		global.MAINMENUE.fantasia.massivem = new char *[global.MAINMENUE.fantasia.chw]; //создаю массив под полученные размеры
		for (int i = 0; i < global.MAINMENUE.fantasia.chw; ++i) {
			global.MAINMENUE.fantasia.massivem[i] = new char[global.MAINMENUE.fantasia.chw];
		}

		for (int i = 0; i < global.MAINMENUE.fantasia.chh; ++i) {
			for (int j = 0; j < global.MAINMENUE.fantasia.chw; ++j) {
				file >> global.MAINMENUE.fantasia.massivem[j][i];
			}
		}

		file >> global.MAINMENUE.fantasia.size_game_menue;//ввод/чтение размера массива координат указателя
		global.MAINMENUE.fantasia.coordinates[0] = new int[global.MAINMENUE.fantasia.size_game_menue];
		global.MAINMENUE.fantasia.coordinates[1] = new int[global.MAINMENUE.fantasia.size_game_menue];

		for (int i = 0; i < global.MAINMENUE.fantasia.size_game_menue; ++i) {
			file >> global.MAINMENUE.fantasia.coordinates[0][i] >> global.MAINMENUE.fantasia.coordinates[1][i];// соответственно x, y
		}

		file >> global.MAINMENUE.fantasia.chtx >> global.MAINMENUE.fantasia.chty;// ввод ширины и высоты табло tablo 
		global.MAINMENUE.fantasia.tablo = new int *[global.MAINMENUE.fantasia.chtx];
		for (int i = 0; i < global.MAINMENUE.fantasia.chtx; ++i) {
			global.MAINMENUE.fantasia.tablo[i] = new int[global.MAINMENUE.fantasia.chty];
		}
		//expansion.tablo[expansion.chtx][expansion.chty]

		for (int i = 0; i < global.MAINMENUE.fantasia.chty; ++i) {
			for (int j = 0; j < global.MAINMENUE.fantasia.chtx; ++j) {
				file >> global.MAINMENUE.fantasia.tablo[j][i];
			}
		}

		global.MAINMENUE.fantasia.num_work = 0; //номер указателя, на который ложится мышь в первый раз 

		global.MAINMENUE.fantasia.tab_x = 0; global.MAINMENUE.fantasia.tab_y = 0;

		file >> global.MAINMENUE.fantasia.mouse; //внешний вид мыши
		break;
	}
	
	file.close();
	
	// дальше работа со структурой открытой менюхи, сейчас структуру напишу
	// структуру дописал, ридер меню игрового процесса дописал
	//сейчас буду писать MoveWin - описание реакций структур меню игрового процесса, сундуков и инвентаря
}
//чтение файла с меню игрового процесса

void MoveWin() {
	switch (jojos) {
	case meny: /////////////////

		switch (Winment)
		{
		case upt:
			switch (layer)
			{
			case GLOBAL: 
				if (global.fantasia.tab_y > 0) {
					global.fantasia.num_work = global.fantasia.tablo[global.fantasia.tab_x][global.fantasia.tab_y - 1];
					global.fantasia.tab_y--;
				}
				break;
			case SETINGS:
				if (global.SETTINGS.fantasia.tab_y > 0) {
					global.SETTINGS.fantasia.num_work = global.SETTINGS.fantasia.tablo[global.SETTINGS.fantasia.tab_x][global.SETTINGS.fantasia.tab_y - 1];
					global.SETTINGS.fantasia.tab_y--;
				}
				break;
			case RAD:
				if (global.SETTINGS.RAD.fantasia.tab_y > 0) {
					global.SETTINGS.RAD.fantasia.num_work = global.SETTINGS.RAD.fantasia.tablo[global.SETTINGS.RAD.fantasia.tab_x][global.SETTINGS.RAD.fantasia.tab_y - 1];
					global.SETTINGS.RAD.fantasia.tab_y--;
				}
				break;
			case SOUND:
				if (global.SETTINGS.SOUND.fantasia.tab_y > 0) {
					global.SETTINGS.SOUND.fantasia.num_work = global.SETTINGS.SOUND.fantasia.tablo[global.SETTINGS.SOUND.fantasia.tab_x][global.SETTINGS.SOUND.fantasia.tab_y - 1];
					global.SETTINGS.SOUND.fantasia.tab_y--;
				}
				break;
			case DIFFICULT:
				if (global.SETTINGS.DIFFICULT.fantasia.tab_y > 0) {
					global.SETTINGS.DIFFICULT.fantasia.num_work = global.SETTINGS.DIFFICULT.fantasia.tablo[global.SETTINGS.DIFFICULT.fantasia.tab_x][global.SETTINGS.DIFFICULT.fantasia.tab_y - 1];
					global.SETTINGS.DIFFICULT.fantasia.tab_y--;
				}
				break;
			case MAINMENUE:
				if (global.MAINMENUE.fantasia.tab_y > 0) {
					global.MAINMENUE.fantasia.num_work = global.MAINMENUE.fantasia.tablo[global.MAINMENUE.fantasia.tab_x][global.MAINMENUE.fantasia.tab_y - 1];
					global.MAINMENUE.fantasia.tab_y--;
				}
				break;
			}
			break;


		case leftt:
			switch (layer)
			{
			case GLOBAL:
				if (global.fantasia.tab_x > 0) {
					global.fantasia.num_work = global.fantasia.tablo[global.fantasia.tab_x - 1][global.fantasia.tab_y];
					global.fantasia.tab_x--;
				}
				break;
			case SETINGS:
				if (global.SETTINGS.fantasia.tab_x > 0) {
					global.SETTINGS.fantasia.num_work = global.SETTINGS.fantasia.tablo[global.SETTINGS.fantasia.tab_x - 1][global.SETTINGS.fantasia.tab_y];
					global.SETTINGS.fantasia.tab_x--;
				}
				break;
			case RAD:
				if (global.SETTINGS.RAD.fantasia.tab_x > 0) {
					global.SETTINGS.RAD.fantasia.num_work = global.SETTINGS.RAD.fantasia.tablo[global.SETTINGS.RAD.fantasia.tab_x - 1][global.SETTINGS.RAD.fantasia.tab_y];
					global.SETTINGS.RAD.fantasia.tab_x--;
				}
				break;
			case SOUND:
				if (global.SETTINGS.SOUND.fantasia.tab_x > 0) {
					global.SETTINGS.SOUND.fantasia.num_work = global.SETTINGS.SOUND.fantasia.tablo[global.SETTINGS.SOUND.fantasia.tab_x - 1][global.SETTINGS.SOUND.fantasia.tab_y];
					global.SETTINGS.SOUND.fantasia.tab_x--;
				}
				break;
			case DIFFICULT:
				if (global.SETTINGS.DIFFICULT.fantasia.tab_x > 0) {
					global.SETTINGS.DIFFICULT.fantasia.num_work = global.SETTINGS.DIFFICULT.fantasia.tablo[global.SETTINGS.DIFFICULT.fantasia.tab_x - 1][global.SETTINGS.DIFFICULT.fantasia.tab_y];
					global.SETTINGS.DIFFICULT.fantasia.tab_x--;
				}
				break;
			case MAINMENUE:
				if (global.MAINMENUE.fantasia.tab_x > 0) {
					global.MAINMENUE.fantasia.num_work = global.MAINMENUE.fantasia.tablo[global.MAINMENUE.fantasia.tab_x - 1][global.MAINMENUE.fantasia.tab_y];
					global.MAINMENUE.fantasia.tab_x--;
				}
				break;
			}
			break;


		case rightt:
			switch (layer)
			{
			case GLOBAL:
				if (global.fantasia.tab_x < global.fantasia.chtx - 1) {
					global.fantasia.num_work = global.fantasia.tablo[global.fantasia.tab_x + 1][global.fantasia.tab_y];
					global.fantasia.tab_x++;
				}
				break;
			case SETINGS:
				if (global.SETTINGS.fantasia.tab_x < global.SETTINGS.fantasia.chtx - 1) {
					global.SETTINGS.fantasia.num_work = global.SETTINGS.fantasia.tablo[global.SETTINGS.fantasia.tab_x + 1][global.SETTINGS.fantasia.tab_y];
					global.SETTINGS.fantasia.tab_x++;
				}
				break;
			case RAD:
				if (global.SETTINGS.RAD.fantasia.tab_x < global.SETTINGS.RAD.fantasia.chtx - 1) {
					global.SETTINGS.RAD.fantasia.num_work = global.SETTINGS.RAD.fantasia.tablo[global.SETTINGS.RAD.fantasia.tab_x + 1][global.SETTINGS.RAD.fantasia.tab_y];
					global.SETTINGS.RAD.fantasia.tab_x++;
				}
				break;
			case SOUND:
				if (global.SETTINGS.SOUND.fantasia.tab_x < global.SETTINGS.SOUND.fantasia.chtx - 1) {
					global.SETTINGS.SOUND.fantasia.num_work = global.SETTINGS.SOUND.fantasia.tablo[global.SETTINGS.SOUND.fantasia.tab_x + 1][global.SETTINGS.SOUND.fantasia.tab_y];
					global.SETTINGS.SOUND.fantasia.tab_x++;
				}
				break;
			case DIFFICULT:
				if (global.SETTINGS.DIFFICULT.fantasia.tab_x < global.SETTINGS.DIFFICULT.fantasia.chtx - 1) {
					global.SETTINGS.DIFFICULT.fantasia.num_work = global.SETTINGS.DIFFICULT.fantasia.tablo[global.SETTINGS.DIFFICULT.fantasia.tab_x + 1][global.SETTINGS.DIFFICULT.fantasia.tab_y];
					global.SETTINGS.DIFFICULT.fantasia.tab_x++;
				}
				break;
			case MAINMENUE:
				if (global.MAINMENUE.fantasia.tab_x < global.MAINMENUE.fantasia.chtx - 1) {
					global.MAINMENUE.fantasia.num_work = global.MAINMENUE.fantasia.tablo[global.MAINMENUE.fantasia.tab_x + 1][global.MAINMENUE.fantasia.tab_y];
					global.MAINMENUE.fantasia.tab_x++;
				}
				break;
			}
			break;


		case downt:
			switch (layer)
			{
			case GLOBAL:
				if (global.fantasia.tab_y < global.fantasia.chty - 1) {
					global.fantasia.num_work = global.fantasia.tablo[global.fantasia.tab_x][global.fantasia.tab_y + 1];
					global.fantasia.tab_y++;
				}
				break;
			case SETINGS:
				if (global.SETTINGS.fantasia.tab_y < global.SETTINGS.fantasia.chty - 1) {
					global.SETTINGS.fantasia.num_work = global.SETTINGS.fantasia.tablo[global.SETTINGS.fantasia.tab_x][global.SETTINGS.fantasia.tab_y + 1];
					global.SETTINGS.fantasia.tab_y++;
				}
				break;
			case RAD:
				if (global.SETTINGS.RAD.fantasia.tab_y < global.SETTINGS.RAD.fantasia.chty - 1) {
					global.SETTINGS.RAD.fantasia.num_work = global.SETTINGS.RAD.fantasia.tablo[global.SETTINGS.RAD.fantasia.tab_x][global.SETTINGS.RAD.fantasia.tab_y + 1];
					global.SETTINGS.RAD.fantasia.tab_y++;
				}
				break;
			case SOUND:
				if (global.SETTINGS.SOUND.fantasia.tab_y < global.SETTINGS.SOUND.fantasia.chty - 1) {
					global.SETTINGS.SOUND.fantasia.num_work = global.SETTINGS.SOUND.fantasia.tablo[global.SETTINGS.SOUND.fantasia.tab_x][global.SETTINGS.SOUND.fantasia.tab_y + 1];
					global.SETTINGS.SOUND.fantasia.tab_y++;
				}
				break;
			case DIFFICULT:
				if (global.SETTINGS.DIFFICULT.fantasia.tab_y < global.SETTINGS.DIFFICULT.fantasia.chty - 1) {
					global.SETTINGS.DIFFICULT.fantasia.num_work = global.SETTINGS.DIFFICULT.fantasia.tablo[global.SETTINGS.DIFFICULT.fantasia.tab_x][global.SETTINGS.DIFFICULT.fantasia.tab_y + 1];
					global.SETTINGS.DIFFICULT.fantasia.tab_y++;
				}
				break;
			case MAINMENUE:
				if (global.MAINMENUE.fantasia.tab_y < global.MAINMENUE.fantasia.chty - 1) {
					global.MAINMENUE.fantasia.num_work = global.MAINMENUE.fantasia.tablo[global.MAINMENUE.fantasia.tab_x][global.MAINMENUE.fantasia.tab_y + 1];
					global.MAINMENUE.fantasia.tab_y++;
				}
				break;
			}
			break;


		case entert:
			switch (layer)
			{
			case GLOBAL:
				switch (global.fantasia.num_work + 1) {
				case 1: mendifferent = true; jojo = game; menueover = 1; break;//continue
				case 2: ipmenue = 2; if (!Interfaceio[ipmenue]) mendifferent = true; layer = SETINGS; break;//setting
				case 3: ipmenue = 3; if (!Interfaceio[ipmenue]) mendifferent = true; layer = CHARACTER; break;//character
				case 4: ipmenue = 4; if (!Interfaceio[ipmenue]) mendifferent = true; layer = MAINMENUE; break;//mainmenue
				}
				break;

			case SETINGS:
				switch (global.SETTINGS.fantasia.num_work + 1) {
				case 1: ipmenue = 7; if (!Interfaceio[ipmenue]) mendifferent = true; layer = RAD; break;
				case 2: ipmenue = 9; if (!Interfaceio[ipmenue]) mendifferent = true; layer = SOUND; break;
				case 3: ipmenue = 5; if (!Interfaceio[ipmenue]) mendifferent = true; layer = DIFFICULT; break;
				case 4: ipmenue = 2; layer = GLOBAL; break;
				}
				break;

			case RAD:
				switch (global.SETTINGS.RAD.fantasia.num_work + 1) {
				case 1: //ipmenue = ?; if (!Interfaceio[ipmenue]) mendifferent = true; layer = RADIUSE; 
					break;
				case 2: ipmenue = 2; layer = SETINGS; break;
				}
				break;

			case SOUND:
				switch (global.SETTINGS.SOUND.fantasia.num_work + 1) {
				case 1: //ipmenue = ?; if (!Interfaceio[ipmenue]) mendifferent = true; layer = VOLUME; 
					break;
				case 2: ipmenue = 2; layer = SETINGS; break;
				}
				break;

			case DIFFICULT:
				switch (global.SETTINGS.DIFFICULT.fantasia.num_work + 1) {
				case 1: //ipmenue = ?; if (!Interfaceio[ipmenue]) mendifferent = true; layer = DIFFICULT; 
					break;
				case 2: ipmenue = 2; layer = SETINGS; break;
				}
				break;

			case MAINMENUE:
				switch (global.MAINMENUE.fantasia.num_work + 1) {
				case 1: menueover = 1;//ТУТ ИГРА ВООБЩЕ ЗАКРЫВАЕТСЯ И ПОКАЗЫВАЕТ НАЧАЛЬНУЮ ЗАСТАВКУ и сэйвятся все файлы, кк, в отдельную бухгалтерию
					break;
				case 2: ipmenue = 2; layer = GLOBAL; break;
				}
				break;
			case CHARACTER:
				switch (global.CHARACTER.fantasia.num_work + 1) {
				case 1: //ipmenue = ?; if (!Interfaceio[ipmenue]) mendifferent = true; layer = DIFFICULT; 
					break;
				case 2: ipmenue = 2; layer = SETINGS; break;
				case 3: ipmenue = 2; layer = SETINGS; break;
				}
				break;
			}

			break;
		}

		

		break;
	case sunduk: //////////////////////////
		switch (Winment)
		{
		case upt:
			if (I == 1 or xm > 1)
				if (xm > 0) {
					xm--;
				}
			break;
		case downt:
			if (xm + 1 < sizemdm) {
				xm++;
			}
			break;
		case entert:
			if (xm == 0) {
				jojo = game;
			}
			if (xm == 1) {
				jojo = game;
				ifmap = 0;
				//fileroom(1);
				g = 1;
			}
			I = 1;
			break;

		}
	}

	Winment = stopt;
}
//реакция меню игрового процесса, сундуков и инвентаря

void ImputMenue() {
	char fu;
	if (_kbhit()) {

		switch (fu = _getch())
		{
		case 'a':
			Winment = leftt;
			break;
		case 'w':
			Winment = upt;
			break;
		case 'd':
			Winment = rightt;
			break;
		case 's':
			Winment = downt;
			break;
		}
		if (static_cast<int>(fu) == 13)
		{
			Winment = entert;
		}
		
	}
}

void DravMenue() {
	switch (layer)
	{
	case GLOBAL:
		for (int i = 0; i < global.fantasia.chh; ++i) {
			for (int j = 0; j < global.fantasia.chw; ++j) {
				if (global.fantasia.coordinates[0][global.fantasia.num_work] == j && global.fantasia.coordinates[1][global.fantasia.num_work] == i) {
					cout << global.fantasia.mouse;
				}
				else if (global.fantasia.massivem[j][i] == f) {
					cout << " ";
				}
				else {
					cout << global.fantasia.massivem[j][i];
				}

			}
			cout << endl;
		}
		break;
	case SETINGS:
		for (int i = 0; i < global.SETTINGS.fantasia.chh; ++i) {
			for (int j = 0; j < global.SETTINGS.fantasia.chw; ++j) {
				if (global.SETTINGS.fantasia.coordinates[0][global.SETTINGS.fantasia.num_work] == j && global.SETTINGS.fantasia.coordinates[1][global.SETTINGS.fantasia.num_work] == i) {
					cout << global.SETTINGS.fantasia.mouse;
				}
				else if (global.SETTINGS.fantasia.massivem[j][i] == f) {
					cout << " ";
				}
				else {
					cout << global.SETTINGS.fantasia.massivem[j][i];
				}

			}
			cout << endl;
		}
		break;
	case RAD:
		for (int i = 0; i < global.SETTINGS.fantasia.chh; ++i) {
			for (int j = 0; j < global.SETTINGS.fantasia.chw; ++j) {
				if (global.SETTINGS.fantasia.coordinates[0][global.SETTINGS.fantasia.num_work] == j && global.SETTINGS.fantasia.coordinates[1][global.SETTINGS.fantasia.num_work] == i) {
					cout << global.SETTINGS.fantasia.mouse;
				}
				else if (global.SETTINGS.fantasia.massivem[j][i] == f) {
					cout << " ";
				}
				else {
					cout << global.SETTINGS.fantasia.massivem[j][i];
				}

			}
			cout << endl;
		}
		break;
	case SOUND:
		for (int i = 0; i < global.SETTINGS.SOUND.fantasia.chh; ++i) {
			for (int j = 0; j < global.SETTINGS.SOUND.fantasia.chw; ++j) {
				if (global.SETTINGS.SOUND.fantasia.coordinates[0][global.SETTINGS.SOUND.fantasia.num_work] == j && global.SETTINGS.SOUND.fantasia.coordinates[1][global.SETTINGS.SOUND.fantasia.num_work] == i) {
					cout << global.SETTINGS.SOUND.fantasia.mouse;
				}
				else if (global.SETTINGS.SOUND.fantasia.massivem[j][i] == f) {
					cout << " ";
				}
				else {
					cout << global.SETTINGS.SOUND.fantasia.massivem[j][i];
				}

			}
			cout << endl;
		}
		break;
	case DIFFICULT:
		for (int i = 0; i < global.SETTINGS.DIFFICULT.fantasia.chh; ++i) {
			for (int j = 0; j < global.SETTINGS.DIFFICULT.fantasia.chw; ++j) {
				if (global.SETTINGS.DIFFICULT.fantasia.coordinates[0][global.SETTINGS.DIFFICULT.fantasia.num_work] == j && global.SETTINGS.DIFFICULT.fantasia.coordinates[1][global.SETTINGS.DIFFICULT.fantasia.num_work] == i) {
					cout << global.SETTINGS.DIFFICULT.fantasia.mouse;
				}
				else if (global.SETTINGS.DIFFICULT.fantasia.massivem[j][i] == f) {
					cout << " ";
				}
				else {
					cout << global.SETTINGS.DIFFICULT.fantasia.massivem[j][i];
				}

			}
			cout << endl;
		}
		break;
	case CHARACTER:
		for (int i = 0; i < global.CHARACTER.fantasia.chh; ++i) {
			for (int j = 0; j < global.CHARACTER.fantasia.chw; ++j) {
				if (global.CHARACTER.fantasia.coordinates[0][global.CHARACTER.fantasia.num_work] == j && global.CHARACTER.fantasia.coordinates[1][global.CHARACTER.fantasia.num_work] == i) {
					cout << global.CHARACTER.fantasia.mouse;
				}
				else if (global.CHARACTER.fantasia.massivem[j][i] == f) {
					cout << " ";
				}
				else {
					cout << global.CHARACTER.fantasia.massivem[j][i];
				}

			}
			cout << endl;
		}
		break;
	case MAINMENUE:
		for (int i = 0; i < global.MAINMENUE.fantasia.chh; ++i) {
			for (int j = 0; j < global.MAINMENUE.fantasia.chw; ++j) {
				if (global.MAINMENUE.fantasia.coordinates[0][global.MAINMENUE.fantasia.num_work] == j && global.MAINMENUE.fantasia.coordinates[1][global.MAINMENUE.fantasia.num_work] == i) {
					cout << global.MAINMENUE.fantasia.mouse;
				}
				else if (global.MAINMENUE.fantasia.massivem[j][i] == f) {
					cout << " ";
				}
				else {
					cout << global.MAINMENUE.fantasia.massivem[j][i];
				}

			}
			cout << endl;
		}
		break;
	}
	
}

void lolkaMENUE() {
	jojos = meny;
	layer = GLOBAL;
	
	
	/////////////////
	
	mendifferent = true;
	Menueroom(ipmenue = 1, layer);
	menueover = 0;
	Interfaceio[0] = true;

	while (!menueover) {
		system("cls");
		DravMenue();
		while (Interfaceio[0])
		{
			if (_kbhit())
			{
				ImputMenue();
				Interfaceio[0] = false;

			}
		}
		Interfaceio[0] = true;
		MoveWin();
		if (mendifferent) {
			Menueroom(ipmenue, layer);
		}
	}

}

void gogplus(int Q) {

	//Name
	switch (sushestvo[Q].Name) {
	case Jon: cout << "Jon";
		break;
	case Ron: cout << "Ron";
		break;
	case Bob: cout << "Bob";
		break;
	case Sasha: cout << "Sasha";
		break;
	}
	//Number
	cout << endl << sushestvo[Q].number << endl;
	//Gender
	if (sushestvo[Q].gender == true) {
		cout << "Men" << endl;
	}
	else if (sushestvo[Q].gender == false) {
		cout << "Women" << endl;
	}
	//Race
	switch (sushestvo[Q].race) {
	case HUMAN: cout << "HUMAN";
		break;
	case TROLL: cout << "TROLL";
		break;
	case BOSS: cout << "BOSS";
		break;
	}
	cout << endl;
	//Class
	switch (sushestvo[Q]._class)
	{
	case soldier: cout << "soldier";
		break;
	case wiz: cout << "wiz";
		break;
	case wizard: cout << "wizard";
		break;
	case king: cout << "king";
		break;
	case npc: cout << "npc";
		break;
	case guard: cout << "guard";
		break;
	}
	cout << endl;

	//Weapon
	for (int i = 0; i < sushestvo[Q].num_weapon; ++i) {
		switch (sushestvo[Q].armor[i].weapon) {
		case 0: cout << "sword"; break;
		case 1: cout << "bow"; break;
		case 2: cout << "arrows"; break;
		case 3: cout << "conjuration"; break;
		}
	}
	cout << endl;

	//Bronya
	for (int i = 0; i < sushestvo[Q].num_bronya; ++i) {
		switch (sushestvo[Q].arms[i].bronya) {
		case 0: cout << "vest"; break;
		case 1: cout << "helmet"; break;
		case 2: cout << "boots"; break;
		}
	}
	cout << endl;

}


void timming() { //времянка, хотя потом ее можно использовать, как обновление оснастки в игровом роцессе
	USER.armor = new ARMOR[1];
	USER.armor[0].weapon = sword;
	USER.num_gen = 0;
	//USER.arms = new ARMS[1];

}

void interaction() { // мое взаимодействие с мобом
	switch (USER.armor[USER.num_gen].weapon) {
	case armor: USER.rad_attack = ARMSR; USER.armor[USER.num_gen].damag = ARMSD; break;
	case sword: USER.rad_attack = SWORD; USER.armor[USER.num_gen].damag = SWORDD; break;
	case bow: USER.rad_attack = BOW; USER.armor[USER.num_gen].damag = BOWD; break;
	case conjuration: USER.rad_attack = CONJURATION; USER.armor[USER.num_gen].damag = CONJURATIOND; break;
	}


	switch (interactions) {
	case hit: for (int q = 0; q < num_mobs; ++q) {
		if (x - USER.rad_attack - 1 < sushestvo[q].x_mob && sushestvo[q].x_mob < x + USER.rad_attack + 1 && y - USER.rad_attack - 1 < sushestvo[q].y_mob && sushestvo[q].y_mob < y + USER.rad_attack + 1) {
			//sushestvo[q].mode == active
			
			sushestvo[q].health = sushestvo[q].health - USER._ap;
			if (sushestvo[q].health < 0) {
				sushestvo[q].health = 0;
			}
			for (int q = 0; q < num_mobs; ++q) {
				if (sushestvo[q].health == 0) {
					//delete[] sushestvo[q];
					sushestvo[q].leaving = false;
					if (!sushestvo[q].leaving && dead[q]) {
						USER._expirience = USER._expirience + sushestvo[q]._expiriencegiven;
						Userchange();
						dead[q] = false;
					}

				}
			}
			
		}
	} break;
	case take:;
	}


	interactions = stoped;
	if (USER.health <= 0) gameover = true;
}

void setup()
{
	char h = 0;
	cout << "Вы находитесь в редакторе персонажа." << endl << "Нажмите <t> для продолжения." << endl;
	while (h != 't') {
		if (_kbhit())
			h = _getch();
	}
	h = 'i';
	cout << "Введите ваше имя: ";
	getline(cin, USER.Name); //Name
	cout << "Привет, " << USER.Name << "." << endl;

	char time_sex{ 'f' }; //Gender
	while (time_sex != '}')
	{
		cout << "введите пол (m - мужчина, w - женщина): ";
		cin >> time_sex;
		if (time_sex == 'm') {
			USER.gender = true;
			time_sex = '}';

		}
		else if (time_sex == 'w') {
			USER.gender = false;
			time_sex = '}';
		}
	}


	/////////////////////
	cout << "Нажмите <t> для продолжения." << endl;
	while (h != 't') {
		if (_kbhit())
			h = _getch();
	}
	gameover = 1;
}
// создание гп

void fileroom(int v_2) {
	if (!ifmap) {
		switch (v_2)//инициализация адреса карты
		{
		case 1:
			room = "D:\\Users\\samsonp\\Documents\\GitHub\\arazu.mai.cpp\\arazu.vs\\arazu.vs\\1\\map1.txt";
			break;
		case 2:
			room = "D:\\Users\\samsonp\\Documents\\GitHub\\arazu.mai.cpp\\arazu.vs\\arazu.vs\\1\\map2.txt";
			break;
		case 3:
			room = "D:\\Users\\samsonp\\Documents\\GitHub\\arazu.mai.cpp\\arazu.vs\\arazu.vs\\1\\end.txt";
			break;
		case 4:
			room = "D:\\Users\\samsonp\\Documents\\GitHub\\arazu.mai.cpp\\arazu.vs\\arazu.vs\\1\\menue1.txt";
			break;
		case 5:
			room = "D:\\Users\\samsonp\\Documents\\GitHub\\arazu.mai.cpp\\arazu.vs\\arazu.vs\\1\\map3.txt";
			break;
		case 6:
			room = "D:\\Users\\samsonp\\Documents\\GitHub\\arazu.mai.cpp\\arazu.vs\\arazu.vs\\1\\map4.txt";
			break;
		case 7:
			room = "D:\\Users\\samsonp\\Documents\\GitHub\\arazu.mai.cpp\\arazu.vs\\arazu.vs\\1\\map5.txt";
			break;
		case 8:
			room = "D:\\Users\\samsonp\\Documents\\GitHub\\arazu.mai.cpp\\arazu.vs\\arazu.vs\\1\\mob1.txt";
			//room = "d:\\1\\Loading.txt";
			break;
		case 9:
			room = "D:\\Users\\samsonp\\Documents\\GitHub\\arazu.mai.cpp\\arazu.vs\\arazu.vs\\1\\LABIRINTE.txt";
			break;
		case 10: 
			room = "D:\\Users\\samsonp\\Documents\\GitHub\\arazu.mai.cpp\\arazu.vs\\arazu.vs\\1\\lastmainmap!.txt";
			break;
		}
		ifmap = 1;
	}

	//if (file) { cout << 404; } // нужно проверять, открылся ли файл
	//file.open(room, ifstream::app);
	ifstream file(room);
	switch (jojo) {
	case menue:
		file >> hdm >> wdm;
		file >> sizemdm; // size menue description massiv))
		menue_description[0] = new int[sizemdm];
		menue_description[1] = new int[sizemdm];

		for (int i = 0; i < sizemdm; ++i)
		{
			for (int j = 0; j < 2; ++j) {
				file >> menue_description[j][i];
			}
		}
		xm = 1;
		for (int i = 0; i <= hdm; ++i) {
			for (int j = 0; j <= wdm; ++j) {
				file >> masmenue[i][j];
			}
		}
		break;
	case game:
		file >> hd >> wd >> x >> y;
		for (int i = 0; i <= hd; ++i) {
			for (int j = 0; j <= wd; ++j) {
				file >> mas[i][j];
			}
		}
		file >> num_mobs;
		dead = new bool [num_mobs];
		for (int y = 0; y < num_mobs; ++y) {
			dead[y] = true;
		}
		nmm = new int[num_mobs];
		for (int q = 0; q < num_mobs; ++q) {
			nmm[q] = 0;
		}
		sushestvo = new Character[num_mobs];
		for (int Q = 0; Q < num_mobs; ++Q) {
			//gog(i);
			////////////////////
			int you;
			file >> you;//Name
			switch (you) {
			case 0: sushestvo[Q].Name = Jon;
				break;
			case 1: sushestvo[Q].Name = Ron;
				break;
			case 2: sushestvo[Q].Name = Bob;
				break;
			case 3: sushestvo[Q].Name = Sasha;
				break;
			}

			counter++;
			sushestvo[Q].number = counter;

			//Gender
			file >> you;
			if (you == 1) {
				sushestvo[Q].gender = true;
			}
			else if (you == 0) {
				sushestvo[Q].gender = false;
			}

			file >> you; //Race
			switch (you) {
			case 0: sushestvo[Q].race = HUMAN;
				break;
			case 1: sushestvo[Q].race = TROLL;
				break;
			case 2: sushestvo[Q].race = BOSS;
				break;
			case 3: //sushestvo[Q].race = DWARF;
				break;
			}

			file >> you; //Class
			switch (you) {
			case 0: sushestvo[Q]._class = soldier;
				break;
			case 1: sushestvo[Q]._class = wiz;
				break;
			case 2: sushestvo[Q]._class = wizard;
				break;
			case 3: sushestvo[Q]._class = king;
				break;
			case 4: sushestvo[Q]._class = npc;
				break;
			case 5: sushestvo[Q]._class = guard;
				break;
			}
			//soldier = 0, wiz, wizard, king, npc, guard

			file >> sushestvo[Q].num_weapon; //количество оружия
			sushestvo[Q].armor = new ARMOR[sushestvo[Q].num_weapon];
			for (int i = 0; i < sushestvo[Q].num_weapon; ++i) {//Weapon
				file >> you;
				switch (you) {
				case 0: sushestvo[Q].armor[i].weapon = sword; break;
				case 1: sushestvo[Q].armor[i].weapon = bow; break;
				case 2: sushestvo[Q].armor[i].weapon = arrows; break;
				case 3: sushestvo[Q].armor[i].weapon = conjuration; break;
				}
			}

			file >> sushestvo[Q].num_bronya; //количество брони
			sushestvo[Q].arms = new ARMS[sushestvo[Q].num_bronya];
			for (int i = 0; i < sushestvo[Q].num_bronya; ++i) {//Bronya
				file >> you;
				switch (you) {
				case 0: sushestvo[Q].arms[i].bronya = vest; break;
				case 1: sushestvo[Q].arms[i].bronya = helmet; break;
				case 2: sushestvo[Q].arms[i].bronya = boots; break;
				}
			}

			//Agres
			file >> you;
			switch (you) {
			case 0: sushestvo[Q].agres = enemy; break;
			case 1: sushestvo[Q].agres = potential_enemy; break;
			case 2: sushestvo[Q].agres = coward; break;
			case 3: sushestvo[Q].agres = outlaw; break;
			}

			file >> sushestvo[Q].sizepath;//размер траектории
			sushestvo[Q].path = new ch_path[sushestvo[Q].sizepath];

			for (int q = 0; q < sushestvo[Q].sizepath; ++q) { //считывание траектории
				file >> you;
				switch (you) {
				case 0: sushestvo[Q].path[q] = vv; break;
				case 1: sushestvo[Q].path[q] = vn; break;
				case 2: sushestvo[Q].path[q] = vl; break;
				case 3: sushestvo[Q].path[q] = vp; break;
				}

			}

			//ввод номера атакующего оружия
			file >> sushestvo[Q].num_gen;



			//радиус, которого моб будет держаться от нас, чтобы атаковать. он зависит от вида оружия. дамаг и хп оружия
			switch (sushestvo[Q].armor[sushestvo[Q].num_gen].weapon) {
			case sword: sushestvo[Q].rad_attack = SWORD; sushestvo[Q].armor[sushestvo[Q].num_gen].damag = SWORDD;  break;
			case bow: sushestvo[Q].rad_attack = BOW; sushestvo[Q].armor[sushestvo[Q].num_gen].damag = BOWD; break;
			case conjuration: sushestvo[Q].rad_attack = CONJURATION; sushestvo[Q].armor[sushestvo[Q].num_gen].damag = CONJURATIOND; break;
			}


			//Health
			sushestvo[Q].health = 3;

			//Skin
			switch (sushestvo[Q]._class) {
			case soldier: sushestvo[Q].skin = 'd'; break;
			case wiz: sushestvo[Q].skin = 'V'; break;
			case wizard: sushestvo[Q].skin = 'A'; break;
			case king: sushestvo[Q].skin = 'W'; break;
			case npc: sushestvo[Q].skin = 'o'; break;
			case guard: sushestvo[Q].skin = 'U'; break;
			}

			file >> sushestvo[Q].x_mob;
			file >> sushestvo[Q].y_mob;
			////////////////////
		}

		break;
	case ima:
		file >> hi >> wi;
		for (int i = 0; i <= hi; ++i) {
			for (int j = 0; j <= wi; ++j) {
				file >> mas_imagine[i][j];
			}
		}
		break;
	}
	file.close();
}
// работа с файлами, инициализация карт и объектов

bool way(int zx, int zy) {
	if (mas[zy][zx] != f) {
		return false;
	}
}
// проверка на то, занята ли клетка

void WatchMenue()
{
	if (!ifmap) { //переименовать ifmap в iffile
		fileroom(4);//выгрузка изображения меню в специализированный массив masmenue[][]
	}

	for (int i = 0; i <= hdm; ++i)
	{
		for (int j = 0; j <= wdm; ++j)
		{
			if ((menue_description[0][xm] == j) && (menue_description[1][xm] == i))
			{
				cout << mouse;
			}
			else if (masmenue[i][j] == f)
			{
				cout << " ";
			}
			else
			{
				cout << masmenue[i][j];
			}
		}
		cout << endl;
	}
}

void map(int v)
{
	if (!ifmap) { // если карта поменялась, значит нужно записать новую
		/*for (int i = 0; i < 99; ++i)    // удалление массива
		delete[] mas[i];
	delete[] mas;*/
		fileroom(v);//... чтение карты
	}

	//... дальше здесь отрисовка этой карты

	
	cout << "Expirience:" << USER._expirience << "/" << USER._expiriencecap << "Health:" << USER.health << "/" << USER._maxhealth << " Enemy Health:" << sushestvo[0].health << " Level:" << USER._level << endl;
	cout << "Mob cordinates:" << way(sushestvo[0].x_mob, sushestvo[0].y_mob) << endl;
	for (int i = 0; i <= hd; ++i)
	{
		//cout << "                               ";
		if ((y - rad) < i && i < (y + rad)) {
			
			for (int j = 0; j <= wd; ++j)
			{
				int left = -1, right = -1;
				char timmi;
				for (int u = 0; u < num_mobs; ++u) {
					if (sushestvo[u].leaving && sushestvo[u].x_mob == j && sushestvo[u].y_mob == i) {
						left = i;
						right = j;
						timmi = sushestvo[u].skin;
					}
				}
				if ((x - rad) < j && j < (x + rad)) {
					if (i == y && j == x)
					{
						cout << USER.skin;
					}
					else if (i == left && j == right)
					{
						cout << timmi;
					}
					else if (mas[i][j] == f)
					{
						cout << " ";
					}
					else {
						cout << mas[i][j];
					}
				}
			}
			cout << endl;
		}
	}

	//cout << sushestvo[0].x_mob << " " << sushestvo[0].y_mob;

	/*for (int i = 0; i < h; ++i)    // удалление массива
		delete[] mas[i];
	delete[] mas;*/
}
// отрисовка карт

void Input() {
	char fu;
	if (_kbhit()) {

		switch (fu = _getch())
		{
		case 'a':
			movement = LEVT;
			break;
		case 'w':
			movement = UP;
			break;
		case 'd':
			movement = RIGHT;
			break;
		case 's':
			movement = DOWN;
			break;
			/*case 'q':
				if (jojo == menue) {
					jojo = game;
					ifmap = 0;
				}
				break;*/
		case 'x':
			gameover = 1;
			break;
		case 'j':
			system("cls");
			ifmap = 0;
			cin >> g;
			break;
		case 'f':
			jojo = menue;
			break;
		case 'y':
			jojo = pause;
			break;
		case 'q': interactions = hit; break;
		case 'e': interactions = take; break;
		}
		if (static_cast<int>(fu) == 13)
		{
			movement = ENTER;
		}
		goy = 1;
	}
}
// ввод, реакция на нажатие

void MoveLogic() {
	switch (jojo) {
	case game:
		switch (movement)
		{
		case LEVT:
			if (x > 1 && mas[y][x - 1] == f && way(x - 1, y)) {
				x = x - 1;
			}
			break;


		case UP:
			if (y > 1 && mas[y - 1][x] == f && way(x, y - 1)) {
				y = y - 1;
			}
			break;


		case RIGHT:
			if (x < wd && mas[y][x + 1] == f && way(x + 1, y)) {
				x = x + 1;
			}
			break;


		case DOWN:
			if (y < hd - 1 && mas[y + 1][x] == f && way(x, y + 1)) {
				y = y + 1;
			}
			break;
		}
	case menue:
		switch (movement)
		{
		case UP:
			if (I == 1 or xm > 1)
				if (xm > 0) {
					xm--;
				}
			break;
		case DOWN:
			if (xm + 1 < sizemdm) {
				xm++;
			}
			break;
		case ENTER:
			if (xm == 0) {
				jojo = game;
			}
			if (xm == 1) {
				jojo = game;
				ifmap = 0;
				fileroom(1);
				g = 1;
			}
			I = 1;
			break;

		}
	}

	movement = STOP;
}
// логика перемещения

void settings()
{
	//symbols();
	jojo = menue;
	ifmap = 0;
	xm = 0;
	timming();
}

void imagine(int number_ima) {
	if (!ifmap) {
		fileroom(number_ima);
	}

	for (int i = 0; i <= hi; ++i)
	{

		for (int j = 0; j <= wi; ++j)
		{

			if (mas_imagine[i][j] == f)
			{
				cout << " ";
			}
			else {
				cout << mas_imagine[i][j];
			}

		}
		cout << endl;

	}


}
//нужна функция для беспонтовой отрисовки чего-либо в консоль... хмхм

void GO()
{
	system("cls");
	ifmap = 0;
	jojo = ima;
	imagine(3);
	//cout << "you lose! =3"; // написать прикольную заставку
}
// конец игры

void MoveMob() {
	for (int q = 0; q < num_mobs; ++q)
	{
		if (sushestvo[q].leaving) {
			if (sushestvo[q].x_mob - sushestvo[q].rad_mob < x && x < sushestvo[q].x_mob + sushestvo[q].rad_mob) {
				if (sushestvo[q].y_mob - sushestvo[q].rad_mob < y && y < sushestvo[q].y_mob + sushestvo[q].rad_mob) {
					switch (sushestvo[q].agres) {
					case enemy: sushestvo[q].mode = active;
					case outlaw: sushestvo[q].mode = active;
					}
				}
			}
			else {
				sushestvo[q].mode = passive;
			}

			//////////////
			bool poor = false;
			if (sushestvo[q].mode == passive) { // если моб пассивен, то он движется по заданной траектории
				if (sushestvo[q].jorgan) {
					switch (sushestvo[q].path[nmm[q]]) {
					case vv: if (way(sushestvo[q].x_mob, sushestvo[q].y_mob - 1)) { sushestvo[q].y_mob--; poor = false; }
							 else { poor = true; }; break;
					case vn: if (way(sushestvo[q].x_mob, sushestvo[q].y_mob + 1)) { sushestvo[q].y_mob++; poor = false; }
							 else { poor = true; }; break;
					case vl: if (way(sushestvo[q].x_mob - 1, sushestvo[q].y_mob)) { sushestvo[q].x_mob--; poor = false; }
							 else { poor = true; }; break;
					case vp: if (way(sushestvo[q].x_mob + 1, sushestvo[q].y_mob)) { sushestvo[q].x_mob++; poor = false; }
							 else { poor = true; }; break;
					}
					if (!poor) { nmm[q]++; }
				}
				else {
					switch (sushestvo[q].path[nmm[q]]) {
					case vv: if (way(sushestvo[q].x_mob, sushestvo[q].y_mob - 1)) { sushestvo[q].y_mob--; poor = false; }
							 else { poor = true; }; break;
					case vn: if (way(sushestvo[q].x_mob, sushestvo[q].y_mob + 1)) { sushestvo[q].y_mob++; poor = false; }
							 else { poor = true; }; break;
					case vl: if (way(sushestvo[q].x_mob - 1, sushestvo[q].y_mob)) { sushestvo[q].x_mob--; poor = false; }
							 else { poor = true; }; break;
					case vp: if (way(sushestvo[q].x_mob + 1, sushestvo[q].y_mob)) { sushestvo[q].x_mob++; poor = false; }
							 else { poor = true; }; break;
					}
					if (!poor) { nmm[q]--; }
				}


				if (nmm[q] == sushestvo[q].sizepath, nmm[q] == 0) {
					sushestvo[q].jorgan = !sushestvo[q].jorgan;
				}
			}


			if (sushestvo[q].mode == active) {

				if (sushestvo[q].x_mob > x + sushestvo[q].rad_attack && way(sushestvo[q].x_mob - 1, sushestvo[q].y_mob)) {
					sushestvo[q].x_mob--;
				}
				if (sushestvo[q].x_mob < x - sushestvo[q].rad_attack && way(sushestvo[q].x_mob + 1, sushestvo[q].y_mob)) {
					sushestvo[q].x_mob++;
				}
				if (sushestvo[q].y_mob > y + sushestvo[q].rad_attack && way(sushestvo[q].x_mob, sushestvo[q].y_mob - 1)) {
					sushestvo[q].y_mob--;
				}
				if (sushestvo[q].y_mob < y - sushestvo[q].rad_attack && way(sushestvo[q].x_mob, sushestvo[q].y_mob + 1)) {
					sushestvo[q].y_mob++;
				}
			}

			if (sushestvo[q].x_mob - sushestvo[q].rad_attack - 1 < x && x < sushestvo[q].x_mob + sushestvo[q].rad_attack + 1) {
				if (sushestvo[q].y_mob - sushestvo[q].rad_attack - 1 < y && y < sushestvo[q].y_mob + sushestvo[q].rad_attack + 1) {
					USER.health = USER.health - sushestvo[q].armor[sushestvo[q].num_gen].damag;
				}
			}
		}
	}
	//jojo = game;
	//ifmap = 0;
	//fileroom(8);


}


int main()
{
	setlocale(LC_ALL, "Russian");
	
	settings();
	goy = 0;
	
	while (jojo == menue) {
		system("cls");
		WatchMenue();
		while (!goy)
		{
			Input();
		}
		MoveLogic();
		goy = 0;
	}

	jojo = game;
	g = 1;

	while (!gameover)
	{
		system("cls");
		switch (jojo) {
		case pause:
			map(g);
			while (!goy)
			{
				if (_kbhit())
				{
					goy = 1;
					if (_getch() == 'y')
					{
						jojo = game;
					}
				}
			}
			goy = 0;
			break;
		case menue:
			
			lolkaMENUE();
			jojo = game;
			break;
		case game:
			map(g);

			while (!goy)
			{
				Input();
			}
			MoveLogic();
			MoveMob();
			interaction();
			if (USER.health == 0) {
				gameover = 1;
			}
			goy = 0;
			break;
		}



	}

	GO();

}