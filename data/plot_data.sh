gnuplot -e "set terminal dumb 80 25; plot '<(head -50 two_class_100.txt)' pt \"*\", '<(tail -50 two_class_100.txt)' pt \"o\""
