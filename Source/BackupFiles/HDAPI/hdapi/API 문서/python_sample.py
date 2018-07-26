#-*- coding: utf-8 -*-

# Form implementation generated from reading ui file './python_sample.ui'
#
# Created by: PyQt4 UI code generator 4.11.4
#
# WARNING! All changes made in this file will be lost!
# Python version 2.7

from PyQt4 import QtCore, QtGui
#from PyQt4.QAxContainer import *
#from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4.QAxContainer import *
import time
import os

import sys
reload(sys)
sys.setdefaultencoding('utf-8')


try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_MainWindow(object):
    CONST_FO_SERIES = "101L6000"
    CONST_FO_SISE_51 = "51" # 국내선물호가
    CONST_FO_SISE_65 = "65" # 국내선물체결
    CONST_FO_SISE_58 = "58" # 국내상품선물호가
    CONST_FO_SISE_71 = "71" # 국내상품선물체결
    
    CONST_FO_ORDER_181 = "181" # 국내주문실시간접수
    CONST_FO_ORDER_182 = "182" # 국내주문실시간미체결
    CONST_FO_ORDER_183 = "183" # 국내주문실시간미결제
    CONST_FO_ORDER_184 = "184" # 국내주문실시간잔고
    CONST_FO_ORDER_185 = "185" # 국내주문실시간체결
    
    CONST_FR_SERIES = "6AH16"
    CONST_FR_SISE_76 = "76" # 해외선물호가
    CONST_FR_SISE_82 = "82" # 해외선물체결
    
    CONST_FR_ORDER_196 = "196" # 해외주문실시간접수
    CONST_FR_ORDER_186 = "186" # 해외주문실시간미체결
    CONST_FR_ORDER_187 = "187" # 해외주문실시간미결제
    CONST_FR_ORDER_188 = "188" # 해외주문실시간잔고
    CONST_FR_ORDER_189 = "189" # 해외주문실시간체결
    
    CONST_FX_SERIES = "EUR/USD"
    CONST_FX_SISE_171 = "171"
    
    #attributes
    m_AccListFO = {}
    m_AccListFR = {}
    m_AccListFX = {}
    
    m_TrQueryListFO = {}    # 국내 조회
    m_FidQueryListFO = {}     # 국내 FID조회
    m_TrOrderListFO = {}    # 국내 주문
    
    
    m_TrQueryListFR = {}
    m_FidQueryListFR = {}     # 해외 FID조회
    m_TrOrderListFR = {}
    
    m_TrQueryListFX = {}
    m_FidQueryListFX = {}     # 해외 FID조회
    m_TrOrderListFX = {}
    
    # 실시간
    m_RealSiseListFO = {}
    m_RealOrderListFO = {}
    
    m_RealSiseListFR = {}
    m_RealOrderListFR = {}
    
    m_RealSiseListFX = {}
    m_RealOrderListFX = {}
    
    m_bUseComboTrChange = True
    
    def setupUi(self, MainWindow):
        _TOP_Pos = 50
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        MainWindow.resize(800, 600)
        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        self.label_6 = QtGui.QLabel(self.centralwidget)
        self.label_6.setGeometry(QtCore.QRect(10, 10, 56, 12))
        self.label_6.setObjectName(_fromUtf8("label_6"))
        self.Edit_Path = QtGui.QLineEdit(self.centralwidget)
        self.Edit_Path.setGeometry(QtCore.QRect(70, 7, 400, 20))
        self.Edit_Path.setObjectName(_fromUtf8("Edit_Path"))
        self.BTN_Connect = QtGui.QPushButton(self.centralwidget)
        self.BTN_Connect.setGeometry(QtCore.QRect(10, _TOP_Pos+10, 121, 23))
        self.BTN_Connect.setObjectName(_fromUtf8("BTN_Connect"))
        self.BTN_DisConnect = QtGui.QPushButton(self.centralwidget)
        self.BTN_DisConnect.setGeometry(QtCore.QRect(150, _TOP_Pos+10, 111, 23))
        self.BTN_DisConnect.setObjectName(_fromUtf8("BTN_DisConnect"))
        self.label = QtGui.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(10, _TOP_Pos+50, 56, 12))
        self.label.setObjectName(_fromUtf8("label"))
        self.label_2 = QtGui.QLabel(self.centralwidget)
        self.label_2.setGeometry(QtCore.QRect(200, _TOP_Pos+50, 70, 12))
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.label_3 = QtGui.QLabel(self.centralwidget)
        self.label_3.setGeometry(QtCore.QRect(401, _TOP_Pos+50, 111, 16))
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.Edit_UserID = QtGui.QLineEdit(self.centralwidget)
        self.Edit_UserID.setGeometry(QtCore.QRect(70, _TOP_Pos+45, 113, 20))
        self.Edit_UserID.setObjectName(_fromUtf8("Edit_UserID"))
        self.Edit_UserPW = QtGui.QLineEdit(self.centralwidget)
        self.Edit_UserPW.setGeometry(QtCore.QRect(280, _TOP_Pos+46, 113, 20))
        self.Edit_UserPW.setEchoMode(QtGui.QLineEdit.Password)
        self.Edit_UserPW.setObjectName(_fromUtf8("Edit_UserPW"))
        self.Edit_CertPW = QtGui.QLineEdit(self.centralwidget)
        self.Edit_CertPW.setGeometry(QtCore.QRect(510, _TOP_Pos+47, 113, 20))
        self.Edit_CertPW.setEchoMode(QtGui.QLineEdit.Password)
        self.Edit_CertPW.setObjectName(_fromUtf8("Edit_CertPW"))
        self.Btn_Login = QtGui.QPushButton(self.centralwidget)
        self.Btn_Login.setGeometry(QtCore.QRect(390, _TOP_Pos+80, 75, 23))
        self.Btn_Login.setObjectName(_fromUtf8("Btn_Login"))
        self.Btn_Logout = QtGui.QPushButton(self.centralwidget)
        self.Btn_Logout.setGeometry(QtCore.QRect(550, _TOP_Pos+80, 75, 23))
        self.Btn_Logout.setObjectName(_fromUtf8("Btn_Logout"))
        
        # 국내/해외/FX
        self.groupBox1 = QtGui.QGroupBox(self.centralwidget)
        self.groupBox1.setGeometry(QtCore.QRect(10, _TOP_Pos+110, 200, 41))
        self.groupBox1.setObjectName(_fromUtf8("groupBox"))
        self.BtnRadio1 = QtGui.QRadioButton(self.groupBox1) 
        self.BtnRadio1.setGeometry(QtCore.QRect(10, 10, 111, 22))
        self.BtnRadio1.setObjectName(_fromUtf8("BtnRadio1"))
        self.BtnRadio2 = QtGui.QRadioButton(self.groupBox1) 
        self.BtnRadio2.setGeometry(QtCore.QRect(70, 10, 111, 22))
        self.BtnRadio2.setObjectName(_fromUtf8("BtnRadio2"))
        self.BtnRadio3 = QtGui.QRadioButton(self.groupBox1) 
        self.BtnRadio3.setGeometry(QtCore.QRect(140, 10, 111, 22))
        self.BtnRadio3.setObjectName(_fromUtf8("BtnRadio3"))
        
        # Tr조회, Fid조회, 주문
        self.groupBox2 = QtGui.QGroupBox(self.centralwidget)
        self.groupBox2.setGeometry(QtCore.QRect(251, _TOP_Pos+110, 200, 41))
        self.groupBox2.setObjectName(_fromUtf8("groupBox2"))
        self.BtnRadioQry1 = QtGui.QRadioButton(self.groupBox2) 
        self.BtnRadioQry1.setGeometry(QtCore.QRect(10, 10, 111, 22))
        self.BtnRadioQry1.setObjectName(_fromUtf8("BtnRadioQry1"))
        self.BtnRadioQry2 = QtGui.QRadioButton(self.groupBox2) 
        self.BtnRadioQry2.setGeometry(QtCore.QRect(70, 10, 111, 22))
        self.BtnRadioQry2.setObjectName(_fromUtf8("BtnRadioQry2"))
        self.BtnRadioQry3 = QtGui.QRadioButton(self.groupBox2) 
        self.BtnRadioQry3.setGeometry(QtCore.QRect(140, 10, 111, 22))
        self.BtnRadioQry3.setObjectName(_fromUtf8("BtnRadioQry3"))
        
        #해외 종목코드 수신
        self.Btn_ReqJMCodeFR = QtGui.QPushButton(self.centralwidget)
        self.Btn_ReqJMCodeFR.setGeometry(QtCore.QRect(460, _TOP_Pos+120, 120, 23))
        self.Btn_ReqJMCodeFR.setObjectName(_fromUtf8("Btn_ReqJMCodeFR"))

        # 계좌 콤보
        self.label_4 = QtGui.QLabel(self.centralwidget)
        self.label_4.setGeometry(QtCore.QRect(10, _TOP_Pos+165, 56, 12))
        self.label_4.setObjectName(_fromUtf8("label_4"))        
        self.ComboAcc = QtGui.QComboBox(self.centralwidget)
        self.ComboAcc.setGeometry(QtCore.QRect(70, _TOP_Pos+160, 111, 22))
        self.ComboAcc.setObjectName(_fromUtf8("ComboAcc"))
        self.label_acctNm = QtGui.QLabel(self.centralwidget)
        self.label_acctNm.setGeometry(QtCore.QRect(450, _TOP_Pos+165, 56, 12))
        self.label_acctNm.setObjectName(_fromUtf8("label_acctNm"))
        
        #계좌비번
        self.label_7 = QtGui.QLabel(self.centralwidget)
        self.label_7.setGeometry(QtCore.QRect(200, _TOP_Pos+165, 56, 12))
        self.label_7.setObjectName(_fromUtf8("label_7"))        
        self.Edit_AcctPW = QtGui.QLineEdit(self.centralwidget)
        self.Edit_AcctPW.setGeometry(QtCore.QRect(280, _TOP_Pos+160, 113, 20))
        self.Edit_AcctPW.setEchoMode(QtGui.QLineEdit.Password)
        self.Edit_AcctPW.setObjectName(_fromUtf8("Edit_AcctPW"))
        
        #서비스 콤부
        self.label_5 = QtGui.QLabel(self.centralwidget)
        self.label_5.setGeometry(QtCore.QRect(10, _TOP_Pos+200, 56, 12))
        self.label_5.setObjectName(_fromUtf8("label_5"))
        self.ComboTr = QtGui.QComboBox(self.centralwidget)
        self.ComboTr.setGeometry(QtCore.QRect(70, _TOP_Pos+195, 200, 22))
        self.ComboTr.setObjectName(_fromUtf8("ComboTr"))
        self.label_8 = QtGui.QLabel(self.centralwidget)
        self.label_8.setGeometry(QtCore.QRect(280, _TOP_Pos+200, 60, 12))
        self.label_8.setObjectName(_fromUtf8("label_8"))
        self.label_tr = QtGui.QLabel(self.centralwidget)
        self.label_tr.setGeometry(QtCore.QRect(340, _TOP_Pos+200, 150, 12))
        self.label_tr.setObjectName(_fromUtf8("label_tr"))

        # 입력 부분        
        self.label_10 = QtGui.QLabel(self.centralwidget)
        self.label_10.setGeometry(QtCore.QRect(10, _TOP_Pos+230, 60, 12))
        self.label_10.setObjectName(_fromUtf8("label_10"))
        self.Edit_Input = QtGui.QLineEdit(self.centralwidget)
        self.Edit_Input.setGeometry(QtCore.QRect(70, _TOP_Pos+225, 450, 20))
        self.Edit_Input.setObjectName(_fromUtf8("Edit_Input"))
        
        self.Edit_Input_FID = QtGui.QLineEdit(self.centralwidget)
        self.Edit_Input_FID.setGeometry(QtCore.QRect(525, _TOP_Pos+225, 120, 20))
        self.Edit_Input_FID.setObjectName(_fromUtf8("Edit_Input_FID"))
        
        self.BTN_Query = QtGui.QPushButton(self.centralwidget)
        self.BTN_Query.setGeometry(QtCore.QRect(650, _TOP_Pos+224, 60, 22))
        self.BTN_Query.setObjectName(_fromUtf8("BTN_Query"))
        
        # 실시간
        self.groupBox3 = QtGui.QGroupBox(self.centralwidget)
        self.groupBox3.setGeometry(QtCore.QRect(10, _TOP_Pos+250, 200, 41))
        self.groupBox3.setObjectName(_fromUtf8("groupBox"))
        self.label_11 = QtGui.QLabel(self.groupBox3) 
        self.label_11.setGeometry(QtCore.QRect(10, 10, 111, 22))
        self.label_11.setObjectName(_fromUtf8("label_11"))
        self.BtnRadioSiseReal = QtGui.QRadioButton(self.groupBox3) 
        self.BtnRadioSiseReal.setGeometry(QtCore.QRect(70, 10, 111, 22))
        self.BtnRadioSiseReal.setObjectName(_fromUtf8("BtnRadioSiseReal"))
        self.BtnRadioOrderReal = QtGui.QRadioButton(self.groupBox3) 
        self.BtnRadioOrderReal.setGeometry(QtCore.QRect(140, 10, 111, 22))
        self.BtnRadioOrderReal.setObjectName(_fromUtf8("BtnRadioOrderReal"))
        
         # 실시간 등록/해지
        self.groupBox4 = QtGui.QGroupBox(self.centralwidget)
        self.groupBox4.setGeometry(QtCore.QRect(251, _TOP_Pos+250, 150, 41))
        self.groupBox4.setObjectName(_fromUtf8("groupBox"))
        self.BtnRadioRealRegister = QtGui.QRadioButton(self.groupBox4) 
        self.BtnRadioRealRegister.setGeometry(QtCore.QRect(10, 10, 111, 22))
        self.BtnRadioRealRegister.setObjectName(_fromUtf8("BtnRadioRealRegister"))
        self.BtnRadioRealUnRegister = QtGui.QRadioButton(self.groupBox4) 
        self.BtnRadioRealUnRegister.setGeometry(QtCore.QRect(70, 10, 111, 22))
        self.BtnRadioRealUnRegister.setObjectName(_fromUtf8("BtnRadioRealUnRegister"))
        
        self.ComboReal = QtGui.QComboBox(self.centralwidget)
        self.ComboReal.setGeometry(QtCore.QRect(420, _TOP_Pos+260, 200, 22))
        self.ComboReal.setObjectName(_fromUtf8("ComboReal"))
        
        self.label_12 = QtGui.QLabel(self.centralwidget)
        self.label_12.setGeometry(QtCore.QRect(10, _TOP_Pos+300, 60, 12))
        self.label_12.setObjectName(_fromUtf8("label_12"))
        
        self.Edit_Input_Real_Key = QtGui.QLineEdit(self.centralwidget)
        self.Edit_Input_Real_Key.setGeometry(QtCore.QRect(70, _TOP_Pos+295, 50, 20))
        self.Edit_Input_Real_Key.setObjectName(_fromUtf8("Edit_Input_Real_Key"))
        self.Edit_Input_Real_Val = QtGui.QLineEdit(self.centralwidget)
        self.Edit_Input_Real_Val.setGeometry(QtCore.QRect(130, _TOP_Pos+295, 250, 20))
        self.Edit_Input_Real_Val.setObjectName(_fromUtf8("Edit_Input_Real_Val"))
        
        self.BTN_Query_Real = QtGui.QPushButton(self.centralwidget)
        self.BTN_Query_Real.setGeometry(QtCore.QRect(380, _TOP_Pos+294, 60, 22))
        self.BTN_Query_Real.setObjectName(_fromUtf8("BTN_Query_Real"))
        
        self.multiline = QtGui.QPlainTextEdit(self.centralwidget)
        self.multiline.setGeometry(QtCore.QRect(10, _TOP_Pos+330, 781, 191))
        self.multiline.setObjectName(_fromUtf8("listView"))
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 21))
        self.menubar.setObjectName(_fromUtf8("menubar"))
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(MainWindow)
        self.statusbar.setObjectName(_fromUtf8("statusbar"))
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        
        self.SetupTabOrder()
        self.SetupSignal()
        
    def SetupSignal(self):
        QtCore.QObject.connect(self.BTN_Connect, QtCore.SIGNAL(_fromUtf8("clicked()")), self.OnConnect)
        QtCore.QObject.connect(self.BTN_DisConnect, QtCore.SIGNAL(_fromUtf8("clicked()")), self.OnDisConnect)
        QtCore.QObject.connect(self.Btn_Login, QtCore.SIGNAL(_fromUtf8("clicked()")), self.OnLogin)
        QtCore.QObject.connect(self.Btn_Logout, QtCore.SIGNAL(_fromUtf8("clicked()")), self.OnLogout)
        
        QtCore.QObject.connect(self.BtnRadio1, QtCore.SIGNAL(_fromUtf8("clicked()")), self.OnRadioAcctTpChange)
        QtCore.QObject.connect(self.BtnRadio2, QtCore.SIGNAL(_fromUtf8("clicked()")), self.OnRadioAcctTpChange)
        QtCore.QObject.connect(self.BtnRadio3, QtCore.SIGNAL(_fromUtf8("clicked()")), self.OnRadioAcctTpChange)
        
        QtCore.QObject.connect(self.BtnRadioQry1, QtCore.SIGNAL(_fromUtf8("clicked()")), self.OnRadioTrChange)
        QtCore.QObject.connect(self.BtnRadioQry2, QtCore.SIGNAL(_fromUtf8("clicked()")), self.OnRadioTrChange)
        QtCore.QObject.connect(self.BtnRadioQry3, QtCore.SIGNAL(_fromUtf8("clicked()")), self.OnRadioTrChange)
        
        QtCore.QObject.connect(self.Btn_ReqJMCodeFR, QtCore.SIGNAL(_fromUtf8("clicked()")), self.OnReqJmCodeFr)
        
        
        QtCore.QObject.connect(self.ComboAcc, QtCore.SIGNAL(_fromUtf8("currentIndexChanged(QString)")), self.OnComboAccChange)
        QtCore.QObject.connect(self.ComboTr, QtCore.SIGNAL(_fromUtf8("currentIndexChanged(QString)")), self.OnComboTrChange)
        
        QtCore.QObject.connect(self.BTN_Query, QtCore.SIGNAL(_fromUtf8("clicked()")), self.OnQuery)
        
        QtCore.QObject.connect(self.BtnRadioSiseReal, QtCore.SIGNAL(_fromUtf8("clicked()")), self.OnRadioRealChange)
        QtCore.QObject.connect(self.BtnRadioOrderReal, QtCore.SIGNAL(_fromUtf8("clicked()")), self.OnRadioRealChange)
        
        QtCore.QObject.connect(self.ComboReal, QtCore.SIGNAL(_fromUtf8("currentIndexChanged(QString)")), self.OnComboReal)
        QtCore.QObject.connect(self.BTN_Query_Real, QtCore.SIGNAL(_fromUtf8("clicked()")), self.OnRealRegister)
        
    def SetupTabOrder(self):
        QtCore.QMetaObject.connectSlotsByName(MainWindow)
        MainWindow.setTabOrder(self.BTN_Connect, self.Edit_UserID)
        MainWindow.setTabOrder(self.Edit_UserID, self.Edit_UserPW)
        MainWindow.setTabOrder(self.Edit_UserPW, self.Edit_CertPW)
        MainWindow.setTabOrder(self.Edit_CertPW, self.Btn_Login)
        MainWindow.setTabOrder(self.Btn_Login, self.ComboAcc)
        MainWindow.setTabOrder(self.ComboAcc, self.ComboTr)
        MainWindow.setTabOrder(self.ComboTr, self.Btn_Logout)
        MainWindow.setTabOrder(self.Btn_Logout, self.BTN_DisConnect)
    
    def SetupHDFOcx(self, MainWindow):
        self.HDF = QAxWidget("HDFCOMMAGENT.HDFCommAgentCtrl.1")
        self.HDF.connect(self.HDF, QtCore.SIGNAL("OnDataRecv(QString, int)"), self.OnDataRecv)
        self.HDF.connect(self.HDF, QtCore.SIGNAL("OnGetBroadData(QString, int)"), self.OnGetBroadData)
        self.HDF.connect(self.HDF, QtCore.SIGNAL("OnGetMsgWithRqId(int, QString, QString)"), self.OnGetMsgWithRqId)

        # 실행위치 설정        
        strPath = os.getcwd()
        self.Edit_Path.setText(_translate("MainWindow", strPath, None))
        self.HDF.dynamicCall("CommSetOCXPath(" + strPath + ")")
        
    def HD_CommGetRepeatCnt(self, strTrCode, nRealType, strFildNm):
        nRqID = self.HDF.dynamicCall("CommGetRepeatCnt(QString, int, QString)", strTrCode, nRealType, strFildNm)
        return range(nRqID.toLongLong()[0])
    
    def HD_CommRqData(self, strTrCode, strInputData, nInputLength, strPrevOrNext):
        nRqID = self.HDF.dynamicCall("CommRqData(QString, QString, LONG, QString)", strTrCode, strInputData, nInputLength, strPrevOrNext)
        return nRqID.toLongLong()[0]
    
    def HD_CommFIDRqData(self, strTrCode, strInputData, sReqFidList, sPrevOrNext):
        #BSTR sFidCode, BSTR sInputData, BSTR sReqFidList, LONG nLength, BSTR sPrevOrNext
        nRqID = self.HDF.dynamicCall("CommFIDRqData(QString, QString, QString, LONG, QString)", strTrCode, strInputData, sReqFidList, len(strInputData), sPrevOrNext)
        return nRqID.toLongLong()[0]
    
    def HD_CommJumunSvr(self, strTrCode, strInputData):
        nRqID = self.HDF.dynamicCall("CommJumunSvr(QString, QString)", strTrCode, strInputData)
        return nRqID.toLongLong()[0]
    
    def HD_CommGetData(self, strTrCode, nRealType, strRecNm, nIndex, strFieldNm):
        strFieldNm = unicode(str(strFieldNm).strip(), 'utf-8')
        strData = self.HDF.dynamicCall("CommGetData(QString, int, QString, int, QString)", strTrCode, nRealType, strRecNm, nIndex, strFieldNm)
        return strData.toString()
    
    def HD_RegReal(self, bReg, strValue, nKeyType):
        if ( bReg == True ):
            nRet = self.HDF.dynamicCall("CommSetBroad(QString, LONG)", strValue, nKeyType)
        else:
            nRet = self.HDF.dynamicCall("CommRemoveBroad(QString, LONG)", strValue, nKeyType)
        return nRet
        
    def HD_CommGetAccInfo(self):
        strData = self.HDF.dynamicCall("CommGetAccInfo()")
        return strData.toString()
    
    def HD_Login(self, strUserID, strUserWd, strCertPw):
        ret = self.HDF.dynamicCall("CommLogin(QString, QString, QString)", strUserID, strUserWd, strCertPw)
        
    def HD_Logout(self, strUserID):
        ret = self.HDF.dynamicCall("CommLogout(QString)", strUserID)
        
    def HD_CommReqMakeCod(self):
        ret = self.HDF.dynamicCall("CommReqMakeCod(QString, BOOL)", "JMCODE", False)
    
    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow", None))
        self.label_6.setText(_translate("MainWindow", "실행위치", None))
        self.BTN_Connect.setText(_translate("MainWindow", "통신관리자 실행", None))
        self.BTN_DisConnect.setText(_translate("MainWindow", "통신관리자 종료", None))
        self.label.setText(_translate("MainWindow", "사용자 ID", None))
        self.label_2.setText(_translate("MainWindow", "HTS비번", None))
        self.label_3.setText(_translate("MainWindow", "공인인증 비밀번호", None))
        self.Btn_Login.setText(_translate("MainWindow", "로그인", None))
        self.Btn_Logout.setText(_translate("MainWindow", "로그아웃", None))
        self.label_4.setText(_translate("MainWindow", "계좌번호", None))
        self.label_7.setText(_translate("MainWindow", "계좌비번", None))
        self.label_5.setText(_translate("MainWindow", "조회유형", None))
        self.label_8.setText(_translate("MainWindow", "TRCode : ", None))
        self.label_10.setText(_translate("MainWindow", "Input : ", None))
        self.BTN_Query.setText(_translate("MainWindow", "조회", None))
        
        self.BtnRadio1.setText(_translate("MainWindow", "국내", None))
        self.BtnRadio2.setText(_translate("MainWindow", "해외", None))
        self.BtnRadio3.setText(_translate("MainWindow", "FX", None))
        self.BtnRadio1.setChecked(True)
        
        self.BtnRadioQry1.setText(_translate("MainWindow", "TR", None))
        self.BtnRadioQry2.setText(_translate("MainWindow", "FID", None))
        self.BtnRadioQry3.setText(_translate("MainWindow", "주문", None))
        self.BtnRadioQry1.setChecked(True)
        
        self.Btn_ReqJMCodeFR.setText(_translate("MainWindow", "해외종목코드 수신", None))
        
        self.label_11.setText(_translate("MainWindow", "실시간", None))
        self.BtnRadioSiseReal.setText(_translate("MainWindow", "시세", None))
        self.BtnRadioOrderReal.setText(_translate("MainWindow", "주문", None))
        self.BtnRadioSiseReal.setChecked(True)
        
        self.BtnRadioRealRegister.setText(_translate("MainWindow", "등록", None))
        self.BtnRadioRealUnRegister.setText(_translate("MainWindow", "해지", None))
        self.BtnRadioRealRegister.setChecked(True)
        
        self.label_12.setText(_translate("MainWindow", "실시간", None))
        self.BTN_Query_Real.setText(_translate("MainWindow", "등록/해지", None))
        
        self.Edit_UserID.setText(u"sivas99")
        self.Edit_UserPW.setText(u"qwe123")
        self.Edit_AcctPW.setText(u"1234")
        
        self.SetTrToDictionary_FO()
        self.SetTrToDictionary_FR()
        self.SetTrToDictionary_FX()
        
        self.SetRealToDictionary_FO()
        self.SetRealToDictionary_FR()
        self.SetRealToDictionary_FX()
        
        self.OnRadioTrChange()
        self.SetComboReal()
        
        pass
        
    def SetTrToDictionary_FO(self):
        self.m_TrQueryListFO.update({u"01.국내미체결주문조회"   :[u"g11002.DQ0104&",u"계좌번호,11,딜러번호,3,계좌비번,8"]})
        self.m_TrQueryListFO.update({u"02.국내체결내역리스트"   :[u"g11002.DQ0107&",u"계좌번호,11,딜러번호,3,계좌비번,8"]})
        self.m_TrQueryListFO.update({u"03.국내미결제약정조회"   :[u"g11002.DQ0110&",u"계좌번호,11,딜러번호,3,계좌비번,8"]})
        self.m_TrQueryListFO.update({u"04.국내고객예탁금조회"   :[u"g11002.DQ0242&",u"계좌번호,11,계좌비번,8"         ]})
        self.m_TrQueryListFO.update({u"05.국내종목명조회"      :[u"g11002.DQ0622&",u"exchtp,1,exchid,2,fotp,1"]})
        self.m_TrQueryListFO.update({u"06.야간CME미체결주문조회":[u"g11002.DQ0116&",u"계좌번호,11,딜러번호,3,계좌비번,8"]})
        self.m_TrQueryListFO.update({u"07.야간CME체결내역리스트":[u"g11002.DQ0119&",u"계좌번호,11,딜러번호,3,계좌비번,8"]})
        self.m_TrQueryListFO.update({u"08.야간CME미결제약정조회":[u"g11002.DQ0122&",u"계좌번호,11,딜러번호,3,계좌비번,8"]})
        
        self.m_FidQueryListFO.update({u"국내선물옵션마스터"     :[u"s20001",u"종목,8,조회모드,1,조회건수,4",u"000075051057"]})
        self.m_FidQueryListFO.update({u"국내주식선물마스터"     :[u"s30001",u"종목,8,조회모드,1,조회건수,4",u"000075051057"]})
        self.m_FidQueryListFO.update({u"야간CME선물옵션마스터"  :[u"s21001",u"종목,8,조회모드,1,조회건수,4",u"000075051057"]})
        
        self.m_TrOrderListFO.update({u"01.국내신규주문"      :[u"g12001.DO1601&",u"계좌번호,11,계좌비번,8,종목,32,매매구분,1,체결구분,1,체결조건,1,주문가격,13,주문수량,5"]})
        self.m_TrOrderListFO.update({u"02.국내정정주문"      :[u"g12001.DO1901&",u"계좌번호,11,계좌비번,8,종목,32,매매구분,1,체결구분,1,체결조건,1,주문가격,13,주문수량,5,주문번호,7"]})
        self.m_TrOrderListFO.update({u"03.국내취소주문"      :[u"g12001.DO1701&",u"계좌번호,11,계좌비번,8,종목,32,매매구분,1,체결구분,1,체결조건,1,주문가격,13,주문수량,5,주문번호,7"]})
        self.m_TrOrderListFO.update({u"04.CME국내신규주문"   :[u"g12001.DO2201&",u"계좌번호,11,계좌비번,8,종목,32,매매구분,1,체결구분,1,체결조건,1,주문가격,13,주문수량,5"]})
        self.m_TrOrderListFO.update({u"05.CME국내정정주문"   :[u"g12001.DO2101&",u"계좌번호,11,계좌비번,8,종목,32,매매구분,1,체결구분,1,체결조건,1,주문가격,13,주문수량,5,주문번호,7"]})
        self.m_TrOrderListFO.update({u"06.CME국내취소주문"   :[u"g12001.DO2001&",u"계좌번호,11,계좌비번,8,종목,32,매매구분,1,체결구분,1,체결조건,1,주문가격,13,주문수량,5,주문번호,7"]})
        pass
    
    def SetTrToDictionary_FR(self):
        self.m_TrQueryListFR.update({u"01.해외미체결주문내역"      :[u"g11004.AQ0401%",u"조회유형,1,사용자ID,8,계좌번호,6,계좌비번,8"]})
        self.m_TrQueryListFR.update({u"02.해외체결주문내역"        :[u"g11004.AQ0402%",u"조회유형,1,사용자ID,8,계좌번호,6,계좌비번,8"]})
        self.m_TrQueryListFR.update({u"03.해외미결제주문내역"      :[u"g11004.AQ0403%",u"조회유형,1,사용자ID,8,계좌번호,6,계좌비번,8"]})
        self.m_TrQueryListFR.update({u"04.해외예탁자산조회"       :[u"g11004.AQ0607%",u"조회유형,1,사용자ID,8,계좌번호,6,계좌비번,8,groupnm,20,통화코드,3"]})
        self.m_TrQueryListFR.update({u"05.해외일주월분틱"       :[u"o44005",u"keyvalue,18,코드,32,조회일자,8,조회시간,10,다음구분,1,데이터구분,1,시간구분,3,데이터건수,5,장구분,1,허봉실봉구분,1"]})
        
        self.m_FidQueryListFR.update({u"01.해외선물옵션마스터"     :[u"o51000",u"종목,32", u"000001002003004005006007008009010011012013014015016017018019020021022023024025026027028029030031032033034035036037"]})
        self.m_FidQueryListFR.update({u"02.해외호가정보"          :[u"o51010",u"종목,32", u"000001002003004005006007"]})
        self.m_FidQueryListFR.update({u"03.해외종목정보"          :[u"o51210",u"종목,32", u"000001002003004005006007008009010011012013014015016017018019020021"]})
        
        self.m_TrOrderListFR.update({u"01.해외신규주문"      :[u"g12003.AO0401%",u"계좌번호,6,계좌비번,8,종목,32,매매구분,1,체결구분,1,체결조건,1,주문가격,15,주문수량,10,strtp,1,stopp,15,minqty,10"]})
        self.m_TrOrderListFR.update({u"02.해외정정주문"      :[u"g12003.AO0402%",u"계좌번호,6,계좌비번,8,종목,32,매매구분,1,체결구분,1,체결조건,1,주문가격,15,주문수량,10,주문번호,10,strtp,1,stopp,15,minqty,10"]})
        self.m_TrOrderListFR.update({u"03.해외취소주문"      :[u"g12003.AO0403%",u"계좌번호,6,계좌비번,8,종목,32,체결구분,1,체결조건,1,주문가격,15,주문수량,10,주문번호,10"]})
        pass
    
    def SetTrToDictionary_FX(self):
        self.m_TrQueryListFX.update({u"01.FX자산내역조회"        :[u"g11004.AQ0901%",u"조회유형,1,사용자ID,8,계좌번호,6,계좌비번,8"]})
        self.m_TrQueryListFX.update({u"02.FX미체결내역조회"      :[u"g11004.AQ0904%",u"조회유형,1,사용자ID,8,계좌번호,6,계좌비번,8"]})
        self.m_TrQueryListFX.update({u"03.FX계좌정보Summary"     :[u"g11004.AQ906%",u"조회유형,1,사용자ID,8,계좌번호,6,계좌비번,8"]})
        self.m_TrQueryListFX.update({u"04.FX미청산포지션조회"     :[u"g11004.AQ0908%",u"조회유형,1,사용자ID,8,계좌번호,6,계좌비번,8"]})
        self.m_TrQueryListFX.update({u"05.FX청산포지션조회"      :[u"g11004.AQ0910%",u"조회유형,1,사용자ID,8,계좌번호,6,계좌비번,8,groupnm,20,시작일자,8,종료일자,8"]})

        self.m_FidQueryListFX.update({u"01.FX마스터"       :[u"x00001",u"종목,10", u"000001002003004005006007008009010011012013014015016017018019020021022"]})
            
        self.m_TrOrderListFX.update({u"01.FX신규주문"      :[u"g12003.AO0501%",u"계좌번호,6,계좌비번,8,종목,32,매매구분,1,주문가격,20,주문수량,15,ordertr,10,체결구분,1,strtp,1,stopp,20,limitp,20,mrktrange,5,trailingstop,10,trdno,20,trdseq,5"]})
        self.m_TrOrderListFX.update({u"02.FX정정주문"      :[u"g12003.AO0502%",u"계좌번호,6,계좌비번,8,종목,32,주문번호,20,주문SEQ,15,주문가격,15,주문수량,10,ordertr,10,strtp,1,stopp,15,limitp,20,trailingstop,10"]})
        self.m_TrOrderListFX.update({u"03.FX취소주문"      :[u"g12003.AO0503%",u"계좌번호,6,계좌비번,8,종목,32,주문번호,20"]})
        pass
    
    def SetRealToDictionary_FO(self):
        self.m_RealSiseListFO.update({u"01.국내선물호가"   :[self.CONST_FO_SISE_51,u"종목"]})
        self.m_RealSiseListFO.update({u"02.국내선물체결"   :[self.CONST_FO_SISE_65,u"종목"]})
        self.m_RealSiseListFO.update({u"03.국내옵션호가"   :["52",u"종목"]})
        self.m_RealSiseListFO.update({u"04.국내옵션체결"   :["66",u"종목"]})
        self.m_RealSiseListFO.update({u"05.국내상품선물호가"   :[self.CONST_FO_SISE_58,u"종목"]})
        self.m_RealSiseListFO.update({u"06.국내상품선물체결"   :[self.CONST_FO_SISE_71,u"종목"]})
        self.m_RealSiseListFO.update({u"07.국내상품옵션호가"   :[u"59",u"종목"]})
        self.m_RealSiseListFO.update({u"08.국내상품옵션체결"   :[u"73",u"종목"]})
        self.m_RealSiseListFO.update({u"09.국내CME선물호가"   :[u"75",u"종목"]})
        self.m_RealSiseListFO.update({u"10.국내CME선물체결"   :[u"77",u"종목"]})
        self.m_RealSiseListFO.update({u"11.국내주식선물호가"   :[u"56",u"종목"]})
        self.m_RealSiseListFO.update({u"12.국내주식선물체결"   :[u"68",u"종목"]})
        
        self.m_RealOrderListFO.update({u"01.국내주문실시간접수"     :[self.CONST_FO_ORDER_181,u"계좌번호"]})
        self.m_RealOrderListFO.update({u"02.국내주문실시간미체결"   :[self.CONST_FO_ORDER_182,u"계좌번호"]})
        self.m_RealOrderListFO.update({u"03.국내주문실시간미결제"   :[self.CONST_FO_ORDER_183,u"계좌번호"]})
        self.m_RealOrderListFO.update({u"04.국내주문실시간잔고"    :[self.CONST_FO_ORDER_184,u"계좌번호"]})
        self.m_RealOrderListFO.update({u"05.국내주문실시간체결"    :[self.CONST_FO_ORDER_185,u"계좌번호"]})
        pass
    
    def SetRealToDictionary_FR(self):
        self.m_RealSiseListFR.update({u"01.해외선물호가"   :[self.CONST_FR_SISE_76,u"종목"]})
        self.m_RealSiseListFR.update({u"02.해외선물체결"   :[self.CONST_FR_SISE_82,u"종목"]})
        
        self.m_RealOrderListFR.update({u"01.해외주문실시간접수"     :[self.CONST_FR_ORDER_196,u"계좌번호"]})
        self.m_RealOrderListFR.update({u"02.해외주문실시간미체결"   :[self.CONST_FR_ORDER_186,u"계좌번호"]})
        self.m_RealOrderListFR.update({u"03.해외주문실시간미결제"   :[self.CONST_FR_ORDER_187,u"계좌번호"]})
        self.m_RealOrderListFR.update({u"04.해외주문실시간잔고"    :[self.CONST_FR_ORDER_188,u"계좌번호"]})
        self.m_RealOrderListFR.update({u"05.해외주문실시간체결"    :[self.CONST_FR_ORDER_189,u"계좌번호"]})
        pass
    
    def SetRealToDictionary_FX(self):
        self.m_RealSiseListFX.update({u"01.FX체결"   :[self.CONST_FX_SISE_171,u"종목"]})
        
        self.m_RealOrderListFX.update({u"01.FX주문실시간접수"     :[u"197",u"계좌번호"]})
        self.m_RealOrderListFX.update({u"02.FX주문실시간미체결"   :[u"191",u"계좌번호"]})
        self.m_RealOrderListFX.update({u"03.FX주문실시간미청산"   :[u"192",u"계좌번호"]})
        self.m_RealOrderListFX.update({u"04.FX주문실시간청산"    :[u"193",u"계좌번호"]})
        self.m_RealOrderListFX.update({u"05.FX주문실시간자산"    :[u"194",u"계좌번호"]})
        self.m_RealOrderListFX.update({u"06.FX주문실시간요약"    :[u"195",u"계좌번호"]})
        
        pass
    
    def Util_WriteLog(self, strMsg):
        now = time.localtime(time.time())
        strHead = time.strftime("[%H:%M:%S]", now)
        self.multiline.insertPlainText( strHead + strMsg + "\n")
        self.multiline.moveCursor(QTextCursor.End)

    def Util_GetCurDate(self):
        now = time.localtime(time.time())
        strDate = time.strftime("%Y%m%d", now)
        return strDate
    
    def Util_FillStr(self, strSrc, nTotLen, chPad):
        nPadLen = nTotLen - len(strSrc)
        strPad = strSrc + chPad * nPadLen
        return strPad

    def OnConnect(self):
        self.Util_WriteLog("Connect")
        ret = self.HDF.dynamicCall("CommInit(1)")
        
    def OnDisConnect(self):
        self.OnLogout()
        self.HDF.dynamicCall("CommTerminate(1)")
    
    def OnLogin(self):
        self.Util_WriteLog("Login")
        strUserID = self.Edit_UserID.text()
        strUserPW = self.Edit_UserPW.text()
        strCertPW = self.Edit_CertPW.text()
        nRet = self.HD_Login(strUserID, strUserPW, strCertPW)
        pass
    
    def OnLogout(self):
        self.Util_WriteLog("Logout")
        strUserID = self.Edit_UserID.text()
        self.HD_Logout(strUserID)
        pass
    
    def OnReqJmCodeFr(self):
        self.Util_WriteLog("해외종목요청")
        self.HD_CommReqMakeCod()
        pass
    
    def GetAccOrTrDic(self, strQryTp):
        tmpList = []
        tmpAccDic = {}
        tmpTrDic = {}
        tmpFidDic = {}
        
        if ( self.BtnRadio1.isChecked() == True ):      #국내
            tmpAccDic = self.m_AccListFO
            if ( self.BtnRadioQry1.isChecked() == True ):   #TR조회
                tmpTrDic = self.m_TrQueryListFO
            elif ( self.BtnRadioQry2.isChecked() == True ): #FID조회
                tmpTrDic = self.m_FidQueryListFO
            elif ( self.BtnRadioQry3.isChecked() == True ): #주문
                tmpTrDic = self.m_TrOrderListFO
        elif ( self.BtnRadio2.isChecked() == True ):    #해외
            tmpAccDic = self.m_AccListFR
            if ( self.BtnRadioQry1.isChecked() == True ):   #TR조회
                tmpTrDic = self.m_TrQueryListFR
            elif ( self.BtnRadioQry2.isChecked() == True ): #FID조회
                tmpTrDic = self.m_FidQueryListFR
            elif ( self.BtnRadioQry3.isChecked() == True ): #주문
                tmpTrDic = self.m_TrOrderListFR
        elif ( self.BtnRadio3.isChecked() == True ):    #FX
            tmpAccDic = self.m_AccListFX
            if ( self.BtnRadioQry1.isChecked() == True ):   #TR조회
                tmpTrDic = self.m_TrQueryListFX
            elif ( self.BtnRadioQry2.isChecked() == True ): #FID조회
                tmpTrDic = self.m_FidQueryListFX
            elif ( self.BtnRadioQry3.isChecked() == True ): #주문
                tmpTrDic = self.m_TrOrderListFX
                
        if ( strQryTp == "ACC"):
            return tmpAccDic
        elif (strQryTp == "TR"):
            return tmpTrDic
            pass
        
        pass
    
    def GetRealDic(self):
        tmpRealDic = {}
        
        if ( self.BtnRadio1.isChecked() == True ):  # 국내
            if ( self.BtnRadioSiseReal.isChecked() == True ):
                # 시세 로드
                tmpRealDic = self.m_RealSiseListFO 
            else:
                #주문 로드
                tmpRealDic = self.m_RealOrderListFO
            pass
        elif ( self.BtnRadio2.isChecked() == True ):
            if ( self.BtnRadioSiseReal.isChecked() == True ):
                # 시세 로드
                tmpRealDic = self.m_RealSiseListFR 
            else:
                #주문 로드
                tmpRealDic = self.m_RealOrderListFR
            pass
        elif ( self.BtnRadio3.isChecked() == True ):
            if ( self.BtnRadioSiseReal.isChecked() == True ):
                # 시세 로드
                tmpRealDic = self.m_RealSiseListFX 
            else:
                #주문 로드
                tmpRealDic = self.m_RealOrderListFX
            pass
        
        return tmpRealDic
        
        pass
    
    def OnRadioAcctTpChange(self):
        self.ComboAcc.clear()
        
        tmpAccDic = self.GetAccOrTrDic("ACC")
        tmpList = list(tmpAccDic.keys())
        for i in range(len(tmpList)):
            strAcctNo = tmpList[i]
            self.ComboAcc.addItem(strAcctNo)
            pass
        
        self.OnRadioTrChange()
        self.OnRadioRealChange()
        
        pass
    
    def OnRadioTrChange(self):
        self.ComboTr.clear()
        
        tmpTrDic = self.GetAccOrTrDic("TR")
        tmpList = list(tmpTrDic.keys())
        for i in range(len(tmpList)):
            strServiceNm = tmpList[i]
            self.ComboTr.addItem(strServiceNm)
        
        self.ComboTr.model().sort(0)
        
        bShow = False
        if ( self.BtnRadioQry2.isChecked() == True ):
            bShow = True
            
        self.Edit_Input_FID.setVisible(bShow)
        
        self.m_bUseComboTrChange = True
        
        pass
    
    def OnComboAccChange(self, strSelData):
        strInData = unicode(strSelData, 'utf-8')
        tmpAccDic = self.GetAccOrTrDic("ACC")
        strAcctNm = tmpAccDic.get(strInData)
        self.label_acctNm.setText(_fromUtf8(strAcctNm))
        
        self.OnComboTrChange(self.ComboTr.currentText())
        pass
    
    def OnComboTrChange(self, strSelData):
        strInData = unicode(strSelData, 'utf-8')
        tmpList = []
        tmpTrDic = {}

        tmpTrDic = self.GetAccOrTrDic("TR")
        tmpList = tmpTrDic.get(strInData)

        if ( tmpList is None ):
            return None

        strTrCode = tmpList[0]
        self.label_tr.setText(strTrCode) 
        
        #data parsing
        strData = str(tmpList[1])
        commaArr = strData.split(",")
        strInput = ""
        self.Edit_Input.setText(strInput)
        
        for strItem in commaArr:
            if ( strItem == "계좌번호" ):
                strAcctNo = self.ComboAcc.currentText()
                strAcctNo = str(strAcctNo).strip()
                if ( self.BtnRadio1.isChecked() == True ):      #국내
                    strInput += self.Util_FillStr(strAcctNo, 11, ' ')
                else:
                    strInput += self.Util_FillStr(strAcctNo, 6, ' ')
            elif ( strItem == "계좌비번" ):
                strInput += self.Util_FillStr(self.Edit_AcctPW.text(), 8, ' ')
            elif ( strItem == "딜러번호" ):
                strInput += self.Util_FillStr("001", 3, ' ')    #무조건 "001"
            elif ( strItem == "조회일자" or strItem == "시작일자" or strItem == "종료일자"):
                strInput += self.Util_FillStr(self.Util_GetCurDate(), 8, ' ')
            elif ( strItem == "exchtp" ):
                strInput += self.Util_FillStr("1", 1, ' ')   #1:KRX
            elif ( strItem == "exchid" ):
                strInput += self.Util_FillStr("01", 2, ' ')   #종목명의 2,3번째자리 ex)101FC000 -> 01
            elif ( strItem == "fotp" ):
                strInput += self.Util_FillStr("F", 1, ' ')   #F:선물, O:옵션
            elif ( strItem == "종목" ):
                if ( self.BtnRadio1.isChecked() == True ):      #국내
                    if ( self.BtnRadioQry2.isChecked() == True ):
                        strInput += self.Util_FillStr(self.CONST_FO_SERIES, 8, ' ')   #F:선물, O:옵션
                    else:
                        strInput += self.Util_FillStr(self.CONST_FO_SERIES, 32, ' ')   #F:선물, O:옵션
                elif ( self.BtnRadio2.isChecked() == True ):
                    strInput += self.Util_FillStr(self.CONST_FR_SERIES, 32, ' ')
                elif ( self.BtnRadio3.isChecked() == True ):
                    strInput += self.Util_FillStr(self.CONST_FX_SERIES, 32, ' ')
            elif ( strItem == "매매구분" ):
                strInput += self.Util_FillStr("1", 1, ' ')   #1:매수, 2:매도
            elif ( strItem == "체결구분" ):
                strInput += self.Util_FillStr("1", 1, ' ')   #1:지정가, 2:시장가
            elif ( strItem == "체결조건" ):
                if ( self.BtnRadio1.isChecked() == True ):      #국내
                    strInput += self.Util_FillStr("1", 1, ' ')   #1:FAS, 2:FOK, 3:FAK
                else:
                    strInput += self.Util_FillStr("1", 1, ' ')   #0:DAY, 1:GTC, 3:IOC FAK 4:FOK
            elif ( strItem == "주문가격" ):
                if ( self.BtnRadio1.isChecked() == True ):      #국내
                    strInput += self.Util_FillStr("23055", 13, ' ')   #주문가격, 소수점 없음
                else:
                    strInput += self.Util_FillStr("8808", 15, ' ')   #주문가격, 소수점 없음
            elif ( strItem == "주문수량" ):
                strInput += self.Util_FillStr("1", 5, ' ')   #주문수량
            elif ( strItem == "주문번호" ):
                if ( self.BtnRadio1.isChecked() == True ):      #국내
                    strInput += self.Util_FillStr("1111111", 7, ' ')
                else:
                    strInput += self.Util_FillStr("0000000000", 10, ' ')
            elif ( strItem == "사용자ID" ):
                strInput += self.Util_FillStr(self.Edit_UserID.text(), 8, ' ')
            elif ( strItem == "조회유형" ): #무조건 1
                strInput += self.Util_FillStr("1", 1, ' ')
            elif ( strItem == "groupnm" ):
                strInput += self.Util_FillStr(" ", 20, ' ')
            elif ( strItem == "통화코드" ):
                strInput += self.Util_FillStr("USD", 3, ' ') #USD(미국달러),EUR(유로화),JPY(일본엔화),KRW(한국원화)
            elif ( strItem == "부서코드" ):
                strInput += self.Util_FillStr("001", 3, ' ')
            elif ( strItem == "적요구분" ):
                strInput += self.Util_FillStr("1", 1, ' ') # 0(전체), 1(손익), 2(이자), 3(수수료), 4(입금), 5(출금), 6(기타)
            elif ( strItem == "조회모드" ):
                strInput += self.Util_FillStr("3", 1, ' ')   #(1:LT 2:LE 3:EQ 4:GE 5:GT)
            elif ( strItem == "조회건수" ):
                strInput += self.Util_FillStr("0001", 4, ' ')
            elif ( strItem == "keyvalue"):
                strInput += self.Util_FillStr(" ", 18, ' ')
            elif ( strItem == "조회시간"):
                strInput += self.Util_FillStr("9999999999", 10, ' ')
            elif ( strItem == "다음구분"):
                strInput += self.Util_FillStr("0", 1, ' ')   # 0:기본
            elif ( strItem == "데이터구분"):
                strInput += self.Util_FillStr("1", 1, ' ')   # 3:일, 4:주, 5:월, 2:분, 6:틱
            elif ( strItem == "시간구분"):
                strInput += self.Util_FillStr("001", 3, ' ')   # N분/틱
            elif ( strItem == "데이터건수"):
                strInput += self.Util_FillStr("00010", 5, ' ')   # 조회요청개수
            elif ( strItem == "장구분"):
                strInput += self.Util_FillStr("1", 1, ' ')   # 1:전산장, 0:본장
            elif ( strItem == "허봉실봉구분"):
                strInput += self.Util_FillStr("0", 1, ' ')   # 0:실봉만, 1:허봉+실봉
            
            else:
                try:
                    nFieldLen = int(strItem)
                    if ( type(nFieldLen) is int ):  #입력필드 길이 이므로 pass
                        pass
                    else:
                        self.Util_WriteLog("UnKnown FieldNm : " + strItem)
                except ValueError:
                    pass
        
        self.Edit_Input.setText( "[" + strInput + "]")
        
        if ( self.BtnRadioQry2.isChecked() == True ):
            strFidInput = str(tmpList[2])
            self.Edit_Input_FID.setText("[" + strFidInput + "]")
            pass
    
    def OnQuery(self):
        strTr = self.label_tr.text()
        
        strInput = self.Edit_Input.text()
        strInput.replace("[", "")
        strInput.replace("]", "")
        
        strInputFid = self.Edit_Input_FID.text()
        strInputFid.replace("[", "")
        strInputFid.replace("]", "")
        
        nRqID = 0
        strMsg = ""
        if ( self.BtnRadioQry3.isChecked() == True ):
            nRqID = self.HD_CommJumunSvr(strTr, strInput)
            strMsg = u"주문전송 UniqueID : " + str(nRqID)
        elif ( self.BtnRadioQry2.isChecked() == True ):
            nRqID = self.HD_CommFIDRqData(strTr, strInput, strInputFid, "")
            strMsg = u"조회요청(FID) UniqueID : " + str(nRqID)
        else:
            nRqID = self.HD_CommRqData(strTr, strInput, len(strInput), u"")
            strMsg = u"조회요청(TR) UniqueID : " + str(nRqID)
            
        self.Util_WriteLog( strMsg )
        
        pass
    
    def OnRadioRealChange(self):
        
        self.ComboReal.clear()
        self.SetComboReal()
        
        strSelData = self.ComboReal.currentText()
        self.OnComboReal(strSelData)
        
        pass
    
    def SetComboReal(self):
        tmpRealDic = {}
        
        tmpRealDic = self.GetRealDic()
        
        tmpList = []
        tmpList = list(tmpRealDic.keys())
        for i in range(len(tmpList)):
            strRealServNo = tmpList[i]
            self.ComboReal.addItem(strRealServNo)
            
        self.ComboReal.model().sort(0)
        
        pass
    
    
    def OnComboReal(self, strSelData):
        
        strInData = unicode(strSelData, 'utf-8')
        tmpList = []
        
        tmpRealDic = {}
        tmpRealDic = self.GetRealDic()
        tmpList = tmpRealDic.get(strInData)
        
        strInput = ""
        
        try:
            strItem = tmpList[0]
            self.Edit_Input_Real_Key.setText(tmpList[0])
            
            strItem = tmpList[1]
            if ( strItem == "종목" ):
                if ( self.BtnRadio1.isChecked() == True ):
                    strInput = self.Util_FillStr(self.CONST_FO_SERIES, 32, ' ')
                elif ( self.BtnRadio2.isChecked() == True ):
                    strInput += self.Util_FillStr(self.CONST_FR_SERIES, 32, ' ')
                elif ( self.BtnRadio3.isChecked() == True ):
                    strInput += self.Util_FillStr(self.CONST_FX_SERIES, 10, ' ')
                pass
            elif (strItem == "계좌번호" ):
                strAcctNo = self.ComboAcc.currentText()
                strInput = self.Util_FillStr(strAcctNo, 11, ' ')
                pass
            
        except TypeError:
            pass
        
        self.Edit_Input_Real_Val.setText( "[" + strInput + "]" )
        
        pass
    
    def OnRealRegister(self):
        bReg = True
        strMsg = ""
        strValue = self.Edit_Input_Real_Val.text()
        nKeyType = int(self.Edit_Input_Real_Key.text())
        
        strValue.replace("[", "")
        strValue.replace("]", "")
        
        if ( self.BtnRadioRealRegister.isChecked() == True ):
            bReg = True
            strMsg = u"실시간 등록 요청 Value[%s] Key[%d]" %(strValue, nKeyType) 
        elif ( self.BtnRadioRealUnRegister.isChecked() == True ):
            bReg = False
            strMsg = u"실시간 해지 요청 Value[%s] Key[%d]" %(strValue, nKeyType) 
        
        self.Util_WriteLog( strMsg )
        
        self.HD_RegReal(bReg, strValue, nKeyType)
        pass
    
    def OnDataRecv(self, sTrCode, nRqID):
        strRecv = u"조회응답 Tr = [%s] UniqueID = [%d]" %(sTrCode, nRqID)
        self.Util_WriteLog( strRecv )
        i = 0
        if ( sTrCode == "o44020"): #MRKT.cod
            pass
        elif ( sTrCode == "o44021"): #PMCode.cod
            pass
        elif ( sTrCode == "o44022" ): #JMCode.cod(해외)
            nRptCnt = self.HD_CommGetRepeatCnt(sTrCode, -1, u"OutRec1")
            
            self.Util_WriteLog( u"수신개수 : " + str(nRptCnt))
            
            for i in nRptCnt:
                strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"종목코드")
                self.Util_WriteLog(strData)
            pass
        elif (sTrCode == "g11002.DQ0104&" ):    #국내미체결조회
            self.procDQ0104(sTrCode)
            pass
        elif (sTrCode == "g11002.DQ0107&" ):    #국내체결조회
            self.procDQ0107(sTrCode)
            pass
        elif (sTrCode == "g11002.DQ0110&" ):    #국내미체결조회
            self.procDQ0110(sTrCode)
            pass
        elif (sTrCode == "g11002.DQ0242&" ):    #국내고객예탁금조회
            self.procDQ0242(sTrCode)
            pass
        elif  sTrCode == "g11002.DQ0622&":#종목코드(국내)
            nRptCnt = self.HD_CommGetRepeatCnt(sTrCode, -1, u"OutRec1")
            for i in nRptCnt:
                strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"종목코드")
                self.Util_WriteLog(strData)
            pass
        elif (sTrCode == "g11002.DQ0116&"):
            pass
        elif (sTrCode == "g11002.DQ0119&"): #야간CME체결내역리스트
            pass
        elif (sTrCode == "g11002.DQ0122&"): #야간CME미결제약정조회
            pass
        elif (sTrCode == "s20001" ):            #국내선물옵션마스터
            self.procs20001(sTrCode)
            pass
        elif (sTrCode == "s30001" ):            #국내주식선물마스터
            pass
        elif (sTrCode == "s21001" ):            #야간CME선물옵션마스터
            pass
        elif (sTrCode == "g12001.DO1601&" ):    #국내신규주문
            self.procDO1000(sTrCode)
            pass
        elif (sTrCode == "g12001.DO1901&" ):    #국내정정주문
            self.procDO1000(sTrCode)
            pass
        elif (sTrCode == "g12001.DO1701&" ):    #국내취소주문
            self.procDO1000(sTrCode)
            pass
        elif (sTrCode == "g12001.DO2201&" ):    #CME국내신규주문
            self.procDO2000(sTrCode)
            pass
        elif (sTrCode == "g12001.DO2101&" ):    #CME국내정정주문
            self.procDO2000(sTrCode)
            pass
        elif (sTrCode == "g12001.DO2001&" ):    #CME국내취소주문
            self.procDO2000(sTrCode)
            pass
        
        elif (sTrCode == "g11004.AQ0401%"):     #해외미체결주문내역
            self.procAQ0401(sTrCode)
            pass
        elif (sTrCode == "g11004.AQ0402%"):     #해외미체결주문내역
            self.procAQ0402(sTrCode)
            pass
        elif (sTrCode == "g11004.AQ0403%"):
            self.procAQ0403(sTrCode)
            pass
        elif (sTrCode == "g11004.AQ0607%"):
            self.procAQ0607(sTrCode)
            pass
        elif (sTrCode == "o51000" ):            #해외체결정보
            self.proco51000(sTrCode)
            pass
        elif (sTrCode == "o51010" ):            #해외호가정보
            self.proco51010(sTrCode)
            pass
        elif (sTrCode == "o51210" ):            #해외종목정보
            self.proco51210(sTrCode)
            pass
        elif (sTrCode == "g12003.AO0401" ):            #해외신규주문
            self.procAO0400(sTrCode)
            pass
        elif (sTrCode == "g12003.AO0402" ):            #해외정정주문
            self.procAO0400(sTrCode)
            pass
        elif (sTrCode == "g12003.AO0403" ):            #해외취소주문
            self.procAO0400(sTrCode)
            pass

        elif sTrCode == "g11004.AQ0101%":   #ㅖ좌목록 수신
            self.Util_WriteLog(u"Recv Account List")
            self.procAQ0101(sTrCode)
            pass
        elif sTrCode == "g11004.AQ0450%":       #해외미체결주문내역
            self.procAQ0450(sTrCode)
            pass
        
    def OnGetBroadData(self, sTrCode, nRealType):
        strRealType = unicode(str(nRealType), 'utf-8') 
        self.Util_WriteLog (u"실시간 수신 [%s] [%s]" % (sTrCode, strRealType))
        if ( strRealType == self.CONST_FO_SISE_51 ):
            self.procReal51(sTrCode, nRealType)
            pass
        elif ( strRealType == self.CONST_FO_SISE_65 ):
            self.procReal65(sTrCode, nRealType)
            pass
        elif ( strRealType == self.CONST_FO_SISE_58 ):
            self.procReal58(sTrCode, nRealType)
            pass
        elif ( strRealType == self.CONST_FO_SISE_71 ):
            self.procReal71(sTrCode, nRealType)
            pass
        elif ( strRealType == self.CONST_FO_ORDER_181 ):
            self.procReal181(sTrCode, nRealType)
            pass
        elif ( strRealType == self.CONST_FO_ORDER_182 ):
            self.procReal182(sTrCode, nRealType)
            pass
        elif ( strRealType == self.CONST_FO_ORDER_183 ):
            self.procReal183(sTrCode, nRealType)
            pass
        elif ( strRealType == self.CONST_FO_ORDER_184 ):
            self.procReal184(sTrCode, nRealType)
            pass
        elif ( strRealType == self.CONST_FO_ORDER_185 ):
            self.procReal185(sTrCode, nRealType)
            pass
        
        elif ( strRealType == self.CONST_FR_SISE_76 ):
            self.procReal76(sTrCode, nRealType)
            pass
        elif ( strRealType == self.CONST_FR_SISE_82 ):   # 해외 시세 체결
            strCode = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"종목코드")
            strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"기준체결시간")
            strPric = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"체결가")
            self.Util_WriteLog("[" + strCode + "][" + strData + "][" + strPric + "]")
            pass
        elif (strRealType == self.CONST_FR_ORDER_196 ):
            self.procReal196(sTrCode, nRealType)
            pass
        elif (strRealType == self.CONST_FR_ORDER_186 ):
            self.procReal186(sTrCode, nRealType)
            pass
        elif (strRealType == self.CONST_FR_ORDER_187 ):
            self.procReal187(sTrCode, nRealType)
            pass
        elif (strRealType == self.CONST_FR_ORDER_188 ):
            self.procReal188(sTrCode, nRealType)
            pass
        elif (strRealType == self.CONST_FR_ORDER_189 ):
            self.procReal189(sTrCode, nRealType)
            pass
        elif (strRealType == self.CONST_FX_SISE_171):
            self.procReal171(sTrCode, nRealType)
            pass
        
        pass
    
    def procReal171(self, sTrCode, nRealType):
        strTime = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"호가시간    ")
        strBid = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매수호가    ")
        strData = strTime + " " + strBid
        self.Util_WriteLog(strData)
        pass
    
    def procReal189(self, sTrCode, nRealType):
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문번호    ")
        self.Util_WriteLog(u"주문번호    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"계좌번호    ")
        self.Util_WriteLog(u"계좌번호    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"계좌명        ")
        self.Util_WriteLog(u"계좌명        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"종목        ")
        self.Util_WriteLog(u"종목        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매매구분    ")
        self.Util_WriteLog(u"매매구분    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문가격    ")
        self.Util_WriteLog(u"주문가격    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문수량    ")
        self.Util_WriteLog(u"주문수량    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"체결가격    ")
        self.Util_WriteLog(u"체결가격    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"체결수량    ")
        self.Util_WriteLog(u"체결수량    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"STOP가격    ")
        self.Util_WriteLog(u"STOP가격    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"약정금액    ")
        self.Util_WriteLog(u"약정금액    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문유형    ")
        self.Util_WriteLog(u"주문유형    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"수수료        ")
        self.Util_WriteLog(u"수수료        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문일자    ")
        self.Util_WriteLog(u"주문일자    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문시간    ")
        self.Util_WriteLog(u"주문시간    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문자        ")
        self.Util_WriteLog(u"주문자        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"체결일자    ")
        self.Util_WriteLog(u"체결일자        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"체결시간    ")
        self.Util_WriteLog(u"체결시간    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"거래소일자    ")
        self.Util_WriteLog(u"거래소일자    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"거래소시간    ")
        self.Util_WriteLog(u"거래소시간    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"통화코드    ")
        self.Util_WriteLog(u"통화코드    : " + strData)
        pass
    
    def procReal188(self, sTrCode, nRealType):
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"계좌번호  ")
        self.Util_WriteLog(u"계좌번호  : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"계좌명    ")
        self.Util_WriteLog(u"계좌명    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문가능금액")
        self.Util_WriteLog(u"주문가능금액: " + strData)
        pass
        
    def procReal187(self, sTrCode, nRealType):
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"계좌번호        ")
        self.Util_WriteLog(u"계좌번호        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"계좌명            ")
        self.Util_WriteLog(u"계좌명            : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"종목            ")
        self.Util_WriteLog(u"종목            : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매매구분        ")
        self.Util_WriteLog(u"매매구분        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"평균단가        ")
        self.Util_WriteLog(u"평균단가        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"당일미결제수량    ")
        self.Util_WriteLog(u"당일미결제수량    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"미결제약정금액    ")
        self.Util_WriteLog(u"미결제약정금액    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"신규체결수량        ")
        self.Util_WriteLog(u"신규체결수량        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"신규체결금액        ")
        self.Util_WriteLog(u"신규체결금액        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"전환매체결수량    ")
        self.Util_WriteLog(u"전환매체결수량    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"전환매체결금액    ")
        self.Util_WriteLog(u"전환매체결금액    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"전환매Hold수량    ")
        self.Util_WriteLog(u"전환매Hold수량    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"수동청산Hold수량    ")
        self.Util_WriteLog(u"수동청산Hold수량    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"위탁증거금        ")
        self.Util_WriteLog(u"위탁증거금        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"통화코드        ")
        self.Util_WriteLog(u"통화코드        : " + strData)
        pass
    
    def procReal186(self, sTrCode, nRealType):
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"계좌번호        ")
        self.Util_WriteLog(u"계좌번호        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"계좌명            ")
        self.Util_WriteLog(u"계좌명        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"기준일자        ")
        self.Util_WriteLog(u"기준일자        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문번호        ")
        self.Util_WriteLog(u"주문번호        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"종목            ")
        self.Util_WriteLog(u"종목            : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매매구분        ")
        self.Util_WriteLog(u"매매구분        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문가격        ")
        self.Util_WriteLog(u"주문가격        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문수량        ")
        self.Util_WriteLog(u"주문수량        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"체결수량        ")
        self.Util_WriteLog(u"체결수량        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"잔량            ")
        self.Util_WriteLog(u"잔량            : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"STOP 가격        ")
        self.Util_WriteLog(u"STOP 가격    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문시간        ")
        self.Util_WriteLog(u"주문시간        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"거래소시간        ")
        self.Util_WriteLog(u"거래소시간        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문자            ")
        self.Util_WriteLog(u"주문자        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문구분        ")
        self.Util_WriteLog(u"주문구분        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"전략구분        ")
        self.Util_WriteLog(u"전략구분        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"최초원주문번호        ")
        self.Util_WriteLog(u"최초원주문번호    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"그룹주문번호        ")
        self.Util_WriteLog(u"그룹주문번호    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매수주문수량        ")
        self.Util_WriteLog(u"매수주문수량    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매도주문수량        ")
        self.Util_WriteLog(u"매도주문수량    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문유형        ")
        self.Util_WriteLog(u"주문유형        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"가격조건        ")
        self.Util_WriteLog(u"가격조건        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"체결조건        ")
        self.Util_WriteLog(u"체결조건        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"GTD일자        ")
        self.Util_WriteLog(u"GTD일자        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"IOC최소체결수량    ")
        self.Util_WriteLog(u"IOC최소체결수량    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"원주문번호        ")
        self.Util_WriteLog(u"원주문번호        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문순서        ")
        self.Util_WriteLog(u"주문순서        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"통화코드        ")
        self.Util_WriteLog(u"통화코드        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문금액        ")
        self.Util_WriteLog(u"주문금액        : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문시작일자        ")
        self.Util_WriteLog(u"주문시작일자    : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"옵션행사예약여부    ")    
        self.Util_WriteLog(u"옵션행사예약여부    : " + strData)
        pass
    
    def procReal196(self, sTrCode, nRealType):
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문번호")
        self.Util_WriteLog(u"주문번호 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"종목코드")
        self.Util_WriteLog(u"종목코드 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문가격")
        self.Util_WriteLog(u"주문가격 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문수량")
        self.Util_WriteLog(u"주문수량 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매매구분")
        self.Util_WriteLog(u"매매구분 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문시간")
        self.Util_WriteLog(u"주문시간 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문구분")
        self.Util_WriteLog(u"주문구분(1:신규, 2:정정, 3:취소) : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"처리구분")
        self.Util_WriteLog(u"처리구분(0:정상, 1:거부) : " + strData)
        pass
    
    def procReal181(self, sTrCode, nRealType):
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문번호")
        self.Util_WriteLog(u"주문번호 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"종목코드")
        self.Util_WriteLog(u"종목코드 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문가격")
        self.Util_WriteLog(u"주문가격 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문수량")
        self.Util_WriteLog(u"주문수량 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매매구분")
        self.Util_WriteLog(u"매매구분 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"접수시간")
        self.Util_WriteLog(u"접수시간 : " + strData)
        pass
    
    def procReal182(self, sTrCode, nRealType):
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"key")
        self.Util_WriteLog(u"key : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"계좌번호")
        self.Util_WriteLog(u"계좌번호 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"딜러번호")
        self.Util_WriteLog(u"딜러번호 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"계좌명")
        self.Util_WriteLog(u"계좌명 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"딜러명")
        self.Util_WriteLog(u"딜러명 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문번호")
        self.Util_WriteLog(u"주문번호 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"종목코드")
        self.Util_WriteLog(u"종목코드 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매매구분")
        self.Util_WriteLog(u"매매구분 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문가격")
        self.Util_WriteLog(u"주문가격 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문수량")
        self.Util_WriteLog(u"주문수량 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"정정수량")
        self.Util_WriteLog(u"정정수량 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"취소수량")
        self.Util_WriteLog(u"취소수량 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"체결수량")
        self.Util_WriteLog(u"체결수량 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"잔량")
        self.Util_WriteLog(u"잔량 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"원주문번호")
        self.Util_WriteLog(u"원주문번호 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"최초원주문번호")
        self.Util_WriteLog(u"최초원주문번호 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"통신주문구분")
        self.Util_WriteLog(u"통신주문구분 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문전략구분")
        self.Util_WriteLog(u"주문전략구분 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문조작구분")
        self.Util_WriteLog(u"주문조작구분 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"거래소접수시간")
        self.Util_WriteLog(u"거래소접수시간 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"작업사원")
        self.Util_WriteLog(u"작업사원 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"그룹주문번호")
        self.Util_WriteLog(u"그룹주문번호 : " + strData)
        pass
    
    def procReal183(self, sTrCode, nRealType):
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"key")
        self.Util_WriteLog(u"key : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"계좌번호")
        self.Util_WriteLog(u"계좌번호 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"딜러번호")
        self.Util_WriteLog(u"딜러번호 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"계좌명")
        self.Util_WriteLog(u"계좌명 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"딜러명")
        self.Util_WriteLog(u"딜러명 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"종목")
        self.Util_WriteLog(u"종목 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매매구분")
        self.Util_WriteLog(u"매매구분 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"전일 미결제 수량")
        self.Util_WriteLog(u"전일 미결제 수량 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"당일 미결제 수량")
        self.Util_WriteLog(u"당일 미결제 수량 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"미체결수량")
        self.Util_WriteLog(u"미체결수량 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"평균 단가")
        self.Util_WriteLog(u"평균 단가 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"당일미결제약정금액")
        self.Util_WriteLog(u"당일미결제약정금액 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"신규체결수량")
        self.Util_WriteLog(u"신규체결수량 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"신규체결금액")
        self.Util_WriteLog(u"신규체결금액 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"전환매 체결수량")
        self.Util_WriteLog(u"전환매 체결수량 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"전환매 체결금액")
        self.Util_WriteLog(u"전환매 체결금액 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"행사 신청수량")
        self.Util_WriteLog(u"행사 신청수량 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"행사 배정수량")
        self.Util_WriteLog(u"행사 배정수량 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"행사 거부수량")
        self.Util_WriteLog(u"행사 거부수량 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"만기수량")
        self.Util_WriteLog(u"행사 만기수량 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"장부단가")
        self.Util_WriteLog(u"장부단가 : " + strData)
        pass
    
    def procReal184(self, sTrCode, nRealType):
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"계좌번호")
        self.Util_WriteLog(u"계좌번호 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"딜러번호")
        self.Util_WriteLog(u"딜러번호 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"계좌명")
        self.Util_WriteLog(u"계좌명 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"딜러명")
        self.Util_WriteLog(u"딜러명 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문가능금액")
        self.Util_WriteLog(u"주문가능금액 : " + strData)
        pass
    
    def procReal185(self, sTrCode, nRealType):
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"key")
        self.Util_WriteLog(u"key : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"계좌번호")
        self.Util_WriteLog(u"계좌번호 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"딜러번호")
        self.Util_WriteLog(u"딜러번호 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"계좌명")
        self.Util_WriteLog(u"계좌명 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"딜러명")
        self.Util_WriteLog(u"딜러명 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"주문번호")
        self.Util_WriteLog(u"주문번호 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"종목코드")
        self.Util_WriteLog(u"종목코드 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매매구분")
        self.Util_WriteLog(u"매매구분 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"체결가격")
        self.Util_WriteLog(u"체결가격 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"체결수량")
        self.Util_WriteLog(u"체결수량 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"체결시간")
        self.Util_WriteLog(u"체결시간 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"전략구분")
        self.Util_WriteLog(u"전략구분 : " + strData)
        pass
    
    def procReal76(self, sTrCode, nRealType):
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"종목코드")
        self.Util_WriteLog(u"종목코드 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"호가시간")
        self.Util_WriteLog(u"호가시간 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매도호가1")
        self.Util_WriteLog(u"매도호가1 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매수호가1")
        self.Util_WriteLog(u"매수호가1 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매도호가수량1")
        self.Util_WriteLog(u"매도호가수량1 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매수호가수량1")
        self.Util_WriteLog(u"매수호가수량1 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매도호가건수1")
        self.Util_WriteLog(u"매도호가건수1 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매수호가건수1")
        self.Util_WriteLog(u"매수호가건수1 : " + strData)
        pass
    
    def procReal71(self, sTrCode, nRealType):
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"종목코드")
        self.Util_WriteLog(u"종목코드 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"체결시간")
        self.Util_WriteLog(u"체결시간 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"change")
        self.Util_WriteLog(u"전일대비 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"현재가")
        self.Util_WriteLog(u"현재가 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"cvolume")
        self.Util_WriteLog(u"체결량 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"누적거래량")
        self.Util_WriteLog(u"누적거래량 : " + strData)
        pass
    
    def procReal58(self, sTrCode, nRealType):
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"kfutcode")
        self.Util_WriteLog(u"종목코드 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"hotime")
        self.Util_WriteLog(u"호가시간 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"offerho1")
        self.Util_WriteLog(u"매도호가1 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"bidho1")
        self.Util_WriteLog(u"매수호가1 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"offerrem1")
        self.Util_WriteLog(u"매도호가수량1 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"bidrem1")
        self.Util_WriteLog(u"매수호가수량1 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"offercnt1")
        self.Util_WriteLog(u"매도호가건수1 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"bidcnt1")
        self.Util_WriteLog(u"매수호가건수1 : " + strData)
        pass
    
    def procReal51(self, sTrCode, nRealType):
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"종목코드")
        self.Util_WriteLog(u"종목코드 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"호가시간")
        self.Util_WriteLog(u"호가시간 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매도호가1")
        self.Util_WriteLog(u"매도호가1 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매수호가1")
        self.Util_WriteLog(u"매수호가1 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매도호가수량1")
        self.Util_WriteLog(u"매도호가수량1 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매수호가수량1")
        self.Util_WriteLog(u"매수호가수량1 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매도호가건수1")
        self.Util_WriteLog(u"매도호가건수1 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"매수호가건수1")
        self.Util_WriteLog(u"매수호가건수1 : " + strData)
        pass
    
    def procReal65(self, sTrCode, nRealType):
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"종목코드")
        self.Util_WriteLog(u"종목코드 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"체결시간")
        self.Util_WriteLog(u"체결시간 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"전일대비")
        self.Util_WriteLog(u"전일대비 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"현재가")
        self.Util_WriteLog(u"현재가 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"체결량")
        self.Util_WriteLog(u"체결량 : " + strData)
        strData = self.HD_CommGetData(sTrCode, nRealType, u"OutRec1", 0, u"누적거래량")
        self.Util_WriteLog(u"누적거래량 : " + strData)
        pass
    
    def procDO1000(self, sTrCode):
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"접수구분")
        self.Util_WriteLog( u"접수구분                : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"처리구분")
        self.Util_WriteLog( u"처리구분                : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"계좌번호")
        self.Util_WriteLog( u"계좌번호                : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"주문번호")
        self.Util_WriteLog( u"주문번호                : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"종목코드")
        self.Util_WriteLog( u"종목코드                : " + strData)
        pass

    def procDO2000(self, sTrCode):
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"접수구분")
        self.Util_WriteLog( u"접수구분                : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"처리구분")
        self.Util_WriteLog( u"처리구분                : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"계좌번호")
        self.Util_WriteLog( u"계좌번호                : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"주문번호")
        self.Util_WriteLog( u"주문번호                : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"종목코드")
        self.Util_WriteLog( u"종목코드                : " + strData)
        pass
    
    def procAO0400(self, sTrCode):
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"접수구분")
        self.Util_WriteLog( u"접수구분                : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"주문번호")
        self.Util_WriteLog( u"주문번호                : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"처리코드")
        self.Util_WriteLog( u"처리코드                : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"처리메시지")
        self.Util_WriteLog( u"처리메시지                : " + strData)
        pass
    
    def procAQ0401(self, sTrCode):
        nRptCnt = self.HD_CommGetRepeatCnt(sTrCode, -1, u"OutRec1")
        self.Util_WriteLog( u"AQ0401 반복회수 : " + str(nRptCnt) )
        for i in nRptCnt:
            self.Util_WriteLog( u"===============================================" )
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문번호        ")
            self.Util_WriteLog( u"주문번호                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"계좌번호        ")
            self.Util_WriteLog( u"계좌번호                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"계좌명            ")
            self.Util_WriteLog( u"계좌명                    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"종목코드        ")
            self.Util_WriteLog( u"종목코드                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"매매구분        ")
            self.Util_WriteLog( u"매매구분                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문가격        ")
            self.Util_WriteLog( u"주문가격                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문수량        ")
            self.Util_WriteLog( u"주문수량                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"체결수량        ")
            self.Util_WriteLog( u"체결수량                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"잔량            ")
            self.Util_WriteLog( u"잔량                    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문유형        ")
            self.Util_WriteLog( u"주문유형                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"STOP 가격        ")
            self.Util_WriteLog( u"STOP 가격                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문시간        ")
            self.Util_WriteLog( u"주문시간                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문자            ")
            self.Util_WriteLog( u"주문자                    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"통신구분        ")
            self.Util_WriteLog( u"통신구분                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"전략구분        ")
            self.Util_WriteLog( u"전략구분                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"최초원주문번호    ")
            self.Util_WriteLog( u"최초원주문번호                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문일자        ")
            self.Util_WriteLog( u"주문일자                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"그룹 주문 번호    ")
            self.Util_WriteLog( u"그룹 주문 번호            : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"가격조건        ")
            self.Util_WriteLog( u"가격조건                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"체결조건        ")
            self.Util_WriteLog( u"체결조건                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문종료일자        ")
            self.Util_WriteLog( u"주문종료일자                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"IOC 최소체결수량    ")
            self.Util_WriteLog( u"IOC 최소체결수량            : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"통화코드        ")
            self.Util_WriteLog( u"통화코드                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문금액        ")
            self.Util_WriteLog( u"주문금액                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문시작일자        ")
            self.Util_WriteLog( u"주문시작일자                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"옵션행사예약여부    ")
            self.Util_WriteLog( u"옵션행사예약여부            : " + strData)
        pass
    
    def procAQ0402(self, sTrCode):
        nRptCnt = self.HD_CommGetRepeatCnt(sTrCode, -1, u"OutRec1")
        self.Util_WriteLog( u"AQ0402 반복회수 : " + str(nRptCnt) )
        for i in nRptCnt:
            self.Util_WriteLog( u"===============================================" )
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문번호    ")
            self.Util_WriteLog( u"주문번호    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"계좌번호    ")
            self.Util_WriteLog( u"계좌번호    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"계좌명        ")
            self.Util_WriteLog( u"계좌명        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"종목        ")
            self.Util_WriteLog( u"종목        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"매매구분    ")
            self.Util_WriteLog( u"매매구분    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문가격    ")
            self.Util_WriteLog( u"주문가격    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문수량    ")
            self.Util_WriteLog( u"주문수량    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"체결가격    ")
            self.Util_WriteLog( u"체결가격    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"체결수량    ")
            self.Util_WriteLog( u"체결수량    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"STOP가격    ")
            self.Util_WriteLog( u"STOP가격    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"약정금액    ")
            self.Util_WriteLog( u"약정금액    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문유형    ")
            self.Util_WriteLog( u"주문유형    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"수수료        ")
            self.Util_WriteLog( u"수수료        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문일자    ")
            self.Util_WriteLog( u"주문일자    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문시간    ")
            self.Util_WriteLog( u"주문시간    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문자        ")
            self.Util_WriteLog( u"주문자        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"체결일자    ")
            self.Util_WriteLog( u"체결일자    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"체결시간    ")
            self.Util_WriteLog( u"체결시간    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"거래소일자    ")
            self.Util_WriteLog( u"거래소일자    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"거래소시간    ")
            self.Util_WriteLog( u"거래소시간    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"통화코드    ")
            self.Util_WriteLog( u"통화코드    : " + strData)
        pass
    
    def procAQ0403(self, sTrCode):
        nRptCnt = self.HD_CommGetRepeatCnt(sTrCode, -1, u"OutRec1")
        print(nRptCnt)
        self.Util_WriteLog( u"AQ0403 반복회수 : " + str(nRptCnt) )
        for i in nRptCnt:
            self.Util_WriteLog( u"===============================================" )
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"계좌번호            ")
            self.Util_WriteLog( u"계좌번호            : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"계좌명                ")
            self.Util_WriteLog( u"계좌명                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"종목                ")
            self.Util_WriteLog( u"종목                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"매매구분            ")
            self.Util_WriteLog( u"매매구분            : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"당일순 미결제수량        ")
            self.Util_WriteLog( u"당일순 미결제수량        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"평균단가(소수점반영)    ")
            self.Util_WriteLog( u"평균단가(소수점반영)    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"평균단가            ")
            self.Util_WriteLog( u"평균단가            : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"현재가                ")
            self.Util_WriteLog( u"현재가                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"평가손익            ")
            self.Util_WriteLog( u"평가손익            : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"위탁증거금            ")
            self.Util_WriteLog( u"위탁증거금            : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"신규제한일            ")
            self.Util_WriteLog( u"신규제한일            : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"최종거래일            ")
            self.Util_WriteLog( u"최종거래일            : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"통화코드            ")
            self.Util_WriteLog( u"통화코드            : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"옵션시장가치            ")
            self.Util_WriteLog( u"옵션시장가치            : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"옵션행사예약여부        ")
            self.Util_WriteLog( u"옵션행사예약여부        : " + strData)
        pass
    
    def procAQ0607(self, sTrCode):
        nRptCnt = self.HD_CommGetRepeatCnt(sTrCode, -1, u"OutRec1")
        self.Util_WriteLog( u"AQ0607 반복회수 : " + str(nRptCnt) )
        for i in nRptCnt:
            self.Util_WriteLog( u"===============================================" )
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"계좌번호            ")
            self.Util_WriteLog( u"계좌번호            : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"계좌명                ")
            self.Util_WriteLog( u"계좌명                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"통화코드            ")
            self.Util_WriteLog( u"통화코드            : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"예탁금총액           ")
            self.Util_WriteLog( u"예탁금총액           : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"예탁금잔액           ")
            self.Util_WriteLog( u"예탁금잔액           : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"평가금액            ")
            self.Util_WriteLog( u"평가금액            : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"미수금액            ")
            self.Util_WriteLog( u"미수금액            : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"결제금액부족          ")
            self.Util_WriteLog( u"결제금액부족          : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"미결제약정증거금        ")
            self.Util_WriteLog( u"미결제약정증거금        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"인출가능금           ")
            self.Util_WriteLog( u"인출가능금           : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문가능금           ")
            self.Util_WriteLog( u"주문가능금           : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"유지증거금           ")
            self.Util_WriteLog( u"유지증거금           : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문증거금           ")
            self.Util_WriteLog( u"주문증거금           : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"위탁증거금           ")
            self.Util_WriteLog( u"위탁증거금           : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"거래수수료           ")
            self.Util_WriteLog( u"거래수수료           : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"청산손익            ")
            self.Util_WriteLog( u"청산손익            : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"평가손익            ")
            self.Util_WriteLog( u"평가손익            : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"미발생             ")
            self.Util_WriteLog( u"미발생                 : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"추가증거금           ")
            self.Util_WriteLog( u"추가증거금           : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"총계정자산가치         ")
            self.Util_WriteLog( u"총계정자산가치         : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"외환고시환율            ")
            self.Util_WriteLog( u"외환고시환율            : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"옵션매매대금            ")
            self.Util_WriteLog( u"옵션매매대금            : " + strData)
        pass
    
    def proco51000(self, sTrCode):
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"종목코드        ")
        self.Util_WriteLog( u"종목코드        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"한글종목명        ")
        self.Util_WriteLog( u"한글종목명        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"현재가            ")
        self.Util_WriteLog( u"현재가            : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"전일대비구분        ")
        self.Util_WriteLog( u"전일대비구분        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"전일대비        ")
        self.Util_WriteLog( u"전일대비        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"전일대비등락율    ")
        self.Util_WriteLog( u"전일대비등락율    : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"직전대비구분        ")
        self.Util_WriteLog( u"직전대비구분        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"시가            ")
        self.Util_WriteLog( u"시가            : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"고가            ")
        self.Util_WriteLog( u"고가            : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"저가            ")
        self.Util_WriteLog( u"저가            : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"LT고가        ")
        self.Util_WriteLog( u"LT고가        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"LT고가일        ")
        self.Util_WriteLog( u"LT고가일        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"LT저가        ")
        self.Util_WriteLog( u"LT저가        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"LT저가일        ")
        self.Util_WriteLog( u"LT저가일        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"영업일            ")
        self.Util_WriteLog( u"영업일            : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"상승거래량        ")
        self.Util_WriteLog( u"상승거래량        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"하락거래량        ")
        self.Util_WriteLog( u"하락거래량        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"누적거래량        ")
        self.Util_WriteLog( u"누적거래량        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"만기일            ")
        self.Util_WriteLog( u"만기일            : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"정산가            ")
        self.Util_WriteLog( u"정산가            : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"정산일            ")
        self.Util_WriteLog( u"정산일            : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"잔존일수        ")
        self.Util_WriteLog( u"잔존일수        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"매도호가        ")
        self.Util_WriteLog( u"매도호가        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"매수호가        ")
        self.Util_WriteLog( u"매수호가        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"기준가            ")
        self.Util_WriteLog( u"기준가            : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"전일거래량        ")
        self.Util_WriteLog( u"전일거래량        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"전일거래량대비율    ")
        self.Util_WriteLog( u"전일거래량대비율    : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"체결시간        ")
        self.Util_WriteLog( u"체결시간        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"국내시간        ")
        self.Util_WriteLog( u"국내시간        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"거래소구분        ")
        self.Util_WriteLog( u"거래소구분        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"ETH시작        ")
        self.Util_WriteLog( u"ETH시작        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"ETH종료        ")
        self.Util_WriteLog( u"ETH종료        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"영문종목명        ")
        self.Util_WriteLog( u"영문종목명        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"신규거래제한일    ")
        self.Util_WriteLog( u"신규거래제한일    : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"최종거래일        ")
        self.Util_WriteLog( u"최종거래일        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"호가방식        ")
        self.Util_WriteLog( u"호가방식        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"주문제한폭        ")
        self.Util_WriteLog( u"주문제한폭        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"실물인수도시작일자")
        self.Util_WriteLog( u"실물인수도시작일자: " + strData)
        pass
    
    def proco51010(self, sTrCode):
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"종목코드")
        self.Util_WriteLog( u"종목코드                : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"호가시간")
        self.Util_WriteLog( u"호가시간              : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"매도호가1")
        self.Util_WriteLog( u"매도호가1               : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"매수호가1")
        self.Util_WriteLog( u"매수호가1               : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"매도호가잔량1")
        self.Util_WriteLog( u"매도호가잔량1               : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"매수호가잔량1")
        self.Util_WriteLog( u"매수호가잔량1               : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"매도호가건수1")
        self.Util_WriteLog( u"매도호가건수1               : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"매수호가건수1")
        self.Util_WriteLog( u"매수호가건수1               : " + strData)
        pass
    
    def proco51210(self, sTrCode):
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"종목코드")
        self.Util_WriteLog( u"종목코드                : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"최종거래일")
        self.Util_WriteLog( u"최종거래일              : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"거래소")
        self.Util_WriteLog( u"거래소               : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"상장일")
        self.Util_WriteLog( u"상장일               : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"만기일")
        self.Util_WriteLog( u"만기일               : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"삭제일")
        self.Util_WriteLog( u"삭제일               : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"정산가")
        self.Util_WriteLog( u"정산가               : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"정산일")
        self.Util_WriteLog( u"정산일               : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"가격표시")
        self.Util_WriteLog( u"가격표시              : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"장시작시간(CME)")
        self.Util_WriteLog( u"장시작시간(CME)        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"장종료시간(CME)")
        self.Util_WriteLog( u"장종료시간(CME)        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"장시작시간(한국)")
        self.Util_WriteLog( u"장시작시간(한국)        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"장종료시간(한국)")
        self.Util_WriteLog( u"장종료시간(한국)        : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"거래통화")
        self.Util_WriteLog( u"거래통화              : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"상품구분")
        self.Util_WriteLog( u"상품구분              : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"매매여부")
        self.Util_WriteLog( u"매매여부              : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"잔존일수")
        self.Util_WriteLog( u"잔존일수              : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"ticksize")
        self.Util_WriteLog( u"ticksize           : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"tickvalue")
        self.Util_WriteLog( u"tickvalue          : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"최종결제일")
        self.Util_WriteLog( u"최종결제일             : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"증거금")
        self.Util_WriteLog( u"증거금               : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"계약단위")
        self.Util_WriteLog( u"계약단위              : " + strData)
        
        pass
    
    def procs20001(self, sTrCode):
        i = 0
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"단축코드")
        self.Util_WriteLog(strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"호가수신시간")
        self.Util_WriteLog(strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"현재가")
        self.Util_WriteLog(strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"누적거래량")
        self.Util_WriteLog(strData)
        pass
    
    def procAQ0101(self, sTrCode):
        strData = self.HD_CommGetAccInfo()
            
        nAcctCnt = int(strData[0:5])
        nLenAcctNo = 11
        nLenAcctNm = 30
        nLenAcctGb = 1
        
        strAcctInfo = str(strData[5:]).encode('cp949')
        nLenAccInfo = nLenAcctNo + nLenAcctNm + nLenAcctGb
        
        #strSelAccGb = "1" #1:해외, 2:FX, 9:국내
        #if ( self.BtnRadio1.isChecked() == True ):
        #    strSelAccGb = "9"   #국내
        #elif ( self.BtnRadio2.isChecked() == True ):
        #    strSelAccGb = "1"   #해외
        #elif ( self.BtnRadio3.isChecked() == True ):
        #    strSelAccGb = "2"   #FX
            
        for i in range(0, nAcctCnt):
            nStPos = (i*nLenAccInfo)
            strAcctNo = strAcctInfo[nStPos                        :nStPos+nLenAcctNo]
            strAcctNm = strAcctInfo[nStPos+(nLenAcctNo)           :nStPos+nLenAcctNo+nLenAcctNm]
            strAcctGb = strAcctInfo[nStPos+(nLenAcctNo+nLenAcctNm):nStPos+nLenAcctNo+nLenAcctNm+nLenAcctGb]
            strAcctNm = unicode(strAcctNm, 'cp949')
            
            strAcctNo = unicode(strAcctNo.strip(), 'utf-8')
            strAcctNm = str(strAcctNm.strip())
            
            tmpDic = {strAcctNo:strAcctNm}
            if ( strAcctGb == "9" ):
                self.m_AccListFO.update(tmpDic)
            elif(strAcctGb == "1" ):
                self.m_AccListFR.update(tmpDic)
            elif(strAcctGb == "2" ):
                self.m_AccListFX.update(tmpDic)

            self.Util_WriteLog(strAcctNo + "," + strAcctNm + "," + strAcctGb)
            
        self.OnRadioAcctTpChange()
        pass
    
    def procDQ0242(self, sTrCode):
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"계좌번호")
        self.Util_WriteLog( u"계좌번호 : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"예탁금액-총액")
        self.Util_WriteLog( u"예탁금액-총액 :" + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"예탁금액-현금")
        self.Util_WriteLog( u"예탁금액-현금 : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"예탁금액-대용")
        self.Util_WriteLog( u"예탁금액-대용 :" + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"예탁외화")
        self.Util_WriteLog( u"예탁외화 : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"당일손익")
        self.Util_WriteLog( u"당일손익 : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"위탁수수료")
        self.Util_WriteLog( u"위탁수수료 : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"당일순손익")
        self.Util_WriteLog( u"당일순손익 : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"평가예탁총액")
        self.Util_WriteLog( u"평가예탁총액 : " + strData)
        strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", 0, u"순자산-총평가액")
        self.Util_WriteLog( u"순자산-총평가액 : " + strData)
        pass
    
    def procAQ0450(self, sTrCode):
        nRptCnt = self.HD_CommGetRepeatCnt(sTrCode, -1, u"OutRec1")
        self.Util_WriteLog( u"AQ0450 조회개수 : " + str(nRptCnt) )
        for i in nRptCnt:
            self.Util_WriteLog( u"===============================================" )
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문번호")
            self.Util_WriteLog(u"주문번호      : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"계좌번호")
            self.Util_WriteLog(u"계좌번호      : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"계좌명")
            self.Util_WriteLog(u"계좌명       : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"종목코드")
            self.Util_WriteLog(u"종목코드      : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"매매구분")
            self.Util_WriteLog(u"매매구분      : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문가격")
            self.Util_WriteLog(u"주문가격      : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문수량")
            self.Util_WriteLog(u"주문수량      : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"체결수량")
            self.Util_WriteLog(u"체결수량      : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"잔량")
            self.Util_WriteLog(u"잔량         : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문유형")
            self.Util_WriteLog(u"주문유형      : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"STOP 가격")
            self.Util_WriteLog(u"STOP 가격    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문시간")
            self.Util_WriteLog(u"주문시간      : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문자 ")
            self.Util_WriteLog(u"주문자       : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"통신구분")
            self.Util_WriteLog(u"통신구분      : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"전략구분")
            self.Util_WriteLog(u"전략구분      : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"최초원주문번호")
            self.Util_WriteLog(u"최초원주문번호   : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문일자")
            self.Util_WriteLog(u"주문일자      : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"그룹주문번호")
            self.Util_WriteLog(u"그룹주문번호    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"가격조건")
            self.Util_WriteLog(u"가격조건      : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"체결조건")
            self.Util_WriteLog(u"체결조건      : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"GTD일자")
            self.Util_WriteLog(u"GTD일자      : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"IOC최소체결수량")
            self.Util_WriteLog(u"IOC최소체결수량 : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"원주문번호")
            self.Util_WriteLog(u"원주문번호     : " + strData)
        pass
    
    def procDQ0104(self, sTrCode):
        nRptCnt = self.HD_CommGetRepeatCnt(sTrCode, -1, u"OutRec1")
        self.Util_WriteLog( u"DQ0104 조회개수 : " + str(nRptCnt) )
        for i in nRptCnt:
            self.Util_WriteLog( u"===============================================" )
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"계좌번호")
            self.Util_WriteLog( u"계좌번호        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"계좌명")
            self.Util_WriteLog( u"계좌명         : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문번호")
            self.Util_WriteLog( u"주문번호        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"종목코드")
            self.Util_WriteLog( u"종목코드        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"매매구분")
            self.Util_WriteLog( u"매매구분        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문가격")
            self.Util_WriteLog( u"주문가격        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문수량")
            self.Util_WriteLog( u"주문수량        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"정정수량")
            self.Util_WriteLog( u"정정수량        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"취소수량")
            self.Util_WriteLog( u"취소수량        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"체결수량")
            self.Util_WriteLog( u"체결수량        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"잔량")
            self.Util_WriteLog( u"잔량          : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"원주문번호")
            self.Util_WriteLog( u"원주문번호       : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"최초원주문번호")
            self.Util_WriteLog( u"최초원주문번호    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"통신주문구분")
            self.Util_WriteLog( u"통신주문구분     : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문전략구분")
            self.Util_WriteLog( u"주문전략구분     : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문조작구분")
            self.Util_WriteLog( u"주문조작구분     : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"거래소접수시간")
            self.Util_WriteLog( u"거래소접수시간    : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"작업사원")
            self.Util_WriteLog( u"작업사원        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"그룹주문번호")
            self.Util_WriteLog( u"그룹주문번호     : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"딜러번호")
            self.Util_WriteLog( u"딜러번호        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"딜러명")
            self.Util_WriteLog( u"딜러명         : " + strData)
        pass
    
    def procDQ0107(self, sTrCode):
        nRptCnt = self.HD_CommGetRepeatCnt(sTrCode, -1, u"OutRec1")
        self.Util_WriteLog( u"DQ0107 조회개수 : " + str(nRptCnt) )
        for i in nRptCnt:
            self.Util_WriteLog( u"===============================================" )
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"계좌번호")
            self.Util_WriteLog( u"계좌번호        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"계좌명")
            self.Util_WriteLog( u"계좌명         : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"주문번호")
            self.Util_WriteLog( u"주문번호        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"종목코드")
            self.Util_WriteLog( u"종목코드        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"매매구분")
            self.Util_WriteLog( u"매매구분        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"체결가격")
            self.Util_WriteLog( u"체결가격        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"체결수량")
            self.Util_WriteLog( u"체결수량        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"체결시간")
            self.Util_WriteLog( u"체결시간        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"전략구분")
            self.Util_WriteLog( u"전략구분        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"통신구분")
            self.Util_WriteLog( u"통신구분        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"딜러번호")
            self.Util_WriteLog( u"딜러번호        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"딜러좌명")
            self.Util_WriteLog( u"딜러좌명        : " + strData)
        pass
    
    def procDQ0110(self, sTrCode):
        nRptCnt = self.HD_CommGetRepeatCnt(sTrCode, -1, u"OutRec1")
        self.Util_WriteLog( u"DQ0110 조회개수 : " + str(nRptCnt) )
        for i in nRptCnt:
            self.Util_WriteLog( u"===============================================" )
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"계좌번호")
            self.Util_WriteLog( u"계좌번호              : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"계좌명")
            self.Util_WriteLog( u"계좌명                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"종목")
            self.Util_WriteLog( u"종목                : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"매매구분")
            self.Util_WriteLog( u"매매구분              : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"전일미결제수량")
            self.Util_WriteLog( u"전일미결제수량          : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"당일미결제수량")
            self.Util_WriteLog( u"당일미결제수량          : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"미체결 수량")
            self.Util_WriteLog( u"미체결 수량            : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"평균단가")
            self.Util_WriteLog( u"평균단가              : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"장부단가")
            self.Util_WriteLog( u"장부단가              : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"당일미결제약정금액")
            self.Util_WriteLog( u"당일미결제약정금액        : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"당일체결수량")
            self.Util_WriteLog( u"당일체결수량           : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"당일체결금액")
            self.Util_WriteLog( u"당일체결금액           : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"행사신청수량")
            self.Util_WriteLog( u"행사신청수량           : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"행사배정수량")
            self.Util_WriteLog( u"행사배정수량           : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"행사거부수량")
            self.Util_WriteLog( u"행사거부수량           : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"만기수량")
            self.Util_WriteLog( u"만기수량              : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"딜러번호")
            self.Util_WriteLog( u"딜러번호              : " + strData)
            strData = self.HD_CommGetData(sTrCode, -1, u"OutRec1", i, u"딜러명")
            self.Util_WriteLog( u"딜러명               : " + strData)
        pass

    def OnGetMsgWithRqId(self, nRqID, strErrCode, strErrMsg):
        strRecv = "조회응답 Tr = [%s] UniqueID = [%d]" %(strErrCode, nRqID)
        strRecvMsg =  "메시지수신 UniqueID = [%s] ErrorCode=[%s] ErrorMessage=[%s]" % (str(nRqID), unicode(strErrCode), unicode(strErrMsg))
        self.Util_WriteLog( strRecvMsg )

if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    MainWindow = QtGui.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    ui.SetupHDFOcx(MainWindow)
    
    MainWindow.show()
    sys.exit(app.exec_())

