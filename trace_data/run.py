#!/usr/bin/python

import subprocess

platforms = {
    "ring" : "../platforms/ring/config.xml"
}
hostfiles = {
    "ring" : "../platforms/ring/hostfile.txt"
}
implementation = {
    "bcast" : "../implementation/bcast.c"
}

for imp_name,imp_file in implementation.items() :
    for plat_name,plat_file in platforms.items() :
        subprocess.run(["smpicc", "-O4", imp_file], capture_output=True)

        trace_filename = imp_name + "_" + plat_name + ".trace"
        dump_filename = imp_name + "_" + plat_name + ".dump"

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
            "--cfg=tracing/filename:" + trace_filename], capture_output=True)
        output = subprocess.run(["pj_dump", trace_filename], capture_output=True).stdout.decode("utf-8")

        f = open(dump_filename, "w")
        f.write(output)
        f.close()

