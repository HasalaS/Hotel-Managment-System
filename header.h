#ifndef HEAD   /* Include guard */
#define HEAD
#define _CRT_SECURE_NO_WARNINGS
#define SWAP(type,a,b) {type temp; temp = a;a=b;b=temp;}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#define BYTE unsigned char
#define NUM_OF_BIT_ELEMENTS 7
#define TOKEN " "
#define EXIT "exit"
#define GET_APT "get-an-apt"
#define BUY_APT "buy-an-apt"
#define ADD_APT "add-an-apt"
#define DELETE_APT "delete-an-apt"
#define HIGH_TO_LOW 1//"-sr"
#define LOW_TO_HIGH 0//"-s"
#define DONT_SORT -1
#define N 7
#define SORT_DEF 0
#define SORT_PRICE 1
#define SORT_NUM_Of_ROOMS 2
#define SORT_DATE 3
#define PRINT_LONG_HISTORY "history"
#define PRINT_SHORT_HISTORY "short_history"
#define DAY_IN_SEC 86400
#define ROOM_MASK 0xF0
#define DAY_OF_THE_YEAR_MASK1 0x0F
#define DAY_OF_THE_YEAR_MASK2 0x80
#define MOUNTH_MASK1 0x70
#define MOUNTH_MASK2 0x08
#define YEAR_MASK1 0x07
#define YEAR_MASK2 0xF0
#define INSERT_DAY_OF_THE_YEAR_MASK1 0xF0
#define INSERT_DAY_OF_THE_YEAR_MASK2 0x08
#define INSERT_MOUNTH_MASK1 0x07
#define INSERT_MOUNTH_MASK2 0x80
#define INSERT_YEAR_MASK1 0x70
#define INSERT_YEAR_MASK2 0x0F
char *short_term_history[N];

static short int APT_ID;// gets updated with the reading of  the file
typedef struct Date {
	short int year;
	short int month;
	short int dayOfTheMonth;
}Date;
typedef struct Apartment {
	short int uniqueId;
	char* address;
	int price;
	short int numOfRooms;
	Date leavingDate;
	Date insertDate;
	struct Apartment* next;
	struct Apartment* prev;
}Apartment; 

typedef struct list {
	Apartment* head;
	Apartment* tail;
}List;
typedef struct getCommand {
	char* command;
	int value;
}GetCommand;

typedef struct commandHist {
	char* command;
	short int id;
	struct commandHist* next;
	struct commandHist* prev;
}CommandHist;
typedef struct commandHistList {
	CommandHist* head;
	CommandHist* tail;
}CommandHistList;
//apt_command.c declareations
void get_an_apt(GetCommand** com, int command_list_size, List* aptList, int* sortType);
void add_an_apt(List* apt_list, Apartment apt);
void delete_an_apt(List * apt_list, int days);
void buy_an_apt(List* apt_list, short int ID);
void command_MaximumPrice(List apt_list, int price, List * filterList);
void command_MinimumNumRooms(List apt_list, int minRooms, List * filterList);
void command_MaximumNumRooms(List apt_list, int maxRooms, List * filterList);
void command_tillDate(List apt_list, Date _date, List * filterList);
Date int_to_date(int number);
List makeEmptyList();
int isEmptyList(List lst);
Apartment* createNode(char* address, int price, short int numOfRooms, Date leavingDate);
Apartment* reCreateNode(char* address, int price, short int numOfRooms, Date leavingDate, Date insertDate, short int ID);
void deleteNode(List* lst, Apartment* apt);
void insertNodeToTail(List* lst, Apartment* newTail);
void freeAptInList(List lst);
void freeNode(Apartment* apt);
Apartment parseAddApt(char* cmd);
GetCommand ** parseGetCommand(char * cmd, int * lsize, int * sortType);
List* readFromBinFileToApt(char * fileName);
void bitToData(unsigned char ** bitData, Date * leavingDate, Date * insertDate, short int * numOfRooms);
void writeToBinFileFromApt(char * fileName, List * apt_list);
int convert(int dec);


// sorting decleration
//Apartment *merge(Apartment *L, Apartment *R, int whatToSort, int sortType);
void sort(List * apt_list, int whatToSort, int sortType);
//Apartment *mergeSort(Apartment *head, int whatToSort, int sortType);
void printApt(Apartment apt);
time_t getEpochTime();
Date curentDate();
Date getDate(time_t time);
void printList(List lst);
int isEqual(char * str1, char * str2);
void copyApt(Apartment * dst, Apartment * src);
void sortList(List * apt_list, int whatToSort, int sortOrder);
int merge(int arr[], int l, int m, int h, int whatToSort, int sortOrder);
void mergeSort(int * arr, int start, int end, int whatToSort, int sortOrder);
void swapNodes(Apartment * L, Apartment * R);
void copyAptList(List sorce, List * dest);
Apartment ** listNodeToArray(List lst, int * arrSize);
void printArr(Apartment ** arr, int size);
Date getDateXDays(int x);
//Ran
//File managment declerations
CommandHistList* read_cmd_from_long_file(char* filename);
CommandHistList makeEmptyHistoryList();
CommandHist* createHistNode(char* cmd, short int id);
//CommandHist* deleteNode(CommandHistList* lst, CommandHist* cmd);
int isEmptyHistList(CommandHistList lst);
int checkFile(FILE* fl);
int charcount(FILE *const fin);
char* get_last_command(CommandHistList* historyList);
char* change_history(short int id, char* source, char* replacment, CommandHistList* list);
void commandManagement(List * lst, CommandHistList * historyLst);
void create_cmd(List* lst, char* cmd);
void insertHistNodeToTail(CommandHistList* lst, CommandHist* newTail);
void insertHistNodeToHead(CommandHistList * lst, CommandHist * newTail);
void print_long_history(CommandHistList * list);
void transfer_to_long_history(char* cmd, CommandHistList *list);
void print_short_history(CommandHistList *list);
void parse_recall_string(CommandHistList* list, char* cmd);
void write_cmd_to_short(char* commandString, CommandHistList* cmdHistList);
void write_cmd_to_long_file(CommandHistList* list, char* filename);
void freeCmdInList(CommandHistList lst);


#endif // HEAD