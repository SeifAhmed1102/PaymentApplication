#include"application.h"
#include <stdio.h>
#include <string.h>

void appstart(void) {

	ST_cardData_t cardData;
	ST_terminalData_t terminalData;
	ST_transaction_t transactionData;

	while (getCardHolderName(&cardData) == WRONG_NAME)
		printf("\n\t\tWRONG_NAME\n\nRe-");

	while (getCardPAN(&cardData) == WRONG_PAN)
		printf("\n\t\tWRONG_PAN\n\nRe-");

	while (getCardExpiryDate(&cardData) == WRONG_EXP_DATE)
		printf("\n\t\tWRONG_EXP_DATE\n\nRe-");

	while (getTransactionDate(&terminalData) == WRONG_DATE)
		printf("\n\t\tWRONG_DATE\n\nRe-");

	if (isCardExpired(&cardData, &terminalData)) {
		printf("\n\t\tThe Card is Expired.\n\n");
		return;
	}

	while (setMaxAmount(&terminalData) == INVALID_MAX_AMOUNT)
		printf("\n\t\tINVALID_MAX_AMOUNT\n\nRe-");

	while (getTransactionAmount(&terminalData) == INVALID_AMOUNT)
		printf("\n\t\tINVALID_AMOUNT\n\nRe-");


	transactionData.cardHolderData = cardData;
	transactionData.terminalData = terminalData;
	EN_transState_t transStatus = recieveTransactionData(&transactionData);
	if (transStatus == DECLINED_STOLEN_CARD)
		printf("\n\t\tThis Card is Stolen\n\n");
	else if (transStatus == DECLINED_INSUFFECIENT_FUND)
		printf("\n\t\tDECLINED_INSUFFECIENT_FUND\n\n");
	else if (transStatus == INTERNAL_SERVER_ERROR)
		printf("\n\t\tINTERNAL_SERVER_ERROR\n\n");
	else if (transStatus == DECLINED_EXEED_MAX_AMOUNT)
		printf("\n\t\tDECLINED_EXEED_MAX_AMOUNT\n\n");
	else
		printf("\n\t\tAPPROVED...\n");
}