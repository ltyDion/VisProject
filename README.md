# VisProject
cron job
*/5 * * * * /usr/bin/python3 /home/pi/ConfigDomain/ConfigDomain.py >> /home/pi/Desktop/py_log.txt
@reboot echo running python script > /home/pi/Desktop/log.txt
