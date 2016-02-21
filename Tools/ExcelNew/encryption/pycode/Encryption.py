#coding=utf-8
'''
#=============================================================================
#     FileName: Encryption.py
#         Desc: 
#       Author: MM
#        Email: 
#     HomePage: 
#      Version: 0.0.6
#   LastChange: 2014-08-31
#=============================================================================
'''
import sys
import os
import getopt
import io
import codecs
import base64
import string

#totalNum(2)/nameLen(2)/nameStr(?)/fileLen(10)/fileStream(?)
class Encryption(object):
    def __init__(self):
        # 来源目录
        self._srcDir = os.getcwd() + "/../plist/"
        # 目标目录
        self._dstDir = os.getcwd() + "/datazip/sanguobigpic/"
        # 目标文件路径
        self._ffPath = self._dstDir + "sanguo.zip"
        # 目标文件
        self._ff = None
        # 加密阵列
        self._codec = (0b010101, 0b101010, 0b110011, 0b001100, 0b101101, 0b011101, 0b011110, 0b100100, 0b100001, 0b100010)

    def bingo(self):
        self._ff = open(self._ffPath, "wb")

        files = os.listdir(self._srcDir)

        totalNum = len(files)
        totalNumStr = "%02d"%(totalNum)
        self._ff.write(self.enhandle(totalNumStr))
        print "totalNum:", totalNumStr

        for f in files:
            if cmp(f[-6:], ".plist") == 0 or cmp(f[-4:], ".xml") == 0:
                self.encryptOneFile(f)
            else:
                print "invalid file for encryption: " + f

        self._ff.flush()
        self._ff.close()
        return 

    def unbingo(self):
        self._ff = open(self._ffPath, "rb")

        totalNumBa = self._ff.read(2)
        totalNum = string.atoi(self.dehandle(totalNumBa))
        print "totalNum:", totalNum

        for i in range(0, totalNum):
            self.decryptOneFile(i)

        self._ff.close()
        return

    #encrypt one file
    def encryptOneFile(self, fName):
        # nameLen
        nameLen = len(fName)
        nameLenStr = "%02d"%(nameLen)
        self._ff.write(self.enhandle(nameLenStr))

        # nameStr
        self._ff.write(self.enhandle(fName))

        # fileLen
        inFile = open(self._srcDir + fName, "rb")
        buffer = inFile.read()
        fileLen = len(buffer)
        fileLenStr = "%10d"%(fileLen)
        self._ff.write(self.enhandle(fileLenStr))

        # fileStr
        self._ff.write(self.enhandle(buffer))

        inFile.close()
        print "encryptOneFile: name=", fName, "len=", fileLen

    #decrypt one file
    def decryptOneFile(self, ord):
        # nameLen
        nameLenBa = self._ff.read(2)
        nameLen = string.atoi(self.dehandle(nameLenBa))

        # nameStr
        nameBa = self._ff.read(nameLen)
        nameStr = self.dehandle(nameBa)

        # fileLen
        fileLenBa = self._ff.read(10)
        fileLen = string.atoi(self.dehandle(fileLenBa))

        # fileStream
        fileBa = self._ff.read(fileLen)
        # fileStream = self.dehandle(fileBa)
        # print "fileStream", fileStream

        print "decryptOneFile: name=", nameStr, "len=", fileLen

    # return bytearray
    def enhandle(self, str):
        ba = bytearray(str)
        baLen = len(ba)
        k = 0
        for i in range(0, baLen):
            ba[i] ^= self._codec[k]
            k += 1
            if k > 9:
                k = 0
        return ba

    # return string
    def dehandle(self, str):
        ba = bytearray(str)
        baLen = len(ba)
        k = 0
        for i in range(0, baLen):
            ba[i] ^= self._codec[k]
            k += 1
            if k > 9:
                k = 0
        return ba.decode("utf-8")        

#base64.b64encode(string)
#base64.b64decode(string)

enc = Encryption()
enc.bingo()
# enc.unbingo()

# #png到etc pvr到png
# class Convert(object):
#     __pic_dirs = ''
#     __pic_ext = []
#     __which_convert = ""

#     def check_extend_name(self, file_name, *extend_name):
#         array = map(file_name.endswith, extend_name)

#         return any(array)

#     def get_command(self, f_name):
#         command = ""
#         #pvr.ccz convert to png
#         if self.__which_convert == "pvr2png":
#             new_file_name = f_name[:-7] + ("png")
#             command = "TexturePacker "+f_name+" --sheet "+new_file_name + " --texture-format png --disable-rotation --allow-free-size"
#         #png convert to pkm
#         if self.__which_convert == "png2pkm":
#             path = os.path.split(f_name)[0]
#             command = "etcpack "+ f_name + " " + path +" -c etc1 -f RGB -as"
        
#         return command


#     def trans_file(self, file_name):
#         if os.path.isfile(file_name):
#             #excute python
#             command = self.get_command(file_name)
#             print "[debug]: " + command
#             cmd = command.split(" ")[0]
#             if self.check_cmd_in_path(cmd) == 1 or self.check_cmd_in_path(cmd) == 2:
#                 os.system(command)
#             else:
#                 print ("command [" + cmd + "] not in system path")
#                 sys.exit(0)
            
#     def list_file_and_trans(self, file_name):
#         if os.path.isdir(file_name):
#             files = os.listdir(file_name)
#             for l in files:
#                 #build path for current file
#                 new_file_path = file_name + os.sep + l

#                 if os.path.isdir(new_file_path):
#                     self.list_file_and_trans(new_file_path)
#                 else:
#                     if self.check_extend_name(new_file_path, self.__pic_ext):
#                         self.trans_file(new_file_path)


#     def get_dirs(self):
#         print "__pic_dirs: ", self.__pic_dirs
#         return self.__pic_dirs

#     def compute_all_dir(self, *file_groups):
#         for file in file_groups:
#             print file
#             self.list_file_and_trans(file)


#     def in_sys_path(self, command):
#         cmdlist = []
#         xtpath = os.getenv('PATH')
#         xtpathlist = xtpath.split(':')
#         for directory in xtpathlist:
#             x = directory + '/' + command
#             cmdlist.append(x)
#         #print cmdlist
#         for cmd in cmdlist:
#             #os.access判断文件是否有可执行权限
#             if os.access(cmd, os.X_OK):
#                 #有执行权限返回值为2
#                 return 2

#     def check_cmd_in_path(self, command):
#         #判断命令参数为/usr/bin/python这种的。
#         if command.startswith('/'):
#             if os.access(command, os.X_OK):
#                 return 1  
#         else:
#             return self.in_sys_path(command)     

#     def get_convert_opt(self):
        
#         try:
#             opts, args = getopt.getopt(sys.argv[1:], "hc:d:", ["help", "convert" ,"dir"])
#             if (opts == []):
#                 print ("[Convert Usage]: Convert.py -d ~/six/pics -c pvr2png")
#                 sys.exit(0)

                
#             for opt, arg in opts:
#                 if opt in ("-h", "--help"):
#                     #print usage
#                     print ("[Convert Usage]: Convert.py -d ~/six/pics -c pvr2png")
#                 if opt in ("-d", "--dir"):
#                     print arg
#                     self.__pic_dirs = arg
#                 if opt in ("-c", "--convert"):
#                     self.__which_convert = arg
#                     if self.__which_convert == "pvr2png":
#                         self.__pic_ext = "pvr.ccz"
#                     elif self.__which_convert == "png2pkm":
#                         self.__pic_ext = "png"
#                     else:
#                         print ("[debug]: param error")
#                         sys.exit(0)
#         except getopt.GetoptError:
#             #output usage
#             print ("[Convert Usage]: Convert.py -d ~/six/pics -c pvr2png")
        

# # convert = Convert()
# # convert.get_convert_opt()
# # convert.compute_all_dir(convert.get_dirs())


