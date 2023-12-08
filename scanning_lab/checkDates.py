import os
from datetime import datetime, date, timedelta

"""
Usage:
Place yourself in the bricks mother directory (e.g. /home/scanner/sndlhc/RUN1/)
-b                  : allows you to select bricks (-b 21 22 23 24)
--link/--align      : select between linkig or alignment date-checking
--date              : specify the date you to start with in the format dd/mm/yy

Example from command line:  python checkDates.py --date 16/05/23 -b 21 22 23 24 --link
"""

def computeLinkingDates():
    for brick in options.brickIDs:
        for plate in range(1, 58):
            linkfile_path = parent_path+'/b'+str(brick).zfill(6)+'/p'+str(plate).zfill(3)+'/'+str(brick)+'.'+str(plate)+'.0.0.cp.root'
            if not os.path.isfile(linkfile_path): continue
            file_m_time = datetime.fromtimestamp(os.path.getmtime(linkfile_path))
            for week in weeks.keys():
                if week <= file_m_time.date() < week+timedelta(days=7): weeks[week]+=1

def computeAlignDates():
    for brick in options.brickIDs:
        for plate in range(2, 58):
            align_path = parent_path+'/b'+str(brick).zfill(6)+'/AFF/'+str(brick)+'.'+str(plate)+'.0.0_'+str(brick)+'.'+str(plate-1)+'.0.0.aff.par'
            if not os.path.isfile(align_path): continue
            file_m_time = datetime.fromtimestamp(os.path.getmtime(align_path))
            for week in weeks.keys():
                if week <= file_m_time.date() < week+timedelta(days=7): weeks[week]+=1

from argparse import ArgumentParser
parser = ArgumentParser()
parser.add_argument('-b', nargs='+', dest="brickIDs", help='List of bricks', required=False)
parser.add_argument("--link", dest="link", action='store_true', required=False, default=False)
parser.add_argument("--align", dest="align", action='store_true', required=False, default=False)
parser.add_argument("--date", dest="startdate", type=str, required=True, help="Date in dd/mm/yy")
options = parser.parse_args()

today_date = datetime.today()

date_from = datetime.strptime(options.startdate, "%d/%m/%y").date()

parent_path = '.'
weeks = {}
for iweek in range(100):
    if date_from+timedelta(days=iweek*7) > today_date.date(): break
    weeks[date_from+timedelta(days=iweek*7)] = 0

weeklist = list(weeks.keys())
weeklist.sort(key=lambda x: x)

if options.link:
    computeLinkingDates()
elif options.align:
    computeAlignDates()
elif not options.link and not options.align:
    raise Exception("You must specify at least one between link or align options!")

for week in weeklist:
    print('Week of '+str(week)+' n:', weeks[week])
