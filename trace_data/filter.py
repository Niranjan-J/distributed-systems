#!/usr/bin/python
import csv

def find_headers(lines) :
    host_headers = ['host']
    for line in lines :
        if(line.count("State") > 0 and line.count("rank-1") > 0) :
            t = line.split(",")
            host_headers.append(t[7])
    return host_headers

def print_data(file_name) :
    f = open("./dump_files/file_name.dump", "r")
    lines = f.readlines()
            
    links_data = [['source', 'dest', 'hopcount']]
    hosts_map = {}
    hosts_data = []

    for line in lines :
        
        if(line.count("MPI_LINK") > 0) :
            t = line.split(",")[9].split("_")
            src = (int(t[0]))
            dst = (int(t[1]))
            count = (int(t[3]))
            links_data.append([src,dst,count])

        if(line.count("State") > 0) :
            t = line.split(",")
            t[1] = t[1].strip()
            t[7] = t[7].strip()

            if( t[1] not in hosts_map.keys()) :
                hosts_map[t[1]] = {}
            hosts_map[t[1]][t[7]] = max(t[3:7])
            # hosts_data.append([t[1] ])


    # host_map to host_data
    x = list(hosts_map['rank-1'].keys())
    x.insert(0,"host")
    hosts_data.append(x)

    for x in hosts_map.keys() :
        t = []
        t.append(x)
        for y in hosts_map[x].keys() :
            t.append(hosts_map[x][y])
        hosts_data.append(t)

    with open('./csv/link_data/file_name.csv', 'w') as csvFile:
        writer = csv.writer(csvFile)
        writer.writerows(links_data)

    with open('./csv/host_data/file_name.csv', 'w') as csvFile:
        writer = csv.writer(csvFile)
        writer.writerows(hosts_data)
    f.close()
