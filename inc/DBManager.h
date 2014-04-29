#ifndef DBMANAGER_H_
#define DBMANAGER_H_

#include <FBase.h>
#include <FIo.h>

using namespace Tizen::Base;
using namespace Tizen::Io;
using namespace Tizen::Base::Collection;

typedef struct __MEMBERINFO : public Tizen::Base::Object
{
	int seq;
	String id;
	String name;
	int age;
}MEMBERINFO;

/**
 * DBManager is a class only for database queries that provide
 * the functions to insert, view, modify and delete simple member information.
 */

class DBManager
{
private:
	static DBManager* __pInstance;
public :
	static DBManager* GetDB()
	{
		if(__pInstance == null)
		{
			__pInstance = new DBManager();
		}

		return __pInstance;
	}
	static void ReleaseDB()
	{
		if(__pInstance)
		{
			delete __pInstance;
			__pInstance = null;
		}
	}
protected:
	DBManager();
	virtual ~DBManager();

private:
	result Open(String dbName);
	void  Init();
	result CreateTable();
	result Close();

public:
	result Execute(String sql);

	bool InsertMember(String id , String name , int age);
	//result Select(String sql,D3INFO& dinfo);

	int GetMaxSeq();
	bool IsMember(String id);
	result GetAllMember(ArrayList& list);
	result GetMemberByID(String id,MEMBERINFO& info);
	result GetMemberByName(String name,MEMBERINFO& info);
	result GetMemberBySeq(int seq,MEMBERINFO& info);

	result UpdateMember(int seq,String id , String name , int age);

	result DeleteMember(int seq);
	result DeleteMemberById(String id);

private:
	Database* __pDatabase;
	String __dbName;

};

inline DBManager* GetDB(){return DBManager::GetDB();}
inline void ReleaseDB(){DBManager::ReleaseDB();}

#endif /* DBMANAGER_H_ */
