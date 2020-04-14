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
  int lg_local;
  
  while ((c = getopt(argc, argv, "pn:su")) != -1) {
	  
	  
    switch (c) {
    case 'p':
      if (source == 1) {
	printf("usage: cmd [-p|-s][-n ##]\n");
	exit(1);
      }
      source = 0;
      break;

    case 's':
      if (source == 0) {
	printf("usage: cmd [-p|-s][-n ##]\n");
	exit(1) ;
      }
      dest=argv[argc-2];
      source = 1;
      break;

    case 'n':
      nb_message = atoi(optarg);
      break;

    case 'u':
      if (tcp ==0){
	printf("usage : cmd [-p| -s] [-u][-n##]\n");
      }
      tcp=0;
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
  if (tcp==1)
    {
      printf("On utilise TCP\n");
    }
  else
    {printf("On utilise UDP\n");}


  if (source == 1)
    printf("on est dans le source\n");
  else
    printf("on est dans le puits\n");
	

  if (nb_message != -1)
    {
    if (source == 1)
      printf("nb de tampons à envoyer : %d\n", nb_message);
    else
      printf("nb de tampons à recevoir : %d\n", nb_message);
    }
  else
    {
    if (source == 1)
      {
	nb_message = 10 ;
	printf("nb de tampons à envoyer = 10 par défaut\n");
      }
    else
      printf("nb de tampons à recevoir = infini\n");	    

  }


  //Envoi de message, Source en UDP
  
  int length_addr_dist;
  length_addr_dist=sizeof(struct sockaddr_in);


  
  if(source==1 & tcp==0)
    {
      envoi_UDP(nb_message,port,30);
    }

  //PUITS de réception en UDP
  
  else if (source==0 & tcp==0)
    {
      reception_UDP(port,nb_message,30);
    }


  else if (source==0 & tcp==1)
  {
    printf("On utilise TCP en puits\n");
    ServeurTCP(port, nb_message, 30);
  }

  else if (source==1 & tcp==1)
  {
    printf("On utilise TCP en source\n");
    ClientTCP(port,nb_message,30);
  }

  exit(0); 
}


