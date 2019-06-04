#define _CRT_SECURE_NO_WARNINGS
#include "header.h"
/*
#define SORT_DEF 0
#define SORT_PRICE 1
#define SORT_NUM_Of_ROOMS 2
#define SORT_DATE 3

#define HIGH_TO_LOW 0 //"-sr"
#define LOW_TO_HIGH 1 //"-s"
*/

void get_an_apt(GetCommand** com,int command_list_size, List* aptList, int* sortType) {
	List filterList = makeEmptyList();
	Apartment** arr;
	copyAptList(*aptList, &filterList);
	int i, whatToSort = SORT_DEF, arrSize;
	char* _com;
	for (i = 0; i < command_list_size; i++) {
		_com = (*(com + i))->command;
		if (isEqual(_com, "-MaximumPrice")) {
			whatToSort = SORT_PRICE;
			command_MaximumPrice(*aptList, (*(com + i))->value, &filterList);
		}
		if (isEqual(_com, "-MinimumNumRooms")) {
			command_MinimumNumRooms(*aptList, (*(com + i))->value, &filterList);
			whatToSort = SORT_NUM_Of_ROOMS;
		}
		if (isEqual(_com, "-MaximumNumRooms")) {
			command_MaximumNumRooms(*aptList, (*(com + i))->value, &filterList);
			whatToSort = SORT_NUM_Of_ROOMS;
		}
		if (isEqual(_com, "-Date")) {
			command_tillDate(*aptList, int_to_date((*(com + i))->value), &filterList);
			whatToSort = SORT_DATE;
		}
		
	}
	
		arr = listNodeToArray(filterList, &arrSize);
		sort(arr, arrSize, whatToSort, *sortType);
		printArr(arr,arrSize);
	// freeing the filtered list
	freeAptInList(filterList);
}
void add_an_apt(List* apt_list, Apartment apt) {
	insertNodeToTail(apt_list, createNode(apt.address, apt.price, apt.numOfRooms, apt.leavingDate));
}

void delete_an_apt(List* apt_list,int days) {
	Date date = getDateXDays(days);
	Apartment* apt;

	for (apt = apt_list->head; apt != NULL; apt) {
		if (apt->leavingDate.dayOfTheMonth >= date.dayOfTheMonth&& apt->leavingDate.month >= date.month&& apt->leavingDate.year >= date.year) {
			deleteNode(apt_list, apt);
		}
		apt = apt->next;
	}

}
void buy_an_apt(List* apt_list, short int ID) {
	Apartment* head = apt_list->head;
	while (head != NULL) {
		if (head->uniqueId == ID) {
			deleteNode(apt_list, head);
			free(head);
			return;
		}
		head = head->next;
		
	}
}
void command_MaximumPrice(List apt_list, int price, List* filterList){
	Apartment* head = filterList->head;
	while (head != NULL) {
		if (head->price > price) {
			deleteNode(filterList, head);
		}
		
		head = head->next;
		
	}
	
}

void command_MinimumNumRooms(List apt_list, int minRooms, List* filterList) {
	Apartment* head = filterList->head;
	while (head != NULL) {
		if (head->numOfRooms< minRooms) {
			deleteNode(filterList, head);
		}
		
		head = head->next;
		
	}
}												   
void command_MaximumNumRooms(List apt_list, int maxRooms, List* filterList) {
	Apartment* head = filterList->head;
	while (head != NULL) {
		if (head->numOfRooms> maxRooms) {
			deleteNode(filterList, head);
		}
		
		head = head->next;
		
	}
}
void command_tillDate(List apt_list, Date _date, List* filterList) {
	Apartment* head = filterList->head;
	while (head != NULL) {
		if (head->leavingDate.year > _date.year) {
			deleteNode(filterList, head);
		}
		else {
			if (head->leavingDate.month > _date.month&& head->leavingDate.year == _date.year) {
				deleteNode(filterList, head);
			}
			else {
				if (head->leavingDate.dayOfTheMonth > _date.dayOfTheMonth&& head->leavingDate.month == _date.month && head->leavingDate.year == _date.year) {
					deleteNode(filterList, head);
				}
				
			}
		}
		head = head->next;
		
		
	}
}
Date int_to_date(int number) {
	Date _date;
	_date.year = (short int)(number % 10000);
	number /= 10000;
	_date.month = (short int)(number % 100);
	number /= 100;
	_date.dayOfTheMonth = (short int)number;

	return _date;
}

void sort(Apartment** aptArr,int arrSize,int whatToSort, int sortType) {
	mergeSort(aptArr,0,arrSize-1, whatToSort, sortType);
}
List makeEmptyList()
{
	List result;

	result.head = result.tail = NULL;
	return result;
}
int isEmptyList(List lst)
{
	return (lst.head == NULL);
}
Apartment* createNode(char* address,int price,short int numOfRooms,Date date)
{
	Apartment *result;
	int str_len = strlen(address);
	result = (Apartment *)malloc(sizeof(Apartment));
	result->address = (char*)malloc((sizeof(char)*str_len)+1);
	strcpy( result->address , address);
	result->price = price;
	result->numOfRooms = numOfRooms;
	result->leavingDate.dayOfTheMonth = date.dayOfTheMonth;
	result->leavingDate.month = date.month;
	result->leavingDate.year = date.year;
	result->insertDate = curentDate();
	result->uniqueId = ++APT_ID;
	
	return result;
}
Apartment* reCreateNode(char* address, int price, short int numOfRooms, Date leavingDate, Date insertDate,short int ID)
{
	Apartment *result;
	int str_len = strlen(address);
	result = (Apartment *)malloc(sizeof(Apartment));
	result->address = (char*)malloc((sizeof(char)*str_len) + 1);
	strcpy(result->address, address);
	result->price = price;
	result->numOfRooms = numOfRooms;
	result->leavingDate = leavingDate;
	result->insertDate = insertDate;
	result->uniqueId = ++APT_ID;

	return result;
}
void deleteNode(List* lst, Apartment* apt)
{

	Apartment* next = apt->next;
	Apartment* prev = apt->prev;
	if (next != NULL) {
		next->prev = prev;
	}
	else {
		lst->tail = prev;
	}
	if (prev != NULL) {
		prev->next = next;
	}
	else {
		lst->head = next;
	}
	

}
void insertNodeToTail(List* lst, Apartment* newTail)
{
	Apartment* CopyedApt = malloc(sizeof(Apartment));
	copyApt(CopyedApt, newTail);
	CopyedApt->next = NULL;

	if (isEmptyList(*lst)) {
		lst->head = lst->tail = CopyedApt;
		CopyedApt->prev = NULL;
	}
	else
	{
		CopyedApt->prev = lst->tail;
		lst->tail->next = CopyedApt;
		lst->tail = CopyedApt;
	}
}
void freeAptInList(List lst)
{
	Apartment* curr = lst.head, *next;

	while (curr != NULL)
	{
		next = curr->next;
		freeNode(curr);
		curr = next;
	}
}
void freeNode(Apartment* apt) {
	free(apt->address);
	free(apt);
}
void printApt(Apartment apt) {
	printf("\nApt details:\n");
	printf("code: %d\n", apt.uniqueId);
	printf("Address: %s\n", apt.address);
	printf("Number of rooms: %d\n", apt.numOfRooms);
	printf("Price: %d\n", apt.price);
	printf("Entry date: %d.%d.%d\n", apt.leavingDate.dayOfTheMonth, apt.leavingDate.month, apt.leavingDate.year);
	printf("Database entry date: %d.%d.%d\n", apt.insertDate.dayOfTheMonth, apt.insertDate.month, apt.insertDate.year);

}
time_t getEpochTime() {
	time_t seconds;
	seconds = time(NULL);
	return seconds;	
}
Date curentDate() {
	int epoch = getEpochTime();
	
	return getDate(epoch);
}
Date getDate(time_t time) {
	Date date;
	struct tm *start = localtime(&time);
	date.dayOfTheMonth = start->tm_mday;
	date.month = start->tm_mon;
	date.year = start->tm_year+1900;
	return date;
}
/*
1 hour	3600 seconds
1 day	86400 seconds
1 week	604800 seconds
1 month (30.44 days) 	2629743 seconds
1 year (365.24 days)	31556926 seconds
*/
Date getDateXDays(int x) {
	int time = getEpochTime();

	time -= DAY_IN_SEC*x;
	return getDate(time);
}
void printList(List lst)
{
	Apartment *p;

	for (p = lst.head; p != NULL; p = p->next)
		printApt(*p);

}
int isEqual(char* str1, char* str2) {
	if (str1 == NULL || str2 == NULL) {
		return 0;
	}
	int str1L = strlen(str1), str2L = strlen(str2),i;
	if (str1L != str2L) {
		return 0;
	}
	for (i = 0; i < str1L; i++) {
		if (str1[i] != str2[i]) {
			return 0;
		}
	}
	return 1;
}
void copyApt(Apartment* dst, Apartment* src) {
	int addL = strlen(src->address);
	dst->address = malloc(addL+1);
	strcpy(dst->address, src->address);
	dst->insertDate = src->insertDate;
	dst->leavingDate = src->leavingDate;
	dst->numOfRooms = src->numOfRooms;
	dst->price = src->price;
	dst->uniqueId = src->uniqueId;

}




int merge(Apartment** arr, int l, int m, int h,int whatToSort,int sortOrder)
{
	Apartment** arr1,** arr2;  // Two temporary arrays to
	int n1, n2, i, j, k=0;
	n1 = m - l + 1;
	n2 = h - m;
	arr1 = malloc(sizeof(Apartment**)*n1);
	arr2 = malloc(sizeof(Apartment**)*n2);  // Two temporary arrays to
	for (i = 0; i<n1; i++)
		arr1[i] = arr[l + i];
	for (j = 0; j<n2; j++)
		arr2[j] = arr[m + j + 1];

	

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray
	j = 0; // Initial index of second subarray
	k = l; // Initial index of merged subarray
	while(i<n1&& j < n2)  //process of combining two sorted arrays
	{
		switch (whatToSort)
		{
		case 1: {// sort by price
			if (sortOrder) {
				if (arr1[i]->price > arr2[j]->price) 
					arr[k] = arr1[i++];
				else
					arr[k] = arr2[j++];
			}
			else {
				if (arr1[i]->price < arr2[j]->price)
					arr[k] = arr1[i++];
				else
					arr[k] = arr2[j++];
			}
		}
				break;
		case 2: {//sort by number of rooms
			if (sortOrder) {
				if (arr1[i]->numOfRooms >  arr2[j]->numOfRooms) 
					arr[k] = arr1[i++];
				else
					arr[k] = arr2[j++];

			}
			else {
				if (arr1[i]->numOfRooms <arr2[j]->numOfRooms)
					arr[k] = arr1[i++];
				else
					arr[k] = arr2[j++];
				

			}
		}
				break;
		case 3: {// sort by date
			if (sortOrder) {
				if ((arr1[i]->leavingDate.year > arr2[j]->leavingDate.year)|| (arr1[i]->leavingDate.month > arr2[j]->leavingDate.month&& arr1[i]->leavingDate.year == arr2[j]->leavingDate.year)|| arr1[i]->leavingDate.dayOfTheMonth > arr2[j]->leavingDate.dayOfTheMonth&& arr1[i]->leavingDate.month == arr2[j]->leavingDate.month && arr1[i]->leavingDate.year == arr2[j]->leavingDate.year)
					arr[k] = arr1[i++];
				else
					arr[k] = arr2[j++];

				
				

			}
			else {
				if ((arr1[i]->leavingDate.year < arr2[j]->leavingDate.year) || (arr1[i]->leavingDate.month < arr2[j]->leavingDate.month&& arr1[i]->leavingDate.year == arr2[j]->leavingDate.year) || arr1[i]->leavingDate.dayOfTheMonth < arr2[j]->leavingDate.dayOfTheMonth&& arr1[i]->leavingDate.month == arr2[j]->leavingDate.month && arr1[i]->leavingDate.year == arr2[j]->leavingDate.year)
					arr[k] = arr1[i++];
				else
					arr[k] = arr2[j++];

			}
		}
				break;

		default: {// sort by id or by defalt
			if (sortOrder) {
				if (arr1[i]->uniqueId < arr2[j]->uniqueId) 
					arr[k] = arr1[i++];
				else
					arr[k] = arr2[j++];
				
			}
			else {
				if (arr1[i]->uniqueId > arr2[j]->uniqueId) 
					arr[k] = arr1[i++];
				else
					arr[k] = arr2[j++];

			}}
				 break;

		}
		k++;
	}
	while (i<n1)
	{
		arr[k++] = arr1[i++];

	}
	while (j<n2)
	{
		arr[k++] = arr2[j++];

	}
	return 0;
}
void mergeSort(Apartment**arr, int start, int end,int whatToSort,int sortOrder) {
	int mid;
	if (start < end) {
		mid = (start + end) / 2;
		mergeSort(arr, start, mid,  whatToSort,  sortOrder);
		mergeSort(arr, mid + 1, end,  whatToSort,  sortOrder);
		merge(arr, start, mid, end,  whatToSort,  sortOrder);
	}
}
int sizeOfAptList(List lst) {
	Apartment* p = lst.head;
	int count = 0;
	while (p != NULL) {
		count++;
		p = p->next;
	}
	return count;
}
void swapNodes(Apartment* L, Apartment* R) {
	Apartment* NextR, *PreL,*TempL;

	NextR = R->next;
	R->next = L->next;
	PreL = R->prev;
	R->prev = L->prev;
	L->next = NextR;
	L->prev = PreL;
	TempL = L;
	L = R;
	R = TempL;

}
void copyAptList(List sorce, List* dest) {
	Apartment* p = sorce.head;
	while (p != NULL) {
		insertNodeToTail(dest, p);
		p = p->next;
	}
}

Apartment** listNodeToArray(List lst,int* arrSize) {
	Apartment** arr = malloc(sizeof(Apartment*)*2);
	Apartment* apt = lst.head;
	int arrLsize = 0, arrPsize = 2;
	while (apt != NULL) {
		if (arrLsize == arrPsize) {
			arrPsize *= 2;
			arr = (Apartment**)realloc(arr, sizeof(Apartment**)*arrPsize);
		}
		arr[arrLsize] = apt;
		apt = apt->next;
		arrLsize++;
	}
	arr = (Apartment**)realloc(arr, sizeof(Apartment**)*arrLsize);
	*arrSize = arrLsize;
	return arr;
}
void printArr(Apartment** arr, int size) {
	int i;
	for (i = 0; i < size; i++) {
		printApt(*(arr[i]));
	}
}