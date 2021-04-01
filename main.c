#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project.h"

int main(){
	char choice = 'a';
	int checker;
	int unitCounter = 0;
	char tempChoice[30];
	char tempNumber[30];
	Unit *units = NULL, *searchedUnit = NULL;
	Tenant *tenant = NULL;
	FILE *fp;

	int numberOfTenants; //check if input is number
	char name[30];
	char firstName[30];
	char lastName[30];
	char mobileNum[12];
	char searchFirst[30];
	char searchLast[30];
	char confirm[10];
	int count;
	int max;

	fp = fopen("tenant.txt","r");

	//read files
	if(fp != NULL){
		fscanf(fp, "%i\n",&unitCounter);
		for(int i = 0;i<unitCounter;i++){
			//for unit
			fgets(name, 30, fp);
			name[strlen(name) - 1] = '\0';
			fscanf(fp,"%i\n",&max);
			fscanf(fp,"%i\n",&count);	
			addUnit(&units,name,max);
			for(int j = 0;j<count;j++){
				//for tenant
				fgets(firstName,30,fp);
				firstName[strlen(firstName) - 1] = '\0';
				fgets(lastName,30,fp);
				lastName[strlen(lastName) - 1] = '\0';
				fscanf(fp,"%s\n",mobileNum);
				addTenant(&units,name,firstName,lastName,mobileNum);
			}
		}
		printf("\n\ndata loaded successfully\n\n");	
	}
	else{
		printf("Tenant file is Empty\n");
	}
	fclose(fp);

	while(strcmp(tempChoice,"0") != 0){
		//shows menu
		menu();
		//menu selection
		fgets(tempChoice,30,stdin);
		tempChoice[strlen(tempChoice) - 1] = '\0';
		//add units
		if(strcmp(tempChoice,"1") == 0){
			printf("enter unit name: ");
			fgets(name,30,stdin);
			name[strlen(name) - 1] = '\0'; 
			printf("enter max number of tenants: ");
			scanf("%s",tempNumber);
			searchedUnit = unitSearch(&units,name);
			checker = checkInput(tempNumber);

			if(searchedUnit == NULL){
				if(checker == 0){
				numberOfTenants =  atoi(tempNumber);
				addUnit(&units,name,numberOfTenants);
				unitCounter++;
				}
				else printf("invalid number of tenants\n");
			}
			else{
				printf("unit already exits!\n");
			}
			
			
			getchar();
		}
		//add tenant
		else if(strcmp(tempChoice,"2") == 0){
			printf("enter unit name where the tenant will be added: ");
			fgets(name,30,stdin);
			name[strlen(name) - 1] = '\0'; 
			printf("enter first name: ");
			fgets(firstName,30,stdin);
			firstName[strlen(firstName) - 1] = '\0';
			printf("enter last name: ");
			fgets(lastName,30,stdin);
			lastName[strlen(lastName) - 1] = '\0';
			printf("enter 11-digit mobile number: ");
			scanf("%s",mobileNum);
			int checker = mobileChecker(mobileNum);
			if(checker == 0){

				printf("invalid mobile number!\n");
				getchar();
				continue;
			}

			tenant = searchTenant(&units,firstName,lastName);
			if(tenant == NULL){
				addTenant(&units,name,firstName,lastName,mobileNum);
				printTenants(units->tenants);
			}
			else{
				printf("Tenant already exits!\n");
			}
			getchar();
		}
		//search tenant
		else if(strcmp(tempChoice,"3") == 0){
			printf("enter first name of tenant: ");
			fgets(searchFirst,30,stdin);
			searchFirst[strlen(searchFirst) - 1] = '\0';
			printf("enter last name of tenant: ");
			fgets(searchLast,30,stdin);
			searchLast[strlen(searchLast) - 1] = '\0';
			tenant = searchTenant(&units,searchFirst,searchLast);
			if(tenant != NULL){
				printTenantInfo(tenant,units);
			}
			else{
				printf("Tenant does not exists!\n");
			}
		}
		//move tenant
		else if(strcmp(tempChoice,"4") == 0){
			printf("enter first name: ");
			fgets(firstName,30,stdin);
			firstName[strlen(firstName) - 1] = '\0';
			printf("enter last name: ");
			fgets(lastName,30,stdin);
			lastName[strlen(lastName) - 1] = '\0';
			tenant = searchTenant(&units,firstName,lastName);
			if (tenant != NULL){
				moveTenant(units,tenant);
				/*searchedUnit = findUnit(&units,tenant->firstName,tenant->lastName);
				printAvailableUnit(&units);
				printf("Enter unit name to move: ");
				fgets(name, 30,stdin);
				name[strlen(name) - 1] = '\0';
				addTenant(&units,name,tenant->firstName,tenant->lastName,tenant->mobileNum);
				removeTenant(&units,searchedUnit->unitName,tenant);*/
			}
			else printf("Tenant does not exists! \n");
		}
		//remove tenant
		else if(strcmp(tempChoice,"5") == 0){
			printf("enter first name: ");
			fgets(firstName,30,stdin);
			firstName[strlen(firstName) - 1] = '\0';
			printf("enter last name: ");
			fgets(lastName,30,stdin);
			lastName[strlen(lastName) - 1] = '\0';
			tenant = searchTenant(&units,firstName,lastName);
			if (tenant != NULL){
				searchedUnit = findUnit(&units,tenant->firstName,tenant->lastName);
				printf("%s\n",searchedUnit->unitName);
				printf("Do you really want to delete tenant %s %s? (Y/N): ", tenant->firstName, tenant->lastName);

				fgets(confirm,30,stdin);
				confirm[strlen(confirm) - 1] = '\0';
				if(strcasecmp(confirm,"y") == 0){
					removeTenant(&units,searchedUnit->unitName,tenant);
					printf("Tenant deleted successfully!\n");
				}
				else{
					printf("Action terminated.\n");
					continue;
				}
			}
			else printf("Tenant does not exists! \n");
		}
		//edit tenant
		else if(strcmp(tempChoice,"6") == 0){
			printf("enter first name: ");
			fgets(firstName,30,stdin);
			firstName[strlen(firstName) - 1] = '\0';
			printf("enter last name: ");
			fgets(lastName,30,stdin);
			lastName[strlen(lastName) - 1] = '\0';
			tenant = searchTenant(&units,firstName,lastName);
			if(tenant !=NULL){
				printf("enter new mobile number: ");
				scanf("%s",mobileNum);
				int checker = mobileChecker(mobileNum);
				if(checker == 0){

					printf("invalid mobile number!\n");
					continue;
				}
				editTenant(&tenant,mobileNum);
				printf("Tenant edited successfully! \n");
				getchar();
			}
			else printf("Tenant does not exists! \n");
		}
		//prints master list
		else if(strcmp(tempChoice,"7") == 0){
			if(units != NULL){
				printMasterList(&units);
			}
			else printf("the Unit list is empty\n");
		}

		//prints all the tenants in a unit
		else if(strcmp(tempChoice,"8") == 0){
			if(units!= NULL){
				printf("enter unit name: ");
				fgets(name,30,stdin);
				name[strlen(name) - 1] = '\0'; 
				printUnitOccupants(&units,name);
			}
			else printf("the Unit list is empty\n" );
		}
		//removes a unit
		else if(strcmp(tempChoice,"9") == 0){
			if(units!=NULL){
				printf("enter unit name: ");
				fgets(name,30,stdin);
				name[strlen(name) - 1] = '\0';
				searchedUnit = unitSearch(&units,name);
				if(searchedUnit != NULL){
					printf("do you really want to remove unit %s? (Y/N) ", searchedUnit->unitName);
					fgets(confirm,30,stdin);
					confirm[strlen(confirm) - 1] = '\0';
					if(strcasecmp(confirm,"y") == 0){
						removeUnit(&units,name,&searchedUnit);
						printf("Unit removed successfully! \n");
						unitCounter--;
					}
					else{
						break;
					}
				}
				else printf("unit does not exists!\n");
			}
			else printf("the Unit list is empty\n");
		}
		//exits the program
		else if(strcmp(tempChoice,"0") == 0){
			saveTenant(fp,&units,unitCounter); //saves data into file
			//free memory
			freeMemory(units);
			free(units);
			printf("BYE!!!\n");
		}
		else printf("invalid input! \n");
	}

	return 0;
}