# SSL certificate fetcher using Python?
import ssl, socket
import nmapthon as nm


nmap_ip_range =  ['127.0.0.1']
nmap_args = '-sS -Pn -n -T5 --min-hostgroup=2000 --max-rtt-timeout=500ms --min-rate=10000'
nmap_ports = [22, 443]

try:
    # Attempt nmap scan
    print("NMAP-BASED IP SCANNING THING!\n")
    nmap_scan = nm.NmapScanner(nmap_ip_range, ports = nmap_ports, arguments = nmap_args )
    nmap_scan.run()
    
    # get information
    print('scan started at {}'.format(nmap_scan.start_time))
    print('Used {} nmap version.'.format(nmap_scan.version))
    print("The tolerant errors were:\n{}".format(nmap_scan.tolerant_errors))
    
    # list of scanned hosts
    for  host in nmap_scan.scanned_hosts():
        #stuff
        print("Host: {}\tState: {}\tReason: {}".format(host, nmap_scan.state(host), nmap_scan.reason(host))
        print("Hostname: {}".format(','.join(nmap_scan.hostnames(host))))
        # Get scanned protocols
        for proto in nmap_scan.all_protocols(host):
            # Get scanned portsl
            for port in nmap_scan.scanned_ports(host, proto):
                state, reason = nmap_scan.port_state(host, proto, port)
                print("Port: {0:<7}State:{1:<9}Reason:{2}".format(port, state, reason))
    
    
except nm.NmapScanError as e:
    # Errors occurred
    print( 'Uh oh! Things went screwy!: {}'.format(e) )
    
    
    
