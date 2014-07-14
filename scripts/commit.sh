pss='/Users/marc_leef/Desktop/Work/PSS'
pssWeb='/Users/marc_leef/Desktop/Work/web'

c1=$1
c2=$2


cd $pss
git add -A
git commit -m $c1
git push -u origin master

cd $pssWeb
git add -A
git commit -m $c2
git push -u origin master

