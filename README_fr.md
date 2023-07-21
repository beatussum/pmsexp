# <img src="share/icons/com.github.PMSExp.svg" width="64" height="64"/> PMSExp

[![Licence](https://img.shields.io/github/license/beatussum/pmsexp?label=licence)](LICENSE) [![Version](https://img.shields.io/github/v/release/beatussum/pmsexp?filter=*&label=version)](https://github.com/beatussum/pmsexp/releases/)

[English](README.md)

## Table des matières

- [Qu'est-ce que PMSExp ?](#quest-ce-que-pmsexp)
- [Pourquoi PMSExp (Parking Monitoring System Experiment) ?](#pourquoi-pmsexp-parking-monitoring-system-experiment)
- [Comment utiliser PMSExp ?](#comment-utiliser-pmsexp)
- [Licences](#licences)
- [Installation et construction](#installation-et-construction)

## Qu'est-ce que PMSExp ?

[**PMSExp**](https://github.com/beatussum/pmsexp/) est un outil libre (sous licence GPL-3 ou ultérieure) permettant d'automatiser le pointage vidéo d'un objet.

En particulier, il permet de récupérer les informations suivantes :
- le cap de l'objet (l'avant de l'objet étant déterminé à partir de sa forme) au cours du temps ;
- son ordonnée et son abscisse au cours du temps.

Pour fonctionner, il s'appuie sur différentes bibliothèques :
- [Qt](https://www.qt.io/) pour l'interface graphique ;
- [OpenCV](https://opencv.org/) pour le traitement de fichiers vidéos ;
- [QCustomPlot](https://www.qcustomplot.com/) pour l'affichage des données dans des graphes.

## Pourquoi PMSExp (Parking Monitoring System Experiment) ?

Initialement, le développement de ce logiciel a commencé pour la mise au point d'un autre logiciel. En effet, en tant qu'élève de [C.P.G.E.](https://www.enseignementsup-recherche.gouv.fr/fr/classes-preparatoires-aux-grandes-ecoles-cpge-46496), j'ai dû comme l'ensemble des _taupins_ de France réaliser un [T.I.P.E.](https://www.scei-concours.fr/tipe.php). Étant donné la filière P.T. dans laquelle je me trouvais, mon T.I.P.E. devait consister en système technique.

J'ai d'abord opté pour un robot permettant de contrôler le temps de stationnement des véhicules se trouvant dans un parking privé. J'ai donc commencé à écrire le programme [Arduino](https://www.arduino.cc/) qui doit être chargé sur la carte et je l'ai nommé **P.M.S.** ou **P**arking **M**onitoring **S**ystem (~~très~~ légérement pompeux mais dont le sens transparaît un peu).

Cependant, l'idée d'avoir un véhicule de la taille d'une voiture miniature, circulant dans un parking (et donc très susceptible de se faire écraser ou vandaliser), n'a pas fait l'unanimité parmi mes professeurs. Sachant, de plus, qu'un banal automate armé d'une lice ferait tout aussi bien l'affaire (si ce n'est mieux). J'ai donc transformé mon robot en robot aspirateur, tout en conservant le nom du programme pour des raisons, pour ainsi dire, historiques…

À la fin de mon projet, j'ai dû tester la validité de mon système (suivait-il le parcours que je lui demandais de suivre ?). Pour cela, il fallait un moyen de récupérer la position de mon robot au cours du temps à partir d'un fichier vidéo. C'est donc ainsi que j'ai commencé à développer **PMSExp** (il m'a suffit de suffixer _« Experiment »_ au nom du projet parent).

En fin de compte, trouvant le projet intéressant, j'ai décidé de continuer son développement en _standalone_.

Je suis en train de me rendre compte que cela m'a pris beaucoup de lignes pour expliquer la simple et triste réalité : le nom de ce projet n'a vraiment aucun de sens…

## Comment utiliser PMSExp ?

Prière de lire [le fichier correspondant](share/doc/UserGuide.md).

## Licences

Comme expliqué ci-dessus, le code de ce logiciel est sous licence GPL-3 ou toute version ultérieure. Les détails concernant les droits s'appliquant sur les différents fichiers tiers sont décrits dans le fichier [copyright](copyright) au [format des fichiers `debian/copyright` de Debian](https://www.debian.org/doc/packaging-manuals/copyright-format/1.0/).

## Installation et construction

Prière de consulter [la page du wiki correspondante](https://github.com/beatussum/pmsexp/wiki/Installing-PMSExp).
