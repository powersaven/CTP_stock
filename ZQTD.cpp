// ZQTD.cpp: implementation of the CZQTD class.
//
//////////////////////////////////////////////////////////////////////
#include "util.h"
#include "ZQTD.h"
#include <iostream>
using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZQTD::CZQTD()
{

}

CZQTD::~CZQTD()
{

}

void CZQTD::Init()
{

	// 产生一个CThostFtdcTraderApi实例
	m_pTdApi = CZQThostFtdcTraderApi::CreateFtdcTraderApi("");

	// 注册一事件处理的实例
	m_pTdApi->RegisterSpi(this);
	
	// 订阅私有流
	//        TERT_RESTART:从本交易日开始重传
	//        TERT_RESUME:从上次收到的续传
	//        TERT_QUICK:只传送登录后私有流的内容
	m_pTdApi->SubscribePrivateTopic(ZQTHOST_TERT_RESUME);
	
	// 订阅公共流
	//        TERT_RESTART:从本交易日开始重传
	//        TERT_RESUME:从上次收到的续传
	//        TERT_QUICK:只传送登录后公共流的内容
	m_pTdApi->SubscribePublicTopic(ZQTHOST_TERT_RESUME);

	// 设置交易托管系统服务的地址，可以注册多个地址备用
	/*printf("请输入交易服务器地址..(例如：tcp://180.166.11.40:41205,必须输对，否则无法连接，报错)\n");
	char buffer[40];
    cin>>buffer;
	char *add_td=buffer;*/
	m_pTdApi->RegisterFront("tcp://180.166.11.40:41205");

	// 使客户端开始与后台服务建立连接
	m_pTdApi->Init();
}

void CZQTD::OnFrontConnected()
{
	CZQThostFtdcReqUserLoginField reqUserLogin;
	memset(&reqUserLogin,0,sizeof(reqUserLogin));

	strcpy(reqUserLogin.BrokerID,"2011");//此处2011不要改

	strcpy(reqUserLogin.UserID,"20000003");

	strcpy(reqUserLogin.Password,"112233");
	int login=m_pTdApi->ReqUserLogin(&reqUserLogin, 1);
	printf("登录请求完毕,返回值%d\n",login); 
}
void CZQTD::OnRspUserLogin(CZQThostFtdcRspUserLoginField *pRspUserLogin, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	printf("登录请求响应：pRspInfo->ErrorID:%d,pRspInfo->ErrorMsg:%s\n",pRspInfo->ErrorID,pRspInfo->ErrorMsg);	
	//报单功能代码
	//CZQThostFtdcInputOrderField pInputOrder;
	//memset(&pInputOrder,0,sizeof(pInputOrder));
	//CZQThostFtdcInputOrderField * pIptOrdFld=&pInputOrder;
	////经纪公司代码
	//strcpy_s(pIptOrdFld->BrokerID,"2011");
	////投资者代码
	//strcpy_s(pIptOrdFld->InvestorID,"20000002");
	////合约代码
	//strcpy_s(pIptOrdFld->InstrumentID,"601857");
	////交易所代码  
	//strcpy_s(pIptOrdFld->ExchangeID,"SSE");//SSE上交所 SZE深交所
	////报单价格条件  
	//pIptOrdFld->OrderPriceType=THOST_FTDC_OPT_LimitPrice;
	////买卖方向
	//pIptOrdFld->Direction=THOST_FTDC_D_Buy;
	////数量
	//pIptOrdFld->VolumeTotalOriginal=20000;
	////有效期类型
	//pIptOrdFld->TimeCondition = THOST_FTDC_TC_GFD;
	////成交量类型
	//pIptOrdFld->VolumeCondition=THOST_FTDC_VC_AV;
	////触发条件
	//pIptOrdFld->ContingentCondition = THOST_FTDC_CC_Immediately;
	////强平原因
	//pIptOrdFld->ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	////价格
	//strcpy_s(pIptOrdFld->LimitPrice,"9.89");
	//pIptOrdFld->CombOffsetFlag[0]=THOST_FTDC_OF_Open;
	//pIptOrdFld->CombHedgeFlag[0]=THOST_FTDC_HF_Speculation;
	//strcpy_s(pIptOrdFld->UserID,"20000002");
	//pIptOrdFld->IsAutoSuspend = 0;
	//pIptOrdFld->UserForceClose = 0;
	//int resCode=m_pTdApi->ReqOrderInsert(pIptOrdFld, ++nRequestID);
	//printf("ReqOrderInsert:<-返回值->%d\n",resCode);	
	
	//查持仓
	//CZQThostFtdcQryInvestorPositionField pField;
	//memset(&pField,0,sizeof(pField));
	//CZQThostFtdcQryInvestorPositionField *pFld=&pField;
	//strcpy_s(pFld->BrokerID,"2011");
	//strcpy_s(pFld->InvestorID,"20000002");
	////strcpy_s(pFld->InstrumentID,"600002");//填写此项标明查得具体某个证券的持仓信息，不填表示查所有持仓
	//int lo=m_pTdApi->ReqQryInvestorPosition(pFld,nRequestID);
	}

void CZQTD::OnRspQryInvestorPosition(CZQThostFtdcInvestorPositionField *pInvestorPosition, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	//printf("BrokerID经纪公司:%s  InstrumentID证券代码:%s  InvestorID投资者:%s  Position今日持仓:%d  Position上日持仓:%d  OpenVolume开仓量:%d   Commission手续费:%f\n",pInvestorPosition->BrokerID,pInvestorPosition->InstrumentID,pInvestorPosition->InvestorID,pInvestorPosition->Position,pInvestorPosition->YdPosition,pInvestorPosition->OpenVolume,pInvestorPosition->Commission);
	return;
}
void CZQTD::OnRspQryInstrument(CZQThostFtdcInstrumentField *pInstrument, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pInstrument!=NULL)
		printf("Instrument:%s;Name:%s!\n",pInstrument->InstrumentID,pInstrument->InstrumentName);
}
void CZQTD::OnRspOrderInsert(CZQThostFtdcInputOrderField *pInputOrder2, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
 {
	printf("ReqOrderAction---pRspInfo->ErrorID:%d,pRspInfo->ErrorMsg:%s\n",pRspInfo->ErrorID,pRspInfo->ErrorMsg);
	return;
}
void CZQTD::OnRspQryOrder(CZQThostFtdcOrderField *pOrder, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
 {
	 if(pOrder!=NULL){
     CZQThostFtdcInputOrderActionField pInputOrder;
	 memset(&pInputOrder,0,sizeof(pInputOrder));
	 CZQThostFtdcInputOrderActionField *pIptOrdFld=&pInputOrder;
	 /*撤单方式一，同一个session*/
	 //pIptOrdFld->FrontID=pOrder->FrontID;
	 //pIptOrdFld->SessionID=pOrder->SessionID;
	 //strcpy_s(pIptOrdFld->OrderRef,pOrder->OrderRef);
	 //pIptOrdFld->ActionFlag=THOST_FTDC_AF_Delete;

	 /*撤单方式二，不是同一个session*/
	 strcpy(pIptOrdFld->OrderLocalID,pOrder->OrderLocalID);
     strcpy(pIptOrdFld->TraderID,pOrder->TraderID);
	 strcpy(pIptOrdFld->BrokerID,pOrder->BrokerID);
	 strcpy(pIptOrdFld->ExchangeID,pOrder->ExchangeID);
	 pIptOrdFld->ActionFlag=THOST_FTDC_AF_Delete;

	// if(pOrder->OrderStatus=='1'){
	 printf("--查得可撤单数据-!!%s!!%d!!%d\n",pOrder->OrderLocalID,pOrder->OrderStatus,pOrder->LimitPrice);
	 int resCode2=m_pTdApi->ReqOrderAction(pIptOrdFld, ++nRequestID);
	 printf("ReqOrderAction撤单提交:%d,++nRequestID%d\n",resCode2,nRequestID);	
	//}
	 }
	return;
}
void CZQTD::OnRspQryTradingCode(CZQThostFtdcTradingCodeField *pTradingCode, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("InvestorID:%d   ClientID:%d\n",pTradingCode->InvestorID,pTradingCode->ClientID);
	return;
}
void CZQTD::OnRspQryInstrumentCommissionRate(CZQThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	return;
}
void CZQTD::OnRspError(CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("--OnRspError--:%s\n",pRspInfo->ErrorMsg);
	return;
}
void CZQTD::OnErrRtnOrderInsert(CZQThostFtdcInputOrderField *pInputOrder, CZQThostFtdcRspInfoField *pRspInfo)
{
	printf("--OnErrRtnOrderInsert--:%s\n",pRspInfo->ErrorMsg);
	return;
}
void CZQTD::OnErrRtnOrderAction(CZQThostFtdcOrderActionField *pOrderAction, CZQThostFtdcRspInfoField *pRspInfo)
{
	printf("--OnErrRtnOrderAction--:%s--\n",pRspInfo->ErrorMsg);
	return;
}
//Rtn/////////////////////////////////////////////////////////////////////

void CZQTD::OnRtnOrder(CZQThostFtdcOrderField *pOrder)
{
	printf("--pOrder-----%d!!%d!!%s!!%s!!%s\n",pOrder->FrontID,pOrder->SessionID,pOrder->OrderLocalID,pOrder->TraderID,pOrder->StatusMsg);	
	return;
}

void CZQTD::OnRspOrderAction(CZQThostFtdcInputOrderActionField *pInputOrderAction, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("=撤单响应==价格：%s报单引用:%s!!本地报单编号:%s--ErrorMsg：%s\n",pInputOrderAction->LimitPrice,pInputOrderAction->OrderRef,pInputOrderAction->OrderLocalID,pRspInfo->ErrorMsg);
	return;
}

void CZQTD::OnRtnTrade(CZQThostFtdcTradeField *pTrade)
{
	printf("Instrument:%s;Price:%s;dbPrice:%n!\n",pTrade->InstrumentID,pTrade->Price,getPrice(pTrade->Price));
	return;
}
void CZQTD::OnRtnInstrumentStatus(CZQThostFtdcInstrumentStatusField *pInstrumentStatus)
{
	return;
}

double CZQTD::getPrice(TZQThostFtdcStockPriceType price)
{
	return sizeof(price) / sizeof(price[0]);
}
