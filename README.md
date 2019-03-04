## Compile

make all

###########

All files are in the folder:
�� mysort2GB.slurm --slurm mysort 2GB file
�� mysort20GB.slurm--slurm mysort 20GB file
�� linsort2GB.slurm--slurm linux sort 2GB file
�� linsort20GB.slurm--slurm linux sort 20GB file
�� Makefile--make file
�� mysort.c--mysort source c code
�� pa2a_report.pdf--report of pa2a
�� readme.txt--this file
�� mysort2GB.log--valide my 2GB sort data
�� mysort20GB.log--valide my 20GB sort data
�� linsort2GB.log--valide linux sort 2GB sort data
�� linsort20GB.log--valide linux 20GB sort data


To compile the code, you can type bash commend as:

gcc mysort.c -pthread -lm -o mysort
or you can just make to generate executing file.

And it would produce executing file--mysort.
you can directly run this file by command:./mysort 2G for sorting data-2G.in, and ./mysort 20G for sorting data-20G.in


or you can send this executing file to slurm by running script:

sbatch ./mysort20GB.slurm
sbatch ./mysort2GB.slurm

***************************************
If you need run Linux sort, 
just running sbatch ./linsort2GB.slurm or 
sbatch ./linsort20GB.slurm

***************************************

All the sorted result is verified by varsort

Author: Yunjie Xu













