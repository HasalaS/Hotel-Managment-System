#include "header.h"

void commandManagement(List* lst, CommandHistList *historyLst) {

	int i = 0, size;
	char ch;

	while (true)
	{

		int commandListLogicalsize = 0, inputStringLengthLogical = 0, inputStringLengthPhisical = 2;
		char* cmd = (char*)malloc(sizeof(char)*inputStringLengthPhisical);
		ch = getchar();
		while (ch != '\n')
		{
			if (inputStringLengthLogical == inputStringLengthPhisical)
			{
				inputStringLengthPhisical *= 2;
				cmd = (char *)realloc(cmd, (inputStringLengthPhisical) + 1);

			}
			cmd[inputStringLengthLogical++] = ch;
			ch = getchar();
		}
		cmd = (char *)realloc(cmd, (sizeof(char) *inputStringLengthLogical) + 1);
		cmd[inputStringLengthLogical] = '\0';

		if (isEqual(cmd, EXIT))
			break;
		//Write cmd to short

		if (cmd[0] == '!')
			parse_recall_string(historyLst, cmd);

		write_cmd_to_short(cmd, historyLst);

		// Call the	print long history function
		if (isEqual(cmd, PRINT_LONG_HISTORY))
			print_long_history(historyLst);
		else {
			// Call the print short history function
			if (isEqual(cmd, PRINT_SHORT_HISTORY))
				print_short_history(historyLst);
			else
				create_cmd(lst, cmd);
		}
	}
}
void create_cmd(List* lst,char* cmd)
{
	char* cmdVal;
	int sortType = -1,days, commandListsize=0;
	char** commandList;
	cmdVal = strtok(cmd, TOKEN);

	if (isEqual(cmdVal, GET_APT))
	{
		commandList = parseGetCommand(cmd, &commandListsize, &sortType);
		get_an_apt(commandList, commandListsize, lst, &sortType);
	}
	else if (isEqual(cmdVal, ADD_APT) )
	{
		add_an_apt(lst, parseAddApt(cmd));
	}
	else if (isEqual(cmdVal, BUY_APT) )
	{
		//int temp = );
		buy_an_apt(lst, (short int)(atoi(strtok(NULL, TOKEN))));
	}
	else if (isEqual(cmdVal, DELETE_APT))
	{

		fscanf(cmd, "%d", &days);
		delete_an_apt(lst, days);
	}
}


Apartment parseAddApt(char* cmd) {
	Apartment apt;
	char* cmdTemp;
	int i = 0;
	int psize = 1;
	//Parsing
	apt.address = strtok(NULL, "\"");
	apt.price = atoi(strtok(NULL, TOKEN));
	apt.numOfRooms = (short int)atoi(strtok(NULL, TOKEN));
	apt.leavingDate.dayOfTheMonth= (short int)atoi(strtok(NULL, TOKEN));
	apt.leavingDate.month = (short int)atoi(strtok(NULL, TOKEN));
	apt.leavingDate.year = 2000+(short int)atoi(strtok(NULL, TOKEN));
	return apt;
}


GetCommand** parseGetCommand(char* cmd, int *lsize,int *sortType) {
	int psize = 1;
	
	GetCommand** cmdList= (GetCommand**)malloc(sizeof(GetCommand*) * psize);;
	char* cmdTemp;
	
	//Parsing
	while ((cmdTemp = strtok(NULL, TOKEN)) != NULL)
	{
		if (isEqual("-sr", cmdTemp) ) {
			*sortType = HIGH_TO_LOW;
			cmdTemp = strtok(NULL, TOKEN);

		}
		if (isEqual("-s", cmdTemp))
		{
			*sortType = LOW_TO_HIGH;
			cmdTemp = strtok(NULL, TOKEN);
		}
		if (cmdTemp == NULL)
			break;
		if (*lsize == psize)
		{
			psize *= 2;
			cmdList = (GetCommand**)realloc(cmdList, sizeof(GetCommand*) * psize);
		}
		cmdList[*lsize] = malloc(sizeof(GetCommand));

		cmdList[*lsize]->command = (char*)malloc(sizeof(char) * strlen(cmdTemp + 1));
		strcpy(cmdList[*lsize]->command, cmdTemp);

		cmdList[*lsize]->value = atoi(strtok(NULL, TOKEN));

		(*lsize)++;
	}
	cmdList = (GetCommand**)realloc(cmdList, sizeof(GetCommand*) * (*lsize));
	return cmdList;
}

void parse_recall_string(CommandHistList *list, char* cmd)
{
	char* source;
	char* replacment;
	short int id;

	if (cmd[1] == '!')
	{
		//Get last command
		cmd = (char*)realloc(cmd, sizeof(char) * strlen(short_term_history[0]) + 1);
		strcpy(cmd, get_last_command(list));
	}
	else
	{
		id =(short int)( atoi(strtok(cmd, "!^"))); // ID of the command
		source = strtok(NULL, "^"); //string to search and replace
		replacment = strtok(NULL, "^"); // String to replace with
		strcpy(cmd, change_history(id, source, replacment, list)); //Create the command and save it into short.
	}
}