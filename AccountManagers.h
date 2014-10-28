#ifndef ACCOUNT_Managers_H
#define ACCOUNT_Managers_H

#include <string>
#include <list>
#include <map>
#include <QVector>
#include <QStringList>
#include <QString>
#include <QObject>
#include <cm.h>
#include <QMap>
#include <QJsonObject>
#include <QList>
#include "uiinterface.h"
#include "cm.h"
#include "global.h"
#include "symbolparse.h"
#include "exchangedata.h"
#include "varietydata.h"
#include "symboldata.h"
#include "CustomSymbolGroup.h"

using namespace std;

class ExchangeData;
class VarietyData;

struct RiskSet 
{
	QString accountID;
	QString symbol;
	QString avaible;

};



class IdManagers
{
public:
	IdManagers();
	~IdManagers();
	std::string create_new_order_id();
	std::string create_new_trade_id();
	std::string get_order_id(OrderFieldHKY &order);
	std::string get_trade_id(TradeFieldHKY &trade);
};


//定义一个账户所有的特征，方便后续的多账户模块开发，现在是揉在一起的，扩展性不强
class AccountManagers
{
public:
	AccountManagers(std::string & accountid);
	~AccountManagers();
	//账户id
	std::string _accountid;
	bool _is_connected;//当前连接是否正常

	std::string _front_id;//当前连接的前置机编号
	std::string _session_id;//会话编号


	//账户资金
	AccountFieldHKY _account;
	//委托表
	std::vector<OrderFieldHKY> _vc_order_detail;
	std::vector<OrderFieldHKY> _vc_order_pending;
	std::map<std::string,OrderFieldHKY> _map_id_order;

	//成交表
	std::vector<TradeFieldHKY> _vc_trade_detail;
	//std::map<QString,TradeFieldHKY> _vc_trade_base;
	//持仓表
	std::vector<PositionFieldHKY> _vc_position_detail;
	std::map<std::string,PositionFieldHKY> _map_position_base;
	//合约信息列表
	//std::vector<InstrumentField> _vc_instruments;
	std::map<std::string,InstrumentField> _map_instruments_field;
	std::map<std::string,InstrumentSet> _map_instruments_set;	
	
	//行情
	std::map<std::string,TickFieldHKY> _map_last_tick;
	//止盈止损订单
	std::map<std::string,SymbolProfitStop> _map_profit_stop;
	


	void handle_order(OrderFieldHKY & order);
	void handle_trade(TradeFieldHKY & trade);
	void handle_tick(TickFieldHKY & tick);


	void cal_all();
	void reset_paper();
	//////////////////////////////////////////////////////////////////////////order 
	bool check_symbol_is_traded(const std::string & sym);
	bool check_symbol_is_trade_time(const std::string &sym);
	bool check_symbol_is_max(const std::string &sym);

	std::vector<std::string> _vc_can_trade_symbols;
	std::map<std::string,InstrumentSet> _instrument_set;
	void cal_account();

	//////////////////////////////////////////////////////////////////////////get
	void get_tick( const std::string & instrument,TickFieldHKY & tick );
	//////////////////////////////////////////////////////////////////////////
	std::string define_order_status( const int & status );
	//////////////////////////////////////////////////////////////////////////after
	void after();
	InstrumentSet get_instrument_set(const QString &sym);
	InstrumentField get_instrument_field(const QString &sym);
	PositionFieldHKY & get_instrument_position(const QString &sym);


	//与前台业务功能的对接
	bool send_order(PlaceOrderField  order);
	bool cancel_order(QString orderID);
	bool cancel_all_order();
	std::vector<PositionFieldHKY> get_position_detail();//持仓明细
	std::map<std::string,PositionFieldHKY> get_position_base();//持仓汇总
	std::vector<TradeFieldHKY> get_all_trade_detail();//成交明细
	void get_all_trade_base();//成交汇总
	std::vector<OrderFieldHKY> get_all_order_detail();//报单明细
	std::vector<OrderFieldHKY> get_net_order_list();//挂单
	void close_symbol_all(QString &sym);//全平
	std::map<std::string,InstrumentField> get_all_symbols();//获得所有合约信息
	TickFieldHKY get_latest_tick();//获得最新的行情
	AccountFieldHKY get_accountInfo();//获取最新的资金


	//初始化的时候从服务器获得这个账户的所有信息
	void slots_on_account_field(const AccountFieldHKY & account);
	void slots_on_order_field(const OrderFieldHKY & order);
	void slots_on_trade_field(const TradeFieldHKY & trade);
	void slots_on_position_field(const PositionFieldHKY & position);
	void slots_on_login_respose_field(const QStringList  tick);
	bool account_initial;

	//log
	void lg(int &level,std::string &lgstr);

	//止损止盈实现、预埋单实现

	std::list<PlaceOrderField> _order_list;
	std::list<SymbolProfitStop> _profit_stop;
	//订单线程
	void thread_send_order();

	//预埋单监控线程
	void thread_pury_order();

	//风控
	void risk(TickFieldHKY &tick);
};



class MultipleAccounts
{
public:
	MultipleAccounts();
	~MultipleAccounts();

	void add_instrument_set(InstrumentSet &ins_set);
	void del_instrument_set(InstrumentSet &ins_set);
	

	//
	void send_order(PlaceOrderField * order);
	void cancel_order();
	void handle_order(OrderFieldHKY & order);
	void handle_trade(TradeFieldHKY & trade);
	void handle_tick(TickFieldHKY & tick);

private:
	void save_all();
	void load_all();

private:

	std::map<std::string,AccountManagers> _map_account_managers;
	std::vector<std::string> _vc_accountID;

	std::map<std::string,std::vector<std::string>> _map_vc_ac;
	std::map<std::string,AccountManagers> _map_managers;
	void cal_after();

	bool create_paper(AccountManagers * account_manager);
	IdManagers * _id_managers;
};








#endif