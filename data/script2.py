i=1
left=[-1]
right=[-1]
f=open("Model1.attach", "r")
for line in f:
  if '0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 ' in line:
    left+=[i]
  elif '0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 ' in line:
    right+=[i]
  i+=1

j=1
l=1
r=1
f2=open("Model1.obj", "r")
fa=open("vertices-all.txt", "w")
fl=open("vertices-left.txt", "w")
fr=open("vertices-right.txt", "w")
for line2 in f2:
  if l < len(left) and left[l] == j:
    fa.write(line2)
    fl.write(line2)
    fr.write("v 0 0 0\n")#0
    l+=1
  elif r < len(right) and right[r] == j:
    fa.write(line2)
    fr.write(line2)
    fl.write("v 0 0 0\n")#0
    r+=1
  elif line2[0] == 'v':
    fa.write("v 0 0 0\n")#0
    fr.write("v 0 0 0\n")#0
    fl.write("v 0 0 0\n")#0
  elif line2[0] == 'f':
    split = line2.split()
    if int(split[1]) in left and int(split[2]) in left and int(split[3]) in left:
      fa.write(line2)
      fl.write(line2)
    if int(split[1]) in right and int(split[2]) in right and int(split[3]) in right:
      fa.write(line2)
      fr.write(line2)
  j+=1
