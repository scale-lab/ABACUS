#!/bin/bash
#Generation=$1
Output=$1
Original_area=$2
Modified_area=$3
Original_power=$4
Modified_power=$5
Extra_saving=$6
Original_slack=$7
Modified_slack=$8
POP_DIR=$9
if [ -z $Output ] || [ -z $Original_area ] || [ -z $Modified_area ] \
    || [ -z $Original_power ] || [ -z $Modified_power ] || [ -z $Extra_saving ] \
    || [ -z $Original_slack ] || [ -z $Modified_slack ]; then

    echo "Missing one or more report variable"
    echo '-1'
    exit 1
fi

#if [ ! -f $POP_DIR/FilesInfo_G$Generation.txt ]; then
#    echo "Failed to open FilesInfo_G$Generation.txt"
#    echo '-1'
#    exit 1
#fi
                   
                 

Area_saving=$(echo "scale=4; (100*($Original_area - $Modified_area))/$Original_area" | bc);
Power_saving=$(echo "scale=4; (100*($Original_power - $Modified_power))/$Original_power" | bc);
Extra_power_saving=$(echo "scale=4; (100 - ((100*($Original_power - $Extra_saving))/$Original_power))"\
                    | bc);
Net_slack=$(echo "scale=4; $Modified_slack - $Original_slack" | bc);

if  (( $(echo "$Power_saving < -10" | bc -l) )) \
    || (( $(echo "$Area_saving < -10" | bc -l) )); then
    while
         last_line=$(tail -1 "$Output")
          [[ "$last_line" =~ ^$ ]] || [[ "$last_line" =~ ^[[:space:]]+$ ]]
    do
           sed -i '$d' "./file.txt"
    done
    head -n -1 $Output > $POP_DIR/tmp
    mv $POP_DIR/tmp $Output
    echo '-2'
else
    perl -pi -e 'chomp if eof' $Output  
    echo -e "\t\t$Area_saving\t\t$Power_saving\t\t$Net_slack\t\t$Extra_power_saving" \
        >> $Output
    echo '0'
fi
