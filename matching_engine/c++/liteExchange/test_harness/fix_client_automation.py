#!/usr/bin/python
import sys
import socket
import os
import os.path
from sys import platform as _platform
#As Cpython ( default python engine) uses GIL ( https://wiki.python.org/moin/GlobalInterpreterLock )
#using process instead to benefit from multicore : http://stackoverflow.com/questions/1182315/python-multicore-processing
from multiprocessing import Process, Queue, Lock
import threading
from threading import Thread
import time
from datetime import datetime

class Utility:
    CONSOLE_RED = '\033[91m'
    CONSOLE_BLUE = '\033[94m'
    CONSOLE_YELLOW = '\033[93m'
    CONSOLE_END = '\033[0m'

    @staticmethod
    def getCurrentTimeInMilliseconds():
        return int(round(time.time() * 1000))

    @staticmethod
    def pressAnyKey():
        if _platform == "linux" or _platform == "linux2":
            os.system('read -s -n 1 -p "Press any key to continue..."')
        elif _platform == "win32":
            os.system('pause')

    @staticmethod
    def changeWorkingDirectoryToScriptLocation():
        absolutePath = os.path.abspath(__file__)
        dirName = os.path.dirname(absolutePath)
        os.chdir(dirName)

    @staticmethod
    def askQuestion(questionText, defaultAnswer = ""):
        actualQuestionText = questionText
        if len(defaultAnswer) == 0:
            actualQuestionText += " ( Press enter for " + defaultAnswer + " ) "
        actualQuestionText += " : "
        answer = raw_input(actualQuestionText)
        if not answer:
            answer = defaultAnswer
        return answer

    @staticmethod
    def yesNoQuestion(questionText):
        actualQuestionText = questionText + " ( Y/y or N/n ) : "
        retVal = False
        while True:
            answer = raw_input(actualQuestionText)
            answer = answer.lower()
            if not answer:
                continue

            if answer == "y" or answer == "yes":
                retVal = True
                break

            if answer == "n" or answer == "no":
                break

        return  retVal


    @staticmethod
    def writeColorMessage(message, colorCode):
        if _platform == "linux" or _platform == "linux2":
            print(colorCode + message + Utility.CONSOLE_END)
        elif _platform == "win32":
            os.system("echo " + message)

    @staticmethod
    def writeMessage(message):
        Utility.writeColorMessage(message, Utility.CONSOLE_BLUE)

    @staticmethod
    def writeErrorMessage(message):
        Utility.writeColorMessage(message, Utility.CONSOLE_RED)

    @staticmethod
    def writeInfoMessage(message):
        Utility.writeColorMessage(message, Utility.CONSOLE_YELLOW)

class StopWatch:
    def __init__(self):
        self.startTime = 0
        self.endTime = 0

    def start(self):
        self.startTime = Utility.getCurrentTimeInMilliseconds()

    def stop(self):
        self.endTime = Utility.getCurrentTimeInMilliseconds()

    def elapsedTimeInMilliseconds(self):
        return self.endTime - self.startTime


class FixConstants:
    # GENERAL
    EQUALS = '='
    DELIMITER = chr(1)
    # VERSIONS
    VERSION_4_0 = "FIX.4.0"
    VERSION_4_1 = "FIX.4.1"
    VERSION_4_2 = "FIX.4.2"
    VERSION_4_3 = "FIX.4.3"
    VERSION_4_4 = "FIX.4.4"
    VERSION_5_0 = "FIX.5.0"
    VERSION_5_0SP1 = "FIX.5.0SP1"
    VERSION_5_0SP2 = "FIX.5.0SP2"
    # TIMESTAMP PRECISION
    TIMESTAMP_PRECISION_SECONDS=1
    TIMESTAMP_PRECISION_MILLISECONDS=2
    TIMESTAMP_PRECISION_MICROSECONDS=3
    # HEADER TAGS
    TAG_VERSION = 8                 #REQUIRED
    TAG_BODY_LENGTH = 9             #REQUIRED
    TAG_MESSAGE_TYPE = 35           #REQUIRED
    TAG_SENDER_COMPID = 49          #REQUIRED
    TAG_TARGET_COMPID = 56          #REQUIRED
    TAG_ON_BEHALF_OF_COMPID = 115
    TAG_DELIVER_TO_COMPID = 128
    TAG_SECURE_DATA_LEN = 90
    TAG_SECURE_DATA = 91
    TAG_SEQUENCE_NUMBER = 34        #REQUIRED
    TAG_SENDER_SUBID = 50
    TAG_SENDER_LOCATION_ID = 142
    TAG_TARGET_SUBID = 57
    TAG_TARGET_LOCATION_ID = 143
    TAG_ON_BEHALF_OF_SUBID = 116
    TAG_ON_BEHALF_OF_LOCATIONID = 144
    TAG_DELIVER_TO_SUBID = 129
    TAG_DELIVER_TO_LOCATIONID = 145
    TAG_POSS_DUPP_FLAG=43
    TAG_POSS_RESEND=97
    TAG_SENDING_TIME = 52           #REQUIRED
    TAG_ORIG_SENDING_TIME = 122
    TAG_XML_DATA_LEN = 212
    TAG_XML_DATA = 213
    TAG_MESSAGE_ENCODING = 347
    TAG_LAST_MSQ_SEQ_NUM_PROCESSED = 369
    TAG_NO_HOPS = 627
    TAG_HOP_COMP_ID = 628
    TAG_HOP_SENDING_TIME = 629
    TAG_HOP_REF_ID = 630
    # HEADER TAGS ORDER , NEED TO BUILD FIX HEADER TAGS IN THIS ORDER
    HEADER_TAGS = [ 
                        TAG_VERSION, 
                        TAG_BODY_LENGTH,
                        TAG_MESSAGE_TYPE, 
                        TAG_SENDER_COMPID,
                        TAG_TARGET_COMPID,
                        TAG_ON_BEHALF_OF_COMPID,
                        TAG_DELIVER_TO_COMPID,
                        TAG_SECURE_DATA_LEN,
                        TAG_SECURE_DATA,
                        TAG_SEQUENCE_NUMBER,
                        TAG_SENDER_SUBID,
                        TAG_SENDER_LOCATION_ID,
                        TAG_TARGET_SUBID,
                        TAG_TARGET_LOCATION_ID,
                        TAG_ON_BEHALF_OF_SUBID,
                        TAG_ON_BEHALF_OF_LOCATIONID,
                        TAG_DELIVER_TO_SUBID,
                        TAG_DELIVER_TO_LOCATIONID,
                        TAG_POSS_DUPP_FLAG,
                        TAG_POSS_RESEND,
                        TAG_SENDING_TIME,
                        TAG_ORIG_SENDING_TIME,
                        TAG_XML_DATA_LEN,
                        TAG_XML_DATA,
                        TAG_MESSAGE_ENCODING,
                        TAG_LAST_MSQ_SEQ_NUM_PROCESSED,
                        TAG_NO_HOPS,
                        TAG_HOP_COMP_ID,
                        TAG_HOP_SENDING_TIME,
                        TAG_HOP_REF_ID
                      ]
    # FIX BIDIRECTIONAL HEADER TAGS, NEED TO KNOW CORRESPONDING HEADER TAGS FOR FIX SESSIONS
    BIDIRECTIONAL_HEADER_TAGS = [ 
                                      [TAG_SENDER_COMPID, TAG_TARGET_COMPID],
                                      [TAG_SENDER_SUBID, TAG_TARGET_SUBID],
                                      [TAG_SENDER_LOCATION_ID, TAG_TARGET_LOCATION_ID]
                                    ]
    # TRAILER TAGS
    TAG_SIGNAGURE_LENGTH = 93
    TAG_SIGNATURE = 89
    TAG_BODY_CHECKSUM = 10          #REQUIRED
    TRAILER_TAGS = [ 
                        TAG_SIGNAGURE_LENGTH,
                        TAG_SIGNATURE,
                        TAG_BODY_CHECKSUM
                      ]
    # BODY TAGS
    TAG_AVERAGE_PRICE = 6
    TAG_CLIENT_ORDER_ID = 11
    TAG_CUMULATIVE_QUANTITY = 14
    TAG_EXEC_ID = 17
    TAG_EXEC_INST = 18
    TAG_EXEC_TRANSTYPE = 20
    TAG_HAND_INST = 21
    TAG_LAST_PRICE = 31
    TAG_LAST_QUANTITY = 32
    TAG_ORDER_ID = 37
    TAG_ORDER_QUANTITY = 38
    TAG_ORDER_STATUS = 39
    TAG_ORDER_TYPE = 40
    TAG_ORIG_CLIENT_ORDER_ID = 41
    TAG_ORDER_PRICE = 44
    TAG_SECURITY_ID = 48
    TAG_ORDER_SIDE = 54
    TAG_SYMBOL = 55
    TAG_FREE_TEXT = 58
    TAG_TIME_IN_FORCE = 59
    TAG_TRANSACTION_TIME = 60
    TAG_ENCRYPT_METHOD = 98
    TAG_HEARTBEAT_INTERVAL = 108
    TAG_TEST_REQ_ID = 112
    TAG_EXEC_TYPE = 150
    TAG_LEAVES_QTY = 151
    TAG_USERNAME = 553
    TAG_PASSWORD = 554
    TAG_USER_REQUEST_ID = 923
    TAG_USER_PASSWORD = 924
    # MESSAGE TYPES
    MESSAGE_HEARTBEAT = "0"
    MESSAGE_TEST_REQUEST = "1"
    MESSAGE_ADMIN_REJECT = "3"
    MESSAGE_USER_LOGON = "BE"
    MESSAGE_USER_RESPONSE = "BF"
    MESSAGE_LOG_ON = "A"
    MESSAGE_LOG_OFF = "5"
    MESSAGE_EXECUTION_REPORT = "8"
    MESSAGE_ORDER_CANCEL_REJECT = "9"
    MESSAGE_NEW_ORDER = "D"
    MESSAGE_AMEND_ORDER = "G"
    MESSAGE_CANCEL_ORDER = "F"
    MESSAGE_BUSINES_REJECT = "j"
    # ORDER STATUS
    ORDER_STATUS_NEW = '0'
    ORDER_STATUS_PARTIALLY_FILLED = '1'
    ORDER_STATUS_FILLED = '2'
    ORDER_STATUS_DONE_FOR_DAY = '3'
    ORDER_STATUS_CANCELED = '4'
    ORDER_STATUS_REPLACED = '5'
    ORDER_STATUS_PENDING_CANCEL = '6'
    ORDER_STATUS_STOPPED = '7'
    ORDER_STATUS_REJECTED = '8'
    # ORDER TYPE
    ORDER_TYPE_MARKET= '1'
    ORDER_TYPE_LIMIT = '2'
    # SIDE
    ORDER_SIDE_BUY = '1'
    ORDER_SIDE_SELL = '2'
    # TIME IN FORCE
    ORDER_TIF_DAY = '0'
    # ENCRYPTION METHODS
    ENCRYPTION_NONE = '0'

    @staticmethod
    def isHeaderTag(tag):
        for headerTag in FixConstants.HEADER_TAGS:
            if headerTag == tag:
                return True
        return False

    @staticmethod
    def isTrailerTag(tag):
        for trailerTag in FixConstants.TRAILER_TAGS:
            if trailerTag == tag:
                return True
        return False


class FixMessage:
    def __init__(self, tagValueTupleArray=None):
        if tagValueTupleArray:
            self.setTags(tagValueTupleArray)
        self.__tagValuePairs = [] # Array of tuples

    def hasTag(self, tag):
        for tagValuePair in self.__tagValuePairs:
            currentTag, currentValue = tagValuePair
            if tag == currentTag:
                return True
        return False

    # Index and count use for supporting repeating groups
    def getTagValue(self, tag, index=1):
        count = 0
        for tagValuePair in self.__tagValuePairs:
            currentTag, currentValue = tagValuePair
            if tag == currentTag:
                count = count + 1
                if count == index:
                    return currentValue
        return None

    def setTag(self, tag, value, index=1):
        if self.hasTag(tag) == False:
            self.appendTag(tag, value)
            return True
        # Dealing with a repeating group tag
        
        # Early exit
        if index == 1:
            return False

        # This part is for re setting value of an existing repeating group tag
        count = 0
        listIndex = -1
        for tagValuePair in self.__tagValuePairs:
            listIndex = listIndex + 1
            currentTag, currentValue = tagValuePair
            if tag == currentTag:
                count = count + 1
                if count == index:
                    self.__tagValuePairs[listIndex] = tuple((tag, str(value)))
                    return True

        # If reached here , append repeating group tag if index specified correctly
        if index == count+1:
            self.appendTag(tag, value)
            return True
        # Did not add , check arguments
        return False

    def appendTag(self, tag, value):
        self.__tagValuePairs.append( tuple((tag, str(value))) )

    def removeTag(self, tag, index = 1):
        if self.hasTag(tag) == False:
            return
        count = 0
        listIndex = -1
        for tagValuePair in self.__tagValuePairs:
            listIndex = listIndex + 1
            currentTag, currentValue = tagValuePair
            if tag == currentTag:
                count = count + 1
                if count == index:
                    del self.__tagValuePairs[listIndex]
                    return
        
    def setTags(self , tagValueTupleArray):
        for tagValuePair in tagValueTupleArray:
            tag, value = tagValuePair
            self.appendTag(tag, value)

    def hasTags(self, tagArray):
        for tag in tagArray:
            if self.hasTag(tag) == False:
                return False
        return True

    def removeTags(self, tagArray):
        for tag in tagArray:
            self.removeTag(tag)

    def getMessageType(self):
        return self.getTagValue(FixConstants.TAG_MESSAGE_TYPE)

    def loadFromString(self, input=""):
        tagValuePairs = input.split(FixConstants.DELIMITER)
        for tagValuePair in tagValuePairs:
            if len(tagValuePair) < 3:
                continue
            tokens = tagValuePair.split(FixConstants.EQUALS)
            tag = tokens[0]
            value = tokens[1]
            self.appendTag(int(tag), value)

    @staticmethod
    def loadFromFile(fileName):
        fixMessages = []
        with open(fileName, "r") as fileContent:
            for line in fileContent:
                if line.startswith('#') is False:
                    fixMessage = FixMessage()
                    fixMessage.loadFromString(line)
                    fixMessages.append(fixMessage)
        return fixMessages

    @staticmethod
    def calculateChecksum(message):
        checksum = ""
        sum = 0
        for c in message:
            sum += ord(c)
        sum = sum % 256
        checksum = str(sum)
        checksum.zfill(3)
        return checksum

    def calculateBodyLength(self):
        bodyLength = 0
        for tagValue in self.__tagValuePairs:
            tag , value = tagValue
            if tag is FixConstants.TAG_VERSION:
                continue
            if tag is FixConstants.TAG_BODY_LENGTH:
                continue
            if tag is FixConstants.TAG_BODY_CHECKSUM:
                continue
            bodyLength += len(str(tag)) + len( str(value) ) + 2 # +2 is because of = and delimiter
        return bodyLength

    def toString(self, sendingAsMessage = False, usePipeAsDelimiter = False):
        # If it was Python3 could use a non local variable, instead  making string a mutable array
        messageAsString = [""]
        
        def appendToMessageAsString(tag, value = ""):
            if len(value) == 0:
                value = self.getTagValue(tag)
            messageAsString[0] += str(tag) + FixConstants.EQUALS + value
            if usePipeAsDelimiter is False:
                messageAsString[0] += FixConstants.DELIMITER
            else:
                messageAsString[0] += '|'

        # CALCULATE BODY LENGTH
        if sendingAsMessage is True:
            bodyLength = str(self.calculateBodyLength())
            self.setTag(FixConstants.TAG_BODY_LENGTH, bodyLength)
            
        # ADD HEADER TAGS
        for headerTag in FixConstants.HEADER_TAGS:
            if self.hasTag(headerTag):
                appendToMessageAsString(headerTag)

        # ADD BODY TAGS
        for tagValue in self.__tagValuePairs:
            tag, value = tagValue
            if FixConstants.isHeaderTag(tag) is True:
                continue
            if FixConstants.isTrailerTag(tag) is True:
                continue
            appendToMessageAsString(tag, value)

        # ADD TRAILER TAGS EXCLUDING CHECKSUM
        for trailerTag in FixConstants.TRAILER_TAGS:
            if self.hasTag(trailerTag):
                if trailerTag != FixConstants.TAG_BODY_CHECKSUM:
                    appendToMessageAsString(trailerTag)

        # ADD CHECKSUM
        if sendingAsMessage is True:
            checksumValue = FixMessage.calculateChecksum(messageAsString[0])
            appendToMessageAsString(FixConstants.TAG_BODY_CHECKSUM, checksumValue)
        else:
            if self.hasTag(FixConstants.TAG_BODY_CHECKSUM):
                appendToMessageAsString(FixConstants.TAG_BODY_CHECKSUM)

        return messageAsString[0]
    
    def toReadableString(self):
        ret = self.toString(False, True)
        return ret

    def __str__(self):
        return self.toReadableString()
    
        
class FixTCPTransport:
    def __init__(self):
        self.networkTimeoutInSeconds = 0
        self.targetAddress = ""
        self.targetPort = 0
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setblocking(True)
        self.socket.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        self.serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.serverSocket.setblocking(True)
        self.serverSocket.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)

    def setNetworkTimeoutInSeconds(self, timeout):
        self.networkTimeoutInSeconds = timeout

    def connect(self):
        connected = False
        message = None
        # Not supporting timeouts in accept and connect methods
        originalTimeoutInSeconds = self.networkTimeoutInSeconds
        self.networkTimeoutInSeconds = 0
        try:
            while True:
                try:
                    self.socket.connect((self.targetAddress, self.targetPort))
                    connected = True
                    break
                except socket.error, v:
                    errorCode = v[0]
                    if errorCode == 10061: # Remote actively refused it
                        continue
                    raise Exception(v)
        except Exception as e:
            exceptionMessage = "Error during a connection attempt : "
            exceptionMessage += str(e)
            if message != None:
                exceptionMessage += "\n"
                exceptionMessage += message.toString()
            print(exceptionMessage)
        self.networkTimeoutInSeconds = originalTimeoutInSeconds
        return  connected
        
    def accept(self):
        connected = False
        message = None
        # Not supporting timeouts in accept and connect methods
        originalTimeoutInSeconds = self.networkTimeoutInSeconds
        self.networkTimeoutInSeconds = 0
            
        try:
            self.serverSocket.bind( (self.targetAddress, self.targetPort))
            self.serverSocket.listen(100)
            self.socket, address = self.serverSocket.accept()
            connected = True                            
        except Exception as e:
            exceptionMessage = "Error during a connection attempt : "
            exceptionMessage += str(e)
            if message != None:
                exceptionMessage += "\n"
                exceptionMessage += message.toString()
            print(exceptionMessage)
        self.networkTimeoutInSeconds = originalTimeoutInSeconds
        return  connected

    def close(self):
        self.serverSocket.close()
        self.socket.close()
            
    def enableSocketBlocking(self):
        self.socket.settimeout(None) # Put socket back into blocking mode
        
    def arrangeSocketTimeOut(self):
        if self.networkTimeoutInSeconds > 0:
            self.socket.settimeout(self.networkTimeoutInSeconds)

    def send(self, message):
        self.arrangeSocketTimeOut()
        sentBytes = 0
        
        try:
            sentBytes = self.socket.send( message.toString(True) )
        except socket.timeout:
            sentBytes = 0
        
        self.enableSocketBlocking()
        
        return sentBytes > 0

    def recvString(self, size):
        data = ""
        
        try:
            data = self.socket.recv(size)
        except socket.error:
            data = None
            print "Socket error"
        return data

    def recv(self):
        self.arrangeSocketTimeOut()
        message = None
        try:
            initialBuffer = self.recvString(20) # Length of 8=FIX.4.2@9=7000@35= so we always get 35=A

            if initialBuffer is None:
                raise Exception("Receive failed")

            if len(initialBuffer) is 0:
                raise Exception("Receive failed")

            # Find all bytes from tag body length
            allBytes = int ( initialBuffer.split(FixConstants.DELIMITER)[1].split(FixConstants.EQUALS)[1] )
            # Calculate remaining bytes
            remainingBytes = allBytes - 20 + initialBuffer.find("35=")
            remainingBytes += 7 #7 is because of 10=081@
            restOfBuffer = self.recvString(remainingBytes)
            
            if restOfBuffer is None:
                raise Exception("Receive failed")

            if len(restOfBuffer) is 0 and remainingBytes > 0:
                raise Exception("Receive failed")
            
            message = FixMessage()
            message.loadFromString( initialBuffer + restOfBuffer )

        except Exception as e:
            print( "Error during recv : " + str(e))
        
        self.enableSocketBlocking() # Put socket back into blocking mode # Put socket back into blocking mode
        return  message

class FixSession:
    def __init__(self):
        self.connected = False
        self.server = False
        self.restoreSequenceNumberFromFile = False
        self.incomingSequenceNumber = 1
        self.incomingSequenceNumber = 1
        self.heartbeatInterval = 30                                 #Required for logon message
        self.encryptionMethod = FixConstants.ENCRYPTION_NONE    #Required for logon message
        self.fixVersion = ""                                        #Required for every message
        self.senderCompid = ""                                      #Required for every message
        self.targetCompid = ""                                      #Required for every message
        self.additionalHeaderTagValuePairs = [] # Array of tuples, will be applied to each message for ex senderSubId & targetSubId
        self.timePrecision = FixConstants.TIMESTAMP_PRECISION_MILLISECONDS
        self.fixTransport = FixTCPTransport()
        self.heartbeatTimer = None
        self.mutex = Lock()

    @staticmethod
    def getCurrentUTCDateTimeSeconds():
        retVal = datetime.utcnow().strftime("%Y%m%d-%H:%M:%S")
        return retVal

    @staticmethod
    def getCurrentUTCDateTimeMilliSeconds():
        retVal = datetime.utcnow().strftime("%Y%m%d-%H:%M:%S.%f")[:-3]
        return retVal
        
    @staticmethod
    def getCurrentUTCDateTimeMicroSeconds():
        retVal = datetime.utcnow().strftime("%Y%m%d-%H:%M:%S.%f")
        return retVal
        
    def getCurrentUTCDateTime(self):
        if self.timePrecision == FixConstants.TIMESTAMP_PRECISION_MICROSECONDS:
            return FixSession.getCurrentUTCDateTimeMicroSeconds()
        if self.timePrecision == FixConstants.TIMESTAMP_PRECISION_MILLISECONDS:
            return FixSession.getCurrentUTCDateTimeMilliSeconds()
        return FixSession.getCurrentUTCDateTimeSeconds()
        
    def setUseSequenceNumberFile(self, value):
        self.restoreSequenceNumberFromFile = value

    def addHeaderTagValuePair(self, tag, value):
        self.additionalHeaderTagValuePairs.append((tag, value))

    def addHeaderTagValuePairs(self, tagValueTupleArray):
        for tagValue in tagValueTupleArray:
            self.additionalHeaderTagValuePairs.append(tagValue)
        
    def setTimePrecision(self, precision):
        self.timePrecision = precision

    def getBaseMessage(self, messageType):
        message = FixMessage()
        message.setTag(FixConstants.TAG_MESSAGE_TYPE, messageType)
        return  message

    def getLogonMessage(self):
        message = self.getBaseMessage(FixConstants.MESSAGE_LOG_ON)
        message.setTag(FixConstants.TAG_ENCRYPT_METHOD, self.encryptionMethod)
        message.setTag(FixConstants.TAG_HEARTBEAT_INTERVAL, self.heartbeatInterval)
        return message

    def getLogoffMessage(self):
        message = self.getBaseMessage(FixConstants.MESSAGE_LOG_OFF)
        return message

    def getHeartbeatMessage(self):
        message = self.getBaseMessage(FixConstants.MESSAGE_HEARTBEAT)
        return message

    def getSequenceFileName(self):
        ret = (self.senderCompid) + "_" + (self.targetCompid) + "_sequence.txt"
        return ret

    def saveSequenceNumberToFile(self):
        fileName = self.getSequenceFileName()
        if os.path.exists(fileName):
            os.remove(fileName)
        with open(fileName, "w") as textFile:
            textFile.write(str(self.incomingSequenceNumber) + "," + str(self.incomingSequenceNumber))

    def restoreSequenceNumber(self):
        incomingNumber = 1
        outgoingNumber = 1
        try:
            fileName = self.getSequenceFileName()

            if os.path.exists(fileName):
                with open(fileName, "r") as fileContent:
                    line = fileContent.readline()
                    outgoingNumberString = line.split(',')[0]
                    incomingNumberString = line.split(',')[1]

                    if incomingNumberString.isdigit() is True:
                        incomingNumber = int(incomingNumberString)

                    if outgoingNumberString.isdigit() is True:
                        outgoingNumber = int(outgoingNumberString)
        except:
            print("Warning : Error during opening sequence number file , sequence numbers set to 1")

        self.incomingSequenceNumber = incomingNumber
        self.incomingSequenceNumber = outgoingNumber

    def connect(self, logonMessage):
        message = None

        try:
            if self.fixTransport.connect() == False:
                return False

            if self.restoreSequenceNumberFromFile is True:
                self.restoreSequenceNumber()

            if logonMessage is None:
                logonMessage = self.getLogonMessage()
            
            self.send(logonMessage)
            message = self.recv()

            if message.getMessageType() != FixConstants.MESSAGE_LOG_ON:
                raise Exception("Incoming message was not a logon response")            
            
            if self.restoreSequenceNumberFromFile is False:
                self.incomingSequenceNumber = int( message.getTagValue(FixConstants.TAG_SEQUENCE_NUMBER) )
                
            self.heartbeatTimer = threading.Timer(self.heartbeatInterval, self.heartbeatFunction)
            self.heartbeatTimer.start()
            
            self.connected = True
        except Exception as e:
            exceptionMessage = "Error during a connection attempt : "
            exceptionMessage += str(e)
            if message != None:
                exceptionMessage += "\n"
                exceptionMessage += message.toString()
            print(exceptionMessage)

        return  self.connected
        
    def accept(self, logOnResponse):
        message = None
            
        try:
            if self.fixTransport.accept() == False:
                return False
            
            message = self.recv()

            if message.getMessageType() != FixConstants.MESSAGE_LOG_ON:
                raise Exception("Incoming message was not a logon message")                
                
            if self.restoreSequenceNumberFromFile is False:
                self.incomingSequenceNumber = int( message.getTagValue(FixConstants.TAG_SEQUENCE_NUMBER) )
            else:
                self.restoreSequenceNumber()
                
            self.heartbeatInterval = int( message.getTagValue(FixConstants.TAG_HEARTBEAT_INTERVAL) )
            self.targetCompid = message.getTagValue(FixConstants.TAG_SENDER_COMPID)
            
            self.fixVersion = message.getTagValue(FixConstants.TAG_VERSION)
            
            if message.hasTag(FixConstants.TAG_ENCRYPT_METHOD):
                self.encryptionMethod = int( message.getTagValue(FixConstants.TAG_ENCRYPT_METHOD) )

            for bidirectionalTagPair in FixConstants.BIDIRECTIONAL_HEADER_TAGS:
                tag = bidirectionalTagPair[0]
                if tag == FixConstants.TAG_SENDER_COMPID:
                    continue
                correspondingTag = bidirectionalTagPair[1]
                if message.hasTag(tag):
                    value = message.getTagValue(tag)
                    self.addHeaderTagValuePair(correspondingTag, value)
            
            if logOnResponse is None:
                logOnResponse = self.getLogonMessage()
            
            self.send(logOnResponse)
            
            self.connected = True                            
        except Exception as e:
            exceptionMessage = "Error during a connection attempt : "
            exceptionMessage += str(e)
            if message != None:
                exceptionMessage += "\n"
                exceptionMessage += message.toString()
            print(exceptionMessage)

        return  self.connected

    def heartbeatFunction(self):
        self.send(self.getHeartbeatMessage())

    def disconnect(self, logoffMessage = None):
        if self.connected:
            if self.heartbeatTimer:
                self.heartbeatTimer.cancel()
            try:
                if logoffMessage is None:
                    logoffMessage = self.getLogoffMessage()
                self.send(logoffMessage)
                if self.server == False:
                    logoffResponse = self.recv()
            except:
                pass
            finally:
                self.fixTransport.close()
            self.connected = False
            self.saveSequenceNumberToFile()

    def lock(self):
        self.mutex.acquire()

    def unlock(self):
        self.mutex.release()

    def send(self, message):
        message.setTag(FixConstants.TAG_VERSION, self.fixVersion)
        message.setTag(FixConstants.TAG_SENDER_COMPID, self.senderCompid)
        message.setTag(FixConstants.TAG_TARGET_COMPID, self.targetCompid)
        message.setTag(FixConstants.TAG_SENDING_TIME, self.getCurrentUTCDateTime())

        #Apply additional session header tag value pairs
        for tagValuePair in self.additionalHeaderTagValuePairs:
            currentTag, currentValue = tagValuePair
            message.setTag(currentTag, currentValue)

        self.lock()
        
        message.setTag(FixConstants.TAG_SEQUENCE_NUMBER, self.incomingSequenceNumber)

        success = self.fixTransport.send( message )

        if success == True:
            self.incomingSequenceNumber += 1
        
        self.unlock()
        
        return success

    def recv(self):
        self.lock()

        message = None
        message = self.fixTransport.recv()

        if message != None:
            self.incomingSequenceNumber = int(message.getTagValue(FixConstants.TAG_SEQUENCE_NUMBER))
        
        self.unlock()
        
        return  message

class FixClient:
    def __init__(self):
        self.orderId = 0
        self.fixSession = FixSession()

    HEARTBEAT_INTERVAL_DEFAULT = 30
    ENCRYPTION_METHOD_DEFAULT = 0

    def connect(self, address, port, fixVersion, compId, targetCompid, heartbeatInterval=HEARTBEAT_INTERVAL_DEFAULT, encryptionMethod=ENCRYPTION_METHOD_DEFAULT):
        self.fixSession.fixTransport.targetAddress = address
        self.fixSession.fixTransport.targetPort = port
        self.fixSession.fixVersion = fixVersion
        self.fixSession.senderCompid = compId
        self.fixSession.targetCompid = targetCompid
        self.fixSession.heartbeatInterval = heartbeatInterval
        self.fixSession.encryptionMethod = encryptionMethod
        return self.fixSession.connect(None)

    def connectWithCustomLogonMessage(self, adress, port, logOnMessage):
        self.fixSession.targetAddress = address
        self.fixSession.targetPort = port

        mandatoryTags = [FixConstants.TAG_VERSION, FixConstants.TAG_SENDER_COMPID, FixConstants.TAG_TARGET_COMPID]

        if logOnMessage.hasTags(mandatoryTags) == False:
            raise Exception("You have to specify fix version, sender compid and target compid in a custom logon message ")

        self.fixSession.fixVersion = logOnMessage.getTagValue(FixConstants.TAG_VERSION)
        self.fixSession.senderCompid = logOnMessage.getTagValue(FixConstants.TAG_SENDER_COMPID)
        self.fixSession.targetCompid = logOnMessage.getTagValue(FixConstants.TAG_TARGET_COMPID)

        logOnMessage.removeTags(mandatoryTags)

        if logOnMessage.hasTag(FixConstants.TAG_HEARTBEAT_INTERVAL):
            self.fixSession.heartbeatInterval = logOnMessage.getTagValue(FixConstants.TAG_HEARTBEAT_INTERVAL)
        else:
            self.fixSession.heartbeatInterval = FixClient.HEARTBEAT_INTERVAL_DEFAULT

        if logOnMessage.hasTag(FixConstants.TAG_ENCRYPT_METHOD):
            self.fixSession.encryptionMethod = logOnMessage.getTagValue(FixConstants.TAG_ENCRYPT_METHOD)
        else:
            self.fixSession.encryptionMethod = FixClient.HEARTBEAT_INTERVAL_DEFAULT

        return self.fixSession.connect(logOnMessage)

    def disconnect(self, logOffMessage = None):
        self.fixSession.disconnect(logOffMessage)

    def recv(self):
        return self.fixSession.recv()

    def send(self, fixMessage, sendClientOrderId=True, timeoutSeconds=0):
        if sendClientOrderId is True:
            self.orderId += 1
            fixMessage.setTag(FixConstants.TAG_CLIENT_ORDER_ID, self.orderId)
        ret = False
        
        def getTimeInSeconds():
            return int(round(time.time()))
        
        trialStart = getTimeInSeconds()
        while True:
            ret = self.fixSession.send(fixMessage)
            if ret is True:
                break
                
            if timeoutSeconds > 0:
                deltaTime = getTimeInSeconds() - trialStart
                if deltaTime >= timeoutSeconds:
                    break
            else:
                break
            
        return ret
        
class FixServer:
    def __init__(self):
        self.fixSession = FixSession()
        self.execId = 0

    def initialiseSession(self, port, compId):
        self.fixSession.fixTransport.targetPort = port
        self.fixSession.senderCompid = compId
        self.fixSession.server = True
        
    def start(self, port, compId):
        self.initialiseSession(port, compId)
        return self.fixSession.accept(None)

    def startWithCustomLogonResponse(self, port, customLogonResponse):
        if customLogonResponse.hasTag(FixConstants.TAG_SENDER_COMPID) == false:
            raise Exception("You have to specify sender comp id tag in a custom logon response message")
        compId = customLogonResponse.getTagValue(FixConstants.TAG_SENDER_COMPID)
        self.initialiseSession(port, compId)
        customLogonResponse.removeTag(FixConstants.TAG_SENDER_COMPID)
        return self.fixSession.accept(customLogonResponse)

    def disconnect(self, logOffResponse = None):
        self.fixSession.disconnect(logOffResponse)
        
    def send(self, fixMessage, sendExecutionId=False):
        if sendExecutionId is True:
            fixMessage.setTag(FixConstants.TAG_EXEC_ID, self.execId)
        return self.fixSession.send(fixMessage)
        
    def recv(self):
        message = self.fixSession.recv()
        self.fixSession.saveSequenceNumberToFile()
        return message
               
class FixAutomationClient(FixClient):
    def __init__(self):
        FixClient.__init__(self)
        self.ordersToSend = []
        self.executionReports = []

    def setOrders(self, orders):
        self.ordersToSend = orders

    def getOrders(self):
        return self.ordersToSend

    def getExecutionReports(self):
        return self.executionReports

    def addExecutionReport(self, fixMessage):
        self.executionReports.append(fixMessage)

def syncronisedPrint(mutex, message):
        mutex.acquire()
        print(message)
        mutex.release()
        
FIX_CLIENT_AUTOMATION_MUTEX = Lock()
        
def fixClientAutomationClientThread(fixClientAutomationMutex, resultsQueue, ordersFile, fixVersion, address, port, baseCompId, clientIndex, targetCompid):
    
    senderCompId = baseCompId + str(clientIndex)

    fixClient = FixAutomationClient()
    fixClient.fixSession.restoreSequenceNumberFromFile = True
    
    orders = FixMessage.loadFromFile(ordersFile)
    ordersCount = len(orders)
    
    fixClient.connect(address, port, fixVersion, senderCompId, targetCompid)
    connected = fixClient.fixSession.connected

    if connected is True:
        syncronisedPrint(fixClientAutomationMutex, senderCompId + " connected" )

        processedCount = 0

        for order  in orders:
            order.setTag(FixConstants.TAG_TRANSACTION_TIME, fixClient.fixSession.getCurrentUTCDateTime())
            fixClient.send(order, True, 120)

        syncronisedPrint(fixClientAutomationMutex, senderCompId + " fired all orders")

        # Collect execution reports
        while True:
            message = fixClient.recv()

            if message is None:
                continue

            if message.getMessageType() is FixConstants.MESSAGE_HEARTBEAT:
                continue

            resultsQueue.put( message.toString(False) )

            if message.hasTag(FixConstants.TAG_ORDER_STATUS):
                orderStatus = message.getTagValue(FixConstants.TAG_ORDER_STATUS)
                if orderStatus is FixConstants.ORDER_STATUS_FILLED:
                    processedCount += 1
                    syncronisedPrint(fixClientAutomationMutex, senderCompId + " received a fill :" + str(processedCount) + " of " + str(ordersCount) )
                if orderStatus is FixConstants.ORDER_STATUS_CANCELED:
                    processedCount += 1
                    ordersCount -= 1
                    syncronisedPrint(fixClientAutomationMutex, senderCompId + " received a cancel :" + str(processedCount) + " of " + str(ordersCount) )

            if processedCount == ordersCount:
                break

        fixClient.disconnect()
        syncronisedPrint(fixClientAutomationMutex, senderCompId + " disconnected")

class FixClientAutomation:
    def __init__(self):
        self.resultsQueue = []
        self.fixClientThreads = []
        self.fixVersion = ""
        self.numberOfClients = 0
        self.ordersFile = ""
        self.address = ""
        self.port = 0
        self.compIdBase = ""
        self.targetCompId = ""

    def __del__(self):
        self.shutdown()

    def initialise(self, numberOfClients, ordersFile, fixVersion, address, port, compdIdBase, targetCompid):
        self.numberOfClients = numberOfClients
        self.ordersFile = ordersFile
        self.fixVersion = fixVersion
        self.address = address
        self.port = port
        self.compIdBase = compdIdBase
        self.targetCompId = targetCompid

    def shutdown(self):
        for thread in self.fixClientThreads:
            if thread.is_alive():
                thread.join()

    def start(self):
        for i in range(0, self.numberOfClients):
            currentQueue = Queue()
            self.resultsQueue.append(currentQueue)
            fixClientThread = Process(target=fixClientAutomationClientThread,
                                      args=[FIX_CLIENT_AUTOMATION_MUTEX, self.resultsQueue[i], self.ordersFile, self.fixVersion, self.address, self.port, self.compIdBase, i+1, self.targetCompId ])
            self.fixClientThreads.append(fixClientThread)
            self.fixClientThreads[len(self.fixClientThreads) - 1].start()

    def join(self):
        for thread in self.fixClientThreads:
            thread.join()

    def report(self, reportFileName):
        report = ""

        numberOfClients = len(self.fixClientThreads)
        clientExecReports = []

        for i in range(0, numberOfClients):
            list = []
            clientExecReports.append(list)
            while not self.resultsQueue[i].empty():
                result = self.resultsQueue[i].get()
                clientExecReports[i].append(result)

        for i in range(0, numberOfClients):
            # Sender comp id
            currentClientFirstExecReport = FixMessage()
            currentClientFirstExecReport.loadFromString( clientExecReports[i][0] )
            report += currentClientFirstExecReport.getTagValue(FixConstants.TAG_TARGET_COMPID)

            report += "\n"
            report += "\n"

            for execReport in clientExecReports[i]:
                report += execReport
                report += "\n"

            report += "\n"
            report += "\n"

        if os.path.exists(reportFileName):
            os.remove(reportFileName)

        with open(reportFileName, "w") as textFile:
            textFile.write(report)
        
def main():
    try:
        Utility.changeWorkingDirectoryToScriptLocation()

        fixClientAutomation = FixClientAutomation()
        numberOfClients = 1
        ordersFile = "test_cases_simple.txt"
        fixVersion = "FIX.4.2"
        server = "127.0.0.1"
        serverPort = 5001
        clientCompIdBase = "TEST_CLIENT"
        serverCompId = "LITEEXCHANGE"

        reportFile = "report.txt"

        # GATHER ARGUMENTS
        while True :
            Utility.writeInfoMessage("Current automation parameters are : ")
            print("")
            Utility.writeInfoMessage("1 Number of concurrent clients : " + str(numberOfClients))
            Utility.writeInfoMessage("2 Test case fix file : " + ordersFile)
            Utility.writeInfoMessage("3 Fix version : " + fixVersion)
            Utility.writeInfoMessage("4 Server address : " + server)
            Utility.writeInfoMessage("5 Server port : " + str(serverPort))
            Utility.writeInfoMessage("6 Server comp id : " + serverCompId)
            Utility.writeInfoMessage("7 Client comp id base value : " + clientCompIdBase)

            print("")
            answer = Utility.yesNoQuestion("Do you want to change any parameters ? ")
            print("")

            if answer is False:
                break

            if answer is True:
                try:
                    parameterIndexToChange = int( Utility.askQuestion("Enter a number between 1 and 7 :") )
                except:
                    continue
                if parameterIndexToChange >= 1 and parameterIndexToChange <= 7:
                    parameterNewValue = Utility.askQuestion("Enter value for the parameter you selected")

                    try:
                        if parameterIndexToChange == 1:
                            numberOfClients = int(parameterNewValue)
                        elif parameterIndexToChange == 2:
                            ordersFile = parameterNewValue
                        elif parameterIndexToChange == 3:
                            fixVersion = parameterNewValue
                        elif parameterIndexToChange == 4:
                            server = parameterNewValue
                        elif parameterIndexToChange == 5:
                            serverPort = int(parameterNewValue)
                        elif parameterNewValue == 6:
                            serverCompId = parameterNewValue
                        elif parameterNewValue == 7:
                            clientCompIdBase = parameterNewValue
                    except:
                        continue

        # RUN AUTOMATION
        print("")
        Utility.writeInfoMessage("Client automation starting")
        print("")

        stopwatch = StopWatch()
        stopwatch.start()

        fixClientAutomation.initialise(numberOfClients, ordersFile, fixVersion, server, serverPort, clientCompIdBase, serverCompId)
        fixClientAutomation.start()
        fixClientAutomation.join()

        stopwatch.stop()

        # DISPLAY RESULTS
        print("")
        Utility.writeInfoMessage("Client automation took " + str(stopwatch.elapsedTimeInMilliseconds()) + " milliseconds" )
        print("")

        fixClientAutomation.report(reportFile)

        if _platform == "linux" or _platform == "linux2":
            Utility.writeInfoMessage(reportFile + " created")
        elif _platform == "win32":
            os.system("notepad " + reportFile )

    except ValueError as err:
        Utility.writeErrorMessage(err.args)

# Entry point
if __name__ == "__main__":
    main()