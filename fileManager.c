#include "header.h"
//		2		1			0
//xxxx yyyy  yxxx xyyy  yyyy 0000

 List* readFromBinFileToApt(char* fileName) {// reading the binery data from the file.bin
	 FILE* file = fopen(fileName, "rb");
	 unsigned char** bitData = malloc(5);
	 Date insertDate, leavingDate;
	 List* apt_list = malloc(sizeof(List));
	 *apt_list = makeEmptyList();
	 char* address;
	 short int id, addressLen;
	 long int sizeOfFile;
	 int i,price, numOfRooms;
	 if (checkFile(file)) {
		 fseek(file, 0, SEEK_END);
		 sizeOfFile = ftell(file);
		 fseek(file, 0, SEEK_SET);
		 while (ftell(file) < sizeOfFile) {
			 fread(&id, sizeof(short int), 1, file);
			 fread(&addressLen, sizeof(short int), 1, file);
			 address = malloc((sizeof(char)*addressLen) + 1);
			 fread(address, sizeof(char), addressLen, file);
			 address[addressLen] = '\0';
			 fread(&price, sizeof(int), 1, file);
			 for (i = 0; i < 5; i++) {// collecting the 5 byte array
				 bitData[i] = calloc(sizeof(unsigned char),1);
				 fread(bitData[i], sizeof(unsigned char), 1, file);
			 }
			 bitToData(bitData, &leavingDate, &insertDate, &numOfRooms);// converting the bits to user readable data
			 insertNodeToTail(apt_list, reCreateNode(address, price, numOfRooms, leavingDate, insertDate, id));//inserting the data into the node and the to the Linked list
			 free(address);// freeing th addres that was read from the binery folder
		 }
		fclose(file);
		if(apt_list->tail)
			APT_ID = apt_list->tail->uniqueId;
	 }
	 else {
		 APT_ID = 0;
	 }

	 return apt_list;
}
 
 void bitToData(unsigned char** bitData, Date* leavingDate, Date* insertDate, short int* numOfRooms) {// converting the bits to user readable data
	 *numOfRooms = (short int)(((*(bitData[0]) & ROOM_MASK))>>4);
	 leavingDate->dayOfTheMonth = (short int)(((*(bitData[0]) & DAY_OF_THE_YEAR_MASK1)<<1)| ((*(bitData[1]) & DAY_OF_THE_YEAR_MASK2)>>7));
	 leavingDate->month = (short int)(((*(bitData[1]) & MOUNTH_MASK1) >>3) | ((*(bitData[1]) & MOUNTH_MASK2)>>3));
	 leavingDate->year = ((short int)(((*(bitData[1]) & YEAR_MASK1) << 4) | (*(bitData[2]) & YEAR_MASK2)>>4)) + 2000;



	 insertDate->dayOfTheMonth = (short int)(((*(bitData[3]) & INSERT_DAY_OF_THE_YEAR_MASK1) >>3) | ((*(bitData[3]) & INSERT_DAY_OF_THE_YEAR_MASK2) >> 3));
	 insertDate->month = (short int)(((*(bitData[3]) & INSERT_MOUNTH_MASK1) <<1)| ((*(bitData[4]) & INSERT_MOUNTH_MASK2) >> 7));
	 insertDate->year = ((short int)(((*(bitData[4]) & INSERT_YEAR_MASK1) ) | (*(bitData[4]) & INSERT_YEAR_MASK2)))+2000;
 }

 /*
//		1		0
// yyyy yxxx  xyyy yyyy 
#define INSERT_DAY_OF_THE_YEAR_MASK1 0xF0
#define INSERT_DAY_OF_THE_YEAR_MASK2 0x08
#define INSERT_MOUNTH_MASK1 0x07
#define INSERT_MOUNTH_MASK2 0x80
#define INSERT_YEAR_MASK1 0x70
#define INSERT_YEAR_MASK2 0x0F
 */

 void writeToBinFileFromApt(char* fileName, List* apt_list) {// writing the data to binery file and disasembaling it to bits
	 FILE* file = fopen(fileName, "wb");
	 int addressLen;
	 Apartment* apt;
	 unsigned char* bitData = calloc(sizeof(unsigned char),5);
	 BYTE* bitsArr;
	 bitsArr = (BYTE*)calloc(sizeof(BYTE), 5);
	 for (apt = apt_list->head; apt != NULL; apt = apt->next) {
		 addressLen = strlen(apt->address);
		 
		 fwrite(&(apt->uniqueId), sizeof(short int), 1, file);
		 fwrite(&addressLen  , sizeof(short int), 1, file);
		 fwrite(apt->address , sizeof(char)		, addressLen, file);
		 fwrite(&(apt->price)   , sizeof(int)		, 1, file);
		 bitsArr[0] = bitsArr[0] | (((unsigned char)(apt->numOfRooms) << 4)	|(((unsigned char)(apt->leavingDate.dayOfTheMonth) >> 1)));
		 bitsArr[1] = bitsArr[1] | (((unsigned char)(apt->leavingDate.dayOfTheMonth) << 7)|(((unsigned char)(apt->leavingDate.month) << 3))|(((unsigned char)((apt->leavingDate.year) - 2000) >> 4)));
		 bitsArr[2] = bitsArr[2] | (((unsigned char)((apt->leavingDate.year)-2000) << 4));
		 bitsArr[3] = bitsArr[3] | (((unsigned char)(apt->insertDate.dayOfTheMonth) << 3)|((unsigned char)(apt->insertDate.month) >> 1));
		 bitsArr[4] = bitsArr[4] | (((unsigned char)(apt->insertDate.month) << 7)|((unsigned char)(apt->insertDate.year) - 2000));
		 fwrite(bitsArr, sizeof(BYTE), 5, file);
		 bitsArr[0] = bitsArr[1] = bitsArr[2] = bitsArr[3] = bitsArr[4] = NULL;
		 
	 }
	 fclose(file);// closing the file
	 //freeing the apartment list
	 freeAptInList(*apt_list);
	 free(bitsArr);
	 free(apt);
	 free(apt_list);
 }
 /*
 //		0		 1		     2			3		   4
 //xxxx yyyy  yxxx xyyy  yyyy 0000  yyyy yxxx  xyyy yyyy
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
 */

