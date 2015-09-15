// ZQTD.h: interface for the CZQTD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZQTD_H__3CA7A2E1_A491_47BE_A447_669B265F00B9__INCLUDED_)
#define AFX_ZQTD_H__3CA7A2E1_A491_47BE_A447_669B265F00B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThostFtdcTraderApiSSE.h"

class CZQTD : public CZQThostFtdcTraderSpi  
{
public:
	void Init();
	void OnFrontConnected();
	void OnRspUserLogin(CZQThostFtdcRspUserLoginField *pRspUserLogin, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspQryInstrument(CZQThostFtdcInstrumentField *pInstrument, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspOrderInsert(CZQThostFtdcInputOrderField *pInputOrder, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspQryOrder(CZQThostFtdcOrderField *pOrder, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspQryTradingCode(CZQThostFtdcTradingCodeField *pTradingCode, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspQryInstrumentCommissionRate(CZQThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspOrderAction(CZQThostFtdcInputOrderActionField *pInputOrderAction, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	void OnRspError(CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnErrRtnOrderInsert(CZQThostFtdcInputOrderField *pInputOrder, CZQThostFtdcRspInfoField *pRspInfo);
	void OnErrRtnOrderAction(CZQThostFtdcOrderActionField *pOrderAction, CZQThostFtdcRspInfoField *pRspInfo);
	void OnRspQryInvestorPosition(CZQThostFtdcInvestorPositionField *pInvestorPosition, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	void OnRtnOrder(CZQThostFtdcOrderField *pOrder);
	void OnRtnTrade(CZQThostFtdcTradeField *pTrade);
	void OnRtnInstrumentStatus(CZQThostFtdcInstrumentStatusField *pInstrumentStatus);

	CZQTD();
	virtual ~CZQTD();

private:
	double getPrice(TZQThostFtdcStockPriceType price);
	
	CZQThostFtdcTraderApi* m_pTdApi;
};
#endif // !defined(AFX_ZQTD_H__3CA7A2E1_A491_47BE_A447_669B265F00B9__INCLUDED_)
