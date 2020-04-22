//
// Created by Béranger Quintana | 3AE E
//

#ifndef PROGC_BAL_H
#define PROGC_BAL_H
//---------------------------------------------------
//---------------Structure Liste---------------------
//---------------------------------------------------

typedef struct BAL
{
    int num_bal ;
    struct LETTRE* lfirst ;
    struct LETTRE* lcurrent ;
    struct LETTRE* llast ;
    struct BAL* suiv ;
}BAL;

typedef struct LETTRE
{
    int  num;
    char* message ;
    struct LETTRE* suiv ;
};

typedef struct LISTE {
    struct BAL * bal_premier ;
    struct BAL * bal_dernier ;
    struct BAL * bal_courant ;
}LISTE_BAL;
//---------------------------------------------------
//              GESTION BAL

LISTE_BAL* initialiser_bal(){

    LISTE_BAL liste ;
    liste = (LISTE_BAL)malloc(sizeof(struct LISTE));
    liste->bal_premier = NULL;
    liste->bal_dernier = NULL;
    liste->bal_courant = NULL;
    return liste ;
}

void ajouter_bal(int num, LISTE_BAL * liste){

    BAL ma_bal = (BAL)malloc(sizeof(struct BAL));
    ma_bal->num_bal = num ;
    ma_bal->l_premier=NULL;
    ma_bal->l_dernier=NULL;
    ma_bal->l_courant=NULL;
    ma_bal->bal_suiv = NULL;


    if (liste == NULL)
    {
        liste->bal_premier = ma_bal ;
        liste->bal_dernier = ma_bal ;
    }
    else {
        liste->bal_dernier->bal_suiv = ma_bal ;
        liste->bal_dernier = ma_bal ;
    }
    liste->bal_courant = ma_bal ;
}


LISTE_BAL*


#endif //PROGC_BAL_H
