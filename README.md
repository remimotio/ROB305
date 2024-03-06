Séance n°1 : [TD-1] Mesure de temps et échantillonage en temps - Partie a) Gestion simplifie du temps Posix

Instructions de compilation : g++ -std=c++14 -Wall -Wextra timespec.cpp -o timespec

Puis pour l'exécution : ./timespec temps_1 temps_2 où temps_1 et temps_2 peuvent être des flottants positifs ou négatifs

*****

Séance n°2 : [TD-3] Classes pour la gestion du temps - Partie b) Classe Timer

Instructions de compilation :  g++ -std=c++14 -Wall -Wextra main.cpp Timer.cpp CountDown.cpp -o Countdown -lrt -lpthread

Puis pour l'exécution : ./Countdown

On voit apparaître dans le terminal : 

Countdown: 10
Countdown: 9
Countdown: 8
Countdown: 7
Countdown: 6
Countdown: 5
Countdown: 4
Countdown: 3
Countdown: 2
Countdown: 1
Countdown: 0

Réponses aux questions : 

1. Pour chaque élément de classe de la figure 2, expliquez pourquoi il est public, privé ou protégé.

Le constructeur, le destructeur et la fonction start sont déclarés comme publics pour pouvoir créer, détruire et initialiser des objets Timer : il faut pouvoir y accéder en dehors de la classe Timer.
Le paramètre timer_t désigne l'identifiant du timer, il n'y a pas de raisons pour un utilisateur d'y accéder en dehors de la classe. La fonction call_callback est interne, appelée par la classe pour appeler les fonctions callback des classes dérivées : call_callback n'a pas pour but d'être appelée directement en dehors de la classe Timer.
Enfin, la fonction callback est déclarée comme protégée afin de pouvoir être utilisée par les classes dérivées, qui en ont besoin, mais pas par des utilisateurs : le niveau d'accès "protégé" les empêche de la modifier en dehors de la classe.

2. Expliquez quelle est l’utilité de la méthode de classe (statique) call_callback().

La fonction call_callback() joue le rôle de "handler" : lorsqu'un signal est reçu, l'OS déclenche call_callback(). En pratique, cette fonction se charge d'appeler la fonction "callback" implémentée dans la fonction dérivée avec laquelle on travaille. L'utilité d'utiliser des fonctions dérivées est d'implémenter pour chaque type de données une forme adaptée de la fonction, et ainsi de pouvoir traiter de nombreux cas de figure tout en préservant une certaine simplicité du code (notion de polymorphisme).

3. Spécifiez quelles opérations doivent être définies comme virtuelles.

Le destructeur ~Time() doit être défini comme virtuel, puisque la classe Time l'est. Chaque classe dérivée aura son implémentation spécifique du destructeur. Et callback() doit être définie comme virtuelle, puisqu'il s'agit d'une méthode abstraite. La différence entre abstrait et virtuel est qu'une opération abstraite n'est pas implémentée dans la classe de base, uniquement dans les classes dérivées, tandis qu'une opération virtuelle non abstraite possède une implémentation dans la classe originelle. 

*****

Séance n°3 : 
- [TD-1] Mesure de temps et échantillonage en temps - Partie c) Fonction simple consommant du GPU
- [TD-2] Familiarisation avec l'API multitâches pthread - Partie a) Exécution sur plusieurs tâches sans mutex
- [TD-2] Familiarisation avec l'API multitâches pthread - Partie b) Exécution sur plusieurs tâches avec mutex

1) [TD-1] Mesure de temps et échantillonage en temps - Partie c) Fonction simple consommant du GPU

Instructions de compilation : g++ -std=c++14 -Wall -Wextra main2.cpp timespec.cpp -o counterchrono

Puis pour l'exécution : ./counterchrono nLoops où nLoops est un entier naturel

Par exemple, pour ./counterchrono 1000000000, on voit apparaître dans le terminal : 

secondes: 1707214883
nanosecondes: 324400585
Compteur = 1e+09
secondes: 1707214891
nanosecondes: 815027632
secondes: 8
nanosecondes: 490627047

2) [TD-2] Familiarisation avec l'API multitâches pthread - Partie a) Exécution sur plusieurs tâches sans mutex

Commenter le main() de timespec.cpp pour empêcher la présence de deux fonctions main()

Instructions de compilation : g++ -std=c++14 -Wall -Wextra main3.cpp timespec.cpp -o countersansmutex -lrt -lpthread

Puis pour l'exécution : ./countersansmutex nLoops nTasks où nLoops et nTasks sont des entiers naturels

Par exemple, pour ./countersansmutex 1000000000 2, on voit apparaître dans le terminal : 

secondes: 1707215198
nanosecondes: 162107132
Counter value = 1.12902e+09
secondes: 1707215208
nanosecondes: 694426514
secondes: 10
nanosecondes: 532319382

Réponses aux questions : 

Nous pouvons observer que la valeur du compteur n'est pas correcte, au lieu de valoir 2e+09 elle vaut 1.12902e+09. 
Ce phénomène est dû au fait que l'opération d'incrémentation d'un double n'est pas atomique. Le programme prend la variable, la met dans le registre de données, l'incrémente dans le registre, puis remet la valeur dans la mémoire. Or, la seconde tâche préempte la valeur du compteur avant que la première ait pu retourner la valeur. Par conséquent, au lieu de prendre le résultat de la première tâche, la seconde tâche repart de la valeur "initiale", avant son passage dans le premier thread : on a "perdu" une partie de l'action de la première tâche. 

Pour remédier à ce problème, on introduit la notion de mutex (mutual exclusion) : un jeton fourni par l'OS, donné à la tâche jusqu'à la fin de son opération.

3) [TD-2] Familiarisation avec l'API multitâches pthread - Partie b) Exécution sur plusieurs tâches avec mutex

Commenter le main() de timespec.cpp pour empêcher la présence de deux fonctions main()

Instructions de compilation : g++ -std=c++14 -Wall -Wextra main4.cpp timespec.cpp -o counteravecmutex -lrt -lpthread

Puis pour l'exécution : ./counteravecmutex nLoops nTasks protected où nLoops et nTasks sont des entiers naturels (et "protected" est juste le mot en tant que tel)

Par exemple, pour ./counteravecmutex 1000000000 2 protected, on voit apparaître dans le terminal : 

secondes: 1707215321
nanosecondes: 19596034
Counter value = 2e+09
secondes: 1707215446
nanosecondes: 497567339
secondes: 125
nanosecondes: 477971305

Réponses aux questions : 

Grâce à l'implémentation des mutex, la valeur du compteur est bien correcte cette fois-ci.

*****

[TD-3] Classes pour la gestion du temps - Partie a) Classe Chrono

Commenter le main() de timespec.cpp pour empêcher la présence de deux fonctions main()

Instructions de compilation : g++ -std=c++14 -Wall -Wextra timespec.cpp Chrono.cpp testChrono.cpp -o chronometre -lrt -lpthread

Puis pour l'exécution : ./chronometre 

On obtient le résultat suivant, conforme à nos attentes : 

Démarrage du chronomètre
secondes: 1709131628
nanosecondes: 36605086
secondes: 1709131631
nanosecondes: 36847557
Durée: 2999.76 ms
Arret...
secondes: 1709131631
nanosecondes: 36920233
secondes: 1709131633
nanosecondes: 37381184
Durée écoulée depuis l'arrêt: 1999.54 ms

En effet, on peut vérifier que le chronomètre tourne bien durant 3 secondes, puis s'arrête durant 2 secondes. 

*****

[TD-3] Classes pour la gestion du temps - Partie c) Calibration en temps d'une boucle

Instructions de compilation :  g++ -Wall -Wextra timespec.cpp Timer.cpp Calibrator.cpp CpuLoop.cpp testCalibrator.cpp -o calibrateur -lrt -lpthread

Puis pour l'exécution : ./calibrateur 

On voit apparaître dans le terminal : 

i : 0
nLoops = DBL_MAX
nboucles : 1e+08
Nb boucles / unité tps = 232931
i : 1
nLoops = DBL_MAX
nboucles : 1e+08
Nb boucles / unité tps = 174820
i : 2
nLoops = DBL_MAX
nboucles : 1e+08
Nb boucles / unité tps = 205703
i : 3
nLoops = DBL_MAX
nboucles : 1e+08
Nb boucles / unité tps = 224582
i : 4
nLoops = DBL_MAX
nboucles : 1e+08
Nb boucles / unité tps = 239077
i : 5
nLoops = DBL_MAX
nboucles : 1e+08
Nb boucles / unité tps = 235209
i : 6
nLoops = DBL_MAX
nboucles : 1e+08
Nb boucles / unité tps = 236954
i : 7
nLoops = DBL_MAX
nboucles : 1e+08
Nb boucles / unité tps = 239121
i : 8
nLoops = DBL_MAX
nboucles : 1e+08
Nb boucles / unité tps = 238496
i : 9
nLoops = DBL_MAX
nboucles : 1e+08
Nb boucles / unité tps = 235987
Résultats de la calibration : a = 20.4113, b = 207918
Calibrator initialisé
cpuLoop initialisé
a : 20.4113
duration_ms : 2000
b : 207918
nLoops = 248740
Fin de la fonction runtime
runtime terminé
Calibration results: a = 20.4113, b = 207918

Ainsi, le calibrateur fonctionne bien, ce qu'on peut d'ailleurs vérifier en faisant varier la valeur de "maxIterations" dans Calibrator::Looper::runLoop.

*****

[TD-4] Classes de base pour la programmation multitâches 

Partie a) Classe Thread

Instructions de compilation : g++ -Wall -Wextra timespec.cpp PosixThread.cpp Thread.cpp testThread_sans_mutex.cpp -o thread1 -lrt -lpthread

Puis pour l'exécution : ./thread1 nLoops nTasks où nLoops et nTasks sont des entiers naturels 

Par exemple, pour ./thread1 4 2, on voit apparaître dans le terminal : 

secondes: 1709139976
nanosecondes: 384186946
Heure de début : 1.70914e+12 ms
Local counter value = 2
Local counter value = 2
Local counter value = 3
Counter value = 3
Local counter value = 3
Counter value = 3
secondes: 1709139976
nanosecondes: 585036006
Heure de fin : 1.70914e+12 ms
Counter value = 0
Temps d'exécution : 200.849 ms

On constate qu'en l'absence de mutex, les différents threads sont en conflit pour l'accès au compteur, ce qui souligne de nouveau la nécessité d'implémenter des Mutex.

Partie b) Classes Mutex et Mutex::Lock

Instructions de compilation : g++ -Wall -Wextra timespec.cpp PosixThread.cpp Thread2.cpp Mutex.cpp testThread_avec_mutex.cpp -o thread2 -lrt -lpthread

Puis pour l'exécution : ./thread2 nLoops nTasks où nLoops et nTasks sont des entiers naturels

Par exemple, pour ./thread2 3 4, on voit apparaître dans le terminal : 

secondes: 1709144441
nanosecondes: 144192779
Heure de début : 1.70914e+12 ms
Local counter value = 1
Local counter value = 2
Local counter value = 3
Counter value = 3
Local counter value = 4
Local counter value = 5
Local counter value = 6
Counter value = 6
Local counter value = 7
Local counter value = 8
Local counter value = 9
Counter value = 9
Local counter value = 10
Local counter value = 11
Local counter value = 12
Counter value = 12
secondes: 1709144442
nanosecondes: 349778600
Heure de fin : 1.70914e+12 ms
Counter value = 12
Temps d'exécution : 1205.59 ms

On vérifie bien que les mutex permettent de résoudre les conflits d'accès des threads au compteur, dont la valeur correspond bien à ce qui est attendu.

Partie c) Classe Semaphore

Instructions de compilation : g++ -Wall -Wextra timespec.cpp PosixThread.cpp Thread2.cpp Mutex.cpp testSemaphore.cpp -g -o semaphore -lrt -lpthread

Puis pour l'exécution : ./semaphore

*****

[TD-5] Inversion de priorité 
