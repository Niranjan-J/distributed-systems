#!/usr/bin/python

import subprocess
import os
import csv
import  matplotlib.pyplot as plt

platforms = {
    "ring" : "../platforms/ring/config.xml",
    "hypercube2D" : "../platforms/hypercube-2d/config.xml",
    "hypercube3D" : "../platforms/hypercube-3d/config.xml",
    "torus" : "../platforms/torus/config.xml",
    "hierarchial" : "../platforms/hierarchial/config.xml",
    "ccgraph" : "../platforms/ccgraph/config.xml",
    "mesh" : "../platforms/mesh/config.xml",
    "linear" : "../platforms/linear/config.xml",
}
hostfiles = {
    "ring" : "../platforms/ring/hostfile.txt",
    "hypercube2D" : "../platforms/hypercube-2d/hostfile.txt",
    "hypercube3D" : "../platforms/hypercube-3d/hostfile.txt",
    "torus" : "../platforms/torus/hostfile.txt",
    "hierarchial" : "../platforms/hierarchial/hostfile.txt",
    "ccgraph" : "../platforms/ccgraph/hostfile.txt",
    "mesh" : "../platforms/mesh/hostfile.txt",
    "linear" : "../platforms/linear/hostfile.txt",
}
implementation = {
    "bcast" : "../implementation/bcast.c",
    "allgather" : "../implementation/allgather.c",
    "gather" : "../implementation/gather.c",
    "scatter" : "../implementation/scatter.c",
    "alltoall" : "../implementation/alltoall.c",
    "allreduce" : "../implementation/allreduce.c",
}
    # "reduce" : "../implementation/reduce.c",
    # "reduce_scatter" : "../implementation/reduce_scatter.c",
data_paths = {
    "bcast" : "./plot_data/bcast/",
    "allgather" : "./plot_data/allgather/",
    "gather" : "./plot_data/gather/",
    "scatter" : "./plot_data/scatter/",
    "alltoall" : "./plot_data/alltoall/",
    "allreduce" : "./plot_data/allreduce/",
}

def produce_data() :

    for imp_name,imp_file in implementation.items() :
        for plat_name,plat_file in platforms.items() :
            subprocess.run(["smpicc", "-O4", imp_file], capture_output=True)

            trace_filename = "trace_files/" + imp_name + "_" + plat_name + ".trace"
            # dump_filename = "dump_files/" + imp_name + "_" + plat_name + ".dump"
            dump_filename = imp_name + "_" + plat_name + ".csv"

            data = [["size","time"]]

            compile_proc = subprocess.run(["smpicc", "-O4", imp_file], capture_output=True)
            sz = 1 
            while (sz <= 3000) :
                final_val = 0
                cnt = 0
                for i in range(0,25) :
                    simulate_proc = subprocess.run(["smpirun",
                        "--cfg=smpi/host-speed:1",
                        "-hostfile",
                        hostfiles[plat_name],
                        "-platform",
                        plat_file,
                        "./a.out",
                        str(sz),
                        ], capture_output=True)
                    console_op = simulate_proc.stdout.decode("utf-8").strip()
                    try:
                        final_val += int(console_op)
                        cnt = cnt + 1
                    except:
                        print(console_op)
                if cnt != 0 :
                    data.append([sz,int(final_val/cnt)])
                sz = sz * 2
            print(dump_filename)
            with open(data_paths[imp_name] + dump_filename , 'w') as csvFile:
                writer = csv.writer(csvFile)
                writer.writerows(data)

def plot_data() :
    
    for data_name,data_path in data_paths.items() :
        for filename in os.listdir(data_path) :
            x = []
            y = []
            t_path = data_path + filename
            with open(t_path) as csv_file:
                csv_reader = csv.reader(csv_file, delimiter=',')
                line_count = 0
                for row in csv_reader:
                    if line_count == 0:
                        pass
                    else:
                        x.append(int(row[0]))
                        y.append(int(row[1]))
                    line_count += 1
            plt.plot(x,y,label=filename[0:-4].split('_')[1])
        plt.ylabel('Time')
        plt.xlabel('Data Size')
        plt.title(filename.split('_')[0])
        plt.legend(loc='upper left')
        plt.savefig("./plots/"+filename.split('_')[0]+'.png')
        plt.clf()


produce_data()
plot_data()
