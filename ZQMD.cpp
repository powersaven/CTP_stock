// ZQMD.cpp: implementation of the CZQMD class.
//
//////////////////////////////////////////////////////////////////////

#include "util.h"
#include "ZQMD.h"
#include "libmysql/include/mysql.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace std;

const int SZ_MAX = 20000;
const int SZ_NUM = 1718;    // FIXME, when new stocks are open
const int SH_MAX = 20000;
const int SH_NUM = 1062;
char shenzhen_input[SZ_MAX];
char shanghai_input[SH_MAX];
char trading_day[9];

CZQMD::CZQMD()
{

}

CZQMD::~CZQMD()
{
    
}

void CZQMD::Init()
{
    // 产生一个CThostFtdcMdApi实例
    m_pMdApi=CZQThostFtdcMdApi::CreateFtdcMdApi("");
    m_pMdApi->RegisterSpi(this);
    m_pMdApi->RegisterFront("tcp://180.166.11.40:41213");
    m_pMdApi->Init();
}

void CZQMD::OnFrontConnected()
{
    CZQThostFtdcReqUserLoginField reqUserLogin;
    memset(&reqUserLogin,0,sizeof(reqUserLogin));
    strcpy(reqUserLogin.BrokerID,"2011");
    strcpy(reqUserLogin.UserID,"20000003");
    //strcpy_s(reqUserLogin.Password,"112233");
    m_pMdApi->ReqUserLogin(&reqUserLogin,1);

    fprintf(stderr, "CZQMD::OnFrontConnected\n");
}

void CZQMD::OnFrontDisconnected(int nReason)
{
    fprintf(stderr, "CZQMD::OnFrontDisconnected\n");
}

void CZQMD::OnRspUserLogin(CZQThostFtdcRspUserLoginField *pRspUserLogin, 
        CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
    fprintf(stderr, "CZQMD::OnRspUserLogin\n");

    int fdsz, fdsh, nread;

    fdsz = open("./sz", O_RDONLY);
    nread = pread(fdsz, shenzhen_input, SZ_MAX, 0);

    fdsh = open("./sh", O_RDONLY);
    nread = pread(fdsh, shanghai_input, SH_MAX, 0);

    char *sz_stock[SZ_NUM+1];
    char *sh_stock[SH_NUM+1];

    for (int i = 0; i < SZ_NUM; i++) {
        shenzhen_input[i*7+6] = '\0';
        sz_stock[i] = &shenzhen_input[i*7];
    }
 
    for (int i = 0; i < SH_NUM; i++) {
        shanghai_input[i*7+6] = '\0';
        sh_stock[i] = &shanghai_input[i*7];
    }

    //订阅行情
    m_pMdApi->SubscribeMarketData (sh_stock, SH_NUM, "SSE");
    m_pMdApi->SubscribeMarketData (sz_stock, SZ_NUM, "SZE");
}

uint64_t calculate_max(uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5) {
    
    uint64_t ret = 0;

    ret = v1 > ret ? v1 : ret;
    ret = v2 > ret ? v2 : ret;
    ret = v3 > ret ? v3 : ret;
    ret = v4 > ret ? v4 : ret;
    ret = v5 > ret ? v5 : ret;

    return ret;
}

void CZQMD::OnRtnDepthMarketData(CZQThostFtdcDepthMarketDataField *pDepthMarketData) 
{   
    uint64_t BidMoney1, BidMoney2, BidMoney3, BidMoney4, BidMoney5;
    uint64_t AskMoney1, AskMoney2, AskMoney3, AskMoney4, AskMoney5;
    uint64_t MaxBidMoney = 0;
    uint64_t MaxAskMoney = 0;

    char sql[2048];
    int ret = -1;

    if (0 != strcmp(pDepthMarketData->TradingDay, "\0")) {
        strncpy(trading_day, pDepthMarketData->TradingDay, strlen(pDepthMarketData->TradingDay));    
    }

    BidMoney1 = pDepthMarketData->BidPrice1 * pDepthMarketData->BidVolume1;
    BidMoney2 = pDepthMarketData->BidPrice2 * pDepthMarketData->BidVolume2;
    BidMoney3 = pDepthMarketData->BidPrice3 * pDepthMarketData->BidVolume3;
    BidMoney4 = pDepthMarketData->BidPrice4 * pDepthMarketData->BidVolume4;
    BidMoney5 = pDepthMarketData->BidPrice5 * pDepthMarketData->BidVolume5;
    AskMoney1 = pDepthMarketData->AskPrice1 * pDepthMarketData->AskVolume1;
    AskMoney2 = pDepthMarketData->AskPrice2 * pDepthMarketData->AskVolume2;
    AskMoney3 = pDepthMarketData->AskPrice3 * pDepthMarketData->AskVolume3;
    AskMoney4 = pDepthMarketData->AskPrice4 * pDepthMarketData->AskVolume4;
    AskMoney5 = pDepthMarketData->AskPrice5 * pDepthMarketData->AskVolume5;
   
    MaxBidMoney = calculate_max(BidMoney1, BidMoney2, BidMoney3, BidMoney4, BidMoney5);
    MaxAskMoney = calculate_max(AskMoney1, AskMoney2, AskMoney3, AskMoney4, AskMoney5);

    char *sql_offset = sql;
    sql_offset += sprintf(sql, "insert into ctp ("
        "TradingDay, "
        "InstrumentID, "
        "ExchangeID, "
        "LastPrice, "
        "PreClosePrice, "
        "PreOpenInterest, "
        "OpenPrice, "
        "HighestPrice, "
        "LowestPrice, "
        "Volume, "
        "ClosePrice, "
        "UpdateTime, "
        "AveragePrice, "
        "BidPrice1, "
        "BidVolume1, "
        "AskPrice1, "
        "AskVolume1, "
        "BidPrice2, "
        "BidVolume2, "
        "AskPrice2, "
        "AskVolume2, "
        "BidPrice3, "
        "BidVolume3, "
        "AskPrice3, "
        "AskVolume3, "
        "BidPrice4, "
        "BidVolume4, "
        "AskPrice4, "
        "AskVolume4, "
        "BidPrice5, "
        "BidVolume5, "
        "AskPrice5, "
        "AskVolume5, "
        "BidMoney1, "
        "BidMoney2, "
        "BidMoney3, "
        "BidMoney4, "
        "BidMoney5, "
        "AskMoney1, "
        "AskMoney2, "
        "AskMoney3, "
        "AskMoney4, "
        "AskMoney5, "
        "MaxBidMoney, "
        "MaxAskMoney) values "
        "('%s', '%s', '%s', %f, %f, %f, %f, %f, %f, %d, %f, '%s', %f, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %llu, %llu, %llu, %llu, %llu, %llu, %llu, %llu, %llu, %llu, %llu, %llu)", 
        trading_day, 
        pDepthMarketData->InstrumentID, 
        pDepthMarketData->ExchangeID, 
        pDepthMarketData->LastPrice, 
        pDepthMarketData->PreClosePrice, 
        pDepthMarketData->PreOpenInterest, 
        pDepthMarketData->OpenPrice, 
        pDepthMarketData->HighestPrice, 
        pDepthMarketData->LowestPrice, 
        pDepthMarketData->Volume, 
        pDepthMarketData->ClosePrice, 
        pDepthMarketData->UpdateTime, 
        pDepthMarketData->AveragePrice, 
        pDepthMarketData->BidPrice1, 
        pDepthMarketData->BidVolume1, 
        pDepthMarketData->AskPrice1, 
        pDepthMarketData->AskVolume1, 
        pDepthMarketData->BidPrice2, 
        pDepthMarketData->BidVolume2, 
        pDepthMarketData->AskPrice2, 
        pDepthMarketData->AskVolume2, 
        pDepthMarketData->BidPrice3, 
        pDepthMarketData->BidVolume3, 
        pDepthMarketData->AskPrice3, 
        pDepthMarketData->AskVolume3, 
        pDepthMarketData->BidPrice4, 
        pDepthMarketData->BidVolume4, 
        pDepthMarketData->AskPrice4, 
        pDepthMarketData->AskVolume4, 
        pDepthMarketData->BidPrice5, 
        pDepthMarketData->BidVolume5, 
        pDepthMarketData->AskPrice5, 
        pDepthMarketData->AskVolume5,
        BidMoney1, 
        BidMoney2, 
        BidMoney3, 
        BidMoney4, 
        BidMoney5, 
        AskMoney1, 
        AskMoney2, 
        AskMoney3, 
        AskMoney4, 
        AskMoney5, 
        MaxBidMoney, 
        MaxAskMoney); 
    *sql_offset = '\0';
    
    ret = mysql_real_query(mysql, sql, strlen(sql));
 
    if (0 != ret) {
        fprintf(stderr, "mysql query error : %s\n", mysql_error(mysql));
        exit(1);
    }
    
}
