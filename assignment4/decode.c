# include <stdint.h>
# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include <stdbool.h>
# include <string.h>
# include <ctype.h>
# include <getopt.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <errno.h>

extern char *optarg;

extern int errno;

# include "code.h"
# include "priorityQueue.h"
# include "stack.h"
# include "huffman.h"
# include "bv.h"

const uint32_t MAGIC_NUM = 0xDEADD00D;

static inline void readFilePart(FILE *fp, void *ptr, size_t size, size_t nmemb, char *errorMsg)
{
	if (fread(ptr, size, nmemb, fp) != nmemb)
	{
		fputs(errorMsg, stderr);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv)
{
	// parse options 
	char *inputFilename;
	char *outputFilename = NULL;
	int opt;
	bool verbose = false; // start verbose as off
	bool printT = false;
	
	while ((opt = getopt(argc, argv, "vi:o:p")) != -1)
	{
		switch (opt)
		{
			case 'i':
			{
				inputFilename = strdup(optarg);
				break;
			}
			case 'o':
			{
				outputFilename = strdup(optarg);
				break;
			}
			case 'v':
			{
				verbose = true;
				break;
			}
			case 'p':
			{
				printT = true;
				break;
			}
			default:
			{
				fprintf(stderr, "invalid option passed\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	
	/* Decide output files */
	FILE *outFile;
	struct stat fileInfo;
	if (!outputFilename)
	{
		outFile = stdout;
	}
	else
	{
		if (stat(outputFilename, &fileInfo) < 0)
		{
			/* stat failed, maybe the file doesn't exist */
			if (errno == ENOENT)
			{
				/* file doesn't exist, that's good so we can use it */
				outFile = fopen(outputFilename, "w");
			}
			else
			{
				/* some other error occured with the file */
				perror(argv[0]);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			/* stat succeeded, so the file exists and we can't write to it */
			fprintf(stderr, "the output file %s already exists\n", outputFilename);
			exit(EXIT_FAILURE);
		}
	}
	
	/* Read in the input file */
	FILE *inFile;
	if (!inputFilename)
	{
		inFile = stdin;
	}
	else
	{
		inFile = fopen(inputFilename, "r");
		if (inFile == NULL)
		{
			perror("fopen");
			exit(EXIT_FAILURE);
		}
	}	

	uint32_t magicNum = 0;
	readFilePart(inFile, &magicNum, sizeof(uint32_t), 1, "Not a compressed file");
	if (magicNum != MAGIC_NUM)
	{
		fputs("Not a compressed file\n", stderr);
		exit(EXIT_FAILURE);
	}
	
	uint64_t fileSize = 0;
	readFilePart(inFile, &fileSize, sizeof(uint64_t), 1, "Not a compressed file: can't read file size");

	uint16_t treeSize = 0;
	readFilePart(inFile, &treeSize, sizeof(uint16_t), 1, "Not a compressed file: can't read tree size");

	/* Maybe show info about tree */
	if (verbose)
	{
		fprintf(stderr, "Original %lu bits: tree (%d)\n", fileSize * 8, treeSize);
	}
	
	/* Read in tree */
	uint8_t *treeBytes = calloc(treeSize,sizeof(uint8_t));
	readFilePart(inFile, treeBytes, sizeof(uint8_t), treeSize, "Not a compressed file: can't read tree");
	treeNode *tree = loadTree(treeBytes, treeSize);
	free(treeBytes);
	
	if (printT)
	{
		printTree(tree, 0);
	}
	
	/* Decompress the file */
	treeNode *current = tree;
	bitV *bv = newVec(8);
	int c;
	uint64_t bytesWritten = 0;
	while ((c = fgetc(inFile)) != EOF && bytesWritten < fileSize)
	{
		bv->v[0] = c; /* use small bit vector and then use each bit */
		
		for (uint8_t pos = 0; pos < 8 && bytesWritten < fileSize; pos++)
		{
			uint32_t k = valBit(bv, pos); /* read a bit */
			int32_t sym = stepTree(tree, &current, k); /* step through the tree */
			if (sym >= 0 && bytesWritten < fileSize)
			{
				/* it's a leaf node so print the symbol */
				fputc(sym, outFile);
				bytesWritten++;
			}
		}
	}
	
	delVec(bv);
	delTree(tree);
	
	fclose(outFile);
	fclose(inFile);
	free(inputFilename);
	free(outputFilename);
	
	return 0;
}
