#!/bin/bash

###########################################
# By Tiago Monteiro Fernandes             #
#                                         #
# Plots ./log/plotlog_$1.log with gnuplot #
# Saves at ./log/grafico_$1.jpeg          #
###########################################

if test $1;
    then 
    if test ! -f "./logs/plotlog_$1.log"; 
        then echo "Arquivo ./logs/plotlog_$1.log nao encontrado."; 
    else
        echo "set terminal jpeg" > plota.config.temp;
        echo "set output './logs/grafico_$1.jpeg'" >> plota.config.temp;
        echo "plot './logs/plotlog_$1.log' using 1:3 title 'Best' with lines, './logs/plotlog_$1.log' using 1:2 title 'Mean' with lines" >> plota.config.temp;
        gnuplot plota.config.temp;
        rm plota.config.temp
    fi
else echo "Usage: ./plota.sh number";
fi