#run using python 3
#python3 xml_parsing_script_v1.py

import xml.etree.ElementTree as ET
tree = ET.parse("nmap_out.xml")
root = tree.getroot()

#for child in root:
#	print(f'{child.tag} : {child.attrib}')


print("HOSTS:")
for host in root.iter("host"):
	for hostname in host.iter("hostnames/hostname"):
		print(f'{hostname.tag}: {hostname.attrib}')




