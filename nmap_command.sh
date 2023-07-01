#!usr/bin/env/bash

echo "Running nmap"

nmap -sS -p 22,443 -n -T5 --script ssl-enum-ciphers.nse -D RND:10 -oT nmap_out_txt.txt --min-hostgroup=2000 --max-rtt-timeout=500ms --min-rate=10000 scanme.nmap.org

echo "This is where certificate acquisition should take place using perhaps ssl"


echo "End of bash script" 





