#/bin/sh

for i in $(ls /usr/bin | awk '(NR-605101374)%251 == 0')
do
	ldd '/usr/bin/'$i
done
