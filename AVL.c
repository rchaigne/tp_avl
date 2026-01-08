#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct sdd_synonymes * T_syn;
struct sdd_synonymes{
    char * cur; 
    int eq;     
    T_syn l; // sag
    T_syn r; // sad
};

void S_init_vide(T_syn * p){
    *p = NULL;
}

bool appartient_a(T_syn p, char * mot){
    if(p == NULL)
        return false;
    int cmp = strcmp(p->cur, mot);
    if(cmp == 0){
        return true;
    }
    else if (cmp > 0){
        return appartient_a(p->l, mot);
    }
    else {
        return appartient_a(p->r, mot);
    }
} 

int hauteur(T_syn p){
    return (p == NULL) ? -1 
                       : p->eq;
}
void maj_hauteur(T_syn p){
    if (p != NULL) {
        int hl = hauteur(p->l);
        int hr = hauteur(p->r);
        if (hl>hr)
            p->eq = 1 + hl;
        else
            p->eq = 1 + hr;
    }
}
T_syn Rr(T_syn p){
    T_syn aux = p->l;
    p->l = aux->r;
    aux->r = p;
    maj_hauteur(p);
    maj_hauteur(aux);
    return aux;
}
T_syn Lr(T_syn p){
    T_syn aux = p->r;
    p->r = aux->l;
    aux->l = p;
    maj_hauteur(p);
    maj_hauteur(aux);
    return aux;
}
int equilibre(T_syn p){
    return (p == NULL) ? 0 
                       : hauteur(p->l) - hauteur(p->r);
}
T_syn equilibrer(T_syn p){
    if (p == NULL) 
        return NULL;
    maj_hauteur(p);
    int f = equilibre(p);

    if (f > 1 && equilibre(p->l) >= 0)
        return Rr(p);

    if (f > 1 && equilibre(p->l) < 0){
        p->l = Lr(p->l);
        return Rr(p);
    }

    if (f < -1 && equilibre(p->r) <= 0)
        return Lr(p);
    
    if (f < -1 && equilibre(p->r) > 0){
        p->r = Rr(p->r);
        return Lr(p);
    }
    return p;
}
T_syn asr(T_syn p, char * mot){
    if (p == NULL){
        T_syn new = malloc(sizeof(*new));
        new->cur = mot;
        new->l = NULL;
        new->r = NULL;
        new->eq = 0;
        return new;
    }
    int cmp = strcmp(p->cur, mot);
    if (cmp == 0)
        return p;
    if (cmp > 0)
        p->l = asr(p->l, mot);
    else
        p->r = asr(p->r, mot);
    return equilibrer(p);
}
void ajout_synonyme(T_syn * p, char * mot){
    *p = asr(*p, mot);
}

typedef struct sdd_dico * T_dico;
struct sdd_dico{
    char * mot; 
    int eq;   
    T_syn s;  
    T_dico l; //sag
    T_dico r; //sad
};

void D_init_vide(T_dico *p){
    *p = NULL;
}

bool D_appartient_a(T_dico p, char * mot){
    if(p == NULL){
        return false;
    }
    int cmp = strcmp(p->mot, mot);
    if(cmp == 0){
        return true;
    }
    else if (cmp > 0){
        return D_appartient_a(p->l, mot);
    }
    else {
        return D_appartient_a(p->r, mot);
    }
} 

int D_hauteur(T_dico p){
    return (p == NULL) ? -1 
                       : p->eq;
}
void D_maj_hauteur(T_dico p){
    if (p != NULL) {
        int hl = D_hauteur(p->l);
        int hr = D_hauteur(p->r);
        if (hl>hr)
            p->eq = 1 + hl;
        else
            p->eq = 1 + hr;
    }
}
T_dico D_Rr(T_dico p){
    T_dico aux = p->l;
    p->l = aux->r;
    aux->r = p;
    D_maj_hauteur(p);
    D_maj_hauteur(aux);
    return aux;
}
T_dico D_Lr(T_dico p){
    T_dico aux = p->r;
    p->r = aux->l;
    aux->l = p;
    D_maj_hauteur(p);
    D_maj_hauteur(aux);
    return aux;
}
int D_equilibre(T_dico p){
    return (p == NULL) ? 0 
                       : D_hauteur(p->l) - D_hauteur(p->r);
}
T_dico D_equilibrer(T_dico p){
    if (p == NULL)
        return NULL;
    D_maj_hauteur(p);
    int f = D_equilibre(p);

    if (f > 1 && D_equilibre(p->l) >= 0)
        return D_Rr(p);

    if (f > 1 && D_equilibre(p->l) < 0){
        p->l = D_Lr(p->l);
        return D_Rr(p);
    }

    if (f < -1 && D_equilibre(p->r) <= 0)
        return D_Lr(p);

    if (f < -1 && D_equilibre(p->r) > 0){
        p->r = D_Rr(p->r);
        return D_Lr(p);
    }
    return p;
}
T_dico D_aer(T_dico p, char * mot, T_syn s){
    if (p == NULL){
        T_dico new = malloc(sizeof(*new));
        new->mot = mot;
        new->s = s;
        new->l = NULL;
        new->r = NULL;
        new->eq = 0;
        return new;
    }
    int cmp = strcmp(p->mot, mot);
    if (cmp == 0)
        return p;
    if (cmp > 0)
        p->l = D_aer(p->l, mot, s);
    else
        p->r = D_aer(p->r, mot, s);
    return D_equilibrer(p);
}
void D_ajout_entree(T_dico *p, char*mot, T_syn s){
    *p = D_aer(*p, mot, s);
}

void charger_dico(T_dico *p){
    FILE *stream = fopen("base_synonymes.txt", "r"); 
    char s[100];
    char * mot;
    T_syn smot = NULL;
    int flag = 1; 
    if (stream != NULL) { 
        while (fgets(s, sizeof(s), stream) != NULL) {
            s[strcspn(s, "\n")] = 0;
            if (strcmp(s, "N_ENT") != 0){
                if (flag){
                    mot = malloc(strlen(s) + 1);
                    strcpy(mot, s);
                    flag = 0;
                }
                else{
                    char * syn = malloc(strlen(s) + 1);
                    strcpy(syn, s);
                    ajout_synonyme(&smot, syn);
                }
            }
            else {
                if (mot != NULL) {
                    D_ajout_entree(p, mot, smot);
                }
                flag = 1;
                mot = NULL;
                smot = NULL;
            }
        }
        fclose(stream);
    } 
    else
        printf("il est ou le fichier...\n");
}

T_syn liste_syn(T_dico p, char * mot){
    while(p != NULL) {
        int cmp = strcmp(p->mot, mot);
        if (cmp == 0)
            return p->s;
        p = (cmp > 0) ? p->l : p->r;
    }
    return NULL;
}

bool est_synonyme_de(T_dico p, char*mot1, char*mot2){
    return appartient_a(liste_syn(p, mot1), mot2);
}

void scr(T_syn * u, T_syn u1, T_syn u2){
    if (u1 == NULL)
        return;
    char * mot = u1->cur;
    T_syn p = u2;
    while(p != NULL){
        int cmp = strcmp(p->cur, mot);
        if (cmp == 0){
            ajout_synonyme(u, mot);
            p = NULL;
        }
        else 
            p = (cmp > 0) ? p->l : p->r;
    }
    scr(u, u1->r, u2);
    scr(u, u1->l, u2);
}
T_syn synonymes_communs(T_dico p, char*mot1, char*mot2){
    T_syn ussr = NULL;
    T_syn s1 = liste_syn(p, mot1);
    T_syn s2 = liste_syn(p, mot2);
    scr(&ussr, s1, s2); 
    return ussr;
}

void fafficher(T_syn p){
    if(p == NULL)
        return;
    printf("\033[95m%s\033[0m\n", p->cur);
    fafficher(p->l);
    fafficher(p->r);
}

void menu() {
    printf("\n\033[33m***** DICTIONNAIRE DE SYNONYMES *****\033[0m\n");
    printf("\033[32m");
    printf("1. liste des synonymes d'un mot \n");
    printf("2. savoir si deux mots sont synonymes\n");
    printf("3. synonymes communs de deux mots\n");
    printf("4. partir...\n");
    printf("\033[0m");
}

int main(void){
    T_dico DICO = NULL; 
    charger_dico(&DICO);
    int choix;
    char choix2[5];
    char mot1[100];
    char mot2[100];
    do {
        menu();
        fgets(choix2, 5, stdin);
        sscanf(choix2, "%d", &choix);
        switch(choix) {
            case 1:
                printf("le mot, sivouplai : ");
                fgets(mot1, 100, stdin);
                mot1[strcspn(mot1, "\n")] = 0;
                T_syn liste = liste_syn(DICO, mot1);
                if (liste == NULL)
                    printf("\nRATIO\n");
                else  {
                    printf("\nsynonymes du mot %s : \n\n", mot1);
                    fafficher(liste);
                }
                break;
                
            case 2:
                printf("le premier mot : ");
                fgets(mot1, 100, stdin);
                mot1[strcspn(mot1, "\n")] = 0;
                printf("le mot d'apres : ");
                fgets(mot2, 100, stdin);
                mot2[strcspn(mot2, "\n")] = 0;
                if(D_appartient_a(DICO, mot1)){
                    if (est_synonyme_de(DICO, mot1, mot2))
                        printf("\n%s et %s sont de bo synonymes\n", mot1, mot2);
                    else if (strcmp(mot1, mot2) == 0)
                        printf("\nAvez-vous des problemes d'attention ?\n");
                    else
                        printf("\n%s et %s ne sont PAS DU TOUT, mais alors PAS DU TOUT, synonymes\n", mot1, mot2);
                }
                else printf("\nlourd ratio\n");
                break;
                
            case 3:
                printf("mot 1 : ");
                fgets(mot1, 100, stdin);
                mot1[strcspn(mot1, "\n")] = 0;
                printf("mot 2 : ");
                fgets(mot2, 100, stdin);
                mot2[strcspn(mot2, "\n")] = 0;
                T_syn commun = synonymes_communs(DICO, mot1, mot2);
                if (commun == NULL)
                    printf("\nRATIO\n");
                else if (strcmp(mot1, mot2) == 0)
                    printf("\nveuillez utiliser l'option 1 pour ce type de requette\n");
                else  
                    printf("\n%s et %s ont pour synonymes communs...\n\n", mot1, mot2);
                    fafficher(commun);
                break;
                
            case 4:
                printf("c'est la fin du voyage...\n");
                break;
                
            default:
                printf("\ncertainement pas non, et puis quoi encore\n");
        }
    } while(choix != 4);
    
    return 0;
}