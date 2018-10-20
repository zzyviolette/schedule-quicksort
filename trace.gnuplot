set terminal png size 1024,768
set termoption enhanced
set encoding utf8

# Pour changer le titre
#set title "Quicksort"

set title "`head -1 title.value`"

set style data lines

# Les legendes des axes
set xlabel "Nombre de threads"
set ylabel "Temps"
set logscale y
set xtics 1
# set yrange[0:5]


# Pour changer le nom du fichier produit
set output 'graphique.png'

# La commande pour tracer. Le \ est obligatoire avant un passage à la ligne
plot 'Stealing.values' u ($1+0.05):($2+0.05):3 with labels notitle,\
'LIFO.values' u 1:2 title "LIFO" lw 2 linecolor rgb "red",\
'Stealing.values' u 1:2 title "Stealing" lw 2 linecolor rgb "green"
