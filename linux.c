#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300
#define IS_FILE 1
#define IS_DIR 2
#define SPACE_FOR_LEVEL 4

struct Dir;
struct File;

typedef struct Dir {
    char *name; 
    struct Dir* parent; 
    struct File* head_children_files; 
    struct Dir* head_children_dirs; 
    struct Dir* next; 
} Dir; 

typedef struct File {
    char *name; 
    struct Dir* parent; 
    struct File* next; 
} File;

/*
* Functia aloca o structura de tip director si
* initializeaza campurile acesteia.
*/
Dir *alocDir(char *name) {
    Dir *dir = (Dir *)calloc(1, sizeof(Dir));
    if (!dir)
        return NULL;

    dir->name = calloc(strlen(name) + 1, sizeof(char));
    if (!dir->name) {
        free(dir);
        return NULL;
    }
    strcpy(dir->name, name);

    dir->parent = NULL;
    dir->head_children_dirs = NULL;
    dir->head_children_files = NULL;
    dir->next = NULL;

    return dir;
}

/*
* Functia aloca o structura de tip fisier si
* initializeaza campurile acesteia.
*/
File *alocFile(char *name)
{
    File *file = (File *)calloc(1, sizeof(File));
    if (!file)
        return NULL;

    file->name = calloc(strlen(name) + 1, sizeof(char));
    if (!file->name) {
        free(file);
        return NULL;
    }
    strcpy(file->name, name);

    file->next = NULL;
    return file;
}

/*
* Functia afiseaza o lista de directoare.
*/
void printDir(Dir *dir) 
{
    for (; dir != NULL; dir = dir->next) {
        printf("%s\n", dir->name);
    }
}

/*
* Functia afiseaza o lista de fisiere.
*/
void printFile(File *file) 
{
    for (; file != NULL; file = file->next) {
        printf("%s\n", file->name);
    }
}

/*
* Functia cauta directorul in lista de directoare.
* Intoarce 0 daca l-a gasit, 1 altfel.
*/
int searchDir(Dir *dir, char *name) 
{
    for (; dir != NULL; dir = dir->next) {
        if (strcmp(dir->name, name) == 0) {
            return 0;
        }
    }
    return 1;
}

/*
* Functia cauta fisierul in lista de fisiere.
* Intoarce 0 daca l-a gasit, 1 altfel.
*/
int searchFile(File *file, char *name) 
{
    for (; file != NULL; file = file->next) {
        if (strcmp(file->name, name) == 0) {
            return 0;
        }
    }
    return 1;
}

/*
* Functia creeaza un fisier nou pe care il insereaza la finalul
* listei de fisiere asociata directorului primit ca parametru.
* Daca fisierul se afla deja in lista, se afiseaza un mesaj corespunzator.
* Lista este parcursa pana la final folosind 2 pointeri, cand se ajunge 
* la finalul listei, pointerul ant va retine ultimul element de care 
* vom lega noua celula de lista. Daca acesta este NULL, inseamna ca se 
* insereaza primul element in lista.
*/
void touch(Dir* parent, char* name) 
{
    if (searchFile(parent->head_children_files, name)) { 
        File *list = parent->head_children_files, *ant = NULL, *aux_file;
        for (; list != NULL; ant = list, list = list->next);

        aux_file = alocFile(name);
        if (!aux_file)
            return;

        if (ant == NULL)
            parent->head_children_files = aux_file;
        else
            ant->next = aux_file;
    } else {
        printf("File already exists\n");
    }
}

/*
* Functia creeaza un director nou pe care il insereaza la finalul
* listei de directoare asociata directorului parinte primit ca parametru.
* Daca directorul se afla deja in lista, se afiseaza un mesaj corespunzator.
* Lista este parcursa pana la final folosind 2 pointeri, cand se ajunge 
* la finalul listei, pointerul ant va retine ultimul element de care 
* vom lega noua celula de lista. Daca acesta este NULL, inseamna ca se 
* insereaza primul element in lista.
*/
void mkdir(Dir* parent, char* name) 
{
    if (searchDir(parent->head_children_dirs, name)) { 
        Dir *dir = parent->head_children_dirs, *ant = NULL, *aux_file;
        for (; dir != NULL; ant = dir, dir = dir->next);

        aux_file = alocDir(name);
        aux_file->parent = parent;
        if (!aux_file)
            return;

        if (ant == NULL) 
            parent->head_children_dirs = aux_file;
        else
            ant->next = aux_file;
    } else {
        printf("Directory already exists\n");
    }
}

/*
* Functia afiseaza continutul directorului parinte, mai intai
* directoarele, apoi fisierele din acesta.
*/
void ls(Dir* parent) 
{
    printDir(parent->head_children_dirs);
    printFile(parent->head_children_files);
}

/*
* Functia elibereaza memoria alocata pentru o lista de fisiere.
*/
void freeFile(File *file) 
{
    File *aux_file;
    
    while (file) {
        aux_file = file;
        file = file->next;

        free(aux_file->name);
        free(aux_file);
    }
    file = NULL;
}

/*
* Functia elimina un fisier din lista asociata directorului parinte.
* Mai intai se verifica daca acesta exista in lista de fisiere,
* altfel se afiseaza un mesaj corespunzator. 
*/
void rm(Dir* parent, char* name) 
{
    if (searchFile(parent->head_children_files, name) == 0) { 
        
        File *list = parent->head_children_files, *ant = NULL;
        if (list != NULL && strcmp(list->name, name) == 0) {
            parent->head_children_files = list->next;
            free(list->name);
            free(list);
            return;
        }

        for (; list != NULL;) {
            if (strcmp(list->name, name) == 0) {
                ant->next = list->next;
                free(list->name);
                free(list);
                return;

            } else {
                ant = list;
                list = list->next;
            }
        }

    } else {
        printf("Could not find the file\n");
    }
}

/*
* Functia elibereaza memoria alocata pentru o lista de directoare
* si pentru fiecare director in parte, elibereaza lista de fisiere 
* si numele acestuia.
*/
void freeDir(Dir *dir) 
{
    Dir *aux_dir;

    while (dir) {
        aux_dir = dir;
        dir = dir->next;
        
        freeFile(aux_dir->head_children_files);
        free(aux_dir->name);
        freeDir(aux_dir->head_children_dirs);
        free(aux_dir);
    }

    dir = NULL;
}

/*
* Functia elimina directorul cu numele primit ca parametru
* din lista de directoare ale directorului parinte.
* Se verifica daca acesta se afla in lista de directoare, daca
* este primul element al listei, se schimba inceputul listei
* si se elibereaza memoria alocata pentru directorul respectiv.
* Altfel, se parcurge lista de directoare folosind si un pointer
* care retine celula precedenta, pana se gaseste directorul dorit,
* Cand s-a gasit, se refac legaturile listei si se elibereaza 
* memoria alocata.
*/
void rmdir(Dir* parent, char* name) 
{
    if (searchDir(parent->head_children_dirs, name) == 0) { 

        Dir *list = parent->head_children_dirs, *ant = NULL;
        if (list != NULL && strcmp(list->name, name) == 0) {
            parent->head_children_dirs = list->next;

            freeFile(list->head_children_files);
            freeDir(list->head_children_dirs);
            free(list->name);
            free(list);
            return;
        }

        for (; list != NULL;) {
            if (strcmp(list->name, name) == 0) {
                ant->next = list->next;

                freeFile(list->head_children_files);
                freeDir(list->head_children_dirs);
                free(list->name);
                free(list);
                return;

            } else {
                ant = list;
                list = list->next;
            }
        }

    } else {
        printf("Could not find the dir\n");
    }
}

/*
* Functia cauta in directorul target subdirectorul in care se doreste
* sa se intre. Daca se gaseste, schimba directorul curent in directorul
* cu numele primit ca parametru. Altfel, se afiseaza un mesaj corespunzator.
* La intalnirea caracterului special "..", se va schimba calea cu directorul parinte 
* al directorului curent.
*/
void cd(Dir** target, char *name) 
{
    if (searchDir((*target)->head_children_dirs, name) == 0) { 
        Dir *list = (*target)->head_children_dirs;

        for (; list != NULL; list = list->next) {
            if (strcmp(name, list->name) == 0)
                break;
        }
        (*target) = list;

    } else if (strcmp(name, "..") == 0) {

        if ((*target)->parent != NULL)
            (*target) = (*target)->parent;
    } else {
        printf("No directories found!\n");
    }
}

/*
* Functia returneaza inversul unui sir de caractere.
*/
void mirror(char *str)
{
    int len = strlen(str);
    int end = len - 1;

    for (int i = 0; i < len / 2; i++) {
        char ch = *(str + i);
        *(str + i) = *(str + end - i);
        *(str + end - i) = ch;
    }
}

/*
* Functia parcurge recursiv ierarhia de directoare, de la cel
* mai indepartat parinte, pentru a reconstitui calea catre directorul
* curent. Pentru fiecare director, se adauga caracterul special "/" in fata
* numelui, pentru a reprezenta ca este un director si se concateneaza
* la sirul primit ca parametru, ce reprezinta calea finala.
*/
void pwd_aux(Dir *target, char *pwd)
{
    if (target == NULL)
        return;

    pwd_aux(target->parent, pwd);
    char *cale = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
    strcpy(cale, target->name);

    mirror(cale);
    strcat(cale, "/");
    mirror(cale);
    
    strcat(pwd, cale);
    free(cale);
}

/*
* Functia returneaza un sir de caractere ce defineste
* calea catre directorul primit ca parametru.
*/
char *pwd (Dir* target) 
{
    char *pwd = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
    pwd_aux(target, pwd);

    return pwd;
}

/*
* Functia parcurge recursiv lista de directoare si fisiere din 
* directorul curent si le afiseaza intr-o structura arborescenta.
* Indentarea se face prin crestere unui contor la fiecare nivel
* al recursivitatii. Dupa ce se afiseaza structura de directoare 
* a directorului curent, se afiseaza lista de fisiere si se trece la 
* urmatorul director din lista.
*/
void tree(Dir* target, int level) 
{
    char space[] = "";
    if (!target)
        return;

    printf("%*s%s\n",SPACE_FOR_LEVEL * level,space, target->name);

    tree(target->head_children_dirs, level + 1);
    
    File *file = target->parent->head_children_files;
    for (; file != NULL; file = file->next) {
        printf("%*s%s\n", level *  SPACE_FOR_LEVEL, space, file->name);
    }
    
    tree(target->next, level);
}

/*
* Functia verifica daca sirul primit ca parametru denumeste
* un director sau fisier din directorul parinte si intoarce
* prin efect lateral tipul acestuia.
*/
void checkMvName(Dir *parent, char *oldname, int *find) 
{
    if (parent == NULL)
        return;

    if (searchDir(parent->head_children_dirs, oldname) == 0) 
        *find = IS_DIR;

	if (searchFile(parent->head_children_files, oldname) == 0)
        *find = IS_FILE;

    checkMvName(parent->next, oldname, find);
}

/*
* Functia insereaza celula de lista la sfarsitul listei de
* fisiere primita ca parametru.
*/ 
void insertFileEnd(File **parent, File *file)
{
	File *list = *parent, *ant = NULL;
	for (; list != NULL; ant = list, list = list->next);

	if (ant == NULL) {
		*parent = file;
	} else {
		ant->next = file;
	} 

    file->next = NULL;
}

/*
* Functia insereaza celula de lista la sfarsitul listei de
* directoare primita ca parametru.
*/ 
void insertDirEnd(Dir **parent, Dir *dir)
{
	Dir *list = *parent, *ant = NULL;
	for (; list != NULL; ant = list, list = list->next);

	if (ant == NULL) {
		*parent = dir;
	} else {
		ant->next = dir;
	} 

    dir->next = NULL;
}

/*
* Functia verifica daca numele oldname exista deja, iar apoi daca numele newname 
* nu este folosit deja, in caz contrar va afisa mesaje de eroare.
* Daca se doreste redenumirea unui fisier, mai intai se schimba numele acestuia,
* apoi se extrage celula corespunzatoare lui din lista de fisiere si
* se insereaza la sfarsitul acesteia.
* In mod asemenator se procedeaza daca se doreste redenumirea unui director.
*/
void mv(Dir* parent, char *oldname, char *newname) 
{
    int find_oldname = 0, find_newname = 0;
	int find = 0;
    checkMvName(parent, oldname, &find_oldname);
    checkMvName(parent, newname, &find_newname);

    if (find_oldname == 0) {
        printf("File/Director not found\n");
        return;
    } else if (find_newname != 0) {
        printf("File/Director already exists\n");
        return;
	}

	if (find_oldname == IS_FILE) {
        File *file = parent->head_children_files, *ant = NULL;
		File *list = parent->head_children_files;

		for (; file != NULL;) {

			if (strcmp(file->name, oldname) == 0) {
				free(file->name);
				file->name = malloc(sizeof(char) * strlen(newname) + 1);
				strcpy(file->name, newname);
			
				if (ant == NULL) {
					parent->head_children_files = file->next;
				} else {
					ant->next = file->next;
				}

				insertFileEnd(&list, file);
				return;

			} else {
				ant = file;
				file = file->next;
			}
		}

	} else if (find_oldname == IS_DIR) {

		Dir *dir = parent->head_children_dirs, *ant = NULL;
		Dir *list = parent->head_children_dirs;

		for (; dir != NULL;) {
			if (strcmp(dir->name, oldname) == 0) {
				free(dir->name);
				dir->name = calloc(strlen(newname) + 1, sizeof(char));
				strcpy(dir->name, newname);
			
                if (dir->next == NULL && ant == NULL) {
                    return;
                }

				if (ant == NULL) { 
					parent->head_children_dirs = dir->next;
				} else {
					ant->next = dir->next;
				}

				insertDirEnd(&list, dir);
				return;

			} else {
				ant = dir;
				dir = dir->next;
			}
		}
	} 
}

/*
* Functia opreste aplicatia si elibereaza memoria alocata.
* Se urca in ierarhia de directoare pana se ajunge la directorul
* /home, apoi se elibereaza recursiv memoria alocata pentru 
* directoare si fisiere.
*/
void stop (Dir* target) 
{
    while (target->parent) {
        target = target->parent;
    }
    freeDir(target);
}

/*
* Se aloca directorul parinte home, iar in functie de comanda introdusa,
* se citesc si restul datelor si se apeleaza functiile implementate 
* mai sus.
*/
int main ()
{
    Dir *home = alocDir("home");
    if (!home)
        return -1;

    char *buffer = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
    if (!buffer)
        return -1;

    do
    {
        scanf("%s", buffer);
        if (strcmp(buffer, "touch") == 0) {
            char *buffer_name = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
            scanf("%s", buffer_name);
            touch(home, buffer_name);
            free(buffer_name);

        } else if (strcmp(buffer, "mkdir") == 0) {
            char *buffer_name = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
            scanf("%s", buffer_name);
            mkdir(home, buffer_name);
            free(buffer_name);

        } else if (strcmp(buffer, "ls") == 0) {
            ls(home);

        } else if (strcmp(buffer, "rm") == 0) {
            char *buffer_name = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
            scanf("%s", buffer_name);
            rm(home, buffer_name);
            free(buffer_name);

        } else if (strcmp(buffer, "rmdir") == 0) {
            char *buffer_name = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
            scanf("%s", buffer_name);
            rmdir(home, buffer_name);
            free(buffer_name);

        } else if (strcmp(buffer, "cd") == 0) {
            char *buffer_name = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
            scanf("%s", buffer_name);
            cd(&home, buffer_name);
            free(buffer_name);

        } else if (strcmp(buffer, "pwd") == 0) {
            char *cale = pwd(home);
            if (!cale)
                return -1;

            printf("%s\n", cale);
            free(cale);

        } else if (strcmp(buffer, "stop") == 0) {
            stop(home);

        } else if (strcmp(buffer, "tree") == 0) {
            tree(home->head_children_dirs, 0);

        } else if (strcmp(buffer, "mv") == 0) {
            char *oldname = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
            if (!oldname)
                return -1;

            char *newname = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
            if (!newname)
                return -1;

            scanf("%s", oldname); 
            scanf("%s", newname); 

            mv(home, oldname, newname);
            free(newname);
            free(oldname);
        }
    } while (strcmp(buffer, "stop") != 0);
    
    free(buffer);
    return 0;
}