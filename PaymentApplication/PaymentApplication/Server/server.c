#include "server.h"
#include<stdio.h>
#include<string.h>
#pragma warning (disable:4996)

ST_accountsDB_t accountsDB[255] = {
	{74317, "1098336017914447272"},
	{50210, "9249001981460233515"},
	{24614, "8068631554425884100"},
	{34944, "3544227951277778870"},
	{66915, "1431209889878681918"},
	{26266, "4947584051717646138"},
	{12366, "1065486947567638746"},
	{94985, "3358117730571319758"},
	{52948, "5946795381425590126"},
	{29087, "5055654547391478750"},
	{26990, "2159020137919637257"}
};

static uint32_t trans_index = 0;

EN_transState_t recieveTransactionData(ST_transaction_t* transaction) {

	if (isValidAccount(&transaction->cardHolderData) == ACCOUNT_NOT_FOUND) { //To Check if the PAN is found in the accountDB array
		transaction->transState = DECLINED_STOLEN_CARD;
		saveTransaction(transaction);
		return DECLINED_STOLEN_CARD;
	}
	if (isAmountAvailable(&transaction->terminalData) == LOW_BALANCE) { //To check if the transaction amount can be removed from the account
		transaction->transState = DECLINED_INSUFFECIENT_FUND;
		saveTransaction(transaction);
		return DECLINED_INSUFFECIENT_FUND;
	}
	if (isBelowMaxAmount(&transaction->terminalData) == EXCEED_MAX_AMOUNT) { //To check if the transaction amount is below max-transaction
		transaction->transState = DECLINED_EXEED_MAX_AMOUNT;
		saveTransaction(transaction);
		return DECLINED_EXEED_MAX_AMOUNT;
	}
	if (saveTransaction(transaction) == SAVING_FAILED) { //To check if the saving isn't failed
		transaction->transState = INTERNAL_SERVER_ERROR;
		return INTERNAL_SERVER_ERROR;
	}
	transaction->transState = APPROVED;
	return APPROVED;
}


EN_serverError_t isValidAccount(ST_cardData_t* cardData) {
	for (uint32_t loc_i = 0; loc_i < 11; loc_i++) {
		if (!strncmp(cardData->primaryAccountNumber, accountsDB[loc_i].primaryAccountNumber, 19)) { //Compares input PAN with the PANs in the accountDB array
			trans_index = loc_i;
			return SERVER_OK;
		}
	}
	return ACCOUNT_NOT_FOUND;
}


EN_serverError_t isAmountAvailable(ST_terminalData_t* terminalData) { 
	if (terminalData->transAmount > accountsDB[trans_index].balance) //To check if the transaction amount can be removed from the account
		return LOW_BALANCE;
	return SERVER_OK;
}


EN_serverError_t saveTransaction(ST_transaction_t* transaction) { //Saving into the transactionDB.txt file
	FILE* ptr;
	ptr = fopen("./TransactionsDB.txt", "a");
	if(!ptr)
		return SAVING_FAILED;
	fprintf(ptr, "##############################TRANSACTION##############################\n\n");
	fprintf(ptr, "-Card Holder Name:- %s", transaction->cardHolderData.cardHolderName);
	fprintf(ptr, "-Card PAN:- %s\n", transaction->cardHolderData.primaryAccountNumber);
	fprintf(ptr, "-Card Expiry Date:- %s\n", transaction->cardHolderData.cardExpirationDate);
	fprintf(ptr, "-Transaction Date:- %s", transaction->terminalData.transactionDate);
	fprintf(ptr, "-Transaction Max Amount:- %.2f\n", transaction->terminalData.maxTransAmount);
	fprintf(ptr, "-Transaction Amount:- %.2f\n", transaction->terminalData.transAmount);
	if (!(transaction->transState == DECLINED_STOLEN_CARD)) {
		fprintf(ptr, "-Balance BEFORE Transaction = %.2f\n", accountsDB[trans_index].balance);
		if (!(transaction->transState == DECLINED_EXEED_MAX_AMOUNT) && !(transaction->transState == DECLINED_INSUFFECIENT_FUND))
			accountsDB[trans_index].balance -= transaction->terminalData.transAmount;
		fprintf(ptr, "-New Balance = % .2f\n", accountsDB[trans_index].balance);
	}
	fprintf(ptr, "-Transaction State:- ");
	if (transaction->transState == DECLINED_INSUFFECIENT_FUND)
		fprintf(ptr, "DECLINED_INSUFFECIENT_FUND\n");
	else if (transaction->transState == DECLINED_STOLEN_CARD)
		fprintf(ptr, "DECLINED_STOLEN_CARD\n");
	else if (transaction->transState == DECLINED_EXEED_MAX_AMOUNT)
		fprintf(ptr, "DECLINED_EXEED_MAX_AMOUNT\n");
	else
		fprintf(ptr, "APPROVED\n");
	fprintf(ptr, "\n#######################################################################\n\n");
	fclose(ptr);
	return SERVER_OK;
}

