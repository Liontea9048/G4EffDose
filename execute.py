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
    parser = argparse.ArgumentParser(description="fluxMode, cores, nps, workingType, position, yVarying")

    parser.add_argument("--fluxMode", default=0, help="1 for flux Mode 0 for dose mode, default = 0")
    parser.add_argument("--cores", default=10, help="type the number of cores to be used")
    parser.add_argument("--nps", default=int(1e8), help="type the number of particles to be processed")
    parser.add_argument("--posture", default="op", help="type the posture, op sp sq bd")

    parser.add_argument("--positionList",
                        type=arg_as_list,
                        default=[100],
                        help="type the position of sonde in cm")
    parser.add_argument("--yVarying", default=0, help="1 for yVarying Mode 0 for sonde moving mode, default = 0")
    parser.add_argument("--fixedPos", default=100, help="if yVarying mode is on, you have to choose fixed position of the sonde")

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

def modifyTETDetector(fixedPos, pos, isVarying):
    if isVarying:
        system(f"sed -i \"68s/.*/\tG4double sonde_position = {fixedPos}. * cm;/g\" ./src/TETDetectorConstruction.cc")
        system(f"sed -i \"145s/PhantomBoxY/PhantomBoxY + {pos}. * cm/g\" ./src/TETDetectorConstruction.cc")
    else:
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
    fluxMode = args.fluxMode
    numCores = args.cores
    nps = args.nps
    posture = args.posture
    posLst = args.positionList
    yVarying = args.yVarying
    fixedPos = args.fixedPos

    # get default data path 
    workingDir = getcwd()
    dataDir = "/data/liontea/sonde/geant4"
    date = datetime.today().strftime('%Y-%m-%d')
    outputPath = join(dataDir, date)
    baseDir = join(workingDir, "base")
    fluxDir = join(workingDir, "base_dist_and_flux")
    srcFileDir = join(workingDir, "srcFiles")
    # srcFileDir = join(srcFileDir, f"{posture}")

    def execute(fluxMode, workingDir, posLst, baseOrfluxDir, srcFileDir, numCores, nps, posture, outputPath, fixedPos):
        if fluxMode:
            srcFileDir= join(srcFileDir, f"flux/flux_{posture}")
            for pos in posLst:
                print(f"-------------------------------------------------------------------------------{pos}")
                inputPath = join(workingDir, f"movingY")
                inputPath = join(inputPath, f"{posture}")
                inputPath = join(inputPath, f"{fixedPos}_{pos}")
                try:
                    makedirs(inputPath)
                except:
                    pass

                # change directory for make
                chdir(inputPath)
                print(inputPath)
                # print(getcwd())

                # cp the base templete
                system(f"cp -rf {baseOrfluxDir}/* ./")
                
                # cp src files suitable for the case
                system(f"cp -rf {srcFileDir}/* ./src")

                modifyExample(numCores, nps)
                modifyAmbeSource(fixedPos)
                modifyTETDetector(fixedPos, pos, fluxMode)

                try:
                    makedirs(outputPath)
                except:
                    pass 

            #cmake
            system("cmake ./")
            commands = f"make -j 10 && ./External -m ./example.in -o {outputPath}/{posture}_{fixedPos}_{pos}.out &" 
            system(commands)
            log(workingDir, commands)
        else:
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
                # print(getcwd())

                # cp the base templete
                system(f"cp -rf {baseOrfluxDir}/* ./")
                
                # cp src files suitable for the case
                srcFileDir = join(srcFileDir, f"normal/n_{posture}")
                system(f"cp -rf {srcFileDir}/* ./src")

                modifyExample(numCores, nps)
                modifyAmbeSource(pos)
                modifyTETDetector(fixedPos, pos,fluxMode)

                try:
                    makedirs(outputPath)
                except:
                    pass 

            #cmake
            system("cmake ./")
            commands = f"make -j 10 && ./External -m ./example.in -o {outputPath}/{posture}_{pos}.out &" 
            system(commands)
            log(workingDir, commands)

    if yVarying:
        print(f"position is : {fixedPos}, and posLst are below")
        print("---: ", posLst)
        execute(fluxMode, workingDir, posLst, fluxDir, srcFileDir, numCores, nps, posture, outputPath, fixedPos)
    else:
        print(f"position of the sonde is below ")
        print("---: ", posLst)
        execute(fluxMode, workingDir, posLst, baseDir, srcFileDir, numCores, nps, posture, outputPath, fixedPos)

    