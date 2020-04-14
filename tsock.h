//--------------------------PROG C ET RESEAUX-----------------------
//Nom :    Quintana  -----------------------------------------------
//Prénom : Béranger  -----------------------------------------------
//Groupe : 3AE E     -----------------------------------------------
//------------------------------------------------------------------

#ifndef __tsock__

//---------------------------------------------------
//-------------Déclaration des fonctions-------------
//---------------------------------------------------

void construire_message(char *message, char motif, int lg, int i);
void afficher_message(char *message, int lg);
void envoi_UDP(int port, int nb_message  , int lg_msg, char*dest);
void reception_UDP(int port, int nb_message , int lg_message);
void ClientTCP (int port, int nb_message , int lg_msg, char* dest);
void ServeurTCP(int port , int nb_message, int lg_msg);
void printbuffer(int n);



//---------------------------------------------------
//-------------Définitions des fonctions-------------
//---------------------------------------------------

//-------------PRINTBUFFER
void printbuffer(int n)
{
    int r;

    if (n<10)
    {
        printf("[----%d",n);
    }
    if (n>=10 & n<100)
    {
        printf("[---%d",n);
    }
    if (n>=100 & n<1000)
    {
        printf("[--%d",n);
    }
    if (n>=1000 & n<10000)
    {
        printf("[--%d",n);
    }
    if (n>=10000 & n<100000)
    {
        printf("[-%d",n);
    }
    if (n>=100000 & n<1000000)
    {
        printf("[%d",n);
    }
    if (n>=1000000)
    {
        printf("Trop de messages à envoyer (n>1000000 \n");
        exit(1);
    }
}
//Construction des messages

void construire_message(char *message, char motif, int lg ,int i)

{
	char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
	if (i>26)
	    motif=alphabet[i%26-1];

	else
	    motif = alphabet[i - 1];


  	for (int j=0;j<lg-5;j++)
    {
    	
    	*(message+j) = motif;
    }
}

//---------------------------------------------------
//-------------Affichage des messages----------------
//---------------------------------------------------

void afficher_message(char *message, int lg)

{
  	
  	int i;

  	for (i=0;i<lg;i++)
    {
      	printf("%c", *(message+i));
    }
  	printf("]\n");
}


//----------------------------------------------------
//----------------Envoi UDP---------------------------
//----------------------------------------------------

void envoi_UDP(int port ,int nb_mess , int lg_msg,char*dest)

  
{
	
  	int length_addr_dist;
  	length_addr_dist=sizeof(struct sockaddr_in);
 	 
  	int sock;
 	int sent;
  	struct sockaddr_in addr_distant;
  	struct hostent* hp;
  	char *message=malloc(sizeof(char)*lg_msg) ;

   	if((sock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP))==-1) //création du socket + test erreur (socket retourne -1 si le socket ne s'ouvre pas comme il faut
    {
      printf("Erreur à la création du socket\n");
      exit(1);
    }
  	memset((char*)&addr_distant,0,sizeof(addr_distant));
  	addr_distant.sin_family=AF_INET;
  	addr_distant.sin_port=port;

  	if((hp=gethostbyname(dest))==NULL)
    {
      printf("Erreur gethostbyname\n");
      exit(1);
    }

    printf("Après gethostbyname , nb_message= %d\n",nb_mess);
      
  	memcpy((char*)&(addr_distant.sin_addr.s_addr), hp->h_addr, hp->h_length);
  	printf("Association IP OK\n");
  	printf("Nombre de messages à envoyer : %d\n", nb_mess);
      
  	for (int i=1; i<=nb_mess ; i++)
    {
    	construire_message(message,'a',lg_msg,i);
      	printf("SOURCE : Envoi n°%d (%d) ", i, lg_msg);
      	

      	if((sent=sendto(sock,message,lg_msg,0,(struct sockaddr*)&addr_distant,sizeof(addr_distant)))==-1)   
      	{
      		printf("Erreur sendto\n");
      		exit(1);
      	} 
      	printbuffer(i);
      	afficher_message(message,sent);
    }

  	if(close(sock)==-1) //fermeture + test erreur à la fermeture
    {
      printf("Echec à la destruction du socket\n");
      exit(1);
    }
}

//----------------------------------------------------
//------------------Reception UDP---------------------
//----------------------------------------------------

void reception_UDP(int port, int nb_message, int lg_message)
{

  int sock;
  struct sockaddr_in addr_local,addr_distant;
  int recv;
  int lg_dist;
  char *message=malloc(sizeof(char)*lg_message) ;

  if((sock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP))==-1) //création du socket + test erreur d'ouverture
    {
      printf("Erreur à la création du socket\n");
      exit(1);
    }
  
  	memset((char*)&addr_local,0,sizeof(addr_local));//reset de addr_local
 	addr_local.sin_family=AF_INET;                  //attribution des différents attributs de addr_local
  	addr_local.sin_port=port;
  	addr_local.sin_addr.s_addr=INADDR_ANY;          //On dit que l'on veut recevoir sur n'importe quelle carte réseau de notre machine (=INADDR_ANY)


  	if ((bind(sock,(struct sockaddr*)&addr_local, sizeof(addr_local)))==-1) //bind de la réception + test erreur du bind
    {
      	printf("Echec du Bind\n");
      	exit(1);
    }

    //printf("bind OK\n");
     
  	if (nb_message==-1)
     	nb_message=10;

    //printf("%d messages à recevoir\n",nb_message);
  	lg_dist=sizeof(addr_distant);
  	//printf("lg_dist initialisé\n");
      
  	for (int i=1 ; i<=nb_message ; i++)
    {
	  	
	  	recv=recvfrom(sock,message,lg_message,0,(struct sockaddr*)&addr_distant,&lg_dist);
  		if (recv==-1) //process de réception (recvfrom natif à l'API socket, + test erreur 
		{
		  printf("Erreur receive from\n");
		  exit(1);
		}
    	else 
		{
	 		printf("PUITS : Réception n°%d (%d) :",i,lg_message);
	 		printbuffer(i);
	  		afficher_message(message,recv);
		}
    }
  
   if(close(sock)==-1) //fermeture + test erreur à la fermeture
    {
      printf("Echec à la destruction du socket\n");
      exit(1);
    }
}

//---------------------------------------------------------------
//----------------ClientTCP <=> source=1-------------------------
//---------------------------------------------------------------

void ClientTCP (int port, int nb_message , int lg_msg , char* dest)
{
	//Déclarations
	int sock;
	struct sockaddr_in addr_distant ;
	int lg_addr_distant=sizeof(addr_distant);

	struct hostent *hp;
	char motif;
	char * message=malloc(lg_msg*sizeof(char)); //Penser au free en fin de programme pour libérer l'espace mémoire
	int envoi=-1;

//---------------------------------------
//--------Etablissement connexion--------
//---------------------------------------

	//Création socket 
	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		printf("Erreur à l'ouverture du Socket Stream");
		exit(1);
	}
	
	//Construction adresse socket distant
	memset((char*)&addr_distant,0,sizeof(addr_distant));
	addr_distant.sin_family=AF_INET;  //Internet
	addr_distant.sin_port=port;       //Numéro de Port

	//Affectation IP
	if((hp=gethostbyname(dest))==NULL)
	{
		printf("Erreur de requête IP.\n");
		exit(1);
	}

	memcpy((char*)&(addr_distant.sin_addr.s_addr), hp->h_addr , hp->h_length);

	//Demande de connexion
	
	if (connect(sock,(struct sockaddr *)&addr_distant,sizeof(addr_distant))==-1)
	{
		printf("Erreur lors de la connexion, en attente de la tentative suivante \n");

	}

	//printf("Connexion Réussie !\n");

//-----------------------------------------
//----------TRANSFERT DE DONNEES-----------
//-----------------------------------------

	for (int i=1; i<=nb_message;i++)
	{
		printf("SOURCE : envoi n°%d (%d) ", i,lg_msg);
		//Création du message 
		
		construire_message(message,motif,lg_msg,i);

		printbuffer(i);
		afficher_message(message,lg_msg);

		//Envoi du message 

		if ((envoi=write(sock,message,lg_msg/*,0,(struct sockaddr*)&addr_distant,lg_addr_distant)*/))==-1)
		{
			printf("Echec de l'envoi du message (fonction send en défaut)\n");
			exit(1);
		}
	}

	//Fermeture connexion

	if(shutdown(sock,2)==-1)
	{
		printf("Erreur à la fermeture de la connexion TCP \n");
		exit(1);
	}

	if (close(sock)==-1)
	{
		printf("Echec de la fermeture du socket distant");
		exit(1);
	}

	free(message);

	printf("Envoi effectué avec succès\n");
}

//---------------------------------------------------------------
//----------------ServeurTCP <=> source=0-------------------------
//---------------------------------------------------------------

void ServeurTCP(int port , int nb_message, int lg_msg)
{
	//Déclarations
	int sock , sock2; //sock bis local orienté échanges 
	struct sockaddr* addr_distant;
	struct sockaddr_in addr_local;
	int lg_addr_distant=sizeof(addr_distant);
	int lg_addr_local=sizeof(addr_local);
	struct hostent *hp;
	char motif;
	char * message=malloc(lg_msg*sizeof(char)); //Penser au free en fin de programme pour libérer l'espace mémoire
	int lg_recv=-1;

//-------------
//Connexion ---
//-------------

	//Création socket local 	

	if ((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		printf("Echec de la création d'un socket local\n");
		exit(1);
	}

	//Construction adresse socket local | Affectation port et domaine

	memset((char*)&addr_local, 0 , sizeof(addr_local));
	addr_local.sin_family=AF_INET;
	addr_local.sin_addr.s_addr=INADDR_ANY;
	addr_local.sin_port=port;

	//printf("Socket créé addressé\n");

	//Bind

	if (bind(sock,(struct sockaddr *)&addr_local, lg_addr_local)==-1)
	{
		printf("Echec du bind.\n");
		exit(1);
	}
	//printf("bind effectué\n");

	//Check connexions entrantes 

	if (listen(sock,100)<0)	
	{
		printf("Trop de connexions en attentes, échec de la demande\n");
		exit(1);
	}

	//rintf("listen accepté\n");


	//Accept connexion
	

	if (nb_message==-1)
	{
		nb_message=10;
	}
	if ((sock2 = accept(sock,(struct sockaddr*)&addr_distant,&lg_addr_distant))==-1)
	{
		printf("Refus de connexion par le serveur\n");
		exit(1);
	}
	//printf("Accept validé, %d messages à recevoir\n",nb_message);
	//Reception des messages au niveau du socket d'échange

	for (int i=1;i<=nb_message;i++)
	{
		printf("PUITS : Réception n°%d (%d) " , i , lg_msg);
		if((lg_recv=read(sock2,message, lg_msg))<0)
		{
			printf("Echec de la lecture du message entrant \n");
			exit(1);
		}
		printbuffer(i);
		afficher_message(message, lg_recv);
	}
	//Fermeture connexion

	if ((shutdown(sock , 2))==-1)
	{
		printf("Erreur à la fermeture de la connexion : shutdown\n");
		exit(1);
	}
		
	//Ciao le socket
	if(close(sock)==-1)
	{
		printf("Impossible de fermer le socket");
	}

	free(message);


}

#endif
