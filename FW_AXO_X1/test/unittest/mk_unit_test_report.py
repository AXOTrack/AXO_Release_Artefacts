# make release script
# This script build the software by calling west compiler, 
# create a release folder, copy and rename bin and hex file 
# to the release folder and create a meta file with version,
# hardware ID and SHA1 and SHA256 checksums in JSON format
# also in the release folder.
# Finally the script creates a zip file in the release folder
# with the binary and meta file.
# This script is created for the use on LINUX operating systems.
import os
#import shutil
#import hashlib
#import sys
import datetime
#from zipfile import ZipFile


Version = "1.0"
VersionDate = "10-Sep-2025"
VersionProgramName = "mk_unit_test_report"
VersionCopyrightText = "(c) AXO Track GmbH, Andreas Stenger"

REPORT_FILE_NAME = "total_coverage.html"
UNIT_TEST_DIRS = ["bus",
                  "conf",
                  "data",
                   "disp",   # throws exceptions, see comments in releate test cases
                   "drv",
                   "fs",
                   "modem",
                   "net",  # throws exception in two fuctions HTTP_Connector
                   "pm",
                   "rtos",
                   "update",
                   "update2",
                   "util"]

#print(UNIT_TEST_DIRS)
#print(UNIT_TEST_DIRS[1])
#BIN_FILE_NAME = "./build/zephyr/app_update.bin"
#HEX_FILE_NAME = "./build/zephyr/merged.hex"

gLinesExec   = []
gLinesTotal  = []
gLinesCov    = []
gFuncsExec   = []
gFuncsTotal  = []
gFuncsCov    = []
gBranchExec  = []
gBranchTotal = []
gBranchCov   = []

gOverallLinesExec   = 0
gOverallLinesTotal  = 0
gOverallLinesCov    = 0.0
gOverallFuncsExec   = 0
gOverallFuncsTotal  = 0
gOverallFuncsCov    = 0.0
gOverallBranchExec  = 0
gOverallBranchTotal = 0
gOverallBranchCov   = 0.0

STATE_NONE              = 0
STATE_READ_LINES_EXEC   = 1
STATE_READ_LINES_TOTAL  = 2
STATE_READ_LINES_COV    = 3
STATE_READ_FUNC_EXEC    = 11
STATE_READ_FUNC_TOTAL   = 12
STATE_READ_FUNC_COV     = 13
STATE_READ_BRANCH_EXEC  = 21
STATE_READ_BRANCH_TOTAL = 22
STATE_READ_BRANCH_COV   = 23

STATE_EX_NONE     = 0
STATE_EX_SECOND   = 1
STATE_EX_COUNT    = 2

#exceptions = [['Module', 'Exception']]
exceptions = []



def print_usage():
    print("##########################################################")
    print("")
    print("Python Script 'mk_unit_test_report.py'")
    print("")
    #print("Usage:")
    #print("python mk_release.py 'DESCRIPTION-FILE-NAME'")
    #print("DESCRIPTION-FILE-NAME can be empty ''")
    #print("e.g.:")
    #print("python mk_release.py fw_description.txt")
    #print("python mk_release.py ''")
    #print("")
    #print("NOTE: Implemented for usage on Linux PC !")
    print("##########################################################")

def openReportResultFile():
    report_file = open(REPORT_FILE_NAME, 'w')
    report_file.write('<!DOCTYPE html>\n')
    report_file.write('<html class="theme-green">\n')
    report_file.write('\n')
    report_file.write('  <head>\n')
    report_file.write('    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>\n')
    report_file.write('    <title>AXO X1 Code Coverage Report</title>\n')
    report_file.write('    <meta name="viewport" content="width=device-width, initial-scale=1.0"/>\n')
    report_file.write('    <link rel="stylesheet" href="coverage.css"/>\n')
    report_file.write('  </head>\n')
    report_file.write('\n')
    report_file.write('  <body>\n')
    report_file.write('\n')
    return report_file
    
def writeLegend(report_file):
    report_file.write('    <header>\n')
    report_file.write('      <h1>AXO X1 Code Coverage Report</h1>\n')
    report_file.write('\n')
    report_file.write('      <hr/>\n')
    report_file.write('      <div class="summary">\n')
    report_file.write('<div>\n')
    report_file.write('  <table class="legend">\n')
    report_file.write('    <tr>\n')
    report_file.write('      <th scope="row">Directory:</th>\n')
    report_file.write('      <td>.</td>\n')
    report_file.write('    </tr>\n')
    report_file.write('    <tr>\n')
    report_file.write('      <th scope="row">Date:</th>\n')
    now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    report_file.write('      <td>' + now + '</td>\n')   # HIER HIER HIER Datum !!!!
    report_file.write('    </tr>\n')
    report_file.write('    <tr>\n')
    report_file.write('      <th scope="row">Coverage:</th>\n')
    report_file.write('      <td class="legend">\n')
    report_file.write('        <span class="coverage-low">low: &ge; 0%</span>\n')
    report_file.write('        <span class="coverage-medium">medium: &ge; 75.0%</span>\n')
    report_file.write('        <span class="coverage-high">high: &ge; 90.0%</span>\n')
    report_file.write('      </td>\n')
    report_file.write('    </tr>\n')
    report_file.write('  </table>\n')
    report_file.write('</div>\n')
    report_file.write('\n')

def writeOverAllCoverage(report_file):
    gOverallLinesCov1 = round(gOverallLinesCov,1)
    gOverallFuncsCov1 = round(gOverallFuncsCov,1)
    gOverallBranchCov1 = round(gOverallBranchCov,1)
    report_file.write('<div>\n')
    report_file.write('  <table class="coverage">\n')
    report_file.write('    <tr>\n')
    report_file.write('      <th></th>\n')
    report_file.write('      <th scope="col">Exec</th>\n')
    report_file.write('      <th scope="col">Total</th>\n')
    report_file.write('      <th scope="col">Coverage</th>\n')
    report_file.write('    </tr>\n')
    report_file.write('    <tr>\n')
    report_file.write('      <th scope="row">Lines:</th>\n')
    report_file.write('      <td>' + str(int(gOverallLinesExec)) + '</td>\n')
    report_file.write('      <td>' + str(int(gOverallLinesTotal)) + '</td>\n')
    report_file.write('      <td class="coverage-high">' + str(gOverallLinesCov1) + '%</td>\n')
    report_file.write('    </tr>\n')
    report_file.write('    <tr>\n')
    report_file.write('      <th scope="row">Functions:</th>\n')
    report_file.write('      <td>' + str(int(gOverallFuncsExec)) + '</td>\n')
    report_file.write('      <td>' + str(int(gOverallFuncsTotal)) + '</td>\n')
    report_file.write('      <td class="coverage-high">' + str(gOverallFuncsCov1) + '%</td>\n')
    report_file.write('    </tr>\n')
    report_file.write('    <tr>\n')
    report_file.write('      <th scope="row">Branches:</th>\n')
    report_file.write('      <td>' + str(int(gOverallBranchExec)) + '</td>\n')
    report_file.write('      <td>' + str(int(gOverallBranchTotal)) + '</td>\n')
    report_file.write('      <td class="coverage-high">' + str(gOverallBranchCov1) + '%</td>\n')
    report_file.write('    </tr>\n')
    report_file.write('  </table>\n')
    report_file.write('</div>\n')
    report_file.write('      </div>\n')
    report_file.write('      <hr/>\n')
    report_file.write('    </header>\n')
    report_file.write('\n')
    
def checkBranchCovExceptions(folder):
    global exceptions
    
    exCnt = 0;
    exceptions.append(['FOLDER: ' + folder, ''])
    srcDir = './' + folder + '/src'

    for filename in os.listdir(srcDir):
        stateEx = STATE_EX_NONE
        if filename.startswith('test_'):
            #print(filename)
            with open(srcDir + '/' + filename, 'r') as f:
                for line in f:
                    if '// MISSING for 100% branch coverage' in line:
                        stateEx = STATE_EX_SECOND
                    elif '// here code line' in line:
                        stateEx = STATE_EX_COUNT
                    elif '// are not covered completly.' in line:
                        stateEx = STATE_EX_NONE
                    elif stateEx == STATE_EX_COUNT:
                        if 'static PoolElem e1 = {eTimerExpired' in line:
                            exCnt += 2
                        elif 'static PoolElem e2 = {eOneDayTimerExpired' in line:
                            exCnt += 2
                        elif 'static PoolElem e1 = {eMetricsTxTimerExpired' in line:
                            exCnt += 2
                        elif 'static PoolElem e1 = {eBoostCtrlTimerExpired' in line:
                            exCnt += 2
                        elif 'static PoolElem e1 = {eLteStartDelayTimerExpired' in line:
                            exCnt += 2
                        elif 'static PoolElem e1 = {eAdxlWakeupTimerExpired' in line:
                            exCnt += 2
                        else:
                            exCnt += 1
                        exceptions.append([filename, line[3:]])
    return exCnt

def printExceptions(report_file):
    for i in range(len(exceptions)):
        if 'FOLDER:' in exceptions[i][0]:
            if (i > 0):
                report_file.write('</table>\n')
                report_file.write('\n')
            x = exceptions[i][0]
            foldername = x[8:]
            report_file.write('<table class="file-list">\n')
            report_file.write('  <col/>\n')
            report_file.write('  <col/>\n')
            report_file.write('\n')
            report_file.write('  <tr>\n')
            report_file.write('    <th scope="col">Module:  ' + foldername + '</th>\n')
            report_file.write('    <th scope="col">Exception</th>\n')
            report_file.write('  </tr>\n')
        else:    
            report_file.write('  <tr>\n')
            report_file.write('    <th scope="row">\n')
            report_file.write('      ' + exceptions[i][0] + '\n')
            report_file.write('    </th>\n')
            report_file.write('    <td>' + exceptions[i][1] + '</td>\n')
            report_file.write('  </tr>\n')
    report_file.write('</table>\n')
    report_file.write('\n')
    report_file.write('      <hr/>\n')


    

def writeModuleCoverage(report_file):
    report_file.write('    <nav>Module Summary</a></nav>\n')
    report_file.write('    <main>\n')
    report_file.write('<table class="file-list">\n')
    report_file.write('  <col/>\n')
    report_file.write('  <colgroup span="2"/>\n')
    report_file.write('  <colgroup span="2"/>\n')
    report_file.write('  <colgroup span="2"/>\n')
    report_file.write('  <col/>\n')
    report_file.write('\n')
    report_file.write('  <tr>\n')
    report_file.write('    <th scope="col">Module</th>\n')
    report_file.write('    <th scope="colgroup" colspan=2>Lines</th>\n')
    report_file.write('    <th scope="colgroup" colspan=2>Functions</th>\n')
    report_file.write('    <th scope="colgroup" colspan=2>Branches</th>\n')
    report_file.write('    <th scope="col">Exceptions</th>\n')
    report_file.write('  </tr>\n')
    report_file.write('\n')
    report_file.write('\n')

    #n = len(gLinesExec)
    for i in range(len(gLinesExec)):
        report_file.write('  <tr>\n')
        report_file.write('    <th scope="row">\n')
        href = './' + UNIT_TEST_DIRS[i] + '/gcov_report/coverage.html'
        
        report_file.write('      <a href="' + href + '">' + UNIT_TEST_DIRS[i] + '</a>\n')
        report_file.write('    </th>\n')

        if gLinesCov[i] < 100:
            cov_res = 'line-coverage coverage-low'
        else:
            cov_res = 'line-coverage coverage-high'
        report_file.write('    <td class="CoverValue ' + cov_res + '">' + str(gLinesCov[i]) + '%</td>\n')
        report_file.write('    <td class="CoverValue ' + cov_res + '">' + str(gLinesExec[i]) + ' / ' + str(gLinesTotal[i]) + '</td>\n')

        if gFuncsCov[i] < 100:
            cov_res = 'function-coverage coverage-low'
        else:
            cov_res = 'function-coverage coverage-high'
        report_file.write('    <td class="CoverValue ' + cov_res + '">' + str(gFuncsCov[i]) + '%</td>\n')
        report_file.write('    <td class="CoverValue ' + cov_res + '">' + str(gFuncsExec[i]) + ' / ' + str(gFuncsTotal[i]) + '</td>\n')

        exNumber = gBranchTotal[i] - gBranchExec[i]            # number of exeptions found
        exCount = checkBranchCovExceptions(UNIT_TEST_DIRS[i])  # number of exeptions documented
        if exNumber != exCount:
            cov_res = 'branch-coverage coverage-low'
        else:
            cov_res = 'branch-coverage coverage-high'
        report_file.write('    <td class="CoverValue ' + cov_res + '">' + str(gBranchCov[i]) + '%</td>\n')
        report_file.write('    <td class="CoverValue ' + cov_res + '">' + str(gBranchExec[i]) + ' / ' + str(gBranchTotal[i]) + '</td>\n')
        report_file.write('    <td class="MiddleValue ' + cov_res + '">' + str(exNumber) + '</td>\n')
        report_file.write('  </tr>\n')
        report_file.write('\n')

    report_file.write('</table>\n')
    report_file.write('      <hr/>\n')
    report_file.write('    </main>\n')    
    
    report_file.write('\n') 
    
def closeReportResultFile(report_file):
    report_file.write('    <footer>\n')
    report_file.write('      Generated by: AXO python script mk_unit_test_report.py\n')
    report_file.write('    </footer>\n')
    report_file.write('  </body>\n')
    report_file.write('</html>\n')
    report_file.close()
    
def extractNumber(line):
    #print(line)
    pos1 = line.find('<td>')
    pos2 = line.find('</td>')
    s = line[pos1+4:pos2]
    #print(int(s))
    return int(s)
        
def extractPercent(line):
    #print(line)
    pos1 = line.find('>')
    pos2 = line.find('%</td>')
    s = line[pos1+1:pos2]
    if s == '-':
        s = '0.0'
    #print(float(s))
    return float(s)
        
def calcOverAllCoverage():
    global gOverallLinesExec
    global gOverallLinesTotal
    global gOverallLinesCov
    global gOverallFuncsExec
    global gOverallFuncsTotal
    global gOverallFuncsCov
    global gOverallBranchExec
    global gOverallBranchTotal
    global gOverallBranchCov
    for i in range(len(gLinesExec)):
        gOverallLinesExec    += gLinesExec  [i]
        gOverallLinesTotal   += gLinesTotal [i]
        gOverallLinesCov     += gLinesCov   [i]
        gOverallFuncsExec    += gFuncsExec  [i]
        gOverallFuncsTotal   += gFuncsTotal [i]
        gOverallFuncsCov     += gFuncsCov   [i]
        gOverallBranchExec   += gBranchExec [i]
        gOverallBranchTotal  += gBranchTotal[i]
        gOverallBranchCov    += gBranchCov  [i]   
    gOverallLinesCov = (100.0 * gOverallLinesExec) / gOverallLinesTotal 
    gOverallFuncsCov = (100.0 * gOverallFuncsExec) / gOverallFuncsTotal
    gOverallBranchCov = (100.0 * gOverallBranchExec) / gOverallBranchTotal
    
def parseCoverageFile(filename):
    state = STATE_NONE
    with open(filename, 'r') as f:
        for line in f:
            if '<th scope="row">Lines:</th>' in line:
                state = STATE_READ_LINES_EXEC
            elif state == STATE_READ_LINES_EXEC:
                n = extractNumber(line)
                gLinesExec.append(n)
                state = STATE_READ_LINES_TOTAL
            elif state == STATE_READ_LINES_TOTAL:
                n = extractNumber(line)
                gLinesTotal.append(n)
                state = STATE_READ_LINES_COV
            elif state == STATE_READ_LINES_COV:
                p = extractPercent(line)
                gLinesCov.append(p)
                state = STATE_NONE

            if '<th scope="row">Functions:</th>' in line:
                state = STATE_READ_FUNC_EXEC
            elif state == STATE_READ_FUNC_EXEC:
                n = extractNumber(line)
                gFuncsExec.append(n)
                state = STATE_READ_FUNC_TOTAL
            elif state == STATE_READ_FUNC_TOTAL:
                n = extractNumber(line)
                gFuncsTotal.append(n)
                state = STATE_READ_FUNC_COV
            elif state == STATE_READ_FUNC_COV:
                p = extractPercent(line)
                gFuncsCov.append(p)
                state = STATE_NONE
                
            if '<th scope="row">Branches:</th>' in line:
                state = STATE_READ_BRANCH_EXEC
            elif state == STATE_READ_BRANCH_EXEC:
                n = extractNumber(line)
                gBranchExec.append(n)
                state = STATE_READ_BRANCH_TOTAL
            elif state == STATE_READ_BRANCH_TOTAL:
                n = extractNumber(line)
                gBranchTotal.append(n)
                state = STATE_READ_BRANCH_COV
            elif state == STATE_READ_BRANCH_COV:
                p = extractPercent(line)
                gBranchCov.append(p)
                state = STATE_NONE


def main():
#    if len(sys.argv) != 2:
#        print_usage()
#        return
#    else:
#        description_file_name = sys.argv[1]

    reportFile = openReportResultFile()
    writeLegend(reportFile)
    
    for d in UNIT_TEST_DIRS:
        #print(d)
        #print("###############################")
        parseCoverageFile(d + "/gcov_report/coverage.html") 
        
    print("+++++++++++++++++++++++++++++++++++")
    # print(gLinesExec)
    # print(gLinesTotal)
    # print(gLinesCov)
    # print(gFuncsExec)
    # print(gFuncsTotal)
    # print(gFuncsCov)
    # print(gBranchExec)
    # print(gBranchTotal)
    # print(gBranchCov)
    
    calcOverAllCoverage()
    writeOverAllCoverage(reportFile)
    writeModuleCoverage(reportFile)
    printExceptions(reportFile)
        
    closeReportResultFile(reportFile)

if __name__ == "__main__":
    main()
