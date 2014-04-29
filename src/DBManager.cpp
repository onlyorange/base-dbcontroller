#include <FApp.h>

#include "DBManager.h"

using namespace Tizen::App;

DBManager* DBManager::__pInstance = null;

DBManager::DBManager()
:__pDatabase(null)
{
	// TODO Auto-generated constructor stub
	Init();
}

DBManager::~DBManager()
{
	Close();
	// TODO Auto-generated destructor stub
}

//open db
result
DBManager::Open(String dbName)
{
	AppLog("Open");
	__dbName = dbName;
	result r = E_SUCCESS;
	__pDatabase = new Database;
	__pDatabase->Construct(__dbName,true);

	return r;
}

//create table
result
DBManager::CreateTable()
{
	String sql;
	result r = E_SUCCESS;
	sql.Append(L"CREATE TABLE IF NOT EXISTS  member(seq INTEGER PRIMARY KEY,id varchar(20),name varchar(20),age INTEGER)");
	r = Execute(sql);
	AppLog("create : %s",GetErrorMessage(r));

	return r;
}

//init class
void
DBManager::Init()
{
	Open(App::GetInstance()->GetAppRootPath() + L"data/mdb.db");
	CreateTable();
}

//delete database
result
DBManager::Close()
{
	AppLog("Close");
	result r = E_SUCCESS;
	if(__pDatabase)
	{
		delete __pDatabase;
		__pDatabase = null;
	}

	return r;
}
///////////////////////////////////////////////////////////////////////////
// Process the sql statement
result
DBManager::Execute(String sql)
{
	AppLog("Execute");
	result r = E_SUCCESS;
	__pDatabase->BeginTransaction();
	r = __pDatabase->ExecuteSql(sql, true);
	__pDatabase->CommitTransaction();

	return r;
}

// Get the max sequence
int
DBManager::GetMaxSeq()
{
	int seq = 0;
	String sql;
	sql.Append(L"SELECT MAX(seq) FROM member");

	result r = E_SUCCESS;
	 // select a query using the Database::QueryN() wrapper AP
	DbEnumerator* pEnum = __pDatabase->QueryN(sql);
	if (pEnum)
	{
		while (pEnum->MoveNext() == E_SUCCESS)
		{
			r = pEnum->GetIntAt(0, seq);
		}
		delete pEnum;
	}

	return seq;
}

// Check if a certain id is in the db
bool
DBManager::IsMember(String id)
{
	bool isMember = false;
	String sql;
	sql.Format(100,L"SELECT id FROM member where id = '%ls'",id.GetPointer());

	 // select a query using the Database::QueryN() wrapper AP
	DbEnumerator* pEnum = __pDatabase->QueryN(sql);
	if (pEnum)
	{
		while (pEnum->MoveNext() == E_SUCCESS)
		{
			isMember = true;
		}
		delete pEnum;
	}

	return isMember;
}

// Insert a member
bool
DBManager::InsertMember(String id , String name , int age )
{

	result r = E_SUCCESS;

	int seq = GetMaxSeq();
	bool isMember = IsMember(id);
	if(isMember) return false;

	String sql;
	//INSERT INTO MySchedule ( NUM, D_DAY, S_TITLE, S_DETAIL ) VALUES ( %d, '%04d-%02d-%02d %02d:%02d:%02d', '%ls', '%ls' )
	sql.Format(100,L"insert into member values( %d , '%ls' , '%ls' , %d )" ,
			seq + 1,id.GetPointer(),name.GetPointer(),age);
	AppLog("seq : %d sql : %ls",seq, sql.GetPointer());

	r = Execute(sql);
	AppLog("insert : %s",GetErrorMessage(r));

	return true;
}

// Get all members
result
DBManager::GetAllMember(ArrayList& list)
{
	AppLog("GetAllMember");
	result r = E_SUCCESS;

	String sql;
	sql.Format(50,L"select seq, id, name,age from member");

	MEMBERINFO* pInfo = null;
	 // select a query using the Database::QueryN() wrapper AP
	DbEnumerator* pEnum = __pDatabase->QueryN(sql);
	if (pEnum)
	{
		while (pEnum->MoveNext() == E_SUCCESS)
		{
			pInfo = new MEMBERINFO();
			r = pEnum->GetIntAt(0, pInfo->seq);
			r = pEnum->GetStringAt(1,pInfo->id);
			r = pEnum->GetStringAt(2,pInfo->name);
			r = pEnum->GetIntAt(3,pInfo->age);


			list.Add(*pInfo);
		}
		delete pEnum;
	}

	return r;
}

// Get a member by id
result
DBManager::GetMemberByID(String id,MEMBERINFO& info)
{
	result r = E_SUCCESS;

	String sql;
	sql.Format(50,L"select seq, id, name,age from member where id = '%ls'"
			,id.GetPointer());

	 // select a query using the Database::QueryN() wrapper AP
	DbEnumerator* pEnum = __pDatabase->QueryN(sql);
	if (pEnum)
	{
		while (pEnum->MoveNext() == E_SUCCESS)
		{
			r = pEnum->GetIntAt(0, info.seq);
			r = pEnum->GetStringAt(1,info.id);
			r = pEnum->GetStringAt(2,info.name);
			r = pEnum->GetIntAt(3,info.age);

		}
		delete pEnum;
	}

	return r;
}

// Get a member by name
result
DBManager::GetMemberByName(String name,MEMBERINFO& info)
{
	AppLog("GetAllMember");
	result r = E_SUCCESS;

	String sql;
	sql.Format(100,L"select seq, id, name,age from member where name = '%ls'"
			,name.GetPointer());

	 // select a query using the Database::QueryN() wrapper AP
	DbEnumerator* pEnum = __pDatabase->QueryN(sql);
	if (pEnum)
	{
		while (pEnum->MoveNext() == E_SUCCESS)
		{
			r = pEnum->GetIntAt(0, info.seq);
			r = pEnum->GetStringAt(1,info.id);
			r = pEnum->GetStringAt(2,info.name);
			r = pEnum->GetIntAt(3,info.age);
		}
		delete pEnum;
	}

	return r;


}

// Get a member by seq
result
DBManager::GetMemberBySeq(int seq,MEMBERINFO& info)
{
	AppLog("GetAllMember");
	result r = E_SUCCESS;

	String sql;
	sql.Format(100,L"select seq, id, name,age from member where seq = %d",seq);

	// select a query using the Database::QueryN() wrapper AP
	DbEnumerator* pEnum = __pDatabase->QueryN(sql);
	if (pEnum)
	{
		while (pEnum->MoveNext() == E_SUCCESS)
		{
			r = pEnum->GetIntAt(0, info.seq);
			r = pEnum->GetStringAt(1,info.id);
			r = pEnum->GetStringAt(2,info.name);
			r = pEnum->GetIntAt(3,info.age);
		}
		delete pEnum;
	}

	return r;
}


// Update a member
result
DBManager::UpdateMember(int seq,String id , String name , int age)
{
	String sql;
	sql.Format(100,L"update member set id='%ls', name='%ls', age=%d where seq =%d",
			id.GetPointer(),name.GetPointer(),age,seq);

	return Execute(sql);
}


// Delete a member
result
DBManager::DeleteMember(int seq)
{
	String sql;
	sql.Format(100,L"delete from member where seq = %d",seq);

	return Execute(sql);
}

// Delete a member by id
result
DBManager::DeleteMemberById(String id)
{
	String sql;
	sql.Format(100,L"delete from member where id = '%ls'",id.GetPointer());

	return Execute(sql);
}
