#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//tenant structure
typedef struct tenantTag{
	char firstName[30];
	char lastName[30];
	char mobileNum[12];
	struct tenantTag *nextTenant;
}Tenant;

//unit structure
typedef struct unitTag{
	char unitName[30];
	struct tenantTag *tenants;
	struct unitTag *next;
	int tenantMaxNumber;
	int tenantCount;	
}Unit;

//main menu of the program
void menu(){
	printf("\t=====MENU=====\n");
	printf("[1] ADD UNIT\n");
	printf("[2] ADD TENANT\n");
	printf("[3] SEARCH TENANT\n");
	printf("[4] MOVE TENANT\n");
	printf("[5] REMOVE TENANT\n");
	printf("[6] EDIT TENANT\n");
	printf("[7] PRINT MASTERLIST OF OCCUPANTS\n");
	printf("[8] PRINT LIST OF OCCUPANTS PER UNIT\n");
	printf("[9] REMOVE UNIT\n");
	printf("[0] EXIT\n");
	printf("\nenter choice: ");
}

//prints all the units 
void printUnit(Unit *head){
	Unit *temp;

  	temp = head;
  	while (temp != NULL) {
    	printf("\tUnit name: %s\n", temp->unitName);
    	temp = temp->next;
  	}
  	printf("\n");
}

//prints all the tenants
void printTenants(Tenant *head){
	Tenant *temp;

  	temp = head;
  	printf("\tTenants:\n");
  	while (temp != NULL) {
    	printf("\t\t%s, ",temp->lastName);
    	printf("%s - ",temp->firstName);
    	printf("%s",temp->mobileNum);
    	printf("\n");
    	temp = temp->nextTenant;
    	}
  	printf("\n");
}

//adds a unit
void addUnit(Unit **unitptr, char name[],int numberOfTenants){
	Unit *temp;
	Unit *add = (Unit *) malloc(sizeof(Unit));	
	strlwr(name);
	strcpy(add->unitName,name);
	add->tenantMaxNumber = numberOfTenants;
	add->tenantCount = 0;
	add->tenants = NULL;
	add->next = NULL;

	//add at head
	if(*unitptr != NULL){
		temp = *unitptr;
		
		if(strcmp(temp->unitName,add->unitName)>0){
			add->next = *unitptr;
			*unitptr = add;
		}
		else{
			while(temp->next != NULL && strcmp(temp->unitName,add->unitName) <0 && strcmp(temp->next->unitName,add->unitName)< 0){
				temp = temp->next;
			}
			//add at tail
			if(temp->next == NULL){
				temp->next = add;
			}
			//add at middle
			else{
				add->next = temp->next;
				temp->next = add;
			}

		}
	}
	else{
		*unitptr = add;
	}

}

//insert tenant
void insertTenant(Tenant **tenant, char firstName[], char lastName[], char mobileNum[]){
	Tenant *temp = *tenant;
	Tenant *add = (Tenant *) malloc(sizeof(Tenant));
	add->nextTenant = NULL;
	strcpy(add->firstName, firstName);
	strcpy(add->lastName,lastName);
	strcpy(add->mobileNum, mobileNum);

	char addCompleteName[60]; 
	char tempCompleteName[60];
	char nextCompleteName[60];
	addCompleteName[0] = '\0'; 
	tempCompleteName[0] = '\0';
	nextCompleteName[0] = '\0';

	strcat(addCompleteName,add->lastName);
	strcat(addCompleteName,add->firstName);

	if(temp != NULL){
		strcat(tempCompleteName,temp->lastName);
		strcat(tempCompleteName,temp->firstName);
	}

	if(temp != NULL){
		//add at head
		if(strcmp(tempCompleteName,addCompleteName)>0){
			add->nextTenant = temp;
			*tenant = add;
		}
		else{
			while(temp->nextTenant != NULL){
				strcat(nextCompleteName,temp->nextTenant->lastName);
				strcat(nextCompleteName,temp->nextTenant->firstName);
				if(strcmp(tempCompleteName,addCompleteName) <0 && strcmp(nextCompleteName,addCompleteName) < 0){
					temp = temp->nextTenant;
				}
				else{
					break;
				}
			}
			//add at tail
			if(temp->nextTenant == NULL){
				temp->nextTenant = add;
			}
			//add at middle
			else{
				add->nextTenant = temp->nextTenant;
				temp->nextTenant = add;
			}
		}
	}
	else{
		*tenant = add;
	}
	strcpy(nextCompleteName,"\0");
	strcpy(addCompleteName,"\0");
	strcpy(tempCompleteName,"\0");
}

//main function for adding a tennant
void addTenant(Unit **unitptr,char name[], char firstName[], char lastName[], char mobileNum[]){
	Unit *temp = *unitptr;
	strlwr(name);
	//finds the inputted unit name
	if(strcmp(name, temp->unitName) == 0){
		//if unit is at the head
		if(temp->tenantCount<temp->tenantMaxNumber){
			insertTenant(&temp->tenants,firstName,lastName,mobileNum);
			temp->tenantCount++;
		}
		else{
			printf("max number has been reached!\n");
		}
		
	}
	//if unit is at the middle or end
	else{
		while(temp->next != NULL && strcmp(name, temp->unitName) != 0){
			temp = temp->next;
		}
		if(strcmp(name, temp->unitName) == 0){
			if(temp->tenantCount<temp->tenantMaxNumber){
				insertTenant(&temp->tenants,firstName,lastName,mobileNum);
				temp->tenantCount++;
				printf("%d\n", temp->tenantCount);
			}
			else{
				printf("max number has been reached\n");
			}
		}
		else{
			printf("the input unit name does not exists.\n");
		}

	}
}

//finds if a tenants exists
Tenant * searchInsideTenant(Tenant *ptr, char first[], char last[]){
	Tenant *temp = ptr;
	while(temp != NULL){
		if(strcmp(temp->firstName,first) == 0 && strcmp(temp->lastName,last) == 0){
			return temp;
		}
		temp = temp->nextTenant;
	}
	return NULL;

}

//main function of searching tennant
Tenant * searchTenant(Unit **unitptr, char searchFirst[],char searchLast[]){
	Unit *temp = *unitptr;
	Tenant *tenantPointer = NULL;

	while (temp != NULL){
		if(temp->tenants != NULL){
			tenantPointer = searchInsideTenant(temp->tenants,searchFirst,searchLast);
			if(tenantPointer != NULL){
				return tenantPointer;
			}
		}
		temp = temp->next;
	}
	return NULL;
}



//prints all available units 
void printAvailableUnit(Unit **unitptr){
	Unit *temp = *unitptr;
	printf("Available Units: \n");
	while(temp != NULL){
		if(temp->tenantCount < temp->tenantMaxNumber){
			printf("\tUnit name: %s \n", temp->unitName);
			printf("\tAvailable tenant: %d\n", temp->tenantMaxNumber - temp->tenantCount );
		}
		temp = temp->next;
	}
}

//removes tenant 
void removeTenant(Unit **ptr,char name[],Tenant *tenant){
	Unit *temp = *ptr;
	Tenant *tempTenant = NULL;
	Tenant *deleteTenant = NULL;
	//finds the unit name
	while(temp != NULL && strcmp(name,temp->unitName) != 0){
		temp = temp->next;
	}
	tempTenant = temp->tenants;

	//delete at head
	if(strcmp(tempTenant->firstName,tenant->firstName) == 0 && strcmp(tempTenant->lastName, tenant->lastName) == 0){
		deleteTenant = tempTenant->nextTenant;
		temp->tenants = deleteTenant;
		free(tempTenant);
	}
	else{
		while(strcmp(tempTenant->firstName,tenant->firstName) == 0 && strcmp(tempTenant->lastName, tenant->lastName) == 0 && tempTenant->nextTenant->nextTenant != NULL){
			tempTenant = tempTenant->nextTenant;	
		}
		
		//delete at tail
		if(tempTenant->nextTenant->nextTenant == NULL){
			deleteTenant = tempTenant->nextTenant;
			tempTenant->nextTenant = NULL;
			free(deleteTenant);
		}

		//delete at middle
		else if(tempTenant->nextTenant->nextTenant != NULL){
			deleteTenant = tempTenant->nextTenant;
			tempTenant->nextTenant = deleteTenant->nextTenant;
			free(deleteTenant);
		}
	}

	temp->tenantCount--;
}

//edits the mobile number of tenant
void editTenant(Tenant **ptr,char mobileNum[]){
	Tenant *temp = *ptr;
	strcpy(temp->mobileNum,mobileNum);
}

//prints all the units and its tenants
void printMasterList(Unit **ptr){
	Unit *temp = *ptr;
		while (temp != NULL) {
		printf("\n");
    	printf("Unit name: %s \n", temp->unitName);
    	printTenants(temp->tenants);
    	temp = temp->next;
  	}
  	printf("\n");

}

//searches the unit if exists with unit name parameter
Unit * unitSearch(Unit **ptr, char name[]){
	Unit *temp = *ptr;
	while(temp != NULL){
		if(strcmp(temp->unitName,name) != 0){
			temp = temp->next;
		}
		else{
			return temp;
		}
	}
	return NULL;
}

//prints all the tenants of unit if exists
void printUnitOccupants(Unit **ptr, char name[]){
	Unit *temp = *ptr;
	Unit *searchedUnit = NULL;
	searchedUnit = unitSearch(ptr,name);
	if(searchedUnit != NULL){
		printTenants(searchedUnit->tenants);
	}
	else printf("Unit name does not exists!\n");
}

//searches the unit of a tenant
Unit * findUnit(Unit **ptr,char firstName[],char lastName[]){
	Unit *temp = *ptr;
	Tenant *tenant = NULL;
	while(temp !=NULL){
		tenant = searchInsideTenant(temp->tenants,firstName,lastName);
		if(tenant != NULL){
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

//moves tenant to other unit
void moveTenant(Unit *units, Tenant *tenant){
	Unit *temp = units;
	char name[30];
	Unit *unitMovingTenant = findUnit(&units,tenant->firstName,tenant->lastName);
	printf("%s\n", unitMovingTenant->unitName);
	char moveFirstName[30];
	char moveLastName[30];
	char moveMobileNum[12];
	strcpy(moveFirstName,tenant->firstName);
	strcpy(moveLastName,tenant->lastName);
	strcpy(moveMobileNum,tenant->mobileNum);
	
	printAvailableUnit(&units);
	printf("Enter unit name to move: ");
	fgets(name, 30,stdin);
	name[strlen(name) - 1] = '\0';
	addTenant(&temp,name,moveFirstName,moveLastName,moveMobileNum);
	removeTenant(&temp,unitMovingTenant->unitName,tenant);
}

//removes a unit
void removeUnit(Unit **ptr, char name[], Unit **searchedptr){
	Unit *temp = *searchedptr, *tempUnit = *ptr, *deleteUnit = NULL;
	Tenant *move = temp->tenants, *del = NULL;
	char confirm[30];
	char unitName[30];
	//move first and delete tenants
	if(moveTenant !=NULL){
		while(move != NULL){
			printf("do you want to move tenant %s %s to other unit? (Y/N) ",move->firstName,move->lastName);
			fgets(confirm,30,stdin);
			confirm[strlen(confirm) - 1] = '\0';
			if(strcasecmp(confirm, "y") == 0){
				moveTenant(tempUnit,move);
			}
			temp->tenantCount--;
			del = move;
			move = move->nextTenant;
			free(del);
			printf("tenant deleted!\n");
		}
	}

	//delete Unit
	//delete at head
	if(strcmp(tempUnit->unitName,name) == 0){
		deleteUnit = tempUnit;
		*ptr = tempUnit->next;
		free(deleteUnit);
	}
	else{
		while(tempUnit->next->next != NULL && strcmp(tempUnit->next->unitName,name) !=0){
			tempUnit = temp->next;
		}
		//delete at middle
		if(tempUnit->next->next != NULL){
			deleteUnit = tempUnit->next;
			tempUnit->next = deleteUnit->next;
			free(deleteUnit);
		}
		//delete at end
		else{
			deleteUnit = tempUnit->next;
			tempUnit->next = NULL;
			free(deleteUnit);
		}		
	}
}

//checks the input if it is number
int checkInput(char input[]){
	for(int i = 0;i<strlen(input);i++){
		if(isdigit(input[i]) !=0){
			return 0;
		}
	}
	return 1;
}

//validates mobile number
int mobileChecker(char input[]){
	if(strlen(input) <11){
		for(int i = 0;i<strlen(input);i++){
			if(isdigit(input[i]) != 0){
				return 0;
			}
		}

		if(input[0] != '0'){
		return 0;
		}

		if(input[1] != '9'){
			return 0;
		}
	}

	else{
		return 1;
	}
}

//writes all the inputs in a file
void saveTenant(FILE *fp,Unit **ptr,int unitCount){
	FILE *save = fp;
	save = fopen("tenant.txt","w");
	Unit *temp = *ptr;
	Tenant *tempTenant = NULL;
	fprintf(save, "%d\n", unitCount);
	while(temp != NULL){
		fprintf(save, "%s\n%d\n%d\n",temp->unitName, temp->tenantMaxNumber,temp->tenantCount);
		tempTenant = temp->tenants;
		while(tempTenant != NULL){
			fprintf(save, "%s\n%s\n%s\n", tempTenant->firstName,tempTenant->lastName,tempTenant->mobileNum);
			tempTenant = tempTenant->nextTenant;
		}
		temp = temp->next;
	}


	fclose(save);
}

void readTenant(FILE *fp,Unit **ptr,int *unitCount){
	FILE *read = fp;
	Unit *temp = *ptr;
	int numberOfUnits;
	char name[30];
	int count;
	int max;

	char first[30];
	char last[30];
	char num[12];

	read = fopen("tenant.txt","r");
	
	if(read != NULL){
		fscanf(read, "%i\n",&numberOfUnits);
		*unitCount = numberOfUnits;
		for(int i = 0;i<numberOfUnits;i++){
			//for unit
			fgets(name, 30, read);
			name[strlen(name) - 1] = '\0';
			fscanf(read,"%i\n",&max);
			fscanf(read,"%i\n",&count);	
			addUnit(&temp,name,max);
			for(int j = 0;j<count;j++){
				//for tenant
				fgets(first,30,read);
				first[strlen(first) - 1] = '\0';
				fgets(last,30,read);
				last[strlen(last) - 1] = '\0';
				fscanf(read,"%s\n",num);
				addTenant(&temp,name,first,last,num);
			}
			temp = temp->next;
		}
		*ptr = temp;
		printf("\n\ndata loaded successfully\n\n");	
	}
	else{
		printf("Tenant file is Empty\n");
	}
	*ptr = temp;
	fclose(read);
}

//prints all the tenant's info if searched
void printTenantInfo(Tenant *tenantPointer, Unit *units){
	Unit *temp = findUnit(&units,tenantPointer->firstName,tenantPointer->lastName);
	if (tenantPointer != NULL){
		printf("\n\tDetails of the Tenant: \n");
		printf("\tUnit Name: %s\n",temp->unitName );
		printf("\t\tName: %s, ", tenantPointer->lastName);
		printf("%s\n", tenantPointer->firstName );
		printf("\t\tMobile Number: %s\n\n", tenantPointer->mobileNum);
	}
	else printf("the tenant does not exists! \n");
}

void freeMemory(Unit *units){
	Unit *temp = NULL;
	Tenant *tempT = NULL;
	while(units != NULL){
		while(units->tenants != NULL){
			tempT = units->tenants;
			units->tenants = units->tenants->nextTenant;
			free(tempT);
		}
		temp = units;
		units = units->next;
		free(temp);
	}
}