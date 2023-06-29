import nmapthon2 as nm2

nse = nm2.NSE()
nmap_ip_range =  ['scanme.nmap.org'] # still working this one out
nmap_args = '-sS -Pn -n -T5' #'-sS -Pn -n -T5 --min-hostgroup=2000 --max-rtt-timeout=500ms --min-rate=10000' # given by the paper
nmap_ports = [22, 443] # ports 22 (SSH) and 443 (HTTPS)
nmap_output = 'all'

try:
    print("Attempting nmap scan\n")
    scanner = nm2.NmapScanner()
    result = scanner.scan(nmap_ip_range, ports = nmap_ports, arguments = nmap_args, output = nmap_output)

    print(result.scanner)
    print(result.arguments)
    print(result.start_timestamp)
    print(result.start_datetime)
    print(result.end_timestamp)
    print(result.end_datetime)
    print(result.version)
    print(result.elapsed)
    print(result.summary)
    print(result.exit_status)
    print(result.hosts_up)
    print(result.hosts_down)
    print(result.num_hosts)
    print(result.scan_info)
    print(result.verbose)
    print(result.debug)
    print(result.tolerant_errors)
    

except nm2.exceptions.NmapScanError as e:
    # Errors occurred
    print( 'errors occured: {}'.format(e) )
    
