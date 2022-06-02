#| /bin/bash

echo "bash script test is activated"
Num=0
root_dir=$1
date=$2
cores=$3
Nps=$4
position=$5

savefile="${root_dir}/${date}/${position}"
if [ ! -d ${savefile} ] ; then
 mkdir -p ${savefile}
fi
while (("${Num}" < ${repeat_num}));
do	
	echo "$Num's simulation starts !! "
	FLOOR=10000
	rannum1=0
	while [ "$rannum1" -le $FLOOR ]
	do
		rannum1=$RANDOM
	done
	rannum2=0
	while [ "$rannum2" -le $FLOOR ]
	do
		rannum2=$RANDOM
	done
	rannum1=`expr $rannum1 \* 100000`
	rannum=`expr $rannum1 + $rannum2`

	let rest=${rannum}%2
	if [ ${rest} -eq 0 ] ; then
		(( rannum = "${rannum}" +1))
	fi
	echo "rannum: ${rannum}"
	#echo "oldrannum: ${oldrannum}"
	sed -i "71s/.*/rand  seed=${rannum} $/g" ../inputs/MRCP-AM_external_${condition}_input 
	sed -i "72s/.*/nps ${Nps} $/g" ../inputs/MRCP-AM_external_${condition}_input 
	mcnp6 i=../inputs/MRCP-AM_external_${condition}_input o=${savefile}/${date}_${condition}_seed${rannum}_mcnpum_$Num.o RU=${savefile}/runtp_${condition}_$Num &

#	oldrannum=${rannum}
	(( Num = "${Num}" + 1))
	sleep 3
done

