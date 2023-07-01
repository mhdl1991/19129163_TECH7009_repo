#!usr/bin/env/bash

echo "Running nmap"

nmap -sS -p 22,443 -n -T5 --script ssl-enum-ciphers.nse -D RND:10 -oX nmap_out.xml --min-hostgroup=2000 --max-rtt-timeout=500ms --min-rate=10000 "192.168.10.0/24"

#scanme.nmap.org

echo "This is where certificate acquisition should take place using perhaps ssl"

#echo | openssl s_client -servername 

echo "End of bash script" 





