#!/bin/bash

#######################################
# By Tiago Monteiro Fernandes         #
#                                     #
# Plots ./plotlog_$1.log with gnuplot #
# Saves at ./grafico_$1.jpeg          #
#######################################

if test $1 -a $2 -a $3 -a $4 -a $5;
    then
    if test ! -f "./plotlog_$1.log"; 
        then echo "Arquivo ./plotlog_$1.log nao encontrado."; 
    else
        echo "set terminal jpeg" > plota.config.temp;
        echo "set output './grafico_$1.jpeg'" >> plota.config.temp;
        echo "set xrange [$2:$3]" >> plota.config.temp;
        echo "set yrange [$4:$5]" >> plota.config.temp;
        echo "plot './plotlog_$1.log' using 1:3 title 'Best' with lines, './plotlog_$1.log' using 1:2 title 'Mean' with lines" >> plota.config.temp;
        gnuplot plota.config.temp;
        rm plota.config.temp
    fi
else echo "Usage: ./plota.sh number XrangeInit XrangeEnd YrangeInit YrangeEnd";
fi