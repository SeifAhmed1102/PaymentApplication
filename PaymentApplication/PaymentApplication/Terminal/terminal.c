#pragma warning (disable:4996)
#pragma warning (disable:6031)
#include"terminal.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAXDATE 10

EN_terminalError_t getTransactionDate(ST_terminalData_t* terminalData) {
	if (!terminalData) //Check pointer isn't NULL
		return WRONG_DATE;
	fseek(stdin, 0, SEEK_END); //Ignores past input like '/n'
	uint8_t temp[20];
	uint32_t loc_day, loc_month, loc_year;
	printf("Enter Transaction date [DD/MM/YYYY]:- ");
	fgets(temp, 20, stdin);
	if ((strlen(temp) != MAXDATE + 1) || (temp[2] != '/') || (temp[5] != '/')) //Checks the length of the input date and the dash '/' position
		return WRONG_DATE;
	for (uint32_t loc_i = 0; loc_i < MAXDATE; loc_i++) {
		if (loc_i == 2 || loc_i == 5)
			continue;
		if (!(isdigit(temp[loc_i]))) //Checks the input date is all digit
			return WRONG_DATE;
	}
	loc_day = (temp[0] - '0') * 10 + (temp[1] - '0'); //To Check day is correct
	loc_month = (temp[3] - '0') * 10 + (temp[4] - '0'); //To Check month is correct
	loc_year = (temp[6] - '0') * 1000 + (temp[7] - '0') * 100 + (temp[8] - '0') * 10 + (temp[9] - '0');//To Check year is correct
	if (loc_day <= 0 || loc_day > 31) //Checks for days in all the 31 months between 1 and 31
		return WRONG_DATE;
	if (loc_month <= 0 || loc_month > 12) //Checks for month between 1 and 12
		return WRONG_DATE;
	if (loc_year < 0) //Checks for year
		return WRONG_DATE;
	if (loc_month == 2 && loc_year % 4 == 0) { //Checks for february month for days between 1 and 29 in the leap years
		if(loc_day <= 0 || loc_day > 29)
			return WRONG_DATE;
	}
	if (loc_month == 2 && loc_year % 4 != 0) { //Checks for february month for days between 1 and 28 in the not leap years
		if (loc_day <= 0 || loc_day > 28)
			return WRONG_DATE;
	}
	if (loc_month == 4 || loc_month == 6 || loc_month == 9 || loc_month == 11) { //Checks for days in the months (April, June, September, Novermber) between 1 and 30
		if (loc_day <= 0 || loc_day > 30)
			return WRONG_DATE;
	}
	temp[11] = '\0';
	strcpy(terminalData->transactionDate, temp);
	return TERMINAL_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* terminalData) {
	uint32_t card_month, card_year, transaction_month, transaction_year;
	card_month = (cardData->cardExpirationDate[0] - '0') * 10 + (cardData->cardExpirationDate[1] - '0');
	card_year = (cardData->cardExpirationDate[3] - '0') * 10 + (cardData->cardExpirationDate[4] - '0') + 2000; //Input will be a number of two ex.(25), so I sum 2000 to this number to be like (2025)
	transaction_month = (terminalData->transactionDate[3] - '0') * 10 + (terminalData->transactionDate[4] - '0');
	transaction_year = (terminalData->transactionDate[6] - '0') * 1000 + (terminalData->transactionDate[7] - '0') * 100 + (terminalData->transactionDate[8] - '0') * 10 + (terminalData->transactionDate[9] - '0');
	if ((card_year == transaction_year) && (card_month < transaction_month)) //Compares the two months
		return WRONG_EXP_DATE;
	if(card_year < transaction_year) //Compares the Years
		return WRONG_EXP_DATE;
	return TERMINAL_OK;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* terminalData) {
	float temp = 0;
	printf("Enter transaction amount:- ");
	scanf("%f", &temp);
	if (temp <= 0) //Checks if the Input isn't less than 0
		return INVALID_AMOUNT;
	terminalData->transAmount = temp;
	return TERMINAL_OK;
}


EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* terminalData) {
	float temp_amount, temp_max;
	temp_amount = terminalData->transAmount;
	temp_max = terminalData->maxTransAmount;
	if (temp_amount > temp_max) //Compares transaction amount with max-transaction
		return EXCEED_MAX_AMOUNT;
	return TERMINAL_OK;
}


EN_terminalError_t setMaxAmount(ST_terminalData_t* terminalData) {
	float temp = 0;
	printf("Enter Max transaction amount:- ");
	scanf("%f", &temp);
	if (temp <= 0) //Checks if the Input isn't less than 0
		return INVALID_MAX_AMOUNT;
	terminalData->maxTransAmount = temp;
	return TERMINAL_OK;
}