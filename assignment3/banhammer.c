
# include "linkedList.h"
# include "bloomFilter.h"
# include "hash.h"//"hash_test.h" // HASH.h
# include "bv.h"
# include "words.h"
# include "getopt.h"
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <errno.h>
# include <stdbool.h>
# include <string.h>
# include <ctype.h>

bool moveToFront = false; // By default we do not move to front
uint32_t seeks = 0; // this is an extern variable declared in linkedList.h
uint32_t average = 0;
uint32_t initA[] = {0xDeadD00d, 0xFadedBee, 0xBadAb0de, 0xC0c0aB0a}; // First Bloom filter 
uint32_t initB[] = {0xDeadBeef, 0xFadedB0a, 0xCafeD00d, 0xC0c0aB0a}; // Second Bloom filter 
uint32_t initH[] = {0xDeadD00d, 0xFadedBee, 0xBadAb0de, 0xC0c0Babe}; // Hash table

// Hash the badspeak and newspeak text files into the bloom filters and hash tables
void HashBadspeak (bloomF *bf1,bloomF *bf2,hashTable *HT);
void HashNewspeak (bloomF *bf1,bloomF *bf2,hashTable *HT);

void CheckHT(hashTable *h,char *word, listNode **list,listNode **list2);

int yylex(void); // words.l functions and variables
extern char* yytext;
extern FILE* yyin;

void PrintThoughtCrime(void);
void PrintNewSpeak(void);

static uint32_t dictionary; // These variables are declared static because I want to use 
static uint32_t translations; // them in multiple functions in this file

int main(int argc, char **argv)
{
	uint32_t text = 0; // These variables are used for tracking stats
	dictionary = 0;
	translations = 0;
	
	uint32_t hashSize = 10000; // default hash table size
	uint32_t bloomSize = 1 << 20; // Default bloom filter size
	int argvLocation = 0;
	uint8_t onlyStats = 0; // This will track the stats
	int c;
	
	while((c = getopt(argc,argv, "smbh:f:")) != -1)
	{
		argvLocation++;
		switch(c)
		{
			case('s'):
			{
				onlyStats = 1; // Set stats on
				break;
			}
			case('m'):
			{
				moveToFront = true; // Set the move to front rule on
				break;
			}
			case('b'):
			{
				moveToFront = false; // Set the move to front rule off
				break;
			}	
			case('h'):
			{
				hashSize = atoi(argv[++argvLocation]); // Grab the hash size
				break;
			}
			case('f'):
			{
				bloomSize = atoi(argv[++argvLocation]); // grab the bloom size 
				break;
			}			
		}		
	}
	
	// Create the bloom filters and hash table
	bloomF *BF1;
	bloomF *BF2;
	hashTable *HT;
	BF1 = newBF(bloomSize,initA);
	BF2 = newBF(bloomSize,initB);
	HT = newHT(hashSize,initH);
	
	// Go through the badspeak and newspeak files and hash their values into the bloom filters and hash tables		
	char temp[20];
	char *tempDup;
	FILE *fp;
	fp = fopen("/afs/cats.ucsc.edu/users/g/darrell/badspeak.txt","r");
	// This loop goes through the badspeak file and gets all of the words and puts them in the bloom filters and hash tables
	while(fscanf(fp,"%s ",temp) != EOF)
	{
		dictionary += 1; // Add one to the number of words in th dictionary
		setBF(BF1,temp);
		setBF(BF2,temp); 
		tempDup = strdup(temp);
		insertHT(HT,tempDup,NULL);
	}
	fclose(fp);
	
	char temp1[20], temp2[20]; // Declare arrays that can be used to hold words that are read in from the badspeak and newspeak files
	char *tempDup1; // Declare pointers that will hold the values that will go into the hash table
	char *tempDup2;
	fp = fopen("/afs/cats.ucsc.edu/users/g/darrell/newspeak.txt","r");
	// This loop goes through the badspeak file and gets all of the words and puts them in the bloom filters and hash tables
	while(fscanf(fp,"%s %s ",temp1,temp2) != EOF)
	{
		translations += 1; // Add one to the number of words in the translation file
		setBF(BF1,temp1);
		setBF(BF2,temp1); 
		tempDup1 = strdup(temp1);
		tempDup2 = strdup(temp2);
		insertHT(HT,tempDup1,tempDup2);
	}
	fclose(fp); 
	
	
	
	
	listNode *listOfBadspeak;
	listNode *listOfNewspeak;
	
	listOfBadspeak = newNode("Start",""); // Make new lists to store the words that were found in the output file
	listOfNewspeak = newNode("Start","");
	
	uint32_t tempStrLen; // Declare a variable that gets the length of the strings that are read in from the file
	uint32_t i;
	yyin = stdin;
	int flexID = 0;
	while (flexID != EOF)
	{
		flexID = yylex();
		switch(flexID)
		{
			case WORD:
			{
				tempStrLen = strlen(yytext); // If it is a word then we need to make sure that it is lowercase
				char str[tempStrLen];
				strcpy(str,yytext);
				i = 0;
				while (str[i])
				{
					str[i] = (char) tolower(str[i]); // tolower is a C function that converts uppercase letters into lowercase
					i++;
				}
				text++; // Count the number of words from stdin
				if (memBF(BF1,str)) // Check the first bloom filter
				{
					if (memBF(BF2,str)) // Check the second bloom filter
					{
						// Check the hash table and if it is there, then store the value
						CheckHT(HT,str,&listOfBadspeak,&listOfNewspeak); 
					}
				}			
				break;
			}
			case EOF:
			{
				break;
			}
			default: // If the yylex value returns something other than WORD or EOF then there is an error
			{
				break;
			}
		}
	}
	fclose(yyin); // Close the file
	
	
	if (onlyStats == 0) // If stats are not set
	{
		uint8_t badBool = 0;
		if (strcmp(listOfBadspeak->oldspeak,"Start") != 0) // This makes a check to see if there were any badspeak words that were read in from stdin
		{
			PrintThoughtCrime(); // Print the thoughtcrime response
			printLinkedList(listOfBadspeak); // Print out the list of badspeak words found
			badBool = 1;
		}
		if (badBool == 1 && (strcmp(listOfNewspeak->oldspeak,"Start") != 0)) // This check is for if there were badspeak and new speak words
		{
			printf("\nAnd think of these words on your vacation!\n");
			printLinkedList(listOfNewspeak); // Print the list of newspeak words found
		}
		else if (badBool == 0 && (strcmp(listOfNewspeak->oldspeak,"Start") != 0))
		{
			PrintNewSpeak(); // Print out the translation prompt
			printLinkedList(listOfNewspeak); 
		}
	}
	else // If stats are set
	{
		int count1,count2;
		count1 = (int) countBF(BF1); // Get the number of bits set in each bloom filter
		count2 = (int) countBF(BF2);
		double density1,density2;
		density1 = (double) count1 / lenBF(BF1); // Get the ratio of set bits to total bits in each bloom filter
		density2 = (double) count2 / lenBF(BF2); 		
		double avg = seeks / average;
		// Print out all of the stats
		printf("Seeks %u, Average %f, Dictionary %u, Translations %u, Text %u, Densities: %f, %f\n",
				seeks,avg,dictionary,translations,text,density1,density2);
	}
	
	
	delLinkedList(listOfNewspeak); // Free the memory of words found
	delLinkedList(listOfBadspeak); 
	
	free(yytext);
	delBF(BF2);
	delBF(BF1);
	delHT(HT); 
	
	return 0;
} 

// This takes in words from stdin and then checks if they are in the hash table
void CheckHT(hashTable *h, char *word, listNode **list,listNode **list2)
{
	listNode *tempList;
	tempList = findHT(h,word); // Find if the word is in the hash table
	if (tempList != NULL) // Check if it returned a valid node
	{
		if (tempList->newspeak != NULL) // newspeak
		{
			insertLinkedList(list2,tempList->oldspeak,tempList->newspeak);
		}
		else // badspeak
		{
			insertLinkedList(list,tempList->oldspeak,tempList->newspeak);
		}
	}
	return; // If it could not be found in the hash table then just return
}

// Used to hash all of the words in badspeak and put it in the bloom filters and hash table
void HashBadspeak (bloomF *bf1,bloomF *bf2,hashTable *HT)
{
	char temp[20];
	char *tempDup;
	FILE *fp;
	fp = fopen("/afs/cats.ucsc.edu/users/g/darrell/badspeak.txt","r");
	while(fscanf(fp,"%s ",temp) != EOF)
	{
		dictionary += 1;
		setBF(bf1,temp);
		setBF(bf2,temp); 
		tempDup = strdup(temp);
		insertHT(HT,tempDup,NULL);
	}
	fclose(fp);
	return;
}

void HashNewspeak (bloomF *bf1,bloomF *bf2,hashTable *HT)
{
	char temp[20], temp2[20];
	char *tempDup;
	char *tempDup2;
	FILE *fp;
	fp = fopen("/afs/cats.ucsc.edu/users/g/darrell/newspeak.txt","r");
	while(fscanf(fp,"%s %s ",temp,temp2) != EOF)
	{
		translations += 1;
		setBF(bf1,temp);
		setBF(bf2,temp); 
		tempDup = strdup(temp);
		tempDup2 = strdup(temp2);
		insertHT(HT,tempDup,tempDup2);
	}
	fclose(fp);
	return;
}

// Print the output for a thoughtcrime
void PrintThoughtCrime(void)
{
	printf("Dear Comrade,\n\nYou have chosen to use degenerate words that may cause hurt\nfeelings or cause your comrades to think unpleasant thoughts.\nThis is doubleplus bad. To correct your wrongthink and\nsave community consensus we will be sending you to joycamp\nadministered by Miniluv.\n\nYour Errors:\n");
}

// Print the output for newspeak
void PrintNewSpeak(void)
{
	printf("Dear Comrade,\n\nSubmitting your text helps to preserve feelings and prevent\nbadthink. Some of the words that you used are not goodspeak.\nThe list shows how to turn the oldspeak words into newspeak.\n");
}