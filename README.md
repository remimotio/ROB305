TD n°1 : [TD-1] Mesure de temps et échantillonage en temps - Partie a) Gestion simplifie du temps Posix

Instructions de compilation : g++ -std=c++14 -Wall -Wextra timespec.cpp -o timespec

Puis pour l'exécution : ./timespec temps_1 temps_2 où temps_1 et temps_2 peuvent être des flottants positifs ou négatifs

TD n°2 : [TD-3] Classes pour la gestion du temps - Partie b) Classe Timer

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

TD n°3 : 
- [TD-1] Mesure de temps et échantillonage en temps - Partie c) Fonction simple consommant du GPU
- [TD-2] Familiarisation avec l'API multitâches pthread - Partie a) Exécution sur plusieurs tâches sans mutex
- [TD-2] Familiarisation avec l'API multitâches pthread - Partie b) Exécution sur plusieurs tâches avec mutex

*****

- [TD-1] Mesure de temps et échantillonage en temps - Partie c) Fonction simple consommant du GPU

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

Réponses aux questions : 

- [TD-2] Familiarisation avec l'API multitâches pthread - Partie a) Exécution sur plusieurs tâches sans mutex

Instructions de compilation :  

Puis pour l'exécution : ./

On voit apparaître dans le terminal : 

Réponses aux questions : 

- [TD-2] Familiarisation avec l'API multitâches pthread - Partie b) Exécution sur plusieurs tâches avec mutex

Instructions de compilation :  

Puis pour l'exécution : ./

On voit apparaître dans le terminal : 

Réponses aux questions : 
