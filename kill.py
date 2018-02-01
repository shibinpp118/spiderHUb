import psutil
import sys
import time
from Naked.toolshed.shell import execute_js
from Naked.toolshed.shell import muterun_js
pid=[]
pid=psutil.pids()

for i in pid:
    #print i
    p=psutil.Process(i)
    #time.sleep(2)
    p.name()
    #print p.name()
    name=p.name()
    if name=="node-red":
       status = p.status()
       #print status
       time.sleep(2)
       if status=="running":
           print "Process is Running"
       else:
           print "Process is Not running"
           time.sleep(2)
           print "Restarting the process"
           success = execute_js('node1.js')       
    else:
       #print"Error"
       continue







"""
import sys
from Naked.toolshed.shell import execute_js
from Naked.toolshed.shell import muterun_js


for i in range (0,10):

    response = muterun_js('node1.js')

    if response.exitcode == 0:
         print "Node js thread is running"
         #print response.stdout
         break;
    else:
         success = execute_js('node1.js')
         print("Js restarted")
         break;

"""

