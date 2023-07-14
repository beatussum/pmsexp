# <img src="share/icons/com.github.PMSExp.png" width="64" height="64"/> PMSExp

[Français](README_fr.md)

## Table of contents

- [What is PMSExp?](#what-is-pmsexp)
- [Why PMSExp (Parking Monitoring System Experiment)?](#why-pmsexp-parking-monitoring-system-experiment)
- [Licenses](#licenses)
- [Installing and building](#installing-and-building)

## What is PMSExp?

[**PMSExp**](https://github.com/beatussum/pmsexp/) is free software (licensed under GPL-3 or any later version) allowing to automate video pointing of an object.

In particular, it allows to recover the follwing informations:
- the heading of the object (the front of the object being determined by its shape) over time;
- its coordinates over time.

To work, it depends on the following libraries:
- [Qt](https://www.qt.io/) for the user interface;
- [OpenCV](https://opencv.org/) for the video processing;
- [QCustomPlot](https://www.qcustomplot.com/) for displaying data in graphs.

## Why PMSExp (Parking Monitoring System Experiment)?

Initially, the development of this software was started as part of the development of another project. In fact, as a [C.P.G.E.](https://www.enseignementsup-recherche.gouv.fr/fr/classes-preparatoires-aux-grandes-ecoles-cpge-46496) student, I had to do a [T.I.P.E.](https://www.scei-concours.fr/tipe.php) like all the other French student in this field. Given the P.T. pathway I was in, my T.I.P.E. had to consist of a technical system.

My first choice was a robot to control the parking time of vehicles in a private parking lot. So, I started writing the [Arduino](https://www.arduino.cc/) program to be loaded into the board, and named it **P.M.S.** or **P**arking **M**onitoring **S**ystem (~~very~~ slightly pompous, but the meaning comes through a little).

However, the idea of having a vehicle the size of a miniature car, circulating in a parking lot (and therefore very likely to be run over or vandalized), did not meet with unanimous approval among my teachers. Knowing, moreover, that an ordinary automaton armed with a lice would do just as well (if not better). So, I transformed my robot into a robot vacuum cleaner, while keeping the program's name for historical reasons, so to speak…

At the end of my project, I had to test the validity of my system (did it follow the path I asked it to follow?). To do this, I needed a way of retrieving my robot's position over time from a video file. So, I started developing **PMSExp** (I just had to suffix _"Experiment"_ to the name of the parent project).

Eventually, finding the project interesting, I decided to continue its development on a standalone basis.

I'm now realizing that it took me a lot of lines to explain the simple and sad truth: the name of this project really doesn't make any sense…

## Licenses

As explained above, the code of this software is licensed under GPL-3 or any later version. Details of the rights applying to the various third-party files are described in the [copyright](copyright) file in [the Debian `debian/copyright` file format](https://www.debian.org/doc/packaging-manuals/copyright-format/1.0/).

## Installing and building

Please, see [the corresponding wiki page](https://github.com/beatussum/pmsexp/wiki/Installing-PMSExp).
