i=0
#all[500]
#left[500]
#right[500]
#declare -a all
#declare -a left
#declare -a right
#a=0
#l=0
#r=0
while read line; do
  if expr "$line" : '0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 '; then
        echo "$i" >> "vertices-all.txt"
	echo "$i" >> "vertices-left.txt"
	#all[$a] = $i;
	#left[$l] = $i;
	#a=$(($a+1));
	#l=$(($l+1));
  elif expr "$line" : '0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 '; then
        echo "$i" >> "vertices-all.txt"
	echo "$i" >> "vertices-right.txt"
	#all[$a] = $i;
	#right[$r] = $i;
	#a=$(($a+1));
	#r=$(($r+1));
  fi
  i=$(($i+1));
done < "Model1.attach"

#j=0
#l=0
#r=0
#while read line2; do
#  if ${left[$l]} == $j; then
#	echo "$line2" >> "vertices-all.txt"
#	echo "$line2" >> "vertices-left.txt"
#	l=$(($l+1));
#  elif ${right[$r]} == $j; then
#	echo "$line2" >> "vertices-all.txt"
#	echo "$line2" >> "vertices-left.txt"
#	r=$(($r+1));
#  fi
#  j=$(($j+1));
#done < "Model1.obj"

echo "Final line count is: $i";
