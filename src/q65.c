/*
 * message.c
 *
 *  Created on: 18 Dec 2025
 *  Functions are from RFzero modes library
 *  https://rfzero.net/
 *
 */


#include <stdio.h>
#include <stdint.h>
#include <string.h>




static int Q65_vector[] = { 0, 8, 11, 12, 14, 21, 22, 25, 26, 32, 34, 37, 45, 49, 54, 59, 61, 65, 68, 73, 75, 84 };

uint8_t symbolSequence[85];

unsigned int GetCharNo(const char *validChars, const char ch)
{
	const char *ptrCh;

	ptrCh = strchr(validChars, ch);
	if (ptrCh == NULL)
		return (unsigned int) (strchr(validChars, ' ') - validChars);
	else
		return (unsigned int) (ptrCh - validChars);
}


void wsjtxPack77Bits(const uint8_t * msgIn, uint8_t * msgPacked)
{
	static const char wsjtx77bitsChars[] = " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ+-./?";

	char msgPadded[13 + 1];
	sprintf(msgPadded, "%13s", msgIn);

	for (int i = 0; i < 13; i++)
	{
		// Mult, existing positions * 42 in the array
		uint16_t ireg = 0;  // 16 bits word
		for (int j = 8; j >= 0; j--)
		{
			ireg = (uint16_t)msgPacked[j] * 42 + ireg;
			msgPacked[j] = ireg;
			ireg >>= 8;
		}

		// Add, new bits to the array
		ireg = (uint16_t)GetCharNo(wsjtx77bitsChars, msgPadded[i]);

		for (int j = 8; j >= 0; j--)
		{
			if (!ireg)
				break;
			ireg = msgPacked[j] + ireg;
			msgPacked[j] = ireg;
			ireg >>= 8;
		}
	}

	// Left justify bits
	for (int i = 0; i < 10; i++)
	{
		msgPacked[i] <<= 1;
		if (i < 8)
			msgPacked[i] |= ((msgPacked[i + 1] >> 7) & 1);
	}
}


int q65_crc12(int * msg)
{
	int sr = 0;

	for (int i = 0; i < 13; i++)
	{
		int t = msg[i];

		for (int j = 0; j < 6; j++)
		{
			if ((t ^ sr) & 0x01)
				sr = (sr >> 1) ^ 0xF01;
			else
				sr = sr >> 1;

			t >>= 1;
		}
	}

	return sr & 0xFFF;
}

int gf64_add(int x, int y)
{
	return (x ^ y) & 63;
}

int gf64_mult(int x, int y)
{
	static const int gf64log[64] = { -1, 0, 1, 6, 2, 12, 7, 26, 3, 32, 13, 35, 8, 48, 27, 18, 4, 24, 33, 16, 14, 52, 36, 54, 9, 45, 49, 38, 28, 41, 19, 56, 5, 62, 25, 11, 34, 31, 17, 47, 15, 23, 53, 51, 37, 44, 55, 40, 10, 61, 46, 30, 50, 22, 39, 43, 29, 60, 42, 21, 20, 59, 57, 58 };

	static const int gf64antilog[64] = { 1, 2, 4, 8, 16, 32, 3, 6, 12, 24, 48, 35, 5, 10, 20, 40, 19, 38, 15, 30, 60, 59, 53, 41, 17, 34, 7, 14, 28, 56, 51, 37, 9, 18, 36, 11, 22, 44, 27, 54, 47, 29, 58, 55, 45, 25, 50, 39, 13, 26,	52, 43, 21, 42, 23, 46, 31, 62, 63, 61,	57, 49, 33 };

	if ((x == 0) || (y == 0))
		return 0;
	else if (x == 1)
		return y;
	else if (y == 1)
		return x;
	else
	{
		int value = (gf64log[x] + gf64log[y]) % 63;
		return gf64antilog[value];
	}
}

void q65codeword(int * msgIn_6b, int *cw_6b)
{
	static const int generator[50][15] ={
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
		{ 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
		{ 0, 20, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
		{ 0, 20, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
		{ 0, 20, 0, 1, 1, 0, 0, 0, 10, 0, 0, 0, 0, 1, 0 },
		{ 0, 20, 0, 1, 1, 0, 0, 0, 10, 0, 0, 0, 44, 1, 0 },
		{ 0, 20, 0, 1, 1, 0, 0, 0, 10, 1, 0, 0, 44, 1, 0 },
		{ 0, 20, 0, 1, 1, 0, 0, 0, 10, 1, 0, 0, 44, 1, 14 },
		{ 0, 20, 0, 1, 1, 0, 0, 0, 10, 1, 31, 0, 44, 1, 14 },
		{ 0, 20, 0, 1, 1, 33, 0, 0, 10, 1, 31, 0, 44, 1, 14 },
		{ 56, 20, 0, 1, 1, 33, 0, 0, 10, 1, 31, 0, 44, 1, 14 },
		{ 56, 20, 0, 1, 1, 33, 0, 1, 10, 1, 31, 0, 44, 1, 14 },
		{ 56, 1, 0, 1, 1, 33, 0, 1, 10, 1, 31, 0, 44, 1, 14 },
		{ 56, 1, 0, 1, 1, 33, 0, 1, 10, 1, 31, 36, 44, 1, 14 },
		{ 56, 1, 0, 1, 1, 33, 0, 1, 43, 1, 31, 36, 44, 1, 14 },
		{ 56, 1, 0, 1, 1, 33, 0, 1, 43, 17, 31, 36, 44, 1, 14 },
		{ 56, 1, 0, 1, 1, 33, 0, 1, 43, 17, 31, 36, 36, 1, 14 },
		{ 56, 1, 0, 1, 1, 33, 53, 1, 43, 17, 31, 36, 36, 1, 14 },
		{ 56, 1, 0, 35, 1, 33, 53, 1, 43, 17, 31, 36, 36, 1, 14 },
		{ 56, 1, 0, 35, 1, 33, 53, 1, 43, 17, 30, 36, 36, 1, 14 },
		{ 56, 1, 0, 35, 1, 33, 53, 52, 43, 17, 30, 36, 36, 1, 14 },
		{ 56, 1, 0, 35, 1, 32, 53, 52, 43, 17, 30, 36, 36, 1, 14 },
		{ 56, 1, 60, 35, 1, 32, 53, 52, 43, 17, 30, 36, 36, 1, 14 },
		{ 56, 1, 60, 35, 1, 32, 53, 52, 43, 17, 30, 36, 36, 49, 14 },
		{ 56, 1, 60, 35, 1, 32, 53, 52, 43, 17, 30, 36, 37, 49, 14 },
		{ 56, 1, 60, 35, 54, 32, 53, 52, 43, 17, 30, 36, 37, 49, 14 },
		{ 56, 1, 60, 35, 54, 32, 53, 52, 1, 17, 30, 36, 37, 49, 14 },
		{ 1, 1, 60, 35, 54, 32, 53, 52, 1, 17, 30, 36, 37, 49, 14 },
		{ 1, 0, 60, 35, 54, 32, 53, 52, 1, 17, 30, 36, 37, 49, 14 },
		{ 1, 0, 60, 35, 54, 32, 53, 52, 1, 17, 30, 37, 37, 49, 14 },
		{ 1, 0, 61, 35, 54, 32, 53, 52, 1, 17, 30, 37, 37, 49, 14 },
		{ 1, 0, 61, 35, 54, 32, 53, 52, 1, 48, 30, 37, 37, 49, 14 },
		{ 1, 0, 61, 35, 54, 32, 53, 52, 1, 48, 30, 37, 37, 49, 15 },
		{ 1, 0, 61, 35, 54, 0, 53, 52, 1, 48, 30, 37, 37, 49, 15 },
		{ 1, 0, 61, 35, 54, 0, 52, 52, 1, 48, 30, 37, 37, 49, 15 },
		{ 1, 0, 61, 35, 54, 0, 52, 52, 1, 48, 30, 37, 37, 0, 15 },
		{ 1, 0, 61, 35, 54, 0, 52, 34, 1, 48, 30, 37, 37, 0, 15 },
		{ 1, 0, 61, 35, 54, 0, 52, 34, 1, 48, 30, 37, 0, 0, 15 },
		{ 1, 0, 61, 35, 54, 0, 52, 34, 1, 48, 30, 20, 0, 0, 15 },
		{ 1, 0, 0, 35, 54, 0, 52, 34, 1, 48, 30, 20, 0, 0, 15 },
		{ 1, 0, 0, 35, 54, 0, 52, 34, 1, 0, 30, 20, 0, 0, 15 },
		{ 0, 0, 0, 35, 54, 0, 52, 34, 1, 0, 30, 20, 0, 0, 15 },
		{ 0, 0, 0, 35, 54, 0, 52, 34, 1, 0, 38, 20, 0, 0, 15 },
		{ 0, 0, 0, 35, 0, 0, 52, 34, 1, 0, 38, 20, 0, 0, 15 },
		{ 0, 0, 0, 35, 0, 0, 52, 0, 1, 0, 38, 20, 0, 0, 15 },
		{ 0, 0, 0, 35, 0, 0, 52, 0, 1, 0, 38, 20, 0, 0, 0 },
		{ 0, 0, 0, 35, 0, 0, 52, 0, 0, 0, 38, 20, 0, 0, 0 },
		{ 0, 0, 0, 35, 0, 0, 52, 0, 0, 0, 38, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 52, 0, 0, 0, 38, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 38, 0, 0, 0, 0 }
	};

	for (int i = 0; i < 15; i++)
		cw_6b[i] = msgIn_6b[i];

	for (int i = 0; i < 15; i++)
		for (int j = 15; j < 65; j++)
			cw_6b[j] = gf64_add(cw_6b[j], gf64_mult(msgIn_6b[i], generator[j - 15][i]));
}



void gen_q65_symbols(char *msg){
    const unsigned int msgLen = strlen(msg);

    // Pad free message if shorter than 13 chars
    char message[msgLen + 1];
    sprintf(message, "%-13s", msg);

    // Source encoding and packing
    uint8_t packed[12] = { 0 };

    wsjtxPack77Bits((uint8_t *) msg, (uint8_t *) packed);

       // Packed 6 bits
    int packed_6b[15] = { 0 };
    for (int i = 0; i < 4; i++)
    {
    	packed_6b[i * 4] = (packed[i * 3] >> 2);
    	packed_6b[i * 4 + 1] = ((packed[i * 3] << 4) & 0x30) + ((packed[i * 3 + 1] >> 4) & 0x0F);
    	packed_6b[i * 4 + 2] = ((packed[i * 3 + 1] << 2) & 0x3C) + ((packed[i * 3 + 2] >> 6) & 0x03);
    	if (i < 3)
    		packed_6b[i * 4 + 3] = packed[i * 3 + 2] & 0x3F;
    }

    // CRC
    int crc12 = q65_crc12(packed_6b);

    packed_6b[13] = crc12 & 0x03F;
    packed_6b[14] = crc12 >> 6;

    // Codeword
    int codeword[65] = { 0 };

    q65codeword((int*) packed_6b, (int*) codeword);

    // Merge with Sync Vector
    int j = 0, k = 0;
    for (int i = 0; i < 85; i++)
    {
    	if (Q65_vector[j] == i)
    	{
    		symbolSequence[i] = 0;
    		j++;
    	}
    	else
    	{
    		if ((i == 11) || (i == 18))
    			k += 2;
    		symbolSequence[i] = codeword[k++] + 1;
    	}
    }
}
