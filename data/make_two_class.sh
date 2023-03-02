cat rand_100_2.txt | awk 'NR<=50 {print $1-.6, $2-.6, 0} NR>50 {print $1+.6, $2+.6, 1}' > two_class_100.txt
