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


//����һ���˻����е���������������Ķ��˻�ģ�鿪��������������һ��ģ���չ�Բ�ǿ
class AccountManagers
{
public:
	AccountManagers(std::string & accountid);
	~AccountManagers();
	//�˻�id
	std::string _accountid;
	bool _is_connected;//��ǰ�����Ƿ�����

	std::string _front_id;//��ǰ���ӵ�ǰ�û����
	std::string _session_id;//�Ự���


	//�˻��ʽ�
	AccountFieldHKY _account;
	//ί�б�
	std::vector<OrderFieldHKY> _vc_order_detail;
	std::vector<OrderFieldHKY> _vc_order_pending;
	std::map<std::string,OrderFieldHKY> _map_id_order;

	//�ɽ���
	std::vector<TradeFieldHKY> _vc_trade_detail;
	//std::map<QString,TradeFieldHKY> _vc_trade_base;
	//�ֱֲ�
	std::vector<PositionFieldHKY> _vc_position_detail;
	std::map<std::string,PositionFieldHKY> _map_position_base;
	//��Լ��Ϣ�б�
	//std::vector<InstrumentField> _vc_instruments;
	std::map<std::string,InstrumentField> _map_instruments_field;
	std::map<std::string,InstrumentSet> _map_instruments_set;	
	
	//����
	std::map<std::string,TickFieldHKY> _map_last_tick;
	//ֹӯֹ�𶩵�
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


	//��ǰ̨ҵ���ܵĶԽ�
	bool send_order(PlaceOrderField  order);
	bool cancel_order(QString orderID);
	bool cancel_all_order();
	std::vector<PositionFieldHKY> get_position_detail();//�ֲ���ϸ
	std::map<std::string,PositionFieldHKY> get_position_base();//�ֲֻ���
	std::vector<TradeFieldHKY> get_all_trade_detail();//�ɽ���ϸ
	void get_all_trade_base();//�ɽ�����
	std::vector<OrderFieldHKY> get_all_order_detail();//������ϸ
	std::vector<OrderFieldHKY> get_net_order_list();//�ҵ�
	void close_symbol_all(QString &sym);//ȫƽ
	std::map<std::string,InstrumentField> get_all_symbols();//������к�Լ��Ϣ
	TickFieldHKY get_latest_tick();//������µ�����
	AccountFieldHKY get_accountInfo();//��ȡ���µ��ʽ�


	//��ʼ����ʱ��ӷ������������˻���������Ϣ
	void slots_on_account_field(const AccountFieldHKY & account);
	void slots_on_order_field(const OrderFieldHKY & order);
	void slots_on_trade_field(const TradeFieldHKY & trade);
	void slots_on_position_field(const PositionFieldHKY & position);
	void slots_on_login_respose_field(const QStringList  tick);
	bool account_initial;

	//log
	void lg(int &level,std::string &lgstr);

	//ֹ��ֹӯʵ�֡�Ԥ��ʵ��

	std::list<PlaceOrderField> _order_list;
	std::list<SymbolProfitStop> _profit_stop;
	//�����߳�
	void thread_send_order();

	//Ԥ�񵥼���߳�
	void thread_pury_order();

	//���
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