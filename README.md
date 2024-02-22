Meilleur parsing + switch/case -> mael + parsing dans first_connection

Commandes : 

Imrane : PING/PONG, PASS, NICK, QUIT, USER

Mael : JOIN, PART, MODE, KILL, PRIVMSG

Gael : TOPIC, KICK, WALLOPS, OPER, NOTICE

Class command;




Les ajouts par rapport au parsing / gestion des requetes : 
	- Dans la fonction requestParsing : 
		
		- Compte le nombre de requetes selon les \r et \n
		
		- Remplit le vecteur qui contient toutes les requetes dans fillVectorRequest
		
		- Remplit une MAP<string, string>, dans fillCmdMap qui va nous servir pour le choix des commandes. l'idee : associer la 				commande (arg 1 de la map) et ses arguements (arg 2 de la map). Les arg 1 vont servir pour savoir quelle commande 					utiliser. Les arg 2 vont servir dans les fonctions propres a chaque commande : si on appelle NICK, dans la fonction 				implementee pour NICK, on utilisera. les arguments pour remplir le role de la commande.

		- Ensuite on rentre dans chooseAndExecuteAction. Ici on utilise le premier vecteurs qui contient les commandes en brut (le 						vecteur s'appelle M_command) et on compare les commandes en brut avec les clefs de la map dams une double boucle. Au 				sein de cette boucle on fait evoluer un iterateur i qui avance en meme temps que le vecteur qui contient les commandes. 				Ce qui signifie que i suit les commande,  exemple : si i = 1, on est sur NICK, si i = 2, on est sur PASS etc. Au 					moment ou la cle est trouvee, on passe i dans une fonction qui utilise le switch/case pour lancer les fonctions qui 				executeront les commandes.

		- J'ai laisser des prints pour que l'on voit bien le comportements

IMRANE CHANGEMENT :
- ajout clientFd en param dans chooseAndExecuteAction et excecuteCmd
- Passage de M_struct private à public
- Ajout de l'objet command dans la structure de Serveur
- ajout de la class Serveur dans la class command
- création de la command QUIT



02-02-2024 : Mael

- Remplacement de la map dans le serveur : vecteur a la place de string
- Changement dans le parsing dans fillCmdMap -> remplit le vecteur
- Ajout de std::vector<std::string> split_string_v2(const std::string& s, char delimiter)
- Changement de la fonction USER dans command -> adaptee a la map mais meme logique/comportement
- Adaptation de JOIN
- Rajout de la maccro BANNEDFROMCHAN pour la gestion d'erreur dans JOIN
- Changement du prototype de executeCmd -> plus de parameter, mieux pour les fonctions d'Imrane
- Rajout de "tmp = tmp.substr(token + 1, tmp.size())" dans fillVectorRequest -> permet de ne pas prendre en compte l'espace



04_05_06/02/2024 : Mael

- Fonction JOIN terminee -> mais encore un petit soucis avec envoi d'erreur concernant les mauvais noms de channel
- Commencement de MODE -> Creation de l'operateur "=" dans channel + nouvelles fonctions pour copie profonde


A FAIRE AVANT 14h le 22/02 !!!

Revoir les messages dans OPER : fait mais a revoir dans le cannal

Revoir les deux dernieres commandes KILL et NOTICE : a faire avec Gael

Ajouter message pour topic

Faire message de bienvenue dans JOIN + revoir le message en cas de mauvais nom de cannal : a faire -> ok

Dans part = faire un message KICK au moment de partir et laisser le message PART pour les clients presents dans le cannal + laisser le 
message PART pour le client qui a quitter : a faire -> ok

Dans KICK -> faire un message au client virer du cannal : a faire + les raisons

2 erreurs Valgrind a gerer + already nick use + faire les leaks dans QUIT + nickname + enleve parsing MDP : a faire


server : ligne 275