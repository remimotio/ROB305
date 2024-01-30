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
