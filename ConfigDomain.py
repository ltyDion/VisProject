import socket
import requests
import subprocess
import os.path
import datetime

currentDT = datetime.datetime.now()
print(str(currentDT) + '\n')

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.connect(('8.8.8.8', 80))
ip = s.getsockname()[0]
s.close()

if os.path.isfile("ip_log.txt"):

    f = open("ip_log.txt", "r+")
    ip_data = f.readline()
    print("current ip is " + ip + "\n")

    if len(ip_data) == 0:
        print("first time running, new ip is " + ip + "\n")
        f.write(str(ip) + "\n")
        response = requests.get('https://www.duckdns.org/update?domains=ltytest&token=f7168886-0b07-4802-9bd9-a6b1dd158746&ip='+ip)
        print(response)
    else:
        #compare to the old ip address
        oldIp = str(ip_data[0:-1])
        print("old ip is " + oldIp + "\n")
        if oldIp != str(ip):
            f.write(ip + "\n")
            response = requests.get('https://www.duckdns.org/update?domains=ltytest&token=f7168886-0b07-4802-9bd9-a6b1dd158746&ip='+ip)
            print(response)

else: # file does not exist before
    print("file does not exist. Creating with new ip " + ip + "\n")
    f = open("ip_log.txt", "w+")
    f.write(str(ip) + "\n")
    response = requests.get('https://www.duckdns.org/update?domains=ltytest&token=f7168886-0b07-4802-9bd9-a6b1dd158746&ip='+ip)
    print(response)

print("\n")
f.close()


