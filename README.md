Meilleur parsing + switch/case -> mael + parsing dans first_connection

Commandes : 

Imrane : PING/PONG, PASS, NICK, QUIT, USER, MODE

Mael : OPER, JOIN, PART, MODE, KILL, NOTICE

Gael : TOPIC, KICK, MODE, PRIVMSG, WALLOPS

Class command;




Les ajouts par rapport au parsing / gestion des requetes : 
	- Dans la fonction requestParsing : 
		
		- Compte le nombre de requetes selon les \r et \n
		
		- Remplit le vecteur qui contient toutes les requetes dans fillVectorRequest
		
		- Remplit une MAP<string, string>, dans fillCmdMap qui va nous servir pour le choix des commandes. l'idee : associer la 				commande (arg 1 de la map) et ses arguements (arg 2 de la map). Les arg 1 vont servir pour savoir quelle commande 					utiliser. Les arg 2 vont servir dans les fonctions propres a chaque commande : si on appelle NICK, dans la fonction 				implementee pour NICK, on utilisera. les arguments pour remplir le role de la commande.

		- Ensuite on rentre dans chooseAndExecuteAction. Ici on utilise le premier vecteurs qui contient les commandes en brut (le 						vecteur s'appelle M_command) et on compare les commandes en brut avec les clefs de la map dams une double boucle. Au 				sein de cette boucle on fait evoluer un iterateur i qui avance en meme temps que le vecteur qui contient les commandes. 				Ce qui signifie que i suit les commande,  exemple : si i = 1, on est sur NICK, si i = 2, on est sur PASS etc. Au 					moment ou la cle est trouvee, on passe i dans une fonction qui utilise le switch/case pour lancer les fonctions qui 				executeront les commandes.

		- J'ai laisser des prints pour que l'on voit bien le comportements
