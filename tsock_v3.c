//--------------------------PROG C ET RESEAUX-----------------------
//Nom :    Quintana  -----------------------------------------------
//Prénom : Béranger  -----------------------------------------------
//Groupe : 3AE E     -----------------------------------------------
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
/* pour afficher l'adresse d'un socket */
#include <arpa/inet.h>


/**********************************************************************
Programme Principal
**********************************************************************/

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
  
  	int port = -1; //Numéro de port
  	port =htons(port);
  
  	char *dest ;
  	int lg =30;
  
  	while ((c = getopt(argc, argv, "pn:sul:")) != -1) 
  	{ 
	  
    	switch (c) 
    	{
    		case 'p':
      			if (source == 1) 
      			{
					printf("usage: cmd [-p|-s][-n ##]\n");
					exit(1);
      			}
      			source = 0;
      		break;

    		case 's':
      			if (source == 0) 
      			{
    				printf("usage: cmd [-p|-s][-n ##]\n");
					exit(1) ;
      			}
      			dest=argv[argc-2];
      			source = 1;
      		break;

    		case 'n':
      			nb_message = atoi(optarg);
            if (nb_message>100000)
            {
              printf("Trop d'envois, n> 100 000\n");
              exit (1);
            }
      		break;	

    		case 'u':
      			tcp=0;
      		break;

          case 'l':
            lg = atoi(optarg);
          break;  	  		        

		    default:
    			printf("usage: cmd [-p|-s][-n ##]\n");
      		break;
    	}
  	}

  	if ((port=atoi(argv[argc-1]))!=-1)
    {
      port=htons(port);
    }
  	dest=argv[argc-2];

  	if (source == -1)
    {
    	printf("usage: cmd [-p|-s][-n ##]\n");
      exit(1) ;
    }

  	if (source == 1)
    	printf("SOURCE : ");
  	else
    	printf("PUITS : ");

  	
  	if (tcp==1)
    	printf("Protocole de transport : TCP | ");
    
  	else
    	printf("Protocole de transport : UDP | ");

  	if (nb_message == -1)
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
    	{
			
			printf("Nombre d'envois = %d | ", nb_message);
    	}
    	else
    	{
    		printf("nb de tampons à recevoir = %d | ",nb_message);	
    	}	
    }

    printf("Destinataire : %s\n", dest);


  	//Envoi de message, Source en UDP
  
  	int length_addr_dist;
  	length_addr_dist=sizeof(struct sockaddr_in);

  
  	if(source==1 & tcp==0)
    {
      envoi_UDP(port,nb_message,lg,dest);
    }

  	//PUITS de réception en UDP
  
  	else if (source==0 & tcp==0)
    {
      reception_UDP(port,nb_message,lg);
    }


  	else if (source==0 & tcp==1)
  	{
    	
    	ServeurTCP(port, nb_message, lg);
  	}

  	else if (source==1 & tcp==1)
  	{
    	
    	ClientTCP(port,nb_message,lg,dest);
  	}

	exit(0); 
}


