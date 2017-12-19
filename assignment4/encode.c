# include <stdint.h>
# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include <stdbool.h>
# include <string.h>
# include <ctype.h>
# include <getopt.h>
# include <unistd.h>

extern char *optarg;

# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>

# include "code.h"
# include "priorityQueue.h"
# include "stack.h"
# include "huffman.h"
# include "bv.h"

# define MAGIC_NUMBER 0xDEADD00D

static void append(bitV *bv, code s, uint32_t *bvPos)
{
	uint32_t k;
	uint32_t len = s.l;
	
	for (uint32_t i = 0; i < len; i++)
	{
		k = s.bits[i / 8] & (1 << (i % 8));
		if (k)
		{
			setBit(bv, *bvPos);
		}
		/* don't need to clrBit since it's already a 0 by default */
		(*bvPos)++;
	}
}

int main(int argc, char **argv)
{
	/* parse options */
	char *inputFilename = NULL;
	char *outputFilename = NULL;
	int opt = -1;
	bool verbose = false; // Verbose starts as off
	bool print = false;
	
	while ((opt = getopt(argc, argv, "pvi:o:")) != -1)
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
				verbose = true; // Set the verbose flag on
				break;
			}
			case 'p':
			{
				print = true; // If the user wanted the tree to be printed
				break;
			}
			default:
			{
				fprintf(stderr, "invalid option passed\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	
	/* Decide in- and output files */
	FILE *inputFile;
	if (!inputFilename)
	{
		inputFile = stdin;
	}
	else
	{
		inputFile = fopen(inputFilename, "r");
		if (inputFile == NULL)
		{
			perror("fopen");
			exit(EXIT_FAILURE);
		}
	}
	
	FILE *outputFile;
	struct stat fileInfo;
	if (!outputFilename)
	{
		outputFile = stdout;
	}
	else
	{
		if (stat(outputFilename, &fileInfo) < 0)
		{
			/* stat failed, maybe the file doesn't exist */
			if (errno == ENOENT)
			{
				/* file doesn't exist, that's good so we can use it */
				outputFile = fopen(outputFilename, "w");
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
	
	/* Read file and make histogram */
	uint32_t histogram[256] = { 0 };
	int c;
	uint64_t inputSize = 0;
	while ((c = fgetc(inputFile)) != EOF)
	{
		histogram[c]++;
		inputSize++;
	}
	rewind(inputFile); /* reset it for later */
	
	histogram[0] += 1; // set the first and last value in the histogram so that there will always be a valid tree
	histogram[255] += 1;

	queue *theQueue = newQueue(256); /* queue has max 256 elements */

	/* add the items to the queue */
	uint16_t leafCount = 0;
	for (int sym = 0; sym < 256; sym++)
	{
		if (histogram[sym] > 0)
		{
			treeNode *n = newNode(sym, true, histogram[sym]);
			assert(enqueue(theQueue, n));
			leafCount++;
		}
	}
	
	uint16_t treeSize = (3 * leafCount) - 1;

	/* Create the huffman tree */
	item *a, *b, *joined;
	
	while (!emptyQ(theQueue))
	{
		assert(dequeue(theQueue,&a)); /* there must be at least one element */
		if(!dequeue(theQueue, &b))
		{
			/* no second element, now 'a' is the huffman tree */
			break;
		}
		
		joined = join(a, b);
		enqueue(theQueue,joined);
	}
	
	if (print)
	{
		printTree(a, 0);
	}
	
	/* Build the codes using post-order traversal of huffman tree */
	code table[256];
	code s = newCode();
	buildCode(a, s, table);
	
	/* Write out the file header and tree */
	uint32_t magic = MAGIC_NUMBER;
	fwrite(&magic, sizeof(uint32_t), 1, outputFile);
	fwrite(&inputSize, sizeof(uint64_t), 1, outputFile);
	fwrite(&treeSize, sizeof(uint16_t), 1, outputFile);
	dumpTree(a, outputFile);
	
	/* Write out the compressed file */
	uint32_t bitsNeeded = 0;
	for (int sym = 0; sym < 256; sym++)
	{
		bitsNeeded += histogram[sym] * table[sym].l; /* that many symbols times the number of bits it needs */
	}
	
	bitV *bv = newVec(bitsNeeded);
	uint32_t position = 0;
	while ((c = fgetc(inputFile)) != EOF)
	{
		append(bv, table[c], &position);
		//printf("%c\t%x %x\n", c, bv->v[1], bv->v[0]);
	}
	
	fwrite(bv->v, sizeVec(bv), sizeof(uint8_t), outputFile);
	
	// If verbose print tree size, file size, etc.
	if (verbose)
	{
		float percentage = 100. * bitsNeeded / (inputSize * 8);
		fprintf(stderr, "Original %lu bits: leaves %d (%d bytes) encoding %u bits (%f%%).\n", inputSize * 8, leafCount, treeSize, lenVec(bv), percentage);
	}
	
	if (outputFilename)
	{
		fclose(outputFile); /* don't bother closing stdout */
	}
	fclose(inputFile);
	
	delQueue(theQueue);
	delTree(a);
	delVec(bv);
	
	free(outputFilename);
	free(inputFilename);
	return 0;
}
