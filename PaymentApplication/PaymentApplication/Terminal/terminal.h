#ifndef TERMINAL_H
#define TERMINAL_H

#include"../Card/card.h"

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef struct ST_terminalData_t
{
	float transAmount;
	float maxTransAmount;
	uint8_t transactionDate[11]; //  DD/MM/YYYY
}ST_terminalData_t;

typedef enum EN_terminalError_t
{
	TERMINAL_OK,
	WRONG_DATE,
	EXPIRED_CARD, 
	INVALID_AMOUNT, 
	EXCEED_MAX_AMOUNT,
	INVALID_MAX_AMOUNT 
}EN_terminalError_t;

EN_terminalError_t getTransactionDate(ST_terminalData_t*);
EN_terminalError_t isCardExpired(ST_cardData_t*, ST_terminalData_t*);
EN_terminalError_t getTransactionAmount(ST_terminalData_t*);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t*);
EN_terminalError_t setMaxAmount(ST_terminalData_t*);
#endif