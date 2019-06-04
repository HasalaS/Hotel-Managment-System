#include "header.h"


/* Writing commands to a TEXT file */
//void write_to_file(char** short_list)
//{
//	int i;
//	FILE* fl;
//	fl = fopen("CommandHistory.txt", "wt");
//	for (i = 0; i < 7; i++)
//	{
//		fprintf(fl, "%s\n", short_list[i]);
//	}
//}



void write_cmd_to_long_file(CommandHistList* list, char* filename)
{
	int i;
	CommandHist *curr = list->head;
	FILE* fl = fopen(filename, "wt");
	if (checkFile(fl))
	{
		for (i = N - 1; i >= 0; i--)
		{
			if (short_term_history[i] != NULL)
				fprintf(fl, "%s\n", short_term_history[i]);
		}
		while (curr != NULL)
		{
			fprintf(fl, "%s\n", curr->command);
			curr = curr->next;
		}
	}
	fclose(fl);
	freeCmdInList(*list);
}

CommandHistList* read_cmd_from_long_file(char* filename)
{
	FILE* fl;
	char ch;
	char* cmd;
	int cmdLen = 0, i;
	CommandHistList* res = malloc(sizeof(CommandHistList));
	CommandHist* curr;

	int lSize, count=0, numOfLines=0;
	int pSize = 2;
	long int sizeOfFile;
	char* strFromFile = (char*)malloc(sizeof(char)*pSize);
	fl = fopen(filename, "rt");
	*res = makeEmptyHistoryList();

	if (checkFile(fl))
	{
		fseek(fl, 0, SEEK_END);
		sizeOfFile = ftell(fl);
		fseek(fl, 0, SEEK_SET);
		while (ftell(fl) < sizeOfFile) {
			ch = fgetc(fl);
			if (ch == '\n') {
				numOfLines++;
			}
		}
		fseek(fl, 0, SEEK_SET);
		while (ftell(fl) < sizeOfFile) {
			lSize = 0;
			pSize = 2;
			ch = fgetc(fl);
			while (ch != '\n')
			{
				strFromFile[lSize] = ch;
				lSize++;
				if (lSize == pSize)
				{
					pSize *= 2;
					strFromFile = (char*)realloc(strFromFile, sizeof(char)*pSize);
				}
				ch = fgetc(fl);
			}
			strFromFile = (char*)realloc(strFromFile, sizeof(char)*lSize);
			strFromFile[lSize] = '\0';
			if (count++ < N) {
				for (i = N - 2; i >= 0; i--)
				{
					short_term_history[i+1] = short_term_history[i];
				}
				
				//Send old cmd (tempCmd) to older history//
				short_term_history[0] = (char*)malloc(strlen(strFromFile) + 1);
				strcpy(short_term_history[0],strFromFile );
			}
			else {
				insertHistNodeToHead(res, createHistNode(strFromFile, (numOfLines--)-7));
				//transfer_to_long_history(strFromFile, res);
				
			}

		}
		fclose(fl);
	}
	return res;
}

int checkFile(FILE* fl)
{
	if (fl != NULL)
		return 1;
	else
		return 0;
}

void write_cmd_to_short(char* commandString, CommandHistList *cmdHistList)
{
	//printf("Hey im in \n");
	char* tempCmd, temp;
	int i = 0, j, counter = 0;

	if (strcmp(commandString, PRINT_LONG_HISTORY) == 0 || strcmp(commandString, PRINT_SHORT_HISTORY) == 0 || strcmp(commandString, "!!") == 0)
		return;

	while (i < N)
	{
		if (short_term_history[i] != NULL)
			counter++;
		i++;
	}

	if (counter == N)
	{
		tempCmd = (char*)malloc(sizeof(char) * (strlen(short_term_history[0]) + 1));
		strcpy(tempCmd, short_term_history[0]);
		for (j = 1; j < N; j++)
		{
			short_term_history[j - 1] = short_term_history[j];
		}
		//Send old cmd (tempCmd) to older history//
		transfer_to_long_history(tempCmd, cmdHistList);
		short_term_history[6] = (char*)malloc(strlen(commandString) + 1);
		strcpy(short_term_history[6], commandString);
		free(tempCmd);
	}
	else
	{
		short_term_history[counter] = (char*)malloc(strlen(commandString) + 1);
		strcpy(short_term_history[counter], commandString);
	}
}

void transfer_to_long_history(char* cmd, CommandHistList* list)
{
	CommandHist* node;
	if (list->tail == NULL)
	{
		node = createHistNode(cmd, 1);
	}
	else
	{
		node = createHistNode(cmd, list->tail->id + 1);
	}
	insertHistNodeToTail(list, node);
}

CommandHistList makeEmptyHistoryList()
{
	CommandHistList result;

	result.head = result.tail = NULL;
	return result;
}

int isEmptyHistList(CommandHistList lst)
{
	return (lst.head == NULL);
}

CommandHist* createHistNode(char* cmd, short int id)
{
	CommandHist *result;
	result = (CommandHist*)malloc(sizeof(CommandHist));
	result->id = id;
	result->command = (char*)malloc(sizeof(char)*(strlen(cmd) + 1));
	strcpy(result->command, cmd);
	return result;
}

void insertHistNodeToTail(CommandHistList* lst, CommandHist* newTail)
{
	newTail->next = NULL;

	if (isEmptyHistList(*lst))
	{
		lst->head = lst->tail = newTail;
		newTail->prev = NULL;
	}
	else
	{
		newTail->prev = lst->tail;
		lst->tail->next = newTail;
		lst->tail = newTail;
	}
}
void insertHistNodeToHead(CommandHistList* lst, CommandHist* newTail)
{
	newTail->prev = NULL;

	if (isEmptyHistList(*lst))
	{
		lst->head = lst->tail = newTail;
		newTail->next = NULL;
	}
	else
	{
		newTail->next = lst->head;
		lst->head->prev = newTail;
		lst->head = newTail;
	}
}
// Print  Long History
void print_long_history(CommandHistList *list)
{
	CommandHist *node = list->head;
	int i=1, ShortIndex;
	while (node != NULL)
	{
		printf("%d: %s\n",i++, node->command);
		node = node->next;
	}
	print_short_history(list);

}

// Print Short History
void print_short_history(CommandHistList *list)
{
	CommandHist *node = list->head;
	int i, ShortIndex;
	for (i = 0; i < N; i++)
	{
		if (list->tail == NULL)
			ShortIndex = 1;
		else
			ShortIndex = list->tail->id + i + 1;
		if (short_term_history[i] != NULL)
		{
			printf("%d: %s\n", ShortIndex, short_term_history[i]);
		}
	}
}

char* change_history(short int id, char* source, char* replacment, CommandHistList *list)
{
	int length, index, i, tempInd, j;
	char* temp;
	CommandHist* curr = list->head;
	if (list->tail != NULL)
	{
		length = strlen(curr->command);
		if (strlen(replacment) == strlen(source))
			temp = (char*)malloc(sizeof(char) * (length + 1));
		else
			temp = (char*)malloc(sizeof(char) * (strlen(replacment) - strlen(source) + 1));

		if (id < list->tail->id)
		{
			//Traverse long history for the id
			for (curr = list->head; curr != NULL; curr = curr->next)
			{
				if (curr->id = id)
				{
					// Found the right command , copying it to a new string and changing the command
					index = curr->command - strstr(curr->command, source);

					//Copying the replacment to the source
					for (i = 0; i < index; i++)
						temp[i] = curr->command[i];
					for (i = 0; i < strlen(replacment); i++)
						temp[index++] = replacment[i];
					for (i = index; temp[i] == '\0'; i++)
						temp[i] = curr->command[i];
					temp[i] = '\0';
				}
			}
		}
		else // id > list.tail->id
		{
			// The command is in short history
			for (i = 0; i < N; i++)
			{
				tempInd = list->tail->id + N - i;
				if (tempInd == id)
				{
					index = (&strstr(short_term_history[tempInd], source)[0]) - &(short_term_history[tempInd])[0];

					//Copying the replacment to the source
					for (j = 0; j < index; j++)
						temp[j] = short_term_history[i][j];
					for (j = 0; j < strlen(replacment); j++)
						temp[index++] = replacment[j];
					for (j = index; temp[j] == '\0'; j++)
						temp[j] = short_term_history[i][j];
					temp[j] = '\0';
				}
			}
		}
		write_cmd_to_short(temp, list);
		return temp;
	}
}

char* get_last_command(CommandHistList *historyList)
{
	// Write the last command to short history
	write_cmd_to_short(short_term_history[0], historyList);

	//Print to prompt
	//printf("%s\n", short_term[0]);
	return short_term_history[0];
}

int charcount(FILE *fin)
{
	int c, count;

	count = 0;
	for (;; )
	{
		c = fgetc(fin);
		if (c == EOF || c == '\n')
			break;
		++count;
	}
	return count;
}
//CommandHist* deleteNode(CommandHistList* lst, CommandHist* cmd)
//{
//
//	CommandHist* next = cmd->next;
//	CommandHist* prev = cmd->prev;
//	if (next != NULL) {
//		next->prev = prev;
//	}
//	else {
//		lst->tail = prev;
//	}
//	if (prev != NULL) {
//		prev->next = next;
//	}
//	else {
//		lst->head = next;
//	}
//	freeNode(cmd);
//	cmd = NULL;
//	return next;
//}

void freeCmdInList(CommandHistList lst)
{
	CommandHist* curr = lst.head, *next;

	while (curr != NULL)
	{
		next = curr->next;
		free(curr->command);
		free(curr);
		curr = next;
	}
}