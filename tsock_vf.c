//--------------------------PROG C ET RESEAUX-----------------------
//                          Nom :    Quintana
//                          Prénom : Béranger
//                             Grp : 3AE E
//------------------------------------------------------------------


/* librairie standard ... */
#include <stdlib.h>
/* pour getopt */
#include <unistd.h>
/* déclaration des types de base */
#include <sys/types.h>
/* constantes relatives aux domaines, types et protocoles */
#include <sys/socket.h>
/* constantes et structures propres au domaine UNIX */
#include <sys/un.h>
/* constantes et structures propres au domaine INTERNET */
#include <netinet/in.h>
/* structures retournées par les fonctions de gestion de la base de
données du réseau */
#include <netdb.h>
/* pour les entrées/sorties */
#include <stdio.h>
/* pour la gestion des erreurs */
#include <errno.h>
/* fonctions de tsock*/
#include "tsock.h"
/* fonctions de BAL */
#include "bal.h"
/* pour afficher l'adresse d'un socket */
#include <arpa/inet.h>


//----------------------------------------------------------------------
//                        Programme Principal
//----------------------------------------------------------------------

void main (int argc, char **argv)
{

  	//Déclarations
  
  	int c;
  	extern char *optarg;
  	extern int optind;
  	int nb_message = -1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
  	int source = -1 ; /* 0=puits, 1=source */
  	int tcp=1; //tcp=1 => tcp ; tcp=0 =>udp
  	struct hostent *hp;
    int bal=-1;
  	int port = -1; //Numéro de port
    int recep=-1;
  	port =htons(port);
  	int nb_lettres=3;
    int nBAL=-1;
  	char *dest ;
  	int lg =30;
  
  	while ((c = getopt(argc, argv, "pn:sul:be:r:")) != -1) 
  	{
    	switch (c) 
    	{
    		case 'p':
      			if (source == 1) 
      			{
                    printf("ATTENTION :\n On ne peut être source et puits en même temps, Veuillez enlever une des deux options -s ou -p\n\n");
					exit(1);
      			}
      			source = 0;
      		break;
    		case 's':
      			if (source == 0) 
      			{
    				  printf("ATTENTION :\n On ne peut être source et puits en même temps, Veuillez enlever une des deux options -s ou -p\n\n");
    				  exit(1) ;
      			}
      			dest=argv[argc-2];
      			source = 1;
      		break;
            case 'b' :
                if (source!=-1)
                {
                    printf("ATTENTION :\nOn ne peut utiliser le service de Boite aux lettres et l'usage classique de TCP|UDP en même temps.\nVeuillez retirer une des options -b ou -s|-p\n\n");
                    exit(1);
                }
                bal=1;
                break;
            case 'e' :
                if (source!=-1)
                {
                    printf("ATTENTION :\nOn ne peut utiliser le service de Boite aux lettres et l'usage classique de TCP|UDP en même temps.\nVeuillez retirer une des options -e ou -s|-p\n\n");
                    exit(1);
                }
                if (recep==1)
                {
                    printf("On ne peut pas être émetteur et récepteur, -r ou -e, pas les deux\n");
                    exit(1);
                }
                bal=1;
                recep=0;
                nBAL = atoi(optarg);
                break;
            case 'r' :
                if (recep==0)
                {
                    printf("On ne peut être émetteur et récepteur en même temps : -e ou -r mais pas les deux\n");
                    exit(1);
                }
                bal=1;
                recep=1;
                nBAL = atoi(optarg);
                break;
    		case 'n':
      			nb_message = atoi(optarg);
                if (nb_message>100000)
                {
                    printf("Trop d'envois, n> 100 000\n");
                    exit (1);
                }
                if (recep==1)
                {
                    printf("ATTENTION :\nVous devez récupérer l'intégralité du contenu de la BAL. Vous ne pouvez donc pas fixer n\n\n");
                    exit(1);
                }
      		break;
    		case 'u':
      			tcp=0;
      			if (bal!=-1)
                {
      			    printf("ATTENTION : \nL'envoi sur BAL ne se fait que via TCP, Veuillez enlever l'option -u\n\n");
      			    exit(1);
                }
      		break;
            case 'l':
                lg = atoi(optarg);
                if (lg>maxsize)
                {
                    printf("Taille des messages supérieure à la limite fixée à %d\n",maxsize);
                }
                if (recep==1)
                {
                    printf("ATTENTION :\nVous devez récupérer l'intégralité du contenu de la BAL. Vous ne pouvez donc pas fixer l\n\n");
                    exit(1);
                }
            break;
		    default:
    			printf("usage: ./tsock [-p|-s] ou [-b|-e|-r] [-n ##]\n");
    			exit(1);
      		break;
    	}
  	}


  	if ((port=atoi(argv[argc-1]))!=-1)
    {
        port=htons(port);
    }

  	dest=argv[argc-2];

  	if (source == -1 & bal ==-1)
    {
    	printf("usage: ./tsock [-p|-s] ou [-b|-e|-r] + [-n ##]\n");
        exit(1) ;
    }

//Service Partie 1 Sans BAL
    if (bal == -1)
    {
        if (source == 1)
            printf("SOURCE : ");
        else
            printf("PUITS : ");

      
        if (tcp==1)
            printf("Protocole de transport : TCP | ");
      
        else
            printf("Protocole de transport : UDP | ");

        if (nb_message == -1)  //Attribution du nombre de message à envoyer/recevoir si absence de -n XX
        {
            if (source == 1)
            {
                nb_message = 10;
                printf("Nombre d'envois : %d | ", nb_message);
            }
            else
                printf("Nombre de receptions : infini | ");
        }

        else
        {
            if (source == 1)
                printf("Nombre d'envois = %d | ", nb_message);
            else
                printf("nb de tampons à recevoir = %d | ",nb_message);
        }

        printf("Destinataire : %s\n", dest);


    //Envoi de message, Source en UDP

        if(source==1 & tcp==0)
            envoi_UDP(port,nb_message,lg,dest);


    //PUITS de réception en UDP
        else if (source==0 & tcp==0)
            reception_UDP(port,nb_message,lg);

    //Envoi de message en TCP
        else if (source==0 & tcp==1)
            ServeurTCP(port, nb_message, lg);

        else if (source==1 & tcp==1)
            ClientTCP(port,nb_message,lg,dest);
    }
    else //Partie 2 : UDP,TCP+BAL
    {
        if (nb_message == -1)  //Attribution du nombre de message à envoyer/recevoir si absence de -n XX
        {
            if (recep == 0)
                nb_message = 10;

        }
        if (bal==1 &recep==-1)
        {
            printf("____________________________________________________________________\n\n");
            printf("                             SERVEUR BAL                            \n");
            printf("____________________________________________________________________\n\n");
            SBAL(port, dest);
        }

        else if (bal==1 & recep ==0)
        {
            printf("____________________________________________________________________\n\n");
            printf("                             EMETTEUR BAL                           \n");
            printf("____________________________________________________________________\n\n");
            EBAL(port, dest,nb_message,lg, nBAL);
        }

        else if(bal==1 &recep == 1)
        {
            printf("____________________________________________________________________\n\n");
            printf("                            RECEPTEUR BAL                           \n");
            printf("____________________________________________________________________\n\n");
            RBAL(port,dest,nBAL);
        }
    }

	exit(0);
}


