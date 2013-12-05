i=0
left=[]
right=[]
f=open("Model1.attach", "r")
for line in f:
  if '0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 ' in line:
    left+=[i]
  elif '0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 ' in line:
    right+=[i]
  i+=1

j=0
l=0
r=0
f2=open("Model1.obj", "r")
fa=open("vertices-all.txt", "w")
fl=open("vertices-left.txt", "w")
fr=open("vertices-right.txt", "w")
for line2 in f2:
  if l < len(left):
    if left[l] == j:
      fa.write(line2)
      fl.write(line2)
      l+=1
  if r < len(right):
    if right[r] == j:
      fa.write(line2)
      fr.write(line2)
      r+=1
  j+=1
