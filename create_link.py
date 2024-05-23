from argparse import ArgumentParser
import os

parser = ArgumentParser()
parser.add_argument("-p", nargs='+', dest="plates", help="plates files", required=False)
parser.add_argument("--mic", dest="microscope", help="microscope", type=int, required=False)
parser.add_argument("--lab", dest="labname", help="labName (NA, BO, CE)", type=str, required=False)
parser.add_argument("--run", dest="run", help="Emulsion run", type=str, required=False)
parser.add_argument("--brickID", dest="brickID", help="brickID", type=int, required=False)
parser.add_argument("-r", nargs='+', dest="rplates", help="plates rescan", required=False)
args = parser.parse_args()

labDict = {'NA': 'Napoli', 'BO': 'Bologna', 'CE': 'CERN'}

brID = args.brickID
wall = int(brID/10)
brick = brID%10
brickfolder = str(brID).zfill(6)

rawdata_prepath = '/eos/experiment/sndlhc/emulsionData/2022/'
raw_data_path_dict = {'NA': labDict[args.labname]+'/SND/mic'+str(args.microscope)+'/RUN'+args.run+'_W'+str(wall)+'_B'+str(brick)+'/', 'BO': labDict[args.labname]+'/'+args.run+'_W'+str(wall)+'_B'+str(brick)+'/', 'CE':labDict[args.labname]+'/SND_mic'+str(args.microscope)+'/RUN'+args.run+'/'+'RUN'+args.run+'_W'+str(wall)+'_B'+str(brick)+'/'}

"""raw_data_path = '/eos/experiment/sndlhc/emulsionData/2022/'+labDict[args.labname]+'/SND/mic'+str(args.microscope)+'/'+args.run+'_W'+str(wall)+'_B'+str(brick)+'/'"""
reco_data_path = '/eos/experiment/sndlhc/emulsionData/2022/emureco_'+labDict[args.labname]+'/RUN'+args.run+'/b'+str(brickfolder)+'/'

if len(args.plates) > 2:
  raise Exception('Too many plate number given, 2 are needed')

rawplatedict = {}
platedict = {}

for plate in range(int(args.plates[1]), int(args.plates[0])+1):
  rawplatedict[plate] = 'P'+str(plate).zfill(2)
  platedict[plate] = 'p'+str(plate).zfill(3)

if args.rplates is not None and len(args.rplates) >0:
  for rplate in args.rplates:
    if args.labname == 'CE':
      rawplatedict[rplate] = 'P'+str(rplate).zfill(2)+'_rescan'
    else:
      rawplatedict[rplate] = 'P'+str(rplate).zfill(2)+'_RESCAN'

for plate in platedict.keys():
  os.system('mkdir '+reco_data_path+'/'+platedict[plate])
  os.system('ln -s '+rawdata_prepath+raw_data_path_dict[args.labname]+'/'+rawplatedict[plate]+'/tracks.raw.root '+reco_data_path+'/'+platedict[plate]+'/'+str(brID)+'.'+str(plate)+'.0.0.raw.root')
  print('created link ', platedict[plate],'to folder', raw_data_path_dict[args.labname]+'/'+rawplatedict[plate])
