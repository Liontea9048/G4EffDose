from os import chdir, getcwd, listdir, system, makedirs
from os.path import join
import argparse
import ast
from datetime import datetime
from time import sleep


def arg_as_list(s):
    v = ast.literal_eval(s)
    if type(v) is not list:
        raise argparse.ArgumentTypeError("Argument \"%s\" is not a list" % (s))
    return v

def get_args():
    parser = argparse.ArgumentParser(description="cores, nps, workingType, position")

    parser.add_argument("--cores", default=10, help="type the number of cores to be used")
    parser.add_argument("--nps", default=int(1e8), help="type the number of particles to be processed")
    parser.add_argument("--posture", default="op", help="type the posture, op sp sq bd")

    parser.add_argument("--positionList",
                        type=arg_as_list,
                        default=[100],
                        help="type the position of sonde in cm")

    args=parser.parse_args()
    return args

def check_argument(args):
    print(f"info_columns : {args.positionList}")

def modifyExample(numCores, nps):
    # modify example.in file
    with open("./example.in", "w") as f:

        f.write("# Example macro file to run in the batch mode\n\n")

        f.write("# Set the verbose\n")
        f.write("/run/verbose 0\n")
        f.write("/tracking/verbose 0\n\n")

        f.write("# Set the number of threads for multi-threading mode\n")
        f.write(f"/run/numberOfThreads {numCores}\n\n")

        f.write("# Initialize\n")
        f.write("/run/initialize\n\n")

        f.write("# Source setting\n")
        f.write("/control/execute AmbeSource.mac\n\n")

        f.write("# Set the nps\n")
        f.write(f"/run/beamOn {nps}")
        f.close()

def modifyAmbeSource(pos):
    with open("./AmbeSource.mac", "w") as f:
        f.write("#/run/verbose 0\n")
        f.write("#/tracking/verbose 0\n")

        f.write("/gps/particle neutron\n")
        f.write("/gps/pos/type Point\n")
        f.write("/gps/ang/type iso\n")

        # Source Center position
        ### 250.001 -> 0 cm
        ### pos : from 100 cm to - 100 cm. each step gap : 10 cm
        position = 250.001 + int(pos)
        f.write(f"/gps/pos/centre 0.0 0.0 {position} cm # at {pos} cm\n")

        f.write("/gps/ene/type User\n")

        f.write("# Source Energy\n")
        f.write("/gps/hist/type energy\n")
        f.write("/control/execute hist_source_point.dat\n")
        f.close()

def modifyTETDetector(pos):
    system(f"sed -i \"68s/.*/\tG4double sonde_position = {pos}. * cm;/g\" ./src/TETDetectorConstruction.cc")

def log(workingDir, txt):
    file = join(workingDir, "log.txt")
    f = open(file, "a")
    # Get thr current time in YYYY-MM-DD form 
    timeline = datetime.now()
    args = get_args()
    f.write(f"{timeline} : {txt} | args : core: {args.cores}, nps: {args.nps}, posture: {args.posture}, posLst: {args.positionList}\n")

if __name__ == "__main__":
    # args control
    args = get_args()
    check_argument(args)

    # get args
    numCores = args.cores
    nps = args.nps
    posture = args.posture
    posLst = args.positionList

    # get default data path 
    workingDir = getcwd()
    dataDir = "/data/liontea/sonde/geant4"
    date = datetime.today().strftime('%Y-%m-%d')
    outputPath = join(dataDir, date)
    baseDir = join(workingDir, "base")
    srcFileDir = join(workingDir, "srcFiles")
    srcFileDir = join(srcFileDir, f"{posture}")
    
    print(posLst)
    for pos in posLst:
        print(f"-------------------------------------------------------------------------------{pos}")
        inputPath = join(workingDir, f"{posture}")
        inputPath = join(inputPath, f"{pos}")
        try:
            makedirs(inputPath)
        except:
            pass

        # change directory for make
        chdir(inputPath)
        print(inputPath)
        print(getcwd())
        # cp the base templete
        system(f"cp -rf {baseDir}/* ./")
        # cp src files suitable for the case
        system(f"cp -rf {srcFileDir}/* ./src")

        modifyExample(numCores, nps)
        modifyAmbeSource(pos)
        modifyTETDetector(pos)

        try:
            makedirs(outputPath)
        except:
            pass 

        #cmake
        system("cmake ./")
        commands = f"make -j 10 && ./External -m ./example.in -o {outputPath}/{posture}_{pos}.out &" 
        system(commands)
        log(workingDir, commands)


