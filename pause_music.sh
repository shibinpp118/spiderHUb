flag_m=`cat play_status.txt`
if [ "$flag_m" = "playing" ];
then
pkill -STOP mpg123
echo 'paused' > play_status.txt
elif [ "$flag_m" = "paused" ];
then
pkill -CONT mpg123
echo 'playing' > play_status.txt
else
echo 'stopped' > play_status.txt
fi
