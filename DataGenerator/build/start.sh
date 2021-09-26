#! /bin/bash

program="./DataGenerator"

qsn_1d=(500000 1000000 2000000 5000000 10000000)
qsn_2d=(500000 1000000 2000000 5000000 10000000)
qsn_3d=(50000 500000 1000000 2000000 5000000)
qsn_4d=(100000 200000 500000 1000000 2000000)

qsn_u_1d=(50000 100000 200000 500000 1000000 2000000 )
qsn_u_2d=(10000 20000 50000 100000 200000 500000)
qsn_u_3d=(2000 5000 10000 20000 50000 100000)


p_alive=(0.05 0.1 0.2 0.5 1)
d=(1 2 3 4)
Us=(1000 10000 100000 1000000 10000000 100000000)
U_d=100000
tau=(1000000 2000000 5000000 10000000 )

dp=0.2
scale=0.01
thres=1000000
qsn_index=2
universe=10000000
w_mean=10

if (($1 == 21))
then

lf="./E21"
echo "start to run in file fold: "${lf} > bash.log
if  [ ! -d  "$lf"  ]; then
mkdir  "$lf"
fi
for((j=0;j<${#d[*]};j++))
do

if  [ ! -d  "$lf/d${d[j]}"  ]; then
mkdir  "$lf/d${d[j]}"
fi
eval qsn=(\${qsn_${d[j]}d[@]})
for((i=0;i<${#qsn[*]};i++))
do
	echo "start to run commond line: "${line} >>bash.log
	echo "-------------------------------------" >>bash.log
    line=${program}" -qsn ${qsn[i]} -qin ${qsn[i]} -lf ${lf}/d${d[j]} -rf ../../ic2rb2.csv -s ${scale} -dp ${dp} -U ${universe} -t ${qsn[i]} -w ${w_mean} -real 1 -d ${d[j]}  >>bash.log"
	echo ${line}
    eval ${line}

  done
  done
fi





if (($1 == 11))
then

lf="./E11"
echo "start to run in file fold: "${lf} > bash.log
if  [ ! -d  "$lf"  ]; then
mkdir  "$lf"
fi
for((j=0;j<${#d[*]};j++))
do

if  [ ! -d  "$lf/d${d[j]}"  ]; then
mkdir  "$lf/d${d[j]}"
fi
eval qsn=(\${qsn_${d[j]}d[@]})
for((i=0;i<${#qsn[*]};i++))
#for((i=2;i<3;i++))
do
	echo "start to run commond line: "${line} >>bash.log
	echo "-------------------------------------" >>bash.log
    line=${program}" -qsn ${qsn[i]} -qin ${qsn[i]} -lf ${lf}/d${d[j]} -rf ../../ic2rb2.csv -s ${scale} -dp ${dp} -U ${universe} -t ${qsn[i]} -w ${w_mean} -real 0 -d ${d[j]}  >>bash.log"
	echo ${line}
#    eval ${line}
  done
  done
fi




if (($1 == 22))
then

lf="./E22"
echo "start to run in file fold: "${lf} > bash.log
if  [ ! -d  "$lf"  ]; then
mkdir  "$lf"
fi
for((j=0;j<${#d[*]};j++))
do

if  [ ! -d  "$lf/d${d[j]}"  ]; then
mkdir  "$lf/d${d[j]}"
fi
eval qsn=(\${qsn_${d[j]}d[@]})
for((i=0;i<${#p_alive[*]};i++))
do
	echo "start to run commond line: "${line} >>bash.log
	echo "-------------------------------------" >>bash.log
    line=${program}" -qsn ${qsn[qsn_index]} -qin ${qsn[qsn_index]} -lf ${lf}/d${d[j]} -rf ../../ic2rb2.csv -s ${scale} -dp ${p_alive[i]} -U ${universe} -t ${qsn[qsn_index]} -w ${w_mean} -real 1 -d ${d[j]}  >>bash.log"
	echo ${line}
    eval ${line}

  done
  done
fi


if (($1 == 31))
then

lf="./E31"
echo "start to run in file fold: "${lf} > bash.log
if  [ ! -d  "$lf"  ]; then
mkdir  "$lf"
fi
for((j=0;j<${#d[*]};j++))
do

if  [ ! -d  "$lf/d${d[j]}"  ]; then
mkdir  "$lf/d${d[j]}"
fi
eval qsn=(\${qsn_${d[j]}d[@]})
for((i=0;i<${#Us[*]};i++))
do
	echo "start to run commond line: "${line} >>bash.log
	echo "-------------------------------------" >>bash.log
    line=${program}" -qsn ${qsn[0]} -qin ${qsn[0]} -lf ${lf}/d${d[j]} -rf ../../ic2rb2.csv -s ${scale} -dp ${dp} -U ${Us[i]} -t ${qsn[0]} -w ${w_mean} -real 0 -d ${d[j]}  >>bash.log"
	echo ${line}
#    eval ${line}

  done
  done
fi


if (($1 == 32))
then

lf="./E32"
echo "start to run in file fold: "${lf} > bash.log
if  [ ! -d  "$lf"  ]; then
mkdir  "$lf"
fi
for((j=0;j<${#d[*]};j++))
do

if  [ ! -d  "$lf/d${d[j]}"  ]; then
mkdir  "$lf/d${d[j]}"
fi
eval qsn=(\${qsn_u_${d[j]}d[@]})
for((i=0;i<${#qsn[*]};i++))
do
	echo "start to run commond line: "${line} >>bash.log
	echo "-------------------------------------" >>bash.log
    line=${program}" -qsn ${qsn[i]} -qin ${qsn[i]} -lf ${lf}/d${d[j]} -rf ../../ic2rb2.csv -s ${scale} -dp ${dp} -U ${U_d} -t ${qsn[i]} -w ${w_mean} -real 0 -d ${d[j]} >>bash.log"
	echo ${line}
#    eval ${line}

  done
  done
fi





