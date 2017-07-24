# Lizard

A Markdown note taker. C++ implementation. I did this project first in Python with PyQt5, but decide to rebuild it with Qt Creator in C++. I use this editor for note taking during class. Also, it was a way to discover the Qt library and practice my C++ skills.

## Prerequisites

I recommend having **Qt Creator** installs on your machine. Then simply open the *.pro* file.

~~You can also build the binary with the help of the **Makefile**. However, make sure to install all dependencies.~~

There's no **Makefile** anymore. You have to build the editor with the help of **qmake** command. Howerver, I still recommend to use **Qt Creator** for that purpose. The build created then contain all the object file in **build** directory, the actual binary executable in **bin** and a **Makefile** at top of the build.

## Installing
The current **Makefile** doesn't have an install command yet. Simply call **make** to build the binay in the **bin** folder.

## Authors

- **Olivier Dion**

## Versions

- 24/07/2017: **1.2.0**
- 20/07/2017: **1.1.0**
- 18/07/2017 : **1.0.0**