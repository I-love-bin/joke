#include<iostream>
#include<string>
#include<vector>
#include<regex>
#include<mariadb/mysql.h>

using namespace std;

//Inesrt error message, unfixed code and fixed code to MySQL.
//query:
//insert into MessageAndCode select * from (select 
//	'medp',
//	'huga',
//	'ahoge'
//) as tmp where not exists( select * from MessageAndCode where Message like 
//	'm%p'
//) limit 1;
//
//option:
//	-I/usr/include/mariadb -L/usr/lib/x86_64-linux-gnu/ -lmariadbclient
//	*read 
void insertToDataBase( vector<vector<string>> dead )
{
	MYSQL *connection=NULL;
	MYSQL_ROW row;
	string sqlServer="localhost";
	string userName="ErrorCollector";
	string passWord="ecnerd";
	string dataBase="compile_error";
	string queryOne="insert into MessageAndCode select * from (select '";
	string queryTwo="') as tmp where not exists (select * from MessageAndCode where Message like '";
	string queryThree="') limit 1;";

	connection=mysql_init(NULL);
	if( !mysql_real_connect(connection,sqlServer.c_str(),userName.c_str(),passWord.c_str(),dataBase.c_str(),0,NULL,0) )
	{
		cout<<"SQL server connection failed."<<endl;
		exit(EXIT_FAILURE);
	}
	for( int i=0 ; i<dead.size() ; i++ )
	{
		queryOne+=(dead[i][0]+"','"+dead[i][1]+"','"+dead[i][2]+queryTwo+regex_replace( dead[i][0] , regex("‘.+?’") , "‘%’" )+queryThree+"\0");
		if( mysql_query(connection,queryOne.c_str()) )
		{
			cout<<"SQL query failed."<<endl;
			cout<<mysql_error(connection)<<endl;
			mysql_close(connection);
			exit(EXIT_FAILURE);
		}
		queryOne="insert into MessageAndCode select * from (select '";
	}
	mysql_close(connection);
}
