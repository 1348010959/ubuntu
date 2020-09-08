#include "MysqlManager.h"
#include <sstream>
#include "../base/AsyncLogging.h"
#include "../base/Singleton.h"
#include "MysqlThrdMgr.h"


CMysqlManager::CMysqlManager(void)
{
	//TODO: m_strCharactSet���Է��ڳ�ʼ���б��г�ʼ��
	m_strCharactSet = "utf8";

	// ��ʼ���� 
	// 1. t_user 
	{
		STableInfo info;
		//name type desc?
		info.m_strName = "t_user";
		info.m_mapField["f_id"] = { "f_id", "bigint(20) NOT NULL AUTO_INCREMENT COMMENT '����ID'", "bigint(20)" };
		info.m_mapField["f_user_id"] = { "f_user_id", "bigint(20) NOT NULL COMMENT '�û�ID'", "bigint(20)" };
		info.m_mapField["f_username"] = { "f_username", "varchar(64) NOT NULL COMMENT '�û���'", "varchar(64)" };
		info.m_mapField["f_nickname"] = { "f_nickname", "varchar(64) NOT NULL COMMENT '�û��ǳ�'", "varchar(64)" };
		info.m_mapField["f_password"] = { "f_password", "varchar(64) NOT NULL COMMENT '�û�����'", "varchar(64)" };
		info.m_mapField["f_facetype"] = { "f_facetype", "int(10) DEFAULT 0 COMMENT '�û�ͷ������'", "int(10)" };
		info.m_mapField["f_customface"] = { "f_customface", "varchar(32) DEFAULT NULL COMMENT '�Զ���ͷ����'", "varchar(32)" };
		info.m_mapField["f_customfacefmt"] = { "f_customfacefmt", "varchar(6) DEFAULT NULL COMMENT '�Զ���ͷ���ʽ'", "varchar(6)" };
		info.m_mapField["f_gender"] = { "f_gender", "int(2)  DEFAULT 0 COMMENT '�Ա�'", "int(2)" };
		info.m_mapField["f_birthday"] = { "f_birthday", "bigint(20)  DEFAULT 19900101 COMMENT '����'", "bigint(20)" };
		info.m_mapField["f_signature"] = { "f_signature", "varchar(256) DEFAULT NULL COMMENT '��ַ'", "varchar(256)" };
		info.m_mapField["f_address"] = { "f_address", "varchar(256) DEFAULT NULL COMMENT '��ַ'", "varchar(256)" };
		info.m_mapField["f_phonenumber"] = { "f_phonenumber", "varchar(64) DEFAULT NULL COMMENT '�绰'", "varchar(64)" };
		info.m_mapField["f_mail"] = { "f_mail", "varchar(256) DEFAULT NULL COMMENT '����'", "varchar(256)" };
		info.m_mapField["f_owner_id"] = { "f_owner_id", "bigint(20) DEFAULT 0 COMMENT 'Ⱥ�˺�Ⱥ��userid'", "bigint(20)" };
		info.m_mapField["f_teaminfo"] = { "f_teaminfo", "blob default null comment '���ѷ�����Ϣ'", "blob" };
		info.m_mapField["f_register_time"] = { "f_register_time", "datetime NOT NULL COMMENT 'ע��ʱ��'", "datetime" };
		info.m_mapField["f_remark"] = { "f_remark", "varchar(64) NULL COMMENT '��ע'", "varchar(64)" };
		info.m_mapField["f_update_time"] = { "f_update_time", "timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '����ʱ��'", "timestamp" };

		info.m_strKeyString = "PRIMARY KEY (f_user_id), INDEX f_user_id (f_user_id), KEY  f_id  ( f_id )";
		m_vecTableInfo.push_back(info);

		//info.m_mapField["AccountID"] = { "AccountID", "integer NOT NULL AUTO_INCREMENT PRIMARY KEY", "int(11)" };
		//info.m_mapField["MobilePhone"] = { "MobilePhone", "bigint NOT NULL", "bigint(20)" };
		//info.m_mapField["NickName"] = { "NickName", "char(65) not null", "char(65)" };
		//info.m_mapField["PassWord"] = { "PassWord", "char(65) not null", "char(65)" };
		////info.m_mapField["Friend"] = { "Friend", "blob not null", "blob" };
  //      info.m_mapField["Friend"] = { "Friend", "blob default null", "blob" };
		//info.m_mapField["PersonInfo"] = { "PersonInfo", "blob default null", "blob" };
		//
		//info.m_strKeyString = "UNIQUE KEY(MobilePhone)";
		//m_vecTableInfo.push_back(info);
	}

	// 2. t_user_relationship 
	{
		STableInfo info;
		//name type desc?
		info.m_strName = "t_user_relationship";
		info.m_mapField["f_id"] = { "f_id", "bigint(20) NOT NULL AUTO_INCREMENT COMMENT '����ID'", "bigint(20)" };
		info.m_mapField["f_user_id1"] = { "f_user_id1", "bigint(20) NOT NULL COMMENT '�û�ID'", "bigint(20)" };
		info.m_mapField["f_user_id2"] = { "f_user_id2", "bigint(20) NOT NULL COMMENT '�û�ID'", "bigint(20)" };
		info.m_mapField["f_user1_teamname"] = { "f_user1_teamname", "VARCHAR(32) NOT NULL DEFAULT 'My Friends' COMMENT '�û�2���û�1�ĺ��ѷ�������'", "VARCHAR(32)" };
		info.m_mapField["f_user2_teamname"] = { "f_user2_teamname", "VARCHAR(32) NOT NULL DEFAULT 'My Friends' COMMENT '�û�1���û�2�ĺ��ѷ�������'", "VARCHAR(32)" };
		info.m_mapField["f_user1_markname"] = { "f_user1_markname", "VARCHAR(32) COMMENT '�û�2���û�1�ı�ע����'", "VARCHAR(32)" },
			info.m_mapField["f_user2_markname"] = { "f_user2_markname", "VARCHAR(32) COMMENT '�û�1���û�2�ı�ע����'", "VARCHAR(32)" },
			info.m_mapField["f_remark"] = { "f_remark", "varchar(64) NULL COMMENT '��ע'", "varchar(64)" };
		info.m_mapField["f_update_time"] = { "f_update_time", "timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '����ʱ��'", "timestamp" };

		info.m_strKeyString = "PRIMARY KEY (f_id), INDEX f_id (f_id)";
		m_vecTableInfo.push_back(info);

		//info.m_mapField["AccountID"] = { "AccountID", "integer NOT NULL AUTO_INCREMENT PRIMARY KEY", "int(11)" };
		//info.m_mapField["MobilePhone"] = { "MobilePhone", "bigint NOT NULL", "bigint(20)" };
		//info.m_mapField["NickName"] = { "NickName", "char(65) not null", "char(65)" };
		//info.m_mapField["PassWord"] = { "PassWord", "char(65) not null", "char(65)" };
		////info.m_mapField["Friend"] = { "Friend", "blob not null", "blob" };
		//      info.m_mapField["Friend"] = { "Friend", "blob default null", "blob" };
		//info.m_mapField["PersonInfo"] = { "PersonInfo", "blob default null", "blob" };
		//
		//info.m_strKeyString = "UNIQUE KEY(MobilePhone)";
		//m_vecTableInfo.push_back(info);
	}

	// 3. t_chatmsg 
	{
		STableInfo chat;
		chat.m_strName = "t_chatmsg";
		chat.m_mapField["f_id"] = { "f_id", "bigint(20) NOT NULL AUTO_INCREMENT COMMENT '����ID'", "bigint(20)" };
		chat.m_mapField["f_senderid"] = { "f_senderid", "bigint(20) NOT NULL COMMENT '������id'", "bigint(20)" };
		chat.m_mapField["f_targetid"] = { "f_targetid", "bigint(20) NOT NULL COMMENT '������id'", "bigint(20)" };
		chat.m_mapField["f_msgcontent"] = { "f_msgcontent", "BLOB NOT NULL COMMENT '��������'", "BLOB" };
		chat.m_mapField["f_create_time"] = { "f_create_time", "timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '����ʱ��'", "timestamp" };
		chat.m_mapField["f_remark"] = { "f_remark", "varchar(64) NULL COMMENT '��ע'", "varchar(64)" };

		chat.m_strKeyString = "PRIMARY KEY (f_id), INDEX f_id (f_id)";
		m_vecTableInfo.push_back(chat);
	}
}

CMysqlManager::~CMysqlManager(void)
{

}

#include <iostream>
bool CMysqlManager::init(const char* host, const char* user, const char* pwd, const char* dbname)
{
	m_strHost = host;
	m_strUser = user;
	//���ݿ��������Ϊ��
	if (pwd != NULL)
		m_strPassword = pwd;
	m_strDataBase = dbname;

	//ע�⣺������ݿ��Ƿ����ʱ����Ҫ�����ݿ���������Ϊ��
	m_poConn.reset(new CDatabaseMysql());
	if (!m_poConn->initialize(m_strHost, m_strUser, m_strPassword, ""))
	{
		//LOG_FATAL << "CMysqlManager::Init failed, please check params(" << m_strHost << ", " << m_strUser << ", " << m_strPassword << ")";
		return false;
	}

	////////////////////// 1. �����Ƿ���� /////////////////////////
	if (!isDBExist())
	{
		if (!createDB())
		{
			return false;
		}
	}

	//�ٴ�ȷ���Ƿ�������������ݿ�
	m_poConn.reset(new CDatabaseMysql());
	if (!m_poConn->initialize(m_strHost, m_strUser, m_strPassword, m_strDataBase))
	{
		//LOG_FATAL << "CMysqlManager::Init failed, please check params(" << m_strHost << ", " << m_strUser
		//	<< ", " << m_strPassword << ", " << m_strDataBase << ")";
		return false;
	}

	////////////////////// 2. �����б��Ƿ���ȷ /////////////////////////
	for (size_t i = 0; i < m_vecTableInfo.size(); i++)
	{
		STableInfo table = m_vecTableInfo[i];
		if (!checkTable(table))
		{
			//LOG_FATAL << "CMysqlManager::Init, table check failed : " << table.m_strName;
			return false;
		}
	}
	////////////////////// 2. �����б��Ƿ���ȷ /////////////////////////

	m_poConn.reset();
	return true;
}

bool CMysqlManager::isDBExist()
{
	if (NULL == m_poConn)
	{
		return false;
	}

	QueryResult* pResult = m_poConn->query("show databases");
	if (NULL == pResult)
	{
		//LOGI << "CMysqlManager::_IsDBExist, no database(" << m_strDataBase << ")";
		return false;
	}

	Field* pRow = pResult->fetch();
	while (pRow != NULL)
	{
		std::string name = pRow[0].getString();
		if (name == m_strDataBase)
		{
			//LOGI << "CMysqlManager::_IsDBExist, find database(" << m_strDataBase << ")";
			pResult->endQuery();
			return true;
		}

		if (pResult->nextRow() == false)
		{
			break;
		}
		pRow = pResult->fetch();
	}

	//LOGI << "CMysqlManager::_IsDBExist, no database(" << m_strDataBase << ")";
	pResult->endQuery();

	delete pResult;
	return false;
}

bool CMysqlManager::createDB()
{
	if (NULL == m_poConn)
	{
		return false;
	}

	uint32_t uAffectedCount = 0;
	int nErrno = 0;

	std::stringstream ss;
	ss << "create database " << m_strDataBase;
	if (m_poConn->execute(ss.str().c_str(), uAffectedCount, nErrno))
	{
		if (uAffectedCount == 1)
		{
			//LOGI << "CMysqlManager::_CreateDB, create database " <<
			//	m_strDataBase << " success";
			return true;
		}
	}
	else
	{
		//LOGE << "CMysqlManager::_CreateDB, create database " << m_strDataBase << " failed("
		//	<< nErrno << ")";
		return false;
	}
	return false;
}

bool CMysqlManager::checkTable(const STableInfo& table)
{
	if (NULL == m_poConn)
	{
		return false;
	}

	if (table.m_strName.find_first_not_of("\t\r\n ") == std::string::npos)
	{
		//LOGW << "CMysqlManager::_CheckTable, tale info not valid";
		return true;
	}

	std::stringstream ss;
	ss << "desc " << table.m_strName;
	QueryResult* pResult = m_poConn->query(ss.str());
	if (NULL == pResult)
	{
		//LOGI << "CMysqlManager::_CheckTable, no table" << table.m_strName << ", begin create.....";
		if (createTable(table))
		{
			//LOGI << "CMysqlManager::_CheckTable, " << table.m_strName << ", end create.....";
			return true;
		}
		return false;
	}
	else // ����ֶ��Ƿ�ƥ�䣬 ��ʱֻ����Ƿ���ڣ� �����һ���������Ƿ���Ҫ�޸� 
	{
		std::map<std::string, std::string> mapOldTable;
		Field* pRow = pResult->fetch();
		while (pRow != NULL)
		{
			std::string name = pRow[0].getString();
			std::string type = pRow[1].getString();
			mapOldTable[name] = type;

			if (pResult->nextRow() == false)
			{
				break;
			}
			pRow = pResult->fetch();
		}

		pResult->endQuery();
		delete pResult;

		for (std::map<std::string, STableField>::const_iterator it = table.m_mapField.begin();
			it != table.m_mapField.end(); ++it)
		{
			STableField field = it->second;
			if (mapOldTable.find(field.m_strName) == mapOldTable.end())
			{
				std::stringstream ss;
				ss << "alter table " << table.m_strName << " add column "
					<< field.m_strName << " " << field.m_strType;

				std::string sql = ss.str();
				if (m_poConn->execute(sql.c_str()))
				{
					//LOGI << sql;
					continue;
				}
				else
				{
					//LOGE << "CMysqlManager::_CheckTable failed : " << sql;
					return false;
				}
			}
		}
	}

	return true;
}

bool CMysqlManager::createTable(const STableInfo& table)
{
	if (table.m_mapField.size() == 0)
	{
		//LOGE << "CMysqlManager::_CreateTable, table info not valid, " << table.m_strName;
		return false;
	}

	std::stringstream ss;
	ss << "CREATE TABLE IF NOT EXISTS " << table.m_strName << " (";

	for (std::map<std::string, STableField>::const_iterator it = table.m_mapField.begin();
		it != table.m_mapField.end(); ++it)
	{
		if (it != table.m_mapField.begin())
		{
			ss << ", ";
		}

		STableField field = it->second;
		ss << field.m_strName << " " << field.m_strType;
	}

	if (table.m_strKeyString != "")
	{
		ss << ", " << table.m_strKeyString;
	}

	ss << ")default charset = utf8, ENGINE = InnoDB;";
	if (m_poConn->execute(ss.str().c_str()))
	{
		return true;
	}

	LOG_ERROR("Create table error, sql: %s", ss.str().c_str());
	return false;
}

