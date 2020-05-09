                    __________________________________________________________________________
                    ||              Projet de Programmation C orientée réseaux              ||
                    ||                                                                      ||
                    ||              Prénom : Béranger                                       ||
                    ||              Nom :    Quintana                                       ||
                    ||              Groupe : 3AE E                                          ||
                    ||______________________________________________________________________||

Bonjour, bienvenue sur ce programme d'échanges basé sur les protocoles TCP et UDP.
Avec ce programme vous pourrez utiliser ces protocoles sous différentes formes :

                    -De manière classique, avec un puits (-p) qui reçoit les informations d'une source (-s)
                    -Avec une nouvelle fonctionnalité de serveur de Boite aux Lettres

La fonctionnalité qui sera lancée au démarrage du programme sera celle de BAL.

________________________________________________________________________________________________________________________
                                                    Lancement de tsock
________________________________________________________________________________________________________________________

Pour lancer le programme, ouvrez un terminal dans le répertoire de travail, puis tapez la commande :

                                                           make

Cela compilera vos fichiers. Vous pourrez par la suite lancer vos différents programmes.
________________________________________________________________________________________________________________________
                                               USAGE de la Boite Aux Lettres
________________________________________________________________________________________________________________________
                                                ==========================
                                                LANCEMENT D'UN SERVEUR BAL
                                                ==========================
Ici on peut lancer un serveur de Boites aux lettres sur une adresse IP et un port donné. Ce serveur se comporte comme
une boucle infinie qui stocke les messages entrants dans des Boites aux lettres et les délivre par la suite après
requête d'un récepteur.

Pour lancer notre serveur BAL, il faut utiliser l'option -b du programme et lui attribuer un port d'écoute
    Exemple, je veux lancer un serveur BAL sur le port 9000 :
        Commande à utiliser :
                                                     ./tsock -b 9000

                                                   ====================
                                                   EMISSION DE MESSAGES
                                                   ====================

Pour envoyer une ou plusieurs lettres,  à un boite aux lettres, il faut utiliser l'option -e# du programme avec # le
numéro de Boite aux lettres destinataire, ainsi que fournir l'adresse IP du serveur, et le port d'écoute.
    Exemple je veux envoyer des messages à la BAL n°5 sur l'IP 127.0.0.1, sur le port 9000
        Commande à utiliser :
                                               ./tsock -e5 127.0.0.1 9000

        NB : si vous ne précisez pas le numéro de BAL cela envoira par défaut sur la BAL n°0

Options disponibles pour l'émetteur :
    -n# : fixe le nombre de message à émettre avec # le nombre de messages
    -l# : fixe la longueur des messages à émettre avec # la longueur des messages pour cet envoi.
    ATTENTION : On ne peut pas envoyer de messages plus court que les messages présents dans la BAL sous peine de
                rogner les messages ces derniers. Le programme ne vous en informera pas lors de l'envoi...
                En revanche, on peut envoyer des messages plus long que ceux précédemment envoyés.

                                                   =====================
                                                   RECEPTION DE MESSAGES
                                                   =====================

Pour récupérer les lettres d'une boite aux lettres le principe est le même :
    Exemple : Je veux récupérer les lettres de la BAL n°5, à l'IP 127.0.0.1, sur le port 9000 :
        Commande à utiliser :
                                                 ./tsock -r5 localhost 9000

        Vous ne pouvez ici que récupérer l'ensemble des lettres disponibles dans une des Boites aux lettres.
        Les options -n et -l ne sont donc pas utilisables.

========================================================================================================================
========================================================================================================================

Avec ces fichiers vous pourrez également utiliser TCP et UDP dans des usages plus classiques d'envois et réceptions
directes

________________________________________________________________________________________________________________________
                                                    USAGE CLASSIQUE
________________________________________________________________________________________________________________________

    -Il est nécessaire d'activer le puits en premier et en lui attribuant une adresse IP distante ainsi qu'un
    numéro de port
        Exemple : Je veux recevoir depuis l'hôte local sur le port 9000
            Commande à utiliser :
                                              ./tsock -p localhost 9000

    Seulement après avoir activé notre puits, on peut lancer notre source toujours en lui attribuant l'IP destinataire
    et le numéro de port ou l'on veut envoyer nos informations.
        Exemple : Je veux envoyer à destination de l'hote local sur le port 9000
            Commande à utiliser :
                                              ./tsock -s localhost 9000

Liste des options :

        -n : permet de fixer le nombre de messages à envoyer (par défaut : 10)
        -l : permet de fixer la longueur des messages à envoyer (par défaut 30)
        -u : permet d'utiliser le protocole UDP car TCP est utilisé par défaut

      Pour l'usage classique, il est important que l'utilisateur du puits, paramètre correctement la longueur des
      messages à recevoir pour pouvoir les recevoir correctement, sinon, la longueur par défaut sera de 30 caractères.

      De plus, concernant le nombre de messages, si l'utilisateur du puits utilise l'option -n, et que le n du puits
      est inférieur au -n du récepteur, il ne recevra pas les denrniers messages.

________________________________________________________________________________________________________________________
                                                 Remarques Générales
________________________________________________________________________________________________________________________

        -En principe, l'utilisation de fonctions de la partie Classique sur une serveur BAL ne fonctionne pas.
         Par exemple, l'envoi depuis une source TCP (-s) vers un serveur BAL (-b) en fonctionnement contenant déjà des
         lettres, enverra les lettres sur la dernière BAL utilisée. L'envoi depuis une source vers un serveur BAL ne
         contenant pas de lettres sera bloquant. Il faudra relancer le serveur BAL pour pouvoir recevoir des lettres.

        -La problématique de la longueur des messages à recevoir côté serveur n'a pas pu être traitée par manque de
         temps.

        -Il reste surement des coquilles....

Si besoin, n'hésitez pas à me contacter : bquintan@etud.insa-toulouse.fr

Bonne correction!