#include "card.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#pragma warning (disable:4996)
#define MaxName 24
#define MinName 20
#define MaxDate 5
#define MaxPAN 19
#define MinPAN 16

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
	if (!cardData) //Validates that the pointer isn't NULL
		return WRONG_NAME;
	fseek(stdin, 0, SEEK_END); //Ignores past input like '/n'
	uint8_t temp[40];
	printf("Enter Card holder name [20-24 characters]:- ");
	fgets(temp, 40, stdin);
	if (strlen(temp) < MinName + 1 || strlen(temp) > MaxName + 1) //Check Length of Input name between 20 & 24
		return WRONG_NAME;
	for (uint32_t loc_i = 0; loc_i < strlen(temp) - 1; loc_i++) {
		if (!isalpha(temp[loc_i]) && temp[loc_i] != ' ') //Checks if Input alphabets and white space only
			return WRONG_NAME;
	}
	temp[24] = '\0';
	strcpy(cardData->cardHolderName, temp);
	return CARD_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
	if (!cardData) //Validates that the pointer isn't NULL
		return WRONG_EXP_DATE;
	fseek(stdin, 0, SEEK_END); //Ignores past input like '/n'
	uint8_t temp[20];
	uint32_t loc_month, loc_year;
	printf("Enter expiry date [MM/YY]:- ");
	fgets(temp, 20, stdin);
	if ((strlen(temp)  != MaxDate + 1) || (temp[2] != '/')) //Checks the length and the dash position
		return WRONG_EXP_DATE;
	for (uint32_t loc_i = 0; loc_i < MaxDate; loc_i++) {
		if (loc_i == 2)
			continue;
		if (!(isdigit(temp[loc_i]))) //Checks if the date is all digits
			return WRONG_EXP_DATE;
	}
	loc_month = (temp[0] - '0') * 10 + (temp[1] - '0'); //To Check the input month is between 1 and 12  ex.(12)
	loc_year = (temp[3] - '0') * 10 + (temp[4] - '0'); //To Check the input year is greater than 0  ex.(25)
	if ((loc_month <= 0 || loc_month > 12) || (loc_year <= 0))
		return WRONG_EXP_DATE;
	temp[5] = '\0';
	strcpy(cardData->cardExpirationDate, temp);
	return CARD_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	if (!cardData) //Validates that the pointer isn't NULL
		return WRONG_PAN;
	fseek(stdin, 0, SEEK_END); //Ignores past input like '/n'
	uint8_t temp[30];
	printf("Enter Primary Account Number [16-19 characters]:- ");
	fgets(temp, 30, stdin);
	if (strlen(temp) < MinPAN + 1 || strlen(temp) > MaxPAN + 1) //Checks length of PAN is between 16 and 19
		return WRONG_PAN;
	for (uint32_t loc_i = 0; loc_i < strlen(temp) - 1; loc_i++) {
		if (!isdigit(temp[loc_i])) //Checks if Input number is a digit
			return WRONG_PAN;
	}
	temp[19] = '\0';
	strcpy(cardData->primaryAccountNumber, temp);
	return CARD_OK;
}