#pragma once

	enum  TCMDType { cmtIOCPEvent, cmtAccept, cmtDisconnect, cmtTR_CODE };
	enum  TR_CODE { ctDestAdd, ctDestDel, ctDestPushSend, ctDestRRSend, ctDestRRSendResponse, ctUserID, ctDisconnect, ctUserConnectOk, ctUsersAlready, ctClientUniqKey };
	enum  TDeliveryMode { Delivery_RR, Delivery_RR_Response, Delivery_Push };

	const int MESSAGE_TO_SEND = 0;
	const int RECEIVED_MESSAGE = 1;
	const int RESPOND_MESSAGE = 2;

	/*
		0 - success
		else - fail
	*/
	typedef long int(__stdcall *TInitialize)();

	typedef long int(__stdcall *TSMClientConnect)(int index, char* host, long int port);

	typedef long int(__stdcall *TSMClientDisconnect)(int index);

	typedef bool(__stdcall *TSMClientIsConnected)(int index);

	typedef long int(__stdcall *TSMSetMessageParameters)(int index, int messtype,
		TDeliveryMode DeliveryMode, char* Destination, char* Msg);

	typedef long int(__stdcall *TSMSetMessageBinaryField)(int index, int messtype,
		char* FieldName, char* Data, int DataSize);

	typedef long int(__stdcall *TSMSetMessageStringField)(int index, int messtype,
		char* FieldName, char* Str);

	typedef long int(__stdcall *TSMSetMessageIntegerField)(int index, int messtype,
		char* FieldName, int Val);

	typedef long int(__stdcall *TSMSendMessage)(int index, int messtype);

	typedef char* (__stdcall *TSMGetClientUniqKey)(int index);

	typedef char* (__stdcall *TSMGetClientIP)(int index);

	typedef long int(__stdcall *TSMEventAddDestination)(int index, char* Dest, char* Msg);

	typedef long int(__stdcall *TSMEventRemoveDestination)(int index, char* Dest, char* Msg);

	typedef long int(__stdcall *TSMEventAllRemoveDestination)(int index);

	typedef long int(__stdcall *TSMGetReceivedCnt)(int index);

	typedef long int(__stdcall *TSMSetWorkEventCallBack)(int index, char* CallBackProc);

	typedef char* (__stdcall *TSMMessageGetBinaryFieldValue)(int index, int messtype, char* FieldName);

	typedef char* (__stdcall *TSMMessageGetStringFieldValue)(int index, int messtype, char* FieldName);

	typedef int(__stdcall *TSMMessageGetIntegerFieldValue)(int index, int messtype, char* FieldName);

	typedef long int(__stdcall *TSMGetObjectsNumber)();

	typedef long int(__stdcall *TSMGetMaximumObjectsNumber)();

	typedef long int(__stdcall *TSMCreateInstance)();

	typedef int(__stdcall *TSMMessageGetDeliveryType)(int index, int messtype);

	typedef int(__stdcall *TSMSendResponse)(int index, int messtype);

	typedef int(__stdcall *TSMGetDeliveryModeOfRecvMsg)(int index);

	typedef char*(__stdcall *TSMGetDestinationOfRecvMsg)(int index);

	typedef char*(__stdcall *TSMGetMsgOfRecvMsg)(int index);

	typedef int(__stdcall *TSMGetClientSessionOfRecvMsg)(int index);

	typedef int(__stdcall *TSMSMessageToSMessageEx)(int index);

	typedef int(__stdcall *TSetSMMessageHeader)(int index, int DeliveryMode, char* pDest, char* pMsg, int ClientSession);


	//function SMSetMessageParameters(DeliveryMode : TDeliveryMode; Destination : PChar; Msg : PChar) : integer;
	//function SMSetMessageField(FieldName : PChar; Data : Pointer; DataSize : integer) : integer; stdcall;


	// CLENT  [] => SERVER ( Request & Reply )
	const char   SISE_PROC[] = "\\SISE\\SVR\\";
	const char   ORDER_PROC[] = "\\ORDER\\SVR\\";
	const char   SEARCH_PROC[] = "\\SEARCH\\SVR\\";
	const char   BANKING_PROC[] = "\\BANKING\\SVR\\";
	const char   ACCOUNT_PROC[] = "\\ACCOUNT\\SVR\\";
	const char   MANAGER_PROC[] = "\\MANAGER\\SVR\\";
	const char   LOGIN_PROC[] = "\\LOGIN\\SVR\\";
	const char   BATCH_PROC[] = "\\BATCH\\SVR\\";
	const char   SYS_ORD_PROC[] = "\\SYS\\ORD\\SVR\\";

	// GW
	const char   ORDER_GW[] = "\\ORDER\\GW\\";
	const char   *SISE_GW = "\\SISE\\GW\\";
	const char   EXEC_GW[] = "\\EXEC\\GW\\";

	// REAL GW
	const char   ORD_REAL_GW[] = "\\ORD\\REAL\\GW\\";
	const char   EXEC_REAL_GW[] = "\\EXEC\\REAL\\GW\\";

	// RCV
	const char   ORDER_RCV[] = "\\ORDER\\RESPONSE\\";
	const char   ORD_REAL_RCV[] = "\\ORD\\REAL\\RESPONSE\\";

	// EXCHANGE
	const char   EXCH_PROC[] = "\\EXCHANGE\\SVR\\";

	// EXEC
	const char   EXEC_PROC[] = "\\EXEC\\SVR\\";

	// SERVER [] => CLIENT ( Push )
	const char   SISE_REQ[] = "\\SISE\\CLI\\";
	const char   ORDER_SEND[] = "\\ORDER\\CLI\\";
	const char   MANAGER_SEND[] = "\\MANAGER\\CLI\\";
	const char   ALL_CLIENT[] = "\\ALL\\CLIENT\\";

	// Server [] => Server ( Push )
	const char   LOSSCUT_PROC[] = "\\LOSSCUT\\SVR\\";
	const char   HEDGE_PROC[] = "\\HEDGE\\SVR\\";

	// ALL SERVER
	const char   ALL_SERVER[] = "\\ALL\\SERVER\\";

	// SIGNAL & STRAT
	const char   SIG_REALY[] = "\\SIG\\RELAY\\";



	const char fldHDUserID[] = "H001";
	const char fldHDWndHandle[] = "H002";
	const char fldHDIPAddr[] = "H003";
	const char fldHDErrFlag[] = "H004";
	const char fldHDMsgCode[] = "H005";
	const char fldHDMsg[] = "H006";
	const char fldHDMacAddr[] = "H007";


	const char fldFutMast[] = "S001";
	const char fldFutHoga[] = "S002";
	const char fldFutExec[] = "S003";
	const char fldOptMast[] = "S004";
	const char fldOptHoga[] = "S005";
	const char fldOptExec[] = "S006";
	const char fldFXMast[] = "S007";
	const char fldFXHoga[] = "S008";
	const char fldFXExec[] = "S009";


	const char fldSPOTMast[] = "S010";
	const char fldSPOTHoga[] = "S011";
	const char fldSPOTExec[] = "S012";


	const char fldEurexMast[] = "S013";
	const char fldEurexHoga[] = "S014";
	const char fldEurexExec[] = "S015";

	// Binary Field
	const char fldBinOrder[] = "B001";  // TTradeOrder
	const char fldBinOrigOrder[] = "B002";  // TTradeOrder
	const char fldBinOpenInterest[] = "B003";  // TTradeOpenInterest
	const char fldBinBalance[] = "B004";  // TTradeBalance

	const char fldBinTotOrder[] = "B005";  // TTradeTotOrder
	const char fldBinTotOI[] = "B006";  // TTradeTotOpenInterest
	const char fldBinTotBalance[] = "B007";  // TTradeTotBalance
	const char fldBinTotSysOrder[] = "B008";  // TTradeTotOrder
	const char fldBinTotSysOI[] = "B009";  // TTradeTotOpenInterest
	const char fldBinUser[] = "B010";  // TTradeUser

									   // Field
	const char fldSymbol[] = "U001";
	const char fldAccountNo[] = "U002";
	const char fldOrderQty[] = "U003";
	const char fldPrice[] = "U004";
	const char fldSide[] = "U005";
	const char fldOrdType[] = "U006";
	const char fldHostOrdNo[] = "U007";
	const char fldOrigHostOrdNo[] = "U008";
	const char fldExecQty[] = "U009";
	const char fldExecPrice[] = "U010";
	const char fldOrderID[] = "U011";
	const char fldOrigOrderID[] = "U012";
	const char fldSpeedOrdFlag[] = "U013";
	const char fldPendingStatus[] = "U014";
	const char fldSystemOrdFlag[] = "U015";
	const char fldBankingType[] = "U016";
	const char fldComments[] = "U017";
	const char fldDepositReqMoney[] = "U018";
	const char fldDepositMoney[] = "U019";
	const char fldWithDrawReqMoney[] = "U020";
	const char fldWithDrawMoney[] = "U021";
	const char fldTransferFee[] = "U022";
	const char fldBankName[] = "U023";
	const char fldBankAccountNo[] = "U024";
	const char fldBankAccountHolder[] = "U025";
	const char fldBankingProcFlag[] = "U026";
	const char fldDepositHolder[] = "U027";
	const char fldReqSeq[] = "U028";
	const char fldReqType[] = "U029";
	const char fldUserStatus[] = "U030";
	const char fldLicenseType[] = "U031";
	const char fldLoanCount[] = "U032";
	const char fldReqProcFlag[] = "U033";
	const char fldUserID[] = "U034";
	const char fldUserName[] = "U035";
	const char fldPassword[] = "U036";
	const char fldEMail[] = "U037";
	const char fldPhoneNo[] = "U038";
	const char fldMobilePhoneNo[] = "U039";
	const char fldUserAddress[] = "U040";
	const char fldFutCommissionRate[] = "U041";
	const char fldOptCommissionRate[] = "U042";
	const char fldLicenseFee[] = "U043";
	const char fldLicenseImposeType[] = "U044";
	const char fldRoleID[] = "U045";
	const char fldLicenseMoney[] = "U046";
	const char fldUseDays[] = "U047";
	const char fldTotRec[] = "U048";
	const char fldTotRec2[] = "U049";
	const char fldNoticeSeq[] = "U050";
	const char fldNoticeSubject[] = "U051";
	const char fldNoticeText[] = "U052";
	const char fldTradeDate[] = "U053";
	const char fldRecStatus[] = "U054";
	const char fldMoney[] = "U055";
	const char fldCmplSeq[] = "U056";
	const char fldCmplType[] = "U057";
	const char fldBankingSeq[] = "U058";
	const char fldUpdateTime[] = "U059";
	const char fldTotRec3[] = "U060";
	const char fldTotRec4[] = "U061";
	const char fldExpireFlag[] = "U062";
	const char fldExecMsgType[] = "U063";
	const char fldBalanceMoney[] = "U064";
	const char fldClearCondition[] = "U065";
	const char fldTradeLimitFlag[] = "U066";
	const char fldEvalMoney[] = "U067";
	const char fldLossCutFlag[] = "U068";
	const char fldHostExecID[] = "U069";
	const char fldExecutionID[] = "U070";
	const char fldNewUserFlag[] = "U071";

	const char fldHedgeAccountNo[] = "U072";
	const char fldFutFlag[] = "U073";
	const char fldOptFlag[] = "U074";
	const char fldHedgeAccountFlag[] = "U076";
	const char fldNewOrdType[] = "U077";
	const char fldNewOrdTick[] = "U078";
	const char fldClearOrdType[] = "U079";
	const char fldClearOrdTick[] = "U080";
	const char fldErrorMsg[] = "U081";
	const char fldLeverageType[] = "U082";
	const char fldSMSRcvFlag[] = "U083";
	const char fldEMailRcvFlag[] = "U084";
	const char fldServerIP[] = "U085";
	const char fldServerPort[] = "U086";
	const char fldMarketServerIP[] = "U087";
	const char fldMarketServerPort[] = "U088";
	const char fldDBServerIP[] = "U089";
	const char fldDbServerPort[] = "U090";
	const char fldMileage[] = "U091";
	const char fldSwitchMileage[] = "U092";
	const char fldFxBaseCommission[] = "U093";

	const char fldFutOpenTime[] = "U094";
	const char fldFutCloseTime[] = "U095";
	const char fldFutExpOpenTime[] = "U096";
	const char fldFutExpCloseTime[] = "U097";
	const char fldFutOrdCloseFlag[] = "U098";
	const char fldFutConfigMsg[] = "U099";
	const char fldFxOpenTime[] = "U100";
	const char fldFxCloseTime[] = "U101";
	const char fldFxExpOpenTime[] = "U102";
	const char fldFxExpCloseTime[] = "U103";
	const char fldFxOrdCloseFlag[] = "U104";
	const char fldFxConfigMsg[] = "U105";
	const char fldRunBackupOrder[] = "U106";
	const char fldRunUploadSecurity[] = "U107";
	const char fldRunEventDeposit[] = "U108";
	const char fldBeableQty[] = "U109";
	const char fldBeableOptQty[] = "U301";
	const char fldBeableFutQty[] = "U302";
	const char fldBeableFxQty[] = "U303";
	const char fldProduct[] = "U110";
	const char fldAuthorityKey[] = "U111";
	const char fldForciblyOrdFlag[] = "U112";
	const char fldOpenPriceFlag[] = "U113";
	const char fldFXCommissionType[] = "U114";
	const char fldAlreadyLoginFlag[] = "U115";
	const char fldUserIdentity[] = "U116";
	const char fldUSDExRate[] = "U117";
	const char fldPopupFlag[] = "U118";
	const char fldCmeCommissionRate[] = "U119";
	const char fldOrderPath[] = "U120";
	const char fldCustNo[] = "U121";
	const char fldMaxBeableQty[] = "U122";
	const char fldTradeProduct[] = "U123";
	const char fldOrdCondType[] = "U124";
	const char fldSecurityKey[] = "U125";
	const char fldOrigMsgType[] = "U126";
	const char fldRealHostOrdNo[] = "U127";
	const char fldFutGrade[] = "U128";
	const char fldFXGrade[] = "U129";
	const char fldFutUseFlag[] = "U130";
	const char fldOptUseFlag[] = "U131";
	const char fldFXUseFlag[] = "U132";
	const char fldHighPrice[] = "U133";
	const char fldLowPrice[] = "U134";
	const char fldLossCutMoney[] = "U135";
	const char fldGoodsType[] = "U136";
	const char fldCloseOrdFlag[] = "U137";
	const char fldRealOrdFlag[] = "U138";
	const char fldAutoCxlFlag[] = "U139";
	const char fldOrdHandleType[] = "U140";
	const char fldFXVIPOrdFlag[] = "U141";
	const char fldOverNightOrdFlag[] = "U142";
	const char fldOverNightAccountNo[] = "U143";
	const char fldRecommenderName[] = "U144";
	const char fldCMEOpenTime[] = "U145";
	const char fldCMECloseTime[] = "U146";
	const char fldCMEOrdCloseFlag[] = "U147";
	const char fldSTOPSetType[] = "U148";
	const char fldSTOPBasePrice[] = "U149";
	const char fldSTOPProfitUseFlag[] = "U150";
	const char fldSTOPProfitTick[] = "U151";
	const char fldSTOPLossUseFlag[] = "U152";
	const char fldSTOPLossTick[] = "U153";
	const char fldMITPlcType[] = "U154";
	const char fldMITPrice[] = "U155";
	const char fldMITOrigPrice[] = "U156";
	const char fldStrikePrice[] = "U157";
	const char fldMktSvrType[] = "U158";
	const char fldFXOptGrade[] = "U159";

	const char fldEurexOpenTime[] = "U160";
	const char fldEurexCloseTime[] = "U161";
	const char fldEurexExpOpenTime[] = "U162";
	const char fldEurexExpCloseTime[] = "U163";
	const char fldEurexOrdCloseFlag[] = "U164";
	const char fldEurexConfigMsg[] = "U165";

	const char fldHKDExRate[] = "U166";
	const char fvBreakTimeFlag[] = "U167";



	// HEADER PART
	const char fldHStratCode[] = "S001";
	const char fldHStratUserID[] = "S002";
	const char fldHStratCompID[] = "S003";
	const char fldHStratTp[] = "S004";
	const char fldHStratSymbol[] = "S005";
	const char fldHStratGroupKey[] = "S006";
	const char fldHStratApiYN[] = "S007";
	const char fldHStratTM[] = "S008";
	const char fldHStratReserve[] = "S009";

	const char fldStratID[] = "S010";
	const char fldStratChartTp[] = "S011";
	const char fldStratChartNm[] = "S012";
	const char fldStratPrc[] = "S013";
	const char fldStratChartGb[] = "S014";
	const char fldStratArrowType[] = "S015";

	const char fldStratMatchedChart[] = "S016";
	const char fldStratClrYN[] = "S017";
	const char fldStratCrossTp[] = "S018";
	const char fldStratClrAssistPrc[] = "S019";
	const char fldStratNowPrc[] = "S020";

	const char fldStratBreakTp[] = "S021";
	const char fldStratMAPrice[] = "S022";

	const char fldStratSide[] = "S023";
	const char fldStratOrdTp[] = "S024";
	const char fldStratProcTp[] = "S025";
	const char fldStratOrdPrc[] = "S026";
	const char fldStratOrdQty[] = "S027";
	const char fldStratCnclAllYN[] = "S028";
	const char fldStratOrigNo[] = "S029";
	const char fldStratOrigPrc[] = "S030";
	const char fldStratSLTick[] = "S031";
	const char fldStratPTTick[] = "S032";


	const char  MSG_ORD_FUT_NEW[] = "\\FUT\\NEW\\";
	const char  MSG_ORD_FUT_REP[] = "\\FUT\\REP\\";
	const char  MSG_ORD_FUT_CXL[] = "\\FUT\\CXL\\";
	const char  MSG_ORD_OPT_NEW[] = "\\OPT\\NEW\\";
	const char  MSG_ORD_OPT_REP[] = "\\OPT\\REP\\";
	const char  MSG_ORD_OPT_CXL[] = "\\OPT\\CXL\\";
	const char  MSG_ORD_FX_NEW[] = "\\FX\\NEW\\";
	const char  MSG_ORD_FX_REP[] = "\\FX\\REP\\";
	const char  MSG_ORD_FX_CXL[] = "\\FX\\CXL\\";
	const char  MSG_ORD_CME_NEW[] = "\\CME\\NEW\\";
	const char  MSG_ORD_CME_REP[] = "\\CME\\REP\\";
	const char  MSG_ORD_CME_CXL[] = "\\CME\\CXL\\";


	const char  MSG_ORD_All_CLEAR[] = "\\ORD\\ALL\\CLEAR\\";
	const char  MSG_ORD_ALL_CXL[] = "\\ORD\\ALL\\CXL\\";
	const char  MSG_ORD_ALL_REP[] = "\\ORD\\ALL\\REP\\";
	const char  MSG_ORD_HEDGE_ALL_CLEAR[] = "\\ORD\\HEDGE_ALL\\CLEAR\\";


	const char  MSG_ORD_SYMBOL_CLEAR[] = "\\ORD\\SYMBOL\\CLEAR\\";
	const char  MSG_ORD_SYMBOL_CXL[] = "\\ORD\\SYMBOL\\CXL\\";


	const char  MSG_ORD_HSI_BREAK[] = "\\ORD\\HSI\\BREAK\\";


	const char  MSG_ORD_SYS_CXL[] = "\\ORD\\SYS\\CXL\\";
	const char  MSG_ORD_SYS_CLEAR[] = "\\ORD\\SYS\\CLEAR\\";
	const char  MSG_ORD_SYS_CLOSE[] = "\\ORD\\SYS\\CLOSE\\";
	const char  MSG_ORD_SYS_HTS[] = "\\ORD\\SYS\\HTS\\";
	const char  MSG_ORD_SYS_CME_CLOSE[] = "\\ORD\\SYS\\CME\\CLOSE\\";
	const char  MSG_ORD_SYS_EXPIRE[] = "\\ORD\\SYS\\EXPIRE\\";


	const char  MSG_ORD_LOSSCUT[] = "\\ORD\\SYS\\LOSSCUT\\";
	const char  MSG_MGR_LOSSCUT[] = "\\ORD\\MGR\\LOSSCUT\\";

	const char  MSG_RCV_LOSSCUT[] = "\\RCV\\SYS\\LOSSCUT\\";

	const char  MSGP_ORD_NEW[] = "\\SEND\\ORD\\NEW\\";
	const char  MSGP_ORD_REP[] = "\\SEND\\ORD\\REP\\";
	const char  MSGP_ORD_CXL[] = "\\SEND\\ORD\\CXL\\";

	const char  MSGR_GW_EXCH[] = "\\R\\GW\\EXCH\\";

	const char  MSG_EXCH_RCV[] = "\\EXCH\\RCV\\";

	const char  MSG_EXEC[] = "\\EXEC\\";
	const char  MSG_EXCH_REP[] = "\\EXCH\\REP\\";
	const char  MSG_EXCH_CXL[] = "\\EXCH\\CXL\\";
	const char  MSG_EXCH_REJ[] = "\\EXCH\\REJ\\";

	const char  MSGR_REQ_DEPOSIT[] = "\\RCV\\REQ\\DEPOSIT\\";
	const char  MSGR_REQ_WITHDRAW[] = "\\RCV\\REQ\\WITHDRAW\\";
	const char  MSGR_CHG_ACCSTATUS[] = "\\RCV\\CHG\\ACCSTATUS\\";
	const char  MSGR_CHG_LICENSE[] = "\\RCV\\CHG\\LICENSE\\";
	const char  MSGR_CHG_LOAN_CNT[] = "\\RCV\\CHG\\LOANCNT\\";
	const char  MSGR_REQ_BEABLE_QTY[] = "\\RCV\\REQ\\BEABLE_QTY\\";
	const char  MSGR_REQ_OVERNIGHT[] = "\\RCV\\REQ\\OVERNIGHT\\";
	const char  MSGR_REQ_ONCLOSE[] = "\\RCV\\REQ\\ONCLOSE\\";
	const char  MSGR_REQ_CMEUSE[] = "\\RCV\\REQ\\CMEUSE\\";
	const char  MSGR_REQ_EXPIRECALC[] = "\\RCV\\REQ\\EXPIRECALC\\";
	const char  MSGR_SET_ORDCOND[] = "\\RCV\\SET\\ORDCOND\\";
	const char  MSGR_RESET_ORDCOND[] = "\\RCV\\RESET\\ORDCOND\\";
	const char  MSGP_CHG_ORDCOND[] = "\\SEND\\CHG\\ORDCOND\\";

	const char  MSGP_REQ_DEPOSIT[] = "\\SEND\\REQ\\DEPOSIT\\";
	const char  MSGP_REQ_WITHDRAW[] = "\\SEND\\REQ\\WITHDRAW\\";
	const char  MSGP_REQ_OVERNIGHT[] = "\\SEND\\REQ\\OVERNIGHT\\";
	const char  MSGP_CHG_ACCSTATUS[] = "\\SEND\\CHG\\ACCSTATUS\\";
	const char  MSGP_CHG_LICENSE[] = "\\SEND\\CHG\\LICENSE\\";
	const char  MSGP_CHG_LOAN_CNT[] = "\\SEND\\CHG\\LOANCNT\\";
	const char  MSGR_CHG_LEVERAGE[] = "\\SEND\\CHG\\LEVERAGE\\";
	const char  MSGR_CHG_USERINFO[] = "\\SEND\\CHG\\USERINFO\\";
	const char  MSGR_SWITCH_MILEAGE[] = "\\SEND\\SWITCH\\MILEAGE\\";
	const char  MSGP_REQ_BEABLE_QTY[] = "\\SEND\\REQ\\BEABLE_QTY\\";

	const char  MSGR_DEPOSIT[] = "\\MGR\\DEPOSIT\\";
	const char  MSGR_WITHDRAW[] = "\\MGR\\WITHDRAW\\";
	const char  MSGR_EVENT_DEPOSIT[] = "\\MGR\\EVENT\\DEPOSIT\\";
	const char  MSGR_MILEAGE_DEPOSIT[] = "\\MGR\\MILEAGE\\DEPOSIT\\";
	const char  MSGR_MILEAGE_WITHDRAW[] = "\\MGR\\MILEAGE\\WITHDRAW\\";

	const char  MSGR_LICENSE_WITHDRAW[] = "\\MGR\\LICENSE\\WITHDRAW\\";
	const char  MSGR_LOAN_DEPOSIT[] = "\\MGR\\LOAN\\DEPOSIT\\";
	const char  MSGR_LOAN_WITHDRAW[] = "\\MGR\\LOAN\\WITHDRAW\\";
	const char  MSGR_LICENSE_IMPOSE[] = "\\MGR\\LICENSE\\IMPOSE\\";
	const char  MSGR_ACCSTATUS[] = "\\MGR\\ACCSTATUS\\";
	const char  MSGR_INCREASE_DAYS[] = "\\MGR\\INCREASE\\DAYS\\";
	const char  MSGR_LICENSE_CHARGE[] = "\\MGR\\LICENSE\\CHARGE\\";
	const char  MSGR_BEABLE_QTY[] = "\\MGR\\BEABLE_QTY\\";


	const char  MSGP_LOSSCUT_MONEY[] = "\\SEND\\LOSSCUT\\MONEY\\";
	const char  MSGP_LOSSCUT_SET_MONEY[] = "\\SEND\\LOSSCUT\\SET_MONEY\\";
	const char  MSGP_LOSSCUT_RELOAD[] = "\\SEND\\LOSSCUT\\RELOAD\\";
	const char  MSGP_BEABLE_QTY[] = "\\SEND\\BEABLE_QTY\\";

	const char  MSGR_WORK_COMPLETE[] = "\\MGR\\WORK\\COMPLETE\\";    // Manager Complete

	const char  MSGR_USER_BANK_ACCOUNT[] = "\\REG\\USER_BANK_ACCCOUNT\\";
	const char  MSGR_BANK_ACCOUNT[] = "\\REG\\BANK_ACCCOUNT\\";

	const char  MSGR_TOT_BALANCE[] = "\\SCH\\TOT\\BALANCE\\";

	const char  MSGP_TOT_BALANCE[] = "\\TOT\\BALANCE\\";
	const char  MSGP_TOT_OPENINT[] = "\\TOT\\OPENINT\\";
	const char  MSGP_TOT_ORDER[] = "\\TOT\\ORDER\\";

	const char  MSGR_USER_REG[] = "\\MGR\\REG\\USER\\";
	const char  MSGR_USER_DEL[] = "\\MGR\\DEL\\USER\\";
	const char  MSGR_USER_RESET[] = "\\MGR\\RESET\\USER\\";
	const char  MSGR_NOTICE_REG[] = "\\MGR\\REG\\NOTICE\\";
	const char  MSGR_NOTICE_CHG[] = "\\MGR\\CHG\\NOTICE\\";
	const char  MSGR_NOTICE_DEL[] = "\\MGR\\DEL\\NOTICE\\";

	const char  MSGP_USER_REG[] = "\\SEND\\REG\\USER\\";
	const char  MSGP_USER_DEL[] = "\\SEND\\DEL\\USER\\";

	const char  MSGR_CONFIRM_MEMBER[] = "\\MGR\\CONFIRM\\MEMBER\\";
	const char  MSGR_REJECT_MEMBER[] = "\\MGR\\REJECT\\MEMBER\\";

	const char  MSGR_CHG_FUT_CONFIG[] = "\\MGR\\CHG\\FUT\\CONFIG\\";
	const char  MSGR_CHG_EUREX_CONFIG[] = "\\MGR\\CHG\\EUREX\\CONFIG\\";
	const char  MSGR_CHG_FX_CONFIG[] = "\\MGR\\CHG\\FX\\CONFIG\\";
	const char  MSGR_CHG_CME_CONFIG[] = "\\MGR\\CHG\\CME\\CONFIG\\";
	const char  MSGR_CLOSE_FUT[] = "\\MGR\\CLOSE\\FUT\\";
	const char  MSGR_CLOSE_FX[] = "\\MGR\\CLOSE\\FX\\";
	const char  MSGR_CLOSE_CME[] = "\\MGR\\CLOSE\\CME\\";

	const char  MSGR_ORDER_DEL[] = "\\MGR\\DEL\\ORDER\\";
	const char  MSGP_ORDER_DEL[] = "\\SEND\\DEL\\ORDER\\";
	const char  MSGR_MANUAL_EXEC[] = "\\MGR\\MANUAL\\EXEC\\";
	const char  MSGP_MANUAL_EXEC[] = "\\SEND\\MANUAL\\EXEC\\";

	const char  MSGP_NOTICE[] = "\\SEND\\NOTICE\\";
	const char  MSGP_WARNING[] = "\\SEND\\WARNING\\";
	const char  MSGP_MESSAGE[] = "\\SEND\\MESSAGE\\";

	const char  MSGP_CHG_MONEY[] = "\\SEND\\CHG\\MONEY\\";

	// SERVER RESTART
	const char  MSGP_RESTART[] = "\\SERVER\\RESTART\\"; // SERVER RESTART

														// Hedge
	const char  MSGR_HEDGE_USER_REG[] = "\\MGR\\REG\\HEDGE_USER\\";
	const char  MSGR_SET_HEDGE[] = "\\MGR\\SET\\HEDGE\\";
	const char  MSGP_SET_HEDGE[] = "\\SEND\\SET\\HEDGE\\";

	// MARKET PRICE
	const char  MSG_SISE_FUT_MAST[] = "\\FUT\\MAST\\";
	const char  MSG_SISE_FUT_HOGA[] = "\\FUT\\HOGA\\";
	const char  MSG_SISE_FUT_EXEC[] = "\\FUT\\EXEC\\";
	const char  MSG_SISE_OPT_MAST[] = "\\OPT\\MAST\\";
	const char  MSG_SISE_OPT_HOGA[] = "\\OPT\\HOGA\\";
	const char  MSG_SISE_OPT_EXEC[] = "\\OPT\\EXEC\\";
	const char  MSG_SISE_FX_MAST[] = "\\FX\\MAST\\";
	const char  MSG_SISE_FX_HOGA[] = "\\FX\\HOGA\\";
	const char  MSG_SISE_FX_EXEC[] = "\\FX\\EXEC\\";
	const char  MSG_SISE_CME_MAST[] = "\\CME\\MAST\\";
	const char  MSG_SISE_CME_HOGA[] = "\\CME\\HOGA\\";
	const char  MSG_SISE_CME_EXEC[] = "\\CME\\EXEC\\";

	const char  MSG_SISE_SPOT_MAST[] = "\\SPOT\\MAST\\";
	const char  MSG_SISE_SPOT_HOGA[] = "\\SPOT\\HOGA\\";
	const char  MSG_SISE_SPOT_EXEC[] = "\\SPOT\\EXEC\\";

	const char  MSG_SISE_EUREX_MAST[] = "\\EUREX\\MAST\\";
	const char  MSG_SISE_EUREX_HOGA[] = "\\EUREX\\HOGA\\";
	const char  MSG_SISE_EUREX_EXEC[] = "\\EUREX\\EXEC\\";

	// MARKET PRICE
	const char  MSG_MKT_KOSPI200[] = "\\MKT\\KOSPI200\\EXEC\\";
	const char  MSG_MKT_FUT_HOGA[] = "\\MKT\\FUT\\HOGA\\";
	const char  MSG_MKT_FUT_EXEC[] = "\\MKT\\FUT\\EXEC\\";
	const char  MSG_MKT_OPT_HOGA[] = "\\MKT\\OPT\\HOGA\\";
	const char  MSG_MKT_OPT_EXEC[] = "\\MKT\\OPT\\EXEC\\";
	const char  MSG_MKT_FX_HOGA[] = "\\MKT\\FX\\HOGA\\";
	const char  *MSG_MKT_FX_EXEC = "\\MKT\\FX\\EXEC\\";
	const char  MSG_MKT_CME_HOGA[] = "\\MKT\\CME\\HOGA\\";
	const char  MSG_MKT_CME_EXEC[] = "\\MKT\\CME\\EXEC\\";
	const char  MSG_MKT_CME_MAST[] = "\\MKT\\CME\\MAST\\";

	const char  MSG_MKT_SPOT_HOGA[] = "\\MKT\\SPOT\\HOGA\\";
	const char  MSG_MKT_SPOT_EXEC[] = "\\MKT\\SPOT\\EXEC\\";

	const char  MSG_MKT_EUREX_HOGA[] = "\\MKT\\EUREX\\HOGA\\";
	const char  MSG_MKT_EUREX_EXEC[] = "\\MKT\\EUREX\\EXEC\\";
	const char  MSG_MKT_EUREX_MAST[] = "\\MKT\\EUREX\\MAST\\";

	// Order GW

	const char  MSGR_GW_ORD_NEW[] = "\\R\\GW\\ORD\\NEW\\";
	const char  MSGR_GW_ORD_REP[] = "\\R\\GW\\ORD\\REP\\";
	const char  MSGR_GW_ORD_CXL[] = "\\R\\GW\\ORD\\CXL\\";

	/*
	// Order REAL GW

	MSGR_GW_ORD_NEW    [] = "\\R\\REAL\\GW\\FUT\\NEW\\";
	MSGR_GW_ORD_REP    [] = "\\R\\REAL\\GW\\FUT\\REP\\";
	MSGR_GW_ORD_CXL    [] = "\\R\\REAL\\GW\\FUT\\CXL\\";
	*/

	// Order Rcv
	const char  MSGR_RCV_ORD_NEW[] = "\\R\\RCV\\ORD\\NEW\\";
	const char  MSGR_RCV_ORD_REP[] = "\\R\\RCV\\ORD\\REP\\";
	const char  MSGR_RCV_ORD_CXL[] = "\\R\\RCV\\ORD\\CXL\\";
	const char  MSGR_RCV_ORD_REJ[] = "\\R\\RCV\\ORD\\REJ\\";

	//MSGR_LOGIN
	const char  MSGR_LOGIN[] = "\\R\\RCV\\LOGIN\\";
	const char  MSGR_LOGOUT[] = "\\R\\RCV\\LOGOUT\\";
	const char  MSGP_LOGOUT[] = "\\P\\SEND\\LOGOUT\\";
	const char  MSGR_KILL_USER[] = "\\R\\KILL\\USER\\";
	const char  MSGP_KILL_USER[] = "\\P\\KILL\\USER\\";

	//MSGR_RUN_DAILY
	const char  MSGR_RUN_DAILY[] = "\\R\\RCV\\RUN\\DAILY\\";
	//MSGR_RUN_SECURITY
	const char  MSGR_RUN_SECURITY[] = "\\R\\RCV\\RUN\\SECURITY\\";

	//Change Order Close Time
	const char  MSGP_SYS_ORD_TIME[] = "\\P\\SYS\\ORD\\TIME\\";


	const char  MSGR_SET_BEABLE_QTY[] = "\\R\\RCV\\SET\\BEABLE_QTY\\";


	const char  MSGR_SET_USD_EX_RATE[] = "\\R\\RCV\\SET\\USD_EX_RATE\\";
	const char  MSGP_SET_USD_EX_RATE[] = "\\P\\SEND\\USD_EX_RATE\\";


	const char  MSGR_SET_USER_GRADE[] = "\\R\\RCV\\SET\\USER_GRADE\\";
	const char  MSGP_SET_USER_GRADE[] = "\\P\\SEND\\SET\\USER_GRADE\\";


	const char  MSGR_SET_ENVIRONMENT[] = "\\R\\RCV\\SET\\USER_ENV\\";


	const char  MSGR_SET_FX_VIP_ORD[] = "\\R\\RCV\\SET\\FX_VIP_ORD\\";


	const char  MSGP_RELOAD_OI[] = "\\P\\SEND\\RELOAD\\OI\\";


	const char  MSGR_STOP_ORDER[] = "\\R\\RCV\\STOP\\ORDER\\";

	const char  MSGR_MIT_ORDER[] = "\\R\\RCV\\MIT\\ORDER\\";

	const char  MSGR_MANAGER_ORD_REJ[] = "\\R\\RCV\\MANAGER\\ORD\\REJ\\";

	const char  MSGR_OPT_EXPIRE[] = "\\R\\RCV\\OPT\\EXPIRE\\";

	const char  MSGR_EUREX_EXPIRE[] = "\\R\\RCV\\EUREX\\EXPIRE\\";

	const char  MSGR_SCH_MKTSERVER[] = "\\R\\RCV\\SCH\\MKTSERVER\\";
	const char  MSGR_CHG_MKTSERVER[] = "\\R\\RCV\\CHG\\MKTSERVER\\";

	const char  KR_REQ_5611[] = "\\KRSEND\\REQ5611\\";

	const char  fvSide_SELL[] = "1";
	const char  fvSide_BUY[] = "2";
	

