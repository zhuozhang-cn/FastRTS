#! /bin/bash

program="./FastRTS_V"

d=(2)
gammas=(0.01 0.02 0.05 0.1 0.2 )
gam=0.05


if (($1 == 13))
then
	for ((j=0;j<${#d[*]};j++))
	do
	lf="/E13"
	datafile="../../DataGenerator/build/E13/"
	dir="d${d[j]}"
	if  [ ! -d  ".$lf"  ]; then
		mkdir  ".$lf"
	fi

	if  [ ! -d  ".$lf/$dir"  ]; then
		mkdir  ".$lf/$dir"
	fi
	#echo $tau
	detail_fold=".$lf/$dir"
		for file in `eval "ls $datafile$dir"`
		do 
			#echo $file
			name="$datafile$dir/$file"
			for ((i=0;i<${#gammas[*]};i++))
			do
			line="$program $name .$lf/$dir/${file%.*}.timelog ${gammas[i]}|tee -a .$lf/$dir/${file%.*}.${gammas[i]}.runtime_log "
			echo $line
#			eval $line
			done


		done
	done
fi


if (($1 == 12))
then
	for ((j=0;j<${#d[*]};j++))
	do
	lf="/E12"
	datafile="../../DataGenerator/build/E12/"
	dir="d${d[j]}"
	if  [ ! -d  ".$lf"  ]; then
		mkdir  ".$lf"
	fi

	if  [ ! -d  ".$lf/$dir"  ]; then
		mkdir  ".$lf/$dir"
	fi
	#echo $tau
	detail_fold=".$lf/$dir"
		for file in `eval "ls $datafile$dir"`
		do 
			#echo $file
			name="$datafile$dir/$file"
#			for ((i=0;i<${#gammas[*]};i++))
#			do
			line="$program $name .$lf/$dir/${file%.*}.timelog ${gam}|tee -a .$lf/$dir/${file%.*}.runtime_log "
			echo $line
			eval $line
#			done


		done
	done
fi



if (($1 == 11))
then
	for ((j=0;j<${#d[*]};j++))
	do
	lf="/E11"
	datafile="../../DataGenerator/build/E11/"
	dir="d${d[j]}"
	if  [ ! -d  ".$lf"  ]; then
		mkdir  ".$lf"
	fi

	if  [ ! -d  ".$lf/$dir"  ]; then
		mkdir  ".$lf/$dir"
	fi
	#echo $tau
	detail_fold=".$lf/$dir"
		for file in `eval "ls $datafile$dir"`
		do 
			#echo $file
			name="$datafile$dir/$file"
#			for ((i=0;i<${#gammas[*]};i++))
#			do
			line="$program $name .$lf/$dir/${file%.*}.timelog ${gam}|tee -a .$lf/$dir/${file%.*}.runtime_log"
			echo $line
#			eval $line
#			done


		done
	done
fi


if (($1 == 21))
then
	for ((j=0;j<${#d[*]};j++))
	do
	lf="/E21"
	datafile="../../DataGenerator/build/E21/"
	dir="d${d[j]}"
	if  [ ! -d  ".$lf"  ]; then
		mkdir  ".$lf"
	fi

	if  [ ! -d  ".$lf/$dir"  ]; then
		mkdir  ".$lf/$dir"
	fi
	#echo $tau
	detail_fold=".$lf/$dir"
		for file in `eval "ls $datafile$dir"`
		do 
			#echo $file
			name="$datafile$dir/$file"
#			for ((i=0;i<${#gammas[*]};i++))
#			do
			line="$program $name .$lf/$dir/${file%.*}.timelog ${gam}|tee -a .$lf/$dir/${file%.*}.runtime_log "
			echo $line
		#	eval $line
#			done


		done
	done
fi




if (($1 == 22))
then
	for ((j=0;j<${#d[*]};j++))
	do
	lf="/E22"
	datafile="../../DataGenerator/build/E22/"
	dir="d${d[j]}"
	if  [ ! -d  ".$lf"  ]; then
		mkdir  ".$lf"
	fi

	if  [ ! -d  ".$lf/$dir"  ]; then
		mkdir  ".$lf/$dir"
	fi
	#echo $tau
	detail_fold=".$lf/$dir"
		for file in `eval "ls $datafile$dir"`
		do 
			#echo $file
			name="$datafile$dir/$file"
#			for ((i=0;i<${#gammas[*]};i++))
#			do
			line="$program $name .$lf/$dir/${file%.*}.timelog ${gam}|tee -a .$lf/$dir/${file%.*}.runtime_log"
			echo $line
		#	eval $line
#			done


		done
	done
fi


if (($1 == 23))
then
	for ((j=0;j<${#d[*]};j++))
	do
	lf="/E23"
	datafile="../../DataGenerator/build/E23/"
	dir="d${d[j]}"
	if  [ ! -d  ".$lf"  ]; then
		mkdir  ".$lf"
	fi

	if  [ ! -d  ".$lf/$dir"  ]; then
		mkdir  ".$lf/$dir"
	fi
	#echo $tau
	detail_fold=".$lf/$dir"
		for file in `eval "ls $datafile$dir"`
		do 
			#echo $file
			name="$datafile$dir/$file"
			for ((i=0;i<${#gammas[*]};i++))
			do
			line="$program $name .$lf/$dir/${file%.*}.timelog ${gammas[i]}|tee -a .$lf/$dir/${file%.*}.${gammas[i]}.runtime_log "
			echo $line
		#	eval $line
			done


		done
	done
fi






if (($1 == 31))
then
	for ((j=0;j<${#d[*]};j++))
	do
	lf="/E31"
	datafile="../../DataGenerator/build/E31/"
	dir="d${d[j]}"
	if  [ ! -d  ".$lf"  ]; then
		mkdir  ".$lf"
	fi

	if  [ ! -d  ".$lf/$dir"  ]; then
		mkdir  ".$lf/$dir"
	fi
	#echo $tau
	detail_fold=".$lf/$dir"
		for file in `eval "ls $datafile$dir"`
		do 
			#echo $file
			name="$datafile$dir/$file"
			#for ((i=0;i<${#gammas[*]};i++))
			#do
			line="$program $name .$lf/$dir/${file%.*}.timelog ${gam}|tee -a .$lf/$dir/${file%.*}.runtime_log "
			echo $line
		#	eval $line
			#done


		done
	done
fi


if (($1 == 32))
then
	for ((j=0;j<${#d[*]};j++))
	do
	lf="/E32"
	datafile="../../DataGenerator/build/E32/"
	dir="d${d[j]}"
	if  [ ! -d  ".$lf"  ]; then
		mkdir  ".$lf"
	fi

	if  [ ! -d  ".$lf/$dir"  ]; then
		mkdir  ".$lf/$dir"
	fi
	#echo $tau
	detail_fold=".$lf/$dir"
		for file in `eval "ls $datafile$dir"`
		do 
			#echo $file
			name="$datafile$dir/$file"
			#for ((i=0;i<${#gammas[*]};i++))
			#do
			line="$program $name .$lf/$dir/${file%.*}.timelog ${gam}|tee -a .$lf/$dir/${file%.*}.runtime_log "
			echo $line
		#	eval $line
			#done


		done
	done
fi















