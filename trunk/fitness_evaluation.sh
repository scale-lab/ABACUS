#!/bin/bash
# Fitness evaluation

Generation=$1
DESIGN_NAME=$2
DESIGN_DIR=$3
POP_DIR=$DESIGN_DIR/Population
ACC_THRESHOLD=$4
ACC_COEF=$5
PWR_COEF=$6
AREA_COEF=$7

if [ -f $POP_DIR'/FilesInfo_G'$Generation'.txt' ]; then
    awk '{if ($2 ~ /^[0-9\.][0-9\.]*$/){ print $0}}' $POP_DIR'/FilesInfo_G'$Generation'.txt' > tmp
    num_line=`cat tmp | sed '/^\s*$/d' | wc -l`

    File_Name=`awk '{if ($2 ~ /^[0-9\.][0-9\.]*$/){ print $1}}' $POP_DIR'/FilesInfo_G'$Generation'.txt'`
    Mean_Acc=`awk '{if ($2 ~ /^[0-9\.][0-9\.]*$/){ print $2}}' $POP_DIR'/FilesInfo_G'$Generation'.txt'`
    Min_Acc=`awk '{if ($2 ~ /^[0-9\.][0-9\.]*$/){ print $3}}' $POP_DIR'/FilesInfo_G'$Generation'.txt'`
    Area_Saving=`awk '{if ($2 ~ /^[0-9\.][0-9\.]*$/){ print $4}}' $POP_DIR'/FilesInfo_G'$Generation'.txt'`
    Power_Saving=`awk '{if ($2 ~ /^[0-9\.][0-9\.]*$/){ print $5}}' $POP_DIR'/FilesInfo_G'$Generation'.txt'`
    Timing_Saving=`awk '{if ($2 ~ /^[0-9\.][0-9\.]*$/){ print $6}}' $POP_DIR'/FilesInfo_G'$Generation'.txt'`
    Add_Saving=`awk '{if ($2 ~ /^[0-9\.][0-9\.]*$/){ print $7}}' $POP_DIR'/FilesInfo_G'$Generation'.txt'`
    echo -e "$File_Name\n\n$Mean_Acc\n\n$Min_Acc\n\n$Area_Saving\n\n$Power_Saving\n\n$Timing_Saving\n\n$Add_Saving"
    Num_Column=`echo $File_Name | awk '{print NF}'`
    if (( Num_Column==0 )); then
        echo -e "Error : reading from "$POP_DIR"/FilesInfo_G"$Generation".txt" >&2
        exit 1
    fi
    
    max_mean_acc=`echo $Mean_Acc | awk '{for (;NF-1;NF--) if ($1<$NF) $1=$NF} 1'`
    echo "max mean acc: $max_mean_acc"

    success=0;
    if (( $(echo "$max_mean_acc <= $ACC_THRESHOLD" | bc -l) )); then
        echo -e "\n# All mutants are of lower accuracy than desired, repeat Generation $Generation"\
            >> $POP_DIR'/FilesInfo_G'$Generation'.txt'
        success=0;
    else
        combine_array=""
        for ((i=1; i<=$num_line; i++));
        do
            mean_acc=`echo $Mean_Acc | awk -v temp=$i '{print $temp}'`
            power_saving=`echo $Power_Saving | awk -v temp=$i '{print $temp}'`
            area_saving=`echo $Area_Saving | awk -v temp=$i '{print $temp}'`
            evolve=`echo $File_Name | awk -v temp=$i '{print $temp}'`

            if [ -z $mean_acc ] || [ -z $power_saving ]; then
                continue;
            fi
            combine=$(echo "scale=4; $mean_acc*$ACC_COEF + $power_saving*$PWR_COEF + $area_saving*$AREA_COEF" | bc)
            echo -e "('$evolve', ($mean_acc, $power_saving))" >> mat2py.txt
            combine_array=`echo "$combine_array $combine"`
        done
        ind=`echo $combine_array | awk -v ind=1 '{for (;NF-1;NF--) if ($1<$NF) {$1=$NF; ind=NF}}{print ind}'`

        evolve=`echo $File_Name | awk -v temp=$ind '{print $temp}'`
        echo $evolve
        echo -e "\n# The best fit approximate design of Generation $Generation is $evolve"\
            >> $POP_DIR'/FilesInfo_G'$Generation'.txt'
        
        if [ -f $POP_DIR/$evolve".v" ]; then
            cp $POP_DIR/$evolve".v" $DESIGN_DIR/SRC/$DESIGN_NAME"1.v"
            success=1
        else
            success=0
        fi
    fi

    if (( success==0 )); then
        echo "Error : No design selected for the next generation."
    else
        echo "Continue on next Generation with $evolve"
    fi
else
    echo "Error : Fail to open FilesInfo_G$Generation.txt" 1>&2
fi
