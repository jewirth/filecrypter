/*
 * filecrypter.c
 *
 * Author: Jens Wirth jw@snej.de
 * 
 * This tool encrypts a file using another file as password
 *
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Poul-Henning Kamp
 * ----------------------------------------------------------------------------
 *
 */

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h>

int main(int argc, char **argv)
{
	FILE *in   = fopen(argv[1], "rb");
	FILE *out  = fopen(argv[2], "wb");
	FILE *pw   = fopen(argv[3], "rb");
	int inbyte = 0;
	int pwbyte = 0;

	// check arguments
	if (argc != 4 || in == NULL || out == NULL || pw ==NULL)
	{
		printf("Syntax: filecrypter <INFILE> <OUTFILE> <PWFILE>\n");
		return 1;
	}


	// error if input or password file contain no data
	fseek(in, 0, SEEK_END);
	fseek(pw, 0, SEEK_END);
	if (ftell(in) == 0 || ftell(pw) == 0)
	{
		printf("INFILE and OUTFILE must contain data\n");
		return 1;
	}
	fseek(pw, 0, SEEK_SET);
	fseek(in, 0, SEEK_SET);

	// encrypt each byte of input file
	while ((inbyte=fgetc(in)) != EOF)
	{
		// rewind password file
		if ((pwbyte=fgetc(pw)) == EOF)
		{
			fseek(pw, 0, SEEK_SET);
			pwbyte = fgetc(pw);
		}

		// simple XOR encryption
		fputc(inbyte ^ pwbyte, out);
	}

	fclose(pw);
	fclose(out);
	fclose(in);

	return 0;
}

