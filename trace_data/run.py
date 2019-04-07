#!/usr/bin/python

import subprocess

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
    "allreduce" : "../implementation/allreduce.c",
    "alltoall" : "../implementation/alltoall.c",
    "gather" : "../implementation/gather.c",
    "reduce" : "../implementation/reduce.c",
    "scatter" : "../implementation/scatter.c",
    "reduce_scatter" : "../implementation/reduce_scatter.c",
}

for imp_name,imp_file in implementation.items() :
    for plat_name,plat_file in platforms.items() :
        subprocess.run(["smpicc", "-O4", imp_file], capture_output=True)

        trace_filename = "trace_files/" + imp_name + "_" + plat_name + ".trace"
        dump_filename = "dump_files/" + imp_name + "_" + plat_name + ".dump"

        compile_proc = subprocess.run(["smpicc", "-O4", imp_file], capture_output=True)
        simulate_proc = subprocess.run(["smpirun",
            "-hostfile",
            hostfiles[plat_name],
            "-platform",
            plat_file,
            "./a.out",
            "--cfg=tracing:yes",
            "--cfg=tracing/smpi:yes",
            "--cfg=tracing/smpi/internals:yes",
            "--cfg=tracing/uncategorized:yes",
            "--cfg=tracing/filename:" + trace_filename], capture_output=True)

        output = subprocess.run(["pj_dump", trace_filename], capture_output=True).stdout.decode("utf-8")

        f = open(dump_filename, "w")
        f.write(output)
        f.close()

