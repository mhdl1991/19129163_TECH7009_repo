from xml.dom.minidom import parse, parseString

# the nmap command will save it's output to this file
xml_file = "nmap_out.xml"

with open(xml_file) as f:
	document = parse(f)



