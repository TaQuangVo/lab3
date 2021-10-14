#include <stdio.h>
#include <string.h>

#define MAXSTRINGLENGTH 20
#define MAXLISTLENGTH 1000

typedef struct {
    int index;
    char name[MAXSTRINGLENGTH];
    int massNr;
    double hTidInSecond;
}Substance;
typedef struct {
   Substance substances[MAXLISTLENGTH];
   int length; 
}Data;

void initData(Data * data, char fileName []);
void showHomeMeny();
void registerSubstances(Data * data);
void showAllSubstances(Data data);
void sortSubstances(Data data);
int searchSubstances(Data data);
void modifySubstances(Data *data, int isDeleting);
void saveData(Data data, char fileName []);

int main(){
    Data data;
    char filnamn [MAXSTRINGLENGTH];
    printf("\e[1;1H\e[2J");
    printf("Ange filnamn: ");
    scanf("%s",filnamn);
    initData(&data, filnamn);
    
    int input;
    printf("\e[1;1H\e[2J");
    while (1){
        showHomeMeny();
        printf("\nAnge Val: ");
        scanf("%i", &input);
        
        switch (input){
        case 1:
            registerSubstances(&data);
            break;
        case 2:
            printf("Alla amnen\n\n");
            showAllSubstances(data);
            break;
        case 3:
            sortSubstances(data);
            break;
        case 4:
            while (1){
                int searchReturn = searchSubstances(data);
                if(searchReturn == -99) break;
            }break;
        case 5:
            modifySubstances(&data, 0);
            break;
        case 6:
            modifySubstances(&data, 1);
            break;
        case 7:
            saveData(data, filnamn);
            printf("Data sparas och program avslutar...");
            return 0;
        default:
            printf("Välj från 1-7\n");
            break;
        }
    }return 0;
}
//programs dependencies
int handleNameInput(Data * data){
    printf("Ange namn(q for att avsluta): ");
    scanf("%s", data->substances[data->length].name);
    if(strcmp(data->substances[data->length].name, "q") == 0) return -1;
    return 0;
}
int handleMassNrInput(Data * data){
    printf("Ange masstal: ");
    scanf("%d", &data->substances[data->length].massNr);
    return 0;
}
int handleHTidInput(Data * data){
    printf("Halveringstid (s,min,h,dagar,ar): ");
    double number;
    char unit [5];
    scanf("%lf %s", &number, unit);
    if(strcmp(unit,"s") == 0 || strcmp(unit,"") == 0) data->substances[data->length].hTidInSecond = number;
    else if(strcmp(unit,"min") == 0) data->substances[data->length].hTidInSecond = number * 60;
    else if(strcmp(unit,"h") == 0) data->substances[data->length].hTidInSecond = number * 60 * 60;
    else if(strcmp(unit,"dagar") == 0) data->substances[data->length].hTidInSecond = number * 60 * 60 * 24;
    else if(strcmp(unit,"ar") == 0) data->substances[data->length].hTidInSecond = number * 60 * 60 * 24 * 350; 
    else return 1;
    return 0;
}
void swapSubstancesPlace(Substance *a, Substance *b) {
    Substance t = *a;
    *a = *b;
    *b = t;
}
int partition(Substance substances[], int low, int high, int fieldComparing) {
    Substance pivot = substances[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (fieldComparing == 1) {
            int compareToPivot = strcmp(substances[j].name, pivot.name);
            if(compareToPivot <= 0){
                i++;
                swapSubstancesPlace(&substances[i], &substances[j]);
            }
        } else if(fieldComparing == 2){
            if(substances[j].massNr <= pivot.massNr){
                i++;
                swapSubstancesPlace(&substances[i], &substances[j]);
            }
        }else if(fieldComparing == 3){
            if(substances[j].hTidInSecond <= pivot.hTidInSecond){
                i++;
                swapSubstancesPlace(&substances[i], &substances[j]);
            }
        }
    }
    swapSubstancesPlace(&substances[i + 1], &substances[high]);
    return i + 1;
}
void quickSortSubstances(Substance substances[], int low, int high, int sortField) {
    if (low < high) {
        int pi = partition(substances, low, high, sortField);
        quickSortSubstances(substances, low, pi - 1, sortField);
        quickSortSubstances(substances, pi + 1, high, sortField);
    }
}
int search(Data data, char searchName[], int searchMassNr, int min, int max, int searchField){
    Data searchReturn;
    searchReturn.length = 0;
    for (int i = 0; i < data.length; i++){
        if(searchField == 1){
            if(strstr(data.substances[i].name,searchName) != NULL){
                searchReturn.substances[searchReturn.length] = data.substances[i];
                searchReturn.length = searchReturn.length + 1;
            }
        }else if(searchField == 2){
            if(data.substances[i].massNr == searchMassNr){
                searchReturn.substances[searchReturn.length] = data.substances[i];
                searchReturn.length = searchReturn.length + 1;
            }
        }else if(searchField == 3){
            if(data.substances[i].hTidInSecond <= max && data.substances[i].hTidInSecond >= min){
                searchReturn.substances[searchReturn.length] = data.substances[i];
                searchReturn.length = searchReturn.length + 1;
            }
        }
    }
    showAllSubstances(searchReturn);
    if(searchReturn.length == 1) return searchReturn.substances[0].index;
    return -1;
}
void changeHtime(Data * data, int index){
    printf("Ange ny halveringstid i sekunder: ");
    double newHtid;
    scanf("%lf", &newHtid);
    data->substances[index].hTidInSecond = newHtid;
}
void deleteSubstance(Data * data, int index){
    for (int i = index; i < data->length; i++)
    {
        data->substances[i] = data->substances[i+1];
        data->substances[i].index = i;
    }
    data->length = data->length - 1;
}
int isDataExit(Data data, Substance substance){
    for(int i = 0; i < data.length; i++)
        if(strcmp(data.substances[i].name, substance.name) == 0 && data.substances[i].massNr == substance.massNr) return 1;
    return 0;
}
//programs functions
void initData(Data * data, char fileName []){
    const char * fromFileFormat ="{\n\tindex: %d, \n\tname: \"%[^\"]\", \n\tmassRr: %d, \n\thTime: %lf\n}\n";
    FILE * file;
    file = fopen(fileName, "r");

    if(file != NULL) fscanf(file,"{\n\tfileLength: %d\n}\n", &data->length);
    else data->length = 0;

    for (int i = 0; i < data->length; i++)
        fscanf(file, fromFileFormat, &data->substances[i].index, data->substances[i].name, &data->substances[i].massNr, &data->substances[i].hTidInSecond);
    if(file != NULL) fclose(file);
}
void showHomeMeny(){
    printf("Laboration 3 - Ta Quang Vo\n\n");
    printf("(1) Registrera nya radioaktiva amnen\n");
    printf("(2) Alla amnen\n");
    printf("(3) Sortera amnen\n");
    printf("(4) Soka efter amnen\n");
    printf("(5) Andra halveringstid\n");
    printf("(6) Avregistrera amne\n");
    printf("(7) Spara och avsluta\n");
}
void registerSubstances(Data * data){
    while (1){
        int errors = 0;
        printf("Registrera nya radioaktiva amnen\n\n");
        if(data->length >= MAXLISTLENGTH) {
            printf("Max 1000 amne\n\n");
            break;
        }
        errors += handleNameInput(data);
        if(errors == -1) break;
        errors += handleMassNrInput(data);
        if(isDataExit(*data,data->substances[data->length]) == 0){
            errors += handleHTidInput(data);
            if(errors == 0){
                data->substances[data->length].index = data->length;
                data->length = data->length + 1;
                printf("\n");
            }
            else printf("\nFel inmatningar, testa igen!!!\n\n");
        }else printf("Amnet ar redan registrerat!\n\n");
    }
}
void showAllSubstances(Data data){
    printf("Name\t\tMasstal\tHalveringstid(s)\n");
    printf("------------------------------------------\n");
    if(data.length == 0) printf("\t   !!!Inga amne!!!\n");
    for (int i = 0; i < data.length; i++)
        printf("%s\t\t%d\t%.4e\n", data.substances[i].name,data.substances[i].massNr,data.substances[i].hTidInSecond);
    printf("\n");
}
void sortSubstances(Data data){
    Data sortedData = data;
    printf("Vad vill du sortera efter?\n\n");
    printf("(1) namn\n");
    printf("(2) masstal\n");
    printf("(3) halveringstid\n");
    printf("(4) huvudmeny\n");
    printf("Ange val: ");
    int input;
    scanf("%d",&input);
    if(input == 4) return;
    printf("\n");
    quickSortSubstances(sortedData.substances, 0, data.length - 1, input);
    showAllSubstances(sortedData);
}
int searchSubstances(Data data){
    printf("Hur vill du söka? \n\n");
    printf("(1) namn\n");
    printf("(2) masstal\n");
    printf("(3) halveringstid\n");
    printf("(4) huvudmeny\n");
    printf("Ange val: ");
    int input;
    scanf("%d", &input);

    char searchName[MAXSTRINGLENGTH];
    int searchMassNr, min, max;
    if(input == 4) return -99;
    else if(input == 1){
        printf("Ange namn: ");
        scanf("%s", searchName);
        printf("\n");
    }else if(input == 2){
        printf("Ange masstal: ");
        scanf("%d", &searchMassNr);
        printf("\n");
    }else if(input == 3){
        printf("Ange minsta halveringstid: ");
        scanf("%d", &min);
        printf("Ange största halveringstid: ");
        scanf("%d", &max);
    }
    return search(data,searchName, searchMassNr,min,max,input); 
}
void modifySubstances(Data *data, int isDeleting){
    while (1){
        if(isDeleting == 0)printf("Andra halveringstid\n");
        else if(isDeleting == 1) printf("Avregistrera amne\n\n");
        int searchReturn = searchSubstances(*data);
        if(searchReturn >= 0){
            Substance substanceModifing = data->substances[searchReturn];
            if(isDeleting == 0){
                changeHtime(data,searchReturn);
                printf("%s-%d halveringstid andrades till %lf sekunder!!\n", substanceModifing.name, substanceModifing.massNr, data->substances[searchReturn].hTidInSecond);
                break;
            }else if(isDeleting == 1){
                deleteSubstance(data, searchReturn);
                printf("%s-%d avregistreras\n", substanceModifing.name, substanceModifing.massNr);
                break;
            }
        }else if(searchReturn == -1) printf("Du fick inte exakt ett resultat.\n");
        else if(searchReturn == -99) return;
    }
}
void saveData(Data data, char fileName []){
    const char * toFileFormat = "{\n\tindex: %d, \n\tname: \"%s\", \n\tmassRr: %d, \n\thTime: %.4e\n}\n";
    FILE * file;
    file = fopen(fileName,"w");
    if(file == NULL) return;
    fprintf(file,"{\n\tfileLength: %d\n}\n", data.length);
    for (int i = 0; i < data.length; i++)
        fprintf(file,toFileFormat,data.substances[i].index,data.substances[i].name,data.substances[i].massNr,data.substances[i].hTidInSecond);
    fclose(file);
}