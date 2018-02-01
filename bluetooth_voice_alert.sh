pkill mpg123
#espeak "Spyder Hub Smart Home Automation system is now connected"
cd /WiFi_For_SpiderHub/text2speech/
rm -r input.wav > /dev/null 2>&1
rm -r output.mp3 > /dev/null 2>&1
pico2wave -w input.wav "$*" #"Spyder Smart Home Automation System is now connected to JBL" #"$*"
lame -b 320 input.wav output.mp3 > /dev/null 2>&1
find . -iname "output.mp3" | mpg123 --list - > /dev/null 2>&1 &

